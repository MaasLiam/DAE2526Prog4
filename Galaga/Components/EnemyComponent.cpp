#include "EnemyComponent.h"

#include "GameObject.h"
#include "TransformComponent.h"

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

	std::unique_ptr<EnemyState> Update(EnemyComponent& enemy, float deltaTime) override
	{
		auto* transform = enemy.GetOwner()->GetComponent<dae::TransformComponent>();
		if (!transform)
		{
			return nullptr;
		}

		auto position = transform->GetLocalPosition();
		const auto target = enemy.GetFormationPosition();

		const glm::vec3 direction = target - position;
		const float distance = glm::length(direction);

		if (distance < 2.f)
		{
			transform->SetLocalPosition(target);
			return std::make_unique<InFormationEnemyState>();
		}

		const glm::vec3 normalizedDirection = direction / distance;
		position += normalizedDirection * 120.f * deltaTime;

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
	ChangeState(std::make_unique<FlyingIntoFormationEnemyState>());
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