#pragma once
#include "hound/core/object/object.h"

class object_database
{
public:
	friend class engine;
	
	static object_database* get_instance() { return s_instance_; }

	template<typename TObject>
	TObject* create_object_instance();

	template<typename TObject>
	TObject* get_object_instance(object_id id);

	
	void destroy_object_instance(object* instance);

	void destroy_object(object_id id);
	
private:
	static void init();
	static void clean_up();
	
	static object_database* s_instance_;

	std::unordered_map<object_id, object*> m_object_map_;
};

template <typename TObject>
TObject* object_database::create_object_instance()
{
	object_id id(object_id::m_object_counter_++);
	TObject* instance = new TObject();
	
	object* o_instance = static_cast<object*>(instance);
	o_instance->set_object_id(id);
	m_object_map_.insert(std::make_pair(id, o_instance));

	return instance;
}

template <typename TObject>
TObject* object_database::get_object_instance(object_id id)
{
	if(m_object_map_[id])
	{
		return static_cast<TObject*>(id);
	}
	return nullptr;
}
