#include "hound/hnd_pch.h"
#include "hound/main/main.h"
#include "hound/platform/windows/os_windows.h"

#include "application/sandbox_application.h"

int main(int argc, char *argv[])
{
	windows_os os;
	ref<application> app = create_ref<sandbox_application>();

	main::setup(argv[0], argc, argv);

	if (os.initialize(app))
		os.run();

	os.clean_up();
	
	main::clean_up();
	return 0;
}