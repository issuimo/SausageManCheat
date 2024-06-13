#pragma once
#define _WINSOCKAPI_
#define WIN32_LEAN_AND_MEAN
#include <magic_enum_all.hpp>
#include <json.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <map>
#include <unordered_set>
#include <queue>
#include <sstream>
#include <mutex>
#include <ranges>
#include <optional>
#include <exception>
#include <windows.h>
#include <bcrypt.h>
#include <codecvt>
#include <cwchar>
#include <locale>
#include <cmath>
#include <dbghelp.h>
#include <csignal>

#define IMGUI_ENABLE_TEST_ENGINE
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <imconfig.h>
#include <backends/imgui_impl_dx11.h>
#include <imgui_notify.h>
#include <implot.h>

#include <SimpleIni.h>
#include <fmt/format.h>
#include <detours.h>

#include <Base/Logger.h>
#include <Base/HookManager.h>

#include <Base/events/event.hpp>
#include <Base/events/handlers/methodeventhandler.hpp>
#include <Base/events/handlers/functoreventhandler.hpp>
#include <Base/events/joins/handlereventjoin.hpp>
#include <Base/events/joins/eventjoinwrapper.hpp>

#include <Base/globals.h>
