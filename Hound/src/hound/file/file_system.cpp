#include "hound/hnd_pch.h"
#include "file_system.h"

#include <filesystem>
#include <shellapi.h>

file_system* file_system::s_instance_ = nullptr;

file_system::directory_status file_system::directory_exists(const std::string& dir)
{
	return s_instance_->native_dir_exists(dir);
}

file_system::directory_status file_system::directory_create(const std::string& dir)
{
	return s_instance_->native_dir_create(dir);
	BOOL success = CreateDirectoryA(dir.c_str(), NULL);

	directory_status status = DIRECTORY_CREATE_SUCCESS;
	if(!success)
	{
		switch (GetLastError())
		{
		case ERROR_ALREADY_EXISTS:
			status = DIRECTORY_ALREADY_EXISTS;
			break;
		case ERROR_PATH_NOT_FOUND:
			status = DIRECTORY_PATH_NOT_FOUND;
			break;
		default:
			status = DIRECTORY_CREATE_SUCCESS;
			break;
		}
	}
	return status;
}

file_system::directory_status file_system::directory_remove(const std::string& dir, bool recursive)
{
	return s_instance_->native_dir_remove(dir, recursive);
}


file_system::file_status file_system::file_exists(const std::string& file)
{
	return s_instance_->native_file_exists(file);
}

file_system::file_status file_system::file_remove(const std::string& file)
{
	return s_instance_->native_file_remove(file);
}

file_system::file_status file_system::file_write(const std::string& file, char* data, size_t length)
{
	return s_instance_->native_file_write(file, data, length);
}

char* file_system::file_read_bin(const std::string& file, size_t& length)
{
	return s_instance_->native_file_read_bin(file, length);
}

std::string file_system::file_read_txt(const std::string& file)
{
	return s_instance_->native_file_read_txt(file);
}

file_system::file_system()
{
	s_instance_ = this;
}

file_system::~file_system()
{
}
