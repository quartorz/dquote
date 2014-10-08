#pragma once

#if defined _MSC_VER

#define DQUOTE_SELECTANY __declspec(selectany)

#else

#define DQUOTE_SELECTANY __attribute__((selectany))

#endif

#define DQUOTE_DECLARE_BINDER(parent, name) \
	struct name ## _binder{ \
		using type = parent; \
		template <class U, class... Args> \
		auto operator()(U &obj, parent *this_, Args... args) -> decltype(obj.name(*this_, args...)) \
		{ \
			return obj.name(*this_, args...); \
		} \
		template <class U> \
		void operator()(U &, ...) \
		{ \
		} \
	}
