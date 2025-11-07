/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Object.h"
#include "Containers/TreeNode.hpp"

namespace TG
{
    class Scene
    {
    public:
        Scene();

        void AddObject(std::shared_ptr<Object> obj);
        void RemoveObject(std::shared_ptr<Object> obj);

    private:
        TreeNode<Object> m_root;
    };
}
