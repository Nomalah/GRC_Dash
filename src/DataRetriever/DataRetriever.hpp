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

class DataRetriever : public QObject {
    Q_OBJECT
  public:
    DataRetriever(QObject* parent = nullptr);
    ~DataRetriever() = default;

  protected:
    void startReceiving(const char* canbus_interface_name, const std::vector<can_filter>& filters);
    void stopReceiving();

  signals:
    void newFrame(const can_frame);
    void newError();

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