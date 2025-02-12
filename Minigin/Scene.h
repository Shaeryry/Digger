#pragma once
#include "SceneManager.h"
#include "GameObject.h"

namespace dae
{
	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void Add(std::unique_ptr<GameObject>& object);
		void Destroy(std::unique_ptr<GameObject>& object);
		void RemoveAll();

		void FixedUpdate();
		void Update();
		void LateUpdate();
		void Render() const;

		~Scene(); 
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;
	private: 
		explicit Scene(const std::string& name);

		std::string m_Name;
		std::vector < std::unique_ptr<GameObject> > m_Objects{};

		static unsigned int m_IdCounter; 
	};

}
