#pragma once
#include "hound/core/rendering/renderer_cache/cache_object_functions.h"

CACHED_OBJECT(shader, shader_cache_module)
{
	CACHED_OBJECT_DECL(shader, shader_cache_module)
	CACHED_OBJECT_CREATE_FUNC_DECL_NAMED_P1(shader, by_name, const char*, name)
	CACHED_OBJECT_CREATE_FUNC_DECL_NAMED_P1(shader, from_absolute_path, const char*, absolute_path)
	CACHED_OBJECT_CREATE_FUNC_DECL_NAMED_P1(shader, from_asset_path, const char*, asset_path)
	
public:
	HND_PROPERTY_READ_ONLY(name, std::string, m_name_)

	void use();	
	void set_uniform_float(const char* u_name, float value);
	
protected:
	std::string m_name_;
	
	shader() = default;
	virtual ~shader() = default;
};
