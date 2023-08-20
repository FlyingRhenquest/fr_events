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

#include <fr_events/transport.hpp>

namespace fr::events::transport {

  sender::sender() {}
  sender::~sender() {};

  event_handler::event_handler() {};
  
  event_handler::~event_handler() {
    unsubscribe();
  }

  void event_handler::subscribe(receiver* r) {
    boost::signals2::connection subscription =
      r->received.connect([this](std::shared_ptr<base_event> e) { handle(e); });
    subscriptions.push_back(subscription);
  }

  void event_handler::subscribe(receiver& r) {
    subscribe(&r);
  }

  void event_handler::subscribe(std::shared_ptr<receiver> r) {
    subscribe(r.get());
  }

  void event_handler::unsubscribe() {
    for (auto subscription : subscriptions) {
      subscription.disconnect();
    }
  }

  receiver::receiver() {}

  receiver::~receiver() {}

}
