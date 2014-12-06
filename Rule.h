#pragma once

namespace
{
	const static char equalsSymbol = '"';
}

namespace LSys
{
	class Rule
	{
		char* ruleString;
		bool isContextSensitive = false;

		const char* compare;
		const char* replace;
	public:

		Rule(const char * rule)
		{
			size_t size = strlen(rule) +1; //null terminator
			ruleString = new char[size];
			strcpy(ruleString, rule);
			bool afterEquals = false;

			compare = new char[100];
			replace = new char[1000];

			std::string tmp;

			for (int i = 0; i < size; i++)
			{
				if (rule[i] == equalsSymbol)
				{
					compare = tmp.c_str();
					tmp.clear();
					afterEquals = true;
				}
				else
				{
					tmp.push_back(rule[i]);
				}
			}
			replace = tmp.c_str();
		}

		bool GetIsContextSensitive() {	return isContextSensitive; }
		const char* GetRuleString() { return ruleString; }
		const char* GetCompare() { return compare; }
		const char* GetReplace() { return replace; }

		~Rule()
		{
			delete[] ruleString;
		}


		void ApplyRule(char * result, const char * axiom, const int axiomIndex)
		{


		}
	};
}

