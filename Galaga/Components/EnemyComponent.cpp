#include "EnemyComponent.h"

#include "GameObject.h"
#include "TransformComponent.h"
#include "RenderComponent.h"

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
		EnemyType type
	)
	{
		switch (type)
		{
		case EnemyType::Bee:
			return CreateBeeEntryPath(start, target);

		case EnemyType::Butterfly:
			return CreateButterflyEntryPath(start, target);

		case EnemyType::BossGalaga:
			return CreateBossEntryPath(start, target);
		}

		return { target };
	}
}

class EnemyState
{
public:
	virtual ~EnemyState() = default;

	virtual void OnEnter(EnemyComponent&) {}
	virtual void OnExit(EnemyComponent&) {}

	virtual std::unique_ptr<EnemyState> Update(EnemyComponent&, float)
	{
		return nullptr;
	}

	virtual bool IsInFormationState() const
	{
		return false;
	}

	virtual int GetScoreValue(const EnemyComponent& enemy) const = 0;
};

class InFormationEnemyState final : public EnemyState
{
public:

	int GetScoreValue(const EnemyComponent& enemy) const override
	{
		switch (enemy.GetType())
		{
		case EnemyType::Bee: return 50;
		case EnemyType::Butterfly: return 80;
		case EnemyType::BossGalaga: return 150;
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

	std::unique_ptr<EnemyState> Update(EnemyComponent& enemy, float deltaTime) override
	{
		auto* transform = enemy.GetOwner()->GetComponent<dae::TransformComponent>();
		if (!transform)
			return nullptr;

		auto position = transform->GetLocalPosition();

		const float diveSpeed = enemy.GetType() == EnemyType::Butterfly ? 90.f : 70.f;
		position.y += diveSpeed * deltaTime;

		if (enemy.GetType() == EnemyType::Butterfly)
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

	int GetScoreValue(const EnemyComponent& enemy) const override
	{
		switch (enemy.GetType())
		{
		case EnemyType::Bee: return 100;
		case EnemyType::Butterfly: return 160;
		case EnemyType::BossGalaga: return 400;
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

	std::unique_ptr<EnemyState> Update(EnemyComponent& enemy, float deltaTime) override
	{
		auto* transform = enemy.GetOwner()->GetComponent<dae::TransformComponent>();
		if (!transform)
		{
			return nullptr;
		}

		auto position = transform->GetLocalPosition();
		position.y += 35.f * deltaTime;
		transform->SetLocalPosition(position);

		m_Timer += deltaTime;
		if (m_Timer >= 3.f)
		{
			return std::make_unique<InFormationEnemyState>();
		}

		return nullptr;
	}

	int GetScoreValue(const EnemyComponent&) const override
	{
		return 400;
	}

private:
	float m_Timer{};
};

class DeadEnemyState final : public EnemyState
{
public:

	int GetScoreValue(const EnemyComponent&) const override
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

	std::unique_ptr<EnemyState> Update(EnemyComponent& enemy, float deltaTime) override
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

	int GetScoreValue(const EnemyComponent& enemy) const override
	{
		switch (enemy.GetType())
		{
		case EnemyType::Bee:
			return 50;

		case EnemyType::Butterfly:
			return 80;

		case EnemyType::BossGalaga:
			return 150;
		}

		return 0;
	}

private:
	static float GetEntrySpeed(EnemyType type)
	{
		switch (type)
		{
		case EnemyType::Bee:
			return 170.f;

		case EnemyType::Butterfly:
			return 190.f;

		case EnemyType::BossGalaga:
			return 150.f;
		}

		return 170.f;
	}

	std::vector<glm::vec3> m_Waypoints{};
	size_t m_CurrentWaypoint{};
};

EnemyComponent::EnemyComponent(dae::GameObject* owner, EnemyType type)
	: dae::Component(owner)
	, m_Type(type)
{
	if (m_Type == EnemyType::BossGalaga)
	{
		m_Health = 2;
	}

	ChangeState(std::make_unique<InFormationEnemyState>());
}

EnemyComponent::~EnemyComponent() = default;

void EnemyComponent::Update(float deltaTime)
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

int EnemyComponent::GetScoreValue() const
{
	return m_State ? m_State->GetScoreValue(*this) : 0;
}

EnemyType EnemyComponent::GetType() const
{
	return m_Type;
}

EnemyStateId EnemyComponent::GetStateId() const
{
	if (!m_State)
	{
		return EnemyStateId::Dead;
	}

	if (IsDead())
	{
		return EnemyStateId::Dead;
	}

	if (dynamic_cast<FlyingIntoFormationEnemyState*>(m_State.get()))
	{
		return EnemyStateId::FlyingIntoFormation;
	}

	if (dynamic_cast<InFormationEnemyState*>(m_State.get()))
	{
		return EnemyStateId::InFormation;
	}

	if (dynamic_cast<DivingEnemyState*>(m_State.get()))
	{
		return EnemyStateId::Diving;
	}

	if (dynamic_cast<TractorBeamEnemyState*>(m_State.get()))
	{
		return EnemyStateId::TractorBeam;
	}

	return EnemyStateId::Dead;
}

void EnemyComponent::StartDiving()
{
	ChangeState(std::make_unique<DivingEnemyState>());
}

void EnemyComponent::StartTractorBeam()
{
	if (m_Type == EnemyType::BossGalaga)
	{
		ChangeState(std::make_unique<TractorBeamEnemyState>());
	}
}

void EnemyComponent::ReturnToFormation()
{
	auto* transform = GetOwner()->GetComponent<dae::TransformComponent>();
	if (transform)
	{
		transform->SetLocalPosition(m_FormationPosition);
	}

	ChangeState(std::make_unique<InFormationEnemyState>());
}

void EnemyComponent::TakeDamage()
{
	if (IsDead())
	{
		return;
	}

	--m_Health;

	if (m_Type == EnemyType::BossGalaga && m_Health == 1)
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

bool EnemyComponent::IsDead() const
{
	return m_Health <= 0;
}

void EnemyComponent::ChangeState(std::unique_ptr<EnemyState> newState)
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

void EnemyComponent::FlyIntoFormation(const glm::vec3& targetPosition)
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

const glm::vec3& EnemyComponent::GetFormationPosition() const
{
	return m_FormationPosition;
}

void EnemyComponent::SetFormationPosition(const glm::vec3& position)
{
	m_FormationPosition = position;
}

bool EnemyComponent::IsInFormation() const
{
	return m_IsInFormation;
}