#include "UdpSender.h"


using boost::asio::ip::udp;

UdpSender::UdpSender(const std::string& name, int port)
	: sock(io_service, udp::endpoint(udp::v4(), port+1))
{
	udp::resolver resolver(io_service);
	destination = *resolver.resolve({ udp::v4(), name, std::to_string(port) });
}


UdpSender::~UdpSender()
{
}

void UdpSender::send(const std::vector<char>& data)
{
	
	sock.send_to(boost::asio::buffer(data), destination);
}


void UdpSender::send(const std::string& data)
{

	sock.send_to(boost::asio::buffer(data), destination);
}