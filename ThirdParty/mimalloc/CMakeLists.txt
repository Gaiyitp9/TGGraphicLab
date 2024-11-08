add_library(mimalloc SHARED IMPORTED GLOBAL)

set_target_properties(mimalloc PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/lib/Windows/Debug/mimalloc-override.dll"
        IMPORTED_IMPLIB "${CMAKE_CURRENT_LIST_DIR}/lib/Windows/Debug/mimalloc-override.lib"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/include"
        FOLDER ${ThirdPartyFolderName}
)

# 文档 https://microsoft.github.io/mimalloc/
