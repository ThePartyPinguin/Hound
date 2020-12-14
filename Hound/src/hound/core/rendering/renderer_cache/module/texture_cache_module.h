#pragma once
#include "hound/core/bit.h"
#include "hound/core/object/object.h"
#include "hound/core/rendering/renderer_cache/module/renderer_cache_module.h"
#include "hound/core/rendering/renderer_resource_id.h"

class texture_cache_module : public renderer_cache_module
{
public:
	enum texture_type
	{
		TEXTURE_INVALID = BIT(0),
		TEXTURE_TYPE_1D = BIT(1),
		TEXTURE_TYPE_2D = BIT(2),
		TEXTURE_TYPE_3D = BIT(3),
		TEXTURE_TYPE_CUBE = BIT(4),
		TEXTURE_TYPE_1D_ARRAY = BIT(5),
		TEXTURE_TYPE_2D_ARRAY = BIT(6),
		TEXTURE_TYPE_CUBE_ARRAY = BIT(7),
	};

	enum texture_filter_mode
	{
		TEXTURE_FILTER_LINEAR,
		TEXTURE_FILTER_NEAREST
	};

	enum texture_wrap_mode
	{
		TEXTURE_WRAP_CLAMP_TO_EDGE,
		TEXTURE_WRAP_CLAMP_TO_BORDER,
		TEXTURE_WRAP_REPEAT,
		TEXTURE_WRAP_MIRRORED_REPEAT,
		TEXTURE_WRAP_MIRROR_CLAMP_TO_EDGE,
	};

	struct texture_data
	{
		vec2_i size;
		bool has_mip_map;
		unsigned char* data;
		texture_type type;
		texture_filter_mode filter_mode;
		texture_wrap_mode wrap_vertical;
		texture_wrap_mode wrap_horizontal;
	};

	virtual texture_id texture_create_2d(const vec2_i& size) = 0;
	virtual void texture_set_2d_size(texture_id texture, const vec2_i& size) = 0;
	virtual void texture_set_2d_filter_mode(texture_id texture, texture_filter_mode mode) = 0;
	virtual void texture_set_2d_wrap_mode(texture_id texture, texture_wrap_mode h = TEXTURE_WRAP_REPEAT, texture_wrap_mode v = TEXTURE_WRAP_REPEAT) = 0;
	virtual void texture_set_2d_data(texture_id texture, unsigned char* tex_data_buffer) = 0;
	virtual unsigned char* get_texture_2d_data(texture_id texture, vec2_i& size) = 0;
	virtual void texture_generate_2d_mip_map(texture_id texture) = 0;

	virtual bool texture_has_mip_map(texture_id texture) = 0;

protected:
	texture_cache_module() = default;
	virtual ~texture_cache_module() = default;
};

#define TCM texture_cache_module