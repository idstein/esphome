#pragma once

#include "esphome/core/log.h"
#include "esphome/core/string_ref.h"
#include "esphome/core/component.h"
#include "VitoWiFi.h"

namespace esphome {
namespace optolink {

class Optolink;

enum DivRatioSpecial { DIV_RATIO_BINARY = 0, DIV_RATIO_RAW = -1 };

class DatapointComponent : public esphome::PollingComponent {
 public:
  DatapointComponent(Optolink *optolink, bool writeable = false) : dp_value_outstanding_((uint8_t) 0) {
    optolink_ = optolink;
    writeable_ = writeable;
  }

  void set_address(uint32_t address) { address_ = address; }
  void set_bytes(size_t bytes) { bytes_ = bytes; }
  void set_writeable(bool writeable) { writeable_ = writeable; }
  void set_div_ratio(size_t div_ratio) { div_ratio_ = div_ratio; }

 protected:
  virtual const StringRef &get_component_name() = 0;

  uint32_t get_address_() { return address_; }

  void setup_datapoint_();

  void datapoint_read_request_();

  virtual void datapoint_value_changed(float value);
  virtual void datapoint_value_changed(uint8_t value);
  virtual void datapoint_value_changed(uint16_t value);
  virtual void datapoint_value_changed(uint32_t value);
  virtual void datapoint_value_changed(const std::string &value);
  virtual void datapoint_value_changed(uint8_t *value, size_t length);

  void write_datapoint_value_(float value);
  void write_datapoint_value_(uint8_t value);
  void write_datapoint_value_(uint16_t value);
  void write_datapoint_value_(uint32_t value);
  void write_datapoint_value_(uint8_t *value, size_t length);

  void unfitting_value_type_();

  Optolink *optolink_;

 private:
  const size_t max_retries_until_reset_ = 10;
  VitoWiFi::Datapoint *datapoint_ = nullptr;
  size_t read_retries_ = 0;
  int16_t div_ratio_ = 0;
  size_t bytes_;
  uint32_t address_;
  bool writeable_;
  bool is_dp_value_writing_outstanding_ = false;
  VitoWiFi::VariantValue dp_value_outstanding_;

  void datapoint_write_request_(VitoWiFi::VariantValue dp_value);
};

// NOLINTBEGIN
class ScheduleConverter : public VitoWiFi::Converter {
 public:
  VitoWiFi::VariantValue decode(const uint8_t *data, uint8_t len) const override;
  void encode(uint8_t *buf, uint8_t len, const VitoWiFi::VariantValue &val) const override;
};

class DivConvert : public VitoWiFi::Converter {
 public:
  DivConvert(const float divisor) : VitoWiFi::Converter(), divisor_{divisor} {}
  VitoWiFi::VariantValue decode(const uint8_t *data, uint8_t len) const override;
  void encode(uint8_t *buf, uint8_t len, const VitoWiFi::VariantValue &val) const override;

 protected:
  float divisor_;
};
// NOLINTEND

}  // namespace optolink
}  // namespace esphome
