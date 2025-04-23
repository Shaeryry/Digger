#pragma once
#include <memory>
#include "SoundService.h"

namespace Rinigin {
	class ServiceLocator final
	{
	public:
		static void RegisterSoundSystem(std::unique_ptr<SoundService>&& soundSystem) {
			m_SoundService = soundSystem == nullptr ? std::make_unique<NullSoundService>() : std::move(soundSystem);
		};
		static SoundService& GetSoundService() { return *m_SoundService; }
	private:
		static std::unique_ptr<SoundService> m_SoundService; 
	}; 
}