#pragma once

#include <Windows.h>

#include <Base/events/event.hpp>

namespace events
{
	extern TCancelableEvent<short> KeyUpEvent;
	extern TCancelableEvent<HWND, UINT, WPARAM, LPARAM> WndProcEvent;
	extern TEvent<> RenderEvent;
}