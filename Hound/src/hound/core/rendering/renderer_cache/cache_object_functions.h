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
	

#define CACHED_OBJECT_CREATE_FUNC_DECL(Type)\
	public:\
		static Type* create();\
	private:

#define CACHED_OBJECT_CREATE_FUNC_IMPL(Type, CacheModule)\
	Type* Type::create()\
	{\
		return CacheModule::get_instance()->create_##Type();\
	}

#define CACHED_OBJECT_CREATE_FUNC_DECL_P1(Type, P1, P1_Name)\
	public:\
		static Type* create(P1 P1_Name);\
	private:

#define CACHED_OBJECT_CREATE_FUNC_IMPL_P1(Type, CacheModule, P1, P1_Name)\
	Type* Type::create(P1 P1_Name)\
	{\
		return CacheModule::get_instance()->create_##Type(P1_Name);\
	}

#define CACHED_OBJECT_CREATE_FUNC_DECL_NAMED(Type, FuncName)\
	public:\
		static Type* create_##FuncName();\
	private:

#define CACHED_OBJECT_CREATE_FUNC_IMPL_NAMED(Type, CacheModule, FuncName)\
	Type* Type::create_##FuncName()\
	{\
		return CacheModule::get_instance()->create_##FuncName();\
	}

#define CACHED_OBJECT_CREATE_FUNC_DECL_NAMED_P1(Type, FuncName, P1, P1_Name)\
	public:\
		static Type* create_##FuncName(P1 P1_Name);\
	private:

#define CACHED_OBJECT_CREATE_FUNC_IMPL_NAMED_P1(Type, CacheModule, FuncName, P1, P1_Name)\
	Type* Type::create_##FuncName(P1 P1_Name)\
	{\
		return CacheModule::get_instance()->create_##FuncName(P1_Name);\
	}

#endif
