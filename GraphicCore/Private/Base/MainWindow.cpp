/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Base/MainWindow.h"

namespace TG
{
    MainWindow::MainWindow(int x, int y, unsigned int width, unsigned int height, std::string_view name)
        : WindowBase(x, y, width, height, name, WindowType::Main)
    {}
}
