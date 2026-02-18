#pragma once

namespace dae
{
    class GameObject;
    class Component
    {
    public:
        explicit Component(GameObject* parent);
        virtual ~Component() = default;

        virtual void Update(float deltaTime);
        virtual void Render() const;

    protected:
        GameObject* m_Parent{};
    };
}