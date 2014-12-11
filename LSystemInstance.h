#pragma once
#include "../../octet.h"

namespace
{
	static const char *angleKeyword = "angle";
	static const char *axiomKeyword = "axiom";
	static const char *ruleKeyword = "rule";
	static const char *iterationsKeyword = "iterations";
	static const char valueDelimiter = ':';
	static const char equalsSymbol = '=';
}

#define SIZEOF_ARRAY( a ) (sizeof( a ) / sizeof( a[ 0 ] )) 

namespace LSys
{
	class LSystemInstance
	{
	private:
		struct cmp_str
		{
			bool operator()(char const *a, char const *b)
			{
				return strcmp(a, b) < 0;
			}
		};
		std::map<const char*, const char *, cmp_str>ruleMap;
	public:
		class LSystemState
		{
		private:
			int iterationNumber;
			char *result;
		public:

			LSystemState(int iterationNumber, const char *res)
			{
				this->iterationNumber = iterationNumber;
				size_t size = strlen(res) + 1;
				result = new char[size];
				memset(result, 0, sizeof(result));
				strcpy(result, res);
			}

			~LSystemState()
			{
				delete[] result;
			}

			const int getIterationNumber() const { return iterationNumber; }
			const char* getResult() const  { return result; }
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
		float distance = 0.1f;
		octet::vec3 colour = octet::vec3(0.0f, 1.0f, 0.0f);

		char* axiom;
		const char* filePath;

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
							char* rule = (char*)malloc(sizeof(char) * size);
							memcpy(rule, tmpValueBuffer, size);

							//ruleMap
							AddRuleToDictionary(rule);
							free(rule);
						}
					}
				}
			}
		}

		void AddRuleToDictionary(char * rawRule)
		{
			char* compare;
			char* replace;

			size_t size = strlen(rawRule); //null terminator
			bool afterEquals = false;

			compare = new char[100];
			memset(compare, 0, sizeof(compare));
			replace = new char[1000];
			memset(replace, 0, sizeof(replace));

			for (int i = 0; i < size; i++)
			{
				if (rawRule[i] == equalsSymbol)
				{
					afterEquals = true;
				}
				else
				{
					if (!afterEquals)
					{
						strncat(compare, &rawRule[i], 1);
					}
					else
					{
						strncat(replace, &rawRule[i], 1);
					}
				}
			}

			//add to map
			ruleMap[compare] = replace;
		}

		void Run()
		{
			octet::dynarray<char> curAxiom;
			octet::dynarray<char> result;

			for (int i = 0; i < strlen(axiom); i++)
			{
				curAxiom.push_back(axiom[i]);
			}

			for (int i = 0; i < iterations; i++)
			{
				result.reset();

				//loop through each char
				for (unsigned int j = 0; j < curAxiom.size(); j++)
				{
					//grab current letter with null term
					char * letter = new char[2];
					letter[0] = curAxiom[j];
					letter[1] = '\0';

					//Context sensitive = Get max length of rule keys
						//loop backwards from j+MaxLength to j
							//find axoim[j] to axiom[j+ctr] in rules, then replace (i.e. longest takes priority?)
								//if found, increment j by ctr to skip over replaced letters.

					auto matchingRule = ruleMap.find(letter);
					if (matchingRule != ruleMap.end())
					{
						const char* replace = matchingRule->second;
						for (int k = 0; k < strlen(replace); k++)
						{
							result.push_back(replace[k]);
						}
					}
					else
					{
						result.push_back(curAxiom[j]);
					}

					delete[] letter;
				}

				char *c_result = new char[result.size() + 1];
				for (unsigned int k = 0; k < result.size(); k++)
				{
					c_result[k] = result[k];
				}
				c_result[result.size()] = '\0';

				LSystemState *state = new LSystemState(i, c_result);
				states.push_back(state);

				delete[] c_result;

				curAxiom.reset();
				for (unsigned int k = 0; k < result.size(); k++)
				{
					curAxiom.push_back(result[k]);
				}
			}
		}

		const LSystemState* GetCurrentState() const { return states[currentStateIndex]; }
		
		float GetAngle() const { return angle; }
	};
}