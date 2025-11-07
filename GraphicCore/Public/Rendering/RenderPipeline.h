/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <vector>
#include <memory>

namespace TG
{
    class RenderPass;

    class RenderPipeline
    {
    public:
        void Render();

    private:
        std::vector<std::shared_ptr<RenderPass>> m_renderPasses;
    };
}
