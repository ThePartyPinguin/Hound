#pragma once
#include "hound/core/property.h"

struct object_id
{
	friend class object_database;
	
	object_id() = default;

	inline bool is_valid() { return m_id_ != 0; }
	inline bool is_null() { return m_id_ == 0; }
	
	inline operator int64_t() const { return m_id_; }
	inline operator uint64_t() const { return m_id_; }
	
	inline bool operator==(const object_id& other ) const { return m_id_ == other.m_id_; }	
	inline bool operator!=(const object_id& other ) const { return m_id_ != other.m_id_; }
	inline bool operator<(const object_id& other ) const { return m_id_ < other.m_id_; }

private:
	explicit object_id(uint64_t id) { m_id_ = id; }
	explicit object_id(int64_t id) { m_id_ = id; }
	inline static uint64_t m_object_counter_ = 1;
	uint64_t m_id_;
};

/// <summary>
/// Hash function to allow object_id's to be used as keys in unordered_maps
/// </summary>
namespace std {

	template <>
	struct hash<object_id>
	{
		std::size_t operator()(const object_id& k) const noexcept
		{
			const uint64_t id = k;
			return hash<uint64_t>()(id);
		}
	};
}

class object
{
public:
	friend class object_database;
		
	HND_PROP_GET_REF_FUNC(object_id, object_id, m_id_)

	object() = default;
	virtual ~object() = default;
private:
	HND_PROP_SET_FUNC(object_id, object_id, m_id_)
	
	object_id m_id_;
};