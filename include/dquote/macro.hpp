#pragma once

#if defined(_MSC_VER)
# define DQUOTE_SELECTANY __declspec(selectany)
#else
# define DQUOTE_SELECTANY __attribute__((selectany))
#endif

#include <dquote/pp/overload.hpp>

#define DQUOTE_DECLARE_BINDER(...) DQUOTE_PP_OVERLOAD(DQUOTE_DECLARE_BINDER_, __VA_ARGS__)

#define DQUOTE_DECLARE_BINDER_2(parent, funcname) DQUOTE_DECLARE_BINDER_3(parent, funcname, funcname ## _binder)
#define DQUOTE_DECLARE_BINDER_3(parent, funcname, bindername) \
	struct bindername { \
		using type = parent; \
		template <class U, class... Args> \
		auto operator()(U &obj, parent *this_, Args... args) -> decltype(obj.funcname(*this_, args...)) \
		{ \
			return obj.funcname(*this_, args...); \
		} \
		template <class U> \
		void operator()(U &, ...) \
		{ \
		} \
	};
