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
		void ApplyRule(const Rule &rule)
		{


		}
	public:

		LSystemInstance() = default;
		LSystemInstance(const char * filePath)
		{
			this->filePath = filePath;
			Load(filePath);

		}
		~LSystemInstance() {}

		int iterations;
		float angle;
		float distance = 1.0f;

		char* axiom;
		const char* filePath;


		
		octet::dynarray<Rule*> rules;
		octet::dynarray<char> currentSystem;

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

							axiom = (char*)malloc(size);
							memcpy(axiom, tmpValueBuffer, size);
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
							size_t size = valuePtr - tmpValueBuffer + 1;
							char* rule = (char*)malloc(size);
							memcpy(rule, tmpValueBuffer, size);

							Rule *newRule = new Rule(rule);
							rules.push_back(newRule);
						}
					}
				}
			}
		}
	};
}