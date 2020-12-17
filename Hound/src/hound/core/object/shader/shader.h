#pragma once
#include "hound/core/object/object.h"
#include "hound/core/rendering/renderer_cache.h"

class shader : public object
{
public:
	HND_PROPERTY_READ_ONLY(name, std::string, renderer_cache::shader_cache()->get_shader_data(get_object_id()).name)

	virtual void use() = 0;	
	virtual void set_uniform_float(const char* u_name, float value) = 0;

protected:
	shader() = default;
	virtual ~shader() = default;
};
