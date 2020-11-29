#pragma once
#include <type_traits>


#define HND_NUMERIC_CONSTRAINED_TEMPLATE_DECL(Type) template<typename Type, typename = typename std::enable_if<std::is_arithmetic<Type>::value, Type>::type>