#pragma once
#include <string>
class application
{
public:
	friend class main;
	
	enum class application_type
	{
		UNKNOWN = 0,
		GAME,
		EDITOR
	};

	static const std::string& get_startup_path();
	
	virtual application_type get_type() = 0;
	virtual void init() = 0;
	virtual void update() = 0;
	
	application();
	virtual ~application();

private:
	static std::string s_startup_path_;
};

