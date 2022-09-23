#include <fmt/core.h>

#include "CanBusDump.hpp"

using namespace CAN;

CanBusDump::CanBusDump() 
    : Receiver(150){
}

void CanBusDump::startReceiving(){
    this->Receiver::startReceiving("can0", m_filters); // Get everything
}

void CanBusDump::newFrame(const can_frame& frame) {
    fmt::print("ID: 0x{:02X}, Ext: {}, RTR: {}, Err: {}, Payload: 0x{:02X} 0x{:02X} 0x{:02X} 0x{:02X} 0x{:02X} 0x{:02X} 0x{:02X} 0x{:02X}\n",
        CAN::frameId(frame), CAN::frameFormat(frame) == CanFormat::Extended, CAN::isError(frame),
        frame.data[0], frame.data[1], frame.data[2], frame.data[3], frame.data[4], frame.data[5], frame.data[6], frame.data[7]
    );
}

void CanBusDump::newError() {
    fmt::print("Error\n");
};