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
 * Provides a ZMQ Transport for Events. This will send events to
 * ZMQ Queues.
 *
 */

#pragma once

// Enable ZMQ experimental features
#define ZMQ_BUILD_DRAFT_API
#define ZMQ_CPP11
#define ZMQ_HAVE_POLLER

#include <atomic>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/polymorphic.hpp>
#include <fr_events/base_event.hpp>
#include <fr_events/transport.hpp>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include <zmq.hpp>
#include <zmq_addon.hpp>

namespace fr::events::transport {

  // Hold the various ZMQ bits we need for a connection
  struct zmq_data {
    zmq_data() {};
    ~zmq_data() {}
    std::string address;
    zmq::context_t context;
    std::shared_ptr<zmq::socket_t> socket;
  };
  
  /**
   * A sender that sends serialized events via ZMQ. Creating one
   * of these objects starts a service thread from which events
   * are serialized (to portable binary) and dispatched.
   */
  
  class zmq_sender : public sender {

    std::vector<std::shared_ptr<zmq_data>> connections;
      
    // We'll just grab the shared pointer to the event into a vector
    // until we can send it.

    std::vector<std::shared_ptr<fr::events::base_event>> pending;
    std::thread service;
    std::mutex pending_mutex;

    // This gets set to true to shut down the service thread.
    // This only ever happens when the object is deleted.
    // Event queue will be drained prior to exit.
    std::atomic<bool> shutdown = false;

    // The function that runs in service.
    void service_function();

    // Drains the pending queue
    void drain();

  public:
    zmq_sender();
    virtual ~zmq_sender() override;

    /**
     * Creates a ZMQ connection to a remote address. The service
     * thread will dispatch pending data to all existing addresses.
     */
    void add(const std::string& address);

    /**
     * This just puts the base event pointer into pending.
     * It makes no guarantees when the event will actually
     * be sent.
     */
    void send(std::shared_ptr<base_event> event) override;
    
  };

  /**
   * zmq_receiver runs a service that receives and distributes events
   * from a rempote sender. You should start your receivers before
   * starting senders, or the senders will have nothing to send to.
   */

  class zmq_receiver : public receiver {
    std::vector<std::shared_ptr<zmq_data>> connections;
    // Polls and deserializes messages from connection
    zmq::active_poller_t poller;

    // Events are received into pending prior to being dispatched
    // to handlers
    std::vector<std::shared_ptr<fr::events::base_event>> pending;
    std::mutex pending_mutex;
    // Thread for receiving messages
    std::thread receiver;
    // Thread for dispatching events
    std::thread dispatcher;

    // Function to receive messages
    void receiver_service();
    // Function to dispatch messages
    void dispatcher_service();
    // Flag to shut down services. This gets set when the object is
    // destroyed.
    std::atomic<bool> shutdown = false;

  public:
    zmq_receiver();
    virtual ~zmq_receiver() override;

    void add(const std::string& listen_on);
  };
}

