#include "EnemyComponent.h"

#include "GameObject.h"
#include "TransformComponent.h"

class EnemyState
{
public:
	virtual ~EnemyState() = default;

	virtual void OnEnter(EnemyComponent&) {}
	virtual void OnExit(EnemyComponent&) {}
	virtual void Update(EnemyComponent&, float) {}

	virtual EnemyStateId GetId() const = 0;
	virtual int GetScoreValue(const EnemyComponent& enemy) const = 0;
};

class InFormationEnemyState final : public EnemyState
{
public:
	EnemyStateId GetId() const override { return EnemyStateId::InFormation; }

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
};

class DivingEnemyState final : public EnemyState
{
public:
	EnemyStateId GetId() const override { return EnemyStateId::Diving; }

	void Update(EnemyComponent& enemy, float deltaTime) override
	{
		auto* transform = enemy.GetOwner()->GetComponent<dae::TransformComponent>();
		if (!transform)
			return;

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
			enemy.ReturnToFormation();
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
	EnemyStateId GetId() const override { return EnemyStateId::TractorBeam; }

	void Update(EnemyComponent& enemy, float deltaTime) override
	{
		auto* transform = enemy.GetOwner()->GetComponent<dae::TransformComponent>();
		if (!transform)
			return;

		auto position = transform->GetLocalPosition();
		position.y += 35.f * deltaTime;
		transform->SetLocalPosition(position);

		m_Timer += deltaTime;
		if (m_Timer >= 3.f)
			enemy.ReturnToFormation();
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
	EnemyStateId GetId() const override { return EnemyStateId::Dead; }

	int GetScoreValue(const EnemyComponent&) const override
	{
		return 0;
	}
};

class FlyingIntoFormationEnemyState final : public EnemyState
{
public:
	EnemyStateId GetId() const override { return EnemyStateId::FlyingIntoFormation; }

	void Update(EnemyComponent& enemy, float deltaTime) override
	{
		auto* transform = enemy.GetOwner()->GetComponent<dae::TransformComponent>();
		if (!transform)
			return;

		auto position = transform->GetLocalPosition();
		const auto target = enemy.GetFormationPosition();

		const glm::vec3 direction = target - position;
		const float distance = glm::length(direction);

		if (distance < 2.f)
		{
			transform->SetLocalPosition(target);
			enemy.ReturnToFormation();
			return;
		}

		const glm::vec3 normalizedDirection = direction / distance;
		position += normalizedDirection * 120.f * deltaTime;

		transform->SetLocalPosition(position);
	}

	int GetScoreValue(const EnemyComponent&) const override
	{
		return 0;
	}
};

EnemyComponent::EnemyComponent(dae::GameObject* owner, EnemyType type)
	: dae::Component(owner)
	, m_Type(type)
{
	if (m_Type == EnemyType::BossGalaga)
		m_Health = 2;

	ChangeState(std::make_unique<InFormationEnemyState>());
}

EnemyComponent::~EnemyComponent() = default;

void EnemyComponent::Update(float deltaTime)
{
	if (m_State)
		m_State->Update(*this, deltaTime);
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
	return m_State ? m_State->GetId() : EnemyStateId::Dead;
}

void EnemyComponent::SetState(EnemyStateId state)
{
	switch (state)
	{
	case EnemyStateId::InFormation:
		ChangeState(std::make_unique<InFormationEnemyState>());
		break;
	case EnemyStateId::Diving:
		ChangeState(std::make_unique<DivingEnemyState>());
		break;
	case EnemyStateId::TractorBeam:
		ChangeState(std::make_unique<TractorBeamEnemyState>());
		break;
	case EnemyStateId::Dead:
		ChangeState(std::make_unique<DeadEnemyState>());
		break;
	case EnemyStateId::FlyingIntoFormation:
		ChangeState(std::make_unique<FlyingIntoFormationEnemyState>());
		break;
	}
}

void EnemyComponent::StartDiving()
{
	SetState(EnemyStateId::Diving);
}

void EnemyComponent::StartTractorBeam()
{
	if (m_Type == EnemyType::BossGalaga)
		SetState(EnemyStateId::TractorBeam);
}

void EnemyComponent::ReturnToFormation()
{
	auto* transform = GetOwner()->GetComponent<dae::TransformComponent>();
	if (transform)
		transform->SetLocalPosition(m_FormationPosition);

	SetState(EnemyStateId::InFormation);
}

void EnemyComponent::TakeDamage()
{
	if (IsDead())
		return;

	--m_Health;

	if (m_Health <= 0)
		SetState(EnemyStateId::Dead);
}

bool EnemyComponent::IsDead() const
{
	return m_Health <= 0 || GetStateId() == EnemyStateId::Dead;
}

void EnemyComponent::ChangeState(std::unique_ptr<EnemyState> newState)
{
	if (m_State)
		m_State->OnExit(*this);

	m_State = std::move(newState);

	if (m_State)
		m_State->OnEnter(*this);
}

void EnemyComponent::FlyIntoFormation(const glm::vec3& targetPosition)
{
	m_FormationPosition = targetPosition;
	SetState(EnemyStateId::FlyingIntoFormation);
}

const glm::vec3& EnemyComponent::GetFormationPosition() const
{
	return m_FormationPosition;
}

void EnemyComponent::SetFormationPosition(const glm::vec3& position)
{
	m_FormationPosition = position;
}