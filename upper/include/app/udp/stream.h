#pragma once

#include "./defined.h"

NGS_LIB_MODULE_BEGIN

template<class T, class Buffer>
	requires requires(Buffer buffer) { T::check(buffer); }
using checker_context_t = decltype(T::check(::std::declval<Buffer>()));

template<class T, class Buffer>
concept checker = requires(Buffer buffer)
{
	{ T::check(NGS_PP_PERFECT_FORWARD(buffer)) };
	{ T::is_success(T::check(NGS_PP_PERFECT_FORWARD(buffer))) } -> ::std::convertible_to<bool>;

};

struct checker
{
	using buffer_type = ::std::span<::std::byte>;
	using context_type = ::std::size_t;

	template<class T>
	constexpr static bool result_concept()
	{
		return true;
	}

	constexpr static context_type check(const buffer_type& buffer)
	{
		return (::std::ranges::size(buffer) >= 10) * 10;
	}

	constexpr static bool is_success(const context_type& context)
	{
		return context > 0;
	}

	constexpr static ::std::size_t parse_to(const buffer_type& buffer, context_type& context, auto&& result)
		requires (result_concept<decltype(result)>())
	{
		::std::ranges::copy(buffer | ::std::views::take(context), result);
		return context;
	}
};

void process(::std::span<::std::byte> buffer, ::std::array<::std::byte,1>::iterator output)
{
	auto context = checker::check(buffer);
	if (!checker::is_success(context))
	{
		return;
	}

	checker::parse_to(buffer, context, output);
}

NGS_LIB_MODULE_END