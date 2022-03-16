#include "SpriteAnimation.h"

#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "Texture.h"

void SpriteAnimation::CaculateWorldMatrix()
{
	Matrix m_Sc, m_T;
	m_Sc.SetScale(m_scale);
	m_T.SetTranslation(m_position);
	m_worldMatrix = m_Sc * m_T;
}

SpriteAnimation::SpriteAnimation(std::shared_ptr<Model> model, std::shared_ptr<Shader> shader, std::shared_ptr<Texture> texture, GLint numFrames, GLint numActions, GLint currentAction, GLfloat frameTime)
	: BaseObject()
{
	m_pModel = model;
	m_pShader = shader;
	m_pCamera = nullptr;
	m_pTexture = texture;
	m_numFrames = numFrames;
	m_numActions = numActions;
	m_frameTime = frameTime;
	m_currentFrame = 0;
	m_currentAction = 0;
	m_currentTime = 0.0f;
	m_currentAction = currentAction;

	m_position = Vector3(0, 0, 0);
	m_iHeight = 50;
	m_iWidth = 100;
	m_scale = Vector3((float)m_iWidth /  Globals::screenWidth, (float)m_iHeight /  Globals::screenHeight, 1);
}

SpriteAnimation::~SpriteAnimation()
{
}

void SpriteAnimation::Init()
{
	CaculateWorldMatrix();
}

void SpriteAnimation::Draw()
{
	glUseProgram(m_pShader->m_program);
	glBindBuffer(GL_ARRAY_BUFFER, m_pModel->GetVertexObject());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pModel->GetIndiceObject());

	GLuint iTempShaderVaribleGLID = -1;
	Matrix matrixWVP;

	matrixWVP = m_worldMatrix;//* m_pCamera->GetLookAtCamera();

	if (m_pTexture != nullptr)
	{
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_pTexture->Get2DTextureAdd());
		if (m_pShader->m_iTextureLoc[0] != -1)
			glUniform1i(m_pShader->m_iTextureLoc[0], 0);
	}

	iTempShaderVaribleGLID = -1;
	iTempShaderVaribleGLID = m_pShader->GetAttribLocation((char*)"a_posL");
	if (iTempShaderVaribleGLID != -1)
	{
		glEnableVertexAttribArray(iTempShaderVaribleGLID);
		glVertexAttribPointer(iTempShaderVaribleGLID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), VETEX_POSITION);
	}

	iTempShaderVaribleGLID = -1;
	iTempShaderVaribleGLID = m_pShader->GetAttribLocation((char*) "a_uv");
	if (iTempShaderVaribleGLID != -1)
	{
		glEnableVertexAttribArray(iTempShaderVaribleGLID);
		glVertexAttribPointer(iTempShaderVaribleGLID, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), VETEX_UV);
	}

	iTempShaderVaribleGLID = -1;
	iTempShaderVaribleGLID = m_pShader->GetUniformLocation((char*)"u_alpha");
	if (iTempShaderVaribleGLID != -1)
		glUniform1f(iTempShaderVaribleGLID, 1.0);

	iTempShaderVaribleGLID = -1;
	iTempShaderVaribleGLID = m_pShader->GetUniformLocation((char*)"u_matMVP");
	if (iTempShaderVaribleGLID != -1)
		glUniformMatrix4fv(iTempShaderVaribleGLID, 1, GL_FALSE, matrixWVP.m[0]);

	iTempShaderVaribleGLID = -1;
	iTempShaderVaribleGLID = m_pShader->GetUniformLocation((char*)"u_numFrames");
	if (iTempShaderVaribleGLID != -1)
		glUniform1f(iTempShaderVaribleGLID, m_numFrames);

	iTempShaderVaribleGLID = -1;
	iTempShaderVaribleGLID = m_pShader->GetUniformLocation((char*)"u_currentFrame");
	if (iTempShaderVaribleGLID != -1)
		glUniform1f(iTempShaderVaribleGLID, m_currentFrame);

	iTempShaderVaribleGLID = -1;
	iTempShaderVaribleGLID = m_pShader->GetUniformLocation((char*)"u_numActions");
	if (iTempShaderVaribleGLID != -1)
		glUniform1f(iTempShaderVaribleGLID, m_numActions);

	iTempShaderVaribleGLID = -1;
	iTempShaderVaribleGLID = m_pShader->GetUniformLocation((char*)"u_currentAction");
	if (iTempShaderVaribleGLID != -1)
		glUniform1f(iTempShaderVaribleGLID, m_currentAction);

	glDrawElements(GL_TRIANGLES, m_pModel->GetNumIndiceObject(), GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

}

void SpriteAnimation::Update(GLfloat deltatime)
{
	m_currentTime += deltatime;
	if (m_currentTime >= m_frameTime)
	{
		m_currentFrame++;
		if (m_currentFrame >= m_numFrames)
			m_currentFrame = 0;
		m_currentTime -= m_frameTime;
	}
}


void SpriteAnimation::Set2DPosition(GLfloat width, GLfloat height)
{
	m_Vec2DPos.x = width;
	m_Vec2DPos.y = height;

	float xx = (2.0 * m_Vec2DPos.x) /  Globals::screenWidth - 1.0;
	float yy = 1.0 - (2.0 * m_Vec2DPos.y) /  Globals::screenHeight;
	m_position = Vector3(xx, yy, 1.0);

	CaculateWorldMatrix();
}

void SpriteAnimation::Set2DPosition(Vector2 pos)
{
	m_Vec2DPos = pos;

	float xx = (2.0 * m_Vec2DPos.x) /  Globals::screenWidth - 1.0;
	float yy = 1.0 - (2.0 * m_Vec2DPos.y) /  Globals::screenHeight;
	m_position = Vector3(xx, yy, 1.0);

	CaculateWorldMatrix();
}

Vector2 SpriteAnimation::Get2DPosition()
{
	return m_Vec2DPos;
}


void SpriteAnimation::SetSize(GLint width, GLint height)
{
	m_iWidth = width;
	m_iHeight = height;
	m_scale = Vector3((float)m_iWidth /  Globals::screenWidth, (float)m_iHeight /  Globals::screenHeight, 1);
	CaculateWorldMatrix();
}
