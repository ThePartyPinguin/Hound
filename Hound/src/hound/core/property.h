#pragma once

#define HND_PROP_GET_REF_FUNC(name, type, var) inline const type& get_##name() const { return var; }
#define HND_PROP_SET_FUNC(name, type, var) inline void set_##name(const type& value) { var = value; }
#define HND_PROP_SET_CALLBACK_FUNC(name, type, var, func) inline void set_##name(const type& value) { func(value);}

#define HND_PROP_GET_RTR_FUNC(name, type, var) inline type* get_##name() const { return var; }
#define HND_PROP_SET_RTR_FUNC(name, type, var) inline void set_##name(type* value) { var = value; }



#define HND_PROPERTY(name, type, var) HND_PROP_GET_REF_FUNC(name, type, var) HND_PROP_SET_FUNC(name, type, var)
#define HND_PROPERTY_CALLBACK(name, type, var, func) HND_PROP_GET_REF_FUNC(name, type, var) HND_PROP_SET_CALLBACK_FUNC(name, type, var, func)
#define HND_PROPERTY_READ_ONLY(name, type, var) HND_PROP_GET_REF_FUNC(name, type, var)
#define HND_PROPERTY_WRITE_ONLY(name, type, var) HND_PROP_SET_FUNC(name, type, var)

#define HND_PROPERTY_PTR(name, type, var) HND_PROP_GET_RTR_FUNC(name, type, var) HND_PROP_SET_RTR_FUNC(name, type, var)
#define HND_PROPERTY_PTR_READ_ONLY(name, type, var) HND_PROP_GET_RTR_FUNC(name, type, var)


#define HND_STATIC_PROP_GET_FUNC(name, type, var) inline static const type& get_##name() { return var; }
#define HND_STATIC_PROP_SET_FUNC(name, type, var) inline static void set_##name(const type& value) { var = value; }

#define HND_STATIC_PROPERTY(name, type, var) HND_STATIC_PROP_GET_FUNC(name, type, var) HND_STATIC_PROP_SET_FUNC(name, type, var)
#define HND_STATIC_PROPERTY_READ_ONLY(name, type, var) HND_STATIC_PROP_GET_FUNC(name, type, var)
#define HND_STATIC_PROPERTY_WRITE_ONLY(name, type, var) HND_STATIC_PROP_SET_FUNC(name, type, var)