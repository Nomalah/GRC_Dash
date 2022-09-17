#include <fmt/core.h>

#include "CanBusDump.hpp"

using namespace CanBus;

CanBusDump::CanBusDump() 
    : Receiver(150){
}

void CanBusDump::startReceiving(){
    this->Receiver::startReceiving("can0", {}); // Get everything
}

void CanBusDump::newFrame(const can_frame) override {
    fmt::print("ID: 0x{:02X}, Ext: {}, RTR: {}, Err: {}, Payload: 0x{:02X} 0x{:02X} 0x{:02X} 0x{:02X} 0x{:02X} 0x{:02X} 0x{:02X} 0x{:02X}\n",
        CanBus::frameId(can_frame), CanBus::frameFormat(can_frame) == CanFormat::Extended, CanBus::isError(can_frame),
        can_frame.data[0], can_frame.data[1], can_frame.data[2], can_frame.data[3], can_frame.data[4], can_frame.data[5], can_frame.data[6], can_frame.data[7]
    )
}

void CanBusDump::newError() override {
    fmt::print("Error\n");
};