#pragma once
#include "hound/core/base.h"
#include "hound/core/object/object_database.h"
#include "hound/core/object/object.h"
#include "hound/core/rendering/renderer_resource_id.h"

class renderer_cache_module : public object
{
	template<typename TTargetType>
	TTargetType* get_target_object(object_id id);
};


template <typename TTargetType>
TTargetType* renderer_cache_module::get_target_object(object_id id)
{
	return object_database::get_object_instance<TTargetType>(id);
}

#define HND_RENDER_CACHE_CLASS_DECL(Type, ObjectType)\
	class ObjectType;\
	class Type : public renderer_cache_module


#define HND_RENDER_CACHE_BASE_FUNC_DECL(Type, ObjectType)\
	friend class ObjectType;\
	public:\
		ObjectType* create_##ObjectType();\
		\
		ObjectType* get_##ObjectType_instance(resource_id id);\
		\
		virtual void on_create_instance(ObjectType* instance) = 0;\
		\
		static Type* get_instance() { return s_instance_; };\
		\
	private:\
		static Type* s_instance_;

#define HND_RENDER_CACHE_BASE_FUNC_IMPL(Type, ObjectType, DerivedObject)\
	Type* Type::s_instance_ = nullptr;\
	\
	ObjectType* Type::create_##ObjectType()\
	{\
		ObjectType* instance = object_database::get_instance()->create_object_instance<ObjectType>();\
		on_create_instance(instance);\
		return instance;\
	};\
	\
	ObjectType* Type::get_##ObjectType_instance(resource_id id)\
	{\
		ObjectType* instance = object_database::get_instance()->get_object_instance<ObjectType>(id);\
		return instance;\
	};\
