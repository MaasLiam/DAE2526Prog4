#include "ThrashTheCache.h"
#include "SceneManager.h"

#include "imgui.h"
#include "imgui_plot.h"

#include "TransformComponent.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "Scene.h"
#include "RenderComponent.h"
#include "FPSComponent.h"

#include <chrono>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <cfloat>

struct TransformBench
{
    float matrix[16] = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
};

class GameObject3D
{
public:
    TransformBench transform{};
    int ID{ 1 };
};

class GameObject3DAlt
{
public:
    TransformBench* transform{};
    int ID{ 1 };
};

void ThrashTheCache::Initialize()
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
}

void ThrashTheCache::Render()
{
	//EXERCISE 1
    ImGui::SetNextWindowPos(ImVec2(20.f, 80.f), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(360.f, 220.f), ImGuiCond_Once);

    ImGui::Begin("Exercise 1");

    ImGui::PushItemWidth(80.f);
    ImGui::InputInt("##samples", &m_Samples);
    ImGui::PopItemWidth();

    ImGui::SameLine();
    ImGui::Text("# samples");

    m_Samples = std::max(3, m_Samples);

    if (ImGui::Button("Thrash the cache"))
    {
        RunExercise1();
    }

    if (!m_Ex1Timings.empty())
    {
        ImGui::PlotLines(
            "##ex1plot",
            m_Ex1Timings.data(),
            (int)m_Ex1Timings.size(),
            0,
            nullptr,
            0.0f,
            FLT_MAX,
            ImVec2(0, 120)
        );
    }
    else
    {
        ImGui::Text("Run the test to generate the plot.");
    }

    ImGui::End();



	//EXERCISE 2
    ImGui::SetNextWindowPos(ImVec2(420.f, 80.f), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(520.f, 420.f), ImGuiCond_Once);

    ImGui::Begin("Exercise 2");

    ImGui::PushItemWidth(80.f);
    ImGui::InputInt("##samples2", &m_Samples);
    ImGui::PopItemWidth();

    ImGui::SameLine();
    ImGui::Text("# samples");

    m_Samples = std::max(3, m_Samples);

    if (ImGui::Button("Thrash the cache with GameObject3D"))
    {
        RunExercise2();
    }

    if (!m_Ex2Timings.empty())
    {
        ImGui::PlotLines(
            "##ex2plot",
            m_Ex2Timings.data(),
            (int)m_Ex2Timings.size(),
            0,
            nullptr,
            0.0f,
            FLT_MAX,
            ImVec2(0, 140));
    }

    if (ImGui::Button("Thrash the cache with GameObject3DAlt"))
    {
        RunExercise2Alt();
    }

    if (!m_Ex2TimingsAlt.empty())
    {
        ImGui::PlotLines(
            "##ex2plotAlt",
            m_Ex2TimingsAlt.data(),
            (int)m_Ex2TimingsAlt.size(),
            0,
            nullptr,
            0.0f,
            FLT_MAX,
            ImVec2(0, 140));
    }

    if (!m_Ex2Timings.empty() && !m_Ex2TimingsAlt.empty())
    {
        ImGui::Text("Combined");

        const float* timingsList[] = { m_Ex2Timings.data(), m_Ex2TimingsAlt.data() };
        static ImU32 colors[] = { ImColor(0, 255, 0), ImColor(0, 200, 255) };

        float currentMax = 0.0f;

        for (float v : m_Ex2Timings)
            currentMax = std::max(currentMax, v);

        for (float v : m_Ex2TimingsAlt)
            currentMax = std::max(currentMax, v);

        if (currentMax <= 0.0f)
            currentMax = 1.0f;

        ImGui::PlotConfig conf;
        conf.values.ys_list = timingsList;
        conf.values.ys_count = 2;
        conf.values.count = (int)std::min(m_Ex2Timings.size(), m_Ex2TimingsAlt.size());
        conf.values.colors = colors;

        conf.scale.min = 0;
        conf.scale.max = currentMax;

        conf.grid_x.show = true;
        conf.grid_y.show = true;
        conf.frame_size = ImVec2(340, 140);
        conf.line_thickness = 1.f;

        
        ImGui::Plot("##combinedPlot", conf);
    }

    ImGui::End();
}

void ThrashTheCache::RunExercise1()
{
    size_t bufferSize = static_cast<size_t>(pow(2, 26));
    const int nrRuns = std::max(3, m_Samples);

    int* arr = new int[bufferSize] {};

    m_Ex1Timings.clear();
    m_Ex1Timings.reserve(11);


    for (size_t step = 1; step <= 1024; step *= 2)
    {
        std::vector<float> samples;
        samples.reserve(nrRuns);

        for (int r = 0; r < nrRuns; ++r)
        {
            const auto start = std::chrono::high_resolution_clock::now();

            for (size_t i = 0; i < bufferSize; i += step)
            {
                arr[i] *= 2;
            }

            const auto end = std::chrono::high_resolution_clock::now();
            const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            samples.push_back(static_cast<float>(elapsed));
        }

        std::sort(samples.begin(), samples.end());

        float sum = 0;
        for (int i = 1; i < nrRuns - 1; ++i)
            sum += samples[i];

        const float avg = sum / (nrRuns - 2);
        m_Ex1Timings.push_back((float)avg);
    }

	delete[] arr;
}

void ThrashTheCache::RunExercise2()
{
    const size_t count = 10000000;
    const int nrRuns = std::max(3, m_Samples);

    GameObject3D* p_array = new GameObject3D[count];

    m_Ex2Timings.clear();
    m_Ex2Timings.reserve(11);


    for (size_t step = 1; step <= 1024; step *= 2)
    {
        std::vector<float> samples;
        samples.reserve(nrRuns);

        for (int r = 0; r < nrRuns; ++r)
        {
            const auto start = std::chrono::high_resolution_clock::now();

            for (size_t i = 0; i < count; i += step)
            {
                p_array[i].ID *= 2;
            }

            const auto end = std::chrono::high_resolution_clock::now();
            const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            samples.push_back(static_cast<float>(elapsed));
        }

        std::sort(samples.begin(), samples.end());

        float sum = 0;
        for (int i = 1; i < nrRuns - 1; ++i)
            sum += samples[i];

        const float avg = sum / (nrRuns - 2);
        m_Ex2Timings.push_back((float)avg);
    }

	delete[] p_array;
}

void ThrashTheCache::RunExercise2Alt()
{
    const size_t count = 10000000;
    const int nrRuns = std::max(3, m_Samples);

    GameObject3DAlt* p_array = new GameObject3DAlt[count];

    m_Ex2TimingsAlt.clear();
    m_Ex2TimingsAlt.reserve(11);


    for (size_t step = 1; step <= 1024; step *= 2)
    {
        std::vector<float> samples;
        samples.reserve(nrRuns);

        for (int r = 0; r < nrRuns; ++r)
        {
            const auto start = std::chrono::high_resolution_clock::now();

            for (size_t i = 0; i < count; i += step)
            {
                p_array[i].ID *= 2;
            }

            const auto end = std::chrono::high_resolution_clock::now();
            const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            samples.push_back(static_cast<float>(elapsed));
        }

        std::sort(samples.begin(), samples.end());

        float sum = 0;
        for (int i = 1; i < nrRuns - 1; ++i)
            sum += samples[i];

        const float avg = sum / (nrRuns - 2);
        m_Ex2TimingsAlt.push_back((float)avg);
    }

	delete[] p_array;
}
