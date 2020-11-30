#pragma once
#include "hound/core/math/math.h"
#include "hound/core/object/object.h"

class renderer_cache : public object
{
public:
	static renderer_cache* get_instance() { return s_instance_; };
	
	typedef object_id shader_id;

	enum class shader_stage
	{
		VERTEX,
		GEOMETRY,
		TESSELLATION,
		FRAGMENT,
		COMPUTE
	};

	virtual shader_id shader_create() = 0;
	
	virtual void shader_set_source(shader_id shader, shader_stage stage, const std::string& source) = 0;
	virtual const std::string& shader_get_source(shader_id shader, shader_stage stage) = 0;
	virtual bool shader_finalize(shader_id shader) = 0;
	
	typedef object_id mesh_id;
	typedef uint32_t mesh_data_id;

	struct mesh_data
	{
		struct vertex
		{
			vec3_f position;
			vec3_f normal;
			vec2_f uv;
		};

		std::vector<vertex> vertices;		
		std::vector<uint32_t> indices;
	};

	virtual mesh_id mesh_create() = 0;
	virtual mesh_data_id mesh_add_data(mesh_id mesh, const mesh_data& data) = 0;
	virtual bool mesh_override_data(mesh_id mesh, mesh_data_id data_id, const mesh_data& data) = 0;

protected:
	static renderer_cache* s_instance_;
	
	renderer_cache();
	virtual ~renderer_cache();
};

#define RC renderer_cache
