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

        [[nodiscard]] std::size_t GetSizeof() const noexcept;

        [[nodiscard]] bool IsClass() const;
        [[nodiscard]] bool IsTemplateInstantiation() const;
        [[nodiscard]] std::ranges::subrange<std::vector<Type>::iterator> GetTemplateArguments() const;
        [[nodiscard]] bool IsEnumeration() const;
        [[nodiscard]] Enumeration GetEnumeration() const;
        [[nodiscard]] bool IsWrapper() const;
        [[nodiscard]] bool IsArray() const;
        [[nodiscard]] bool IsPointer() const;
        [[nodiscard]] bool IsArithmetic() const;
        [[nodiscard]] bool IsFunctionPointer() const;
        [[nodiscard]] bool IsMemberObjectPointer() const;
        [[nodiscard]] bool IsMemberFunctionPointer() const;
        [[nodiscard]] bool IsDerivedFrom(const Type& other) const;
        template<typename T>
        [[nodiscard]] bool IsDerivedFrom() const;
        [[nodiscard]] bool IsBaseOf(const Type& other) const;
        template<typename T>
        [[nodiscard]] bool IsBaseOf() const;
        [[nodiscard]] std::ranges::subrange<std::vector<Type>::iterator> GetBaseClasses() const;
        [[nodiscard]] std::ranges::subrange<std::vector<Type>::iterator> GetDerivedClasses() const;


    private:
        Type();

        TypeData* m_typeData;
    };
}
