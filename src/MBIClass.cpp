#include "MBIClass.h"
#include <iostream>

MBIClass::MBIClass()
{}

MBIClass::~MBIClass()
{}

void MBIClass::AddQueue(std::string s)
{
	if (mQueues.find(s) != mQueues.end())
	{
		std::cout << "Warning: queue " << s << " already exists" << std::endl;
	}
	else
	{
		mQueues[s] = std::set<std::string>();
	}
}

void MBIClass::AddFanout(std::string s)
{
	if (mFanout.find(s) != mFanout.end())
	{
		std::cout << "Warning: fanout " << s << " already exists" << std::endl;
	}
	else
	{
		mFanout.insert(s);
	}
}

void MBIClass::AddDirect(std::string s)
{
	if (mDirect.find(s) != mDirect.end())
	{
		std::cout << "Warning: direct " << s << " already exists" << std::endl;
	}
	else
	{
		mDirect[s] = std::set<std::string>();
	}
}

void MBIClass::AddQueuesBinding(std::string s1, std::string s2)
{
	if (mQueues.find(s1) == mQueues.end())
	{
		std::cout << "Warning: queue " << s1 << " doesn\'t exist!" << std::endl;
	}
	else
	{
		if (mQueues[s1].find(s2) != mQueues[s1].end())
		{
			std::cout << "Warning: binding " << s1 << "--->" << s2 << " already exists" << std::endl;
		}
		else
		{
			mQueues[s1].insert(s2);
		}
	}
}

void MBIClass::AddDirectBinding(std::string s1, std::string s2)
{
	if (mDirect.find(s1) == mDirect.end())
	{
		std::cout << "Warning: direct " << s1 << " doesn\'t exist!" << std::endl;
	}
	else
	{
		if (mDirect[s1].find(s2) != mDirect[s1].end())
		{
			std::cout << "Warning: binding " << s1 << "--->" << s2 << " already exists" << std::endl;
		}
		else
		{
			mDirect[s1].insert(s2);
		}
	}
}

YAML::Node MBIClass::MakeYAML()
{
	YAML::Node n;
	for (std::map<std::string, std::set<std::string> >::iterator it = mQueues.begin(); it != mQueues.end(); ++it)
	{
		YAML::Node cur;
		YAML::Node seq = YAML::Load("[]");
		for (std::set<std::string>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2) 
		{
			seq.push_back(*it2);
		}
		cur[it->first] = seq;
		n["queues"].push_back(cur);
	}
	for (std::set<std::string>::iterator it = mFanout.begin(); it != mFanout.end(); ++it)
	{
		n["fanout"].push_back(*it);
	}
	YAML::Node Direct;
	for (std::map<std::string, std::set<std::string> >::iterator it = mDirect.begin(); it != mDirect.end(); ++it)
	{
		YAML::Node cur;
		YAML::Node seq = YAML::Load("[]");
		for (std::set<std::string>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
		{
			seq.push_back(*it2);
		}
		cur[it->first] = seq;
		n["direct"].push_back(cur);
	}
	return n;
}
