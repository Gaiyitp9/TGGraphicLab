list(APPEND INCLUDES
        Include/Chronometer.h
        Include/Log.h
        Include/Singleton.hpp

        Include/Exception/BaseException.h

        Include/Editor/Window.h
        Include/Editor/MainWindow.h

        Include/Input/Event.h
        Include/Input/EventDispatcher.h
        Include/Input/EventHandler.h
        Include/Input/KeyCode.h
        Include/Input/Keyboard.h
        Include/Input/Mouse.h
        Include/Input/Manager.hpp

        Include/Math/Assignment.hpp
        Include/Math/Block.hpp
        Include/Math/Core.hpp
        Include/Math/CWiseBinaryOp.hpp
        Include/Math/Dot.hpp
        Include/Math/ForwardDeclarations.hpp
        Include/Math/Matrix.hpp
        Include/Math/MatrixBase.hpp
        Include/Math/Product.hpp
        Include/Math/Quaternion.hpp
        Include/Math/Redux.hpp
        Include/Math/Transpose.hpp

        Include/PAL/Input.h
        Include/PAL/Window.h
        Include/PAL/DebugNew.h
)

list(APPEND SRCS
        Source/Chronometer.cpp

        Source/Exception/BaseException.cpp

        Source/Editor/MainWindow.cpp
        Source/Editor/Window.cpp

        Source/Input/EventDispatcher.cpp
        Source/Input/Keyboard.cpp
        Source/Input/Mouse.cpp
)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    list(APPEND INCLUDES
            Include/PAL/Windows/Auxiliary.h
            Include/PAL/Windows/Win32API.h
            Include/PAL/Windows/Win32Exception.h
            Include/PAL/Windows/NativeWindow.h
    )

    list(APPEND SRCS
            Source/PAL/Windows/Auxiliary.cpp
            Source/PAL/Windows/Win32Exception.cpp
            Source/PAL/Windows/Window.cpp
    )
endif()
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRCS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${INCLUDES})

add_library(RenderCore STATIC ${SRCS} ${INCLUDES})
target_include_directories(RenderCore PUBLIC Include ThirdParty)
target_link_libraries(RenderCore PRIVATE dxgi d3d11 dxguid dbghelp opengl32)

set(TG_MATRIX_STORAGE_ORDER "RowMajor" CACHE STRING "Default matrix storage order" FORCE)
set_property(CACHE TG_MATRIX_STORAGE_ORDER PROPERTY STRINGS "RowMajor" "ColumnMajor")
if (TG_MATRIX_STORAGE_ORDER MATCHES "RowMajor")
    target_compile_definitions(RenderCore PRIVATE TG_ROW_MAJOR_MATRIX)
endif()

string(TOLOWER ${CMAKE_BUILD_TYPE} build_type)
if (CMAKE_SYSTEM_NAME STREQUAL "Windows" AND build_type STREQUAL debug)
    #CRT库查找内存泄漏所需定义的宏
    target_compile_definitions(RenderCore PUBLIC _CRTDBG_MAP_ALLOC)
endif()
