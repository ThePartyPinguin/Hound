#include "hound/hnd_pch.h"
#include "object.h"

object_id object_id::null()
{
	return object_id(static_cast<uint64_t>(0));
}
