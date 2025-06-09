#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <utility> // for std::forward

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

	void ClearTracked()
	{
		m_Instances.clear();
	}

	const std::vector< std::unique_ptr<T>>& GetInstances() { return m_Instances; }
private:
	std::unordered_map<unsigned int, std::unique_ptr<T>> m_Prototypes;
	std::vector<std::unique_ptr<T>> m_Instances;
};
