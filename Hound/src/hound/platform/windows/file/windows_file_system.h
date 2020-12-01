#pragma once
#include "hound/file/file_system.h"

class windows_file_system : public file_system
{
public:
	windows_file_system();
	virtual ~windows_file_system();
protected:
	directory_status native_dir_exists(const std::string& dir) override;
	directory_status native_dir_create(const std::string& dir) override;
	directory_status native_dir_remove(const std::string& dir, bool recursive) override;

	file_status native_file_exists(const std::string& file) override;
	file_status native_file_remove(const std::string& file) override;

	file_status native_file_write(const std::string& file, char* data, size_t length) override;
	char* native_file_read_bin(const std::string& file, size_t& length) override;
	std::string native_file_read_txt(const std::string& file) override;
};

