#include "SceneManager.h"
#include "Scene.h"

void Rinigin::SceneManager::FixedUpdate()
{
	for (auto& scene : m_Scenes)
	{
		scene->FixedUpdate();
	}
}

void Rinigin::SceneManager::Update()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	for(auto& scene : m_Scenes)
	{
		scene->Update();
	}
}

void Rinigin::SceneManager::LateUpdate()
{
	for (auto& scene : m_Scenes)
	{
		scene->LateUpdate();
	}
}

void Rinigin::SceneManager::Render() const
{
	for (const auto& scene : m_Scenes)
	{
		scene->Render();
	}
}

Rinigin::Scene& Rinigin::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_Scenes.emplace_back(scene);
	return *scene;
}
