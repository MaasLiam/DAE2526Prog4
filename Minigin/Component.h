#pragma once

namespace dae
{
	class GameObject;

	class Component
	{
	public:
		explicit Component(GameObject* owner);
		virtual ~Component() = default;

		Component(const Component&) = delete;
		Component(Component&&) = delete;
		Component& operator=(const Component&) = delete;
		Component& operator=(Component&&) = delete;

		GameObject* GetOwner() const;

		virtual void Update(float deltaTime);
		virtual void Render() const;

	private:
		GameObject* m_Owner{};
	};
}