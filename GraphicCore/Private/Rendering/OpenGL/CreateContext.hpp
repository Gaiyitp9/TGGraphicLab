/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Base/Interfaces.h"
#include <memory>

namespace TG::Rendering
{
    template<Platform P>
    std::unique_ptr<typename PlatformTraits<P>::OpenGLContext> CreateContext(const IVideoPort& videoPort)
    {
        if constexpr (P == Platform::Windows)
        {
            return std::make_unique<typename PlatformTraits<P>::OpenGLContext>(videoPort.Surface());
        }
        else
        {
            return std::make_unique<typename PlatformTraits<P>::OpenGLContext>(
                videoPort.Handle(),
                videoPort.Surface(),
                videoPort.Width(),
                videoPort.Height()
            );
        }
    }
}
