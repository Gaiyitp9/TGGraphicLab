/****************************************************************
* TianGong RenderLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Reflection
{
    template<typename... Attrs>
    struct AttrList : ElemList<Attrs...>
    {
        constexpr explicit AttrList(Attrs... attrs) : ElemList<Attrs...>(attrs...) {}
    };
}