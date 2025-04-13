#pragma once

#include "./type.h"
#include "./plot.h"
#include "./reader.h"
#include "./port.h"
#include "./buffer.h"
#include "./saver.h"
#include "./defined.h"

NGS_LIB_MODULE_BEGIN

struct hand
{
	hand(::std::size_t id) : _id(id)
	{
		_buffer.resize(512);
		//::std::ranges::copy(::std::views::iota(0, 64) | ::std::views::transform([](auto&& v) { return unit_t{ static_cast<point_t>(v * 10), }; }), _buffer.packet_buffer().begin());
		//_buffer.transfer();
		_plot.set_independent(::std::views::iota(0u, _buffer.size()));
		auto&& type = _id == 0 ? "left_" : "right" ;
		::std::ranges::copy(type, _saver._type.begin());
		if(_id == 0)
		{
			_port._info.service = ::std::array<char,32>("8001");
		}
		else
		{
			_port._info.service = ::std::array<char, 32>("8002");
		}
	}

	void render_port()
	{
		_port.render();
	}
	void update_port()
	{

	}

	void open_port()
	{
		if(!_reader.open(_port.info().host.data(), _port.info().service.data()))
		{
			NGS_LOGL(error, "open port failed");
		}
	}

	void close_port()
	{
		if (!_reader.close())
		{
			NGS_LOGL(error, "close port failed");
		}
	}
	bool recording() const
	{
		return _saver.channel_output_is_open(0);
	}

	void render_reader()
	{
		_reader.render(_reader.is_open());
	}

	void update_reader()
	{
		if(_reader.is_change().open_or_close)
		{
			if (_reader.is_open())
			{
				close_port();
			}
			else
			{
				open_port();
			}
		}
		_reader.poll();
		while (_reader.buffer().size() >= 16)
		{
			auto unit_buffer = ::std::as_writable_bytes(_buffer.packet_buffer());
			::std::ranges::copy(_reader.buffer() | ::std::views::take(16), unit_buffer.begin());
			_reader.pop_buffer(16);
			_buffer.transfer(1);

			if (recording())
			{
				_saver.write_channel_output_file(0, unit_buffer.data(), unit_buffer.size());
			}
		}
	}

	void render_plot()
	{
		_plot.render("channel 0", { 590,200 }, _buffer.channel<0>(), ::ImPlotFlags_::ImPlotFlags_NoInputs);
		_plot.render("channel 1", { 590,200 }, _buffer.channel<1>(), ::ImPlotFlags_::ImPlotFlags_NoInputs);
		_plot.render("channel 2", { 590,200 }, _buffer.channel<2>(), ::ImPlotFlags_::ImPlotFlags_NoInputs);
		_plot.render("channel 3", { 590,200 }, _buffer.channel<3>(), ::ImPlotFlags_::ImPlotFlags_NoInputs);
		_plot.render("channel 4", { 590,200 }, _buffer.channel<4>(), ::ImPlotFlags_::ImPlotFlags_NoInputs);
		_plot.render("channel 5", { 590,200 }, _buffer.channel<5>(), ::ImPlotFlags_::ImPlotFlags_NoInputs);
		_plot.render("channel 6", { 590,200 }, _buffer.channel<6>(), ::ImPlotFlags_::ImPlotFlags_NoInputs);
		_plot.render("channel 7", { 590,200 }, _buffer.channel<7>(), ::ImPlotFlags_::ImPlotFlags_NoInputs);
	}

	void render_saver()
	{
		_saver.render(recording());
	}
	void update_saver()
	{
		if(_saver.is_change().save_or_close)
		{
			if (recording())
			{
				_saver.close_channel_output_file(0);
			}
			else
			{
				_saver.open_channel_output_file(0);
			}
		}
	}

	void update()
	{
		update_saver();
		update_port();
		update_reader();
	}

	::std::size_t _id;
	plot _plot{};
	reader _reader{};
	port _port{};
	buffer _buffer{};
	saver _saver{};

	bool _record_success = false;
};

NGS_LIB_MODULE_END