#pragma once
#include <string>
#include <functional>
#include "targetver.h"
#include <boost/asio.hpp>
#include "IReceiver.h"

class UdpReceiver : public IReceiver
{
public:
	UdpReceiver(boost::asio::io_service& io_service, int port);
	~UdpReceiver();
private:
	boost::asio::io_service& io_service;
	boost::asio::ip::udp::socket m_sock;
	void doReceive();

	enum { max_length = 2048 };
	char buffer[max_length];
	boost::asio::ip::udp::endpoint sender;


	class Reply : public ISender
	{
		boost::asio::ip::udp::socket& m_sock;
		boost::asio::ip::udp::endpoint m_destination;
		Reply(boost::asio::ip::udp::socket& socket, boost::asio::ip::udp::endpoint destination);
	public:
		void send(const std::string& data) override;
		friend class UdpReceiver;
	};

	void run() override;

};

