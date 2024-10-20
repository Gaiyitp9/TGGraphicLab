/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Base/MainWindow.h"
#include "Input/Event.h"

namespace TG
{
    MainWindow::MainWindow(int x, int y, int width, int height, std::string_view name)
        : Window(x, y, width, height, name, WindowType::Default)
    {}
}
