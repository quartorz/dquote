#include <dquote/dquote.hpp>

#include <dquote/win32/window.hpp>
#include <dquote/win32/create.hpp>
#include <dquote/win32/handlers.hpp>
#include <dquote/win32/message_loop.hpp>

#include <dquote/pp/overload.hpp>
#include <dquote/pp/is_empty.hpp>

class window : public dquote::win32::window<window, dquote::win32::quit_on_close> {
public:
	static const wchar_t *get_class_name()
	{
		return L"dquote sample";
	}

	bool initialize()
	{
		return true;
	}

	void uninitialize()
	{
	}
};

int run()
{
	window w;

	if(!window::register_class())
		return 0;
	if(!dquote::win32::create(w, nullptr, L""))
		return 0;

	w.show();

	return dquote::win32::message_loop(
		dquote::win32::key_down([](unsigned keycode) {
			::MessageBox(nullptr, L"", L"", 0);
		})
	);
}

DQUOTE_DEFINE_MAIN
