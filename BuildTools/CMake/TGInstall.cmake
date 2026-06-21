include_guard(GLOBAL)
include(CMakePackageConfigHelpers)

function(tg_install PACKAGE_NAME CURRENT_DIR BUILD_DIR INSTALL_INCLUDEDIR INSTALL_LIBDIR INSTALL_BINDIR)
    # package相关文件，包括Config.cmake, ConfigTargets.cmake和ConfigVersion.cmake
    set(PROJECT_CONFIG_IN "${CURRENT_DIR}/${PACKAGE_NAME}Config.cmake.in")
    set(PROJECT_CONFIG_OUT "${BUILD_DIR}/${PACKAGE_NAME}Config.cmake")
    set(CONFIG_TARGETS_FILE "${PACKAGE_NAME}ConfigTargets.cmake")
    set(VERSION_CONFIG_FILE "${BUILD_DIR}/${PACKAGE_NAME}ConfigVersion.cmake")
    # 安装目录
    set(EXPORT_DEST_DIR "${INSTALL_LIBDIR}/cmake/${PACKAGE_NAME}")
    # 在构建目录生成package相关文件
    export(TARGETS ${PACKAGE_NAME} NAMESPACE TG:: FILE ${BUILD_DIR}/${CONFIG_TARGETS_FILE})
    configure_package_config_file(${PROJECT_CONFIG_IN} ${PROJECT_CONFIG_OUT}
        INSTALL_DESTINATION ${EXPORT_DEST_DIR})
    write_basic_package_version_file(
        ${VERSION_CONFIG_FILE}
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY ExactVersion
    )
    # 安装头文件
    install(DIRECTORY ${CURRENT_DIR}/Public/ DESTINATION ${INSTALL_INCLUDEDIR}/${PACKAGE_NAME})
    # 安装库文件
    install(TARGETS ${PACKAGE_NAME}
        EXPORT ${PACKAGE_NAME}
        LIBRARY DESTINATION ${INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${INSTALL_LIBDIR}
        RUNTIME DESTINATION ${INSTALL_BINDIR}
    )
    # 安装package相关文件
    install(EXPORT ${PACKAGE_NAME} FILE ${CONFIG_TARGETS_FILE} NAMESPACE TG:: DESTINATION ${EXPORT_DEST_DIR})
    install(FILES ${PROJECT_CONFIG_OUT} ${VERSION_CONFIG_FILE} DESTINATION ${EXPORT_DEST_DIR})
endfunction()
