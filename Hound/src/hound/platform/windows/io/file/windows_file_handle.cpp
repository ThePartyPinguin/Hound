#include "hound/hnd_pch.h"
#include "windows_file_handle.h"

void windows_file_handle::init()
{
    m_w_file_path_ = convert_to_wchar(m_file_path_.c_str());
}

char* windows_file_handle::read(size_t& buffer_Size, bool& success)
{
    if(m_buffer_ != nullptr)
    {
    	//Cleanup if the handle has already read something
        delete[] m_buffer_;
        m_buffer_ = nullptr;
        m_buffer_size_ = 0;
    }

	buffer_Size = 0;
    
    if(!exists())
    {
        HND_CORE_LOG_ERROR("Could not read file (does not exist): ", m_file_path_);
        return nullptr;
    }
	
    const HANDLE handle = CreateFileW(m_w_file_path_, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    LARGE_INTEGER size;

	if(!GetFileSizeEx(handle, &size))
	{
        CloseHandle(handle);
        HND_CORE_LOG_ERROR("Could not read file size!\r\n\tFile: ", m_file_path_, "\r\n\tError: ", GetLastError());
        success = false;
        return nullptr;
	}

    buffer_Size = size.QuadPart;

    char* buffer = new char[buffer_Size];
    DWORD bytes_read = 0;
	if(!ReadFile(handle, buffer, size.LowPart, &bytes_read, NULL))
	{
        delete[] buffer;
        CloseHandle(handle);
        HND_CORE_LOG_ERROR("Could not read file!\r\n\tFile: ", m_file_path_, "\r\n\tError: ", GetLastError());
        success = false;
        return nullptr;
	}


	m_buffer_size_ = buffer_Size;
    m_buffer_ = buffer;
    success = true;
    return m_buffer_;
}

bool windows_file_handle::exists()
{
	const DWORD attrib = GetFileAttributes(m_w_file_path_);

    if (INVALID_FILE_ATTRIBUTES == attrib || FILE_ATTRIBUTE_DIRECTORY & attrib || GetLastError() == ERROR_FILE_NOT_FOUND)
    {
        return false;
    }

    return true;
}

const wchar_t* windows_file_handle::convert_to_wchar(const char* str)
{
    const size_t cSize = strlen(str) + 1;
    wchar_t* wc = new wchar_t[cSize];
    mbstowcs(wc, str, cSize);

    return wc;
}
