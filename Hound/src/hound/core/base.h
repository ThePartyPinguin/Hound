#pragma once

template<typename T>
using scope = std::unique_ptr<T>;

template<typename T, typename ... TArgs>
constexpr scope<T> create_scope(TArgs&& ... args)
{
	return std::make_unique<T>(std::forward<TArgs>(args)...);
}

template<typename T>
using ref = std::shared_ptr<T>;

template<typename T, typename ... TArgs>
constexpr ref<T> create_ref(TArgs&& ... args)
{
	return std::make_shared<T>(std::forward<TArgs>(args)...);
}

