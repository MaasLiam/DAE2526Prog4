#include "Galaga.h"
#include "SceneManager.h"

#include "TransformComponent.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "Scene.h"
#include "RenderComponent.h"
#include "FPSComponent.h"
#include "RotationComponent.h"

void Galaga::Initialize()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();

	//background
	auto go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TransformComponent>();
	go->AddComponent<dae::RenderComponent>("background.png");
	scene.Add(std::move(go));

	//logo 
	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TransformComponent>();
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(358, 180);
	go->AddComponent<dae::RenderComponent>("logo.png");
	scene.Add(std::move(go));

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	// text
	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TransformComponent>();
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(292, 20);
	go->AddComponent<dae::RenderComponent>();
	go->AddComponent<dae::TextComponent>("Programming 4 Assignment", font, SDL_Color{ 255, 0, 0, 255 });
	scene.Add(std::move(go));

	//fps counter
	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TransformComponent>();
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(10, 10);
	go->AddComponent<dae::RenderComponent>();
	go->AddComponent<dae::TextComponent>("0 FPS", font, SDL_Color{ 255, 0, 0, 255 });
	go->AddComponent<dae::FPSComponent>();
	scene.Add(std::move(go));

	//location of the center of rotation
	auto centerGo = std::make_unique<dae::GameObject>();
	auto* centerPtr = centerGo.get();
	centerGo->AddComponent<dae::TransformComponent>();
	centerGo->GetComponent<dae::TransformComponent>()->SetLocalPosition(250.f, 250.f);

	//first fighter
	auto parentFighterGo = std::make_unique<dae::GameObject>();
	auto* parentPtr = parentFighterGo.get();
	parentFighterGo->AddComponent<dae::TransformComponent>();
	parentFighterGo->AddComponent<dae::RenderComponent>("fighter01.png");
	parentFighterGo->AddComponent<dae::RotationComponent>(20.f, 5.f);
	parentFighterGo->SetParent(centerPtr, false);

	//second fighter
	auto childFighterGo = std::make_unique<dae::GameObject>();
	childFighterGo->AddComponent<dae::TransformComponent>();
	childFighterGo->AddComponent<dae::RenderComponent>("fighter01.png");
	childFighterGo->AddComponent<dae::RotationComponent>(70.f, -3.0f);
	childFighterGo->SetParent(parentPtr, false);

	scene.Add(std::move(centerGo));
	scene.Add(std::move(parentFighterGo));
	scene.Add(std::move(childFighterGo));

}
