/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Reflection
{
    // 属性列表
    template<typename... Attrs>
    struct AttributeList : ElementList<Attrs...>
    {
        constexpr explicit AttributeList(Attrs... attrs) : ElementList<Attrs...>{attrs...} {}
    };
}