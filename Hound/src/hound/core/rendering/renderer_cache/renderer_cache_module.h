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

// #define HND_RENDER_CACHE_CLASS_DECL(Type, ObjectType)\
// 	class ObjectType;\
// 	class Type : public renderer_cache_module
//
//
// #define HND_RENDER_CACHE_BASE_FUNC_DECL(Type)\
// 	friend class ObjectType;\
// 	public:\
// 		\
// 		static Type* get_instance() { return s_instance_; };\
// 		\
// 	protected:\
// 		static Type* s_instance_;
//
// #define HND_RENDER_CACHE_BASE_FUNC_IMPL(ModuleType)\
// 		ModuleType* ModuleType::s_instance_ = nullptr;
//
// #define HND_RENDER_CACHE_GET_FUNC_DECL(ObjectType)\
// 	ObjectType* get_##ObjectType##_instance(resource_id id);
//
// #define HND_RENDER_CACHE_GET_FUNC_IMPL(ModuleType, ObjectType)\
// 	ObjectType* ModuleType::get_##ObjectType##_instance(resource_id id)\
// 	{\
// 		ObjectType* instance = object_database::get_object_instance<ObjectType>(id);\
// 		if(!instance)\
// 		{\
// 			HND_CORE_LOG_WARN("Could not find instace of ", #ObjectType, " returning nullptr");\
// 		}\
// 		return instance;\
// 	}
//
// #define HND_RENDER_CACHE_DEFAULT_DATA_STRUCT_DECL(ObjectType)\
// 	protected:\
// 		struct internal_##ObjectType##_data\
// 		{\
// 			resource_id id;\
// 			ObjectType* handle;\
// 		};\
// 	private:
//
// #define HND_RENDER_CACHE_FRIEND_DECL(ObjectType)\
// 	friend class ObjectType;
//
// #define HND_RENDER_CACHE_CREATE_FUNC_SIG_DECL(ObjectType, FuncName, ...) ObjectType* create_##FuncName(__VA_ARGS__);
//
// #define HND_RENDER_CACHE_CREATE_FUNC_SIG_IMPL(ModuleType, ObjectType, FuncName, ...) ObjectType* ModuleType::create_##FuncName()
//
// #define HND_RENDER_CACHE_CREATE_FUNC_DECL(ObjectType)\
// 	HND_RENDER_CACHE_FRIEND_DECL(ObjectType)\
// 	protected:\
// 		HND_RENDER_CACHE_CREATE_FUNC_SIG_DECL(ObjectType, ObjectType)\
// 		\
// 		virtual void on_create_instance(ObjectType* instance) = 0;\
// 	private:
//
// #define HND_RENDER_CACHE_CREATE_FUNC_IMPL(ModuleType, ObjectType)\
// 	HND_RENDER_CACHE_CREATE_FUNC_SIG_IMPL(ModuleType, ObjectType)\
// 	{\
// 		ObjectType* instance = object_database::create_object_instance<ObjectType>();\
// 		on_create_instance(instance);\
// 		return instance;\
// 	};
//
// #define HND_RENDER_CACHE_CREATE_FUNC_DECL_1(ObjectType, Param_1, Param_1_name)\
// 	HND_RENDER_CACHE_FRIEND_DECL(ObjectType)\
// 	protected:\
// 		ObjectType* create_##ObjectType##_##Param_1_name(Param_1 param_1);\
// 		\
// 		virtual void on_create_instance_##Param_1_name(ObjectType* instance, Param_1 Param_1_name) = 0;\
// 	private:
//
// #define HND_RENDER_CACHE_CREATE_FUNC_IMPL_1(ModuleType, ObjectType, Param_1, Param_1_name)\
// 	ObjectType* ModuleType::create_##ObjectType##_##Param_1_name(Param_1 param_1)\
// 	{\
// 		ObjectType* instance = object_database::create_object_instance<ObjectType>();\
// 		on_create_instance_##Param_1_name(instance, param_1);\
// 		return instance;\
// 	}
