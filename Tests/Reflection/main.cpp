/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "gtest/gtest.h"
#include "GlobalArgs.h"
#include "Base/Utility.h"

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);

    for (int i = 0; i < argc; ++i)
        gArgv.emplace_back(argv[i]);
    gArgv.emplace_back();

    return RUN_ALL_TESTS();
}
