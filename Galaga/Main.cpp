#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Minigin.h"
#include "Galaga.h"
#include "ThrashTheCache.h"

#include "ServiceLocator.h"
#include "SDLSoundSystem.h"
#include "LoggingSoundSystem.h"
#include "SoundIds.h"

#include <filesystem>
#include <memory>

namespace fs = std::filesystem;

int main(int, char*[]) {
#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
	fs::path data_location = "./Data/";
	if(!fs::exists(data_location))
		data_location = "../Data/";
#endif
	dae::Minigin engine(data_location, std::make_unique<Galaga>());
#if _DEBUG
	dae::ServiceLocator::RegisterSoundSystem(
		std::make_unique<dae::LoggingSoundSystem>(
			std::make_unique<dae::SDLSoundSystem>()
		)
	);
#else
	dae::ServiceLocator::RegisterSoundSystem(
		std::make_unique<dae::SDLSoundSystem>()
	);
#endif

	auto& soundSystem = dae::ServiceLocator::GetSoundSystem();

	soundSystem.Load(galaga::ToSoundId(galaga::SoundIds::Shoot), (data_location / "Sounds" / "Shoot.wav").string());
	soundSystem.Load(galaga::ToSoundId(galaga::SoundIds::StageStart), (data_location / "Sounds" / "StartStage.mp3").string());
	soundSystem.Load(galaga::ToSoundId(galaga::SoundIds::TractorBeam), (data_location / "Sounds" / "tractorBeam.mp3").string());
	soundSystem.Load(galaga::ToSoundId(galaga::SoundIds::GameOver), (data_location / "Sounds" / "GameOver.mp3").string());
	soundSystem.Load(galaga::ToSoundId(galaga::SoundIds::BossGalagaDestroyed), (data_location / "Sounds" / "bossgalagaDestroyed.mp3").string());
	soundSystem.Load(galaga::ToSoundId(galaga::SoundIds::BossGalagaInjured), (data_location / "Sounds" / "bossgalagaInjured.mp3").string());
	soundSystem.Load(galaga::ToSoundId(galaga::SoundIds::EnemyDestroyed), (data_location / "Sounds" / "galagaDestroyed.mp3").string());
	soundSystem.Load(galaga::ToSoundId(galaga::SoundIds::EnemyDive), (data_location / "Sounds" / "galagaDive.mp3").string());
	soundSystem.Load(galaga::ToSoundId(galaga::SoundIds::PlayerHit), (data_location / "Sounds" / "PlayerHit.mp3").string());

	engine.Run();

	dae::ServiceLocator::RegisterSoundSystem(nullptr);

	return 0;
}
