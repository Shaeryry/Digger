#pragma once
#include "SceneManager.h"
#include "GameObject.h"

namespace Rinigin
{
	class Scene final
	{
	public:
		explicit Scene(const std::string& name);
		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		GameObject* CreateObject(GameObject* object = nullptr);
		GameObject* AddObject(std::unique_ptr<GameObject>& object);
		std::vector<GameObject*> GetObjects() const;
		void Destroy(std::unique_ptr<GameObject>& object); 
		void RemoveAll();

		void FixedUpdate(); 
		void Update(); 
		void LateUpdate();
		void Render() const;

		const std::string& GetName() const { return m_Name; };
		const unsigned int GetID() const { return m_SceneId; };
		// Events
	private: 		 
		std::string m_Name;
		unsigned int m_SceneId;
		std::vector < std::unique_ptr<GameObject> > m_Objects{};

		static unsigned int m_IdCounter; 
	};

}
