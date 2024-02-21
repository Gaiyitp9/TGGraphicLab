/****************************************************************
* TianGong Graphics Tool										*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Reflection
{
    template<typename Name, typename T>
    struct Attribute : NameValuePair<Name, T>
    {
        constexpr Attribute(Name name, T value) : NameValuePair<Name, T>(name, value) {}
    };

    // void不可以作为类型，Attribute(Name, void value)是非法的，所以当T是void时需要实现一个偏特化类
    template<typename Name>
    struct Attribute<Name, void>: NameValuePair<Name, void>
    {
        constexpr explicit Attribute(Name name) : NameValuePair<Name, void>(name) {}
    };
}