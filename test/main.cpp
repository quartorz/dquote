#include "dquote/dquote.hpp"

#include "dquote/win32/window.hpp"
#include "dquote/win32/create.hpp"
#include "dquote/win32/handlers.hpp"
#include "dquote/win32/message_loop.hpp"

class ww: public dquote::win32::window<ww, dquote::win32::quit_on_close>{
public:
	static const wchar_t *get_class_name()
	{
		return L"W";
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
	ww w;

	if(!w.register_class())
		return 0;
	if(!dquote::win32::create(w, nullptr, L""))
		return 0;

	w.show();

	return dquote::win32::message_loop();
}

DQUOTE_DEFINE_MAIN
