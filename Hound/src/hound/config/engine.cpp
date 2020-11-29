#include "hound/hnd_pch.h"
#include "engine.h"

engine* engine::s_instance_ = nullptr;
bool engine::s_initialized_ = false;

void engine::init()
{
	if(s_initialized_)
	{
		HND_LOG_WARN("Engine already initialized");
		return;
	}

	s_instance_ = new engine();
	object_database::init();
	
	s_initialized_ = true;
}

void engine::clean_up()
{
	object_database::clean_up();
}

