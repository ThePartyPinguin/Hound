#include "hound/hnd_pch.h"
#include "hound/main/main.h"
#include "hound/platform/windows/os_windows.h"

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
	
	windows_os os;
	ref<application> app = create_ref<sandbox_application>();

	main::setup(directory, argc, argv);

	if (os.initialize(app))
		os.run();

	os.clean_up();
	
	main::clean_up();
	return 0;
}