/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Simd.hpp"
#include <DirectXColors.h>
#include <unordered_map>

namespace LCH::Math
{
	class Color
	{
		using simd = simd_trait<float, SimdInstruction<float, 4>::type>;
	public:
		Color(float c = {}) { elements.fill(c); }
		Color(float r, float g, float b, float a)
		{
			elements[0] = r;
			elements[1] = g;
			elements[2] = b;
			elements[3] = a;
		}

		const float& r() const { return elements[0]; }
		const float& g() const { return elements[1]; }
		const float& b() const { return elements[2]; }
		const float& a() const { return elements[3]; }
		float& r() { return elements[0]; }
		float& g() { return elements[1]; }
		float& b() { return elements[2]; }
		float& a() { return elements[3]; }

		float const* RGBA() const { return elements.data(); }

		Color operator*(const Color& color) const
		{
			Color result;
			simd::elementwise_product(elements.data(), color.elements.data(), result.elements.data());
			return result;
		}
		Color operator*(float c) const
		{
			Color result(c);
			simd::elementwise_product(elements.data(), result.elements.data(), result.elements.data());
			return result;
		}

	private:
		aligned_array<float, 4, simd::Alignment> elements;

	public:
		static Color AliceBlue;				// 爱丽丝蓝
		static Color AntiqueWhite;			// 古董白
		static Color Aqua;					// 浅绿色
		static Color Aquamarine;			// 碧绿色
		static Color Azure;					// 蔚蓝色
		static Color Beige;					// 米黄色
		static Color Bisque;				// 橘黄色
		static Color Black;					// 黑色
		static Color Blanchedalmond;		// 杏仁白
		static Color Blue;					// 蓝色
		static Color BlueViolet;			// 紫罗兰色
		static Color Brown;					// 棕色
		static Color BurlyWood;				// 实木色
		static Color Chartreuse;			// 查特酒绿
		static Color Chocolate;				// 巧克力色
		static Color Coral;					// 橘红色
		static Color CornflowerBlue;		// 矢车菊蓝
		static Color Cornsilk;				// 玉米丝色
		static Color Crimson;				// 深红色
		static Color Cyan;					// 青色
		static Color DarkBlue;				// 深蓝色
		static Color DarkCyan;				// 深青色
		static Color DarkGoldenrod;			// 深金黄色
		static Color DarkGrey;				// 深灰色
		static Color DarkGreen;				// 深绿色
		static Color DarkKhaki;				// 深卡其色(深黄褐色)
		static Color DarkMagenta;			// 深洋红色
		static Color DarkOliverGreen;		// 深橄榄绿
		static Color DarkOrange;			// 深橘黄色
		static Color DarkOrchid;			// 深紫色
		static Color DarkRed;				// 深红色
		static Color DarkSalmon;			// 深橙红色
		static Color DarkSeaGreen;			// 深海绿色
		static Color DarkSlateBlue;			// 深板岩蓝
		static Color DarkSlateGray;			// 深板岩灰
		static Color DarkTurquoise;			// 深宝石绿
		static Color DarkViolet;			// 深紫色
		static Color DeepPink;				// 深粉红色
		static Color DeepSkyBlue;			// 深天蓝色
		static Color DimGray;				// 暗灰色
		static Color DodgerBlue;			// 道奇蓝(闪蓝色)
		static Color Firebrick;				// 砖色
		static Color FloralWhite;			// 花白色
		static Color ForestGreen;			// 森林绿
		static Color Fuchsia;				// 紫红色
		static Color Gainsboro;				// 淡灰色
		static Color GhostWhite;			// 苍白色
		static Color Gold;					// 金色
	};

	inline Color operator*(float c, const Color& color)
	{
		return color * c;
	}
}