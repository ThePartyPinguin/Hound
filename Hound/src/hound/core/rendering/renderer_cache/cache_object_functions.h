#pragma once
#include "hound/core/object/object.h"

#ifndef CHACHE_OBJECT_FUNCTIONS
#define CHACHE_OBJECT_FUNCTIONS

#define CACHED_OBJECT(type, cache_module)\
	class cache_module;\
	class type : public object

#define CACHED_DERIVED_OBJECT(type, base, cache_module)\
	class cache_module;\
	class type : public base

#define OBJECT_CACHE_FRIEND_CLASS(CacheModule)\
	friend class CacheModule;\
	friend class object_database;

#define GET_CACHE_FUNC_DECL(CacheModule)\
	protected:\
		CacheModule* get_cache();\
	private:

#define GET_CACHE_FUNC_IMPL(Type, CacheModule)\
	CacheModule* Type::get_cache()\
	{\
		return CacheModule::get_instance();\
	}

#define CACHED_OBJECT_DECL(Type, CacheModule)\
	OBJECT_CACHE_FRIEND_CLASS(CacheModule)	GET_CACHE_FUNC_DECL(CacheModule)

// --------------------------------------
// ------ 0 Param create function -------
// --------------------------------------

#define CACHED_OBJECT_CREATE_FUNC_DECL(Type)\
	public:\
		static Type* create();\
	private:\
		void init();

#define CACHED_OBJECT_CREATE_FUNC_IMPL(Type, CacheModule)\
	Type* Type::create()\
	{\
		Type* instance = CacheModule::get_instance()->create_##Type();\
		instance->init();\
		CacheModule::get_instance()->on_create_instance(instance);\
		return instance;\
	}

#define CACHED_OBJECT_CREATE_FUNC_DECL_NAMED(Type, FuncName)\
	public:\
		static Type* create_##FuncName();\
	private:\
		void init_##FuncName();

#define CACHED_OBJECT_CREATE_FUNC_IMPL_NAMED(Type, CacheModule, FuncName)\
	Type* Type::create_##FuncName()\
	{\
		Type* instance = CacheModule::get_instance()->create_##FuncName();\
		instance->init_##FuncName();\
		CacheModule::get_instance()->on_create_instance_##FuncName(instance);\
		return instance;\
	}

// 0 param init function
#define CACHED_OBJECT_DEFAULT_INIT_FUNC_IMPL(Type, CacheModule)\
	void Type::init(){}

#define CACHED_OBJECT_DEFAULT_INIT_FUNC_IMPL_NAMED(Type, CacheModule, FuncName)\
	void Type::init_##FuncName(){}

// --------------------------------------
// ------ 1 Param create function -------
// --------------------------------------
#define CACHED_OBJECT_CREATE_FUNC_DECL_P1(Type, P1, P1_Name)\
	public:\
		static Type* create(P1 P1_Name);\
	private:\
		void init(P1 P1_Name);

#define CACHED_OBJECT_CREATE_FUNC_IMPL_P1(Type, CacheModule, P1, P1_Name)\
	Type* Type::create(P1 P1_Name)\
	{\
		Type* instance = CacheModule::get_instance()->create_##Type(P1_Name);\
		instance->init(P1_Name);\
		CacheModule::get_instance()->on_create_instance(instance, P1_Name);\
		return instance;\
	}

#define CACHED_OBJECT_CREATE_FUNC_DECL_NAMED_P1(Type, FuncName, P1, P1_Name)\
	public:\
		static Type* create_##FuncName(P1 P1_Name);\
	private:\
		void init_##FuncName(P1 P1_Name);

#define CACHED_OBJECT_CREATE_FUNC_IMPL_NAMED_P1(Type, CacheModule, FuncName, P1, P1_Name)\
	Type* Type::create_##FuncName(P1 P1_Name)\
	{\
		Type* instance = CacheModule::get_instance()->create_##FuncName(P1_Name);\
		instance->init_##FuncName(P1_Name);\
		CacheModule::get_instance()->on_create_instance_##FuncName(instance, P1_Name);\
		return instance;\
	}

// 1 param init function
#define CACHED_OBJECT_DEFAULT_INIT_FUNC_IMPL_P1(Type, CacheModule, P1, P1_Name)\
	void Type::init(P1 P1_Name){}

#define CACHED_OBJECT_DEFAULT_INIT_FUNC_IMPL_NAMED_P1(Type, CacheModule, FuncName, P1, P1_Name)\
	void Type::init_##FuncName(P1 P1_Name){}

#define CACHED_OBJECT_CREATE_FUNC_DECL_P2(Type, P1, P1_Name, P2, P2_Name)\
	public:\
		static Type* create(P1 P1_Name, P2 P2_Name);\
	private:\
		void init(P1 P1_Name, P2 P2_Name);

// --------------------------------------
// ------ 2 Param create function -------
// --------------------------------------

#define CACHED_OBJECT_CREATE_FUNC_IMPL_P2(Type, CacheModule, P1, P1_Name, P2, P2_Name)\
	Type* Type::create(P1 P1_Name, P2 P2_Name)\
	{\
		Type* instance = CacheModule::get_instance()->create_##Type(P1_Name, P2_Name);\
		instance->init(P1_Name, P2_Name);\
		CacheModule::get_instance()->on_create_instance(instance, P1_Name, P2_Name);\
		return instance;\
	}

#define CACHED_OBJECT_CREATE_FUNC_DECL_NAMED_P2(Type, FuncName, P1, P1_Name, P2, P2_Name)\
	public:\
		static Type* create_##FuncName(P1 P1_Name, P2 P2_Name);\
	private:\
		void init_##FuncName(P1 P1_Name, P2 P2_Name);

#define CACHED_OBJECT_CREATE_FUNC_IMPL_NAMED_P2(Type, CacheModule, FuncName, P1, P1_Name, P2, P2_Name)\
	Type* Type::create_##FuncName(P1 P1_Name, P2 P2_Name)\
	{\
		Type* instance = CacheModule::get_instance()->create_##FuncName(P1_Name, P2_Name);\
		instance->init_##FuncName(P1_Name, P2_Name);\
		CacheModule::get_instance()->on_create_instance_##FuncName(instance, P1_Name, P2_Name);\
		return instance;\
	}

// 2 param init function
#define CACHED_OBJECT_DEFAULT_INIT_FUNC_IMPL_P2(Type, CacheModule, P1, P1_Name, P2, P2_Name)\
	void Type::init(P1 P1_Name, P2 P2_Name){}

#define CACHED_OBJECT_DEFAULT_INIT_FUNC_IMPL_NAMED_P2(Type, CacheModule, FuncName, P1, P1_Name, P2, P2_Name)\
	void Type::init_##FuncName(P1 P1_Name, P2 P2_Name)

#endif
