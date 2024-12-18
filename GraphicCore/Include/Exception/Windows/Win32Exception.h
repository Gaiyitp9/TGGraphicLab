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
		Win32Exception(HRESULT hr, std::string_view description);
		~Win32Exception() override = default;

		[[nodiscard]] char const* what() const override;
	};

    inline void CheckHResult(HRESULT hr, std::string_view description = "")
    {
        if (hr < 0)
            throw Win32Exception(hr, description);
    }

    inline void CheckLastError(std::string_view description = "")
    {
        auto hr = static_cast<HRESULT>(GetLastError());
        throw Win32Exception(hr, description);
    }
}
