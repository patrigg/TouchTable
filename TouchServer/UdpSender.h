#pragma once
#include <vector>
#include <string>
#include <boost/asio.hpp>
#include "IReceiver.h"

class UdpSender : public ISender
{
	boost::asio::ip::udp::socket m_sock;
	boost::asio::ip::udp::endpoint m_destination;
	
public:
	UdpSender(boost::asio::io_service& io_service, const std::string& name, int port);
	~UdpSender();

	void send(const std::string& data) override;

	void destination(boost::asio::io_service& io_service, const std::string& name, int port);
};

