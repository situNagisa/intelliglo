#pragma once

#include "./udp.h"
#include "./defined.h"

NGS_LIB_BEGIN

struct application
{

	void run()
	{
		while (!_glfw.window().should_close())
		{
			{
				auto guard = _gui.render();
				constexpr auto window_flag =
					ImGuiWindowFlags_NoTitleBar
					 | ImGuiWindowFlags_NoMove
					 | ImGuiWindowFlags_NoResize
					//	| ImGuiWindowFlags_AlwaysAutoResize
					| ImGuiWindowFlags_NoBackground;
				::ImGui::SetNextWindowPos({ 8,23 }, ImGuiCond_FirstUseEver);
				::ImGui::SetNextWindowSize({ 360,420 }, ImGuiCond_FirstUseEver);
				if (::ImGui::Begin("left", nullptr, window_flag))
				{
					_udp.render_left_config();
					::ImGui::End();
				}
				::ImGui::SetNextWindowPos({ 8,23 + 420 }, ImGuiCond_FirstUseEver);
				::ImGui::SetNextWindowSize({ 360,420 }, ImGuiCond_FirstUseEver);
				if (::ImGui::Begin("right", nullptr, window_flag))
				{
					_udp.render_right_config();
					::ImGui::End();
				}
				::ImGui::SetNextWindowPos({ 371,16 }, ImGuiCond_FirstUseEver);
				::ImGui::SetNextWindowSize({ 620,869 }, ImGuiCond_FirstUseEver);
				if (::ImGui::Begin("left hand data", nullptr, window_flag))
				{
					_udp.render_left_plot();
					::ImGui::End();
				}
				::ImGui::SetNextWindowPos({ 371 + 620,16 }, ImGuiCond_FirstUseEver);
				::ImGui::SetNextWindowSize({ 620,869 }, ImGuiCond_FirstUseEver);
				if (::ImGui::Begin("right hand data", nullptr, window_flag))
				{
					_udp.render_right_plot();
					::ImGui::End();
				}
			}
			_udp.update();
		}
	}

	::quick_ui::guard::glfw _glfw{ "data gloves", 1600, 900 };
	::quick_ui::guard::imgui _gui{ _glfw , "./misc/fonts/Roboto-Medium.ttf" };
	udps::instance _udp{};
};

NGS_LIB_END