# 设置测试目录名称
set(TestsFolderName Tests)
# 添加所有测试项目
#list(APPEND AllTests Reflection)
list(APPEND AllTests Template)
list(APPEND AllTests Math)
list(APPEND AllTests WinSocketDemo)
list(APPEND AllTests InputOutput)
foreach(test ${AllTests})
    add_subdirectory(${test})
endforeach ()