#pragma once

#include "./type.h"
#include "./buffer.h"
#include "./defined.h"

NGS_LIB_MODULE_BEGIN

struct reader
{
	NGS_PP_INJECT_BEGIN(reader);
public:
	/*
	struct parser
	{
		using protoc_type = ::ngs::mpl::mstruct::storage<::ngs::layout::no_align,
			::std::array<::std::byte, 2>,
			::std::array<point_t, 8>,
			::std::byte
		>;
		using context_type = ::std::size_t;

		template<class T>
		consteval static decltype(auto) buffer_concept()
		{
			return ::std::ranges::sized_range<T> && ::std::ranges::input_range<T> && ::std::convertible_to<::std::ranges::range_value_t<T>, ::std::byte>;
		}

		template<class T>
			requires (buffer_concept<T>())
		constexpr static decltype(auto) create_context(const T& buffer)
		{
			return context_type();
		}

		template<class T>
			requires (buffer_concept<T>())
		constexpr static decltype(auto) check(const T& buffer, context_type& context)
		{
			::std::array<::std::byte, ::ngs::mpl::mstruct::struct_size_v<protoc_type>> memory{};
			if (::std::ranges::size(buffer) < sizeof(memory))
				return false;
			::std::ranges::copy(buffer | ::std::views::transform([](auto&& t) { return static_cast<::std::byte>(t); }), ::std::ranges::begin(memory));
			auto protoc = ::std::bit_cast<protoc_type>(memory);
			context = 1;
			if(!::std::ranges::equal(::ngs::mpl::mstruct::get<0>(protoc), ::std::array{::std::byte{'i'}, ::std::byte{'g'}}))
			{
				return false;
			}
			constexpr auto byte_plus = [](auto&& l, auto&& r)
				{
					return ::std::to_integer<::std::uint8_t>(static_cast<::std::byte>(l)) + ::std::to_integer<::std::uint8_t>(static_cast<::std::byte>(r));
				};
			if(static_cast<::std::uint8_t>(::std::ranges::fold_left_first(::ngs::mpl::mstruct::get<1>(protoc),byte_plus)) != ::ngs::mpl::mstruct::get<2>(protoc))
			{
				return false;
			}
			return true;
		}


	};
	*/
	auto open(::std::string_view host, ::std::string_view service)
	{
		::boost::system::error_code error_code;
		_io.open(::boost::asio::ip::udp::v4(), error_code);
		if (error_code)
		{
			NGS_LOGL(error, "open serial port error: ", error_code.message());
			return false;
		}
		::boost::asio::ip::udp::resolver resolver(_context);
		_io.bind(*resolver.resolve({ ::boost::asio::ip::udp::v4(), host.data(), service.data() }));

		auto endpoint = _io.local_endpoint();
		NGS_LOGL(info, ::std::format("udp socket open {}: {}", endpoint.address().to_string(), endpoint.port()));

		if (!_io.is_open())
		{
			return false;
		}

		::boost::asio::co_spawn(_context, _receive(), ::boost::asio::detached);

		return true;
	}

	auto close()
	{
		::boost::system::error_code error_code;
		_io.close(error_code);
		if (error_code)
		{
			NGS_LOGL(error, "open serial port error: ", error_code.message());
			return false;
		}
		NGS_LOGL(info, "close udp");
		_context.reset();

		return true;
	}

	auto is_open() const { return _io.is_open(); }

	::boost::asio::awaitable<void> _receive()
	{
		::std::array<::std::byte, 256> buffer{};
		while (true)
		{
			auto n = co_await _io.async_receive_from(::boost::asio::buffer(buffer), _endpoint, ::boost::asio::use_awaitable);
			for (auto i : buffer | ::std::views::take(n))
			{
				_buffer.push_back(i);
			}
			NGS_LOGL(debug, ::std::ranges::fold_left_first(
				buffer 
				| ::std::views::take(n) 
				| ::std::views::transform([](auto&& b) { return ::std::format("{:#02x}, ", ::std::to_integer<::std::uint8_t>(b)); }),
				::std::plus<>()
				).value());
		}
	}

	void render(bool is_open)
	{
		_is_change.open_or_close = ::ImGui::Button(is_open ? "close" : "open", { 100, 0 });
	}

	decltype(auto) poll()
	{
		_context.poll();
	}

	auto&& is_change() const
	{
		return _is_change;
	}

	auto&& buffer(this auto&& self) 
	{
		return self._buffer;
	}

	void pop_buffer(::std::size_t n)
	{
		for (auto i = 0; i < n; ++i)
		{
			_buffer.pop_front();
		}
	}

	::boost::asio::io_context _context{};
	::boost::asio::ip::udp::socket _io{ _context };
	::boost::asio::ip::udp::endpoint _endpoint;
	::std::deque<::std::byte> _buffer{};

	struct
	{
		bool open_or_close : 1 = false;
	}_is_change{};
};

NGS_LIB_MODULE_END