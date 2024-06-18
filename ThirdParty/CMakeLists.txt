# 设置三方库目录名称
set(ThirdPartyFolderName ThirdParty)
# 添加所有测试项目
list(APPEND ThirdParties glad)
list(APPEND ThirdParties spdlog)
foreach(thirdParty ${ThirdParties})
    add_subdirectory(${thirdParty})
endforeach ()
