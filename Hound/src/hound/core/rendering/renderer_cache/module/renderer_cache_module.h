#pragma once
#include "hound/core/object/object_database.h"
#include "hound/core/object/object.h"
#include "hound/core/rendering/renderer_resource_id.h"

class renderer_cache_module : public object
{
	template<typename TTargetType>
	TTargetType* get_target_object(object_id id);
};


template <typename TTargetType>
TTargetType* renderer_cache_module::get_target_object(object_id id)
{
	return object_database::get_object_instance<TTargetType>(id);
}