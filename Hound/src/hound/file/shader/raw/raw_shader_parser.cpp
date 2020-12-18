#include "hound/hnd_pch.h"
#include "raw_shader_parser.h"

void raw_shader_parser::parse(raw_shader& shader, const char* source, size_t length)
{
	raw_shader_parser parser(source, length);
	parser.parse_shader(shader);
}

raw_shader_parser::raw_shader_parser(const char* source, size_t length) : m_file_content_(source, length), m_file_length_(length)
{
	m_current_position_ = 0;
	m_current_column_ = 1;
	m_current_line_ = 1;
}

void raw_shader_parser::parse_shader(raw_shader& shader)
{
	if(!parse_header())
	{
		shader.is_valid = false;
		return;
	}
	
	while (!is_at_end())
	{
		advance_to_next_word();

		if (m_current_word_ == "{")
		{
			++m_current_scope_indent_lvl_;
			continue;
		}

		if (m_current_word_ == "}")
		{
			--m_current_scope_indent_lvl_;
			continue;
		}

		if (m_current_word_ == "ShaderName")
		{
			parse_shader_name(shader);
			continue;
		}

		if (m_current_word_ == "SubShader")
		{
			shader_cache_module::shader_stage stage_type = parse_shader_stage_type();

			if (stage_type == shader_cache_module::SHADER_STAGE_INVALID)
			{
				log_error("Shader stage type is unknown! Possible options are:\r\n\t- Vertex\r\n\t- Geometry\r\n\t- Tessellation\r\n\t- Fragment\r\n\t- Compute");
				shader.is_valid = false;
				return;
			}
			
			if(shader.shader_stages.count(stage_type))
			{
				log_error("Shader source contained multiple of the same shader stages, this is not allowed!");
				shader.is_valid = false;
				return;
			}
			
			if (!parse_shader_stage(shader.shader_stages[stage_type]))
			{
				log_error("Could not parse shader stage!");
				shader.is_valid = false;
				return;
			}

			shader.shader_stages[stage_type].shader_type = stage_type;
		}
	}
}

bool raw_shader_parser::parse_header()
{
	advance_to_next_word();
	if (m_current_word_ != "#Shader")
	{
		log_error("Error in header should be '#Shader'");
		return false;
	}

	return true;
}

bool raw_shader_parser::parse_shader_name(raw_shader& shader)
{
	if (m_current_word_ != "ShaderName")
	{
		log_error("Internal exception, parsing shadername: m_current_word_ should be 'ShaderName' but was: " + m_current_word_);
		return false;
	}
	advance_to_next_word();
	shader.shader_name = parse_current_word_as_string();

	return true;
}

bool raw_shader_parser::parse_shader_stage(raw_shader::raw_shader_stage& stage)
{
	advance_to_next_word();

	if (m_current_word_ != "{")
	{
		log_error("Missing '{'");
		return false;
	}

	const uint32_t previous_scope_indent_lvl_ = m_current_scope_indent_lvl_;
	++m_current_scope_indent_lvl_;

	std::string shader_source;

	while (m_current_scope_indent_lvl_ != previous_scope_indent_lvl_)
	{
		if (is_at_end())
		{
			log_error("Reached end of file while parsing shader stage, you're probably missing a '}'");
			return false;
		}
		
		std::string line = get_next_line();

		if (line.find('{') != std::string::npos)
		{
			++m_current_scope_indent_lvl_;
		}
		if (line.find('}') != std::string::npos)
		{
			--m_current_scope_indent_lvl_;
		}

		if (m_current_scope_indent_lvl_ != previous_scope_indent_lvl_)
		{
			shader_source.append(line);
		}
	}

	stage.shader_source = shader_source;
	return true;
}

shader_cache_module::shader_stage raw_shader_parser::parse_shader_stage_type()
{
	advance_to_next_word();
	std::string type_string = parse_current_word_as_string();

	if (type_string == "Vertex")
	{
		return shader_cache_module::SHADER_STAGE_VERTEX;
	}
	if (type_string == "Geometry")
	{
		return shader_cache_module::SHADER_STAGE_GEOMETRY;
	}
	if (type_string == "Tessellation")
	{
		return shader_cache_module::SHADER_STAGE_TESSELLATION;
	}
	if (type_string == "Fragment")
	{
		return shader_cache_module::SHADER_STAGE_FRAGMENT;
	}
	if (type_string == "Compute")
	{
		return shader_cache_module::SHADER_STAGE_COMPUTE;
	}
	log_error("Invalid shader type!");
	return shader_cache_module::SHADER_STAGE_INVALID;
}

void raw_shader_parser::advance_to_next_word()
{
	m_current_word_.clear();
	skip_white_space();

	do
	{
		m_current_word_.append(1, current_char());
	} while (!next_is_white_space_or_new_line() && next_input());
	next_input();
}

bool raw_shader_parser::is_at_end() const
{
	return m_current_position_ >= m_file_length_ - 1;
}

char raw_shader_parser::current_char() const
{
	return m_file_content_.at(m_current_position_);
}

bool raw_shader_parser::get_char_at(uint32_t index, char& c) const
{
	if (index < m_file_length_)
	{
		c = m_file_content_.at(index);
		return true;
	}
	return false;
}

bool raw_shader_parser::next_input()
{
	if (is_at_end())
		return false;

	++m_current_position_;
	switch (current_char())
	{
	case '\n':
	{
		++m_current_line_;
		break;
	}
	case '\r':
	{
		m_current_column_ = 1;
		break;
	}
	case '\t':
	{
		m_current_column_ += 4;
		break;
	}
	default:
	{
		++m_current_column_;
		break;
	}
	}
	return true;
}

std::string raw_shader_parser::get_next_line()
{
	std::string line;
	do
	{
		line.append(1, current_char());
	} while (current_char() != '\n' && next_input());
	next_input();

	return line;
}

bool raw_shader_parser::next_is_white_space_or_new_line()
{
	if (m_current_position_ + 1 >= m_file_length_)
		return false;

	const char next_char = m_file_content_.at(1 + m_current_position_);

	return next_char == ' ' || next_char == '\t' || next_char == '\n' || next_char == '\r';
}

void raw_shader_parser::skip_white_space()
{
	while (!is_at_end() && (current_char() == '\n' || current_char() == '\r' || current_char() == '\t' || current_char() == ' '))
	{
		next_input();
	}
}

std::string raw_shader_parser::parse_current_word_as_string()
{
	//Check if the word is in between quotations eg. ""
	if (m_current_word_.at(0) == '\"' && m_current_word_.at(m_current_word_.size() - 1) == '\"')
	{
		//Strip of the quotes eg. ""			
		return m_current_word_.substr(0, m_current_word_.size() - 1).substr(1);
	}

	return "";
}

void raw_shader_parser::log_error(const std::string& msg) const
{
	HND_CORE_LOG_ERROR("Shader parse error (line:", m_current_line_, " column: ", m_current_column_, "): ", msg);
}
