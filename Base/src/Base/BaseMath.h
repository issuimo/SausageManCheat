#pragma once
#include <pch.h>
#include "imgui.h"

namespace BaseMath {
	static auto RotatePoint(const ImVec2 xy, const ImVec2 xy2, const float theta, ImVec2& out) -> void {
		// 将角度转换为弧度
		const float radian = theta * 3.14 / 180.0;

		// 计算sin和cos值
		const float sinValue = std::sin(radian);
		const float cosValue = std::cos(radian);

		// 计算坐标差值
		const float deltaX = xy.x - xy2.x;
		const float deltaY = xy.y - xy2.y;

		// 计算旋转后的坐标
		out.x = deltaX * cosValue - deltaY * sinValue + xy2.x;
		out.y = deltaY * cosValue + deltaX * sinValue + xy2.y;
	}
}
