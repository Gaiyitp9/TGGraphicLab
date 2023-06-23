# 设置测试目录名称
set(TestsFolderName Tests)
# 添加所有测试项目
list(APPEND AllTests "Reflection")
foreach(test ${AllTests})
    add_subdirectory(${test})
endforeach ()