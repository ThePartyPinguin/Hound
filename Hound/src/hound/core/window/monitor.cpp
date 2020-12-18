#include "hound/hnd_pch.h"
#include "monitor.h"

monitor::video_mode::video_mode(const vec2_i& resolution, int refresh_rate, int r_bit_depth, int g_bit_depth,	int b_bit_depth) :
	resolution_(resolution),
	refresh_rate_(refresh_rate),
	r_bit_depth_(r_bit_depth),
	g_bit_depth_(g_bit_depth),
	b_bit_depth_(b_bit_depth)
{
	
}
