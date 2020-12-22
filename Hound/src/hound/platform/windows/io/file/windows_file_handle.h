#pragma once
#include "hound/core/io/file/file_handle.h"

class windows_file_handle : public file_handle
{
	friend class windows_os;
	
protected:

	void init();
	
	const wchar_t* m_w_file_path_;
	
	char* read(size_t& buffer_Size, bool& success) override;
	bool exists() override;

	const wchar_t* convert_to_wchar(const char* str);
};
