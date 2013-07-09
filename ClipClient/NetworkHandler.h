#pragma once

#include <string>
#include <zmq.hpp>

#include "Data.h"


class NetworkHandler
{
public:
	NetworkHandler(void);
	~NetworkHandler(void);

	void sendData(Data data);
	

private:
	zmq::context_t* m_context;
	zmq::socket_t* m_publisher;
	zmq::socket_t* m_subscriber;
};

