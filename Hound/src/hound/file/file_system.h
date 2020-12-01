#pragma once
#include <fstream>
#include <string>

class file_system
{
public:
	static file_system* get_instance() { return s_instance_; }

	enum directory_status
	{
		DIRECTORY_CREATE_SUCCESS,
		DIRECTORY_REMOVE_SUCCESS,
		DIRECTORY_EXISTS,
		DIRECTORY_ALREADY_EXISTS,
		DIRECTORY_PATH_NOT_FOUND,
		DIRECTORY_PATH_NOT_VALID,
		DIRECTORY_NOT_EMPTY
	};

	struct directory
	{
		std::string path;
	};
	
	static directory_status directory_exists(const std::string& dir);
	static directory_status directory_create(const std::string& dir);
	static directory_status directory_remove(const std::string& dir, bool recursive);

	enum class file_access
	{
		NONE,
		READ,
		WRITE,
		READ_WRITE
	};
	
	enum file_creation_param
	{
		FILE_OVERWRITE,
		FILE_NEW,
		FILE_OPEN_IF_EXIST,
		FILE_OPEN_AND_TRUNCATE
	};

	enum file_status
	{
		FILE_CREATE_SUCCESS,
		FILE_REMOVE_SUCCESS,
		FILE_EXISTS,
		FILE_ALREADY_EXISTS,
		FILE_PATH_NOT_FOUND,
		FILE_PATH_NOT_VALID
	};
	
	struct file
	{
		std::string path;
		file_access access_type;
	};

	struct file_create_result
	{
		file file;
		file_status status;
	};
	
	static file_status file_exists(const std::string& file);
	static file_status file_remove(const std::string& file);
	
	static file_status file_write(const std::string& file, char* data, size_t length);
	static char* file_read_bin(const std::string& file, size_t& length);
	static std::string file_read_txt(const std::string& file);
	

	file_system();
	virtual ~file_system();

protected:
	static file_system* s_instance_;

	virtual directory_status native_dir_exists(const std::string& dir) = 0;
	virtual directory_status native_dir_create(const std::string& dir) = 0;
	virtual directory_status native_dir_remove(const std::string& dir, bool recursive) = 0;

	virtual file_status native_file_exists(const std::string& file) = 0;
	virtual file_status native_file_remove(const std::string& file) = 0;

	virtual file_status native_file_write(const std::string& file, char* data, size_t length) = 0;
	virtual char* native_file_read_bin(const std::string& file, size_t& length) = 0;
	virtual std::string native_file_read_txt(const std::string& file) = 0;
	
};

