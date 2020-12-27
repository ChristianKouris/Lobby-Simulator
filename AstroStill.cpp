#include "AstroStill.h"

AstroStill::AstroStill(glm::vec3 astro_color)
{
	/* 
	 * TODO: Section 2: Currently, all the points are hard coded below. 
	 * Modify this to read points from an obj file.
	 */
	std::ifstream objFile("amongus_astro_still.obj"); // The obj file we are reading.

	// Astros spawn at random x and random z between -3 and 3
	center = glm::vec3(((double)(rand() % 61 - 30.0)) / 10.0, 0, ((double)(rand() % 61 - 30.0))/10.0);

	// Set the color. 
	color = astro_color;

	// Astros spawn in facing this random vector
	oldDir = glm::normalize(glm::vec3(rand() % 1001 - 500, 0, rand() % 1001 - 500));

	//radius of bounding sphere for astros is 1
	radius = 1;
	//starting time is on creation, stopping time is random time after
	timeToStart = glfwGetTime();
	timeToStop = timeToStart + 5 + (double)(rand() % 1000) / 100.0;

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
		point.x -= xCent - center.x;
		point.y -= yCent;
		point.z -= zCent - center.z;
	}
	
	for (int i = 0; i < vert_inds.size(); i++) {
		vertices.push_back(temp_verts[vert_inds[i]]);
	}

	for (int i = 0; i < norm_inds.size(); i++) {
		normals.push_back(temp_norms[norm_inds[i]]);
	}

	// Set the model matrix to an identity matrix. 
	model = glm::mat4(1);

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 1 to pass point data through to the shader
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(GLfloat), 0);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}
AstroStill::~AstroStill()
{
	// Delete the VBO and the VAO.
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO2);
	glDeleteVertexArrays(1, &VAO);
}

void AstroStill::draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader)
{
	// Actiavte the shader program 
	glUseProgram(shader);

	// Get the shader variable locations and send the uniform data to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(glGetUniformLocation(shader, "colorObj"), 1, glm::value_ptr(color));

	// Bind the VAO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}

void AstroStill::update()
{
	
	if (timeToStop > timeToStart) {
		if (glfwGetTime() >= timeToStop) {
			timeToStart = timeToStop + 5.0 + (double)(rand() % 1000) / 100.0;;
		}
		else if (glfwGetTime() >= timeToStart) {
			move(oldDir);
		}
	}
	else {
		if (glfwGetTime() >= timeToStart) {
			timeToStop = timeToStart + 5.0 + (double)(rand() % 1000) / 100.0;;
			move(oldDir);
		}
		else if (glfwGetTime() >= timeToStop) {}
	}
}

void AstroStill::move(glm::vec3 direction)
{

	float speed = 0.05;
	glm::vec3 translation = speed * glm::normalize(direction);
	model = glm::translate(model, translation);
	center = center + translation;
	//float angle = glm::acos(glm::dot(glm::normalize(direction), glm::normalize(oldDir)));
	//model = glm::rotate(model, angle, glm::vec3(0, 1, 0));
	oldDir = direction;

}

void AstroStill::reflect(glm::vec3 norm) {
	glm::vec3 reflection = glm::normalize(glm::reflect(oldDir, norm));
	move(reflection);
}

glm::vec3 AstroStill::getColor() {
	return color;
}

glm::vec3 AstroStill::getCenter() {
	return center;
}

float AstroStill::getRadius() {
	return radius;
}