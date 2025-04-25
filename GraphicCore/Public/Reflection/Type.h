/****************************************************************
* TianGong GraphicLab										    *
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Reflection/Type/TypeData.h"
#include "Reflection/Enumeration.h"
#include <vector>
#include <ranges>

namespace TG::Reflection
{
    class Type
    {
    public:
        using TypeId = std::uintptr_t;

        template<typename T>
        static Type Get();

        template<typename T>
        static Type Get(T&& object);

        static Type GetByName(std::string_view name);

        static std::ranges::subrange<std::vector<Type>::iterator> GetTypes();

        [[nodiscard]] TypeId GetId() const noexcept;
        [[nodiscard]] std::string_view GetName() const noexcept;
        [[nodiscard]] std::string_view GetFullName() const noexcept;
        [[nodiscard]] bool IsValid() const noexcept;
        explicit operator bool() const noexcept;

        [[nodiscard]] Type GetRawType() const noexcept;
        [[nodiscard]] Type GetWrappedType() const noexcept;

        std::size_t GetSizeof() const noexcept;

        bool IsClass() const;
        bool IsTemplateInstantiation() const;
        std::ranges::subrange<std::vector<Type>::iterator> GetTemplateArguments() const;
        bool IsEnumeration() const;
        Enumeration GetEnumeration() const;
        bool IsWrapper() const;
        bool IsArray() const;
        bool IsPointer() const;
        bool IsArithmetic() const;
        bool IsFunctionPointer() const;
        bool IsMemberObjectPointer() const;
        bool IsMemberFunctionPointer() const;
        bool IsDerivedFrom(const Type& other) const;
        template<typename T>
        bool IsDerivedFrom() const;
        bool IsBaseOf(const Type& other) const;
        template<typename T>
        bool IsBaseOf() const;
        std::ranges::subrange<std::vector<Type>::iterator> GetBaseClasses() const;
        std::ranges::subrange<std::vector<Type>::iterator> GetDerivedClasses() const;


    private:
        Type();

        TypeData* m_typeData;
    };
}
