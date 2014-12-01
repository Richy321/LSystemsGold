#pragma once
namespace LSys
{
	class Rule
	{
		char* ruleString;
	public:

		Rule(const char * rule)
		{
			size_t size = strlen(rule) +1;
			ruleString = new char[size];
			memcpy(ruleString, rule, sizeof(char) * size);
		}

		~Rule()
		{
			delete[] ruleString;
		}
	};
}

