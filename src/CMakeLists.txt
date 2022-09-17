add_subdirectory(Receiver)

set(CODE_FILES
    main.cpp
    # GRC2019/grc2019.cpp
)

set(HEADER_FILES
    GRC2019/grc2019.hpp
)

set(RESOURCE_FILES
    qml.qrc
)

add_executable(DumpCanBus DumpCanBus.cpp)
set_target_properties(DumpCanBus PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<0:>)
target_link_libraries(DumpCanBus PRIVATE CanBusReceiver)


# add_executable(${PROJECT_NAME} ${CODE_FILES} ${HEADER_FILES} ${RESOURCE_FILES})
# set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<0:>)
# target_include_directories(${PROJECT_NAME} PRIVATE
#     ${CMAKE_CURRENT_SOURCE_DIR}
#     ${CMAKE_CURRENT_SOURCE_DIR}/GRC2019
# )
# target_link_libraries(${PROJECT_NAME} PRIVATE qt::qt CanBusReceiver)

# TODO: Add the deployment for linux
if(WIN32)
    set(windeployqt "${QT_BIN_PATH}/windeployqt.exe")
    message(STATUS ${windeployqt})

    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${windeployqt};--qmldir;${CMAKE_SOURCE_DIR};$<TARGET_FILE_DIR:${PROJECT_NAME}>"
        COMMAND_EXPAND_LISTS)
else()
    message(FATAL_ERROR "No deployment for unix, implement.")
endif()