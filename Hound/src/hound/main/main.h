#pragma once
#include "hound/core/base.h"
#include "hound/core/error_code.h"
#include "hound/main/application.h"

class main
{
public:
	static error setup(const char* exec_path, int argc, char* argv[]);
	static bool start(ref<application>& p_application);
	static void run();
	static void clean_up();

private:
	static ref<application> s_application_;
};

