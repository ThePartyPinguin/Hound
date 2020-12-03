#pragma once
#include "hound/core/bit.h"
#include "hound/core/math/math.h"
#include "hound/core/object/object.h"
#include "hound/core/object/object_database.h"
#include "hound/core/object/shader/shader.h"
#include "hound/core/rendering/renderer_cache/module/renderer_cache_module.h"

class renderer_cache : public object
{
public:
	static renderer_cache* get_instance() { return s_instance_; };

	template<typename TModule>
	static TModule* get_module();
	
	typedef object_id shader_id;

	virtual shader_id shader_create() = 0;
	
	virtual void shader_set_source(shader_id shader, shader::stage stage, const std::string& source) = 0;
	virtual const std::string& shader_get_source(shader_id shader, shader::stage stage) = 0;
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

	typedef object_id texture_id;

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

	virtual texture_id texture_create_2d() = 0;
	virtual void texture_set_2d_size(texture_id texture, const vec2_i& size) = 0;
	virtual void texture_set_2d_filter_mode(texture_id texture, texture_filter_mode mode) = 0;
	virtual void texture_set_2d_wrap_mode(texture_id texture, texture_wrap_mode h = TEXTURE_WRAP_REPEAT, texture_wrap_mode v = TEXTURE_WRAP_REPEAT) = 0;
	virtual void texture_set_2d_data(texture_id texture, unsigned char* data) = 0;
	virtual unsigned char* get_texture_2d_data(texture_id texture, vec2_i& size) = 0;
	virtual void texture_generate_2d_mip_map(texture_id texture) = 0;
	
	virtual bool texture_has_mip_map(texture_id texture) = 0;
	
	typedef object_id frame_buffer_id;

	struct frame_buffer_data
	{
		vec2_i size;
	};
	
	virtual frame_buffer_id frame_buffer_create() = 0;
	virtual void frame_buffer_set_size(frame_buffer_id frame_buffer, const vec2_i& size) = 0;
	virtual const vec2_i& frame_buffer_get_size(frame_buffer_id frame_buffer) = 0;
protected:
	static renderer_cache* s_instance_;

	typedef object_id module_id;
	
	template<typename TModule, typename TModuleImpl>
	void register_module();
	
	renderer_cache();
	virtual ~renderer_cache();

private:
	std::unordered_map<module_id, renderer_cache_module*> m_modules_;
	std::unordered_map<const char*, module_id> m_type_module_id_map_;
};

template <typename TModule>
TModule* renderer_cache::get_module()
{
	renderer_cache* _this = get_instance();

	const char* index = typeid(TModule).name();
	
	if(!_this->m_type_module_id_map_.count(index))
	{
		HND_CORE_LOG_ERROR("RenderChache module not foud/registerd, returning null");
		return nullptr;
	}

	const module_id id = _this->m_type_module_id_map_[index];
	return static_cast<TModule*>(get_instance()->m_modules_[id]);
}

template <typename TModule, typename TModuleImpl>
void renderer_cache::register_module()
{
	const char* index = typeid(TModule).name();

	if(m_type_module_id_map_.count(index))
	{
		const char* existing_type = typeid(*m_modules_[m_type_module_id_map_[index]]).name();
		HND_CORE_LOG_WARN("RenderCache module ", index, " already registered, as", existing_type);
		return;
	}

	TModuleImpl* impl_instance = object_database::get_instance()->create_object_instance<TModuleImpl>();

	const module_id id = impl_instance->get_object_id();
	
	m_modules_[id] = impl_instance;
	m_type_module_id_map_[index] = id;
}

#define RC renderer_cache
