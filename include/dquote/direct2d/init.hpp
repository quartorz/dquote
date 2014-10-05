#pragma once

#include "factory.hpp"

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")
#pragma comment(lib, "dwmapi")
#pragma comment(lib, "uxtheme")

namespace dquote{ namespace direct2d{

	namespace detail{

		__declspec(selectany) std::unique_ptr<::dquote::direct2d::factory> factory;

	}

	__declspec(selectany) detail::ID2D1FactoryPtr factory::d2dfactory;
	__declspec(selectany) detail::IDWriteFactoryPtr factory::dwritefactory;
	__declspec(selectany) detail::IWICImagingFactoryPtr factory::wicfactory;

	bool initialize()
	{
		try{
			detail::factory = std::unique_ptr<factory>(new factory(int()));
		}catch(...){
			return false;
		}

		return true;
	}

	void uninitialize()
	{
		detail::factory.reset();

		factory::d2dfactory.Detach();
		factory::dwritefactory.Detach();
		factory::wicfactory.Detach();
	}

} }
