/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Base/CommonInterfaces.h"
#include "Input/Mouse.h"
#include "Input/Keyboard.h"
#include "Modules/InputModule.h"

namespace TG::Input
{
    class Devices final : public IInput
    {
    public:
        Devices(const Devices&) = delete;
        Devices(Devices&&) = delete;
        Devices &operator=(const Devices&) = delete;
        Devices &operator=(Devices&&) = delete;

        static IInput& Instance()
        {
            static Devices instance;
            return instance;
        }

        [[nodiscard]] bool GetKey(KeyCode key) const override;
        [[nodiscard]] bool GetKeyDown(KeyCode key) const override;
        [[nodiscard]] bool GetKeyUp(KeyCode key) const override;
        [[nodiscard]] short MousePositionX() const override;
        [[nodiscard]] short MousePositionY() const override;
        [[nodiscard]] short MouseWheelDelta() const override;

    private:
        Devices() = default;
        ~Devices() override = default;

        static Devices& Self()
        {
            return static_cast<Devices&>(Instance());
        }

        std::weak_ptr<Mouse> m_mouse;
        std::weak_ptr<Keyboard> m_keyboard;

        friend InputModule::InputModule();
    };

    inline bool GetKey(KeyCode key)
    {
        return Devices::Instance().GetKey(key);
    }

    inline bool GetKeyDown(KeyCode key)
    {
        return Devices::Instance().GetKeyDown(key);
    }

    inline bool GetKeyUp(KeyCode key)
    {
        return Devices::Instance().GetKeyUp(key);
    }

    inline short MousePositionX()
    {
        return Devices::Instance().MousePositionX();
    }

    inline short MousePositionY()
    {
        return Devices::Instance().MousePositionY();
    }

    inline short MouseWheelDelta()
    {
        return Devices::Instance().MouseWheelDelta();
    }
}
