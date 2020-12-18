#pragma once
#include "hound/core/rendering/renderer_resource_id.h"
#include "hound/drivers/display_driver.h"

class monitor : public object
{
public:
	struct video_mode
	{
		HND_PROPERTY_READ_ONLY(resolution, vec2_i, resolution_)
		HND_PROPERTY_READ_ONLY(refresh_rate, int, refresh_rate_)
		HND_PROPERTY_READ_ONLY(r_bit_depth, int, r_bit_depth_)
		HND_PROPERTY_READ_ONLY(g_bit_depth, int, g_bit_depth_)
		HND_PROPERTY_READ_ONLY(b_bit_depth, int, b_bit_depth_)

		video_mode() = default;
		video_mode(const vec2_i& resolution, int refresh_rate, int r_bit_depth, int g_bit_depth, int b_bit_depth);
		
	protected:
		vec2_i resolution_;
		int refresh_rate_;
		int r_bit_depth_;
		int g_bit_depth_;
		int b_bit_depth_;
	};

	HND_PROPERTY_READ_ONLY(video_modes, std::vector<video_mode>, m_video_modes_)
	HND_PROPERTY_READ_ONLY(current_video_mode, video_mode, m_current_video_mode_)
	HND_PROPERTY_READ_ONLY(physical_size, vec2_i, m_physical_size_)
	HND_PROPERTY_READ_ONLY(content_scale, vec2_f, m_content_scale_)
	HND_PROPERTY_READ_ONLY(work_area, rect_i, m_work_area_)
	HND_PROPERTY_READ_ONLY(name, std::string, m_name_)
	
protected:
	
	monitor_id m_monitor_id_ = display_driver::INVALID_MONITOR_ID;
	video_mode m_current_video_mode_;
	std::vector<video_mode> m_video_modes_;

	vec2_i m_physical_size_;
	vec2_f m_content_scale_;
	rect_i m_work_area_;
	std::string m_name_;
	
	monitor() = default;
	virtual ~monitor() = default;
};

