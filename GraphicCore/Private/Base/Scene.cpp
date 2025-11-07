/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/

#include "Base/Scene.h"

namespace TG
{
    Scene::Scene() : m_root(std::make_shared<Object>())
    {

    }

    void Scene::AddObject(std::shared_ptr<Object> obj)
    {
        m_root.CreateChild(obj);
    }

    void Scene::RemoveObject(std::shared_ptr<Object> obj)
    {
        m_root.RemoveChild(obj);
    }
}
