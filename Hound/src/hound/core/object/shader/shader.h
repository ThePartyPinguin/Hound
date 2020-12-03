#pragma once
#include "hound/core/object/object.h"

class shader : public object
{
public:
	enum class stage
	{
		INVALID,
		VERTEX,
		GEOMETRY,
		TESSELLATION,
		FRAGMENT,
		COMPUTE
	};

	virtual void use() = 0;	
	virtual void set_uniform_float(const char* u_name, float value) = 0;

protected:
	shader() = default;
	virtual ~shader() = default;
};
