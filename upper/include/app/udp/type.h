#pragma once

#include "./defined.h"

NGS_LIB_MODULE_BEGIN

using point_t = ::std::uint16_t;
using unit_t = ::std::array<point_t, 8>;
using clock_t = ::std::chrono::system_clock;

NGS_LIB_MODULE_END