#pragma once

#include "../tmp/applier.hpp"
#include "../tmp/tautology.hpp"
#include "../tmp/index_tuple.hpp"

#include <type_traits>
#include <utility>

namespace dquote{ namespace base{

	template <class... Procs>
	class procedure{
	protected:
		using applier_type = ::dquote::tmp::applier<Procs...>;

		applier_type procs;

		template <template <class> class Pred>
		class make_filter{
		public:
			template <::std::size_t I>
			using type = Pred<typename ::std::tuple_element<I, typename applier_type::tuple_type>::type>;
		};

		template <class Binder, class... Args>
		void for_each(Args... args)
		{
			for_each<Binder, ::dquote::tmp::tautological_filter>(::std::forward<Args>(args)...);
		}

		template <class Binder, template <class> class Pred, class... Args>
		void for_each(Args... args)
		{
			for_each<Binder, typename make_filter<Pred>::type>(::std::forward<Args>(args)...);
		}

		template <class Binder, template <::std::size_t> class Filter, class... Args>
		void for_each(Args... args)
		{
			for_each<Binder>(typename ::dquote::tmp::make_filtered_index<Filter, sizeof...(Procs)>::type(), ::std::forward<Args>(args)...);
		}

		template <class Binder, ::std::size_t... Indices, class... Args>
		void for_each(::dquote::tmp::index_tuple<Indices...> indices, Args... args)
		{
			procs.call(indices, Binder(), static_cast<typename Binder::type*>(this), ::std::forward<Args>(args)...);
		}

		template <class Binder, class... Args>
		bool all_of(Args... args)
		{
			return all_of<Binder, ::dquote::tmp::tautological_filter>(::std::forward<Args>(args)...);
		}

		template <class Binder, template <class> class Pred, class... Args>
		bool all_of(Args... args)
		{
			return all_of<Binder, typename make_filter<Pred>::type>(::std::forward<Args>(args)...);
		}

		template <class Binder, template <::std::size_t> class Filter, class... Args>
		bool all_of(Args... args)
		{
			return all_of<Binder>(typename ::dquote::tmp::make_filtered_index<Filter, sizeof...(Procs)>::type(), std::forward<Args>(args)...);
		}

		template <class Binder, ::std::size_t Index, ::std::size_t... Rest, class... Args>
		bool all_of(::dquote::tmp::index_tuple<Index, Rest...> indices, Args... args)
		{
			using result_type = decltype(Binder()(::std::get<Index>(procs.get()), static_cast<typename Binder::type*>(this), args...));
			return to_bool<result_type, Binder, Index>(args...)
				? all_of<Binder>(::dquote::tmp::index_tuple<Rest...>(), ::std::forward<Args>(args)...)
				: false;
		}

		template <class Binder, class... Args>
		bool all_of(::dquote::tmp::index_tuple<> indices, Args... args)
		{
			return true;
		}

	private:

		template <class Result, class Binder, ::std::size_t Index, class... Args>
		typename ::std::enable_if<::std::is_convertible<Result, bool>::value, bool>::type to_bool(Args... args)
		{
			return static_cast<bool>(Binder()(::std::get<Index>(procs.get()), static_cast<typename Binder::type*>(this), args...));
		}
		template <class Result, class Binder, ::std::size_t Index, class... Args>
		typename std::enable_if<!::std::is_convertible<Result, bool>::value, bool>::type to_bool(Args... args)
		{
			Binder()(::std::get<Index>(procs.get()), static_cast<typename Binder::type*>(this), args...);
			return true;
		}
	};

} }
