/**
 \file	udpBroadcast.h

 \brief	Declares the UDP broadcast class.
 */
 

#include <zmq.h>
#include <string>

class UDP_Socket
{
public:
	UDP_Socket(int port);
	~UDP_Socket();

	int handle();
	void send(char *buffer, size_t length);

	/**
	 \fn	int UDP_Socket::recv(char *buffer, size_t length);
	
	 \brief	Performs a non synchronous read of the socket
	
	 \param [in,out]	buffer	If non-null, the buffer.
	 \param	length			  	The length.
	
	 \return	.
	 */

	int recv(char *buffer, size_t length, char* sender = NULL);

	char* getAddress();
private:
	void handleIOError(const std::string& reason);

	int m_handle; // Socket for send/recv
	int m_port; // UDP port number we work on
	struct sockaddr_in m_address; // Own address
	struct sockaddr_in m_broadcast; // Broadcast address
};