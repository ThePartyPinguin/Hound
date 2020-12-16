#pragma once
#include "hound/core/object/object.h"
#include "hound/core/object/object_database.h"
#include "hound/core/rendering/renderer_cache/module/renderer_cache_module.h"
#include "hound/core/rendering/renderer_cache/module/texture_cache_module.h"
#include "hound/core/rendering/renderer_cache/module/shader_cache_module.h"
#include "hound/core/rendering/renderer_cache/module/mesh_cache_module.h"
#include "hound/core/rendering/renderer_cache/module/frame_buffer_cache_module.h"
#include "hound/core/rendering/renderer_cache/module/render_target_cache_module.h"


#define HND_RENDER_CACHE_MODULE_ACCESS_FUNCTION(name, module_type) inline static module_type* name##_cache(){ return get_module<module_type>();}

class renderer_cache : public object
{
public:
	static renderer_cache* get_instance() { return s_instance_; };

	HND_RENDER_CACHE_MODULE_ACCESS_FUNCTION(texture, texture_cache_module)
	HND_RENDER_CACHE_MODULE_ACCESS_FUNCTION(shader, shader_cache_module)
	HND_RENDER_CACHE_MODULE_ACCESS_FUNCTION(mesh, mesh_cache_module)
	HND_RENDER_CACHE_MODULE_ACCESS_FUNCTION(frame_buffer, frame_buffer_cache_module)
	HND_RENDER_CACHE_MODULE_ACCESS_FUNCTION(render_target, render_target_cache_module)
	
protected:
	static renderer_cache* s_instance_;

	typedef object_id module_id;
	
	template<typename TModule, typename TModuleImpl>
	void register_module();

	template<typename TModule>
	static TModule* get_module();
	
	renderer_cache();
	virtual ~renderer_cache();

private:
	std::unordered_map<module_id, renderer_cache_module*> m_modules_;
	std::unordered_map<const char*, module_id> m_base_type_module_id_map_;
	std::unordered_map<const char*, module_id> m_impl_type_module_id_map_;

};

template <typename TModule>
TModule* renderer_cache::get_module()
{
	renderer_cache* _this = get_instance();

	const char* index = typeid(TModule).name();

	module_id id = module_id::null();
	
	if (_this->m_base_type_module_id_map_.count(index))
	{
		id = _this->m_base_type_module_id_map_[index];
	}
	else if(_this->m_impl_type_module_id_map_.count(index))
	{
		id = _this->m_impl_type_module_id_map_[index];
	}

	if(id.is_null())
	{
		HND_CORE_LOG_ERROR("RenderChache module not found/registerd, returning null");
		return nullptr;
	}

	return static_cast<TModule*>(_this->m_modules_[id]);
}

template <typename TModule, typename TModuleImpl>
void renderer_cache::register_module()
{
	const char* base_index = typeid(TModule).name();
	const char* impl_index = typeid(TModuleImpl).name();

	if(m_base_type_module_id_map_.count(base_index))
	{
		const char* existing_type = typeid(*m_modules_[m_base_type_module_id_map_[base_index]]).name();
		HND_CORE_LOG_WARN("RenderCache module ", base_index, " already registered, as", existing_type);
		return;
	}
	else if(m_impl_type_module_id_map_.count(impl_index))
	{
		const char* existing_type = typeid(*m_modules_[m_impl_type_module_id_map_[impl_index]]).name();
		HND_CORE_LOG_WARN("RenderCache module ", base_index, " already has an implementation, as", existing_type);
		return;
	}

	TModuleImpl* impl_instance = object_database::get_instance()->create_object_instance<TModuleImpl>();

	const module_id id = impl_instance->get_object_id();
	
	m_modules_[id] = impl_instance;
	m_base_type_module_id_map_[base_index] = id;
	m_impl_type_module_id_map_[impl_index] = id;
}

#define RC renderer_cache
