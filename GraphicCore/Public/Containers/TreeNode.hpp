/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <memory>

namespace TG
{
    template<typename T>
    class TreeNode
    {
    public:
        explicit TreeNode(std::shared_ptr<T> obj) : m_data(obj) {}

        std::shared_ptr<TreeNode> CreateChild(std::shared_ptr<T> obj)
        {
            auto child = std::make_shared<TreeNode>(obj);
            child->m_parent = this;
            m_children.push_back(child);
            return child;
        }

        void AddChild(std::shared_ptr<TreeNode> node)
        {
            node->m_parent = this;
            m_children.push_back(node);
        }

        bool RemoveChild(std::shared_ptr<T> child)
        {
            auto it = std::ranges::find_if(
                m_children,
                [child](std::shared_ptr<TreeNode> node) { return node->m_data == child; }
            );
            if (it != m_children.end())
            {
                (*it)->m_parent = nullptr;
                m_children.erase(it);
                return true;
            }
            return false;
        }

        std::shared_ptr<TreeNode> RemoveChildAt(int index)
        {
            auto it = m_children.erase(m_children.begin() + index);
            if (it != m_children.end())
                return *it;
            return nullptr;
        }

        TreeNode* GetParent() { return m_parent; }

    private:
        std::shared_ptr<T> m_data;
        TreeNode* m_parent{ nullptr };
        std::vector<std::shared_ptr<TreeNode>> m_children;
    };
}
