#pragma once
#include "hound/main/application.h"

class sandbox_application : public application
{
public:
	application_type get_type() override;
	
	void init() override;
	void update() override;
	
	sandbox_application();
	virtual ~sandbox_application();
};

