/****************************************************************
* TianGong GraphicLab										    *
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <string>

namespace TG::Reflection
{
    struct TypeData
    {
        TypeData* rawTypeData{ nullptr };
        TypeData* wrappedType{ nullptr };
        TypeData* arrayRawType{ nullptr };

        std::string name;
        std::string_view typeName;

        std::size_t getSizeof{ 0 };
        std::size_t getPointerDimension{ 0 };

        bool isValid{ false };
    };

    TypeData* GetInvalidTypeData();
}
