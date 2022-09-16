#pragma once

#include <QObject>
#include <atomic>
#include <cstdint>
#include <thread>
#include <vector>

#include <linux/can.h>

namespace CanBus {

enum class CanFormat : uint8_t { Standard, Extended };

enum class RetCode { Success, ReadErr, OpenErr, Timeout, SocketErr };

class DataRetriever {
  public:
    DataRetriever(QObject* parent = nullptr); 
    virtual ~DataRetriever() = default;

  protected:
    void startReceiving(const char* canbus_interface_name, const std::vector<can_filter>& filters);
    void stopReceiving();

    // As of now, newFrame and newError should be extremely fast functions before emitting more signals
    virtual void newFrame(const can_frame) = 0;
    virtual void newError() = 0;

  protected:
    static bool isError(can_frame frame);
    static bool isRemoteTransmissionRequest(can_frame frame);
    static CanFormat frameFormat(can_frame frame);
    static canid_t frameId(can_frame frame);

  private:
    RetCode SocketCan::openSocket(const char* canbus_interface_name, const std::vector<can_filter>& filters);
    RetCode SocketCan::read(can_frame& frame);

  private:
    int32_t m_socket = -1;
    uint32_t m_read_timeout_ms = 50; // 50ms timeout for reading by default

    std::thread m_reading_thread;
    std::atomic<bool> m_should_exit;
};

} // namespace CanBus