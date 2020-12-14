#pragma once
#include "hound/core/base.h"
#include "hound/core/os/os.h"
#include "hound/core/bit.h"
#include "hound/core/object/object.h"
#include "hound/core/math/math.h"
#include "hound/core/rendering/renderer_cache.h"
#include "hound/core/rendering/renderer_cache/module/frame_buffer_cache_module.h"

class window;
class input_event_with_modifier;
class window_event;

class display_manager : public object,
	public event_handler<window_close_event>,
	public event_handler<window_visibility_event>,
	public event_handler<window_title_event>,
	public event_handler<window_resize_event>,
	public event_handler<window_maximize_event>,
	public event_handler<window_minimize_event>,
	public event_handler<window_resizable_event>,
	public event_handler<window_frame_buffer_resize_event>,
	public event_handler<window_content_scale_change_event>,
	public event_handler<window_min_size_change_event>,
	public event_handler<window_max_size_change_event>,
	public event_handler<window_aspect_change_event>,
	public event_handler<window_border_style_change_event>,
	public event_handler<window_always_on_top_change_event>,
	public event_handler<window_move_event>,
	public event_handler<window_mode_change_event>,
	public event_handler<window_focused_event>
{
protected:
	friend class window;
	static display_manager* s_instance_;

public:	
	typedef int window_id;
	typedef int monitor_id;
	
	enum
	{
		MAIN_WINDOW_ID = 0,
		INVALID_WINDOW_ID = -1
	};

	enum window_mode
	{
		windowed = BIT(0),
		minimized = BIT(1),
		maximized = BIT(2),
		full_screen = BIT(3),
		full_screen_windowed = BIT(4)
	};

	enum window_flags
	{
		window_flag_resizable = BIT(1),
		window_flag_border_less = BIT(2),
		window_flag_always_on_top = BIT(3),
		window_flag_focused = BIT(4),
		window_flag_centered = BIT(5)
	};

	enum window_border_style
	{
		border_less,
		bordered
	};

	enum
	{
		MAIN_MONITOR_ID = 0,
		INVALID_MONITOR_ID = -1
	};
	
	struct window_properties
	{
		std::string title;
		rect_i rect;
		vec2_i min_size;
		vec2_i max_size;
		
		window_mode mode;
		bool is_resizable;
		bool is_visible;
		bool is_focused;
		bool is_maximized;
		bool is_minimized;
		bool is_always_on_top;
		window_border_style border_style;
		
		monitor_id monitor_id = MAIN_MONITOR_ID;
	};

	struct window_data
	{
		window_id id;
		window_id parent_id = INVALID_WINDOW_ID;
		std::set<window_id> children;

		std::string title;
		rect_i rect;
		rect_i frame_buffer_rect;

		vec2_f content_scale;
		vec2_i min_size;
		vec2_i max_size;
		vec2_i aspect;
		
		window_mode mode;
		bool should_close;
		bool is_resizable;
		bool is_visible;
		bool is_focused;
		bool is_maximized;
		bool is_minimized;
		bool is_always_on_top;
		window_border_style border_style;

		window* window_object;
		monitor_id monitor_id = MAIN_MONITOR_ID;

		frame_buffer_cache_module::frame_buffer_id frame_buffer;
	};
	
	static display_manager* get_instance() { return s_instance_; }

	virtual void initialize(const window_properties& main_window_properties);
	virtual void redraw_windows() = 0;

	window_id get_main_window();
	window_id request_sub_window(window_id parent_id, const window_properties& properties);
	
	virtual bool destroy_sub_window(window_id id);

	void attach_window_object(window* window_object, window_id window = MAIN_WINDOW_ID);
	window* get_attached_window_object(window_id id = MAIN_WINDOW_ID);

	void subscribe_to_window_events(window* window);
	virtual void process_all_window_events();

	typedef void (*input_event_callback)(const input_event_with_modifier& e);
	typedef void (*window_event_callback)(const window_event& e);

	void window_bind_frame_buffer(window_id window, frame_buffer_cache_module::frame_buffer_id frame_buffer);
	
	static window_properties get_default_properties();

	display_manager();
	virtual ~display_manager();
	
private:
	window_id m_window_id_counter_ = MAIN_WINDOW_ID;
	std::unordered_map<window_id, window_data> m_window_data_;

	const window_data& create_window_data(const window_properties& properties, window* window_object);

	void on_event(const window_close_event& e) override;
	void on_event(const window_visibility_event& e) override;
	void on_event(const window_title_event& e) override;
	void on_event(const window_resize_event& e) override;
	void on_event(const window_maximize_event& e) override;
	void on_event(const window_minimize_event& e) override;
	void on_event(const window_resizable_event& e) override;
	void on_event(const window_frame_buffer_resize_event& e) override;
	void on_event(const window_content_scale_change_event& e) override;
	void on_event(const window_min_size_change_event& e) override;
	void on_event(const window_max_size_change_event& e) override;
	void on_event(const window_aspect_change_event& e) override;
	void on_event(const window_border_style_change_event& e) override;
	void on_event(const window_move_event& e) override;
	void on_event(const window_mode_change_event& e) override;
	void on_event(const window_focused_event& e) override;
	void on_event(const window_always_on_top_change_event& e) override;
	
protected:
	const window_data& get_window_data(window_id window);

	void set_main_window(window* window, window_id id);
	void pass_key_event(window* window, uint32_t window_id, key_code key, key_action action, int modifiers);
	void pass_mouse_position_event(window* window, uint32_t window_id, double pos_x, double pos_y);
	void pass_mouse_button_event(window* window, uint32_t window_id, key_code button, key_action action, int modifiers);

	void set_window_frame_buffer(window_id window_id, const rect_i& rect);
	void set_window_aspect(window_id window_id, const vec2_i& aspect);
	void set_window_content_scale(window_id window_id, const vec2_f& scale);

	virtual void on_init() = 0;
	
	virtual void create_native_window(const window_data& window_data) = 0;

	virtual void redraw_all_native_windows() = 0;
	virtual void redraw_native_window(window_id id) = 0;

	virtual void poll_native_window_events() = 0;
	
	virtual void set_native_window_title(window_id window, const std::string& title) = 0;
	virtual void set_native_window_rect(window_id window, const rect_i& rect) = 0;
	virtual void set_native_window_frame_buffer_rect(window_id window, const rect_i& rect) = 0;
	virtual void set_native_window_min_size(window_id window, const vec2_i& min_size) = 0;
	virtual void set_native_window_max_size(window_id window, const vec2_i& max_size) = 0;
	virtual void set_native_window_mode(window_id window, window_mode mode) = 0;
	virtual void set_native_window_visible(window_id window, bool visible) = 0;
	virtual void set_native_window_focused(window_id window, bool focused) = 0;
	virtual void set_native_window_resizable(window_id window, bool is_resizable) = 0;
	virtual void set_native_window_content_scale(window_id window, const vec2_f& scale) = 0;
	virtual void set_native_window_aspect(window_id window, const vec2_i& aspect) = 0;
	virtual void set_native_window_maximized(window_id window, bool maximized) = 0;
	virtual void set_native_window_minimized(window_id window, bool minimized) = 0;
	virtual void set_native_window_is_always_on_top(window_id window, bool is_always_on_top) = 0;
	virtual void set_native_window_border_style(window_id window, window_border_style style) = 0;
	virtual void native_window_request_attention(window_id window) = 0;
};

