#include "UdpReceiver.h"

using boost::asio::ip::udp;

UdpReceiver::UdpReceiver(boost::asio::io_service& io_service, int port)
	: io_service(io_service), m_sock(io_service, udp::endpoint(udp::v4(), port))
{
	doReceive();
}


UdpReceiver::~UdpReceiver()
{
}

void UdpReceiver::doReceive()
{
	m_sock.async_receive_from(
		boost::asio::buffer(buffer, max_length), sender,
		[this](boost::system::error_code ec, std::size_t bytes_recvd)
	{
		Reply reply(m_sock, sender);
		onReceive(std::string(buffer, buffer + bytes_recvd), reply);
		
		doReceive();
	});
}

UdpReceiver::Reply::Reply(boost::asio::ip::udp::socket& socket, udp::endpoint destination)
	: m_sock(socket), m_destination(destination)
{
}

void UdpReceiver::Reply::send(const std::string& data)
{
	m_sock.send_to(
		boost::asio::buffer(data), m_destination);
}

void UdpReceiver::run()
{
	io_service.run();
}