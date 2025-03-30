#include "Scene.h"
#include "GameObject.h"

#include <algorithm>

using namespace Rinigin;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name) {}

Scene::~Scene() = default;

std::unique_ptr<GameObject>& Scene::Add(std::unique_ptr<GameObject>& object)
{
	m_Objects.emplace_back(std::move(object));
	return m_Objects.back();
}

void Scene::Destroy(std::unique_ptr<GameObject>& object)
{
	m_Objects.erase(std::remove(m_Objects.begin(), m_Objects.end(), object), m_Objects.end());
}

void Scene::RemoveAll()
{
	m_Objects.clear();
}

void Rinigin::Scene::FixedUpdate()
{
	for (auto& object : m_Objects)
	{
		object->FixedUpdate();
	}
}

void Scene::Update()
{
	for(auto& object : m_Objects)
	{
		object->Update();
	}
}

void Rinigin::Scene::LateUpdate()
{
	for (auto& object : m_Objects)
	{
		object->LateUpdate();
	}

	// TODO : Delete objects marked for deletion
}

void Scene::Render() const
{
	for (const auto& object : m_Objects)
	{
		object->Render();
	}
}

