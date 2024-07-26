#pragma once

#include "./type.h"
#include "./plot.h"
#include "./reader.h"
#include "./port.h"
#include "./buffer.h"
#include "./defined.h"

NGS_LIB_MODULE_BEGIN

struct instance
{
	using point_type = point_t;

	instance()
	{
		_buffer.resize(8192);
		_plot.set_independent(::std::views::iota(0, 8192));
	}

	void render_port()
	{
		_port.render();
	}
	void update_port()
	{
		
	}

	void render_reader()
	{
		_reader.render(_reader.is_open());
	}

	void update_reader()
	{
		if(_reader.is_change().open_or_close)
		{
			if (!_reader.is_open())
			{
				_reader.open(_port.info().host.data(), _port.info().service.data());
			}
			else
			{
				_reader.close();
			}
		}
		_reader.poll();
		if(_reader.buffer().size() > 16)
		{
			auto unit_buffer = ::std::as_writable_bytes(_buffer.packet_buffer());
			::std::ranges::copy(_reader.buffer() | ::std::views::take(16), unit_buffer.begin());
			_reader.pop_buffer(16);
			_buffer.transfer();
		}
	}

	void render_message()
	{
		auto alpha = _reader.is_open() ? 1.0f : 0.5f;
		::ImGui::PushStyleVar(::ImGuiStyleVar_Alpha, ::ImGui::GetStyle().Alpha * alpha);

		::ImGui::PopStyleVar();
	}

	void render_plot()
	{
		_plot.render("channel 0", { 900,200 }, _buffer.channel<0>(), ::ImPlotFlags_::ImPlotFlags_NoInputs);
		_plot.render("channel 1", { 900,200 }, _buffer.channel<1>(), ::ImPlotFlags_::ImPlotFlags_NoInputs);
		_plot.render("channel 2", { 900,200 }, _buffer.channel<2>(), ::ImPlotFlags_::ImPlotFlags_NoInputs);
		_plot.render("channel 3", { 900,200 }, _buffer.channel<3>(), ::ImPlotFlags_::ImPlotFlags_NoInputs);
		_plot.render("channel 4", { 900,200 }, _buffer.channel<4>(), ::ImPlotFlags_::ImPlotFlags_NoInputs);
		_plot.render("channel 5", { 900,200 }, _buffer.channel<5>(), ::ImPlotFlags_::ImPlotFlags_NoInputs);
		_plot.render("channel 6", { 900,200 }, _buffer.channel<6>(), ::ImPlotFlags_::ImPlotFlags_NoInputs);
		_plot.render("channel 7", { 900,200 }, _buffer.channel<7>(), ::ImPlotFlags_::ImPlotFlags_NoInputs);
	}

	void update()
	{
		update_reader();
		update_port();
	}

	plot _plot{};
	reader _reader{};
	port _port{};
	buffer _buffer{};
};

NGS_LIB_MODULE_END