/****************************************************************
* TianGong GraphicLab											*
* Copyright (c) Gaiyitp9. All rights reserved.					*
* This code is licensed under the MIT License (MIT).			*
*****************************************************************/
#pragma once

#include <vector>

namespace TG::Algorithm
{
    /**
     * 快速排序
     * @tparam T 数组类型
     * @param array 待排序的数组
     * @param left 排序区间的左边界
     * @param right 排序区间的右边界
     */
    template<typename T>
    void QuickSort(T array[], int left, int right)
    {
        if (left >= right)
            return;

        int pivot = array[left];
        int i = left, j = right;
        while (i < j)
        {
            while (i < j)
            {
                if (array[j] >= pivot)
                    j--;
                else
                {
                    array[i++] = array[j];
                    break;
                }
            }
            while (i < j)
            {
                if (array[i] <= pivot)
                    i++;
                else
                {
                    array[j--] = array[i];
                    break;
                }
            }
        }
        int mid = i;
        array[mid] = pivot;
        QuickSort(array, left, mid - 1);
        QuickSort(array, mid + 1, right);
    }

    /**
     * 冒泡排序
     * @tparam T 数组类型
     * @param array 待排序的数组
     * @param size 数组大小
     */
    template<typename T>
    void BubbleSort(T array[], int size)
    {
        for (int i = 1; i <= size; ++i)
        {
            for (int j = 0; j < size - i; ++j)
            {
                if (array[j] > array[j + 1])
                {
                    int temp = array[j];
                    array[j] = array[j + 1];
                    array[j + 1] = temp;
                }
            }
        }
    }

    template<typename T>
    void Merge(T arr[], int left, int mid, int right)
    {
        std::vector<T> leftArr{arr + left, arr + mid + 1};
        std::vector<T> rightArr{arr + mid + 1, arr + right + 1};

        int i = 0;
        int j = 0;
        int k = left;
        while (i < leftArr.size() && j < rightArr.size())
        {
            if (leftArr[i] <= rightArr[j])
                arr[k++] = leftArr[i++];
            else
                arr[k++] = rightArr[j++];
        }

        while (i < leftArr.size())
            arr[k++] = leftArr[i++];
        while (j < rightArr.size())
            arr[k++] = rightArr[j++];
    }
    /**
     * 归并排序
     * @tparam T 数组类型
     * @param array 待排序的数组
     * @param left 排序区间的左边界
     * @param right 排序区间的右边界
     */
    template<typename T>
    void MergeSort(T array[], int left, int right)
    {
        if (left >= right)
            return;

        int mid = (left + right) / 2;
        MergeSort(array, left, mid);
        MergeSort(array, mid + 1, right);
        Merge(array, left, mid, right);
    }
}
