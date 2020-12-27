#include "hound/hnd_pch.h"
#include "shader.h"
#include "hound/core/rendering/renderer_cache/module/shader_cache_module.h"

GET_CACHE_FUNC_IMPL(shader, shader_cache_module)
CACHED_OBJECT_CREATE_FUNC_IMPL_NAMED_P1(shader, shader_cache_module, by_name, const char*, name)
CACHED_OBJECT_CREATE_FUNC_IMPL_NAMED_P1(shader, shader_cache_module, from_absolute_path, const char*, absolute_path)
CACHED_OBJECT_CREATE_FUNC_IMPL_NAMED_P1(shader, shader_cache_module, from_asset_path, const char*, asset_path)
CACHED_OBJECT_DEFAULT_INIT_FUNC_IMPL_NAMED_P1(shader, shader_cache_module, by_name, const char*, name)
CACHED_OBJECT_DEFAULT_INIT_FUNC_IMPL_NAMED_P1(shader, shader_cache_module, from_absolute_path, const char*, absolute_path)
CACHED_OBJECT_DEFAULT_INIT_FUNC_IMPL_NAMED_P1(shader, shader_cache_module, from_asset_path, const char*, asset_path)

void shader::use()
{
	get_cache()->use_shader(get_object_id());
}

void shader::set_uniform_float(const char* u_name, float value)
{
	get_cache()->set_uniform_float(get_object_id(), u_name, value);
}

void shader::set_uniform_mat4_f(const char* u_name, const mat4_f& value)
{
	get_cache()->set_uniform_mat4_f(get_object_id(), u_name, value);
}

const std::string& shader::get_name()
{
	return get_cache()->get_shader_data(get_object_id()).name;
}
