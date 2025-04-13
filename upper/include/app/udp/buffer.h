#pragma once

#include "./limit.h"
#include "./type.h"
#include "./channels.h"
#include "./defined.h"

NGS_LIB_MODULE_BEGIN

struct buffer
{
	using unit_type = unit_t;
	using point_type = point_t;

	void transfer(::std::size_t size, ::std::size_t active_size)
	{
		auto units = _buffer | ::std::views::take(size);
		_range_limit<0>(units, active_size);
		_range_limit<1>(units, active_size);
		_range_limit<2>(units, active_size);
		_range_limit<3>(units, active_size);
		_range_limit<4>(units, active_size);
		_range_limit<5>(units, active_size);
		_range_limit<6>(units, active_size);
		_range_limit<7>(units, active_size);
		::std::ranges::shift_left(_buffer, static_cast<::std::ptrdiff_t>(size));
	}

	template<::std::size_t Index>
	void _range_limit(::std::ranges::input_range auto&& range, ::std::size_t active_size)
	{
		NGS_LIB_MODULE_NAME::range_limit(range | ::std::views::elements<Index>, channel<Index>(), active_size);
	}

	void transfer()
	{
		transfer(_size, _channels.size());
	}
	void transfer(::std::size_t size)
	{
		transfer(size, _channels.size());
	}

	constexpr void resize(::std::size_t size)
	{
		_size = size;
		_buffer.resize(_size);
		_channels.resize(point_size());
	}

	constexpr void shrink_to_fit()
	{
		_buffer.shrink_to_fit();
		_channels.shrink_to_fit();
	}

	constexpr auto size() const { return _size; }
	constexpr auto point_size() const { return _size; }

	constexpr auto packet_buffer(this auto&& self) { return ::std::span(self._buffer); }
	template<::std::size_t Index>
	constexpr auto channel(this auto&& self) { return self._channels.template channel<Index>(); }

	::std::size_t _size{};
	::std::vector<unit_type> _buffer{};
	channels<point_type, 8> _channels{};
};

NGS_LIB_MODULE_END