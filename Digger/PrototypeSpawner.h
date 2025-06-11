#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <utility>

#include "Helpers.h"

template<typename T>
class PrototypeSpawner final
{
public:
	virtual ~PrototypeSpawner() = default;

	template<typename PT = T,typename... Args>
	void RegisterPrototype(const char* id, Args&&... args)
	{
		m_Prototypes[Rinigin::Helpers::sdbm_hash(id)] = std::make_unique<PT>(std::forward<Args>(args)...);
	}

	T* Spawn(const char* id)
	{
		auto it = m_Prototypes.find(Rinigin::Helpers::sdbm_hash(id));
		if (it != m_Prototypes.end() && it->second)
		{
			std::unique_ptr<T> clone( it->second->Clone() );
			m_Instances.emplace_back(std::move(clone));
	
			return m_Instances.back().get();
		}
		return nullptr;
	}

	void RemoveTracked(T* trackedInstance) {
		//std::erase(m_Instances, trackedInstance);
		std::erase_if(m_Instances, [trackedInstance](const std::unique_ptr<T>& pointer) {
				return pointer.get() == trackedInstance;
			}	
		);
	}

	void ClearTracked()
	{
		if (m_Instances.empty()) return;
		m_Instances.clear();
	}

	const std::vector< std::unique_ptr<T>>& GetInstances() { return m_Instances; }
private:
	std::unordered_map<unsigned int, std::unique_ptr<T>> m_Prototypes;
	std::vector<std::unique_ptr<T>> m_Instances;
};
