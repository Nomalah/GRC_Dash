#pragma once

#include "DataRetriever.hpp"

namespace CanBus {

class CanBusDump : public Receiver {
  public:
    CanBusDump(); 
    ~CanBusDump() = default;

    void startReceiving();

  private:
    // As of now, newFrame and newError should be extremely fast functions before emitting more signals
    void newFrame(const can_frame) override;
    void newError() override;
};

}