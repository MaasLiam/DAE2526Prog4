#include "SDLSoundSystem.h"

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

#include <unordered_map>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <string>

namespace dae
{
	class SDLSoundSystem::Impl
	{
	public:
		Impl()
		{
			MIX_Init();

			SDL_AudioSpec spec{};
			spec.freq = 44100;
			spec.format = SDL_AUDIO_F32;
			spec.channels = 2;

			m_Mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec);

			m_Running = true;
			m_WorkerThread = std::thread(&Impl::ProcessQueue, this);
		}

		~Impl()
		{
			m_Running = false;
			m_Condition.notify_one();

			if (m_WorkerThread.joinable())
				m_WorkerThread.join();

			for (auto* track : m_Tracks)
				MIX_DestroyTrack(track);

			for (auto& [id, audio] : m_Sounds)
				MIX_DestroyAudio(audio);

			if (m_Mixer)
			{
				MIX_DestroyMixer(m_Mixer);
				m_Mixer = nullptr;
			}

			MIX_Quit();
		}

		void Load(SoundId id, const std::string& path)
		{
			std::lock_guard<std::mutex> lock(m_Mutex);
			m_SoundPaths[id] = path;
		}

		void Play(SoundId id, float volume)
		{
			std::lock_guard<std::mutex> lock(m_Mutex);
			m_Queue.push({ id, volume });
			m_Condition.notify_one();
		}

		void ToggleMute()
		{
			m_IsMuted = !m_IsMuted;

			if (m_IsMuted)
			{
				StopAll();
			}
		}

		bool IsMuted() const
		{
			return m_IsMuted;
		}

		void StopAll()
		{
			std::lock_guard<std::mutex> lock(m_Mutex);

			for (auto* track : m_Tracks)
			{
				MIX_DestroyTrack(track);
			}

			m_Tracks.clear();

			std::queue<PlayRequest> emptyQueue;
			m_Queue.swap(emptyQueue);
		}

	private:
		struct PlayRequest
		{
			SoundId id;
			float volume;
		};

		void ProcessQueue()
		{
			while (m_Running)
			{
				std::unique_lock<std::mutex> lock(m_Mutex);

				m_Condition.wait(lock, [this]
					{
						return !m_Queue.empty() || !m_Running;
					});

				while (!m_Queue.empty())
				{
					const PlayRequest request = m_Queue.front();
					m_Queue.pop();

					if (m_IsMuted)
					{
						continue;
					}

					if (!m_Mixer)
						continue;

					auto soundIt = m_Sounds.find(request.id);

					if (soundIt == m_Sounds.end())
					{
						auto pathIt = m_SoundPaths.find(request.id);
						if (pathIt == m_SoundPaths.end())
							continue;

						const std::string path = pathIt->second;

						lock.unlock();

						MIX_Audio* loadedAudio = MIX_LoadAudio(m_Mixer, path.c_str(), true);

						lock.lock();

						if (!loadedAudio)
							continue;

						m_Sounds[request.id] = loadedAudio;
						soundIt = m_Sounds.find(request.id);
					}

					MIX_Audio* audio = soundIt->second;

					lock.unlock();

					MIX_Track* track = MIX_CreateTrack(m_Mixer);
					if (track)
					{
						MIX_SetTrackAudio(track, audio);
						MIX_SetTrackGain(track, request.volume);
						MIX_PlayTrack(track, 0);

						lock.lock();
						m_Tracks.push_back(track);
						lock.unlock();
					}

					lock.lock();
				}
			}
		}

		MIX_Mixer* m_Mixer{ nullptr };

		std::unordered_map<SoundId, std::string> m_SoundPaths;
		std::unordered_map<SoundId, MIX_Audio*> m_Sounds;
		std::vector<MIX_Track*> m_Tracks;

		std::queue<PlayRequest> m_Queue;
		std::mutex m_Mutex;
		std::condition_variable m_Condition;

		std::thread m_WorkerThread;
		std::atomic<bool> m_Running{ false };
		std::atomic<bool> m_IsMuted{ false };
	};

	SDLSoundSystem::SDLSoundSystem()
		: m_Impl(std::make_unique<Impl>())
	{
	}

	SDLSoundSystem::~SDLSoundSystem() = default;

	void SDLSoundSystem::Play(SoundId id, float volume)
	{
		m_Impl->Play(id, volume);
	}

	void SDLSoundSystem::Load(SoundId id, const std::string& path)
	{
		m_Impl->Load(id, path);
	}

	void SDLSoundSystem::ToggleMute()
	{
		m_Impl->ToggleMute();
	}

	bool SDLSoundSystem::IsMuted() const
	{
		return m_Impl->IsMuted();
	}

	void SDLSoundSystem::StopAll()
	{
		m_Impl->StopAll();
	}
}