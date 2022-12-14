#pragma once

#include <gui/guiMath.h>
#include <gui/shader.h>
#include <tiny_obj_loader.h>

// possible loss of data in conversion between double and float
#pragma warning(disable : 4244)
// deprecated/unsafe functions such as fopen
#pragma warning(disable : 4996)

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <map>
#include <vector>

namespace crl {
	namespace gui {
		struct Vertex {
			glm::vec3 position = glm::vec3(0, 0, 0);
			glm::vec3 normal = glm::vec3(0, 0, 0);
			glm::vec2 texCoords = glm::vec2(0, 0);

			bool operator==(const Vertex& other) const {
				return position == other.position && normal == other.normal && texCoords == other.texCoords;
			}
		};

		struct Texture {
			unsigned int id;
			std::string path;
		};

		struct Triangle {
			glm::vec3 point_a, point_b, point_c;
			unsigned int index_a, index_ab, index_b, index_bc, index_c, index_ca;
			Triangle* connection_ab, * connection_bc, * connection_ca;
		};

		class Mesh {
		public:
			enum TextureType { DIFFUSE, SPECULAR, NORMAL, AMBIENT };
			typedef std::map<TextureType, std::vector<Texture>> TextureMap;

			std::vector<Vertex> vertices;
			std::vector<unsigned int> indices;
			TextureMap textures;
			std::vector<tinyobj::face_t> faces;
			std::vector<unsigned int> adjacencyVertices;

			/* the color of the mesh
			 * original (0.9, 0.9, 0.9)
			 * (0.9, 0.9, 1.0)
			 * (1.0, 0.753, 0.796) */
			V3D defaultColor = V3D(1.0, 0.753, 0.796);
		private:
			unsigned int VAO, VBO, EBO;
		public:
			Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
				std::map<TextureType, std::vector<Texture>> textures, std::vector<tinyobj::face_t> faces);

			Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

			/**
			 * render the mesh using shader and specified color
			 */
			void draw(Shader shader, const V3D& color, float alpha = 1.0) const;

			/**
			 * render the mesh using shader and default color
			 */
			void draw(Shader shader, float alpha = 1.0) const;

			void reinitialize(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

			bool connects(glm::vec3 point_a, glm::vec3 point_b);
			void calculateAdjacency();
		private:
			// initializes all the buffer objects/arrays
			void setupMesh();
		};
	}  // namespace gui
}  // namespace crl