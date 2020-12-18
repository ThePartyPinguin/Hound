#pragma once
#include "hound/core/object/object.h"
#include "hound/main/application.h"
#include <string>
#include <fstream>

template<typename TInstance>
class file_handler
{
public:
	file_handler(const file_handler& handler) = delete;
	file_handler() = delete;
	
	explicit file_handler(const std::string& file_extension);
	virtual ~file_handler();
	
	object_id load_from_asset_path(const std::string& asset_path);
	object_id load_from_absolute_path(const std::string& file_path);

protected:
	virtual object_id deserialize(char* buffer, size_t length) = 0;
	
private:
	void fix_file_path(std::string& str, const std::string& from, const std::string& to);
	
	std::string m_file_extension_;
};

template <typename TInstance>
file_handler<TInstance>::file_handler(const std::string& file_extension) : m_file_extension_(file_extension)
{
}

template <typename TInstance>
file_handler<TInstance>::~file_handler()
{
}

template <typename TInstance>
object_id file_handler<TInstance>::load_from_asset_path(const std::string& asset_path)
{
	const std::string file_path = application::get_startup_path() + "/Assets/" + asset_path + m_file_extension_;

	return load_from_absolute_path(file_path);
}

template <typename TInstance>
object_id file_handler<TInstance>::load_from_absolute_path(const std::string& file_path)
{
	std::string fixed_path = file_path;

	fix_file_path(fixed_path, "\\", "/");

	std::ifstream file_stream;
	file_stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		file_stream.open(fixed_path.c_str(), std::ios::binary);

		if (file_stream.is_open())
		{
			file_stream.seekg(0, std::ios::end);
			const size_t length = file_stream.tellg();

			file_stream.seekg(0, std::ios::beg);

			char* buffer = new char[length];
			file_stream.read(buffer, length);
			file_stream.close();

			const object_id id = deserialize(buffer, length);

			delete[] buffer;
			return id;
		}
		else
		{
			HND_CORE_LOG_ERROR("Could not open file: ", fixed_path);
			return object_id::null();
		}
	}
	catch (std::exception& ex)
	{
		HND_CORE_LOG_ERROR("Could not open file: ", ex.what());
		return object_id::null();
	}
}

template <typename TObjectType>
void file_handler<TObjectType>::fix_file_path(std::string& str, const std::string& from, const std::string& to)
{
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to, 0, to.length());
		start_pos += to.length();
	}
}
