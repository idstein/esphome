#include "esphome/core/defines.h"
#include "esphome/core/log.h"
#include "optolink.h"

namespace esphome {
namespace optolink {

static const char *const TAG = "optolink";

void Optolink::setup() {
  ESP_LOGI(TAG, "setup");
  if (communication_suspension_ == 0) {
    set_state_("communication active");
  } else {
    set_state_("communication state unknown");
  }
#if defined(USE_VS2_PROTOCOL)
  vitoWiFi_.onResponse([this](const VitoWiFi::PacketVS2 &response, const VitoWiFi::Datapoint &request) {
    onResponse(response, request);
  });
  void onResponse(const VitoWiFi::PacketVS2 &response, const VitoWiFi::Datapoint &request);
#else
  vitoWiFi_.onResponse([this](const uint8_t *data, uint8_t length, const VitoWiFi::Datapoint &request) {
    onResponse(data, length, request);
  });
#endif

  vitoWiFi_.begin();
}

void Optolink::loop() {
  if (communication_suspension_ > 0) {
    communication_check_();
  }
  if (!communication_suspended()) {
    vitoWiFi_.loop();
  }
}

// int Optolink::get_queue_size() { return vitoWiFi_.queueSize(); }

void Optolink::set_state_(const char *state) { state_ = state; }

#if defined(USE_VS2_PROTOCOL)
void Optolink::onResponse(const VitoWiFi::PacketVS2 &response, const VitoWiFi::Datapoint &request) {
  notify_receive();
};
#else
void Optolink::onResponse(const uint8_t *data, uint8_t length, const VitoWiFi::Datapoint &request) {
  notify_receive();
};
#endif

void Optolink::notify_receive() { timestamp_receive_ = timestamp_loop_; }

void Optolink::notify_send() { timestamp_send_ = timestamp_loop_; }

void Optolink::communication_check_() {
  timestamp_loop_ = millis();

  if (communication_suspended()) {
    if (timestamp_loop_ < timestamp_disruption_ ||
        (timestamp_loop_ - timestamp_disruption_) > communication_suspension_) {
      resume_communication_();
    } else {
      set_state_("communication suspended");
    }
  } else if (timestamp_loop_ < timestamp_send_ || timestamp_loop_ < timestamp_receive_) {
    ESP_LOGI(TAG, "timestamp rollover");
    timestamp_send_ = 0;
    timestamp_receive_ = 0;
  } else if (timestamp_send_ == 0) {
    // too less data to analyze communication statistics
  } else if ((timestamp_loop_ - timestamp_receive_) > COMMUNICATION_CHECK_WINDOW) {
    // last response older than 10 sec - check if there was no request in same time window except last two seconds
    if (timestamp_send_ > timestamp_loop_ - max_response_delay_) {
      // request too fresh -> possiblly still waiting for response
    } else if (timestamp_send_ < timestamp_receive_) {
      // no new and fresh request since last response
      set_state_("communication unused");
    } else {
      suspend_communication_();
    }
  } else {
    set_state_("communication active");
  }
}

void Optolink::suspend_communication_() {
  set_state_("communication suspended");
  ESP_LOGW(TAG,
           "communication disrupted - suspending communication for %u ms; timestamp_loop: %u, timestamp_send: %u,  "
           "timestamp_receive: %u ",
           communication_suspension_, timestamp_loop_, timestamp_send_, timestamp_receive_);
  timestamp_disruption_ = timestamp_loop_;
}

void Optolink::resume_communication_() {
  if (parent_ == nullptr)
    return;
  set_state_("communication state unknown");
  ESP_LOGI(TAG, "resuming communication");
  timestamp_disruption_ = 0;
  timestamp_send_ = 0;
  timestamp_receive_ = 0;
}

bool Optolink::communication_suspended() {
  if (parent_ == nullptr)
    return true;
  return (timestamp_disruption_ != 0);
}

bool Optolink::read_datapoint(VitoWiFi::Datapoint *datapoint) {
  if (datapoint != nullptr && !communication_suspended()) {
    ESP_LOGI(TAG, "requesting value (%d bytes) from datapoint %s", datapoint->length(), datapoint->name());
    if (vitoWiFi_.read(*datapoint)) {
      notify_send();
    } else {
      // TODO implement simple queue
      /*ESP_LOGE(TAG, "read request rejected due to queue overload - queue size: %d", vitoWiFi_.queueSize());
      for (auto *dp : VitoWiFi::Datapoint::getCollection()) {
        ESP_LOGD(TAG, "queued datapoint: %s", dp->name());
      }*/
      return false;
    }
  }
  return true;
}

bool Optolink::write_datapoint(VitoWiFi::Datapoint *datapoint, VitoWiFi::VariantValue dp_value) {
  if (datapoint != nullptr && !communication_suspended()) {
    ESP_LOGI(TAG, "sending %d bytes to datapoint %s", datapoint->length(), datapoint->name());
    if (vitoWiFi_.write(*datapoint, dp_value)) {
      notify_send();
    } else {
      // TODO implement simple queue
      /*ESP_LOGE(TAG, "write request rejected due to queue overload - queue size: %d", vitoWiFi_.queueSize());
      for (auto *dp : VitoWiFi::Datapoint::getCollection()) {
        ESP_LOGE(TAG, "queued dp: %s", dp->name());
      }*/
      return false;
    }
  }
  return true;
}

bool Optolink::write_datapoint(VitoWiFi::Datapoint *datapoint, const uint8_t *value, uint8_t length) {
  if (datapoint != nullptr && !communication_suspended()) {
    ESP_LOGI(TAG, "sending %d bytes to datapoint %s", datapoint->length(), datapoint->name());
    if (vitoWiFi_.write(*datapoint, value, length)) {
      notify_send();
    } else {
      // TODO implement simple queue
      /*ESP_LOGE(TAG, "write request rejected due to queue overload - queue size: %d", vitoWiFi_.queueSize());
      for (auto *dp : VitoWiFi::Datapoint::getCollection()) {
        ESP_LOGE(TAG, "queued dp: %s", dp->name());
      }*/
      return false;
    }
  }
  return true;
}

}  // namespace optolink
}  // namespace esphome
