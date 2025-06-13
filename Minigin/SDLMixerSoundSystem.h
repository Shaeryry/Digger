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
		virtual void Stop(const AudioRequest& request) override;
		virtual void SetMuted(bool muted) override;
		virtual bool IsMuted() const override;
	private:  
		class SDLpimpl;
		std::unique_ptr<SDLpimpl> m_pimpl; 
	}; 
}

  