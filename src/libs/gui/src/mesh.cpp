#include <gui/mesh.h>

namespace crl {
	namespace gui {
		// this one
		Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
			std::map<TextureType, std::vector<Texture>> textures, std::vector<tinyobj::face_t> faces) :
			vertices(vertices), indices(indices), textures(textures), faces(faces) {
			setupMesh();
			std::cout << "vertices size: " << vertices.size() << std::endl;
			std::cout << "indices size: " << indices.size() << std::endl;
			for (int i = 0; i < faces.size(); i++) {
				faces[i].print();
			}
		}

		Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) :
			vertices(vertices), indices(indices) {
			setupMesh();
		}

		// second function
		void Mesh::draw(Shader shader, const V3D& color, float alpha) const {
			shader.setVec3("objectColor", toGLM(color));
			shader.setFloat("alpha", alpha);

			if (textures.find(DIFFUSE) != textures.end()) {
				shader.setBool("use_textures", true);
				// bind appropriate textures
				for (unsigned int i = 0; i < textures.at(DIFFUSE).size(); i++) {
					glActiveTexture(GL_TEXTURE0 + i);  // active proper texture unit before binding
					// retrieve texture number (the N in diffuse_textureN)
					const Texture& texture = textures.at(DIFFUSE)[i];
					// now set the sampler to the correct texture unit
					glUniform1i(glGetUniformLocation(shader.ID, ("texture_diffuse" + std::to_string(i + 1)).c_str()), i);
					// and finally bind the texture
					glBindTexture(GL_TEXTURE_2D, texture.id);
				}
			}
			else {
				// there are no textures... indicate as much...
				shader.setBool("use_textures", false);
			}

			// draw mesh
			glBindVertexArray(VAO);
			if (shader.getName().compare("silhouetteShader") == 0) {
				glDrawElements(GL_TRIANGLES_ADJACENCY, indices.size() * 6, GL_UNSIGNED_INT, nullptr);
			}
			else {
				glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, nullptr);
			}
			glBindVertexArray(0);

			// always good practice to set everything back to defaults once configured.
			glActiveTexture(GL_TEXTURE0);
		}

		// this is the function
		void Mesh::draw(Shader shader, float alpha) const {
			draw(shader, this->defaultColor, alpha);
		}

		void Mesh::reinitialize(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {
			glDeleteVertexArrays(1, &VAO);
			glDeleteBuffers(1, &VBO);
			glDeleteBuffers(1, &EBO);
			this->vertices = vertices;
			this->indices = indices;
			setupMesh();
		}

		void Mesh::setupMesh() {
			// TODO: should the arrays generated here be destroyed/deleted at some
			// point? If so, care must be taken with copy constructors/operators and
			// all that...

			// create buffers/arrays
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			glBindVertexArray(VAO);
			// load data into vertex buffers
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			// A great thing about structs is that their memory layout is sequential
			// for all its items. The effect is that we can simply pass a pointer to
			// the struct and it translates perfectly to a glm::vec3/2 array which
			// again translates to 3/2 floats which translates to a byte array.
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

			// set the vertex attribute pointers
			// vertex Positions
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)nullptr);
			// vertex normals
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
			// vertex texture coords
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

			glBindVertexArray(0);
		}

		void calculateAdjacency(const unsigned int index1, const unsigned int index2, const unsigned int index3) {

		}
	}  // namespace gui
}  // namespace crl