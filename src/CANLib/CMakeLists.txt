set(CODE_FILES
    Receiver.cpp
)

set(HEADER_FILES
    Receiver.hpp
)

add_library(CanBusReceiver ${CODE_FILES} ${HEADER_FILES})
set_target_properties(CanBusReceiver PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/$<0:>)
target_include_directories(CanBusReceiver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})