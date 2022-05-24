/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Simd.hpp"
#include <DirectXColors.h>

namespace LCH::Math
{
	class Color
	{
		using simd = simd_trait<float, SimdInstruction<float, 4>::type>;
	public:
		Color() { elements.fill(0); }
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

		const float* RGBA() const { return elements.data(); }

		Color operator*(const Color& color)
		{
			Color result;
			simd::elementwise_product(elements.data(), color.elements.data(), result.elements.data());
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
	};
}