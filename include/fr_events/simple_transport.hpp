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

#pragma once
#include <fr_events/transport.hpp>

namespace fr::events::transport {

  class simple_receiver;
  
  /**
   * A simple sender that dispatches events in the same thread as the receiver.
   */
  
  class simple_sender : public sender {
  private:
    std::vector<simple_receiver*> receivers;
  public:

    simple_sender();
    virtual ~simple_sender();

    void send(std::shared_ptr<fr::events::base_event> event) override;

    /**
     * Subscribe a receiver to this sender
     */
    void subscribe(simple_receiver& r);
    void subscribe(simple_receiver* r);
    void subscribe(std::shared_ptr<simple_receiver> rec);
    void unsubscribe(simple_receiver* r);
  };

  /**
   * Receives events from simple sender
   */
  
  class simple_receiver : public receiver {
  private:
    std::vector<simple_sender*> subscriptions;
  public:
    simple_receiver();
    virtual ~simple_receiver() override;

    void add(simple_sender* subscription);
  };

}
