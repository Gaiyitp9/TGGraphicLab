add_library(PowerVR INTERFACE)

target_include_directories(PowerVR SYSTEM INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_compile_definitions(PowerVR INTERFACE DYNAMICGLES_NO_NAMESPACE INTERFACE DYNAMICEGL_NO_NAMESPACE)

set_target_properties(PowerVR PROPERTIES FOLDER ${ThirdPartyFolderName})
