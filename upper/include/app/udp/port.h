#pragma once

#include "./defined.h"

NGS_LIB_MODULE_BEGIN

struct port
{
	NGS_PP_INJECT_BEGIN(port);
public:

	void render()
	{
		_is_change.host = ::ImGui::InputText("host", _info.host.data(), 0x20);
		_is_change.service = ::ImGui::InputText("service", _info.service.data(), 0x20);
	}


	auto&& info() const { return _info; }
	auto&& is_change() const { return _is_change; }

	struct
	{
		::std::array<char, 0x20> host = { "0.0.0.0" };
		::std::array<char, 0x20> service = { "6665" };
	}_info{};
	struct
	{
		bool host : 1 = false;
		bool service : 1 = false;
	}_is_change{};
};

NGS_LIB_MODULE_END