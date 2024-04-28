/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

namespace TG::Input
{
    // 鼠标数据
    union MouseData
    {
        struct
        {
            int x;
            int y;
        };
        short delta;
    };

    // 键盘数据
    struct KeyboardData
    {
        char c;
    };
}
