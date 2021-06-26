/*
 * Copyright 2021 Andrew Rossignol andrew.rossignol@gmail.com
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "nerfnet/net/mock_link.h"

#include <gtest/gtest.h>

#include "nerfnet/util/log.h"
#include "nerfnet/util/time.h"

namespace nerfnet {

MockLink::MockLink(const Config& config, uint32_t address)
    : Link(address),
      config_(config),
      start_time_us_(TimeNowUs()),
      beacon_count_(0),
      receive_count_(0) {}

void MockLink::WaitForComplete() {
  while (RelativeTimeUs() <= config_.mock_time_us) {
    SleepUs(100);
  }
}

Link::TransmitResult MockLink::Beacon() {
  uint64_t relative_time_us = RelativeTimeUs();
  uint64_t expected_beacon_time_us = beacon_count_ * config_.beacon_interval_us;
  EXPECT_GE(relative_time_us, expected_beacon_time_us);
  EXPECT_LT(relative_time_us, expected_beacon_time_us + 10000);

  size_t result_index = beacon_count_++ % config_.beacon_result_pattern.size();
  return config_.beacon_result_pattern[result_index];
}

Link::ReceiveResult MockLink::Receive(Frame* frame) {
  if (receive_count_ >= config_.receive_result.size()) {
    return ReceiveResult::NOT_READY;
  }

  const auto& receive_frame = config_.receive_result[receive_count_++];
  if (receive_frame.first == ReceiveResult::SUCCESS) {
    *frame = receive_frame.second;
  }

  return receive_frame.first;
}

Link::TransmitResult MockLink::Transmit(const Frame& frame) {
  // TODO(aarossig): Provide a mocking mechanism.
  return TransmitResult::SUCCESS;
}

uint32_t MockLink::GetMaxPayloadSize() const {
  return config_.max_payload_size;
}

uint64_t MockLink::RelativeTimeUs() const {
  return TimeNowUs() - start_time_us_;
}

}  // namespace nerfnet
