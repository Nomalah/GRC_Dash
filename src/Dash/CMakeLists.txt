set(CODE_FILES
    main.cpp
)

set(HEADER_FILES
)

set(RESOURCE_FILES
    qml.qrc
)

add_executable(GryphonDash ${CODE_FILES} ${HEADER_FILES} ${RESOURCE_FILES})
set_target_properties(GryphonDash PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<0:>)
target_include_directories(GryphonDash PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(GryphonDash PRIVATE fmt::fmt)

# TODO: Add the deployment for linux
if(WIN32)
    set(windeployqt "${QT_BIN_PATH}/windeployqt.exe")
    message(STATUS ${windeployqt})

    add_custom_command(
        TARGET GryphonDash POST_BUILD
        COMMAND "${windeployqt};--qmldir;${CMAKE_SOURCE_DIR}/src;$<TARGET_FILE_DIR:GryphonDash>"
        COMMAND_EXPAND_LISTS)
else()
    # message(FATAL_ERROR "No deployment for unix, implement.")
endif()