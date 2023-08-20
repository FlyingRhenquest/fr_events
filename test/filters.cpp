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

#include <gtest/gtest.h>
#include <fr_events/filters.hpp>
#include <fr_events/simple_transport.hpp>
#include <fr_events/zmq_transport.hpp>

using namespace fr::events;
using namespace fr::events::transport;
using namespace fr::events::filters;

class filter_test_handler : public ::fr::events::transport::event_handler {
public:
  int events_received = 0;

  void handle(std::shared_ptr<base_event> event) override {
    events_received++;
  }
};

TEST(filter_test,simple_filter_on_name) {
  simple_sender test_sender;
  simple_receiver test_receiver;
  filter_test_handler test_handler;
  // Make it ignore events with the generic
  // base_event name
  filter name_filter([](std::shared_ptr<base_event> event) {
    return event->name != "base_event";
  });

  // Set up a chain
  test_sender.subscribe(name_filter);
  name_filter.subscribe(test_receiver);
  test_handler.subscribe(test_receiver);

  auto should_be_ignored = std::make_shared<base_event>();
  auto should_be_handled = std::make_shared<base_event>();
  should_be_handled->name = "Bingo!";

  test_sender.send(should_be_ignored);
  ASSERT_EQ(test_handler.events_received,0);
  test_sender.send(should_be_handled);
  ASSERT_EQ(test_handler.events_received,1);
}
