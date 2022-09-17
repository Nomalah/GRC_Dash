#include "DataRetriever.hpp"
#include <exception>

using namespace CanBus;

Receiver::Receiver(uint32_t read_timeout) : m_read_timeout(50) {}

void Receiver::startReceiving(const char* canbus_interface_name, const std::vector<can_filter>& filters) {
    if (this->openSocket(canbus_interface_name, mask) != RetCode::Success) {
        throw std::exception("Socket could not be opened");
    }

    m_reading_thread = std::thread([]() {
        can_frame frame = {};
        while (!m_should_exit) {
            switch (this->read(frame)) {
            case RetCode::Success:
                newFrame(frame);
                break;
            case RetCode::Timeout:
                break;
            default:
                newError();
                break;
            }
        }
        m_should_exit = false;
    });
}

void Receiver::stopReceiving() {
    m_should_exit = true;
    if (m_reading_thread.joinable()) {
        m_reading_thread.join();
    }
    ::close(m_socket);
}

RetCode Receiver::openSocket(const char* canbus_interface_name, const std::vector<can_filter>& filters) {
    m_socket = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (m_socket < 0) {
        return RetCode::SocketErr;
    }
    ifreq ifr;

    strncpy(ifr.ifr_name, can_interface.c_str(), IFNAMSIZ - 1);
    ifr.ifr_name[IFNAMSIZ - 1] = '\0';
    ioctl(m_socket, SIOCGIFINDEX, &ifr);

    if (filters.size() > 0) {
        if (setsockopt(m_socket, SOL_CAN_RAW, CAN_RAW_FILTER, &filters.data(), sizeof(can_filter) * filters.size()) < 0) {
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
    if (bind(m_socket, &addr, sizeof(addr)) < 0) {
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

bool Receiver::isError(can_frame frame) {
    return frame.can_id & CAN_ERR_FLAG;
}

bool Receiver::isRemoteTransmissionRequest(can_frame frame) {
    return frame.can_id & CAN_RTR_FLAG;
}

CanFormat Receiver::frameFormat(can_frame frame) {
    if (frame & CAN_EFF_FLAG) {
        return CanFormat::Extended
    }
    return CanFormat::Standard;
}

canid_t Receiver::frameId(can_frame frame) {
    switch (Receiver::frameFormat(frame)) {
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