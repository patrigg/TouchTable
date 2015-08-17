#pragma once
#include <string>
#include <functional>

class ISender
{
public:
	virtual void send(const std::string& data) = 0;
	virtual ~ISender()
	{
	}
};

class IReceiver
{
public:
	std::function<void(const std::string&, ISender&)> onReceive;
	virtual ~IReceiver()
	{
	}

	virtual void run() = 0;
};