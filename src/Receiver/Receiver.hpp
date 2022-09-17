#pragma once

#include <atomic>
#include <cstdint>
#include <thread>
#include <vector>
#include <stdexcept>

#include <linux/can.h>

namespace CanBus {

enum class CanFormat : uint8_t { Standard, Extended };

enum class RetCode { Success, ReadErr, OpenErr, Timeout, SocketErr };

bool isError(const can_frame& frame);
bool isRemoteTransmissionRequest(const can_frame& frame);
CanFormat frameFormat(const can_frame& frame);
canid_t frameId(const can_frame& frame);

class Receiver {
  public:
    Receiver(uint32_t read_timeout_ms = 50); 
    virtual ~Receiver();

  protected:
    template <size_t N>
    void startReceiving(const char* canbus_interface_name, can_filter (&filters)[N]){
        if (this->openSocket(canbus_interface_name, filters, N) != RetCode::Success) {
            throw std::runtime_error("Socket could not be opened");
        }

        m_reading_thread = std::thread(&Receiver::readLoop, this);
    }
    void stopReceiving();

    // As of now, newFrame and newError should be extremely fast functions before emitting more signals
    virtual void newFrame(const can_frame&) = 0;
    virtual void newError() = 0;

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

} // namespace CanBus