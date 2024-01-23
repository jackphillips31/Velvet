#pragma once

#include <glm/glm.hpp>

namespace Velvet {

	namespace Primitives {

		struct QuadVertex
		{
			glm::vec3 Position;
			glm::vec4 Color;
			glm::vec2 TexCoord;
			float TexIndex;
			float TilingFactor;
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