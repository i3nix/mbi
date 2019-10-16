#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif // !_CRT_SECURE_NO_WARNINGS


#include <iostream>
#include <fstream>
#include <filesystem>
#include "YAMLParcer.h"
#include "MBIClass.h"
#include "RMQClient.h"

using namespace std;

constexpr const char* default_dir = "migrations";

void ProcessFile(const string& file, MBIClass& mbi)
{
	YAMLParcer p(file);
	list<string> s = p.GetFanout();
	map<string, list<string> > t = p.GetDirect();
	map<string, list<string> > q = p.GetQueues();

	for (list<string>::iterator it = s.begin(); it != s.end(); ++it)
	{
		mbi.AddFanout(*it);
	}
	for (map<string, list<string> >::iterator it = t.begin(); it != t.end(); ++it)
	{
		mbi.AddDirect(it->first);
		for (list<string>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
		{
			mbi.AddDirectBinding(it->first, *it2);
		}
	}
	for (map<string, list<string> >::iterator it = q.begin(); it != q.end(); ++it)
	{
		mbi.AddQueue(it->first);
		for (list<string>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
		{
			mbi.AddQueuesBinding(it->first, *it2);
		}
	}
}

void ProcessRMQ(const string& file, RMQClient &client)
{
	YAMLParcer p(file);
	list<string> s = p.GetFanout();
	map<string, list<string> > t = p.GetDirect();
	map<string, list<string> > q = p.GetQueues();

	for (list<string>::iterator it = s.begin(); it != s.end(); ++it)
	{
		client.ExchangeDeclare(it->c_str(), "fanout", false, true, false);
	}
	for (map<string, list<string> >::iterator it = t.begin(); it != t.end(); ++it)
	{
		client.ExchangeDeclare(it->first.c_str(), "direct", false, true, false);
		for (list<string>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
		{
			client.ExchangeDeclare(it2->c_str(), "topic", false, true, true);
			client.BindExchange(it->first.c_str(), it2->c_str());
		}
	}
	for (map<string, list<string> >::iterator it = q.begin(); it != q.end(); ++it)
	{
		client.QueueDeclare(it->first.c_str(), false, true);
		for (list<string>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
		{
			client.ExchangeDeclare(it2->c_str(), "topic", false, true, true);
			client.BindQueue(it->first.c_str(), it2->c_str());
		}
	}
}

int main(int argc, char** argv)
{
	try {
		if (argc == 1)
		{
			filesystem::path pth(default_dir);
			if (!filesystem::exists(pth))
			{
				cout << "migrations directory does not exist!" << endl;
				return -1;
			}
			set<string> files;
			for (auto& f : filesystem::directory_iterator(pth))
				files.insert(f.path().string());
			MBIClass mbi;
			for (set<string>::iterator it = files.begin(); it != files.end(); it++)
				ProcessFile(*it,mbi);
			ofstream fout;
			fout.open("final.yml");
			fout << mbi.MakeYAML();
			fout.close();

			RMQClient client;
			char* env = ::getenv("AMQP_URL");
			if (!env)
			{
				cout << "AMQP_URL environment variable not set!" << endl;
				return -1;
			}
			client.Connect(env);
			
			ProcessRMQ("final.yml", client);

			client.Disconnect();
		}
		else {
			char* env = ::getenv("AMQP_URL");
			if (!env)
			{
				cout << "AMQP_URL environment variable not set!" << endl;
				return -1;
			}
			RMQClient client;
			client.Connect(env);
			for (size_t t = 1; t < argc; t++)
			{
				filesystem::path pth(argv[t]);
				if (!filesystem::exists(pth))
				{
					cout << argv[t] << " not found!" << endl;
					return -1;
				}
				ProcessRMQ(argv[t], client);
			}
			client.Disconnect();
		}
	}
	catch (YAML::Exception e)
	{
		cout << "YAML Exception :" << e.msg << endl;
		return -1;
	}

	return 0;
}
