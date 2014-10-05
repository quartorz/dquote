#pragma once

#if defined _MSC_VER

#define DQUOTE_SELECTANY __declspec(selectany)

#else

#define DQUOTE_SELECTANY __attribute__((selectany))

#endif

#include "tmp/has_xxx.hpp"

#define DQUOTE_DECLARE_BINDER(name) \
	template <class T> \
	struct name ## _binder{ \
		using type = T; \
		DQUOTE_DECL_HAS_NON_TYPE(name); \
		template <class U, class... Args> \
		typename std::enable_if<has_ ## name<U>::value>::type operator()(U &obj, T *this_, Args... args) \
		{ \
			obj.name(*this_, args...); \
		} \
		template <class U> \
		typename std::enable_if<!has_ ## name<U>::value>::type operator()(U &, ...) \
		{ \
		} \
	}
