#pragma once
#include <unordered_map>

template<typename TEvent>
class event_handler;

class event_publisher
{
public:
	
	template<typename TEvent>
	void subscribe(event_handler<TEvent>* handler);

	template<typename TEvent>
	void un_subscribe(event_handler<TEvent>* handler);

protected:
	template<typename TEvent>
	void publish_event(const TEvent& event);
	
private:
	std::unordered_map <const char*, std::set<void*>> m_handlers_map_;
};

template <typename TEvent>
void event_publisher::subscribe(event_handler<TEvent>* handler)
{
	const char* type_id = typeid(TEvent).name();
	m_handlers_map_[type_id].insert(handler);
	handler->m_publisher_ = this;
}

template <typename TEvent>
void event_publisher::un_subscribe(event_handler<TEvent>* handler)
{
	const char* type_id = typeid(TEvent).name();
	m_handlers_map_[type_id].erase(handler);
}

template <typename TEvent>
void event_publisher::publish_event(const TEvent& event)
{
	const char* type_id = typeid(TEvent).name();
	if (m_handlers_map_.count(type_id))
	{
		const std::set<void*>& handlers = m_handlers_map_[type_id];

		for (const auto& handler : handlers)
		{
			static_cast<event_handler<TEvent>*>(handler)->on_event(event);
		}
	}
}

template<typename TEvent>
class event_handler
{
public:
	friend class event_publisher;

	event_handler() = default;
	virtual ~event_handler();

protected:
	virtual void on_event(const TEvent& e);

protected:
	event_publisher* m_publisher_ = nullptr;
};

template <typename TEvent>
event_handler<TEvent>::~event_handler()
{
	m_publisher_->un_subscribe(this);
}

template <typename TEvent>
void event_handler<TEvent>::on_event(const TEvent& e)
{
}