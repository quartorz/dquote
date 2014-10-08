#pragma once

#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>
#include <comdef.h>

#include <memory>

namespace{
	struct D2D{
	}D2D;
	struct DWrite{
	}DWrite;
	struct WIC{
	}WIC;
}

namespace dquote{ namespace direct2d{

	namespace detail{

		_COM_SMARTPTR_TYPEDEF(ID2D1Factory, __uuidof(ID2D1Factory));
		_COM_SMARTPTR_TYPEDEF(IDWriteFactory, __uuidof(IDWriteFactory));
		_COM_SMARTPTR_TYPEDEF(IWICImagingFactory, __uuidof(IWICImagingFactory));

	}

	class factory{
		friend bool initialize();
		friend void uninitialize();

		static detail::ID2D1FactoryPtr d2dfactory;
		static detail::IDWriteFactoryPtr dwritefactory;
		static detail::IWICImagingFactoryPtr wicfactory;

		factory(int dummy)
		{
			using namespace ::_com_util;

			CheckError(::D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &d2dfactory));
			CheckError(::DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(dwritefactory), reinterpret_cast<IUnknown**>(&dwritefactory)));
			CheckError(wicfactory.CreateInstance(CLSID_WICImagingFactory, nullptr));
		}

	public:
		factory()
		{
			d2dfactory.AddRef();
			dwritefactory.AddRef();
			wicfactory.AddRef();
		}

		~factory()
		{
			d2dfactory.GetInterfacePtr()->Release();
			dwritefactory.GetInterfacePtr()->Release();
			wicfactory.GetInterfacePtr()->Release();
		}

		ID2D1Factory *operator[](struct D2D) const
		{
			return d2dfactory;
		}

		IDWriteFactory *operator[](struct DWrite) const
		{
			return dwritefactory;
		}

		IWICImagingFactory *operator[](struct WIC) const
		{
			return wicfactory;
		}
	};

	namespace detail{

		extern ::std::unique_ptr<::dquote::direct2d::factory> factory;

	}

} }
