/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <memory>

namespace TG
{
    void RunPathTracer(std::atomic<float>& process, std::atomic<bool>& renderDone);
}
