#include "stdafx.h"
#include <string>
#include "NetworkHandler.h"


NetworkHandler::NetworkHandler(void)
	:m_subscriber(NULL)
{
	m_context = new zmq::context_t(1);
	m_publisher = new zmq::socket_t(*m_context,ZMQ_PUB);

	m_publisher->bind("tcp://*:5556");
}


NetworkHandler::~NetworkHandler(void)
{
	m_publisher->close();
	m_context->close();

	delete m_publisher;
	delete m_context;
}


void NetworkHandler::sendData(Data data)
{
	m_publisher->send(data.getData().get(), data.getSize());
}
