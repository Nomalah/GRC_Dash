#pragma once

#include <Interface.hpp>

namespace CAN::Interface {

class Dump : public Interface {
  public:
    CanBusDump(); 
    ~CanBusDump() = default;

    void startReceiving();

  private:
    // As of now, newFrame and newError should be extremely fast functions before emitting more signals
    void newFrame(const can_frame&) override;
    void newError() override;

  private:
    can_filter m_filters[1] = {
        {
            0, 0 // Grab all messages
        }
    };
};

}