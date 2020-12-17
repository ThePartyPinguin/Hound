#pragma once
#include "hound/core/input/key_codes.h"
#include "hound/core/event/event.h"
#include "hound/core/event/window_event.h"
#include "hound/core/object/object.h"
#include "hound/core/math/math.h"

class window;

class input_system : public object,
	public event_publisher,
	public event_handler<window_key_input_event>,
	public event_handler<window_mouse_move_event>,
	public event_handler<window_mouse_button_input_event>
{
	friend class display_manager;
public:
	static input_system* get_instance() { return s_instance_; }

	input_system();
	~input_system() = default;;
private:
	static input_system* s_instance_;

	struct mouse_data
	{
		vec2_d window_mouse_position;
		vec2_d global_mouse_position;
		int mods;
		key_code button;
		key_action button_action;
	};

	mouse_data m_mouse_data_;
	
	void publish_key_event(window* window, uint32_t window_id, key_code key, key_action action, int modifiers);
	void publish_char_input_event(window* window, uint32_t window_id);
	void publish_mouse_position_event(window* window, uint32_t window_id, double pos_x, double pos_y);
	void publish_mouse_button_event(window* window, uint32_t window_id, key_code button, key_action action, int modifiers);

	void on_event(const window_key_input_event& e) override;
	void on_event(const window_mouse_move_event& e) override;
	void on_event(const window_mouse_button_input_event& e) override;
};
