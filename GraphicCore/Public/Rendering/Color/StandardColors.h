/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Color.h"

namespace TG::Rendering
{
	//-----------------------------------------------------------------
	//	颜色取自	DirectXColors.h -- C++ Color Math library
	//	Copyright (c) Microsoft Corporation. All rights reserved.
	//	Licensed under the MIT License.
	//	http://go.microsoft.com/fwlink/?LinkID=615560
	//-----------------------------------------------------------------
	// Standard colors (Red/Green/Blue) in sRGB colorspace
	inline const Color AliceBlue				{ 0.941176534f, 0.972549081f, 1.000000000f };	// 爱丽丝蓝
	inline const Color AntiqueWhite				{ 0.980392218f, 0.921568692f, 0.843137324f };	// 古董白
	inline const Color Aqua						{ 0.000000000f, 1.000000000f, 1.000000000f };	// 浅绿色
	inline const Color Aquamarine				{ 0.498039246f, 1.000000000f, 0.831372619f };	// 碧绿色
	inline const Color Azure					{ 0.941176534f, 1.000000000f, 1.000000000f };	// 蔚蓝色
	inline const Color Beige					{ 0.960784376f, 0.960784376f, 0.862745166f };	// 米黄色
	inline const Color Bisque					{ 1.000000000f, 0.894117713f, 0.768627524f };	// 橘黄色
	inline const Color Black					{ 0.000000000f, 0.000000000f, 0.000000000f };	// 黑色
	inline const Color Blanchedalmond			{ 1.000000000f, 0.921568692f, 0.803921640f };	// 杏仁白
	inline const Color Blue						{ 0.000000000f, 0.000000000f, 1.000000000f };	// 蓝色
	inline const Color BlueViolet				{ 0.541176498f, 0.168627456f, 0.886274576f };	// 紫罗兰色
	inline const Color Brown					{ 0.647058845f, 0.164705887f, 0.164705887f };	// 棕色
	inline const Color BurlyWood				{ 0.870588303f, 0.721568644f, 0.529411793f };	// 实木色
	inline const Color Chartreuse				{ 0.498039246f, 1.000000000f, 0.000000000f };	// 查特酒绿
	inline const Color Chocolate				{ 0.823529482f, 0.411764741f, 0.117647067f };	// 巧克力色
	inline const Color Coral					{ 1.000000000f, 0.498039246f, 0.313725501f };	// 橘红色
	inline const Color CornflowerBlue			{ 0.392156899f, 0.584313750f, 0.929411829f };	// 矢车菊蓝
	inline const Color Cornsilk					{ 1.000000000f, 0.972549081f, 0.862745166f };	// 玉米丝色
	inline const Color Crimson					{ 0.862745166f, 0.078431375f, 0.235294133f };	// 深红色
	inline const Color Cyan						{ 0.000000000f, 1.000000000f, 1.000000000f };	// 青色
	inline const Color DarkBlue					{ 0.000000000f, 0.000000000f, 0.545098066f };	// 深蓝色
	inline const Color DarkCyan					{ 0.000000000f, 0.545098066f, 0.545098066f };	// 深青色
	inline const Color DarkGoldenrod			{ 0.721568644f, 0.525490224f, 0.043137256f };	// 深金黄色
	inline const Color DarkGrey					{ 0.662745118f, 0.662745118f, 0.662745118f };	// 深灰色
	inline const Color DarkGreen				{ 0.000000000f, 0.392156899f, 0.000000000f };	// 深绿色
	inline const Color DarkKhaki				{ 0.741176486f, 0.717647076f, 0.419607878f };	// 深卡其色(深黄褐色)
	inline const Color DarkMagenta				{ 0.545098066f, 0.000000000f, 0.545098066f };	// 深洋红色
	inline const Color DarkOliverGreen			{ 0.333333343f, 0.419607878f, 0.184313729f };	// 深橄榄绿
	inline const Color DarkOrange				{ 1.000000000f, 0.549019635f, 0.000000000f };	// 深橘黄色
	inline const Color DarkOrchid				{ 0.600000024f, 0.196078449f, 0.800000072f };	// 深紫兰色
	inline const Color DarkRed					{ 0.545098066f, 0.000000000f, 0.000000000f };	// 深红色
	inline const Color DarkSalmon				{ 0.913725555f, 0.588235319f, 0.478431404f };	// 深橙红色
	inline const Color DarkSeaGreen				{ 0.560784340f, 0.737254918f, 0.545098066f };	// 深海绿色
	inline const Color DarkSlateBlue			{ 0.282352954f, 0.239215702f, 0.545098066f };	// 深板岩蓝
	inline const Color DarkSlateGray			{ 0.184313729f, 0.309803933f, 0.309803933f };	// 深板岩灰
	inline const Color DarkTurquoise			{ 0.000000000f, 0.807843208f, 0.819607913f };	// 深绿松石色
	inline const Color DarkViolet				{ 0.580392182f, 0.000000000f, 0.827451050f };	// 深紫色
	inline const Color DeepPink					{ 1.000000000f, 0.078431375f, 0.576470613f };	// 深粉红色
	inline const Color DeepSkyBlue				{ 0.000000000f, 0.749019623f, 1.000000000f };	// 深天蓝色
	inline const Color DimGray					{ 0.411764741f, 0.411764741f, 0.411764741f };	// 暗灰色
	inline const Color DodgerBlue				{ 0.117647067f, 0.564705908f, 1.000000000f };	// 道奇蓝(闪蓝色)
	inline const Color Firebrick				{ 0.698039234f, 0.133333340f, 0.133333340f };	// 砖色
	inline const Color FloralWhite				{ 1.000000000f, 0.980392218f, 0.941176534f };	// 花白色
	inline const Color ForestGreen				{ 0.133333340f, 0.545098066f, 0.133333340f };	// 森林绿
	inline const Color Fuchsia					{ 1.000000000f, 0.000000000f, 1.000000000f };	// 紫红色
	inline const Color Gainsboro				{ 0.862745166f, 0.862745166f, 0.862745166f };	// 淡灰色
	inline const Color GhostWhite				{ 0.972549081f, 0.972549081f, 1.000000000f };	// 苍白色
	inline const Color Gold						{ 1.000000000f, 0.843137324f, 0.000000000f };	// 金色
	inline const Color Goldenrod				{ 0.854902029f, 0.647058845f, 0.125490203f };	// 秋麒麟色
	inline const Color Gray						{ 0.501960814f, 0.501960814f, 0.501960814f };	// 灰色
	inline const Color Green					{ 0.000000000f, 0.501960814f, 0.000000000f };	// 绿色
	inline const Color GreenYellow				{ 0.678431392f, 1.000000000f, 0.184313729f };	// 黄绿色
	inline const Color Honeydew					{ 0.941176534f, 1.000000000f, 0.941176534f };	// 哈密瓜色
	inline const Color HotPink					{ 1.000000000f, 0.411764741f, 0.705882370f };	// 亮粉色
	inline const Color IndianRed				{ 0.803921640f, 0.360784322f, 0.360784322f };	// 印第安红
	inline const Color Indigo					{ 0.294117659f, 0.000000000f, 0.509803951f };	// 靛蓝色
	inline const Color Ivory					{ 1.000000000f, 1.000000000f, 0.941176534f };	// 象牙白
	inline const Color Khaki					{ 0.941176534f, 0.901960850f, 0.549019635f };	// 卡其色(黄褐色)
	inline const Color Lavender					{ 0.901960850f, 0.901960850f, 0.980392218f };	// 薰衣草色(淡紫色)
	inline const Color LavenderBlush			{ 1.000000000f, 0.941176534f, 0.960784376f };	// 淡紫红
	inline const Color LawnGreen				{ 0.486274540f, 0.988235354f, 0.000000000f };	// 草坪绿
	inline const Color LemonChiffon				{ 1.000000000f, 0.980392218f, 0.803921640f };	// 柠檬绸色
	inline const Color LightBlue				{ 0.678431392f, 0.847058892f, 0.901960850f };	// 浅蓝色
	inline const Color LightCoral				{ 0.941176534f, 0.501960814f, 0.501960814f };	// 浅珊瑚色
	inline const Color LightCyan				{ 0.878431439f, 1.000000000f, 1.000000000f };	// 浅青绿色
	inline const Color LightGoldenrodYellow		{ 0.980392218f, 0.980392218f, 0.823529482f };	// 浅秋麒麟色
	inline const Color LightGreen				{ 0.564705908f, 0.933333397f, 0.564705908f };	// 浅绿色
	inline const Color LightGray				{ 0.827451050f, 0.827451050f, 0.827451050f };	// 浅灰色
	inline const Color LightPink				{ 1.000000000f, 0.713725507f, 0.756862819f };	// 浅粉色
	inline const Color LightSalmon				{ 1.000000000f, 0.627451003f, 0.478431404f };	// 浅橙红色
	inline const Color LightSeaGreen			{ 0.125490203f, 0.698039234f, 0.666666687f };	// 浅海绿色
	inline const Color LightSkyBlue				{ 0.529411793f, 0.807843208f, 0.980392218f };	// 浅天空蓝
	inline const Color LightSlateGray			{ 0.466666698f, 0.533333361f, 0.600000024f };	// 浅板岩灰
	inline const Color LightSteelBlue			{ 0.690196097f, 0.768627524f, 0.870588303f };	// 浅钢铁蓝
	inline const Color LightYellow				{ 1.000000000f, 1.000000000f, 0.878431439f };	// 浅黄色
	inline const Color Lime						{ 0.000000000f, 1.000000000f, 0.000000000f };	// 青柠色
	inline const Color LimeGreen				{ 0.196078449f, 0.803921640f, 0.196078449f };	// 青柠绿
	inline const Color Linen					{ 0.980392218f, 0.941176534f, 0.901960850f };	// 亚麻色
	inline const Color Magenta					{ 1.000000000f, 0.000000000f, 1.000000000f };	// 洋红色
	inline const Color Maroon					{ 0.501960814f, 0.000000000f, 0.000000000f };	// 褐红色
	inline const Color MediumAquamarine			{ 0.400000036f, 0.803921640f, 0.666666687f };	// 间碧绿色
	inline const Color MediumBlue				{ 0.000000000f, 0.000000000f, 0.803921640f };	// 间蓝色
	inline const Color MediumOrchid				{ 0.729411781f, 0.333333343f, 0.827451050f };	// 间紫兰色
	inline const Color MediumPurple				{ 0.576470613f, 0.439215720f, 0.858823597f };	// 间紫红色
	inline const Color MediumSeaGreen			{ 0.235294133f, 0.701960802f, 0.443137288f };	// 间海绿色
	inline const Color MediumSlateBlue			{ 0.482352972f, 0.407843173f, 0.933333397f };	// 间板岩蓝
	inline const Color MediumSpringGreen		{ 0.000000000f, 0.980392218f, 0.603921592f };	// 间春绿色
	inline const Color MediumTurquoise			{ 0.282352954f, 0.819607913f, 0.800000072f };	// 间绿松石色
	inline const Color MediumVioletRed			{ 0.780392230f, 0.082352944f, 0.521568656f };	// 间紫罗兰红
	inline const Color MidnightBlue				{ 0.098039225f, 0.098039225f, 0.439215720f };	// 午夜蓝
	inline const Color MintCream				{ 0.960784376f, 1.000000000f, 0.980392218f };	// 薄荷奶油色
	inline const Color MistyRose				{ 1.000000000f, 0.894117713f, 0.882353008f };	// 粉玫瑰红
	inline const Color Moccasin					{ 1.000000000f, 0.894117713f, 0.709803939f };	// 鹿皮色
	inline const Color NavajoWhite				{ 1.000000000f, 0.870588303f, 0.678431392f };	// 纳瓦白
	inline const Color Navy						{ 0.000000000f, 0.000000000f, 0.501960814f };	// 藏青色(海军蓝)
	inline const Color OldLace					{ 0.992156923f, 0.960784376f, 0.901960850f };	// 旧布黄
	inline const Color Olive					{ 0.501960814f, 0.501960814f, 0.000000000f };	// 橄榄绿
	inline const Color OliveDrab				{ 0.419607878f, 0.556862772f, 0.137254909f };	// 深绿褐色
	inline const Color Orange					{ 1.000000000f, 0.647058845f, 0.000000000f };	// 橘黄色
	inline const Color OrangeRed				{ 1.000000000f, 0.270588249f, 0.000000000f };	// 橘红色
	inline const Color Orchid					{ 0.854902029f, 0.439215720f, 0.839215755f };	// 兰花紫
	inline const Color PaleGoldenrod			{ 0.933333397f, 0.909803987f, 0.666666687f };	// 苍菊黄色
	inline const Color PaleGreen				{ 0.596078455f, 0.984313786f, 0.596078455f };	// 苍绿色
	inline const Color PaleTurquoise			{ 0.686274529f, 0.933333397f, 0.933333397f };	// 苍宝石绿
	inline const Color PaleVioletRed			{ 0.858823597f, 0.439215720f, 0.576470613f };	// 苍紫红色
	inline const Color PapayaWhip				{ 1.000000000f, 0.937254965f, 0.835294187f };	// 番木瓜色
	inline const Color PeachPuff				{ 1.000000000f, 0.854902029f, 0.725490212f };	// 桃色
	inline const Color Peru						{ 0.803921640f, 0.521568656f, 0.247058839f };	// 秘鲁色
	inline const Color Pink						{ 1.000000000f, 0.752941251f, 0.796078503f };	// 粉色
	inline const Color Plum						{ 0.866666734f, 0.627451003f, 0.866666734f };	// 李子色(浅紫色)
	inline const Color PowderBlue				{ 0.690196097f, 0.878431439f, 0.901960850f };	// 粉末蓝
	inline const Color Purple					{ 0.501960814f, 0.000000000f, 0.501960814f };	// 紫红色
	inline const Color Red						{ 1.000000000f, 0.000000000f, 0.000000000f };	// 红色
	inline const Color RosyBrown				{ 0.737254918f, 0.560784340f, 0.560784340f };	// 玫瑰棕色
	inline const Color RoyalBlue				{ 0.254901975f, 0.411764741f, 0.882353008f };	// 皇家蓝
	inline const Color SaddleBrown				{ 0.545098066f, 0.270588249f, 0.074509807f };	// 马鞍棕
	inline const Color Salmon					{ 0.980392218f, 0.501960814f, 0.447058856f };	// 鲑鱼红
	inline const Color SandyBrown				{ 0.956862807f, 0.643137276f, 0.376470625f };	// 沙褐色
	inline const Color SeaGreen					{ 0.180392161f, 0.545098066f, 0.341176480f };	// 海绿色
	inline const Color SeaShell					{ 1.000000000f, 0.960784376f, 0.933333397f };	// 海贝色
	inline const Color Sienna					{ 0.627451003f, 0.321568638f, 0.176470593f };	// 赭色(褐色)
	inline const Color Silver					{ 0.752941251f, 0.752941251f, 0.752941251f };	// 银色
	inline const Color SkyBlue					{ 0.529411793f, 0.807843208f, 0.921568692f };	// 天蓝色
	inline const Color SlateBlue				{ 0.415686309f, 0.352941185f, 0.803921640f };	// 板岩蓝
	inline const Color SlateGray				{ 0.439215720f, 0.501960814f, 0.564705908f };	// 板岩灰
	inline const Color Snow						{ 1.000000000f, 0.980392218f, 0.980392218f };	// 雪白色
	inline const Color SpringGreen				{ 0.000000000f, 1.000000000f, 0.498039246f };	// 春绿色
	inline const Color SteelBlue				{ 0.274509817f, 0.509803951f, 0.705882370f };	// 钢铁蓝
	inline const Color Tan						{ 0.823529482f, 0.705882370f, 0.549019635f };	// 棕褐色
	inline const Color Teal						{ 0.000000000f, 0.501960814f, 0.501960814f };	// 蓝绿色
	inline const Color Thistle					{ 0.847058892f, 0.749019623f, 0.847058892f };	// 蓟色
	inline const Color Tomato					{ 1.000000000f, 0.388235331f, 0.278431386f };	// 番茄色
	inline const Color Turquoise				{ 0.250980407f, 0.878431439f, 0.815686345f };	// 绿松石色
	inline const Color Violet					{ 0.933333397f, 0.509803951f, 0.933333397f };	// 紫色
	inline const Color Wheat					{ 0.960784376f, 0.870588303f, 0.701960802f };	// 小麦色
	inline const Color White					{ 1.000000000f, 1.000000000f, 1.000000000f };	// 白色
	inline const Color WhiteSmoke				{ 0.960784376f, 0.960784376f, 0.960784376f };	// 白烟色
	inline const Color Yellow					{ 1.000000000f, 1.000000000f, 0.000000000f };	// 黄色
	inline const Color YellowGreen				{ 0.603921592f, 0.803921640f, 0.196078449f };	// 黄绿色
}
