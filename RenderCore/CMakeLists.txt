list(APPEND INCLUDES
        Include/Chronometer.h
        Include/Singleton.hpp

        Include/Diagnostics/BaseException.h
        Include/Diagnostics/Log.hpp

        Include/Editor/Window.h
        Include/Editor/MainWindow.h

        Include/Input/Event.h
        Include/Input/EventData.h
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

        Include/PAL/NativeWindow.h
        Include/PAL/PlatformSpecific.h
)

list(APPEND SRCS
        Source/Chronometer.cpp

        Source/Diagnostics/BaseException.cpp

        Source/Editor/MainWindow.cpp

        Source/Input/Event.cpp
        Source/Input/Keyboard.cpp
        Source/Input/Mouse.cpp
)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    list(APPEND INCLUDES
            Include/PAL/Windows/MemoryLeakDetection.h
            Include/PAL/Windows/Utility.h
            Include/PAL/Windows/Win32API.h
            Include/PAL/Windows/Win32Exception.h
            Include/PAL/Windows/WindowCore.h

            Include/PAL/Windows/Editor/MainWindow.h
            Include/PAL/Windows/Editor/PopupWindow.h
            Include/PAL/Windows/Editor/Window.h
    )

    list(APPEND SRCS
            Source/PAL/Windows/Utility.cpp
            Source/PAL/Windows/Win32Exception.cpp
            Source/PAL/Windows/WindowCore.cpp

            Source/PAL/Windows/Editor/MainWindow.cpp
            Source/PAL/Windows/Editor/PopupWindow.cpp
            Source/PAL/Windows/Editor/Window.cpp
    )
endif()
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRCS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${INCLUDES})

set(TG_MATRIX_STORAGE_ORDER "RowMajor" CACHE STRING "Default matrix storage order" FORCE)
set_property(CACHE TG_MATRIX_STORAGE_ORDER PROPERTY STRINGS "RowMajor" "ColumnMajor")

add_library(RenderCore STATIC ${SRCS} ${INCLUDES})
target_include_directories(RenderCore PUBLIC Include ThirdParty)
target_link_libraries(RenderCore PRIVATE dxgi d3d11 dxguid dbghelp opengl32)

if (TG_MATRIX_STORAGE_ORDER MATCHES "RowMajor")
    target_compile_definitions(RenderCore PRIVATE -DTG_ROW_MAJOR_MATRIX)
endif()
