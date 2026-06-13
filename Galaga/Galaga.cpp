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
#include <memory>
#include <string>

namespace
{
	constexpr float HudX{ 640.f };
	constexpr float HudTopY{ 60.f };
	constexpr float HudLineGap{ 40.f };

	struct TextObject
	{
		dae::GameObject* object{};
		dae::TextComponent* text{};
		dae::TransformComponent* transform{};
	};

	struct PlayerUiObjects
	{
		dae::GameObject* livesObject{};
		dae::GameObject* scoreObject{};
	};

	dae::GameObject* CreateSpriteObject(dae::Scene& scene, const std::string& texturePath, const glm::vec3& position)
	{
		auto object = std::make_unique<dae::GameObject>();
		object->AddComponent<dae::TransformComponent>();
		object->GetComponent<dae::TransformComponent>()->SetLocalPosition(position);
		object->AddComponent<dae::RenderComponent>(texturePath);

		auto* rawObject = object.get();
		scene.Add(std::move(object));

		return rawObject;
	}

	TextObject CreateTextObject(dae::Scene& scene, const std::string& text, std::shared_ptr<dae::Font> font, const SDL_Color& color, const glm::vec3& position)
	{
		auto object = std::make_unique<dae::GameObject>();
		object->AddComponent<dae::TransformComponent>();
		object->GetComponent<dae::TransformComponent>()->SetLocalPosition(position);

		auto* transform = object->GetComponent<dae::TransformComponent>();

		object->AddComponent<dae::RenderComponent>();
		object->AddComponent<dae::TextComponent>(text, font, color);

		auto* textComponent = object->GetComponent<dae::TextComponent>();
		auto* rawObject = object.get();

		scene.Add(std::move(object));

		return TextObject{ rawObject, textComponent, transform };
	}

	void BindPlayerMovementToKeyboard(
		dae::KeyboardInput& keyboard,
		dae::GameObject& player,
		float speed,
		galaga::GalagaGameControllerComponent& gameController,
		galaga::ControlledPlayer controlledPlayer)
	{
		keyboard.AddBinding(
			std::make_unique<galaga::MovePlayerCommand>(player, dae::MoveDirection::Up, speed, gameController, controlledPlayer),
			dae::InputKey::ArrowUp,
			dae::InputState::Pressed);

		keyboard.AddBinding(
			std::make_unique<galaga::MovePlayerCommand>(player, dae::MoveDirection::Down, speed, gameController, controlledPlayer),
			dae::InputKey::ArrowDown,
			dae::InputState::Pressed);

		keyboard.AddBinding(
			std::make_unique<galaga::MovePlayerCommand>(player, dae::MoveDirection::Left, speed, gameController, controlledPlayer),
			dae::InputKey::ArrowLeft,
			dae::InputState::Pressed);

		keyboard.AddBinding(
			std::make_unique<galaga::MovePlayerCommand>(player, dae::MoveDirection::Right, speed, gameController, controlledPlayer),
			dae::InputKey::ArrowRight,
			dae::InputState::Pressed);
	}

	void BindPlayerMovementToController(
		dae::ControllerInput& controller,
		dae::GameObject& player,
		float speed,
		galaga::GalagaGameControllerComponent& gameController,
		galaga::ControlledPlayer controlledPlayer,
		galaga::ControlContext controlContext)
	{
		controller.AddBinding(
			std::make_unique<galaga::MovePlayerCommand>(player, dae::MoveDirection::Up, speed, gameController, controlledPlayer, controlContext),
			dae::InputKey::DPadUp,
			dae::InputState::Pressed);

		controller.AddBinding(
			std::make_unique<galaga::MovePlayerCommand>(player, dae::MoveDirection::Down, speed, gameController, controlledPlayer, controlContext),
			dae::InputKey::DPadDown,
			dae::InputState::Pressed);

		controller.AddBinding(
			std::make_unique<galaga::MovePlayerCommand>(player, dae::MoveDirection::Left, speed, gameController, controlledPlayer, controlContext),
			dae::InputKey::DPadLeft,
			dae::InputState::Pressed);

		controller.AddBinding(
			std::make_unique<galaga::MovePlayerCommand>(player, dae::MoveDirection::Right, speed, gameController, controlledPlayer, controlContext),
			dae::InputKey::DPadRight,
			dae::InputState::Pressed);
	}

	void BindPlayerShootToKeyboard(
		dae::KeyboardInput& keyboard,
		dae::GameObject& player,
		dae::Scene& scene,
		galaga::GalagaGameControllerComponent& gameController,
		galaga::ShootOwner shootOwner)
	{
		keyboard.AddBinding(
			std::make_unique<galaga::ShootCommand>(player, scene, &gameController, shootOwner),
			dae::InputKey::C,
			dae::InputState::Down);

		keyboard.AddBinding(
			std::make_unique<galaga::ShootCommand>(player, scene, &gameController, shootOwner),
			dae::InputKey::X,
			dae::InputState::Down);
	}

	void BindPlayerShootToController(
		dae::ControllerInput& controller,
		dae::GameObject& player,
		dae::Scene& scene,
		galaga::GalagaGameControllerComponent& gameController,
		galaga::ShootOwner shootOwner,
		galaga::ControlContext controlContext)
	{
		controller.AddBinding(
			std::make_unique<galaga::ShootCommand>(player, scene, &gameController, shootOwner, controlContext),
			dae::InputKey::ButtonA,
			dae::InputState::Down);
	}

	void BindVersusBossActionsToController(
		dae::ControllerInput& controller,
		dae::GameObject& boss,
		dae::GameObject& targetPlayer,
		dae::Scene& scene,
		galaga::GalagaGameControllerComponent& gameController,
		galaga::VersusBossComponent& versusBoss)
	{
		controller.AddBinding(
			std::make_unique<galaga::VersusBossShootCommand>(boss, targetPlayer, scene, gameController),
			dae::InputKey::ButtonA,
			dae::InputState::Down);

		controller.AddBinding(
			std::make_unique<galaga::VersusBossDiveCommand>(versusBoss),
			dae::InputKey::ButtonB,
			dae::InputState::Down);

		controller.AddBinding(
			std::make_unique<galaga::VersusBossTractorBeamCommand>(versusBoss),
			dae::InputKey::ButtonY,
			dae::InputState::Down);
	}

	void BindHighScoreInputToKeyboard(dae::KeyboardInput& keyboard, galaga::GalagaGameControllerComponent& gameController)
	{
		keyboard.AddBinding(
			std::make_unique<galaga::ChangeInitialCommand>(gameController, 1),
			dae::InputKey::ArrowUp,
			dae::InputState::Down);

		keyboard.AddBinding(
			std::make_unique<galaga::ChangeInitialCommand>(gameController, -1),
			dae::InputKey::ArrowDown,
			dae::InputState::Down);

		keyboard.AddBinding(
			std::make_unique<galaga::MoveInitialCursorCommand>(gameController, -1),
			dae::InputKey::ArrowLeft,
			dae::InputState::Down);

		keyboard.AddBinding(
			std::make_unique<galaga::MoveInitialCursorCommand>(gameController, 1),
			dae::InputKey::ArrowRight,
			dae::InputState::Down);

		keyboard.AddBinding(
			std::make_unique<galaga::ConfirmHighScoreNameCommand>(gameController),
			dae::InputKey::Enter,
			dae::InputState::Down);

		keyboard.AddBinding(
			std::make_unique<galaga::ConfirmHighScoreNameCommand>(gameController),
			dae::InputKey::C,
			dae::InputState::Down);

		keyboard.AddBinding(
			std::make_unique<galaga::ConfirmHighScoreNameCommand>(gameController),
			dae::InputKey::X,
			dae::InputState::Down);
	}

	void BindHighScoreInputToController(dae::ControllerInput& controller, galaga::GalagaGameControllerComponent& gameController)
	{
		controller.AddBinding(
			std::make_unique<galaga::ChangeInitialCommand>(gameController, 1),
			dae::InputKey::DPadUp,
			dae::InputState::Down);

		controller.AddBinding(
			std::make_unique<galaga::ChangeInitialCommand>(gameController, -1),
			dae::InputKey::DPadDown,
			dae::InputState::Down);

		controller.AddBinding(
			std::make_unique<galaga::MoveInitialCursorCommand>(gameController, -1),
			dae::InputKey::DPadLeft,
			dae::InputState::Down);

		controller.AddBinding(
			std::make_unique<galaga::MoveInitialCursorCommand>(gameController, 1),
			dae::InputKey::DPadRight,
			dae::InputState::Down);

		controller.AddBinding(
			std::make_unique<galaga::ConfirmHighScoreNameCommand>(gameController),
			dae::InputKey::ButtonA,
			dae::InputState::Down);
	}

	PlayerUiObjects CreatePlayerUi(
		dae::Scene& scene,
		std::shared_ptr<dae::Font> font,
		galaga::HealthComponent& health,
		galaga::ScoreComponent& score,
		const std::string& label,
		const glm::vec3& livesPosition,
		const glm::vec3& scorePosition)
	{
		auto livesObject = std::make_unique<dae::GameObject>();
		livesObject->AddComponent<dae::TransformComponent>();
		livesObject->GetComponent<dae::TransformComponent>()->SetLocalPosition(livesPosition);
		livesObject->AddComponent<dae::RenderComponent>();
		livesObject->AddComponent<dae::TextComponent>(label + " Lives: 3", font, SDL_Color{ 255, 255, 255, 255 });
		livesObject->AddComponent<galaga::DisplayLivesComponent>(health, label);

		auto* livesRawObject = livesObject.get();
		scene.Add(std::move(livesObject));

		auto scoreObject = std::make_unique<dae::GameObject>();
		scoreObject->AddComponent<dae::TransformComponent>();
		scoreObject->GetComponent<dae::TransformComponent>()->SetLocalPosition(scorePosition);
		scoreObject->AddComponent<dae::RenderComponent>();
		scoreObject->AddComponent<dae::TextComponent>(label + " Score: 0", font, SDL_Color{ 255, 255, 255, 255 });
		scoreObject->AddComponent<galaga::DisplayScoreComponent>(score, label);

		auto* scoreRawObject = scoreObject.get();
		scene.Add(std::move(scoreObject));

		return PlayerUiObjects{ livesRawObject, scoreRawObject };
	}
}

void Galaga::Initialize()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();

	//background
	CreateSpriteObject(scene, "background.png", glm::vec3{ 0.f, 0.f, 0.f });

	//logo 
	auto* logoObject = CreateSpriteObject(scene, "galaga_logo.png", glm::vec3{ 350.f, 60.f, 0.f });


	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	// game state / result screen text
	auto titleTextObject = CreateTextObject(scene, "", font, SDL_Color{ 255, 0, 0, 255 }, glm::vec3{ 350.f, 25.f, 0.f });
	auto* titleText = titleTextObject.text;

	//score text
	auto scoreTextObject = CreateTextObject(scene, "", font, SDL_Color{ 255, 255, 0, 255 }, glm::vec3{ HudX, HudTopY, 0.f });
	auto* scoreText = scoreTextObject.text;
	auto* scoreTextTransform = scoreTextObject.transform;

	//initials text
	auto initialsTextObject = CreateTextObject(scene, "", font, SDL_Color{ 255, 255, 255, 255 }, glm::vec3{ 390.f, 190.f, 0.f });
	auto* initialsText = initialsTextObject.text;
	auto* initialsTextTransform = initialsTextObject.transform;

	//instruction text
	auto instructionTextObject = CreateTextObject(scene, "", font, SDL_Color{ 255, 255, 255, 255 }, glm::vec3{ 40.f, 280.f, 0.f });
	auto* instructionText = instructionTextObject.text;
	auto* instructionTextTransform = instructionTextObject.transform;

	//high score table title text
	auto tableTitleTextObject = CreateTextObject(scene, "", font, SDL_Color{ 255, 0, 0, 255 }, glm::vec3{ 320.f, 350.f, 0.f });
	auto* tableTitleText = tableTitleTextObject.text;
	auto* tableTitleTextTransform = tableTitleTextObject.transform;

	std::array<dae::TextComponent*, 5> highScoreRows{};

	for (size_t index{}; index < highScoreRows.size(); ++index)
	{
		auto highScoreRow = CreateTextObject(scene, "", font, SDL_Color{ 255, 255, 255, 255 }, glm::vec3{ 330.f, 410.f + static_cast<float>(index) * 34.f, 0.f });
		highScoreRows[index] = highScoreRow.text;
	}

	//fps counter
	auto fpsTextObject = CreateTextObject(scene, "0 FPS", font, SDL_Color{ 255, 0, 0, 255 }, glm::vec3{ 10.f, 10.f, 0.f });
	auto* fpsObject = fpsTextObject.object;
	fpsObject->AddComponent<dae::FPSComponent>();


	auto& inputManager = dae::InputManager::GetInstance();
	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	auto& soundSystem = dae::ServiceLocator::GetSoundSystem();

	inputManager.GetKeyboardInput()->AddBinding(std::make_unique<galaga::ToggleMuteCommand>(soundSystem), dae::InputKey::F2, dae::InputState::Down);

	//add controll info text
	//p1
	auto controlsP1TextObject = CreateTextObject(scene, "Keyboard: ARROWS to move, X and C to shoot", font, SDL_Color{ 255, 255, 255, 255 }, glm::vec3{ 20.f, 520.f, 0.f });
	auto* controlsP1Text = controlsP1TextObject.text;
	auto* controlsP1Object = controlsP1TextObject.object;

	//p2
	auto controlsP2TextObject = CreateTextObject(scene, "Gamepad: dpad to move, A to shoot. Versus: B to dive Y to start beam", font, SDL_Color{ 255, 255, 255, 255 }, glm::vec3{ 20.f, 550.f, 0.f });
	auto* controlsP2Text = controlsP2TextObject.text;
	auto* controlsP2Object = controlsP2TextObject.object;
	auto gameController = std::make_unique<dae::GameObject>();
	auto* gameControllerObject = gameController.get();

	galaga::GameControllerUi gameControllerUi{
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
	};

	auto& gameControllerComponent = gameControllerObject->AddComponent<galaga::GalagaGameControllerComponent>(scene, gameControllerUi);

	gameControllerComponent.RegisterMenuObject(logoObject);
	gameControllerComponent.RegisterMenuObject(controlsP1Object);
	gameControllerComponent.RegisterMenuObject(controlsP2Object);
	// Player 1
	auto go = std::make_unique<dae::GameObject>();
	go->AddComponent<galaga::HealthComponent>(galaga::gameplay::StartingLives);
	go->AddComponent<galaga::ScoreComponent>();
	go->AddComponent<galaga::MissileLimitComponent>();
	go->AddComponent<dae::TransformComponent>();
	go->AddComponent<dae::RenderComponent>(galaga::gameplay::FighterSprite);
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(galaga::gameplay::PlayerOneStartPosition);

	auto* player1Health = go->GetComponent<galaga::HealthComponent>();
	auto* player1Score = go->GetComponent<galaga::ScoreComponent>();

	constexpr float playerSpeed = 100.f;
	//keyboard binding for player 1
	BindPlayerMovementToKeyboard(*inputManager.GetKeyboardInput(), *go, playerSpeed, gameControllerComponent, galaga::ControlledPlayer::PlayerOne);
	BindPlayerShootToKeyboard(*inputManager.GetKeyboardInput(), *go, scene, gameControllerComponent, galaga::ShootOwner::PlayerOne);

	//controller 0 binding for player 1 in singleplayer mode
	BindPlayerMovementToController(
		*inputManager.GetControllerInput(0),
		*go,
		playerSpeed,
		gameControllerComponent,
		galaga::ControlledPlayer::PlayerOne,
		galaga::ControlContext::SinglePlayerOnly);

	BindPlayerShootToController(
		*inputManager.GetControllerInput(0),
		*go,
		scene,
		gameControllerComponent,
		galaga::ShootOwner::PlayerOne,
		galaga::ControlContext::SinglePlayerOnly);

	//controller 1 binding for player 1 in multiplayer mode
	BindPlayerMovementToController(
		*inputManager.GetControllerInput(1),
		*go,
		playerSpeed,
		gameControllerComponent,
		galaga::ControlledPlayer::PlayerOne,
		galaga::ControlContext::MultiplayerOnly);

	BindPlayerShootToController(
		*inputManager.GetControllerInput(1),
		*go,
		scene,
		gameControllerComponent,
		galaga::ShootOwner::PlayerOne,
		galaga::ControlContext::MultiplayerOnly);

	auto* player1Object = go.get();
	player1Object->AddComponent<galaga::CollisionComponent>(galaga::gameplay::FighterCollisionWidth, galaga::gameplay::FighterCollisionHeight, galaga::gameplay::PlayerHitboxOffsetX, galaga::gameplay::PlayerHitboxOffsetY);
	scene.Add(std::move(go));

	gameControllerComponent.RegisterGameplayObject(player1Object, galaga::gameplay::PlayerOneStartPosition);

	//p1 lives and score ui
	const auto player1LivesPosition = glm::vec3{ HudX, HudTopY, 0.f };
	const auto player1ScorePosition = glm::vec3{ HudX, HudTopY + HudLineGap, 0.f };

	const auto player1Ui = CreatePlayerUi(
		scene,
		font,
		*player1Health,
		*player1Score,
		"P1",
		player1LivesPosition,
		player1ScorePosition);

	gameControllerComponent.RegisterGameplayObject(player1Ui.livesObject, player1LivesPosition);
	gameControllerComponent.RegisterGameplayObject(player1Ui.scoreObject, player1ScorePosition);


	// Player 2
	go = std::make_unique<dae::GameObject>();
	go->AddComponent<dae::TransformComponent>();
	go->AddComponent<galaga::HealthComponent>(galaga::gameplay::StartingLives);
	go->AddComponent<galaga::ScoreComponent>();
	go->AddComponent<galaga::MissileLimitComponent>();
	go->AddComponent<galaga::VersusBossComponent>(gameControllerComponent, *player1Object);
	go->AddComponent<dae::RenderComponent>(galaga::gameplay::FighterTwoSprite);
	go->GetComponent<dae::TransformComponent>()->SetLocalPosition(galaga::gameplay::PlayerTwoStartPosition);

	auto* player2Health = go->GetComponent<galaga::HealthComponent>();
	auto* player2Score = go->GetComponent<galaga::ScoreComponent>();
	auto* versusBossComponent = go->GetComponent<galaga::VersusBossComponent>();

	auto beamObject = std::make_unique<dae::GameObject>();
	beamObject->AddComponent<dae::TransformComponent>();
	beamObject->GetComponent<dae::TransformComponent>()->SetLocalPosition(galaga::gameplay::HiddenObjectPosition, galaga::gameplay::HiddenObjectPosition, 0.f);
	beamObject->AddComponent<dae::RenderComponent>(galaga::gameplay::TractorBeamSprite);

	auto* tractorBeamObject = beamObject.get();
	scene.Add(std::move(beamObject));

	versusBossComponent->SetBeamVisual(tractorBeamObject);
	gameControllerComponent.RegisterObjectToHideOnResults(tractorBeamObject);

	BindPlayerMovementToController(
		*inputManager.GetControllerInput(0),
		*go,
		playerSpeed,
		gameControllerComponent,
		galaga::ControlledPlayer::PlayerTwo,
		galaga::ControlContext::MultiplayerOnly);

	BindPlayerShootToController(
		*inputManager.GetControllerInput(0),
		*go,
		scene,
		gameControllerComponent,
		galaga::ShootOwner::PlayerTwo,
		galaga::ControlContext::MultiplayerOnly);

	//versus boss specific actions

	BindVersusBossActionsToController(
		*inputManager.GetControllerInput(0),
		*go,
		*player1Object,
		scene,
		gameControllerComponent,
		*versusBossComponent);

	auto* player2Object = go.get();
	player2Object->AddComponent<galaga::CollisionComponent>(galaga::gameplay::BossCollisionWidth, galaga::gameplay::BossCollisionHeight, galaga::gameplay::BossHitboxOffsetX, galaga::gameplay::BossHitboxOffsetY);
	scene.Add(std::move(go));

	gameControllerComponent.RegisterPlayerTwoGameplayObject(player2Object, galaga::gameplay::PlayerTwoStartPosition);

	//player 2 health and score ui
	const auto player2LivesPosition = glm::vec3{ HudX, HudTopY + HudLineGap * 3.f, 0.f };
	const auto player2ScorePosition = glm::vec3{ HudX, HudTopY + HudLineGap * 4.f, 0.f };

	const auto player2Ui = CreatePlayerUi(
		scene,
		font,
		*player2Health,
		*player2Score,
		"P2",
		player2LivesPosition,
		player2ScorePosition);

	gameControllerComponent.RegisterPlayerTwoGameplayObject(player2Ui.livesObject, player2LivesPosition);
	gameControllerComponent.RegisterPlayerTwoGameplayObject(player2Ui.scoreObject, player2ScorePosition);

	auto collisionManager = std::make_unique<dae::GameObject>();
	collisionManager->AddComponent<galaga::EnemyPlayerCollisionComponent>(scene, gameControllerComponent);

	auto* enemyPlayerCollisionComponent = collisionManager->GetComponent<galaga::EnemyPlayerCollisionComponent>();

	enemyPlayerCollisionComponent->AddPlayer(player1Object);
	enemyPlayerCollisionComponent->AddPlayer(player2Object);


	gameControllerComponent.RegisterPlayer(player1Object);
	gameControllerComponent.RegisterPlayer(player2Object);
	gameControllerComponent.RegisterObjectToHideOnResults(fpsObject);
	gameControllerComponent.RegisterGameplayObject(fpsObject, glm::vec3{ 10.f, 10.f, 0.f });

	gameControllerComponent.ForceRefreshCurrentState();
	

	inputManager.GetKeyboardInput()->AddBinding(std::make_unique<galaga::SkipStageCommand>(gameControllerComponent), dae::InputKey::F1, dae::InputState::Down);

	//bindings for high score name entry
	BindHighScoreInputToKeyboard(*inputManager.GetKeyboardInput(), gameControllerComponent);
	BindHighScoreInputToController(*inputManager.GetControllerInput(0), gameControllerComponent);
	BindHighScoreInputToController(*inputManager.GetControllerInput(1), gameControllerComponent);
	
	scene.Add(std::move(gameController));


	auto formationController = std::make_unique<dae::GameObject>();
	formationController->AddComponent<galaga::EnemyFormationControllerComponent>(scene);
	scene.Add(std::move(formationController));

	scene.Add(std::move(collisionManager));
}
