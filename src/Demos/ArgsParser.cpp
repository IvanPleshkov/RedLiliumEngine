#include "pch.h"

#include "ArgsParser.h"

using namespace RED_LILIUM_NAMESPACE;

ArgsParser::ArgsParser(int argc, char** argv)
{
	int index = 0;
	for (; index < argc; index++)
	{
		std::string p = argv[index];
		if (p.empty())
			continue;

		if (p[0] != '-')
			m_parameters.push_back(p);
		else
			break;
	}

	while (index < argc)
	{
		std::string key = argv[index];
		std::vector<std::string> options;
		index++;

		for (; index < argc; index++)
		{
			std::string option = argv[index];
			if (!option.empty() && option[0] == '-')
				break;

			options.push_back(option);
		}

		m_options.insert({ key, options });
	}
}


bool ArgsParser::Has(const std::string& key, int index) const
{
	if (m_options.find(key) == m_options.end())
		return false;

	const auto& option_params = m_options.at(key);
	return index < (int)option_params.size();
}
