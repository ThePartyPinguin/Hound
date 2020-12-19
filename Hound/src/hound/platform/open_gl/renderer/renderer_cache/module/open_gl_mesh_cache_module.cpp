#include "hound/hnd_pch.h"
#include "open_gl_mesh_cache_module.h"

#include "hound/core/object/object_database.h"
#include "hound/core/object/mesh/mesh.h"

mesh_id open_gl_mesh_cache_module::mesh_create()
{
	const mesh* mesh_instance = object_database::get_instance()->create_object_instance<mesh>();

	gl_object_id id;
	glGenVertexArrays(1, &id);

	m_gl_mesh_map_[mesh_instance->get_object_id()].gl_vertex_array_id = id;

	return mesh_instance->get_object_id();
}

mesh_data_id open_gl_mesh_cache_module::mesh_add_data(mesh_id mesh, const mesh_data& data)
{
	if (!m_gl_mesh_map_.count(mesh))
	{
		HND_LOG_ERROR("No mesh found with id: ", static_cast<uint64_t>(mesh));
		return 0;
	}

	auto& mesh_object = m_gl_mesh_map_[mesh];
	auto& mesh_sub_data = mesh_object.sub_objects[mesh_object.data_count];

	mesh_sub_data.id = mesh_object.data_count;

	mesh_sub_data.vertices.resize(data.vertices.size());
	std::copy(data.vertices.begin(), data.vertices.end(), mesh_sub_data.vertices.begin());

	mesh_sub_data.indices.resize(data.indices.size());
	std::copy(data.indices.begin(), data.indices.end(), mesh_sub_data.indices.begin());

	glBindVertexArray(mesh_object.gl_vertex_array_id);

	gl_object_id gl_vertex_buffer_id;
	glGenBuffers(1, &gl_vertex_buffer_id);

	mesh_sub_data.gl_array_buffer_id = gl_vertex_buffer_id;

	glBindBuffer(GL_ARRAY_BUFFER, gl_vertex_buffer_id);

	glBufferData(GL_ARRAY_BUFFER, mesh_sub_data.vertices.size() * sizeof(mesh_data::vertex), &mesh_sub_data.vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(mesh_data::vertex), reinterpret_cast<void*>(offsetof(mesh_data::vertex, position)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(mesh_data::vertex), reinterpret_cast<void*>(offsetof(mesh_data::vertex, normal)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(mesh_data::vertex), reinterpret_cast<void*>(offsetof(mesh_data::vertex, uv)));
	glEnableVertexAttribArray(2);

	gl_object_id gl_element_buffer_id;
	glGenBuffers(1, &gl_element_buffer_id);

	mesh_sub_data.gl_element_buffer_id = gl_element_buffer_id;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_element_buffer_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_sub_data.indices.size() * sizeof(uint32_t), &mesh_sub_data.indices[0], GL_STATIC_DRAW);

	const mesh_data_id data_id = mesh_object.data_count;

	++mesh_object.data_count;

	return data_id;
}

bool open_gl_mesh_cache_module::mesh_override_data(mesh_id mesh, mesh_data_id data_id, const mesh_data& data)
{
	gl_mesh_object& mesh_data = m_gl_mesh_map_[mesh];

	if (data_id >= mesh_data.data_count)
	{
		HND_LOG_ERROR("Could not overwrite mesh data, data_id cannot be greater than the count");
		return false;
	}

	// mesh_data.sub_objects[data_id] = data;
	return true;
}

const open_gl_mesh_cache_module::gl_mesh_object& open_gl_mesh_cache_module::get_gl_mesh_object(mesh_id mesh)
{	
	return m_gl_mesh_map_[mesh];
}
