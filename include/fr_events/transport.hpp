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
 */

#pragma once

#include <boost/signals2.hpp>
#include <fr_events/base_event.hpp>
#include <memory>
#include <vector>

namespace fr::events::transport {

  class receiver; // Predeclare receiver
  
  /**
   * Sender maintains a service used to send events to a remote
   * endpoint. This could be local memory, shared memory, a
   * MQ Queue or whatever. The base class only defines the API
   * to dispatch an event to the sender.
   *
   * Sender is responsible for serializing an event into a format
   * that receiver can understand.
   *
   * This shares ownership of the event. It would probably be good
   * practice to drop your copy of the event pointer after you
   * submit it to this class.
   */
  
  class sender {
  public:
    sender();
    virtual ~sender();

    virtual void send(std::shared_ptr<base_event> event) = 0;
  };

  /**
   * Event handlers can subscribe to receivers. You have to subclass it
   * to implement the handle() routine.
   */

  class event_handler {
  protected:
    std::vector<boost::signals2::connection> subscriptions;

  public:
    event_handler();

    /**
     * All subscriptions are dropped when the event handler is destroyed.
     */
    
    virtual ~event_handler();

    /**
     * The code that actually handles the event. This must be implemented by the user.
     */
    
    virtual void handle(std::shared_ptr<base_event>) = 0;

    /**
     * Subscribe to a receiver
     */
    
    virtual void subscribe(receiver& r);
    virtual void subscribe(receiver* r);
    virtual void subscribe(std::shared_ptr<receiver> r);

    /**
     * Unsubscribe all subscriptions
     */
    
    virtual void unsubscribe();
  };
  
  /**
   * Receiver receives and dispatches events. This is subscriber-based,
   * so you register a callback to actually receive the event, but also
   * provides a process method that receives one event from the queue and
   * dispatches it to all subscribers.
   */
  
  class receiver {
  public:
    receiver();
    virtual ~receiver();

    /**
     * This is the signal you subscribe to in order to receive events from
     * the receiver. The subscribe method in event_handler will automatically 
     * set up handlers to subscribe to it, but you can also just subscribe 
     * something to it manually using boost signals if you prefer.
     */
    
    boost::signals2::signal<void(std::shared_ptr<base_event>)> received;

  };
  
}
