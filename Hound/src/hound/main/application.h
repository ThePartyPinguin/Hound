#pragma once

class application
{
public:
	enum class application_type
	{
		UNKNOWN = 0,
		GAME,
		EDITOR
	};

	virtual application_type get_type() = 0;
	virtual void init() = 0;
	virtual void update() = 0;
	
	application();
	virtual ~application();
};

