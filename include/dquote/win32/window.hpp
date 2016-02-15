#pragma once

#include <dquote/base/procedure.hpp>
#include <dquote/tmp/has_xxx.hpp>
#include <dquote/macro.hpp>

#include <Windows.h>
#include <windowsx.h>

namespace dquote{ namespace win32{

	template <class Derived, class... Procs>
	class window : public ::dquote::base::procedure<Procs...>{
		DQUOTE_DECLARE_BINDER(Derived, initialize)
		DQUOTE_DECLARE_BINDER(Derived, uninitialize)
		DQUOTE_DECLARE_BINDER(Derived, WindowProc, windowproc_binder)
		DQUOTE_DECLARE_BINDER(Derived, on_close)
		DQUOTE_DECLARE_BINDER(Derived, on_size)
		
		static LRESULT CALLBACK WindowProc0(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			auto w = reinterpret_cast<Derived*>(::GetWindowLongPtrW(hwnd, 0));
			if(msg == WM_NCCREATE || msg == WM_CREATE){
				auto lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
				::SetWindowLongPtrW(hwnd, 0, reinterpret_cast<LONG_PTR>(lpcs->lpCreateParams));
				::SetWindowLongPtrW(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProc1));
				w = reinterpret_cast<Derived*>(lpcs->lpCreateParams);
				w->hwnd = hwnd;
				w->hparent = lpcs->hwndParent;
			}
			if(w == nullptr)
				return ::DefWindowProcW(hwnd, msg, wParam, lParam);
			return w->WindowProc(hwnd, msg, wParam, lParam);
		}

		static LRESULT CALLBACK WindowProc1(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			auto w = reinterpret_cast<Derived*>(::GetWindowLongPtrW(hwnd, 0));
			if(w == nullptr)
				return ::DefWindowProcW(hwnd, msg, wParam, lParam);
			return w->WindowProc(hwnd, msg, wParam, lParam);
		}

	protected:
		HWND hwnd, hparent;

	public:
		window(): hwnd(nullptr), hparent(nullptr)
		{
		}

		static bool register_class(const wchar_t *classname)
		{
			WNDCLASSEXW wcex;
			wcex.cbSize = sizeof(WNDCLASSEX);
			wcex.style = CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc = WindowProc0;
			wcex.cbClsExtra = 0;
			wcex.cbWndExtra = sizeof(void*);
			wcex.hInstance = ::GetModuleHandleW(NULL);
			wcex.hIcon = nullptr;
			wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
			wcex.hbrBackground = nullptr;
			wcex.lpszMenuName = nullptr;
			wcex.lpszClassName = classname;
			wcex.hIconSm = nullptr;

			return ::RegisterClassExW(&wcex) != 0;
		}

		static bool register_class(void)
		{
			return register_class(Derived::get_class_name());
		}

		void *get_handle() const
		{
			return hwnd;
		}

		HWND get_hwnd() const
		{
			return hwnd;
		}

		HWND get_parent() const
		{
			return hparent;
		}

		void enable(void) const
		{
			::EnableWindow(hwnd, TRUE);
		}

		void disable(void) const
		{
			::EnableWindow(hwnd, FALSE);
		}

		void show(void)
		{
			::ShowWindow(hwnd, SW_SHOW);
		}

		void hide(void)
		{
			::ShowWindow(hwnd, SW_HIDE);
		}

		void destroy()
		{
			::DestroyWindow(hwnd);
		}

		::std::pair<int, int> get_size()
		{
			RECT rc;
			::GetClientRect(hwnd, &rc);
			return{ rc.right, rc.bottom };
		}

		::std::pair<int, int> get_position()
		{
			RECT rc;
			::GetWindowRect(hwnd, &rc);
			return{ rc.left, rc.top };
		}

		void set_title(const wchar_t *title)
		{
			::SetWindowTextW(hwnd, title);
		}

		void set_title(const ::std::wstring &title)
		{
			::SetWindowTextW(hwnd, title.c_str());
		}

		LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			switch(msg){
			case WM_CREATE:
				if(!static_cast<Derived*>(this)->initialize())
					return -1l;
				if(!all_of<initialize_binder>())
					return -1l;
				break;
			case WM_DESTROY:
				static_cast<Derived*>(this)->uninitialize();
				for_each<uninitialize_binder>();
				break;
			case WM_CLOSE:
				for_each<on_close_binder>();
			case WM_SIZE:
				for_each<on_size_binder>(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				break;
			}

			if(sizeof...(Procs) > 0){
				LRESULT lresult = 0l;
				if(!all_of<windowproc_binder>(hwnd, msg, wParam, lParam, ::std::ref(lresult)))
					return lresult;
			}

			if(msg == WM_DESTROY)
				this->hwnd = nullptr;

			return ::DefWindowProcW(hwnd, msg, wParam, lParam);
		}
	};

} }
