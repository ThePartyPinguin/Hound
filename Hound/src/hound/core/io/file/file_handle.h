#pragma once

class file_handle : public object
{
	friend class os;
public:	
	virtual char* read(size_t& buffer_Size, bool& success) = 0;
	virtual bool exists() = 0;
	void dispose();
	
	file_handle();
	virtual ~file_handle();
	
protected:
	char* m_buffer_;
	size_t m_buffer_size_;
	std::string m_file_path_;
};

