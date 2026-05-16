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

	dae::ServiceLocator::GetSoundSystem().Load(galaga::SoundIds::Shoot, (data_location / "Sounds" / "Shoot.wav").string());

	engine.Run();

	dae::ServiceLocator::RegisterSoundSystem(nullptr);

	return 0;
}
