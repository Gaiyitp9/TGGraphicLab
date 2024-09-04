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
    // 使用归并排序计算逆序数
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

    template<std::size_t N>
    std::size_t InversionNumber(std::array<std::size_t, N> permutation)
    {
        return MergeSortAndCount<N, 0, permutation.size() - 1>(permutation);
    }
}
