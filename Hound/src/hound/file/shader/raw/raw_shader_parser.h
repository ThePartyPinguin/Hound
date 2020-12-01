#pragma once
#include "hound/file/shader/raw/raw_shader.h"

class raw_shader_parser
{
public:
	static void parse(raw_shader& shader, const char* source, size_t length);

private:
	raw_shader_parser(const char* source, size_t length);

	void parse_shader(raw_shader& shader);
	bool parse_header();
	bool parse_shader_name(raw_shader& shader);
	bool parse_shader_stage(raw_shader::shader_stage& stage);
	raw_shader::shader_stage_type parse_shader_stage_type();
	
	void advance_to_next_word();
	bool is_at_end() const;
	char current_char() const;
	bool get_char_at(uint32_t index, char& c) const;
	bool next_input();
	std::string get_next_line();
	bool next_is_white_space_or_new_line();
	void skip_white_space();

	std::string parse_current_word_as_string();

	void log_error(const std::string& msg) const;

	
	raw_shader* m_raw_shader_;
	std::string m_file_content_;
	size_t m_file_length_;

	std::string m_current_word_;
	uint32_t m_current_position_;
	uint32_t m_current_column_;
	uint32_t m_current_line_;
	
	uint32_t m_current_scope_indent_lvl_;
};

