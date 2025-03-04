#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);

		void FixedUpdate(); 
		void Update();
		void LateUpdate();
		void Render() const;
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector< std::shared_ptr<Scene> > m_Scenes;
	};
}
