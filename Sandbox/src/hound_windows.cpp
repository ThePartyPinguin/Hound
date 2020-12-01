#include "hound/hnd_pch.h"
#include "hound/main/main.h"
#include "hound/platform/windows/os_windows.h"
#include "hound/platform/windows/file/windows_file_system.h"

#include "application/sandbox_application.h"

#include <string>

#define BUFSIZE MAX_PATH

char* wchar_to_utf8(const wchar_t* wc)
{
	int ulen = WideCharToMultiByte(CP_UTF8, 0, wc, -1, nullptr, 0, nullptr, nullptr);
	char* ubuf = new char[ulen + 1];
	WideCharToMultiByte(CP_UTF8, 0, wc, -1, ubuf, ulen, nullptr, nullptr);
	ubuf[ulen] = 0;
	return ubuf;
}

int main(int argc, char *argv[])
{
	TCHAR dir_buffer[BUFSIZE];
	DWORD dwRet = GetCurrentDirectory(BUFSIZE, dir_buffer);
	const char* directory = wchar_to_utf8(dir_buffer);

	windows_file_system fs;

	file_system::directory_status status = file_system::directory_create("F:\\SilverWolf\\Test\\Created");
	
	status = file_system::directory_exists("F:\\SilverWolf\\Test\\Created");
	
	status = file_system::directory_remove("F:\\SilverWolf\\Test\\Created", false);

	DWORD ftyp = GetFileAttributesA("F:\\SilverWolf\\Test\\Created");
	bool is_directory = ftyp & FILE_ATTRIBUTE_DIRECTORY;

	HANDLE file = CreateFileA("F:\\SilverWolf\\Test\\Created\\test.txt", GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if(file == INVALID_HANDLE_VALUE)
	{
		DWORD error = GetLastError();
	}

	std::string content = "klhjadfkl;hasdfkl;hj";

	LPCVOID content_buffer = content.c_str();

	LPDWORD bytes_written = 0;
	
	BOOL write_success = WriteFile(file, content_buffer, content.size(), bytes_written, NULL);

	BOOL closed = CloseHandle(file);
	
	RemoveDirectoryA("F:\\SilverWolf\\Test\\Created");
	ftyp = GetFileAttributesA("F:\\SilverWolf\\Test\\Created");

	bool invalid = ftyp & INVALID_FILE_ATTRIBUTES;
	
	windows_os os;
	ref<application> app = create_ref<sandbox_application>();

	main::setup(directory, argc, argv);

	if (os.initialize(app))
		os.run();

	os.clean_up();
	
	main::clean_up();
	return 0;
}