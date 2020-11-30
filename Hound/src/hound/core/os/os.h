#pragma once
#include "hound/core/base.h"
#include "hound/core/event/event.h"
#include "hound/core/event/window_event.h"
#include "hound/core/input/input_event.h"
#include "hound/core/object/object.h"
#include "hound/main/application.h"

class window;
class display_manager;
class graphics_context;

class os : public object,
	public event_handler<window_close_event>,
	public event_handler<input_event_key>,
	public event_handler<input_event_mouse_move>,
	public event_handler<input_event_mouse>
{
public:

	enum class platform
	{
		unknown = 0x00,
		windows_x64 = 0x01,
		windows_x32 = 0x02,
		linux = 0x03,
		osx = 0x04,
		ios = 0x05,
		android = 0x06
	};

	static os* get_instance() { return s_instance_; }

	platform get_platform();
	
	virtual bool initialize(ref<application> application);
	virtual void run();
	virtual void clean_up();
	
protected:

	friend class main;
	friend class display_manager;
	
	graphics_context* m_graphics_context_;
	display_manager* m_display_manager_;

	ref<application> m_application_;
	
	static os* s_instance_;

	void set_main_window(window* window);

	os();
	virtual ~os();
	
private:
	window* m_main_window_;

	void on_event(const window_close_event&) override;
	void on_event(const input_event_key& e) override;
	void on_event(const input_event_mouse& e) override;
	void on_event(const input_event_mouse_move& e) override;
};
