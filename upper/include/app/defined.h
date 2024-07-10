#pragma once

#define NGS_USE_HPP

#include <boost/asio.hpp>
#include "imgui.h"
#include "implot.h"

#include "quick_ui/quick_ui.h"

#include "NGS/basic/basic.h"
#include "NGS/assert/assert.h"
#include "NGS/external/stl/range.h"
#include "NGS/external/glfw.h"
#include "NGS/external/imgui/component/component.h"

#define NGS_LIB_GLOBAL_BEGIN
#define NGS_LIB_GLOBAL_END

#define NGS_LIB_NAME apps
#define NGS_LIB_VERSION_MAJOR 1
#define NGS_LIB_VERSION_MINOR 0
#define NGS_LIB_VERSION_PATCH 0
#include "NGS/lib/lib.h"