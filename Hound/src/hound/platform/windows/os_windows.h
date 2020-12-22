#pragma once
#include "hound/core/os/os.h"

class windows_os : public os
{
public:
	windows_os();
	virtual ~windows_os();
	
	bool initialize(ref<application> application) override;
	void run() override;
	void clean_up() override;

	file_handle* file_handle_from_asset_path(const char* path) override;
	file_handle* file_handle_from_absolute_path(const char* path) override;

	
private:
	void fix_file_path(std::string& path);
	std::string convert_asset_path_to_absolute(const char* asset_path);
};