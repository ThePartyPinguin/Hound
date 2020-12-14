#pragma once
#include "hound/platform/open_gl/open_gl_core.h"
#include "hound/core/object/shader/shader.h"

class open_gl_shader : public shader
{
public:
	friend class open_gl_shader_cache_module;

	HND_PROPERTY_READ_ONLY(shader_program_id, gl_object_id, m_program_id_)

	void use() override;
	void set_uniform_float(const char* u_name, float value) override;

	open_gl_shader() = default;
	virtual ~open_gl_shader();
	
private:
	HND_PROPERTY_WRITE_ONLY(shader_program_id, gl_object_id, m_program_id_)
	
	gl_object_id m_program_id_;
};
