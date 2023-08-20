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

#include <fr_events/event_buffer.hpp>
#include <stdexcept>

namespace fr::events {

  event_buffer::event_buffer() {}
  event_buffer::~event_buffer() {}

  void event_buffer::handle(std::shared_ptr<base_event> event) {
    std::lock_guard<std::mutex> lock(buffer_mutex);
    buffer.push_back(event);
  }
  
  std::shared_ptr<base_event> event_buffer::get() {
    std::lock_guard<std::mutex> lock(buffer_mutex);
    if (!buffer.size()) {
      throw std::logic_error("No events in buffer to retrieve.");
    }
    std::shared_ptr<base_event> event = buffer.front();
    buffer.erase(buffer.begin());
    return event;
  }

  size_t event_buffer::size() {
    std::lock_guard<std::mutex> lock(buffer_mutex);
    return buffer.size();
  }
  
}
