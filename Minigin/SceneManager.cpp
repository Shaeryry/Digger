#include "SceneManager.h"
#include "Scene.h"

Rinigin::SceneManager::SceneManager() :
	m_ActiveScene(nullptr),
	m_ActiveSceneChangedEvent{ std::make_unique<Event>( NullEventArguments("SceneChanged") )},
	m_SceneCreatedEvent{ std::make_unique<Event>( NullEventArguments("SceneCreated") ) },
	m_SceneRemovedEvent{ std::make_unique<Event>( NullEventArguments("SceneRemoved") ) }
{
}

void Rinigin::SceneManager::Update()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	if (m_ActiveScene) {
		m_ActiveScene->Update();
	}
}

void Rinigin::SceneManager::FixedUpdate()
{
	if (m_ActiveScene) {
		m_ActiveScene->FixedUpdate();
	}
}

void Rinigin::SceneManager::LateUpdate()
{

	if (m_ActiveScene) {
		m_ActiveScene->LateUpdate();
	}
}

void Rinigin::SceneManager::Render() const
{

	if (m_ActiveScene) {
		m_ActiveScene->Render();
	}
}

Rinigin::Scene* Rinigin::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	Scene* newScene = scene.get();  
	m_Scenes.emplace_back(scene);

	m_SceneCreatedEvent->NotifyObservers();
	return newScene;
}


void Rinigin::SceneManager::RemoveScene(Scene* scene)
{
	if (scene) {
		if (scene == m_ActiveScene) {
			m_ActiveScene = nullptr; // Remove the active scene
		}

		m_Scenes.erase(
			std::remove_if(m_Scenes.begin(), m_Scenes.end(), [&](const std::shared_ptr<Scene>& currentScene)
				{
					return currentScene.get() == scene;
				}
			), m_Scenes.end());

		m_SceneRemovedEvent->NotifyObservers();
	}
} 

void Rinigin::SceneManager::SetActiveScene(Scene* sceneToLoad)
{
	m_ActiveScene = sceneToLoad;
	m_ActiveSceneChangedEvent->NotifyObservers();
}
