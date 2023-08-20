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
 *
 * Filters for events.
 */

#pragma once
#include <fr_events/base_event.hpp>
#include <fr_events/transport.hpp>
#include <functional>
#include <memory>

namespace fr::events::filters {

  /**
   * Filters are both receivers and senders. Create a filter with a lambda that takes
   * shared pointer to base_event and have it return true if you want it to forward
   * the event and false if you don't want it to forward the event.
   */

  class filter : public fr::events::transport::sender, public fr::events::transport::receiver {
    std::function<bool(std::shared_ptr<fr::events::base_event>)> condition;
  public:
    filter(std::function<bool(std::shared_ptr<fr::events::base_event>)>);
    virtual ~filter() override;

    void send(std::shared_ptr<fr::events::base_event> event) override;
  };
}
