#pragma once
#include "hound/file/file_handler.h"
#include "hound/core/object/shader/shader.h"

class shader_file_handler : public file_handler<shader>
{
public:
	shader_file_handler(const std::string& file_extension) : file_handler<shader>(file_extension){}

protected:
	void deserialize(object_id instance_id, char* buffer, size_t length) override;
	object_id create_instance() override;
};

