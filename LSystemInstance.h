#pragma once
#include "../../octet.h"
#include "Rule.h"

#define SIZEOF_ARRAY( a ) (sizeof( a ) / sizeof( a[ 0 ] )) 

namespace
{
	static const char *angleKeyword = "angle";
	static const char *axiomKeyword = "axiom";
	static const char *ruleKeyword = "rule";
	static const char *iterationsKeyword = "iterations";
	static const char valueDelimiter = ':';
}

namespace LSys
{
	class LSystemInstance
	{
	private:

	public:

		class LSystemState
		{
			int iterationNumber;
			char *result;

		public:

			LSystemState(int iterationNumber, char *res)
			{
				this->iterationNumber = iterationNumber;
				size_t size = strlen(res) + 1;
				result = new char[size];
				strcpy(result, res);
				result[size - 1] = NULL;
			}

			~LSystemState()
			{
				delete[] result;
			}
		};

		LSystemInstance() = default;
		LSystemInstance(const char * filePath)
		{
			this->filePath = filePath;
			Load(filePath);
		}
		~LSystemInstance() 
		{
			delete axiom;
		}

		int iterations;
		float angle;
		float distance = 1.0f;

		char* axiom;
		const char* filePath;

		//octet::dictionary<char*, char*>

		octet::dynarray<Rule*> rules;
		octet::dynarray<char> currentSystem;
		octet::dynarray<LSystemState*> states;

		int currentStateIndex = 0;

		void Load(const char* filename)
		{
			octet::dynarray<unsigned char> fileBuffer;
			octet::app_utils::get_url(fileBuffer, filename);

			char tmpBuffer[100] = { 0 };
			char *tmpPos = &tmpBuffer[0];
			
			for each (unsigned char c in fileBuffer)
			{
				if (c == '\n')
				{
					//reset tmpBuffer
					memset(tmpBuffer, 0, sizeof(tmpBuffer));
					tmpPos = &tmpBuffer[0];
				}
				else if (c != '\r')
				{
					*tmpPos = c;
					tmpPos++;
				}
				else
				{
					ptrdiff_t size = tmpPos - &tmpBuffer[0];

					char* inst = strstr(tmpBuffer, iterationsKeyword);
					if (inst != NULL && inst + strlen(iterationsKeyword) != tmpPos)
					{
						inst += strlen(iterationsKeyword);

						if (*inst == valueDelimiter)
						{
							inst++;
							iterations = atoi(inst);
						}
					}

					inst = strstr(tmpBuffer, angleKeyword);
					if (inst != NULL && inst + strlen(angleKeyword) != tmpPos)
					{
						inst += strlen(axiomKeyword);

						if (*inst == valueDelimiter)
						{
							inst++;
							char tmpValueBuffer[100];
							memset(tmpValueBuffer, 0, sizeof(tmpValueBuffer));

							char* valuePtr = tmpValueBuffer;
							while (inst != tmpPos)
							{
								*valuePtr = *inst;
								valuePtr++;
								inst++;
							}
							
							ptrdiff_t size = valuePtr - tmpValueBuffer;
							angle = (float)atof(tmpValueBuffer);
						}
					}

					inst = strstr(tmpBuffer, axiomKeyword);
					if (inst != NULL && inst + strlen(axiomKeyword) != tmpPos)
					{
						inst += strlen(axiomKeyword);

						if (*inst == valueDelimiter)
						{
							inst++;
							char tmpValueBuffer[100];
							memset(tmpValueBuffer, 0, sizeof(tmpValueBuffer));
							char* valuePtr = tmpValueBuffer;
							while (inst != tmpPos)
							{
								*valuePtr = *inst;
								valuePtr++;
								inst++;
							}
							size_t size = valuePtr - tmpValueBuffer +1;

							axiom = new char[size];
							strcpy(axiom, tmpValueBuffer);
							//memcpy(axiom, tmpValueBuffer, size);
						}
					}

					inst = strstr(tmpBuffer, ruleKeyword);
					if (inst != NULL && inst + strlen(ruleKeyword) != tmpPos)
					{
						inst += strlen(ruleKeyword);

						if (*inst == valueDelimiter)
						{
							inst++;
							char tmpValueBuffer[100];
							memset(tmpValueBuffer, 0, sizeof(tmpValueBuffer));
							char* valuePtr = tmpValueBuffer;
							while (inst != tmpPos)
							{
								*valuePtr = *inst;
								valuePtr++;
								inst++;
							}
							size_t size = valuePtr - tmpValueBuffer + 1; //null terminator
							char* rule = new char[size];
							memcpy(rule, tmpValueBuffer, size);

							Rule *newRule = new Rule(rule);
							rules.push_back(newRule);

							delete[] rule;
						}
					}
				}
			}
		}

		void Run()
		{
			octet::dynarray<char> result;

			for (int i = 0; i < strlen(axiom); i++)
			{
				result.push_back(axiom[i]);
			}

			for (int i = 0; i < iterations; i++)
			{
				//loop through each char
				//apply rules

				for (unsigned int j = 0; j < result.size(); j++)
				{
					//check if 


					for each(Rule *rule in rules)
					{

						//curItResult
					}
				}

				//LSystemState *state = new LSystemState(i, result);
			}
		}

		LSystemState* GetCurrentState() { return states[currentStateIndex]; }
	};
}