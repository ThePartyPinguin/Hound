#include "hound/hnd_pch.h"
#include "file_handle.h"

void file_handle::dispose()
{
	delete[] m_buffer_;
	m_buffer_ = nullptr;
}

file_handle::file_handle()
{
	m_buffer_ = nullptr;
}

file_handle::~file_handle()
{
	delete[] m_buffer_;
}
