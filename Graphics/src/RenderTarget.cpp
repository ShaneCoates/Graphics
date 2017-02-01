#include "RenderTarget.h"
#include <iostream>
#include <gl_core_4_4.h>
#include <GLFW\glfw3.h>
#include "ShaderLoader.h"
#include "Camera.h"
#include "Debug.h"

RenderTarget::RenderTarget(Camera* _camera) 
{
	m_camera = _camera;
	Init();
}
RenderTarget::~RenderTarget() 
{

}

void RenderTarget::Init() 
{
	//Framebuffer
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	//Generate Render Texture
	glGenTextures(1, &m_fboTexture);
	if(m_MSAA > 1)
	{
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_fboTexture);
		glTexStorage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_MSAA, GL_RGB8, 1320, 800, GL_TRUE);
		glTexParameteri (GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_fboTexture, 0);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, m_fboTexture);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, 1320, 800);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fboTexture, 0);
	}

	//Render Buffer
	glGenRenderbuffers(1, &m_fboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, m_fboDepth);

	if(m_MSAA > 1)
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT24, 1320, 800);
	else
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 1320, 800);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_fboDepth);
	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	//Check status
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) 
	{
		Debug::LogError("Framebuffer Error!");
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	//Generate Full-screen Quad
	float vertexData[] = 
	{
		-1, -1, 0, 1, 0, 0,
		1, -1, 0, 1, 1, 0,
		1, 1, 0, 1, 1, 1,
		-1, 1, 0, 1, 0, 1
	};
	unsigned int indexData[] = 
	{
		0, 1, 2,
		0, 2, 3
	};

	//Generate VAO, VBO and IBO
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, vertexData, GL_STATIC_DRAW);
	glGenBuffers(1, &m_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indexData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, ((char*)0) + 16);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//Load post-processing Shaders
	m_program = ShaderLoader::LoadProgram("res/shaders/post/renderTarget.vs", "res/shaders/post/renderTarget.fs");
}

//Called before rendering scene
void RenderTarget::DrawBegin() 
{

	//Assign Frambuffer and clear it
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glViewport(0, 0, 1320, 800);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

//Called after rendering scene
void RenderTarget::DrawEnd() 
{
	if(m_MSAA > 1)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		//Blit framebuffer with MSAA
		glBlitFramebuffer(0, 0, 1320, 800, 0, 0, 1320, 800, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}
	else

	{
		//Render framebuffer to post-processing shaders
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, 1320, 800);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(m_program);
		int loc = glGetUniformLocation(m_program, "ProjectionView");
		glUniformMatrix4fv(loc, 1, GL_FALSE, &(m_camera->GetProjectionView()[0][0]));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_fboTexture);
		glUniform1i(glGetUniformLocation(m_program, "diffuse"), 0);
		glBindVertexArray(m_vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}
}