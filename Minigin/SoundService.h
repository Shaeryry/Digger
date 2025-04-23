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
		virtual void Init(const std::string& path) = 0;
		virtual void Play(const AudioRequest& request) = 0;
	};

	class NullSoundService final : public SoundService
	{ 
	public:
		void Init(const std::string& /*path*/ = "") override {};
		virtual void Play(const AudioRequest& /*request*/) override {};
	};

}
