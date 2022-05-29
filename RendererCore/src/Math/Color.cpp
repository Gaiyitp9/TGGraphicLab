/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Math/Color.hpp"

namespace LCH::Math
{
	//-----------------------------------------------------------------
	//	颜色取自	DirectXColors.h -- C++ Color Math library
	//	Copyright (c) Microsoft Corporation. All rights reserved.
	//	Licensed under the MIT License.
	//	http://go.microsoft.com/fwlink/?LinkID=615560
	//-----------------------------------------------------------------
	Color Color::AliceBlue		{ 0.941176534f, 0.972549081f, 1.000000000f, 1.000000000f };
	Color Color::AntiqueWhite	{ 0.980392218f, 0.921568692f, 0.843137324f, 1.000000000f };
	Color Color::Aqua			{ 0.000000000f, 1.000000000f, 1.000000000f, 1.000000000f };
	Color Color::Aquamarine		{ 0.498039246f, 1.000000000f, 0.831372619f, 1.000000000f };
	Color Color::Azure			{ 0.941176534f, 1.000000000f, 1.000000000f, 1.000000000f };
	Color Color::Beige			{ 0.960784376f, 0.960784376f, 0.862745166f, 1.000000000f };
	Color Color::Bisque			{ 1.000000000f, 0.894117713f, 0.768627524f, 1.000000000f };
	Color Color::Black			{ 0.000000000f, 0.000000000f, 0.000000000f, 1.000000000f };
	Color Color::Blanchedalmond	{ 1.000000000f, 0.921568692f, 0.803921640f, 1.000000000f };
	Color Color::Blue			{ 0.000000000f, 0.000000000f, 1.000000000f, 1.000000000f };
	Color Color::BlueViolet		{ 0.541176498f, 0.168627456f, 0.886274576f, 1.000000000f };
	Color Color::Brown			{ 0.647058845f, 0.164705887f, 0.164705887f, 1.000000000f };
	Color Color::BurlyWood		{ 0.870588303f, 0.721568644f, 0.529411793f, 1.000000000f };
	Color Color::Chartreuse		{ 0.498039246f, 1.000000000f, 0.000000000f, 1.000000000f };
	Color Color::Chocolate		{ 0.823529482f, 0.411764741f, 0.117647067f, 1.000000000f };
	Color Color::Coral			{ 1.000000000f, 0.498039246f, 0.313725501f, 1.000000000f };
	Color Color::CornflowerBlue	{ 0.392156899f, 0.584313750f, 0.929411829f, 1.000000000f };
	Color Color::Cornsilk		{ 1.000000000f, 0.972549081f, 0.862745166f, 1.000000000f };
	Color Color::Crimson		{ 0.862745166f, 0.078431375f, 0.235294133f, 1.000000000f };
	Color Color::Cyan			{ 0.000000000f, 1.000000000f, 1.000000000f, 1.000000000f };
	Color Color::DarkBlue		{ 0.000000000f, 0.000000000f, 0.545098066f, 1.000000000f };
	Color Color::DarkCyan		{ 0.000000000f, 0.545098066f, 0.545098066f, 1.000000000f };
	Color Color::DarkGoldenrod	{ 0.721568644f, 0.525490224f, 0.043137256f, 1.000000000f };
	Color Color::DarkGrey		{ 0.662745118f, 0.662745118f, 0.662745118f, 1.000000000f };
	Color Color::DarkGreen		{ 0.000000000f, 0.392156899f, 0.000000000f, 1.000000000f };
	Color Color::DarkKhaki		{ 0.741176486f, 0.717647076f, 0.419607878f, 1.000000000f };
	Color Color::DarkMagenta	{ 0.545098066f, 0.000000000f, 0.545098066f, 1.000000000f };
	Color Color::DarkOliverGreen{ 0.333333343f, 0.419607878f, 0.184313729f, 1.000000000f };
	Color Color::DarkOrange		{ 1.000000000f, 0.549019635f, 0.000000000f, 1.000000000f };
	Color Color::DarkOrchid		{ 0.600000024f, 0.196078449f, 0.800000072f, 1.000000000f };
	Color Color::DarkRed		{ 0.545098066f, 0.000000000f, 0.000000000f, 1.000000000f };
	Color Color::DarkSalmon		{ 0.913725555f, 0.588235319f, 0.478431404f, 1.000000000f };
	Color Color::DarkSeaGreen	{ 0.560784340f, 0.737254918f, 0.545098066f, 1.000000000f };
	Color Color::DarkSlateBlue	{ 0.282352954f, 0.239215702f, 0.545098066f, 1.000000000f };
	Color Color::DarkSlateGray	{ 0.184313729f, 0.309803933f, 0.309803933f, 1.000000000f };
	Color Color::DarkTurquoise	{ 0.000000000f, 0.807843208f, 0.819607913f, 1.000000000f };
	Color Color::DarkViolet		{ 0.580392182f, 0.000000000f, 0.827451050f, 1.000000000f };
	Color Color::DeepPink		{ 1.000000000f, 0.078431375f, 0.576470613f, 1.000000000f };
	Color Color::DeepSkyBlue	{ 0.000000000f, 0.749019623f, 1.000000000f, 1.000000000f };
	Color Color::DimGray		{ 0.411764741f, 0.411764741f, 0.411764741f, 1.000000000f };
	Color Color::DodgerBlue		{ 0.117647067f, 0.564705908f, 1.000000000f, 1.000000000f };
	Color Color::Firebrick		{ 0.698039234f, 0.133333340f, 0.133333340f, 1.000000000f };
	Color Color::FloralWhite	{ 1.000000000f, 0.980392218f, 0.941176534f, 1.000000000f };
	Color Color::ForestGreen	{ 0.133333340f, 0.545098066f, 0.133333340f, 1.000000000f };
	Color Color::Fuchsia		{ 1.000000000f, 0.000000000f, 1.000000000f, 1.000000000f };
	Color Color::Gainsboro		{ 0.862745166f, 0.862745166f, 0.862745166f, 1.000000000f };
	Color Color::GhostWhite		{ 0.972549081f, 0.972549081f, 1.000000000f, 1.000000000f };
	Color Color::Gold			{ 1.000000000f, 0.843137324f, 0.000000000f, 1.000000000f };
}