#pragma once

enum class camera_projection
{
	ORTHOGRAPHIC,
	PERSPECTIVE
};

enum class fov_axis
{
	HORIZONTAL,
	VERTICAL
};

struct projection_settings
{
	camera_projection projection;
	vec2_f viewport_size;
	float z_near;
	float z_far;
};

struct orthographic_projection_settings : projection_settings
{
	float size;
};

struct perspective_projection_settings : projection_settings
{
	float fov;
	fov_axis axis;
};
