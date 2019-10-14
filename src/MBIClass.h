#pragma once
#include <map>
#include <set>
#include <list>
#include <string>
#include <yaml-cpp/yaml.h>

class MBIClass
{
public:
													MBIClass();
													~MBIClass();
	void											AddQueue(std::string s);
	void											AddFanout(std::string s);
	void											AddDirect(std::string s);
	void											AddQueuesBinding(std::string s1, std::string s2);
	void											AddDirectBinding(std::string s1, std::string s2);
	YAML::Node										MakeYAML();
private:
	std::map<std::string, std::set<std::string> >	mQueues;
	std::set<std::string>							mFanout;
	std::map<std::string, std::set<std::string> >	mDirect;
};
