/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Platform/Core.h"
#include "Exception/BaseException.h"

namespace TG
{
	class Win32Exception final : public BaseException
	{
	public:
		explicit Win32Exception(HRESULT hr, std::string_view description);
		~Win32Exception() override;

		[[nodiscard]] char const* what() const override;

	private:
		std::stacktrace m_stackTrace{std::stacktrace::current()};
		std::string m_whatBuffer;
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
