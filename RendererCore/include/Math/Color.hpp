/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Core.hpp"

namespace LCH::Math
{
	class Color
	{
	public:
		Color(float c = {}) { m_channels[0] = m_channels[1] = m_channels[2] = c; m_channels[3] = 1.0f; }
		Color(float r, float g, float b, float a)
		{
			m_channels[0] = r;
			m_channels[1] = g;
			m_channels[2] = b;
			m_channels[3] = a;
		}

		const float& r() const { return m_channels[0]; }
		const float& g() const { return m_channels[1]; }
		const float& b() const { return m_channels[2]; }
		const float& a() const { return m_channels[3]; }
		float& r() { return m_channels[0]; }
		float& g() { return m_channels[1]; }
		float& b() { return m_channels[2]; }
		float& a() { return m_channels[3]; }

		float const* RGBA() const { return m_channels.data(); }

		Color operator*(const Color& color) const
		{
			Color result;
			return result;
		}
		Color operator*(float c) const
		{
			Color result(c);
			result.m_channels[0] *= m_channels[0];
			result.m_channels[1] *= m_channels[1];
			result.m_channels[2] *= m_channels[2];
			result.m_channels[3] *= m_channels[3];
			return result;
		}

	private:
		Vector4f m_channels;

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
		static Color DarkOrchid;			// 深紫兰色
		static Color DarkRed;				// 深红色
		static Color DarkSalmon;			// 深橙红色
		static Color DarkSeaGreen;			// 深海绿色
		static Color DarkSlateBlue;			// 深板岩蓝
		static Color DarkSlateGray;			// 深板岩灰
		static Color DarkTurquoise;			// 深绿松石色
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
		static Color Goldenrod;				// 秋麒麟色
		static Color Gray;					// 灰色
		static Color Green;					// 绿色
		static Color GreenYellow;			// 黄绿色
		static Color Honeydew;				// 哈密瓜色
		static Color HotPink;				// 亮粉色
		static Color IndianRed;				// 印第安红
		static Color Indigo;				// 靛蓝色
		static Color Ivory;					// 象牙白
		static Color Khaki;					// 卡其色(黄褐色)
		static Color Lavender;				// 薰衣草色(淡紫色)
		static Color LavenderBlush;			// 淡紫红
		static Color LawnGreen;				// 草坪绿
		static Color LemonChiffon;			// 柠檬绸色
		static Color LightBlue;				// 浅蓝色
		static Color LightCoral;			// 浅珊瑚色
		static Color LightCyan;				// 浅青绿色
		static Color LightGoldenrodYellow;	// 浅秋麒麟色
		static Color LightGreen;			// 浅绿色
		static Color LightGray;				// 浅灰色
		static Color LightPink;				// 浅粉色
		static Color LightSalmon;			// 浅橙红色
		static Color LightSeaGreen;			// 浅海绿色
		static Color LightSkyBlue;			// 浅天空蓝
		static Color LightSlateGray;		// 浅板岩灰
		static Color LightSteelBlue;		// 浅钢铁蓝
		static Color LightYellow;			// 浅黄色
		static Color Lime;					// 青柠色
		static Color LimeGreen;				// 青柠绿
		static Color Linen;					// 亚麻色
		static Color Magenta;				// 洋红色
		static Color Maroon;				// 褐红色
		static Color MediumAquamarine;		// 间碧绿色
		static Color MediumBlue;			// 间蓝色
		static Color MediumOrchid;			// 间紫兰色
		static Color MediumPurple;			// 间紫红色
		static Color MediumSeaGreen;		// 间海绿色
		static Color MediumSlateBlue;		// 间板岩蓝
		static Color MediumSpringGreen;		// 间春绿色
		static Color MediumTurquoise;		// 间绿松石色
		static Color MediumVioletRed;		// 间紫罗兰红
		static Color MidnightBlue;			// 午夜蓝
		static Color MintCream;				// 薄荷奶油色
		static Color MistyRose;				// 粉玫瑰红
		static Color Moccasin;				// 鹿皮色
		static Color NavajoWhite;			// 纳瓦白
		static Color Navy;					// 藏青色(海军蓝)
		static Color OldLace;				// 旧布黄
		static Color Olive;					// 橄榄绿
		static Color OliveDrab;				// 深绿褐色
		static Color Orange;				// 橘黄色
		static Color OrangeRed;				// 橘红色
		static Color Orchid;				// 兰花紫
		static Color PaleGoldenrod;			// 苍菊黄色
		static Color PaleGreen;				// 苍绿色
		static Color PaleTurquoise;			// 苍宝石绿
		static Color PaleVioletRed;			// 苍紫红色
		static Color PapayaWhip;			// 番木瓜色
		static Color PeachPuff;				// 桃色
		static Color Peru;					// 秘鲁色
		static Color Pink;					// 粉色
		static Color Plum;					// 李子色(浅紫色)
		static Color PowderBlue;			// 粉末蓝
		static Color Purple;				// 紫红色
		static Color Red;					// 红色
		static Color RosyBrown;				// 玫瑰棕色
		static Color RoyalBlue;				// 皇家蓝
		static Color SaddleBrown;			// 马鞍棕
		static Color Salmon;				// 鲑鱼红
		static Color SandyBrown;			// 沙褐色
		static Color SeaGreen;				// 海绿色
		static Color SeaShell;				// 海贝色
		static Color Sienna;				// 赭色(褐色)
		static Color Silver;				// 银色
		static Color SkyBlue;				// 天蓝色
		static Color SlateBlue;				// 板岩蓝
		static Color SlateGray;				// 板岩灰
		static Color Snow;					// 雪白色
		static Color SpringGreen;			// 春绿色
		static Color SteelBlue;				// 钢铁蓝
		static Color Tan;					// 棕褐色
		static Color Teal;					// 蓝绿色
		static Color Thistle;				// 蓟色
		static Color Tomato;				// 番茄色
		static Color Transparent;			// 透明
		static Color Turquoise;				// 绿松石色
		static Color Violet;				// 紫色
		static Color Wheat;					// 小麦色
		static Color White;					// 白色
		static Color WhiteSmoke;			// 白烟色
		static Color Yellow;				// 黄色
		static Color YellowGreen;			// 黄绿色
	};

	inline Color operator*(float c, const Color& color)
	{
		return color * c;
	}
}