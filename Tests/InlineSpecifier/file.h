#pragma once

// struct MyData
// {
//     int data[10000000];
// };
// static const MyData data;

inline const int data[10000000]{};
// static const int data[10000000]{};

// 查看file1.obj、main.obj和InlineSpecifier.exe的大小
// 1. inline全局变量只会保存一份副本，所以大小关系为file1.obj = main.obj，InlineSpecifier.exe > fil1.obj
//    因为InlineSpecifier.exe里还保存了其他的数据，所以是大于
// 2. static全局变量在每个.obj文件里都保存一份副本，所以大小关系为InlineSpecifier.exe > file1.obj + main.obj，大于的理由同1
