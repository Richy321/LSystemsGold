#pragma once

#include "../../octet.h"

namespace LSys
{
	class Node
	{
	public:

		octet::mat4t matrix;
		Node(octet::mat4t mat)
		{
			matrix = mat;
		}

		~Node()
		{
		}
	};
}
