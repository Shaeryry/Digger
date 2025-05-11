#pragma once
#include <memory>
#include "SoundService.h"

namespace Rinigin { 
	class SDLMixerSoundService final : public SoundService
	{
	public: 
		SDLMixerSoundService(const std::string& path);
		~SDLMixerSoundService() override;
		virtual void Play(const AudioRequest& request) override;
	private:  
		class SDLpimpl;
		std::unique_ptr<SDLpimpl> m_pimpl; 
	}; 
}

  