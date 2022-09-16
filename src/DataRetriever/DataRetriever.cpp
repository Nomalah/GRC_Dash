#include "DataRetriever.hpp"
#include <exception>

using namespace CanBus;

DataRetriever::DataRetriever(QObject* parent) : QObject(parent) {}

void DataRetriever::startReceiving(const char* canbus_interface_name, const std::vector<can_filter>& filters) {
    if (this->openSocket(canbus_interface_name, mask) != RetCode::Success) {
        throw std::exception("Socket could not be opened");
    }

    m_reading_thread = std::thread([]() {
        can_frame frame = {};
        while (!m_should_exit) {
            switch (this->read(frame)) {
            case RetCode::Success:
                emit newFrame(frame);
                break;
            case RetCode::Timeout:
                break;
            default:
                emit newError();
                break;
            }
        }
        m_should_exit = false;
    });
}

void DataRetriever::stopReceiving() {
    m_should_exit = true;
    if (m_reading_thread.joinable()) {
        m_reading_thread.join();
    }
    ::close(m_socket);
}

RetCode DataRetriever::openSocket(const char* canbus_interface_name, const std::vector<can_filter>& filters) {
    m_socket = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (m_socket < 0) {
        return RetCode::SocketErr;
    }
    ifreq ifr;

    strncpy(ifr.ifr_name, can_interface.c_str(), IFNAMSIZ - 1);
    ifr.ifr_name[IFNAMSIZ - 1] = '\0';
    ioctl(m_socket, SIOCGIFINDEX, &ifr);

    sockaddr_can addr;
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

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

    if (bind(m_socket, &addr, sizeof(addr)) < 0) {
        return RetCode::SocketErr;
    }
    return RetCode::Success;
}

RetCode DataRetriever::read(can_frame& frame) {
    // Read in a CAN frame
    auto num_bytes = ::read(m_socket, &frame, sizeof(frame));
    if (num_bytes != sizeof(frame)) {
        return RetCode::ReadErr;
    }
    return RetCode::Success;
}

bool DataRetriever::isError(can_frame frame) {
    return frame.can_id & CAN_ERR_FLAG;
}

bool DataRetriever::isRemoteTransmissionRequest(can_frame frame) {
    return frame.can_id & CAN_RTR_FLAG;
}

CanFormat DataRetriever::frameFormat(can_frame frame) {
    if (frame & CAN_EFF_FLAG) {
        return CanFormat::Extended
    }
    return CanFormat::Standard;
}

canid_t DataRetriever::frameId(can_frame frame) {
    switch (DataRetriever::frameFormat(frame)) {
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
