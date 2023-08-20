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
#include <condition_variable>
#include <fr_events/zmq_transport.hpp>
#include <mutex>
#include <iostream>

using namespace fr::events;
using namespace fr::events::transport;

// We'll need an event handler

class zmq_test_handler : public event_handler {
public:

  // We use these to wait for the event to be received

  std::mutex event_mutex;
  std::condition_variable event_received;

  bool got_event = false;

  // Just register that we received something.
  void handle(std::shared_ptr<base_event> event) override {
    got_event = true;
    event_received.notify_all();
  };
  
};

TEST(ZmqTest, test_basic) {
  zmq_test_handler handler;
  zmq_receiver receiver;
  handler.subscribe(receiver);
  ASSERT_FALSE(handler.got_event);
  // I'm just assumig you don't already have something listening
  // on port 23456
  receiver.add("tcp://127.0.0.1:23456");
  zmq_sender sender;
  sender.add("tcp://127.0.0.1:23456");
  auto event = std::make_shared<base_event>();
  // Give the connection time to establish
  std::this_thread::sleep_for(std::chrono::milliseconds(250));
  sender.send(event);
  std::unique_lock lock(handler.event_mutex);
  handler.event_received.wait(lock, [&handler]{return handler.got_event;});
  ASSERT_TRUE(handler.got_event);
}
