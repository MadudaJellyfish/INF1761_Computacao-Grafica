#include "disk.h"

#include <iostream>
#include <vector>

#ifdef _WIN32
#include <glad/glad.h>
#else
#include <OpenGL/gl3.h>
#endif

DiskPtr Disk::Make(int num_segments)
{
	return DiskPtr(new Disk(num_segments));
}

Disk::Disk(int num_segments)
{
	//centro do circulo
	coord.push_back(0.0f);
	coord.push_back(0.0f);

	color.push_back(255);
	color.push_back(255);
	color.push_back(255);
	//

	const float twice_pi = 2.0f * 3.142f;
	for (int i = 0; i <= num_segments; i++)
	{
		float angle = twice_pi * float(i) / float(num_segments); //theta_i
		coord.push_back(0.5f * cos(angle));
		coord.push_back(0.5f * sin(angle));
		color.push_back(255);
		color.push_back(255);
		color.push_back(255);
	}
	// create VAO
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	// create coord buffer
	GLuint id[2];
	glGenBuffers(2, id);
	glBindBuffer(GL_ARRAY_BUFFER, id[0]);
	glBufferData(GL_ARRAY_BUFFER, coord.size() * sizeof(float), coord.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0); // coord
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, id[1]);
	glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(unsigned char), color.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0); // color
	glEnableVertexAttribArray(1);

}

Disk::~Disk()
{
}

void Disk::Draw()
{
	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLE_FAN, 0, coord.size());
}