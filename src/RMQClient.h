#pragma once
#include <amqp.h>

class RMQClient
{
public:
							RMQClient();
							~RMQClient();
	void					Connect(char* url);
	void					Disconnect();
	void					ExchangeDeclare(const char* ex, const char* type, bool passive, bool durable, bool internal);
	void					QueueDeclare(const char* q, bool passive, bool durable);
	void					BindExchange(const char* ex, const char* topic);
	void					BindQueue(const char* q, const char* topic);
	void					ExchangeDelete(const char* ex);
	void					QueueDelete(const char* q);
private:
	amqp_socket_t*			mSocket;
	amqp_connection_state_t	mConnection;
};
