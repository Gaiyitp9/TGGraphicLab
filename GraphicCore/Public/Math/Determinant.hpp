/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include "Declarations.hpp"
#include <array>
#include <algorithm>

namespace TG::Math
{
    template<typename Derived, std::size_t Order> requires (Traits<Derived>::Rows == Traits<Derived>::Columns)
    class DeterminantImpl
    {
        using Scalar = Traits<Derived>::Scalar;
        using Matrix = PlainMatrix<Derived>;

    public:
        Scalar operator()(const Matrix& mat) const requires (Order == 1)
        {
            return mat(0, 0);
        }

        Scalar operator()(const Matrix& mat) const requires (Order == 2)
        {
            return mat(0, 0) * mat(1, 1) - mat(1, 0) * mat(0, 1);
        }

        Scalar operator()(const Matrix& mat) const requires (Order == 3)
        {
            // ‌代数余子式法
            return mat(0, 0) * (mat(1, 1) * mat(2, 2) - mat(1, 2) * mat(2, 1))
                - mat(0, 1) * (mat(1, 0) * mat(2, 2) - mat(1, 2) * mat(2, 0))
                + mat(0, 2) * (mat(1, 0) * mat(2, 1) - mat(1, 1) * mat(2, 0));
        }

        Scalar operator()(const Matrix& mat) const requires (Order == 4)
        {
            // ‌行列式等于它的任一行(列)的各元素与其对应的代数余子式的乘积之和，这里取第一行的元素
            Scalar minor00 = Minor4X4(mat, 0, 0);
            Scalar minor01 = Minor4X4(mat, 0, 1);
            Scalar minor02 = Minor4X4(mat, 0, 2);
            Scalar minor03 = Minor4X4(mat, 0, 3);
            return mat(0, 0) * minor00 - mat(0, 1) * minor01 + mat(0, 2) * minor02 - mat(0, 3) * minor03;
        }

        Scalar operator()(const Matrix& mat) const
        {
            // 根据行列式定义来计算行列式
            Scalar det = 0;

            std::array<std::size_t, Order> permutation;
            for (std::size_t i = 0; i < Order; ++i)
                permutation[i] = i;
            do
            {
                std::size_t inversionNumber = InversionNumber(permutation);
                Scalar result = 1;
                for (std::size_t i = 0; i < inversionNumber; ++i)
                    result *= -1;
                for (std::size_t i = 0; i < Order; ++i)
                    result *= mat(i, permutation[i]);
                det += result;
            } while(std::next_permutation(permutation.begin(), permutation.end()));

            return det;
        }

    private:
        static Scalar Det3Helper(const Matrix& mat, std::size_t row0, std::size_t row1, std::size_t row2,
            std::size_t column0, std::size_t Column1, std::size_t Column2)
        {
            // 代数余子式求行列式公式的一部分。计算过程中考虑了(-1)^(row0 + column0)，可参考Cofactor3X3的分析
            return mat(row0, column0) *
                (mat(row1, Column1) * mat(row2, Column2) - mat(row1, Column2) * mat(row2, Column1));
        }
        // 第row行第column列的余子式
        static Scalar Minor4X4(const Matrix& mat, std::size_t row, std::size_t column)
        {
            std::size_t row0 = (row + 1) % 4;
            std::size_t row1 = (row + 2) % 4;
            std::size_t row2 = (row + 3) % 4;
            std::size_t column0 = (column + 1) % 4;
            std::size_t column1 = (column + 2) % 4;
            std::size_t column2 = (column + 3) % 4;
            // 注意，这里的计算公式没有考虑了(-1)^(row + column)，因为下面的式子是求行列式，没有把系数包含进去
            return Det3Helper(mat, row0, row1, row2, column0, column1, column2) +
                Det3Helper(mat, row1, row2, row0, column0, column1, column2) +
                    Det3Helper(mat, row2, row0, row1, column0, column1, column2);
        }

        // 合并两个已排序的子数组，并计算逆序数
        template<std::size_t N, std::size_t Left, std::size_t Mid, std::size_t Right>
        static std::size_t MergeAndCount(std::array<std::size_t, N>& permutation)
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
        static std::size_t MergeSortAndCount(std::array<std::size_t, N>& permutation)
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
        static std::size_t InversionNumber(std::array<std::size_t, N> permutation)
        {
            return MergeSortAndCount<N, 0, permutation.size() - 1>(permutation);
        }
    };
}
