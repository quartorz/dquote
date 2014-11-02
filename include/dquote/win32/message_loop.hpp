#pragma once

#include <Windows.h>

#include "../base/procedure.hpp"
#include "../macro.hpp"

#include <chrono>
#include <thread>

namespace dquote{ namespace win32{

	namespace aux_{

		template <class... Procs>
		class message_loop_procedure: public ::dquote::base::procedure<Procs...>{
			using base = ::dquote::base::procedure<Procs...>;

			DQUOTE_DECLARE_BINDER(message_loop_procedure<Procs...>, on_key_down);
			DQUOTE_DECLARE_BINDER(message_loop_procedure<Procs...>, on_no_message);

		public:
			message_loop_procedure(Procs... p)
				: base(p...)
			{
			}
			bool on_key_down(unsigned keycode)
			{
				return all_of<on_key_down_binder>(keycode);
			}
			void on_no_message()
			{
				for_each<on_no_message_binder>();
			}
		};

	}

	template <class... Procs>
	// アイドル時にfunctionsを呼び出す
	inline int message_loop(Procs... procs)
	{
		auto procedure = aux_::message_loop_procedure<Procs...>(procs...);

		BOOL ret;
		MSG msg;
		do{
			if(::PeekMessageW(&msg, nullptr, 0, 0, PM_NOREMOVE)){
				ret = ::GetMessageW(&msg, nullptr, 0, 0);
				if(ret == 0 || ret == -1)
					break;
				::TranslateMessage(&msg);
				::DispatchMessageW(&msg);
			}else{
				procedure.on_no_message();
			}
		}while(msg.message != WM_QUIT);

		return msg.wParam;
	}

	inline int message_loop(void)
	{
		BOOL ret;
		MSG msg;
		while((ret = ::GetMessageW(&msg, nullptr, 0, 0)) == 1){
			::TranslateMessage(&msg);
			::DispatchMessageW(&msg);
		}

		return msg.wParam;
	}

	template <class Window, unsigned ID = UINT_MAX /* std::numeric_limits<unsigned>::max() */, unsigned FPS = 60>
	class frame_controller{
		Window &window;
		std::chrono::system_clock::time_point first;
		int count = 0;
		bool flag = true;

	public:
		frame_controller(Window &w): window(w)
		{
		}

		void on_no_message(...)
		{
			using namespace std::chrono;
			using std::this_thread::sleep_for;

			milliseconds duration;

			if(count == 0){
				if(flag){
					flag = false;
				}else{
					duration = duration_cast<milliseconds>(first - system_clock::now()) + seconds(1);
				}
			}else{
				duration = duration_cast<milliseconds>(first - system_clock::now()) + count * milliseconds(1000) / FPS;
			}

			sleep_for(duration);

			auto now = system_clock::now();

			if(count == 0)
				first = now;

			++count;
			if(count >= FPS)
				count = 0;

			window.WindowProc(window.get_hwnd(), WM_TIMER, ID, 0);
			window.repaint();
		}
	};

	template <unsigned ID = UINT_MAX, unsigned FPS = 60, class Window>
	frame_controller<Window, ID, FPS> make_frame_controller(Window &window)
	{
		return frame_controller<Window, ID, FPS>(window);
	}

} }
