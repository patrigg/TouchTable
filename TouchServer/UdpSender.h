#pragma once
#include <vector>
#include <string>
#include <boost/asio.hpp>

class UdpSender
{
	boost::asio::io_service io_service;
	boost::asio::ip::udp::socket sock;
	boost::asio::ip::udp::endpoint destination;
	
public:
	UdpSender(const std::string& name, int port);
	~UdpSender();

	void send(const std::vector<char>& data);
	void send(const std::string& data);
};

