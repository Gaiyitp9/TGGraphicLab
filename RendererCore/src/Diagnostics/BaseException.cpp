/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Diagnostics/BaseException.h"

namespace LCH
{
	BaseException::BaseException(int lineNum, std::wstring filePath)
		: lineNum(lineNum), filePath(filePath)
	{

	}

	const char* BaseException::what() const
	{
		return nullptr;
	}


}