/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Declarations.hpp"
#include <array>

namespace TG::Math
{
    // 合并两个已排序的子数组，并计算逆序数
    template<std::size_t N, std::size_t Left, std::size_t Mid, std::size_t Right>
    std::size_t MergeAndCount(std::array<std::size_t, N>& permutation)
    {
        std::size_t inversionNumber = 0;

        constexpr std::size_t leftCount = Mid - Left + 1;
        constexpr std::size_t rightCount = Right - Mid;
        std::array<std::size_t, leftCount> leftPermutation;
        std::array<std::size_t, rightCount> rightPermutation;
        for (std::size_t i = 0; i < leftCount; ++i)
            leftPermutation[i] = permutation[Left + i];
        for (std::size_t i = 0; i < rightCount; ++i)
            rightPermutation[i] = permutation[Mid + 1 + i];

        std::size_t i = 0, j = 0, k = Left;
        while (i < leftCount && j < rightCount)
        {
            if (leftPermutation[i] < rightPermutation[j])
                permutation[k++] = leftPermutation[i++];
            else
            {
                permutation[k++] = rightPermutation[j++];
                inversionNumber += leftCount - i;
            }
        }

        while (i < leftCount)
            permutation[k++] = leftPermutation[i++];
        while (j < rightCount)
            permutation[k++] = rightPermutation[j++];

        return inversionNumber;
    }
    // 使用归并排序计算逆序数，复杂度O(NlogN)
    template<std::size_t N, std::size_t Left, std::size_t Right>
    std::size_t MergeSortAndCount(std::array<std::size_t, N>& permutation)
    {
        std::size_t inversionNumber = 0;
        if constexpr (Left < Right)
        {
            constexpr std::size_t mid = (Left + Right) / 2;
            inversionNumber += MergeSortAndCount<N, Left, mid>(permutation);
            inversionNumber += MergeSortAndCount<N, mid + 1, Right>(permutation);
            inversionNumber += MergeAndCount<N, Left, mid, Right>(permutation);
        }
        return inversionNumber;
    }
    // 排列的逆序数
    template<std::size_t N>
    std::size_t InversionNumber(std::array<std::size_t, N> permutation)
    {
        return MergeSortAndCount<N, 0, permutation.size() - 1>(permutation);
    }

    template<typename Derived, std::size_t Order>
    class DeterminantImpl
    {
        using Scalar = Traits<Derived>::Scalar;

    public:
        Scalar operator()(const Derived& mat) requires (Order == 1)
        {
            return mat(0, 0);
        }

        Scalar operator()(const Derived& mat) requires (Order == 2)
        {
            return mat(0, 0) * mat(1, 1) - mat(1, 0) * mat(0, 1);
        }

        Scalar operator()(const Derived& mat) requires (Order == 3)
        {
            // ‌代数余子式法
            return mat(0, 0) * (mat(1, 1) * mat(2, 2) - mat(1, 2) * mat(2, 1))
                - mat(0, 1) * (mat(1, 0) * mat(2, 2) - mat(1, 2) * mat(2, 0))
                + mat(0, 2) * (mat(1, 0) * mat(2, 1) - mat(1, 1) * mat(2, 0));
        }

        Scalar operator()(const Derived& mat) requires (Order == 4)
        {
            // ‌代数余子式法
            Scalar d2_01 = Det2(mat, 0, 1);
            Scalar d2_02 = Det2(mat, 0, 2);
            Scalar d2_03 = Det2(mat, 0, 3);
            Scalar d2_12 = Det2(mat, 1, 2);
            Scalar d2_13 = Det2(mat, 1, 3);
            Scalar d2_23 = Det2(mat, 2, 3);
            Scalar d3_0 = Det3(mat, 1, d2_23, 2, d2_13, 3, d2_12);
            Scalar d3_1 = Det3(mat, 0, d2_23, 2, d2_03, 3, d2_02);
            Scalar d3_2 = Det3(mat, 0, d2_13, 1, d2_03, 3, d2_01);
            Scalar d3_3 = Det3(mat, 0, d2_12, 1, d2_02, 2, d2_01);
            return -mat(0, 3) * d3_0 + mat(1, 3) * d3_1 - mat(2, 3) * d3_2 + mat(3, 3) * d3_3;
        }

        Scalar operator()(const Derived& mat)
        {
            return {};
        }

    private:
        Scalar Det2(const Derived& mat, std::size_t i0, std::size_t i1)
        {
            return mat(i0, 0) * mat(i1, 1) - mat(i1, 0) * mat(i0, 1);
        }

        Scalar Det3(const Derived& mat, std::size_t i0, Scalar d0, std::size_t i1, Scalar d1,
                                       std::size_t i2, Scalar d2)
        {
            return mat(i0, 2) * d0 - mat(i1, 2) * d1 + mat(i2, 2) * d2;
        }
    };
}
