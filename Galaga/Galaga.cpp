#include "Galaga.h"
#include "SceneManager.h"

#include "TransformComponent.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "Scene.h"
#include "RenderComponent.h"
#include "FPSComponent.h"
#include "InputManager.h"
#include "MoveObjectCommand.h"
#include "HealthComponent.h"
#include "DisplayLivesComponent.h"
#include "DisplayScoreComponent.h"
#include "ShootCommand.h"
#include "EnemyFormationControllerComponent.h"

#include "KeyboardInput.h"
#include "ControllerInput.h"
#include "InputTypes.h"
#include "ScoreComponent.h"
#include "SteamAchievements.h"
#include "SteamAchievementObserver.h"

#include "CollisionComponent.h"
#include "EnemyComponent.h"


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

	////location of the center of rotation
	//auto centerGo = std::make_unique<dae::GameObject>();
	//auto* centerPtr = centerGo.get();
	//centerGo->AddComponent<dae::TransformComponent>();
	//centerGo->GetComponent<dae::TransformComponent>()->SetLocalPosition(250.f, 250.f);

	////first fighter
	//auto parentFighterGo = std::make_unique<dae::GameObject>();
	//auto* parentPtr = parentFighterGo.get();
	//parentFighterGo->AddComponent<dae::TransformComponent>();
	//parentFighterGo->AddComponent<dae::RenderComponent>("fighter01.png");
	//parentFighterGo->AddComponent<dae::RotationComponent>(20.f, 5.f);
	//parentFighterGo->SetParent(centerPtr, false);

	////second fighter
	//auto childFighterGo = std::make_unique<dae::GameObject>();
	//childFighterGo->AddComponent<dae::TransformComponent>();
	//childFighterGo->AddComponent<dae::RenderComponent>("fighter01.png");
	//childFighterGo->AddComponent<dae::RotationComponent>(70.f, -3.0f);
	//childFighterGo->SetParent(parentPtr, false);

	//scene.Add(std::move(centerGo));
	//scene.Add(std::move(parentFighterGo));
	//scene.Add(std::move(childFighterGo));

	auto& inputManager = dae::InputManager::GetInstance();
	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	// Player 1
	/*auto go = std::make_unique<dae::GameObject>();*/
	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::HealthComponent>(3);
	go->AddComponent<dae::ScoreComponent>();
	go->AddComponent<dae::TransformComponent>();
	go->AddComponent<dae::RenderComponent>("Sprites/fighter01.png");
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(glm::vec3{ 472, 280, 0 });

	auto* player1Health = go->GetComponent<dae::HealthComponent>();
	auto* player1Score = go->GetComponent<dae::ScoreComponent>();

	constexpr float player1Speed = 100.f;

	inputManager.GetKeyboardInput()->AddBinding(
		std::make_unique<dae::MoveObjectCommand>(*go, dae::MoveDirection::Up, player1Speed),
		dae::InputKey::ArrowUp,
		dae::InputState::Pressed);

	inputManager.GetKeyboardInput()->AddBinding(
		std::make_unique<dae::MoveObjectCommand>(*go, dae::MoveDirection::Down, player1Speed),
		dae::InputKey::ArrowDown,
		dae::InputState::Pressed);

	inputManager.GetKeyboardInput()->AddBinding(
		std::make_unique<dae::MoveObjectCommand>(*go, dae::MoveDirection::Left, player1Speed),
		dae::InputKey::ArrowLeft,
		dae::InputState::Pressed);

	inputManager.GetKeyboardInput()->AddBinding(
		std::make_unique<dae::MoveObjectCommand>(*go, dae::MoveDirection::Right, player1Speed),
		dae::InputKey::ArrowRight,
		dae::InputState::Pressed);

	inputManager.GetKeyboardInput()->AddBinding(
		std::make_unique<ShootCommand>(*go, scene),
		dae::InputKey::C,
		dae::InputState::Down);

	inputManager.GetKeyboardInput()->AddBinding(
		std::make_unique<ShootCommand>(*go, scene),
		dae::InputKey::X,
		dae::InputState::Down);

	scene.Add(std::move(go));

	//player1 health ui
	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TransformComponent>();
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(glm::vec3{ 20, 60, 0 });
	go->AddComponent<dae::RenderComponent>();
	go->AddComponent<dae::TextComponent>("P1 Lives: 3", font, SDL_Color{ 255, 255, 255, 255 });
	go->AddComponent<dae::DisplayLivesComponent>(*player1Health, "P1");
	scene.Add(std::move(go));

	//player1 score ui
	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TransformComponent>();
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(glm::vec3{ 120, 60, 0 });
	go->AddComponent<dae::RenderComponent>();
	go->AddComponent<dae::TextComponent>("P1 Score: 0", font, SDL_Color{ 255, 255, 255, 255 });
	go->AddComponent<dae::DisplayScoreComponent>(*player1Score, "P1");
	scene.Add(std::move(go));


	// Player 2
	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TransformComponent>();
	go->AddComponent<dae::HealthComponent>(3);
	go->AddComponent<dae::ScoreComponent>();
	go->AddComponent<dae::RenderComponent>("Sprites/BossGalaga.png");
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(glm::vec3{ 552, 280, 0 });

	auto* player2Health = go->GetComponent<dae::HealthComponent>();
	auto* player2Score = go->GetComponent<dae::ScoreComponent>();

	constexpr float player2Speed = 200.f; // double speed

	inputManager.GetControllerInput(0)->AddBinding(
		std::make_unique<dae::MoveObjectCommand>(*go, dae::MoveDirection::Up, player2Speed),
		dae::InputKey::DPadUp,
		dae::InputState::Pressed);

	inputManager.GetControllerInput(0)->AddBinding(
		std::make_unique<dae::MoveObjectCommand>(*go, dae::MoveDirection::Down, player2Speed),
		dae::InputKey::DPadDown,
		dae::InputState::Pressed);

	inputManager.GetControllerInput(0)->AddBinding(
		std::make_unique<dae::MoveObjectCommand>(*go, dae::MoveDirection::Left, player2Speed),
		dae::InputKey::DPadLeft,
		dae::InputState::Pressed);

	inputManager.GetControllerInput(0)->AddBinding(
		std::make_unique<dae::MoveObjectCommand>(*go, dae::MoveDirection::Right, player2Speed),
		dae::InputKey::DPadRight,
		dae::InputState::Pressed);

	inputManager.GetControllerInput(0)->AddBinding(
		std::make_unique<ShootCommand>(*go, scene),
		dae::InputKey::ButtonA,
		dae::InputState::Down);

	//player 2 health ui
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TransformComponent>();
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(glm::vec3{ 20, 90, 0 });
	go->AddComponent<dae::RenderComponent>();
	go->AddComponent<dae::TextComponent>("P2 Lives : 3", font, SDL_Color{ 255, 255, 255, 255 });
	go->AddComponent<dae::DisplayLivesComponent>(*player2Health, "P2");

	scene.Add(std::move(go));

	//player2 score ui
	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TransformComponent>();
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(glm::vec3{ 120, 90, 0 });
	go->AddComponent<dae::RenderComponent>();
	go->AddComponent<dae::TextComponent>("P2 Score: 0", font, SDL_Color{ 255, 255, 255, 255 });
	go->AddComponent<dae::DisplayScoreComponent>(*player2Score, "P2");
	scene.Add(std::move(go));


	//add controll info text
	//p1
	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TransformComponent>();
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(glm::vec3{ 20, 520, 0 });
	go->AddComponent<dae::RenderComponent>();
	go->AddComponent<dae::TextComponent>("P1: WASD to move, X and C to shoot", font, SDL_Color{ 255, 255, 255, 255 });
	scene.Add(std::move(go));

	//p2
	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TransformComponent>();
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(glm::vec3{ 20, 550, 0 });
	go->AddComponent<dae::RenderComponent>();
	go->AddComponent<dae::TextComponent>("P2: D - Pad to move, A to shoot", font, SDL_Color{ 255, 255, 255, 255 });
	scene.Add(std::move(go));


	m_pSteamAchievements = std::make_unique<dae::SteamAchievements>();
	m_pSteamAchievementObserver = std::make_unique<dae::SteamAchievementObserver>(*m_pSteamAchievements);

	player1Score->GetSubject().AddObserver(m_pSteamAchievementObserver.get());
	player2Score->GetSubject().AddObserver(m_pSteamAchievementObserver.get());


	//Enemies 
	//auto enemy = std::make_unique<dae::GameObject>();
	//enemy->AddComponent<dae::TransformComponent>();
	//enemy->GetComponent<dae::TransformComponent>()->SetLocalPosition(300.f, 120.f, 0.f);

	//enemy->AddComponent<dae::RenderComponent>("Sprites/Bee01.png");
	//enemy->AddComponent<CollisionComponent>(32.f, 32.f);
	//enemy->AddComponent<EnemyComponent>(EnemyType::Bee);
	////enemy->GetComponent<EnemyComponent>()->StartDiving()
	//scene.Add(std::move(enemy));

	const int enemyCount = 8;
	const float startX = 250.f;
	const float formationY = 120.f;
	const float spacing = 50.f;

	auto formationController = std::make_unique<dae::GameObject>();
	formationController->AddComponent<EnemyFormationControllerComponent>();

	auto* formationControllerComponent =
		formationController->GetComponent<EnemyFormationControllerComponent>();

	for (int i = 0; i < enemyCount; ++i)
	{
		auto enemy = std::make_unique<dae::GameObject>();

		const glm::vec3 formationPosition{
			startX + i * spacing,
			formationY,
			0.f
		};

		enemy->AddComponent<dae::TransformComponent>();

		enemy->GetComponent<dae::TransformComponent>()->SetLocalPosition(
			formationPosition.x,
			-60.f - i * 20.f,
			0.f
		);

		enemy->AddComponent<dae::RenderComponent>("Sprites/Bee01.png");
		enemy->AddComponent<CollisionComponent>(32.f, 32.f);
		enemy->AddComponent<EnemyComponent>(EnemyType::Bee);

		auto* enemyComponent = enemy->GetComponent<EnemyComponent>();
		enemyComponent->FlyIntoFormation(formationPosition);

		formationControllerComponent->AddEnemy(enemyComponent);

		scene.Add(std::move(enemy));
	}

	scene.Add(std::move(formationController));
}
