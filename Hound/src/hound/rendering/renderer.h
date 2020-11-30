#pragma once
#include "hound/core/object/object.h"
#include "hound/rendering/renderer_cache.h"

class renderer : public object
{
public:
	
	static renderer* get_instance();

	HND_PROPERTY(clear_color, vec4_f, m_clear_color_)
	
	enum class type_api
	{
		INVALID = -1,
		OPEN_GL = 1
	};

	virtual type_api get_api_type();

	virtual void begin_frame() = 0;	
	virtual void render_indexed(RC::shader_id shader, RC::mesh_id mesh) = 0;
	virtual void end_frame() = 0;
	
protected:
	static renderer* s_instance_;
	
	renderer();
	virtual ~renderer();

private:
	vec4_f m_clear_color_;
};

renderer* initialize_renderer();
