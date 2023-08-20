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

#include <chrono>
#include <fr_events/zmq_transport.hpp>
#include <sstream>

namespace fr::events::transport {

  zmq_sender::zmq_sender() {
    service = std::thread([this]{this->service_function();});
  }

  zmq_sender::~zmq_sender() {
    shutdown = true;
    if (service.joinable()) {
      service.join();
    }
  }

  void zmq_sender::service_function() {
    while(!shutdown) {
      if (pending.size()) {
	drain();
      }
    }
    // Shutting down -- final drain
    if (pending.size()) {
      drain();
    }
  }

  void zmq_sender::drain() {
    while(pending.size()) {
      std::shared_ptr<base_event> event;
      {
	std::lock_guard<std::mutex> lock(pending_mutex);
	event = pending.back();
	pending.pop_back();
      }
      // Serialize and transport the event
      std::stringstream buffer;
      zmq::multipart_t multimessage;
      {
	cereal::BinaryOutputArchive oarch(buffer);
	oarch(event);
      }
      multimessage.addstr(buffer.str());
      for (auto connection : connections) {
	if (connection->socket->connected()) {
	  multimessage.send(*(connection->socket));
	} else {
	  // TODO: Throw or something here?
	}
      }
    }
  }

  void zmq_sender::add(const std::string& address) {
    std::shared_ptr<zmq_data> new_connection = std::make_shared<zmq_data>();
    new_connection->address = address;
    new_connection->socket = std::make_shared<zmq::socket_t>(new_connection->context, zmq::socket_type::pub);
    new_connection->socket->connect(address);
    connections.push_back(new_connection);
  }

  void zmq_sender::send(std::shared_ptr<fr::events::base_event> event) {
    std::lock_guard<std::mutex> lock(pending_mutex);
    pending.push_back(event);
  }

  zmq_receiver::zmq_receiver() {
    receiver = std::thread([this](){ this->receiver_service(); });
    dispatcher = std::thread([this](){this->dispatcher_service(); });
  }

  zmq_receiver::~zmq_receiver() {
    shutdown = true;
    if (receiver.joinable()) {
      receiver.join();
    }
    if (dispatcher.joinable()) {
      dispatcher.join();
    }
  }

  void zmq_receiver::receiver_service() {
    // This just keeps waiting for polling message.
    // The actual event processing gets added to the poller
    // in the add(listener) function
    
    // Note: timeout is just a max time timer, so it can be
    // fairly long. The only reason to make it shorter is if you
    // want the object to exit more quickly when it's destroyed.
    // Otherwise these should just fire whenever a message comes
    // in.
    std::chrono::milliseconds timeout(250);
    while(!shutdown) {
      if (poller.size()) {
	auto nsocks = poller.wait(timeout);
      } else {
	// Wait a while and see if anyone adds a poller
	std::this_thread::sleep_for(timeout);
      }
    }
  }

  void zmq_receiver::dispatcher_service() {
    // This just pulls any events off the queue and dipatches them
    // to all subscribed handlers.
    // Note that the handler processing code RUNS IN THIS THREAD.
    // That means YOUR HANDLER CAN BLOCK THIS THREAD if it takes
    // very long to process. IT IS YOUR RESPONSIBILITY TO COPY THE
    // EVENT POINTER SOMEWHERE ELSE AND PROCESS IT IN A DIFFERENT THREAD
    // IF YOUR PROCESING TAKES VERY LONG.
    // Mmkay.
    while (!shutdown) {
      while(pending.size()) {
	std::shared_ptr<base_event> event;
	{
	  std::lock_guard<std::mutex> lock(pending_mutex);
	  event = pending.back();
	  pending.pop_back();
	}
	received(event);
      }
    }
  }

  void zmq_receiver::add(const std::string& listen_on) {
    std::shared_ptr<zmq_data> new_connection = std::make_shared<zmq_data>();
    new_connection->socket = std::make_shared<zmq::socket_t>(new_connection->context, zmq::socket_type::sub);
    new_connection->socket->set(zmq::sockopt::subscribe, "");
    new_connection->socket->bind(listen_on);
    connections.push_back(new_connection);

    // This is what pulls the messages off the network and adds them
    // to pending
    poller.add(*(new_connection->socket),
	       zmq::event_flags::pollin,
	       [this, new_connection](zmq::event_flags flags) {
	std::stringstream buffer;
	zmq::multipart_t multimsg;
	multimsg.recv(*(new_connection->socket));
	auto msgiter = multimsg.begin();
	// TODO: Everything past here feels a bit awkward
	// See if I can do this better.
	zmq::message_t& event_buffer = *msgiter++;
	buffer << event_buffer.to_string();
        std::shared_ptr<fr::events::base_event> event;
	{
	  cereal::BinaryInputArchive iarch(buffer);
	  iarch(event);
	}
	{
	  std::lock_guard<std::mutex> lock(pending_mutex);
	  pending.push_back(event);
	}
      });
  }
  
}
