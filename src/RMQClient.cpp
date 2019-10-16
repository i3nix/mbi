#include "RMQClient.h"
#include <amqp_tcp_socket.h>
#include <iostream>
#include "../examples/utils.h"
#include <amqp_time.h>

RMQClient::RMQClient() : mSocket(NULL), mConnection(amqp_new_connection())
{}

RMQClient::~RMQClient() 
{}

void RMQClient::Connect(char* url)
{
	amqp_connection_info info;
	int x = amqp_parse_url(url, &info);
	if (x == AMQP_STATUS_BAD_URL)
	{
		std::cout << "Coul not parse URL" << std::endl;
		exit(1);
	}

	mSocket = amqp_tcp_socket_new(mConnection);
	
	if (!mSocket)
		die("Could not create TCP socket");
	std::cout << "TCP socket created" << std::endl;
	
	timeval timeout;
	timeout.tv_usec = 0;
	timeout.tv_sec = 5;

	if (amqp_socket_open_noblock(mSocket, info.host, info.port, &timeout))
		die("Could not open TCP socket");
	std::cout << "TCP socket opened" << std::endl;

	die_on_amqp_error(amqp_login(mConnection, info.vhost, 0, AMQP_DEFAULT_FRAME_SIZE, 0, AMQP_SASL_METHOD_PLAIN, info.user, info.password), "Could not log in");
	std::cout << "Logged in" << std::endl;

	amqp_channel_open(mConnection, 1);
	die_on_amqp_error(amqp_get_rpc_reply(mConnection), "Could not open the channel");
	std::cout << "Channel opened" << std::endl;
}

void RMQClient::Disconnect()
{
	die_on_amqp_error(amqp_channel_close(mConnection, 1, AMQP_REPLY_SUCCESS), "Could not close the channel");
	std::cout << "Channel closed" << std::endl;
	die_on_amqp_error(amqp_connection_close(mConnection, AMQP_REPLY_SUCCESS), "Could not close the connection");
	std::cout << "Connection closed" << std::endl;
	die_on_error(amqp_destroy_connection(mConnection), "Could not destroy the connection");
	std::cout << "Connection destroyed" << std::endl;
}

void RMQClient::ExchangeDeclare(const char* ex, const char* type, bool passive, bool durable, bool internal)
{
	amqp_exchange_declare(mConnection, 1, amqp_cstring_bytes(ex), amqp_cstring_bytes(type), passive, durable, false, internal, amqp_empty_table);
	std::string chk = "Could not declare exchange ";
	chk = chk + ex;
	die_on_amqp_error(amqp_get_rpc_reply(mConnection), chk.c_str());
	std::cout << "\t" << ex << " declared" << std::endl;
}

void RMQClient::QueueDeclare(const char* q, bool passive, bool durable)
{
	amqp_queue_declare(mConnection, 1, amqp_cstring_bytes(q), passive, durable, false, false, amqp_empty_table);
	std::string chk = "Could not declare queue ";
	chk += q;
	die_on_amqp_error(amqp_get_rpc_reply(mConnection), chk.c_str());
	std::cout << "\t" << q << " declared" << std::endl;
}

void RMQClient::BindExchange(const char* ex, const char* topic)
{
	amqp_exchange_bind(mConnection, 1, amqp_cstring_bytes(topic), amqp_cstring_bytes(ex), amqp_empty_bytes, amqp_empty_table);
	std::string chk = "Could not bind ";
	chk += ex;
	chk += " to ";
	chk += topic;
	die_on_amqp_error(amqp_get_rpc_reply(mConnection), chk.c_str());
	std::cout << "\t" << ex << " bound to " << topic << std::endl;
}

void RMQClient::BindQueue(const char* q, const char* topic)
{
	amqp_queue_bind(mConnection, 1, amqp_cstring_bytes(q), amqp_cstring_bytes(topic), amqp_empty_bytes, amqp_empty_table);
	std::string chk = "Could not bind ";
	chk += q;
	chk += " to ";
	chk += topic;
	die_on_amqp_error(amqp_get_rpc_reply(mConnection), chk.c_str());
	std::cout << "\t" << q << " bound to " << topic << std::endl;
}

void RMQClient::ExchangeDelete(const char* ex)
{
	amqp_exchange_delete(mConnection, 1, amqp_cstring_bytes(ex), false);
	std::string chk = "Could not delete ";
	chk += ex;
	die_on_amqp_error(amqp_get_rpc_reply(mConnection), chk.c_str());
	std::cout << "\t" << ex << " deleted" << std::endl;
}

void RMQClient::QueueDelete(const char* q)
{
	amqp_queue_delete(mConnection, 1, amqp_cstring_bytes(q), false, false);
	std::string chk = "Could not delete ";
	chk += q;
	die_on_amqp_error(amqp_get_rpc_reply(mConnection), chk.c_str());
	std::cout << "\t" << q << " deleted" << std::endl;
}
