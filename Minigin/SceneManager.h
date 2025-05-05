#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

#include "Event.h"


namespace Rinigin
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene* CreateScene(const std::string& name);
		void RemoveScene(Scene* scene);

		void SetActiveScene(Scene* sceneToLoad);
		Scene* GetActiveScene() const { return m_ActiveScene; };

		void FixedUpdate(); 
		void Update();
		void LateUpdate();
		void Render() const;

		// Events
		Event* GetSceneCreatedEvent() const { return m_SceneCreatedEvent.get(); };
		Event* GetSceneRemovedEvent() const { return m_SceneRemovedEvent.get(); };
		Event* GetActiveSceneChangedEvent() const { return m_ActiveSceneChangedEvent.get(); };
	private:
		friend class Singleton<SceneManager>;  
		SceneManager();

		std::vector< std::shared_ptr<Scene> > m_Scenes;
		Scene* m_ActiveScene;

		std::unique_ptr<Event> m_SceneCreatedEvent;
		std::unique_ptr<Event> m_SceneRemovedEvent;
		std::unique_ptr<Event> m_ActiveSceneChangedEvent;
	};
}
