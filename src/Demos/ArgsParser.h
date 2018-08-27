#pragma once

#include <Core/Common.h>

namespace RED_LILIUM_NAMESPACE
{

class ArgsParser : public RedLiliumObject
{
public:
	ArgsParser(int argc, char** argv);
	~ArgsParser() override {}

	bool Has(const std::string& key, int index = -1) const;

	template<typename T>
	T Get(const std::string& key, int index = 0) const;

	template<>
	std::string Get<std::string>(const std::string& key, int index) const;

	template<>
	int Get<int>(const std::string& key, int index) const;

	template<>
	double Get<double>(const std::string& key, int index) const;

	template<typename T>
	T GetDefault(const std::string& key, T default_value) const;

private:
	std::unordered_map<std::string, std::vector<std::string>> m_options;

	std::vector<std::string> m_parameters;
};

template<typename T>
T ArgsParser::GetDefault(const std::string& key, T default_value) const
{
	if (!Has(key, 0))
		return default_value;

	return Get<T>(key);
}

template<>
std::string ArgsParser::Get<std::string>(const std::string& key, int index) const
{
	const auto& option_params = m_options.at(key);
	return option_params[index];
}

template<>
int ArgsParser::Get<int>(const std::string& key, int index) const
{
	const auto& option_params = m_options.at(key);
	return std::stoi(option_params[index]);
}

template<>
double ArgsParser::Get<double>(const std::string& key, int index) const
{
	const auto& option_params = m_options.at(key);
	return std::stod(option_params[index]);
}

} // namespace RED_LILIUM_NAMESPACE
