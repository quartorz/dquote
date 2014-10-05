#pragma once

#include "../../tmp/applier.hpp"
#include "../../tmp/valid.hpp"
#include "../../tmp/index_tuple.hpp"

namespace dquote{ namespace base{ namespace window{

	template <class Derived, class... Procs>
	class window{
	protected:
		using procs_type = ::dquote::tmp::applier<Procs...>;

		procs_type procs;

		template <template <class> class Pred>
		class make_filter{
		public:
			template <std::size_t I>
			using type = Pred<typename std::tuple_element<I, typename procs_type::tuple_type>::type>;
		};

		template <class Binder>
		void call_proc()
		{
			call_proc<::dquote::tmp::valid, Binder>();
		}

		template <template <class> class Pred, class Binder>
		void call_proc()
		{
			call_proc<make_filter<Pred>, Binder>();
		}

		template <template <std::size_t> class Pred, class Binder>
		void call_proc()
		{
			call_proc<Binder>(typename ::dquote::tmp::make_filtered_index<Pred, sizeof...(Procs)>::type());
		}

		template <class Binder, std::size_t... Indices>
		void call_proc(::dquote::tmp::index_tuple<Indices...> indices)
		{
			procs.call(indices, Binder(), static_cast<typename Binder::type*>(this));
		}

	public:
	};

} } }
