/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#pragma once

#include <winsdkver.h>
#define _WIN32_WINNT 0x0A00
#include <sdkddkver.h>

#define WIN32_LEAN_AND_MEAN

#define NOMINMAX
#define NODRAWTEXT
#define NOGDI
#define NOBITMAP
#define NOMCX
#define NOHELP
#define NOSERVICE

#include <Windows.h>
#include <wrl.h>
#include <tchar.h>

#include <cstdint>
#include <cstdio>
#include <string>
#include <unordered_map>
#include <queue>
#include <mutex>
#include <exception>
#include <optional>

#include "Utility.h"
#include "Debug.h"