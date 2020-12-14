#pragma once
#include "hound/core/rendering/render_target/frame_buffer.h"
#include "hound/core/object/texture/texture.h"

class open_gl_frame_buffer : public frame_buffer
{
public:
	open_gl_frame_buffer();
	virtual ~open_gl_frame_buffer();
	
protected:
	friend class open_gl_renderer_cache;
	
	void create() override;
	void bind() override;
	void un_bind() override;
	void on_set_size(const vec2_i& size) override;

private:
	object_id m_render_texture_id_;
};

