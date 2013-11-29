
#include "UdpSocket.h"
#include "Winsock2.h"

#include <logger.h>
#include <cassert>

#pragma comment(lib, "Ws2_32.lib")

UDP_Socket::UDP_Socket( int port )
{
	m_port = port;

	// Create UDP socket
	m_handle = socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (m_handle == -1)
		handleIOError ("socket");

	// Ask operating system to let us do broadcasts from socket
	int on = 1;
	if (setsockopt (m_handle, SOL_SOCKET,
		SO_BROADCAST, (char*) &on, sizeof (on)) == -1)
		handleIOError ("setsockopt (SO_BROADCAST)");

	// Allow multiple processes to bind to socket; incoming
	// messages will come to each process
	//if (setsockopt (m_handle, SOL_SOCKET,
	//	SO_REUSEADDR,(char*)  &on, sizeof (on)) == -1)
	//	handleIOError ("setsockopt (SO_REUSEADDR)");

	struct sockaddr_in sockaddr = { 0 };
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons (port);
	sockaddr.sin_addr.s_addr = htonl (INADDR_ANY);
	if (bind (m_handle, (SOCKADDR*) &sockaddr, sizeof (sockaddr)) == -1)
		handleIOError ("bind");

	//Set to non blocking
	u_long iMode=1;
	ioctlsocket(m_handle,FIONBIO,&iMode);


# if defined (__UNIX__)
	struct ifaddrs *interfaces;
	if (getifaddrs (&interfaces) == 0) {
		struct ifaddrs *interface = interfaces;
		while (interface) {
			// Hopefully the last interface will be WiFi
			if (interface->ifa_addr->sa_family == AF_INET) {
				self->address = *(struct sockaddr_in *) interface->ifa_addr;
				self->broadcast = *(struct sockaddr_in *) interface->ifa_broadaddr;
				self->broadcast.sin_port = htons (self->port_nbr);
			}
			interface = interface->ifa_next;
		}
	}
	freeifaddrs (interfaces);
# else
//# error "Interface detection TBD on this operating system"
# endif
}

UDP_Socket::~UDP_Socket()
{

}

int UDP_Socket::handle()
{
	return m_handle;
}


void UDP_Socket::handleIOError(const std::string& reason)
{
# ifdef WIN32
	switch (WSAGetLastError ()) 
	{
	case WSAEINTR: errno = EINTR; break;
	case WSAEBADF: errno = EBADF; break;
	case WSAEWOULDBLOCK: errno = EAGAIN; break;
	case WSAEINPROGRESS: errno = EAGAIN; break;
	case WSAENETDOWN: errno = ENETDOWN; break;
	case WSAECONNRESET: errno = ECONNRESET; break;
	case WSAECONNABORTED: errno = EPIPE; break;
	case WSAESHUTDOWN: errno = ECONNRESET; break;
	case WSAEINVAL: errno = EPIPE; break;
	default: errno = GetLastError ();
	}
# endif
	if (errno == EAGAIN
		|| errno == ENETDOWN
		|| errno == EPROTO
		|| errno == ENOPROTOOPT
		
# ifdef ENONET // Linux-specific
		|| errno == EHOSTDOWN
		|| errno == ENONET
# endif
		|| errno == EHOSTUNREACH
		|| errno == EOPNOTSUPP
		|| errno == ENETUNREACH
		|| errno == EWOULDBLOCK
		|| errno == EINTR)
		return; // Ignore error and try again
	else
		if (errno == EPIPE
			|| errno == ECONNRESET)
			return; // Ignore error and try again
		else {
			LOG_ERR() << "UDP " << strerror(errno) << " reason: " << reason << std::endl;
			assert (false);
		}
}

void UDP_Socket::send(char *buffer, size_t length)
{
	sockaddr_in target;
	target.sin_family = AF_INET;
	target.sin_port = htons(5000);
	target.sin_addr.s_addr = inet_addr ("255.255.255.255");
	if (sendto (m_handle, buffer, length, 0,
		(SOCKADDR*) &target, sizeof (struct sockaddr_in)) == -1)
		handleIOError("sendto");	
}

int UDP_Socket::recv(char *buffer, size_t length, char* sender)
{
	struct sockaddr_in sockaddr;
	int si_len = sizeof (struct sockaddr_in);

	int size = recvfrom (m_handle, buffer, length, 0, (SOCKADDR*) &sockaddr, (int *) &si_len);

	if (size == -1)
	{
		int err = WSAGetLastError();
		if (err != WSAEWOULDBLOCK)
			handleIOError("recvfrom");
		return size;
	} 
	//Check that the message is not just echoed to your self
	std::string temp = inet_ntoa(sockaddr.sin_addr);
	if (strcmp(temp.c_str(), getAddress()) == 0)
		return -1;

	LOG() << "UDP packet received from " << temp << std::endl;

	if (sender != NULL)
		strcpy(sender, temp.c_str());
	return size;
}

char* UDP_Socket::getAddress()
{
	char hn[80];
	gethostname(hn, sizeof(hn));
	struct hostent *phe = gethostbyname(hn);
	for (int i = 0; phe->h_addr_list[i] != 0; i++)
	{
		struct in_addr addr;
		memcpy(&addr, phe->h_addr_list[i],sizeof(struct in_addr));
		//LOG( "Address " + std::string(inet_ntoa(addr)));
		return inet_ntoa(addr);
	}
	return NULL;
}
