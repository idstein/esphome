#include "datapoint_component.h"
#include "optolink.h"

namespace esphome {
namespace optolink {

static const char *const TAG = "optolink.datapoint_component";

void DatapointComponent::setup_datapoint_() {
  switch (div_ratio_) {
    case DIV_RATIO_BINARY:
      datapoint_ = new VitoWiFi::Datapoint(get_component_name().c_str(), address_, bytes_, ScheduleConverter());
      /*datapoint_->setCallback([this](const VitoWiFi::Datapoint &dp, VitoWiFi::VariantValue dp_value) {
        uint8_t buffer[bytes_];
        dp_value.getRaw(buffer);
#if ESPHOME_LOG_LEVEL >= ESPHOME_LOG_LEVEL_INFO
        char print_buffer[bytes_ * 2 + 1];
        dp_value.getString(print_buffer, sizeof(print_buffer));
        ESP_LOGI(TAG, "recieved data for datapoint %s: %s", dp.getName(), print_buffer);
#endif
        datapoint_value_changed((uint8_t *) buffer, bytes_);
        read_retries_ = 0;
      });*/
      break;
    case DIV_RATIO_RAW:
      datapoint_ = new VitoWiFi::Datapoint(get_component_name().c_str(), address_, bytes_, VitoWiFi::NoconvConvert());
      /*datapoint_->setCallback([this](const VitoWiFi::Datapoint &dp, VitoWiFi::VariantValue dp_value) {
        char print_buffer[bytes_ * 2 + 1];
        dp_value.getString(print_buffer, sizeof(print_buffer));
        ESP_LOGI(TAG, "recieved data for datapoint %s: %s", dp.getName(), print_buffer);
        datapoint_value_changed(print_buffer);
        read_retries_ = 0;
      });*/
      break;
    case 1:
      switch (bytes_) {
        case 1:
          datapoint_ = new VitoWiFi::Datapoint(get_component_name().c_str(), address_, 1, VitoWiFi::NoconvConvert());
          /*datapoint_->setCallback([this](const VitoWiFi::Datapoint &dp, VitoWiFi::VariantValue dp_value) {
            ESP_LOGI(TAG, "recieved data for datapoint %s: %d", dp.getName(), dp_value.getU8());
            datapoint_value_changed(dp_value.getU8());
            read_retries_ = 0;
          });*/
          break;
        case 2:
        case 3:
          datapoint_ = new VitoWiFi::Datapoint(get_component_name().c_str(), address_, 2, VitoWiFi::NoconvConvert());
          /*datapoint_->setCallback([this](const VitoWiFi::Datapoint &dp, VitoWiFi::VariantValue dp_value) {
            ESP_LOGI(TAG, "recieved data for datapoint %s: %d", dp.getName(), dp_value.getU16());
            datapoint_value_changed(dp_value.getU16());
            read_retries_ = 0;
          });*/
          break;
        case 4:
          datapoint_ = new VitoWiFi::Datapoint(get_component_name().c_str(), address_, 4, VitoWiFi::NoconvConvert());
          /*datapoint_->setCallback([this](const VitoWiFi::Datapoint &dp, VitoWiFi::VariantValue dp_value) {
            ESP_LOGI(TAG, "recieved data for datapoint %s: %d", dp.getName(), dp_value.getU32());
            datapoint_value_changed((uint32_t) dp_value.getU32());
            read_retries_ = 0;
          });*/
          break;
        default:
          unfitting_value_type_();
      }
      break;
    case 10:
      switch (bytes_) {
        case 1:
          datapoint_ = new VitoWiFi::Datapoint(get_component_name().c_str(), address_, 1, DivConvert(10.f));
          /*datapoint_->setCallback([this](const VitoWiFi::Datapoint &dp, VitoWiFi::VariantValue dp_value) {
            ESP_LOGI(TAG, "recieved data for datapoint %s: %f", dp.getName(), dp_value.getFloat());
            datapoint_value_changed(dp_value.getFloat());
            read_retries_ = 0;
          });*/
          break;
        case 2:
        case 3:
          datapoint_ = new VitoWiFi::Datapoint(get_component_name().c_str(), address_, 2, DivConvert(10.f));
          /*datapoint_->setCallback([this](const VitoWiFi::Datapoint &dp, VitoWiFi::VariantValue dp_value) {
            ESP_LOGI(TAG, "recieved data for datapoint %s: %f", dp.getName(), dp_value.getFloat());
            datapoint_value_changed(dp_value.getFloat());
            read_retries_ = 0;
          });*/
          break;
        case 4:
          datapoint_ = new VitoWiFi::Datapoint(get_component_name().c_str(), address_, 4, DivConvert(10.f));
        default:
          unfitting_value_type_();
      }
      break;
    case 100:
      switch (bytes_) {
        case 2:
          datapoint_ = new VitoWiFi::Datapoint(get_component_name().c_str(), address_, 2, DivConvert(100.f));
          /*datapoint_->setCallback([this](const VitoWiFi::Datapoint &dp, VitoWiFi::VariantValue dp_value) {
            ESP_LOGI(TAG, "recieved data for datapoint %s: %f", dp.getName(), dp_value.getFloat());
            datapoint_value_changed(dp_value.getFloat());
            read_retries_ = 0;
          });*/
          break;
        default:
          unfitting_value_type_();
      }
      break;
    case 1000:
      switch (bytes_) {
        case 4:
          datapoint_ = new VitoWiFi::Datapoint(get_component_name().c_str(), address_, 4, DivConvert(1000.f));
          /*datapoint_->setCallback([this](const VitoWiFi::Datapoint &dp, VitoWiFi::VariantValue dp_value) {
            ESP_LOGI(TAG, "recieved data for datapoint %s: %f", dp.getName(), dp_value.getFloat());
            datapoint_value_changed(dp_value.getFloat());
            read_retries_ = 0;
          });*/
          break;
      }
      break;
    case 3600:
      switch (bytes_) {
        case 4:
          datapoint_ = new VitoWiFi::Datapoint(get_component_name().c_str(), address_, 4, VitoWiFi::Div3600Convert());
          /*datapoint_->setCallback([this](const VitoWiFi::Datapoint &dp, VitoWiFi::VariantValue dp_value) {
            ESP_LOGI(TAG, "recieved data for datapoint %s: %f", dp.getName(), dp_value.getFloat());
            datapoint_value_changed(dp_value.getFloat());
            read_retries_ = 0;
          });*/
          break;
      }
      break;
    default:
      unfitting_value_type_();
  }
}

void DatapointComponent::datapoint_read_request_() {
  if (is_dp_value_writing_outstanding_) {
    ESP_LOGI(TAG, "read request for %s deferred due to outstanding write request", get_component_name().c_str());
    datapoint_write_request_(dp_value_outstanding_);
  } else {
    if (!optolink_->communication_suspended()) {
      if ((read_retries_ == 0 || read_retries_ >= max_retries_until_reset_) || get_update_interval() > 10000) {
        if (optolink_->read_datapoint(datapoint_)) {
          read_retries_ = 1;
        }
      } else {
        read_retries_++;
        ESP_LOGW(TAG,
                 "%d. read request for %s rejected due to outstanding running request - check datapoint configuration!",
                 read_retries_, get_component_name().c_str());
      }
    } else {
      read_retries_ = 0;
    }
  }
}

void DatapointComponent::datapoint_value_changed(float value) {
  ESP_LOGW(TAG, "unused value update by sensor %s", get_component_name().c_str());
}

void DatapointComponent::datapoint_value_changed(uint8_t value) {
  ESP_LOGW(TAG, "unused value update by sensor %s", get_component_name().c_str());
}

void DatapointComponent::datapoint_value_changed(uint16_t value) {
  ESP_LOGW(TAG, "unused value update by sensor %s", get_component_name().c_str());
}

void DatapointComponent::datapoint_value_changed(uint32_t value) {
  ESP_LOGW(TAG, "unused value update by sensor %s", get_component_name().c_str());
}

void DatapointComponent::datapoint_value_changed(const std::string &value) {
  ESP_LOGW(TAG, "unused value update by sensor %s", get_component_name().c_str());
}

void DatapointComponent::datapoint_value_changed(uint8_t *value, size_t length) {
  ESP_LOGW(TAG, "unused value update by sensor %s", get_component_name().c_str());
}

void DatapointComponent::datapoint_write_request_(VitoWiFi::VariantValue dp_value) {
  if (!writeable_) {
    ESP_LOGE(TAG, "trying to control not writable datapoint %s", get_component_name().c_str());
  } else if (datapoint_ != nullptr && !optolink_->communication_suspended()) {
#if ESPHOME_LOG_LEVEL >= ESPHOME_LOG_LEVEL_INFO
    /*char buffer[100];
    dp_value.getString(buffer, sizeof(buffer));
    ESP_LOGI(TAG, "trying to update datapoint %s value: %s", get_component_name().c_str(), buffer);*/
#endif

    dp_value_outstanding_ = dp_value;
    if (optolink_->write_datapoint(datapoint_, dp_value_outstanding_)) {
      is_dp_value_writing_outstanding_ = false;
    } else {
      ESP_LOGW(TAG, "write request for %s rejected due to outstanding running request - increase update_interval!",
               get_component_name().c_str());
      is_dp_value_writing_outstanding_ = true;
    }
  }
}

void DatapointComponent::write_datapoint_value_(float value) {
  ESP_LOGI(TAG, "sending value %f (%d bytes) to datapoint %s", value, datapoint_->length(), datapoint_->name());
  if (div_ratio_ > 1) {
    datapoint_write_request_(VitoWiFi::VariantValue(value));
  } else if (div_ratio_ == 1) {
    switch (bytes_) {
      case 1:
        datapoint_write_request_(VitoWiFi::VariantValue((uint8_t) value));
        break;
      case 2:
        datapoint_write_request_(VitoWiFi::VariantValue((uint16_t) value));
        break;
      case 4:
        datapoint_write_request_(VitoWiFi::VariantValue((uint32_t) value));
        break;
      default:
        unfitting_value_type_();
        break;
    }
  } else {
    unfitting_value_type_();
  }
}

void DatapointComponent::write_datapoint_value_(uint8_t value) {
  ESP_LOGI(TAG, "sending value %u (%d bytes) to datapoint %s", value, datapoint_->length(), datapoint_->name());
  if (bytes_ == 1 && div_ratio_ == 1) {
    datapoint_write_request_(VitoWiFi::VariantValue(value));
  } else {
    unfitting_value_type_();
  }
}

void DatapointComponent::write_datapoint_value_(uint16_t value) {
  ESP_LOGI(TAG, "sending value %u (%d bytes) to datapoint %s", value, datapoint_->length(), datapoint_->name());
  if (bytes_ == 2 && div_ratio_ == 1) {
    datapoint_write_request_(VitoWiFi::VariantValue(value));
  } else {
    unfitting_value_type_();
  }
}

void DatapointComponent::write_datapoint_value_(uint32_t value) {
  ESP_LOGI(TAG, "sending value %u (%d bytes) to datapoint %s", value, datapoint_->length(), datapoint_->name());
  if (bytes_ == 4 && div_ratio_ == 1) {
    datapoint_write_request_(VitoWiFi::VariantValue(value));
  } else {
    unfitting_value_type_();
  }
}

void DatapointComponent::write_datapoint_value_(uint8_t *value, size_t length) {
  ESP_LOGI(TAG, "sending value %s (%d bytes) to datapoint %s", format_hex(value, length).c_str(), datapoint_->length(),
           datapoint_->name());
  if (bytes_ == length && div_ratio_ == 0) {
    if (!writeable_) {
      ESP_LOGE(TAG, "trying to control not writable datapoint %s", get_component_name().c_str());
    } else if (datapoint_ != nullptr && !optolink_->communication_suspended()) {
      // dp_value_outstanding_ = dp_value;
      if (optolink_->write_datapoint(datapoint_, value, length)) {
        is_dp_value_writing_outstanding_ = false;
      } else {
        ESP_LOGW(TAG, "write request for %s rejected due to outstanding running request - increase update_interval!",
                 get_component_name().c_str());
        // is_dp_value_writing_outstanding_ = true;
      }
    }
  } else {
    unfitting_value_type_();
  }
}

void DatapointComponent::unfitting_value_type_() {
  ESP_LOGE(TAG, "Unfitting byte/div_ratio combination for sensor/component %s", get_component_name().c_str());
}

void ScheduleConverter::encode(uint8_t *buf, uint8_t len, const VitoWiFi::VariantValue &val) const {
  assert(len == 8);
  uint64_t srcVal = val;
  buf[7] = srcVal >> 56;
  buf[6] = srcVal >> 48;
  buf[5] = srcVal >> 40;
  buf[4] = srcVal >> 32;
  buf[3] = srcVal >> 24;
  buf[2] = srcVal >> 16;
  buf[1] = srcVal >> 8;
  buf[0] = srcVal & 0xFF;
}

VitoWiFi::VariantValue ScheduleConverter::decode(const uint8_t *data, uint8_t len) const {
  assert(len == 8);
  uint64_t retVal = ((uint64_t) data[7]) << 56 | ((uint64_t) data[6]) << 48 | ((uint64_t) data[5]) << 40 |
                    ((uint64_t) data[4]) << 32 | data[3] << 24 | data[2] << 16 | data[1] << 8 | data[0];
  return VitoWiFi::VariantValue(retVal);
}

void DivConvert::encode(uint8_t *buf, uint8_t len, const VitoWiFi::VariantValue &val) const {
  float srcVal = val;
  if (len == 1) {
    int8_t tmp = floor((srcVal * divisor_) + 0.5f);
    buf[0] = tmp & 0xFF;
  } else if (len == 2) {
    int16_t tmp = floor((srcVal * divisor_) + 0.5f);
    buf[1] = tmp >> 8;
    buf[0] = tmp & 0xFF;
  } else if (len == 4) {
    int32_t tmp = floor((srcVal * divisor_) + 0.5f);
    buf[3] = tmp >> 24;
    buf[2] = tmp >> 16;
    buf[1] = tmp >> 8;
    buf[0] = tmp & 0xFF;
  }
}

VitoWiFi::VariantValue DivConvert::decode(const uint8_t *data, uint8_t len) const {
  float retVal = 0;
  if (len == 1) {
    int8_t val = data[0];
    retVal = val / divisor_;
  } else if (len == 2) {
    int16_t val = data[1] << 8 | data[0];
    retVal = val / divisor_;
  } else if (len == 4) {
    int32_t tmp = data[3] << 24 | data[2] << 16 | data[1] << 8 | data[0];
    retVal = tmp / divisor_;
  }
  return VitoWiFi::VariantValue(retVal);
}

}  // namespace optolink
}  // namespace esphome
