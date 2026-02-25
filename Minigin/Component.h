#pragma once

namespace dae
{
    class GameObject;
    class Component
    {
    public:
        explicit Component(GameObject& parent);
        GameObject& GetOwner() const;
        virtual ~Component() = default;
        Component(const Component&) = delete;
        Component(Component&&) = delete;
        Component& operator=(const Component&) = delete;
        Component& operator=(Component&&) = delete;

        virtual void Update(float) {};
        virtual void Render() const {};

    private:
        GameObject& m_Owner;
    };
}