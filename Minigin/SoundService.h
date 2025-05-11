#pragma once
#include <string>
#include <map>

namespace Rinigin {
	struct AudioRequest {
		std::string sound;
		float volume;
		bool isMusic;
	};

	class SoundService
	{
	public:
		virtual ~SoundService() = default;
		virtual void Play(const AudioRequest& request) = 0;
	};

	class NullSoundService final : public SoundService
	{ 
	public:
		virtual void Play(const AudioRequest& /*request*/) override {};
	};

}
