#pragma once

#include <Windows.h>

namespace dquote{ namespace win32{

	struct quit_on_close{
		template <class Window>
		void on_close(Window &w)
		{
			::PostQuitMessage(0);
		}
	};

} }
