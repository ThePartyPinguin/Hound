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
};