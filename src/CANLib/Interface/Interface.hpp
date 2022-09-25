#pragma once

#include <atomic>
#include <cstdint>
#include <cstddef>
#include <thread>
#include <vector>
#include <stdexcept>

#include <defs.hpp>

namespace CAN {

class Interface {
  public:
    Interface(uint32_t read_timeout_ms = 50); 
    virtual ~Interface();

  protected:
    template <size_t N>
    RetCode startReceiving(const char* canbus_interface_name, can_filter (&filters)[N]){
        if (this->openSocket(canbus_interface_name, filters, N) != RetCode::Success) {
            return RetCode::SocketErr;
        }

        m_reading_thread = std::thread(&Interface::readLoop, this);
        return RetCode::Success;
    }
    void stopReceiving();

    // As of now, newFrame and newError should be extremely fast functions before emitting more signals
    virtual void newFrame(const can_frame&) = 0;
    virtual void newError() = 0;

    template <size_t N>
    RetCode write(uint32_t address, const std::byte (&payload)[N], bool extended = false, bool error_frame = false, bool remote_transmission_request = false){
        static_assert(1 <= N && N <= 8, "Size of payload must be between 1 and 8");
        if (address & 0xE0000000) { // Disallow passing anything but the address.
            return RetCode::InvalidParam;
        }

        if ((address & 0x1FFFF800) && !extended) { // Check if there are any bits in the extended section.
            return RetCode::InvalidParam;
        }

        if (extended) {
            address |= CAN_EFF_FLAG; // Set extended format.
        }
        
        can_frame frame;
        frame.can_id = address;
        frame.len = N;
        memcpy(frame.data, payload, N);
        
        if (::write(m_socket, (void*)&frame, sizeof(frame)) != sizeof(frame)) {
            return RetCode::WriteErr;
        }

        return RetCode::Success;
    }

  private:
    RetCode openSocket(const char* canbus_interface_name, can_filter* filters, size_t filter_count);
    RetCode read(can_frame& frame);
    void readLoop();

  private:
    int32_t m_socket = -1;
    uint32_t m_read_timeout_ms; // 50ms timeout for reading by default

    std::thread m_reading_thread;
    std::atomic<bool> m_should_exit = false;
};

} // namespace CAN