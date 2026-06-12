#include "Galaga.h"
#include "SceneManager.h"

#include "SteamAchievements.h"
#include "SteamAchievementObserver.h"

#include "Scene.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "KeyboardInput.h"
#include "ControllerInput.h"
#include "InputTypes.h"
#include "GameplayConstants.h"

#include "TransformComponent.h"
#include "TextComponent.h"
#include "RenderComponent.h"
#include "FPSComponent.h"
#include "DisplayLivesComponent.h"
#include "DisplayScoreComponent.h"
#include "EnemyFormationControllerComponent.h"
#include "MissileLimitComponent.h"
#include "CollisionComponent.h"
#include "EnemyComponent.h"
#include "EnemyPlayerCollisionComponent.h"
#include "GalagaGameControllerComponent.h"
#include "ScoreComponent.h"
#include "HealthComponent.h"
#include "PlayerBulletCollisionComponent.h"
#include "VersusBossComponent.h"

#include "MoveObjectCommand.h"
#include "MovePlayerCommand.h"
#include "SkipStageCommand.h"
#include "ShootCommand.h"
#include "ToggleMuteCommand.h"
#include "ChangeInitialCommand.h"
#include "ConfirmHighScoreNameCommand.h"
#include "MoveInitialCursorCommand.h"
#include "VersusBossShootCommand.h"
#include "VersusBossDiveCommand.h"
#include "VersusBossTractorBeamCommand.h"

#include "ServiceLocator.h"
#include "SoundIds.h"

#include <array>

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
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(358, 95);
	go->AddComponent<dae::RenderComponent>("logo.png");
	auto* logoObject = go.get();
	scene.Add(std::move(go));

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	// game state / result screen text
	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TransformComponent>();
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(360, 35);
	go->AddComponent<dae::RenderComponent>();
	go->AddComponent<dae::TextComponent>("GALAGA", font, SDL_Color{ 255, 0, 0, 255 });
	auto* titleText = go->GetComponent<dae::TextComponent>();
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TransformComponent>();
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(330, 125);
	auto* scoreTextTransform = go->GetComponent<dae::TransformComponent>();
	go->AddComponent<dae::RenderComponent>();
	go->AddComponent<dae::TextComponent>("", font, SDL_Color{ 255, 255, 0, 255 });
	auto* scoreText = go->GetComponent<dae::TextComponent>();
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TransformComponent>();
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(390, 190);
	auto* initialsTextTransform = go->GetComponent<dae::TransformComponent>();
	go->AddComponent<dae::RenderComponent>();
	go->AddComponent<dae::TextComponent>("", font, SDL_Color{ 255, 255, 255, 255 });
	auto* initialsText = go->GetComponent<dae::TextComponent>();
	scene.Add(std::move(go));
	go = std::make_unique<dae::GameObject>();

	go->AddComponent<dae::TransformComponent>();
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(40, 280);
	auto* instructionTextTransform = go->GetComponent<dae::TransformComponent>();
	go->AddComponent<dae::RenderComponent>();
	go->AddComponent<dae::TextComponent>("", font, SDL_Color{ 255, 255, 255, 255 });
	auto* instructionText = go->GetComponent<dae::TextComponent>();
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TransformComponent>();
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(320, 350);
	auto* tableTitleTextTransform = go->GetComponent<dae::TransformComponent>();
	go->AddComponent<dae::RenderComponent>();
	go->AddComponent<dae::TextComponent>("", font, SDL_Color{ 255, 0, 0, 255 });
	auto* tableTitleText = go->GetComponent<dae::TextComponent>();
	scene.Add(std::move(go));

	std::array<dae::TextComponent*, 5> highScoreRows{};

	for (size_t index{}; index < highScoreRows.size(); ++index)
	{
		go = std::make_unique<dae::GameObject>();
		go->AddComponent<dae::TransformComponent>();
		go->GetComponent<dae::TransformComponent>()->SetLocalPosition(330.f, 410.f + static_cast<float>(index) * 34.f);
		go->AddComponent<dae::RenderComponent>();
		go->AddComponent<dae::TextComponent>("", font, SDL_Color{ 255, 255, 255, 255 });
		highScoreRows[index] = go->GetComponent<dae::TextComponent>();
		scene.Add(std::move(go));
	}

	//fps counter
	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TransformComponent>();
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(10, 10);
	go->AddComponent<dae::RenderComponent>();
	go->AddComponent<dae::TextComponent>("0 FPS", font, SDL_Color{ 255, 0, 0, 255 });
	go->AddComponent<dae::FPSComponent>();
	auto* fpsObject = go.get();
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

	auto& soundSystem = dae::ServiceLocator::GetSoundSystem();

	inputManager.GetKeyboardInput()->AddBinding(std::make_unique<ToggleMuteCommand>(soundSystem), dae::InputKey::F2, dae::InputState::Down);

	//add controll info text
	//p1
	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TransformComponent>();
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(glm::vec3{ 20, 520, 0 });
	go->AddComponent<dae::RenderComponent>();
	go->AddComponent<dae::TextComponent>("P1: ARROWS to move, X and C to shoot", font, SDL_Color{ 255, 255, 255, 255 });
	auto* controlsP1Text = go->GetComponent<dae::TextComponent>();
	auto* controlsP1Object = go.get();
	scene.Add(std::move(go));

	//p2
	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TransformComponent>();
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(glm::vec3{ 20, 550, 0 });
	go->AddComponent<dae::RenderComponent>();
	go->AddComponent<dae::TextComponent>("P2: D-Pad to move, A to shoot", font, SDL_Color{ 255, 255, 255, 255 });
	auto* controlsP2Text = go->GetComponent<dae::TextComponent>();
	auto* controlsP2Object = go.get();
	scene.Add(std::move(go));

	auto gameController = std::make_unique<dae::GameObject>();
	gameController->AddComponent<GalagaGameControllerComponent>(
		scene,
		*titleText,
		*scoreText,
		*scoreTextTransform,
		*initialsText,
		*initialsTextTransform,
		*instructionText,
		*instructionTextTransform,
		*tableTitleText,
		*tableTitleTextTransform,
		highScoreRows,
		*controlsP1Text,
		*controlsP2Text
	);

	auto* gameControllerComponent = gameController->GetComponent<GalagaGameControllerComponent>();

	gameControllerComponent->RegisterGameplayObject(controlsP1Object, glm::vec3{ 20.f, 520.f, 0.f });
	gameControllerComponent->RegisterPlayerTwoGameplayObject(controlsP2Object, glm::vec3{ 20.f, 550.f, 0.f });
	// Player 1
	/*auto go = std::make_unique<dae::GameObject>();*/
	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::HealthComponent>(4);
	go->AddComponent<dae::ScoreComponent>();
	go->AddComponent<MissileLimitComponent>();
	go->AddComponent<dae::TransformComponent>();
	go->AddComponent<dae::RenderComponent>(galaga::gameplay::FighterSprite);
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(glm::vec3{ 360, 500, 0 });

	auto* player1Health = go->GetComponent<dae::HealthComponent>();
	auto* player1Score = go->GetComponent<dae::ScoreComponent>();

	constexpr float player1Speed = 100.f;

	inputManager.GetKeyboardInput()->AddBinding(
		std::make_unique<MovePlayerCommand>(*go, dae::MoveDirection::Up, player1Speed, *gameControllerComponent, ControlledPlayer::PlayerOne),
		dae::InputKey::ArrowUp,
		dae::InputState::Pressed);

	inputManager.GetKeyboardInput()->AddBinding(
		std::make_unique<MovePlayerCommand>(*go, dae::MoveDirection::Down, player1Speed, *gameControllerComponent, ControlledPlayer::PlayerOne),
		dae::InputKey::ArrowDown,
		dae::InputState::Pressed);

	inputManager.GetKeyboardInput()->AddBinding(
		std::make_unique<MovePlayerCommand>(*go, dae::MoveDirection::Left, player1Speed, *gameControllerComponent, ControlledPlayer::PlayerOne),
		dae::InputKey::ArrowLeft,
		dae::InputState::Pressed);

	inputManager.GetKeyboardInput()->AddBinding(
		std::make_unique<MovePlayerCommand>(*go, dae::MoveDirection::Right, player1Speed, *gameControllerComponent, ControlledPlayer::PlayerOne),
		dae::InputKey::ArrowRight,
		dae::InputState::Pressed);

	inputManager.GetKeyboardInput()->AddBinding(
		std::make_unique<ShootCommand>(*go, scene, gameControllerComponent, ShootOwner::PlayerOne),
		dae::InputKey::C,
		dae::InputState::Down);

	inputManager.GetKeyboardInput()->AddBinding(
		std::make_unique<ShootCommand>(*go, scene, gameControllerComponent, ShootOwner::PlayerOne),
		dae::InputKey::X,
		dae::InputState::Down);

	auto* player1Object = go.get();
	player1Object->AddComponent<CollisionComponent>(galaga::gameplay::FighterCollisionWidth, galaga::gameplay::FighterCollisionHeight);
	scene.Add(std::move(go));

	gameControllerComponent->RegisterGameplayObject(player1Object, galaga::gameplay::PlayerOneStartPosition);

	//player1 health ui
	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TransformComponent>();
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(glm::vec3{ 20, 60, 0 });
	go->AddComponent<dae::RenderComponent>();
	go->AddComponent<dae::TextComponent>("P1 Lives: 3", font, SDL_Color{ 255, 255, 255, 255 });
	go->AddComponent<dae::DisplayLivesComponent>(*player1Health, "P1");
	auto* player1LivesUiObject = go.get();
	scene.Add(std::move(go));

	gameControllerComponent->RegisterGameplayObject(player1LivesUiObject, glm::vec3{ 20.f, 60.f, 0.f });

	//player1 score ui
	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TransformComponent>();
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(glm::vec3{ 120, 60, 0 });
	go->AddComponent<dae::RenderComponent>();
	go->AddComponent<dae::TextComponent>("P1 Score: 0", font, SDL_Color{ 255, 255, 255, 255 });
	go->AddComponent<dae::DisplayScoreComponent>(*player1Score, "P1");
	auto* player1ScoreUiObject = go.get();
	scene.Add(std::move(go));

	gameControllerComponent->RegisterGameplayObject(player1ScoreUiObject, glm::vec3{ 120.f, 60.f, 0.f });


	// Player 2
	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TransformComponent>();
	go->AddComponent<dae::HealthComponent>(4);
	go->AddComponent<dae::ScoreComponent>();
	go->AddComponent<MissileLimitComponent>();
	go->AddComponent<VersusBossComponent>(*gameControllerComponent, *player1Object);
	go->AddComponent<dae::RenderComponent>("Sprites/BossGalaga.png");
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(glm::vec3{ 440, 500, 0 });

	auto* player2Health = go->GetComponent<dae::HealthComponent>();
	auto* player2Score = go->GetComponent<dae::ScoreComponent>();
	auto* versusBossComponent = go->GetComponent<VersusBossComponent>();

	auto beamObject = std::make_unique<dae::GameObject>();
	beamObject->AddComponent<dae::TransformComponent>();
	beamObject->GetComponent<dae::TransformComponent>()->SetLocalPosition(-1000.f, -1000.f, 0.f);
	beamObject->AddComponent<dae::RenderComponent>(galaga::gameplay::TractorBeamSprite);

	auto* tractorBeamObject = beamObject.get();
	scene.Add(std::move(beamObject));

	versusBossComponent->SetBeamVisual(tractorBeamObject);
	gameControllerComponent->RegisterObjectToHideOnResults(tractorBeamObject);

	constexpr float player2Speed = 200.f; // double speed

	inputManager.GetControllerInput(0)->AddBinding(
		std::make_unique<MovePlayerCommand>(*go, dae::MoveDirection::Up, player2Speed, *gameControllerComponent, ControlledPlayer::PlayerTwo),
		dae::InputKey::DPadUp,
		dae::InputState::Pressed);

	inputManager.GetControllerInput(0)->AddBinding(
		std::make_unique<MovePlayerCommand>(*go, dae::MoveDirection::Down, player2Speed, *gameControllerComponent, ControlledPlayer::PlayerTwo),
		dae::InputKey::DPadDown,
		dae::InputState::Pressed);

	inputManager.GetControllerInput(0)->AddBinding(
		std::make_unique<MovePlayerCommand>(*go, dae::MoveDirection::Left, player2Speed, *gameControllerComponent, ControlledPlayer::PlayerTwo),
		dae::InputKey::DPadLeft,
		dae::InputState::Pressed);

	inputManager.GetControllerInput(0)->AddBinding(
		std::make_unique<MovePlayerCommand>(*go, dae::MoveDirection::Right, player2Speed, *gameControllerComponent, ControlledPlayer::PlayerTwo),
		dae::InputKey::DPadRight,
		dae::InputState::Pressed);

	inputManager.GetControllerInput(0)->AddBinding(
		std::make_unique<ShootCommand>(*go, scene, gameControllerComponent, ShootOwner::PlayerTwo),
		dae::InputKey::ButtonA,
		dae::InputState::Down);

	inputManager.GetControllerInput(0)->AddBinding(
		std::make_unique<VersusBossShootCommand>(*go, *player1Object, scene, *gameControllerComponent),
		dae::InputKey::ButtonA,
		dae::InputState::Down);

	inputManager.GetControllerInput(0)->AddBinding(
		std::make_unique<VersusBossDiveCommand>(*versusBossComponent),
		dae::InputKey::ButtonB,
		dae::InputState::Down);

	inputManager.GetControllerInput(0)->AddBinding(
		std::make_unique<VersusBossTractorBeamCommand>(*versusBossComponent),
		dae::InputKey::ButtonY,
		dae::InputState::Down);

	auto* player2Object = go.get();
	player2Object->AddComponent<CollisionComponent>(galaga::gameplay::BossCollisionWidth, galaga::gameplay::BossCollisionHeight);
	scene.Add(std::move(go));

	gameControllerComponent->RegisterPlayerTwoGameplayObject(player2Object, galaga::gameplay::PlayerTwoStartPosition);

	//player 2 health ui
	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TransformComponent>();
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(glm::vec3{ 20, 90, 0 });
	go->AddComponent<dae::RenderComponent>();
	go->AddComponent<dae::TextComponent>("P2 Lives : 3", font, SDL_Color{ 255, 255, 255, 255 });
	go->AddComponent<dae::DisplayLivesComponent>(*player2Health, "P2");
	auto* player2LivesUiObject = go.get();
	scene.Add(std::move(go));

	gameControllerComponent->RegisterPlayerTwoGameplayObject(player2LivesUiObject, glm::vec3{ 20.f, 90.f, 0.f });

	//player2 score ui
	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TransformComponent>();
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(glm::vec3{ 120, 90, 0 });
	go->AddComponent<dae::RenderComponent>();
	go->AddComponent<dae::TextComponent>("P2 Score: 0", font, SDL_Color{ 255, 255, 255, 255 });
	go->AddComponent<dae::DisplayScoreComponent>(*player2Score, "P2");
	auto* player2ScoreUiObject = go.get();
	scene.Add(std::move(go));

	gameControllerComponent->RegisterPlayerTwoGameplayObject(player2ScoreUiObject, glm::vec3{ 120.f, 90.f, 0.f });
	m_pSteamAchievements = std::make_unique<dae::SteamAchievements>();
	m_pSteamAchievementObserver = std::make_unique<dae::SteamAchievementObserver>(*m_pSteamAchievements);

	player1Score->GetSubject().AddObserver(m_pSteamAchievementObserver.get());
	player2Score->GetSubject().AddObserver(m_pSteamAchievementObserver.get());


	auto collisionManager = std::make_unique<dae::GameObject>();
	collisionManager->AddComponent<EnemyPlayerCollisionComponent>(scene, *gameControllerComponent);

	auto* enemyPlayerCollisionComponent = collisionManager->GetComponent<EnemyPlayerCollisionComponent>();

	enemyPlayerCollisionComponent->AddPlayer(player1Object);
	enemyPlayerCollisionComponent->AddPlayer(player2Object);


	gameControllerComponent->RegisterPlayer(player1Object);
	gameControllerComponent->RegisterPlayer(player2Object);
	gameControllerComponent->RegisterObjectToHideOnResults(logoObject);
	gameControllerComponent->RegisterObjectToHideOnResults(fpsObject);
	gameControllerComponent->RegisterGameplayObject(fpsObject, glm::vec3{ 10.f, 10.f, 0.f });

	gameControllerComponent->ForceRefreshCurrentState();
	

	inputManager.GetKeyboardInput()->AddBinding(std::make_unique<SkipStageCommand>(*gameControllerComponent), dae::InputKey::F1, dae::InputState::Down);
	inputManager.GetKeyboardInput()->AddBinding(
		std::make_unique<ChangeInitialCommand>(*gameControllerComponent, 1),
		dae::InputKey::ArrowUp,
		dae::InputState::Down
	);

	inputManager.GetKeyboardInput()->AddBinding(
		std::make_unique<ChangeInitialCommand>(*gameControllerComponent, -1),
		dae::InputKey::ArrowDown,
		dae::InputState::Down
	);

	inputManager.GetKeyboardInput()->AddBinding(
		std::make_unique<MoveInitialCursorCommand>(*gameControllerComponent, -1),
		dae::InputKey::ArrowLeft,
		dae::InputState::Down
	);

	inputManager.GetKeyboardInput()->AddBinding(
		std::make_unique<MoveInitialCursorCommand>(*gameControllerComponent, 1),
		dae::InputKey::ArrowRight,
		dae::InputState::Down
	);

	inputManager.GetKeyboardInput()->AddBinding(
		std::make_unique<ConfirmHighScoreNameCommand>(*gameControllerComponent),
		dae::InputKey::Enter,
		dae::InputState::Down
	);

	inputManager.GetControllerInput(0)->AddBinding(
		std::make_unique<ChangeInitialCommand>(*gameControllerComponent, 1),
		dae::InputKey::DPadUp,
		dae::InputState::Down
	);

	inputManager.GetControllerInput(0)->AddBinding(
		std::make_unique<ChangeInitialCommand>(*gameControllerComponent, -1),
		dae::InputKey::DPadDown,
		dae::InputState::Down
	);

	inputManager.GetControllerInput(0)->AddBinding(
		std::make_unique<MoveInitialCursorCommand>(*gameControllerComponent, -1),
		dae::InputKey::DPadLeft,
		dae::InputState::Down
	);

	inputManager.GetControllerInput(0)->AddBinding(
		std::make_unique<MoveInitialCursorCommand>(*gameControllerComponent, 1),
		dae::InputKey::DPadRight,
		dae::InputState::Down
	);

	inputManager.GetControllerInput(0)->AddBinding(
		std::make_unique<ConfirmHighScoreNameCommand>(*gameControllerComponent),
		dae::InputKey::ButtonA,
		dae::InputState::Down
	);

	inputManager.GetKeyboardInput()->AddBinding(
		std::make_unique<ConfirmHighScoreNameCommand>(*gameControllerComponent),
		dae::InputKey::C,
		dae::InputState::Down
	);

	inputManager.GetKeyboardInput()->AddBinding(
		std::make_unique<ConfirmHighScoreNameCommand>(*gameControllerComponent),
		dae::InputKey::X,
		dae::InputState::Down
	);
	scene.Add(std::move(gameController));


	auto formationController = std::make_unique<dae::GameObject>();
	formationController->AddComponent<EnemyFormationControllerComponent>(scene);
	scene.Add(std::move(formationController));

	scene.Add(std::move(collisionManager));
}
