/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Win32Lean.h"
#include "GLCreateInfo.hpp"
#include "DeviceContextGL.hpp"
#include "RenderDeviceGL.hpp"
#include "SwapChainGL.hpp"
#include "Diagnostics/BaseException.h"
#include "glad/wgl.h"

namespace TG::Graphics
{
    class FactoryGL
    {
    public:
        FactoryGL() = default;
        FactoryGL(const FactoryGL&) = delete;
        FactoryGL(FactoryGL&&) = delete;
        FactoryGL& operator=(const FactoryGL&) = delete;
        ~FactoryGL();

        // 创建图形设备和上下文
        void CreateDeviceAndContext(const GLCreateInfo& info, RenderDeviceGL** ppDevice, DeviceContextGL** ppContext);

        // 创建交换链
       /*void CreateSwapChain(RenderDeviceGL const* pDevice,
                                     HWND hwnd,
                                     const SwapChainDesc& desc,
                                     ISwapChain** ppSwapChain) const;*/

    private:
        bool m_gladInit = false;         // glad库是否初始化
    };
}