/**
 * Copyright 2023 Bruce Ide
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include <fr_events/simple_transport.hpp>

namespace fr::events::transport {

simple_sender::simple_sender() {}
simple_sender::~simple_sender() {}

  void simple_sender::send(std::shared_ptr<fr::events::base_event> event) {
    // Call receiver->received() directly -- this blocks the current
    // thread until all subscribers ahve received the signal.
    for (auto receiver: receivers) {
      receiver->received(event);
    }
  }

  simple_receiver::simple_receiver() {}
  simple_receiver::~simple_receiver() {}

}
