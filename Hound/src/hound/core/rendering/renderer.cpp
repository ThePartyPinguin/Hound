#include "hound/hnd_pch.h"
#include "renderer.h"

renderer* renderer::s_instance_ = nullptr;

renderer* renderer::get_instance()
{
	return s_instance_;
}

renderer::type_api renderer::get_api_type()
{
	return type_api::INVALID;
}

renderer::renderer()
{
	m_clear_color_ = vec4_f(0.2f, 0.2f, 0.2f, 1.0f);
}

renderer::~renderer()
{
}
