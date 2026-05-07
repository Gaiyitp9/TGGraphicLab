/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Rendering/IContext.h"
#include <memory>

namespace TG::Rendering
{
    class TG_API OpenGLContext : public IContext
    {
        using PlatformTraitsT = PlatformTraits<g_platform>;

    public:
        explicit OpenGLContext(const IVideoPort& videoPort);
        ~OpenGLContext() override;

		[[nodiscard]] const IVideoPort& VideoPort() const override;

        void MakeCurrent() const;
        void SetVSync(bool enable) const;
        void Present() const;

    private:
        const IVideoPort& m_videoPort;
        std::unique_ptr<PlatformTraitsT::OpenGLContext> m_context;
    };
}
