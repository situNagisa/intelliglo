#pragma once

#include "./type.h"
#include "./hand.h"
#include "./defined.h"

NGS_LIB_MODULE_BEGIN

struct instance
{
	using point_type = point_t;

	instance()
	{

	}
	void render_left_config()
	{
		_left.render_reader();
		_left.render_port();
		_left.render_saver();
	}
	void render_right_config()
	{
		_right.render_reader();
		_right.render_port();
		_right.render_saver();
	}

	void render_left_plot()
	{
		_left.render_plot();
	}
	void render_right_plot()
	{
		_right.render_plot();
	}

	void update()
	{
		_left.update();
		_right.update();
	}

	hand _left{0};
	hand _right{1};
};

NGS_LIB_MODULE_END