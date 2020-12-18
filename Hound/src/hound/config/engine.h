#pragma once
#include "hound/logging/logger.h"
#include "hound/core/assertion.h"
#include "hound/core/object/object.h"
#include "hound/core/object/object_database.h"
class window;

class engine
{
public:
	static void init();
	static void clean_up();
	
	template<typename T>
	static void register_singleton();

	template<typename T, typename TImpl>
	static TImpl* register_singleton();

	template<typename T, typename TImpl>
	static TImpl* get_singleton();
	
private:
	static engine* get_instance() { return s_instance_; }
	
	static engine* s_instance_;
	static bool s_initialized_;
	
	std::unordered_map<const char*, object*> m_singletons_;
};

template <typename T>
void engine::register_singleton()
{
	const char* id = typeid(T).name();

	HND_CORE_ASSERT(s_initialized_, "Engine not initialized!")
	if (s_instance_->m_singletons_.find(id) != s_instance_->m_singletons_.end())
	{
		HND_CORE_LOG_ERROR("Singleton already registered!", id);
		return;
	}
	HND_CORE_LOG_TRACE("Registered new singleton: ", id);

	T* instance = object_database::get_instance()->create_object_instance<T>();
	
	s_instance_->m_singletons_[id] = instance;
}

template <typename T, typename TImpl>
TImpl* engine::register_singleton()
{
	const char* id = typeid(T).name();
	
	HND_CORE_ASSERT(s_initialized_, "Engine not initialized!")

	if(s_instance_->m_singletons_.find(id) != s_instance_->m_singletons_.end())
	{
		HND_CORE_LOG_ERROR("Singleton already registered!", id);
	}
	HND_CORE_LOG_TRACE("Registered new singleton: ", id, " \r\n\t- Impl: ", typeid(TImpl).name());

	TImpl* instance = object_database::get_instance()->create_object_instance<TImpl>();
		
	s_instance_->m_singletons_[id] = instance;

	return instance;
}

template <typename T, typename TImpl>
TImpl* engine::get_singleton()
{
	const char* id = typeid(T).name();

	HND_CORE_ASSERT(s_initialized_, "Engine not initialized!")
	if (s_instance_->m_singletons_.find(id) == s_instance_->m_singletons_.end())
	{
		HND_CORE_LOG_ERROR("Singleton not registered!", id);
		return nullptr;
	}
	return reinterpret_cast<TImpl*>(s_instance_->m_singletons_[id]);
}

