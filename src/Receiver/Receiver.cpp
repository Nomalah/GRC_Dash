#include <cassert>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <linux/can/raw.h>

#include "Receiver.hpp"

using namespace CanBus;

Receiver::Receiver(uint32_t read_timeout_ms) : m_read_timeout_ms(50) {}

void Receiver::readLoop(){
    can_frame frame = {};
    while (!this->m_should_exit) {
        switch (this->read(frame)) {
        case RetCode::Success:
            this->newFrame(frame);
            break;
        case RetCode::Timeout:
            break;
        default:
            this->newError();
            break;
        }
    }
    this->m_should_exit = false;
}

void Receiver::stopReceiving() {
    m_should_exit = true;
    if (m_reading_thread.joinable()) {
        m_reading_thread.join();
    }
    ::close(m_socket);
}

RetCode Receiver::openSocket(const char* canbus_interface_name, can_filter* filters, size_t filter_count) {
    m_socket = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (m_socket < 0) {
        return RetCode::SocketErr;
    }
    ifreq ifr;

    strncpy(ifr.ifr_name, canbus_interface_name, IFNAMSIZ - 1);
    ifr.ifr_name[IFNAMSIZ - 1] = '\0';
    ioctl(m_socket, SIOCGIFINDEX, &ifr);

    if (filter_count > 0) {
        if (setsockopt(m_socket, SOL_CAN_RAW, CAN_RAW_FILTER, &filters, sizeof(can_filter) * filter_count) < 0) {
            return RetCode::SocketErr;
        }
    }

    timeval tv;
    tv.tv_sec = 0;                         
    tv.tv_usec = m_read_timeout_ms * 1000; // 50 milliseconds
    
    if (setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(timeval)) < 0) {
        return RetCode::SocketErr;
    }

    
    sockaddr_can addr;
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    if (bind(m_socket, (sockaddr*)(&addr), sizeof(addr)) < 0) {
        return RetCode::SocketErr;
    }
    return RetCode::Success;
}

RetCode Receiver::read(can_frame& frame) {
    // Read in a CAN frame
    auto num_bytes = ::read(m_socket, &frame, sizeof(frame));
    if (num_bytes != sizeof(frame)) {
        return RetCode::ReadErr;
    }
    return RetCode::Success;
}

bool CanBus::isError(const can_frame& frame) {
    return frame.can_id & CAN_ERR_FLAG;
}

bool CanBus::isRemoteTransmissionRequest(const can_frame& frame) {
    return frame.can_id & CAN_RTR_FLAG;
}

CanFormat CanBus::frameFormat(const can_frame& frame) {
    if (frame.can_id & CAN_EFF_FLAG) {
        return CanFormat::Extended;
    }
    return CanFormat::Standard;
}

canid_t CanBus::frameId(const can_frame& frame) {
    switch (frameFormat(frame)) {
    case CanFormat::Standard:
        return frame.can_id & CAN_SFF_MASK; // 11 LSB
    case CanFormat::Extended:
        return frame.can_id & CAN_EFF_MASK; // 29 LSB
    default:
        break;
    }
    assert(false); // Only 2 formats supported
    return 0;
}

Receiver::~Receiver() {
    this->stopReceiving();
}