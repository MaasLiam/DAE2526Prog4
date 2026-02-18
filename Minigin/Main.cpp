#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "Scene.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "FPSComponent.h"

#include <filesystem>
namespace fs = std::filesystem;

static void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene();


	auto go = std::make_unique<dae::GameObject>();
	go->AddComponent(std::make_unique<dae::TransformComponent>(go.get()));
	go->AddComponent(std::make_unique<dae::RenderComponent>(go.get(), "background.png"));
	scene.Add(std::move(go));
	
	go = std::make_unique<dae::GameObject>();
	go->AddComponent(std::make_unique<dae::TransformComponent>(go.get()));
	go->GetComponent<dae::TransformComponent>()->SetPosition(358, 180);
	go->AddComponent(std::make_unique<dae::RenderComponent>(go.get(), "logo.png"));
	scene.Add(std::move(go));

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	go = std::make_unique<dae::GameObject>();
	go->AddComponent(std::make_unique<dae::TransformComponent>(go.get()));
	go->GetComponent<dae::TransformComponent>()->SetPosition(292, 20);
	go->AddComponent(std::make_unique<dae::TextComponent>(go.get(), "Programming 4 Assignment", font, SDL_Color{ 255, 0, 0, 255 }));
	scene.Add(std::move(go));

	go = std::make_unique<dae::GameObject>();
	go->AddComponent(std::make_unique<dae::TransformComponent>(go.get()));
	go->GetComponent<dae::TransformComponent>()->SetPosition(10, 10);
	go->AddComponent(std::make_unique<dae::TextComponent>(go.get(), "0 FPS", font, SDL_Color{ 255, 0, 0, 255 }));
	go->AddComponent(std::make_unique<dae::FPSComponent>(go.get()));
	scene.Add(std::move(go));

}

int main(int, char*[]) {
#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
	fs::path data_location = "./Data/";
	if(!fs::exists(data_location))
		data_location = "../Data/";
#endif
	dae::Minigin engine(data_location);
	engine.Run(load);
    return 0;
}
