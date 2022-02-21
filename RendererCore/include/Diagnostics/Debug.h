/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "WinAPIException.h"

#ifdef ThrowIfFailed
#undef ThrowIfFailed
#endif

#ifdef ThrowIfFailedWithDesc
#undef ThrowIfFailedWithDesc
#endif

#ifdef ThrowLastError
#undef ThrowLastError
#endif

#ifdef ThrowLastErrorWithDesc
#undef ThrowLastErrorWithDesc
#endif

#define ThrowIfFailed(hr)\
		{\
			if (FAILED(hr))\
			{\
				throw LCH::WinAPIException(hr);\
			}\
		}

#define ThrowIfFailedWithDesc(hr, description)\
		{\
			if (FAILED(hr))\
			{\
				throw LCH::WinAPIException(hr, description);\
			}\
		}

#define ThrowLastError()\
		{\
			HRESULT hr = GetLastError();\
			if (FAILED(hr))\
			{\
				throw LCH::WinAPIException(hr);\
			}\
		}

#define ThrowLastErrorWithDesc(description)\
		{\
			HRESULT hr = GetLastError();\
			if (FAILED(hr))\
			{\
				throw LCH::WinAPIException(hr, description);\
			}\
		}