/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Editor/MainWindow.h"

namespace TG
{
    MainWindow::MainWindow(int x, int y, int width, int height, std::string_view name, Window const *parent)
        : Window(x, y, width, height, name, PAL::WindowType::Default, parent)
    {}

    MainWindow::~MainWindow() = default;
}
