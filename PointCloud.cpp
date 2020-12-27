#include "PointCloud.h"

PointCloud::PointCloud(std::string objFilename, GLfloat pointSize, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
	: pointSize(pointSize), ambient(ambient), diffuse(diffuse), specular(specular)
{
	/* 
	 * TODO: Section 2: Currently, all the points are hard coded below. 
	 * Modify this to read points from an obj file.
	 */
	mode = 0;
	std::ifstream objFile(objFilename); // The obj file we are reading.

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
				points.push_back(point);
				
			}
			else if (label == "vn")
			{
				// Read the later three float numbers and use them as the 
				// coordinates.
				glm::vec3 point;
				ss >> point.x >> point.y >> point.z;

				// Process the point. For example, you can save it to a.
				norms.push_back(point);

			}
			else if (label == "f")
			{
				// Read the indices.
				glm::uvec3 vertex;
				glm::uvec3 norm;
				std::string v1, v2, v3;
				
				ss >> v1 >> v2 >> v3;
				//the vertex is the number before the delim
				vertex.x = std::stoul(v1.substr(0, v1.find("//"))) - 1;
				vertex.y = std::stoul(v2.substr(0, v2.find("//"))) - 1;
				vertex.z = std::stoul(v3.substr(0, v3.find("//"))) - 1;
				//the norm is the number after the delim
				norm.x = std::stoul(v1.substr(v1.find("//") + 2)) - 1;
				norm.y = std::stoul(v2.substr(v2.find("//") + 2)) - 1;
				norm.z = std::stoul(v3.substr(v3.find("//") + 2)) - 1;
				// Process the point. For example, you can save it to a.
				indicies.push_back(vertex);
				//indicies.push_back(norm);
			}
		}

	}
	else
	{
		std::cerr << "Can't open the file " << objFilename << std::endl;
	}

	objFile.close();

	/*
	 * TODO: Section 4, you will need to normalize the object to fit in the
	 * screen. 
	 */
	// finding the center point
	double xMax = 0, xMin = 0, yMax = 0, yMin = 0, zMax = 0, zMin = 0;
	for (glm::vec3& point : points) {
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
	for (glm::vec3& point : points) {
		point.x -= xCent;
		point.y -= yCent;
		point.z -= zCent;
	}

	//hardcoded scaling factor
	double scalingFact = 9;
	
	//find the largest parameter in order to scale
	double maxPoint = 0;
	for (glm::vec3& point : points) {
		if (std::abs(point.x) > maxPoint) maxPoint = std::abs(point.x);
		if (std::abs(point.y) > maxPoint) maxPoint = std::abs(point.y);
		if (std::abs(point.z) > maxPoint) maxPoint = std::abs(point.z);
	}
	
	//scale the image by multiplying each point by scaling factor / maxpoint
	for (glm::vec3& point : points) {
		point.x *= scalingFact / maxPoint;
		point.y *= scalingFact / maxPoint;
		point.z *= scalingFact / maxPoint;
	}

	// Set the model matrix to an identity matrix. 
	model = glm::mat4(1);

	// Set the color. 
	color = glm::vec3(1, 0, 0);

	// Generate a Vertex Array (VAO) and Vertex Buffer Object (VBO)
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBO2);
	
	
	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(), points.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * norms.size(), norms.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 1 to pass point data through to the shader
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(GLfloat), 0);

	//generate and bind EBO and store data
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::uvec3) * indicies.size(), &indicies[0], GL_STATIC_DRAW);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

PointCloud::~PointCloud() 
{
	// Delete the VBO and the VAO.
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO2);
	glDeleteVertexArrays(1, &VAO);
}

void PointCloud::draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader)
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
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, 3 * indicies.size(), GL_UNSIGNED_INT, NULL);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}

void PointCloud::update()
{
	// Spin the cube by 1 degree
	/*spin(0.1f);*/
	glPointSize(pointSize);
}

void PointCloud::updatePointSize(GLfloat size) 
{
	/*
	 * TODO: Section 3: Implement this function to adjust the point size.
	 */
	pointSize = size;
	
}

void PointCloud::spin(float angle, glm::vec3 axis)
{
	// Update the model matrix by multiplying a rotation matrix
	model = glm::rotate(glm::mat4(1.0f), angle, axis) * model;
}

void PointCloud::zoom(float factor)
{
	model = glm::scale(model, glm::vec3(factor, factor, factor));
}

void PointCloud::shaderMode(GLuint shader) {
	mode = mode ? 0 : 1;
	glUniform1i(glGetUniformLocation(shader, "mode"), mode);
}