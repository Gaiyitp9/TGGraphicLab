/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Reflection
{
    // 域列表
    template<typename... Fields>
    struct FieldList : ElementList<Fields...>
    {
        constexpr explicit FieldList(Fields... fields) : ElementList<Fields...>{fields...} {}
    };
}
