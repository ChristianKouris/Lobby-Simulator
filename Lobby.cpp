#include "Lobby.h"

Lobby::Lobby()
{
	/* 
	 * TODO: Section 2: Currently, all the points are hard coded below. 
	 * Modify this to read points from an obj file.
	 */
	std::ifstream objFile("amongus_lobby.obj"); // The obj file we are reading.

	// Check whether the file can be opened.
	if (objFile.is_open())
	{
		std::string line; // A line in the file.
		// Read lines from the file.
		while (std::getline(objFile, line))
		{
			// Turn the line into a string stream for processing.
			std::stringstream ss;
			ss << line;

			// Read the first word of the line.
			std::string label;
			ss >> label;

			// If the line is about vertex (starting with a "v").
			if (label == "v")
			{
				// Read the later three float numbers and use them as the 
				// coordinates.
				glm::vec3 point;
				ss >> point.x >> point.y >> point.z;

				// Process the point. For example, you can save it to a.
				temp_verts.push_back(point);
				
			}
			else if (label == "vn")
			{
				// Read the later three float numbers and use them as the 
				// coordinates.
				glm::vec3 point;
				ss >> point.x >> point.y >> point.z;

				// Process the point. For example, you can save it to a.
				temp_norms.push_back(point);

			}
			else if (label == "vt")
			{
				// Read the later three float numbers and use them as the 
				// coordinates.
				glm::vec2 point;
				ss >> point.x >> point.y;

				// Process the point. For example, you can save it to a.
				temp_texts.push_back(point);

			}
			else if (label == "f")
			{
				// Read the indices.
				std::string v1, v2, v3;
				
				ss >> v1 >> v2 >> v3;
				//the vertex is the number before the first delim
				vert_inds.push_back(std::stoul(v1.substr(0, v1.find_first_of("/"))) - 1);
				vert_inds.push_back(std::stoul(v2.substr(0, v2.find_first_of("/"))) - 1);
				vert_inds.push_back(std::stoul(v3.substr(0, v3.find_first_of("/"))) - 1);

				//norms after the second delim
				norm_inds.push_back(std::stoul(v1.substr(v1.find_last_of("/") + 1)) - 1);
				norm_inds.push_back(std::stoul(v2.substr(v2.find_last_of("/") + 1)) - 1);
				norm_inds.push_back(std::stoul(v3.substr(v3.find_last_of("/") + 1)) - 1);

				//texts in between the 2 delims
				text_inds.push_back(std::stoul(v1.substr(v1.find_first_of("/") + 1, v1.find_last_of("/"))) - 1);
				text_inds.push_back(std::stoul(v2.substr(v2.find_first_of("/") + 1, v2.find_last_of("/"))) - 1);
				text_inds.push_back(std::stoul(v3.substr(v3.find_first_of("/") + 1, v3.find_last_of("/"))) - 1);

			}
		}

	}
	else
	{
		std::cerr << "Can't open the file amongus_lobby.obj" << std::endl;
	}

	objFile.close();
	
	//centering vertex by vertex
	double xMax = 0, xMin = 0, yMax = 0, yMin = 0, zMax = 0, zMin = 0;
	for (glm::vec3& point : temp_verts) {
		if (point.x > xMax) xMax = point.x;
		if (point.x < xMin) xMin = point.x;
		if (point.y > yMax) yMax = point.y;
		if (point.y < yMin) yMin = point.y;
		if (point.z > zMax) zMax = point.z;
		if (point.z < zMin) zMin = point.z;
	}
	double xCent = (xMax + xMin) / 2;
	double yCent = (yMax + yMin) / 2;
	double zCent = (zMax + zMin) / 2;
	
	//centering each point
	for (glm::vec3& point : temp_verts) {
		point.x -= xCent;
		point.y -= yCent - 4;
		point.z -= zCent + 8;
	}
	
	for (int i = 0; i < vert_inds.size(); i++) {
		vertices.push_back(temp_verts[vert_inds[i]]);
	}

	for (int i = 0; i < norm_inds.size(); i++) {
		normals.push_back(temp_norms[norm_inds[i]]);
	}

	for (int i = 0; i < text_inds.size(); i++) {
		textures.push_back(temp_texts[text_inds[i]]);
	}

	// Set the model matrix to an identity matrix. 
	model = glm::mat4(1);

	// Set the color. 
	color = glm::vec3(0.7, 0.7, 0.7);

	// Generate a Vertex Array (VAO) and Vertex Buffer Object (VBO)
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBO2);
	
	
	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * textures.size(), textures.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 1 to pass point data through to the shader
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, 2 * sizeof(GLfloat), 0);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	textId = loadTexture("amongus_lobby.png");

}
Lobby::~Lobby() 
{
	// Delete the VBO and the VAO.
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO2);
	glDeleteVertexArrays(1, &VAO);	
}

void Lobby::draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader)
{
	// Actiavte the shader program 
	glUseProgram(shader);

	// Get the shader variable locations and send the uniform data to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));

	// Bind the VAO
	glBindVertexArray(VAO);

	glBindTexture(GL_TEXTURE_2D, textId);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT	);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}

unsigned int Lobby::loadTexture(std::string filename)
{
	GLuint textureId;
	glGenTextures(1, &textureId); // Get unique ID for texture
	glBindTexture(GL_TEXTURE_2D, textureId); // Tell OpenGL which texture to edit
	int width, height, nrChannels;
	string name = "amongus_lobby.png";
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(name.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}
	else
	{
		std::cout << "Tex failed to load at path: " << "amongus_lobby.png" << std::endl;
		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // set bi-linear interpolation
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // for both filtering modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture edge mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	return textureId;
}

void Lobby::update()
{
}
