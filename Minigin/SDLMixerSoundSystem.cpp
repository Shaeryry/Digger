#include "SDLMixerSoundSystem.h"
#include "Helpers.h"

#include <map>
#include <utility>
#include <queue>
#include <mutex>
#include <SDL_mixer.h>
#include <filesystem>

struct Audio {
	Mix_Music* music;
	Mix_Chunk* soundEffect;
	bool isMusic;
	float volume;
};

class Rinigin::SDLMixerSoundService::SDLpimpl {
public:
	SDLpimpl(const std::string& path);
	~SDLpimpl();
	void Play(const Rinigin::AudioRequest& request);
	void SetMuted(bool mute);
	bool IsMuted() const { return m_Muted; }
	void Update(const std::stop_token& stopToken);
private:
	const Audio& GetAudio(const AudioRequest& request);
	void UnloadSounds();

	std::mutex m_Mutex;
	std::jthread m_SoundThread;
	std::condition_variable m_ConditionVariable;

	std::string m_Path; 
	std::map<unsigned int, Audio> m_LoadedAudio;
	std::queue<Rinigin::AudioRequest> m_SoundQueue;

	// Mute
	bool m_Muted = false;
	std::mutex m_MuteMutex;
};

Rinigin::SDLMixerSoundService::SDLpimpl::SDLpimpl(const std::string& path)
{
	m_Path = path;
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	m_SoundThread = std::jthread([&](const std::stop_token& stoptoken)
		{
			Update(stoptoken);
		}
	);
}

Rinigin::SDLMixerSoundService::SDLpimpl::~SDLpimpl()
{
	UnloadSounds();
	Mix_CloseAudio();
	m_SoundThread.request_stop();
	m_ConditionVariable.notify_all();
}

const Audio& Rinigin::SDLMixerSoundService::SDLpimpl::GetAudio(const AudioRequest& request)
{
	const std::string filePath{ m_Path + request.sound };
	const char* soundFile = filePath.c_str();
	unsigned int soundId = Helpers::sdbm_hash(request.sound.c_str());

	if (m_LoadedAudio.find(soundId) == m_LoadedAudio.end())
	{
		Audio audio{};
		audio.isMusic = request.isMusic;
		audio.music = Mix_LoadMUS(soundFile);
		audio.volume = request.volume;
		audio.soundEffect = Mix_LoadWAV(soundFile);
		 
		m_LoadedAudio.insert({ soundId, audio });
	}

	return m_LoadedAudio.at(soundId);
}

void Rinigin::SDLMixerSoundService::SDLpimpl::Update(const std::stop_token& stopToken)
{
	while (!stopToken.stop_requested())
	{
		// Threading
		std::unique_lock<std::mutex> lock(m_Mutex); 
		m_ConditionVariable.wait(lock, [this,stopToken] { return !m_SoundQueue.empty() || stopToken.stop_requested(); });
		if (stopToken.stop_requested()) return;
		//

		const auto soundRequest = m_SoundQueue.front();
		m_SoundQueue.pop();

		// Stop logic

		if (soundRequest.stop) {
			const unsigned int soundId = Helpers::sdbm_hash(soundRequest.sound.c_str());
			auto it = m_LoadedAudio.find(soundId);
			if (it != m_LoadedAudio.end()) {
				const Audio& audio = it->second;
				lock.unlock();

				if (audio.isMusic) {
					Mix_HaltMusic();
				}
				else {
					for (int ch = 0; ch < Mix_AllocateChannels(-1); ++ch) {
						if (Mix_GetChunk(ch) == audio.soundEffect) {
							Mix_HaltChannel(ch);
						}
					}
				}
			}
			else {
				lock.unlock(); // not loaded, nothing to stop
			}

			continue;
		}



		const Audio& pSound = GetAudio(soundRequest);
		lock.unlock();

		const int volume = static_cast<int>(soundRequest.volume * MIX_MAX_VOLUME);
		const int effectiveVolume = m_Muted ? 0 : volume;

		if (pSound.isMusic)
		{
			if (pSound.music)
			{
				Mix_VolumeMusic(effectiveVolume);
				Mix_PlayMusic(pSound.music, 0);
			}
		}
		else {

			if (pSound.soundEffect) {
				Mix_VolumeChunk(pSound.soundEffect, effectiveVolume);
				Mix_PlayChannel(-1, pSound.soundEffect, 0);
			}
		}

	}
}

void Rinigin::SDLMixerSoundService::SDLpimpl::Play(const Rinigin::AudioRequest& request)
{
	std::lock_guard<std::mutex> lock(m_Mutex);
	m_SoundQueue.push(request); 
	m_ConditionVariable.notify_one();
}

void Rinigin::SDLMixerSoundService::SDLpimpl::SetMuted(bool mute)
{
	std::lock_guard<std::mutex> lock(m_MuteMutex);
	m_Muted = mute;

	for (auto& [_, audio] : m_LoadedAudio)
	{
		if (audio.isMusic && audio.music)
		{
			Mix_VolumeMusic(m_Muted ? 0 : static_cast<int>(audio.volume * MIX_MAX_VOLUME));
		}
		else if (audio.soundEffect)
		{
			Mix_VolumeChunk(audio.soundEffect, m_Muted ? 0 : static_cast<int>(audio.volume * MIX_MAX_VOLUME));
		}
	}
}


void Rinigin::SDLMixerSoundService::SDLpimpl::UnloadSounds()
{
	for (auto& sound : m_LoadedAudio)
	{
		if (sound.second.isMusic)
		{
			if (sound.second.music) {
				Mix_FreeMusic(sound.second.music);
			}
		}
		else
		{
			if (sound.second.soundEffect) {
				Mix_FreeChunk(sound.second.soundEffect);
			}
		}
	}

	m_LoadedAudio.clear();
}

// Main class

Rinigin::SDLMixerSoundService::SDLMixerSoundService(const std::string& path) :
	m_pimpl{ std::make_unique<SDLpimpl>(path) }
{
}

Rinigin::SDLMixerSoundService::~SDLMixerSoundService()
{
}


void Rinigin::SDLMixerSoundService::Play(const AudioRequest& request)
{
	m_pimpl->Play(request);
}

void Rinigin::SDLMixerSoundService::Stop(const AudioRequest& request)
{
	AudioRequest req{ request };
	req.stop = true;
	m_pimpl->Play(req);
}

void Rinigin::SDLMixerSoundService::SetMuted(bool muted)
{
	m_pimpl->SetMuted(muted);
}

bool Rinigin::SDLMixerSoundService::IsMuted() const
{
	return m_pimpl->IsMuted();
}

