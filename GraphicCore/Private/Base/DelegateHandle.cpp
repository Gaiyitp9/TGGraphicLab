/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#include "Base/Delegate.hpp"

namespace TG
{
    uint64_t g_nextId = 1;

    uint64_t DelegateHandle::GenerateNewID()
    {
        return g_nextId++;
    }
}
