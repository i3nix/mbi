#include "YAMLParcer.h"

#include <iostream>

YAMLParcer::YAMLParcer(const std::string& FileName)
{
	mConfig = YAML::LoadFile(FileName);
}

YAMLParcer::~YAMLParcer()
{
	
}

std::list<std::string> YAMLParcer::GetFanout()
{
	std::list<std::string> Exchanges;

	if (mConfig["fanout"])
	{
		for (YAML::const_iterator it = mConfig["fanout"].begin(); it != mConfig["fanout"].end(); ++it)
		{
			Exchanges.push_back(it->as<std::string>());
		}
	}

	return Exchanges;
}

std::map<std::string, std::list<std::string> > YAMLParcer::GetDirect()
{
	std::map<std::string, std::list<std::string> >	Topics;
	if (mConfig["direct"])
	{
		YAML::Node x = mConfig["direct"];
		for (YAML::const_iterator it = x.begin(); it != x.end(); ++it)
		{
			YAML::Node y = *(it);
			for (YAML::const_iterator it2 = y.begin(); it2 != y.end(); ++it2)
			{
				std::string s = it2->first.as<std::string>();
				for (YAML::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
					Topics[s].push_back(it3->as<std::string>());
			}
		}
	}

	return Topics;
}

std::map<std::string, std::list<std::string> > YAMLParcer::GetQueues()
{
	std::map<std::string, std::list<std::string> >	Queues;
	if (mConfig["queues"])
	{
		YAML::Node x = mConfig["queues"];
		for (YAML::const_iterator it = x.begin(); it != x.end(); ++it)
		{
			YAML::Node y = *(it);
			for (YAML::const_iterator it2 = y.begin(); it2 != y.end(); ++it2)
			{
				std::string s = it2->first.as<std::string>();
				if (it2->second.size() == 0)
					Queues[s] = std::list<std::string>();
				else
					for (YAML::const_iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
						Queues[s].push_back(it3->as<std::string>());
			}
		}
	}

	return Queues;
}
