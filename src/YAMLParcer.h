#pragma once
#include <yaml-cpp/yaml.h>

class YAMLParcer
{
public:
													YAMLParcer(const std::string &FileName);
													~YAMLParcer();
	std::list<std::string>							GetFanout();
	std::map<std::string, std::list<std::string> >	GetDirect();
	std::map<std::string, std::list<std::string> >	GetQueues();
private:
	YAML::Node		mConfig;
};
