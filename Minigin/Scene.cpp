#include "Scene.h"
#include "GameObject.h"
#include "Helpers.h"

#include <algorithm>
using namespace Rinigin;

Scene::Scene(const std::string& name) :
	m_Name(name),
	m_SceneId(Helpers::sdbm_hash(name.c_str()))
{
}

Scene::~Scene() = default;

GameObject* Rinigin::Scene::AddObject(std::unique_ptr<GameObject>& object)
{
	m_Objects.emplace_back(std::move(object));
	return m_Objects.back().get();
}

std::vector<GameObject*> Rinigin::Scene::GetObjects() const
{
	std::vector<GameObject*> objects{};
	for (auto& object : m_Objects) objects.emplace_back(object.get());

	return objects;
}

GameObject* Rinigin::Scene::CreateObject(GameObject* object)
{
	std::unique_ptr<GameObject> addedObject = std::make_unique<GameObject>(object);
	m_Objects.emplace_back(std::move(addedObject));
	return m_Objects.back().get();
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

void Rinigin::Scene::Update()
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
}

void Rinigin::Scene::Render() const
{
	for (const auto& object : m_Objects)
	{
		object->Render();
	}
}

