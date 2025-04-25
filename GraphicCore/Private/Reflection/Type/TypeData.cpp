/****************************************************************
* TianGong GraphicLab										    *
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Reflection/Type/TypeData.h"

namespace TG::Reflection
{
    TypeData* GetInvalidTypeData()
    {
        static TypeData instance;
        instance.rawTypeData = &instance;
        instance.wrappedType = &instance;
        instance.arrayRawType = &instance;
        return &instance;
    }
}
