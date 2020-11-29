#include "hound/hnd_pch.h"
#include "object_database.h"

object_database* object_database::s_instance_ = nullptr;

void object_database::destroy_object_instance(object* instance)
{
	if(instance == nullptr)
		return;
	
	if(m_object_map_.count(instance->get_object_id()))
	{
		m_object_map_.erase(instance->get_object_id());
		delete instance;
	}
}

void object_database::destroy_object(object_id id)
{
	if (m_object_map_.count(id))
	{
		object* instance = m_object_map_[id];
		m_object_map_.erase(id);
		delete instance;
	}
}

void object_database::init()
{
	if(!s_instance_)
	{
		s_instance_ = new object_database();
	}
}

void object_database::clean_up()
{
	for (auto& pair : s_instance_->m_object_map_)
	{
		delete pair.second;
	}
	
	delete s_instance_;
}
