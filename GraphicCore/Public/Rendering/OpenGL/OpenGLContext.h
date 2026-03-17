/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Rendering/Context.h"

namespace TG::Rendering
{
    class OpenGLContext : public Context
    {
    public:
        explicit OpenGLContext(const IDefaultVideoPort& videoPort);
        ~OpenGLContext() override;

		[[nodiscard]] const IDefaultVideoPort& VideoPort() const override;

        void MakeCurrent() const;
        void SetVSync(bool enable) const;
        void Present() const;

    private:
    	void LoadWGLExtension() const;

    	const IDefaultVideoPort& m_videoPort;
        HDC m_hdc{};
        HGLRC m_wglContext{};
    };
}
