#include "hound/hnd_pch.h"
#include "windows_file_system.h"
#include <filesystem>
#include <fstream>

#define FS std::filesystem

windows_file_system::windows_file_system()
{
	s_instance_ = this;
}

windows_file_system::~windows_file_system()
{
}

file_system::directory_status windows_file_system::native_dir_exists(const std::string& dir)
{
	if (!FS::exists(dir))
		return DIRECTORY_PATH_NOT_FOUND;
	
	if (!FS::is_directory(dir))
		return DIRECTORY_PATH_NOT_VALID;
	
	return DIRECTORY_EXISTS;
}

file_system::directory_status windows_file_system::native_dir_create(const std::string& dir)
{
	const directory_status status = native_dir_exists(dir);

	if (status != DIRECTORY_PATH_NOT_FOUND)
		return status;

	FS::create_directory(dir);

	return DIRECTORY_CREATE_SUCCESS;
}

file_system::directory_status windows_file_system::native_dir_remove(const std::string& dir, bool recursive)
{
	const directory_status status = native_dir_exists(dir);

	if (status != DIRECTORY_ALREADY_EXISTS)
		return status;

	if(recursive)
	{
		FS::remove_all(dir);
	}
	else if(!FS::is_empty(dir))
	{
		return DIRECTORY_NOT_EMPTY;
	}

	FS::remove(dir);

	return DIRECTORY_REMOVE_SUCCESS;
}

file_system::file_status windows_file_system::native_file_exists(const std::string& file)
{
	if (!FS::exists(file))
		return FILE_PATH_NOT_FOUND;
	
	if (!FS::is_regular_file(file))
		return FILE_PATH_NOT_VALID;

	return FILE_EXISTS;
}

file_system::file_status windows_file_system::native_file_remove(const std::string& file)
{
	const file_status status = native_file_exists(file);

	if (status != FILE_EXISTS)
		return status;

	FS::remove(file);

	return FILE_REMOVE_SUCCESS;
}

file_system::file_status windows_file_system::native_file_write(const std::string& file, char* data, size_t length)
{
}

char* windows_file_system::native_file_read_bin(const std::string& file, size_t& length)
{
	const file_status status = native_file_exists(file);

	if (status != FILE_EXISTS)
	{
		length = -1;
		return nullptr;
	}

	std::fstream stream;
	stream.open(file, std::ios::binary | std::ios::in);

	stream.seekp(std::ios::end);
	length = stream.tellg();
	stream.seekp(std::ios::beg);

	char* buffer = new char[length];

	stream.read(buffer, length);

	return buffer;
}

std::string windows_file_system::native_file_read_txt(const std::string& file)
{
}
