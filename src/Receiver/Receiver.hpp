#pragma once

#include <atomic>
#include <cstdint>
#include <thread>
#include <vector>

#include <linux/can.h>

namespace CanBus {

enum class CanFormat : uint8_t { Standard, Extended };

enum class RetCode { Success, ReadErr, OpenErr, Timeout, SocketErr };

bool isError(can_frame frame);
bool isRemoteTransmissionRequest(can_frame frame);
CanFormat frameFormat(can_frame frame);
canid_t frameId(can_frame frame);

class Receiver {
  public:
    Receiver(uint32_t read_timeout_ms = 50); 
    virtual ~Receiver() = default;

  protected:
    void startReceiving(const char* canbus_interface_name, const std::vector<can_filter>& filters);
    void stopReceiving();

    // As of now, newFrame and newError should be extremely fast functions before emitting more signals
    virtual void newFrame(const can_frame) = 0;
    virtual void newError() = 0;

  private:
    RetCode SocketCan::openSocket(const char* canbus_interface_name, const std::vector<can_filter>& filters);
    RetCode SocketCan::read(can_frame& frame);

  private:
    int32_t m_socket = -1;
    uint32_t m_read_timeout_ms; // 50ms timeout for reading by default

    std::thread m_reading_thread;
    std::atomic<bool> m_should_exit;
};

} // namespace CanBus