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
 * Defines a buffer that just holds events after they've been received.
 * This will be useful for the python API which currently doesn't have
 * a way to receive events as well as programs that want to use the
 * event subsystem but don't want to bother with subscribing directly
 * to the event reciever.
 */

#pragma once

#include <fr_events/base_event.hpp>
#include <fr_events/transport.hpp>
#include <memory>
#include <mutex>
#include <vector>

namespace fr::events {

  class event_buffer : public ::fr::events::transport::event_handler {
    std::mutex buffer_mutex;
    std::vector<std::shared_ptr<base_event>> buffer;

  public:
    event_buffer();
    virtual ~event_buffer() override;

    // Queues events up to the buffer as they come in
    void handle(std::shared_ptr<base_event> event) override;
    // retrieves one event from the buffer. This removes the event from
    // the buffer and returns it to the caller.
    std::shared_ptr<base_event> get();
    // Returns the number of events in the buffer.
    size_t size();
  };
}
