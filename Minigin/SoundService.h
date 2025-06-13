#pragma once
#include <string>
#include <map>

namespace Rinigin {
	struct AudioRequest {
		std::string sound;
		float volume;
		bool isMusic = false;
		bool stop = false;
	};

	class SoundService
	{
	public:
		virtual ~SoundService() = default;
		virtual void Play(const AudioRequest& request) = 0;
		virtual void Stop(const AudioRequest& request) = 0;
		virtual void SetMuted(bool muted) = 0;
		virtual bool IsMuted() const = 0;
	};

	class NullSoundService final : public SoundService
	{ 
	public:
		virtual void Play(const AudioRequest& /*request*/) override {};
		virtual void Stop(const AudioRequest& /*request*/) override {};
		virtual void SetMuted(bool /*muted*/) override {};
		virtual bool IsMuted() const override { return false; };
	};

}
