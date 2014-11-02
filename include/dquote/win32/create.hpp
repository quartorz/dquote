#pragma once

#include "../tmp/has_xxx.hpp"

#include <Windows.h>

namespace dquote{ namespace win32{

	namespace aux_{

		/*DQUOTE_DECLARE_HAS_NON_TYPE(is_subclass);

		template <bool, class Window>
		bool set_subclass_impl(HWND hwnd)
		{
			return static_cast<Derived*>(this)->set_subclass(hwnd);
		}
		template <>
		bool set_subclass_impl<false>(HWND)
		{
			return true;
		}

		bool set_subclass_(std::true_type&, HWND hwnd)
		{
			return set_subclass_impl<Derived::is_subclass>(hwnd);
		}
		bool set_subclass_(std::false_type&, HWND)
		{
			return true;
		}*/

	}

	struct creation_params{
		DWORD exstyle, style;
		const wchar_t *classname, *title;
		int x, y, w, h;
		HWND hparent;
	};

	template <class Window>
	bool create(Window &w, creation_params &cp)
	{
		if(cp.w != CW_USEDEFAULT){
			RECT rc = {0, 0, cp.w, cp.h};
			::AdjustWindowRectEx(&rc, cp.style, FALSE, cp.exstyle);
			cp.w = rc.right - rc.left;
			cp.h = rc.bottom - rc.top;
		}
		HWND hwnd = ::CreateWindowExW(
			cp.exstyle,
			cp.classname,
			cp.title,
			cp.style,
			cp.x,
			cp.y,
			cp.w,
			cp.h,
			cp.hparent,
			nullptr,
			::GetModuleHandleW(nullptr),
			std::addressof(w));
		/*if(!set_subclass_(typename aux::has_is_subclass<Derived>::type(), hwnd)){
			::DestroyWindow(hwnd);
			hwnd = nullptr;
		}*/
		return hwnd != nullptr;
	}

	template <class Window, class Parent>
	bool create(Window &window, Parent &parent, const wchar_t *classname=nullptr, const wchar_t *title=L"", int x=INT_MAX, int y=INT_MAX, int w=-1, int h=-1)
	{
		creation_params params ={
			0,
			WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
			((classname != nullptr) ? classname : window.get_class_name()),
			title,
			((x == INT_MAX || y == INT_MAX) ? CW_USEDEFAULT : x),
			y,
			((w == -1 || h == -1) ? CW_USEDEFAULT : w),
			h,
			parent.get_hwnd(),
		};

		return create(window, params);
	}

	template <class Window>
	bool create(Window &window, const wchar_t *classname=nullptr, const wchar_t *title=L"", int x=INT_MAX, int y=INT_MAX, int w=-1, int h=-1)
	{
		creation_params params ={
			0,
			WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
			((classname != nullptr) ? classname : window.get_class_name()),
			title,
			((x == INT_MAX || y == INT_MAX) ? CW_USEDEFAULT : x),
			y,
			((w == -1 || h == -1) ? CW_USEDEFAULT : w),
			h,
			nullptr,
		};

		return create(window, params);
	}

} }
