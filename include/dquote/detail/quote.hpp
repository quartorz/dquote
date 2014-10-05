#pragma once

#include "../win32/init.hpp"
#include "../direct2d/init.hpp"

#include <Windows.h>

namespace dquote{ namespace detail{

#if defined _MSC_VER
	inline bool initialize(HINSTANCE hInst)
	{
		return ::dquote::win32::initialize(hInst) && ::dquote::direct2d::initialize();
	}

	inline void uninitialize()
	{
		::dquote::win32::uninitialize();
		::dquote::direct2d::uninitialize();
	}
#else

#endif

} }
