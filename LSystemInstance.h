#pragma once
#include "../../octet.h"
#include "Rule.h"

namespace
{
	static const char *angleKeyword = "angle";
	static const char *axiomKeyword = "axiom";
	static const char *ruleKeyword = "rule";
	static const char *iterationsKeyword = "iterations";
	static const char valueDelimiter = ':';
}

#define SIZEOF_ARRAY( a ) (sizeof( a ) / sizeof( a[ 0 ] )) 

namespace LSys
{
	class LSystemInstance
	{
	private:
		octet::dictionary<const char*, const char*> ruleMap;
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
				strcpy_s(result, sizeof(res), res);
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

							//ruleMap
							AddRuleToDictionary(rule);
							delete[] rule;
						}
					}
				}
			}
		}

		void AddRuleToDictionary(char * rawRule)
		{
			const char* compare;
			const char* replace;

			size_t size = strlen(rawRule) + 1; //null terminator
			bool afterEquals = false;

			compare = new char[100];
			replace = new char[1000];

			std::string tmp;

			for (int i = 0; i < size; i++)
			{
				if (rawRule[i] == equalsSymbol)
				{
					compare = tmp.c_str();
					tmp.clear();
					afterEquals = true;
				}
				else
				{
					tmp.push_back(rawRule[i]);
				}
			}
			replace = tmp.substr;

			//add to map
			ruleMap[compare] = replace;
		}

		void Run()
		{
			size_t axiomSize = strlen(axiom);
			
			size_t resultSize = axiomSize;
			char* result = new char[resultSize];
			char* resultPtr = result;

			for (int i = 0; i < iterations; i++)
			{
				//loop through each char
				for (unsigned int j = 0; j < axiomSize; j++)
				{
					if (ruleMap.contains(&axiom[j]))
					{
						const char* replace = ruleMap[&axiom[j]];

						if (SIZEOF_ARRAY(result) < strlen(result) + strlen(replace) +1) //+1 for null term
						{
							Grow(result);
						}

						strcpy_s(resultPtr, sizeof(replace), replace);
						resultPtr += SIZEOF_ARRAY(replace);
					}
				}

				

				LSystemState *state = new LSystemState(i, result);
				states.push_back(state);



				Resize(axiom, strlen(result)+1);
				strcpy(axiom, result);
			}
		}

		LSystemState* GetCurrentState() { return states[currentStateIndex]; }
	};


	//Grow char array with doubling
	void Grow(char *arr)
	{
		Resize(arr, SIZEOF_ARRAY(arr) * 2);
	}

	void Resize(char * arr, int size)
	{
		char *newResult = new char[size];
		memcpy(newResult, arr, sizeof(arr));
		delete[] arr;
		arr = newResult;
	}
}