/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Base/Core.h"
#include "Exception/BaseException.h"

namespace TG
{
	class Win32Exception final : public BaseException
	{
	public:
		~Win32Exception() override = default;

		static Win32Exception Create(HRESULT hr, std::string_view message = "");
		static Win32Exception Create(std::string_view message = "");

	private:
		explicit Win32Exception(std::string_view description);
	};
}
