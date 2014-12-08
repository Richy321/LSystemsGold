#pragma once

#include "../../octet.h"

namespace LSys
{
	class Node
	{
	public:

		octet::mat4t matrix;
		int vertexIndex;
		Node(octet::mat4t mat)
		{
			matrix = mat;
		}

		~Node()
		{
		}

		octet::vec3 GetPos()
		{
			return matrix[3].xyz();
		}
	};
}
