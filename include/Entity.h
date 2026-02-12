#pragma once

#include "Components.h"
#include <memory>
#include <string>
#include <tuple>

using ComponentTuple = std::tuple<
	CTransform,
	CShape,
	CCollision,
	CInput,
	CScore,
	CLifespan
>;	

class Entity
{
	friend class EntityManager;

	bool		m_active	= true;
	size_t		m_id		= 0;
	std::string	m_tag		= "default";
	ComponentTuple m_components;
	

	//constructor and destructor
	Entity(const size_t id, const std::string& tag);


public:

	//component pointers
	/*std::shared_ptr<CTransform>	 cTransform;
	std::shared_ptr<CShape>		 cShape;
	std::shared_ptr<CCollision>	 cCollision;
	std::shared_ptr<CInput>		 cInput;
	std::shared_ptr<CScore>		 cScore;
	std::shared_ptr<CLifespan>	 cLifespan;*/

	template <typename T>
	bool has()	const
	{
		return get<T>().exists;
	}

	template <typename T, typename... TArgs>
	T& add(TArgs&&... mArgs)
	{
		auto& component = get<T>();
		component = T(std::forward<TArgs>(mArgs)...);
		component.exists = true;
		return component;
	}

	template <typename T>
	T& get()
	{
		return std::get<T>(m_components);
	}

	template <typename T>
	const T& get() const
	{
		return std::get<T>(m_components);
	}

	template <typename T>
	void remove()
	{
		get<T>() = T();
	}

	//member access functions
	bool isActive() const;
	const std::string& tag() const;
	const size_t id() const;
	void destroy();

	
};