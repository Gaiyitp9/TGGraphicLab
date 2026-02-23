/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Rendering/Shader.h"
#include <unordered_map>
#include <string>
#include <any>

namespace TG::Rendering
{
    class Material
    {
    public:
        std::unordered_map<std::string, std::any> parameters;
        Shader const* shader{ nullptr };
    };
}
