#include "EnemyComponent.h"

#include "GameObject.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "ServiceLocator.h"
#include "SoundIds.h"
#include "GameplayConstants.h"

#include <utility>
#include <vector>

namespace
{
	std::vector<glm::vec3> CreateBeeEntryPath(const glm::vec3& start, const glm::vec3& target)
	{
		const bool entersFromLeft = start.x < target.x;
		const float sideX = entersFromLeft ? 80.f : 720.f;
		const float curveOffset = entersFromLeft ? -70.f : 70.f;

		return
		{
			glm::vec3{ sideX, 70.f, 0.f },
			glm::vec3{ target.x + curveOffset, 170.f, 0.f },
			glm::vec3{ target.x, target.y + 35.f, 0.f },
			target
		};
	}

	std::vector<glm::vec3> CreateButterflyEntryPath(const glm::vec3& start, const glm::vec3& target)
	{
		const bool entersFromLeft = start.x < target.x;
		const float sideX = entersFromLeft ? 60.f : 740.f;
		const float oppositeX = entersFromLeft ? 640.f : 160.f;

		return
		{
			glm::vec3{ sideX, 80.f, 0.f },
			glm::vec3{ oppositeX, 120.f, 0.f },
			glm::vec3{ target.x - 90.f, 190.f, 0.f },
			glm::vec3{ target.x + 90.f, 150.f, 0.f },
			glm::vec3{ target.x, target.y + 30.f, 0.f },
			target
		};
	}

	std::vector<glm::vec3> CreateBossEntryPath(const glm::vec3& start, const glm::vec3& target)
	{
		const bool entersFromLeft = start.x < target.x;
		const float sideX = entersFromLeft ? 80.f : 720.f;
		const float loopDirection = entersFromLeft ? 1.f : -1.f;

		return
		{
			glm::vec3{ sideX, 70.f, 0.f },
			glm::vec3{ target.x + loopDirection * 120.f, 80.f, 0.f },
			glm::vec3{ target.x + loopDirection * 120.f, 170.f, 0.f },
			glm::vec3{ target.x - loopDirection * 60.f, 170.f, 0.f },
			glm::vec3{ target.x - loopDirection * 60.f, 95.f, 0.f },
			target
		};
	}

	std::vector<glm::vec3> CreateEntryPath(
		const glm::vec3& start,
		const glm::vec3& target,
		galaga::EnemyType type
	)
	{
		switch (type)
		{
		case galaga::EnemyType::Bee:
			return CreateBeeEntryPath(start, target);

		case galaga::EnemyType::Butterfly:
			return CreateButterflyEntryPath(start, target);

		case galaga::EnemyType::BossGalaga:
			return CreateBossEntryPath(start, target);
		}

		return { target };
	}
}

class EnemyState
{
public:
	virtual ~EnemyState() = default;

	virtual void OnEnter(galaga::EnemyComponent&) {}
	virtual void OnExit(galaga::EnemyComponent&) {}

	virtual std::unique_ptr<EnemyState> Update(galaga::EnemyComponent&, float)
	{
		return nullptr;
	}

	virtual bool IsInFormationState() const
	{
		return false;
	}

	virtual int GetScoreValue(const galaga::EnemyComponent& enemy) const = 0;
};

class InFormationEnemyState final : public EnemyState
{
public:

	int GetScoreValue(const galaga::EnemyComponent& enemy) const override
	{
		switch (enemy.GetType())
		{
		case galaga::EnemyType::Bee: return 50;
		case galaga::EnemyType::Butterfly: return 80;
		case galaga::EnemyType::BossGalaga: return 150;
		}

		return 0;
	}

	bool IsInFormationState() const override
	{
		return true;
	}
};

class DivingEnemyState final : public EnemyState
{
public:

	std::unique_ptr<EnemyState> Update(galaga::EnemyComponent& enemy, float deltaTime) override
	{
		auto* transform = enemy.GetOwner()->GetComponent<dae::TransformComponent>();
		if (!transform)
			return nullptr;

		auto position = transform->GetLocalPosition();

		const float diveSpeed = enemy.GetType() == galaga::EnemyType::Butterfly ? 90.f : 70.f;
		position.y += diveSpeed * deltaTime;

		if (enemy.GetType() == galaga::EnemyType::Butterfly)
			position.x += m_Direction * 50.f * deltaTime;

		m_WiggleTimer += deltaTime;
		if (m_WiggleTimer >= 0.45f)
		{
			m_WiggleTimer = 0.f;
			m_Direction *= -1.f;
		}

		transform->SetLocalPosition(position);

		if (position.y > 520.f)
		{
			transform->SetLocalPosition(enemy.GetFormationPosition());
			return std::make_unique<InFormationEnemyState>();
		}

		return nullptr;
	}

	int GetScoreValue(const galaga::EnemyComponent& enemy) const override
	{
		switch (enemy.GetType())
		{
		case galaga::EnemyType::Bee: return 100;
		case galaga::EnemyType::Butterfly: return 160;
		case galaga::EnemyType::BossGalaga: return 400;
		}

		return 0;
	}

private:
	float m_WiggleTimer{};
	float m_Direction{ 1.f };
};

class TractorBeamEnemyState final : public EnemyState
{
public:
	void OnEnter(galaga::EnemyComponent& enemy) override
	{
		enemy.SetTractorBeamActive(false);

		auto* transform = enemy.GetOwner()->GetComponent<dae::TransformComponent>();
		if (!transform)
		{
			return;
		}

		const auto currentPosition = transform->GetLocalPosition();
		m_BeamPosition = glm::vec3{ currentPosition.x, galaga::gameplay::TractorBeamY, 0.f };
	}

	void OnExit(galaga::EnemyComponent& enemy) override
	{
		enemy.SetTractorBeamActive(false);
	}

	std::unique_ptr<EnemyState> Update(galaga::EnemyComponent& enemy, float deltaTime) override
	{
		auto* transform = enemy.GetOwner()->GetComponent<dae::TransformComponent>();
		if (!transform)
		{
			return nullptr;
		}

		switch (m_Phase)
		{
		case Phase::MovingToBeamPosition:
			MoveTowards(*transform, m_BeamPosition, 180.f, deltaTime);

			if (IsNear(*transform, m_BeamPosition, 8.f))
			{
				transform->SetLocalPosition(m_BeamPosition);
				enemy.SetTractorBeamActive(true);
				m_Timer = 0.f;
				m_Phase = Phase::BeamActive;

				dae::ServiceLocator::GetSoundSystem().Play(galaga::ToSoundId(galaga::SoundIds::TractorBeam), 1.0f);
			}

			break;

		case Phase::BeamActive:
			m_Timer += deltaTime;

			if (m_Timer >= 2.5f)
			{
				enemy.SetTractorBeamActive(false);
				m_Phase = Phase::Returning;
			}

			break;

		case Phase::Returning:
			MoveTowards(*transform, enemy.GetFormationPosition(), 180.f, deltaTime);

			if (IsNear(*transform, enemy.GetFormationPosition(), 8.f))
			{
				transform->SetLocalPosition(enemy.GetFormationPosition());
				return std::make_unique<InFormationEnemyState>();
			}

			break;
		}

		return nullptr;
	}

	int GetScoreValue(const galaga::EnemyComponent&) const override
	{
		return 400;
	}

private:
	enum class Phase
	{
		MovingToBeamPosition,
		BeamActive,
		Returning
	};

	static void MoveTowards(dae::TransformComponent& transform, const glm::vec3& target, float speed, float deltaTime)
	{
		const auto position = transform.GetLocalPosition();
		const auto difference = target - position;
		const float distance = glm::length(difference);

		if (distance <= 0.001f)
		{
			return;
		}

		const auto direction = difference / distance;
		const auto movement = direction * speed * deltaTime;

		if (glm::length(movement) >= distance)
		{
			transform.SetLocalPosition(target);
			return;
		}

		transform.SetLocalPosition(position + movement);
	}

	static bool IsNear(const dae::TransformComponent& transform, const glm::vec3& target, float distance)
	{
		return glm::length(transform.GetLocalPosition() - target) <= distance;
	}

	Phase m_Phase{ Phase::MovingToBeamPosition };
	glm::vec3 m_BeamPosition{};
	float m_Timer{};
};

class DeadEnemyState final : public EnemyState
{
public:

	int GetScoreValue(const galaga::EnemyComponent&) const override
	{
		return 0;
	}
};

class FlyingIntoFormationEnemyState final : public EnemyState
{
public:
	explicit FlyingIntoFormationEnemyState(std::vector<glm::vec3> waypoints)
		: m_Waypoints(std::move(waypoints))
	{}

	std::unique_ptr<EnemyState> Update(galaga::EnemyComponent& enemy, float deltaTime) override
	{
		auto* transform = enemy.GetOwner()->GetComponent<dae::TransformComponent>();
		if (!transform)
		{
			return nullptr;
		}

		if (m_CurrentWaypoint >= m_Waypoints.size())
		{
			transform->SetLocalPosition(enemy.GetFormationPosition());
			return std::make_unique<InFormationEnemyState>();
		}

		auto position = transform->GetLocalPosition();
		const auto target = m_Waypoints[m_CurrentWaypoint];

		const glm::vec3 direction = target - position;
		const float distance = glm::length(direction);

		if (distance < 4.f)
		{
			transform->SetLocalPosition(target);
			++m_CurrentWaypoint;

			if (m_CurrentWaypoint >= m_Waypoints.size())
			{
				transform->SetLocalPosition(enemy.GetFormationPosition());
				return std::make_unique<InFormationEnemyState>();
			}

			return nullptr;
		}

		const glm::vec3 normalizedDirection = direction / distance;
		const float speed = GetEntrySpeed(enemy.GetType());
		const glm::vec3 movement = normalizedDirection * speed * deltaTime;

		if (glm::length(movement) >= distance)
		{
			transform->SetLocalPosition(target);
			++m_CurrentWaypoint;
			return nullptr;
		}

		position += movement;
		transform->SetLocalPosition(position);

		return nullptr;
	}

	int GetScoreValue(const galaga::EnemyComponent& enemy) const override
	{
		switch (enemy.GetType())
		{
		case galaga::EnemyType::Bee:
			return 50;

		case galaga::EnemyType::Butterfly:
			return 80;

		case galaga::EnemyType::BossGalaga:
			return 150;
		}

		return 0;
	}

private:
	static float GetEntrySpeed(galaga::EnemyType type)
	{
		switch (type)
		{
		case galaga::EnemyType::Bee:
			return 170.f;

		case galaga::EnemyType::Butterfly:
			return 190.f;

		case galaga::EnemyType::BossGalaga:
			return 150.f;
		}

		return 170.f;
	}

	std::vector<glm::vec3> m_Waypoints{};
	size_t m_CurrentWaypoint{};
};

galaga::EnemyComponent::EnemyComponent(dae::GameObject* owner, galaga::EnemyType type)
	: dae::Component(owner)
	, m_Type(type)
{
	if (m_Type == galaga::EnemyType::BossGalaga)
	{
		m_Health = 2;
	}

	ChangeState(std::make_unique<InFormationEnemyState>());
}

galaga::EnemyComponent::~EnemyComponent() = default;

void galaga::EnemyComponent::Update(float deltaTime)
{
	if (!m_State)
	{
		return;
	}

	if (auto newState = m_State->Update(*this, deltaTime))
	{
		ChangeState(std::move(newState));
	}
}

int galaga::EnemyComponent::GetScoreValue() const
{
	return m_State ? m_State->GetScoreValue(*this) : 0;
}

galaga::EnemyType galaga::EnemyComponent::GetType() const
{
	return m_Type;
}

galaga::EnemyStateId galaga::EnemyComponent::GetStateId() const
{
	if (!m_State)
	{
		return galaga::EnemyStateId::Dead;
	}

	if (IsDead())
	{
		return galaga::EnemyStateId::Dead;
	}

	if (dynamic_cast<FlyingIntoFormationEnemyState*>(m_State.get()))
	{
		return galaga::EnemyStateId::FlyingIntoFormation;
	}

	if (dynamic_cast<InFormationEnemyState*>(m_State.get()))
	{
		return galaga::EnemyStateId::InFormation;
	}

	if (dynamic_cast<DivingEnemyState*>(m_State.get()))
	{
		return galaga::EnemyStateId::Diving;
	}

	if (dynamic_cast<TractorBeamEnemyState*>(m_State.get()))
	{
		return galaga::EnemyStateId::TractorBeam;
	}

	return galaga::EnemyStateId::Dead;
}

void galaga::EnemyComponent::StartDiving()
{
	ChangeState(std::make_unique<DivingEnemyState>());

	dae::ServiceLocator::GetSoundSystem().Play(galaga::ToSoundId(galaga::SoundIds::EnemyDive), 1.0f);
}

void galaga::EnemyComponent::StartTractorBeam()
{
	if (m_Type != galaga::EnemyType::BossGalaga)
	{
		return;
	}

	ChangeState(std::make_unique<TractorBeamEnemyState>());

	dae::ServiceLocator::GetSoundSystem().Play(galaga::ToSoundId(galaga::SoundIds::EnemyDive), 1.0f);
}

void galaga::EnemyComponent::ReturnToFormation()
{
	auto* transform = GetOwner()->GetComponent<dae::TransformComponent>();
	if (transform)
	{
		transform->SetLocalPosition(m_FormationPosition);
	}

	ChangeState(std::make_unique<InFormationEnemyState>());
}

void galaga::EnemyComponent::TakeDamage()
{
	if (IsDead())
	{
		return;
	}

	--m_Health;

	if (m_Type == galaga::EnemyType::BossGalaga && m_Health == 1)
	{
		auto* render = GetOwner()->GetComponent<dae::RenderComponent>();
		if (render)
		{
			render->SetTexture("Sprites/BossGalagaDamaged.png");
		}

		return;
	}

	if (m_Health <= 0)
	{
		ChangeState(std::make_unique<DeadEnemyState>());
	}
}

bool galaga::EnemyComponent::IsDead() const
{
	return m_Health <= 0;
}

void galaga::EnemyComponent::ChangeState(std::unique_ptr<EnemyState> newState)
{
	if (m_State)
	{
		m_State->OnExit(*this);
	}

	m_State = std::move(newState);

	m_IsInFormation = m_State && m_State->IsInFormationState();

	if (m_State)
	{
		m_State->OnEnter(*this);
	}
}

void galaga::EnemyComponent::FlyIntoFormation(const glm::vec3& targetPosition)
{
	m_FormationPosition = targetPosition;

	auto* transform = GetOwner()->GetComponent<dae::TransformComponent>();
	if (!transform)
	{
		ChangeState(std::make_unique<FlyingIntoFormationEnemyState>(std::vector<glm::vec3>{ targetPosition }));
		return;
	}

	const auto startPosition = transform->GetLocalPosition();
	auto path = CreateEntryPath(startPosition, targetPosition, m_Type);

	ChangeState(std::make_unique<FlyingIntoFormationEnemyState>(std::move(path)));
}

const glm::vec3& galaga::EnemyComponent::GetFormationPosition() const
{
	return m_FormationPosition;
}

void galaga::EnemyComponent::SetFormationPosition(const glm::vec3& position)
{
	m_FormationPosition = position;
}

bool galaga::EnemyComponent::IsInFormation() const
{
	return m_IsInFormation;
}

bool galaga::EnemyComponent::IsTractorBeamActive() const
{
	return m_IsTractorBeamActive;
}

void galaga::EnemyComponent::SetTractorBeamActive(bool isActive)
{
	m_IsTractorBeamActive = isActive;
}
