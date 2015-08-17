#include "UdpSender.h"


using boost::asio::ip::udp;

UdpSender::UdpSender(boost::asio::io_service& io_service, const std::string& name, int port)
	: m_sock(io_service, udp::endpoint(udp::v4(), port+1))
{
	destination(io_service, name, port);
}


UdpSender::~UdpSender()
{
}

void UdpSender::send(const std::string& data)
{
	m_sock.send_to(boost::asio::buffer(data), m_destination);
}

void UdpSender::destination(boost::asio::io_service& io_service, const std::string& name, int port)
{
	udp::resolver resolver(io_service);
	m_destination = *resolver.resolve({ udp::v4(), name, std::to_string(port) });
}