#pragma once
#include "hound/core/object/object_database.h"

#ifndef CHACHE_MODULE_FUNCTIONS
#define CHACHE_MODULE_FUNCTIONS

#define RENDER_CACHE_CLASS(Type, ObjectType)\
	class ObjectType;\
	class Type : public renderer_cache_module

#define RENDER_CACHE_FRIEND_DECL(ObjectType)\
	friend class ObjectType;

#define RENDER_CACHE_SINGLETON_DECL(Type)\
	protected:\
		inline static Type* get_instance() {return s_instance_;};\
		static Type* s_instance_;\
	private:

#define RENDER_CACHE_SINGLETON_IMPL(Type)\
	Type* Type::s_instance_ = nullptr;

#define RENDER_CACHE_GET_INSTANCE_FUNC_DECL(Type)\
	Type* get_##Type##_instance(resource_id id);

#define RENDER_CACHE_CLASS_DECL(Type, ObjectType)\
	RENDER_CACHE_FRIEND_DECL(ObjectType) RENDER_CACHE_SINGLETON_DECL(Type)

#define RENDER_CACHE_CLASS_IMPL(Type, ObjectType)\
	RENDER_CACHE_SINGLETON_IMPL(Type)

#define HND_RENDER_CACHE_GET_FUNC_IMPL(ModuleType, Type)\
	Type* ModuleType::get_##Type##_instance(resource_id id)\
	{\
		Type* instance = object_database::get_object_instance<Type>(id);\
		if(!instance)\
		{\
			HND_CORE_LOG_WARN("Could not find instace of ", #Type, " returning nullptr");\
		}\
		return instance;\
	}

#define RENDER_CACHE_DEFAULT_DATA_STRUCT_DECL(ObjectType)\
	protected:\
		struct internal_##ObjectType##_data\
		{\
			resource_id id;\
			ObjectType* handle;\
		};\
	private:

#define RENDER_CACHE_GET_FUNC_DECL(ObjectType)\
	protected:\
		virtual ObjectType* get_##ObjectType() = 0;\
	private:

#define RENDER_CACHE_CREATE_FUNC_DECL(ObjectType)\
	protected:\
		ObjectType* create_##ObjectType();\
	protected:\
		virtual void on_create_instance(ObjectType* instance) = 0;\
	private:

#define RENDER_CACHE_CREATE_FUNC_IMPL(ObjectType, CacheModule)\
	ObjectType* CacheModule::create_##ObjectType()\
	{\
		return object_database::create_object_instance<ObjectType>();\
	}

#define RENDER_CACHE_CREATE_FUNC_DECL_P1(ObjectType, P1, P1_Name)\
	public:\
		ObjectType* create_##ObjectType(P1 P1_Name);\
	protected:\
		virtual void on_create_instance(ObjectType* instance, P1 P1_Name) = 0;\
	private:

#define RENDER_CACHE_CREATE_FUNC_IMPL_P1(ObjectType, CacheModule, P1, P1_Name)\
	ObjectType* CacheModule::create_##ObjectType(P1 P1_Name)\
	{\
		return object_database::create_object_instance<ObjectType>();\
	}

#define RENDER_CACHE_CREATE_FUNC_DECL_NAMED(ObjectType, FuncName)\
	public:\
		ObjectType* create_##FuncName();\
	protected:\
		virtual void on_create_instance_##FuncName(ObjectType* instance) = 0;\
	private:

#define RENDER_CACHE_CREATE_FUNC_IMPL_NAMED(ObjectType, CacheModule, FuncName)\
	ObjectType* CacheModule::create_##FuncName()\
	{\
		return object_database::create_object_instance<ObjectType>();\
	}

#define RENDER_CACHE_CREATE_FUNC_DECL_NAMED_P1(ObjectType, FuncName, P1, P1_Name)\
	public:\
		ObjectType* create_##FuncName(P1 P1_Name);\
	protected:\
		virtual void on_create_instance_##FuncName(ObjectType* instance, P1 P1_Name) = 0;\
	private:

#define RENDER_CACHE_CREATE_FUNC_IMPL_NAMED_P1(ObjectType, CacheModule, FuncName, P1, P1_Name)\
	ObjectType* CacheModule::create_##FuncName(P1 P1_Name)\
	{\
		return object_database::create_object_instance<ObjectType>();\
	}

#define RENDER_CACHE_CREATE_FUNC_DECL_NAMED_P2(ObjectType, FuncName, P1, P1_Name, P2, P2_Name)\
	public:\
		ObjectType* create_##FuncName(P1 P1_Name, P2 P2_Name);\
	protected:\
		virtual void on_create_instance_##FuncName(ObjectType* instance, P1 P1_Name, P2 P2_Name) = 0;\
	private:

#define RENDER_CACHE_CREATE_FUNC_IMPL_NAMED_P2(ObjectType, CacheModule, FuncName, P1, P1_Name, P2, P2_Name)\
	ObjectType* CacheModule::create_##FuncName(P1 P1_Name, P2 P2_Name)\
	{\
		return object_database::create_object_instance<ObjectType>();\
	}

#endif
