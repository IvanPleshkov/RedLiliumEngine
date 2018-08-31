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

	template<typename T>
	T GetDefault(const std::string& key, T default_value) const;

private:
    using OptionsMap = std::unordered_map<std::string, std::vector<std::string>>;

    OptionsMap m_options;
	std::vector<std::string> m_parameters;
};

namespace ArgsParserDetails
{

template<typename T>
T Get(const std::string& parameter)
{
    throw std::exception();
}

/*
template<>
std::string Get<std::string>(const std::string& parameter)
{
    return parameter;
}

template<>
int Get<int>(const std::string& parameter)
{
    return std::stoi(parameter);
}

template<>
double Get<double>(const std::string& parameter)
{
    return std::stod(parameter);
}
*/
}

template<typename T>
T ArgsParser::GetDefault(const std::string& key, T default_value) const
{
    if (!Has(key, 0))
        return default_value;

    return Get<T>(key);
}

template<typename T>
T ArgsParser::Get(const std::string& key, int index) const
{
    RED_LILIUM_ASSERT(m_options.count(key) > 0);
    const auto& optionParams = m_options.at(key);
    RED_LILIUM_ASSERT(optionParams.size() > index);
    return ArgsParserDetails::Get<T>(optionParams[index]);
}

} // namespace RED_LILIUM_NAMESPACE
