#pragma once

#include <glm/glm.hpp>

namespace Velvet {

	namespace Primitives {

		struct QuadVertex
		{
			glm::vec3 Position;
			glm::vec2 TexCoord;
			glm::vec4 Color;
		};

		namespace Quad {

			const std::vector<glm::vec4> Vertices = {
				{ -0.5f, -0.5f, 0.0f, 1.0f },
				{ 0.5f, -0.5f, 0.0f, 1.0f },
				{ 0.5f,  0.5f, 0.0f, 1.0f },
				{ -0.5f,  0.5f, 0.0f, 1.0f }
			};

			const std::vector<int> Indices = {
				0, 1, 2, 2, 3, 0
			};

			const std::vector<glm::vec2> TextureCoords = {
				{ 0.0f, 0.0f },
				{ 1.0f, 0.0f },
				{ 1.0f, 1.0f },
				{ 0.0f, 1.0f }
			};

		}

	}

}