#pragma once

#include "./defined.h"

NGS_LIB_MODULE_BEGIN

struct instance
{
	void update()
	{
		_context.poll();
	}

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

		if(!_io.is_open())
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

		_context.reset();

		return true;
	}

	::boost::asio::awaitable<void> _receive()
	{
		::std::array<::std::byte, 256> buffer{};
		while (true)
		{
			auto n = co_await _io.async_receive_from(::boost::asio::buffer(_buffer), _endpoint,::boost::asio::use_awaitable);
		}
	}

	void render_config()
	{
		
	}

	void render_control()
	{
		if (_io.is_open())
		{
			if (::ImGui::Button("close", { 100, 0 }))
			{
				close();
			}
		}
		else
		{
			if (::ImGui::Button("open", { 100, 0 }))
			{
				open("127.0.0.1", "6666");
			}
		}
	}

	void render_message()
	{
		auto alpha = _io.is_open() ? 1.0f : 0.5f;
		::ImGui::PushStyleVar(::ImGuiStyleVar_Alpha, ::ImGui::GetStyle().Alpha * alpha);

		::ImGui::PopStyleVar();
	}

	::boost::asio::io_context _context{};
	::boost::asio::ip::udp::socket _io{ _context };
	::boost::asio::ip::udp::endpoint _endpoint;
	::std::array<::std::byte, 0x100> _buffer{};
};

NGS_LIB_MODULE_END