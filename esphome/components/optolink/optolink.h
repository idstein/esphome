#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"
#include "VitoWiFi.h"

namespace esphome {
namespace optolink {

class Optolink : public esphome::Component, public uart::UARTDevice, public VitoWiFiInternals::SerialInterface {
 protected:
  std::string state_ = "initializing";
  uint32_t timestamp_loop_ = 0;
  uint32_t timestamp_disruption_ = 0;
  uint32_t timestamp_receive_ = 0;
  uint32_t timestamp_send_ = 0;
  uint32_t communication_suspension_ = 20000;
  uint32_t max_response_delay_ = 2000;

  static const uint32_t COMMUNICATION_CHECK_WINDOW = 10000;

 public:
  Optolink() : esphome::Component(), uart::UARTDevice(), VitoWiFiInternals::SerialInterface(), vitoWiFi_(this) {}
  void setup() override;

  void loop() override;

  void set_communication_suspension(uint32_t communication_suspension) {
    communication_suspension_ = communication_suspension;
  }
  void set_max_response_delay(uint32_t set_max_response_delay) { max_response_delay_ = set_max_response_delay; }

  bool write_datapoint(VitoWiFi::Datapoint *datapoint, VitoWiFi::VariantValue dp_value);
  bool write_datapoint(VitoWiFi::Datapoint *datapoint, const uint8_t *value, uint8_t length);
  bool read_datapoint(VitoWiFi::Datapoint *datapoint);

  std::string get_state() { return state_; }

  // int get_queue_size();
#if defined(USE_VS2_PROTOCOL)
  void onResponse(const VitoWiFi::PacketVS2 &response, const VitoWiFi::Datapoint &request);
#else
  void onResponse(const uint8_t *data, uint8_t length, const VitoWiFi::Datapoint &request);
#endif
  bool communication_suspended();
  void notify_receive();
  void notify_send();

  // VitoWiFiInternals::SerialInterface
  bool begin() { return parent_ != nullptr; }
  void end() {}
  std::size_t write(const uint8_t *data, uint8_t length) {
    if (parent_ == nullptr)
      return 0;
    write_array(data, length);
    return length;
  }
  uint8_t read() override {
    if (parent_ == nullptr)
      return -1;
    return esphome::uart::UARTDevice::read();
  }
  size_t available() override {
    if (parent_ == nullptr)
      return 0;
    return esphome::uart::UARTDevice::available();
  }

 private:
  void set_state_(const char *state);

  void communication_check_();
  void suspend_communication_();
  void resume_communication_();

  VitoWiFi::VitoWiFi<USE_OPTOLINK_VITOWIFI_PROTOCOL> vitoWiFi_;  // NOLINT
};

}  // namespace optolink
}  // namespace esphome
