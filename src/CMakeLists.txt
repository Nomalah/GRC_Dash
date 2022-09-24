add_subdirectory(RuntimeQml)

set(CODE_FILES
    main.cpp
)

set(HEADER_FILES
)

set(RESOURCE_FILES
    qml.qrc
)

add_executable(${PROJECT_NAME} ${CODE_FILES} ${HEADER_FILES} ${RESOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<0:>)
target_compile_definitions(${PROJECT_NAME} PRIVATE ROOT_SOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(${PROJECT_NAME} PRIVATE qt::qt fmt::fmt RuntimeQml)

# TODO: Add the deployment for linux
if(WIN32)
    set(windeployqt "${QT_BIN_PATH}/windeployqt.exe")
    message(STATUS ${windeployqt})

    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${windeployqt};--qmldir;${CMAKE_SOURCE_DIR}/src;$<TARGET_FILE:${PROJECT_NAME}>"
        COMMAND_EXPAND_LISTS)
elseif(UNIX AND NOT APPLE)
    message(STATUS "No deployment for Linux, implement.")
elseif(APPLE)
    message(STATUS "No deployment for Apple, implement.")
else()
    message(STATUS "No deployment for unknown platform, implement.")
endif()