#pragma once

#include <dquote/detail/dquote.hpp>

#include <functional>

#define DQUOTE_ON_INITIALIZE_FAILED []{}

#if defined _MSC_VER

#include <Windows.h>
#include <objbase.h>

namespace dquote{

	inline bool initialize(HINSTANCE hInst)
	{
		return detail::initialize(hInst);
	}

	inline void uninitialize()
	{
		return detail::uninitialize();
	}

}

#define DQUOTE_DEFINE_MAIN \
	int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) \
	{ \
		if(FAILED(::CoInitialize(NULL)) || !::dquote::initialize(hInstance)){ \
			DQUOTE_ON_INITIALIZE_FAILED(); \
			return 0; \
		} \
		int r = ::run(); \
		::dquote::uninitialize(); \
		::CoUninitialize(); \
		return r; \
	}

#else
#endif
