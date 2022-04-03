#define NOMINMAX
#include "mesh.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

std::map<std::string, Mesh*> Mesh::meshes;

// Helper functions
int indexOfNumberLetter(std::string& str, int offset);
int lastIndexOfNumberLetter(std::string& str);
std::vector<std::string> split(const std::string &s, char delim);

// Constructor - load mesh from file
Mesh::Mesh(std::string filename, bool keepLocalGeometry) {
	minBB = glm::vec3(std::numeric_limits<float>::max());
	maxBB = glm::vec3(std::numeric_limits<float>::lowest());

	vao = 0;
	vbuf = 0;
	vcount = 0;
	load(filename, keepLocalGeometry);
}

// Draw the mesh
void Mesh::draw() {
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, vcount);
	glBindVertexArray(0);
}

// Load a wavefront OBJ file
void Mesh::load(std::string filename, bool keepLocalGeometry) {
	// Release resources
	release();

	std::ifstream file(filename);
	if (!file.is_open()) {
		std::stringstream ss;
		ss << "Error reading " << filename << ": failed to open file";
		throw std::runtime_error(ss.str());
	}

	// Store vertex and normal data while reading
	std::vector<glm::vec3> raw_vertices;
	std::vector<glm::vec3> raw_normals;
	std::vector<unsigned int> v_elements;
	std::vector<unsigned int> n_elements;

	std::string line;
	while (getline(file, line)) {
		if (line.substr(0, 2) == "v ") {
			// Read position data
			int index1 = indexOfNumberLetter(line, 2);
			int index2 = lastIndexOfNumberLetter(line);
			std::vector<std::string> values = split(line.substr(index1, index2 - index1 + 1), ' ');
			glm::vec3 vert(stof(values[0]), stof(values[1]), stof(values[2]));
			raw_vertices.push_back(vert);

			// Update bounding box
			minBB = glm::min(minBB, vert);
			maxBB = glm::max(maxBB, vert);
		} else if (line.substr(0, 3) == "vn ") {
			// Read normal data
			int index1 = indexOfNumberLetter(line, 2);
			int index2 = lastIndexOfNumberLetter(line);
			std::vector<std::string> values = split(line.substr(index1, index2 - index1 + 1), ' ');
			raw_normals.push_back(glm::vec3(stof(values[0]), stof(values[1]), stof(values[2])));

		} else if (line.substr(0, 2) == "f ") {
			// Read face data
			int index1 = indexOfNumberLetter(line, 2);
			int index2 = lastIndexOfNumberLetter(line);
			std::vector<std::string> values = split(line.substr(index1, index2 - index1 + 1), ' ');
			for (int i = 0; i < int(values.size()) - 2; i++) {
				// Split up vertex indices
				std::vector<std::string> v1 = split(values[0], '/');		// Triangle fan for ngons
				std::vector<std::string> v2 = split(values[i+1], '/');
				std::vector<std::string> v3 = split(values[i+2], '/');

				// Store position indices
				v_elements.push_back(stoul(v1[0]) - 1);
				v_elements.push_back(stoul(v2[0]) - 1);
				v_elements.push_back(stoul(v3[0]) - 1);

				// Check for normals
				if (v1.size() >= 3 && v1[2].length() > 0) {
					n_elements.push_back(stoul(v1[2]) - 1);
					n_elements.push_back(stoul(v2[2]) - 1);
					n_elements.push_back(stoul(v3[2]) - 1);
				}
			}
		}
	}
	file.close();

	// Check if the file was invalid
	if (raw_vertices.empty() || v_elements.empty()) {
		std::stringstream ss;
		ss << "Error reading " << filename << ": invalid file or no geometry";
		throw std::runtime_error(ss.str());
	}

	std::vector<glm::vec3> face_normals(v_elements.size());
	std::vector<glm::vec3> accumulated_normals(raw_vertices.size(), glm::vec3(0.0f));
	for (int i = 0; i < int(v_elements.size()); i += 3) {

		// Flat Normal
		glm::vec3 normal = glm::normalize(glm::cross(raw_vertices[v_elements[i + 1]] - raw_vertices[v_elements[i + 0]], raw_vertices[v_elements[i + 2]] - raw_vertices[v_elements[i + 0]]));
		face_normals[i / 3] = normal;

		// Smooth Normal
		accumulated_normals[v_elements[i + 0]] += normal;
		accumulated_normals[v_elements[i + 1]] += normal;
		accumulated_normals[v_elements[i + 2]] += normal;

	}

	// Create vertex array
	vertices = std::vector<Vertex>(v_elements.size());
	for (int i = 0; i < int(v_elements.size()); i += 3) {
		// Store positions
		vertices[i+0].pos = raw_vertices[v_elements[i+0]];
		vertices[i+1].pos = raw_vertices[v_elements[i+1]];
		vertices[i+2].pos = raw_vertices[v_elements[i+2]];

		// Check for normals
		if (n_elements.size() > 0) {
			// Store normals
			vertices[i + 0].norm = raw_normals[n_elements[i+0]];
			vertices[i + 1].norm = raw_normals[n_elements[i+1]];
			vertices[i + 2].norm = raw_normals[n_elements[i+2]];
		} else {
			// Calculate normal
			vertices[i + 0].norm = glm::normalize(accumulated_normals[v_elements[i + 0]]);
			vertices[i + 1].norm = glm::normalize(accumulated_normals[v_elements[i + 1]]);
			vertices[i + 2].norm = glm::normalize(accumulated_normals[v_elements[i + 2]]);
		}

		// Check for uvs
		vertices[i + 0].uv = glm::vec2(raw_vertices[v_elements[i + 0]].x, raw_vertices[v_elements[i + 0]].z);
		vertices[i + 1].uv = glm::vec2(raw_vertices[v_elements[i + 1]].x, raw_vertices[v_elements[i + 1]].z);
		vertices[i + 2].uv = glm::vec2(raw_vertices[v_elements[i + 2]].x, raw_vertices[v_elements[i + 2]].z);

	}
	vcount = (GLsizei)vertices.size();

	// Load vertices into OpenGL
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbuf);
	glBindBuffer(GL_ARRAY_BUFFER, vbuf);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	// Vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
	// Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)sizeof(glm::vec3));
	// UVs
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(2 * sizeof(glm::vec3)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Delete local copy of geometry
	if (!keepLocalGeometry)
		vertices.clear();

}

/// <summary>
/// Updates this mesh's vertices, faces and normals
/// </summary>
/// <param name="raw_vertices">The vertices for the mesh</param>
/// <param name="raw_triangles">The triangles for the mesh</param>
/// <param name="keepLocalGeometry">Preserve the mesh data in memory locally</param>
void Mesh::updateMesh(std::vector<glm::vec3> raw_vertices, std::vector<int> raw_triangles, std::vector<glm::vec2> raw_uvs, bool keepLocalGeometry) {

	release();

	std::vector<unsigned int> v_elements;
	
	// Construct the v_elements for faces
	for (int index = 0; index < raw_triangles.size(); index += 3) {

		v_elements.push_back(raw_triangles[index + 0]);
		v_elements.push_back(raw_triangles[index + 1]);
		v_elements.push_back(raw_triangles[index + 2]);

	}

	// Check if the data is invalid
	if (raw_vertices.empty() || v_elements.empty()) {
		std::stringstream ss;
		ss << "Error compiling mesh! invalid or no geometry detected!";
		throw std::runtime_error(ss.str());
	}

	std::vector<glm::vec3> face_normals(v_elements.size());
	std::vector<glm::vec3> accumulated_normals(raw_vertices.size(), glm::vec3(0.0f));
	for (int i = 0; i < int(v_elements.size()); i += 3) {

		// Flat Normal
		glm::vec3 normal = glm::normalize(glm::cross(raw_vertices[v_elements[i + 1]] - raw_vertices[v_elements[i + 0]], raw_vertices[v_elements[i + 2]] - raw_vertices[v_elements[i + 0]]));
		face_normals[i / 3] = normal;

		// Smooth Normal
		accumulated_normals[v_elements[i + 0]] += normal;
		accumulated_normals[v_elements[i + 1]] += normal;
		accumulated_normals[v_elements[i + 2]] += normal;

	}

	// Create vertex array
	vertices = std::vector<Vertex>(v_elements.size());
	for (int i = 0; i < int(v_elements.size()); i += 3) {
		// Store positions
		vertices[i + 0].pos = raw_vertices[v_elements[i + 0]];
		vertices[i + 1].pos = raw_vertices[v_elements[i + 1]];
		vertices[i + 2].pos = raw_vertices[v_elements[i + 2]];

		// Calculated normal
		vertices[i + 0].norm = glm::normalize(accumulated_normals[v_elements[i + 0]]);
		vertices[i + 1].norm = glm::normalize(accumulated_normals[v_elements[i + 1]]);
		vertices[i + 2].norm = glm::normalize(accumulated_normals[v_elements[i + 2]]);

		
		if (raw_uvs.size() > 0) {
			vertices[i + 0].uv = raw_uvs[v_elements[i + 0]];
			vertices[i + 1].uv = raw_uvs[v_elements[i + 1]];
			vertices[i + 2].uv = raw_uvs[v_elements[i + 2]];
		} 
		else {
			// Calculate uvs
			vertices[i + 0].uv = glm::vec2(raw_vertices[v_elements[i + 0]].x, raw_vertices[v_elements[i + 0]].z);
			vertices[i + 1].uv = glm::vec2(raw_vertices[v_elements[i + 1]].x, raw_vertices[v_elements[i + 1]].z);
			vertices[i + 2].uv = glm::vec2(raw_vertices[v_elements[i + 2]].x, raw_vertices[v_elements[i + 2]].z);
		}
		
	}
	vcount = (GLsizei)vertices.size();

	

	// Update the buffer with the new mesh data
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbuf);
	glBindBuffer(GL_ARRAY_BUFFER, vbuf);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	// Vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
	// Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)sizeof(glm::vec3));
	// UVs
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(2 * sizeof(glm::vec3)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Delete local copy of geometry
	if (!keepLocalGeometry)
		vertices.clear();

}

Mesh* Mesh::getMesh(std::string filename, bool keepLocalGeometry)
{
	
	if (meshes.count(filename) > 0) {
		return meshes.at(filename);
	}
	return new Mesh(filename, keepLocalGeometry);

}

// Release resources
void Mesh::release() {
	minBB = glm::vec3(std::numeric_limits<float>::max());
	maxBB = glm::vec3(std::numeric_limits<float>::lowest());

	vertices.clear();
	if (vao) { glDeleteVertexArrays(1, &vao); vao = 0; }
	if (vbuf) { glDeleteBuffers(1, &vbuf); vbuf = 0; }
	vcount = 0;
}

int indexOfNumberLetter(std::string& str, int offset) {
	for (int i = offset; i < int(str.length()); ++i) {
		if ((str[i] >= '0' && str[i] <= '9') || str[i] == '-' || str[i] == '.') return i;
	}
	return (int)str.length();
}
int lastIndexOfNumberLetter(std::string& str) {
	for (int i = int(str.length()) - 1; i >= 0; --i) {
		if ((str[i] >= '0' && str[i] <= '9') || str[i] == '-' || str[i] == '.') return i;
	}
	return 0;
}
std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;

	std::stringstream ss(s);
    std::string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }

    return elems;
}
