
#include "Vertex.h"
#include "SceneManager.h"
#include "SceneObject.h"


SceneObject::SceneObject(Vector3 pos, Vector3 rot, Vector3 scale, std::string name, bool depth_test)
	: m_position(pos), m_rotation(rot), m_scale(scale), m_depth_test(depth_test), m_is_wired(false), m_name(name),
	m_model(nullptr), m_shader(nullptr), m_trajectory(nullptr)
{
	/*if (true == depth_test)
		glEnable(GL_DEPTH_TEST);*/
}


SceneObject::~SceneObject()
{
	
}

void SceneObject::Init()
{
}

void SceneObject::FixedUpdate()
{
}

void SceneObject::Update()
{
	if (nullptr != m_trajectory)
	{
		m_trajectory->NextPosition(m_position, m_rotation);
	}

	GeneralUpdate();
}

void SceneObject::Draw()
{
	glUseProgram(m_shader->GetProgramID());

	glBindBuffer(GL_ARRAY_BUFFER, m_model->GetVBO());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_model->GetIBO(m_is_wired));

	//glActiveTexture(GL_TEXTURE0);
	
	SharedDrawElements();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SceneObject::Destroy()
{
	if (nullptr != m_trajectory) {
		delete m_trajectory;
		m_trajectory = nullptr;
	}
}

std::string SceneObject::ToString()
{
	return std::string("TODO SceneObject string");
}

std::string SceneObject::GetClassName()
{
	return std::string("SceneObject");
}

void SceneObject::SetWired(bool is_wired)
{
	m_is_wired = is_wired;
}

void SceneObject::SetBlend(bool use_blend)
{
	if (true == use_blend) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else {
		glDisable(GL_BLEND);
	}
}

void SceneObject::SetModel(Model * model)
{
	m_model = model;
}

void SceneObject::SetShader(Shader * shader)
{
	m_shader = shader;
}

void SceneObject::SetTrajectory(Trajectory * trajectory)
{
	m_trajectory = trajectory;
}

void SceneObject::AddTexture(Texture * texture)
{
	m_textures.push_back(texture);
}

// void SceneObject::AddLightID(std::string id)
// {
// 	m_light_ids.push_back(id);
// }

bool SceneObject::Collides(SceneObject * obj)
{
	return GetBB().Collides(obj->GetBB());
}

bool SceneObject::Contains(const Vector3 & point)
{
	return GetBB().Contains(point);
}

void SceneObject::SharedDrawElements()
{
	Shaders objShader = m_shader->GetShaderData();
	Camera *cam = SceneManager::GetInstance()->GetActiveCamera();

	for (int i = 0; i < m_textures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(m_textures[i]->GetTextureType(), m_textures[i]->GetID());
		if (objShader.texUniform[i] != -1) {
			glUniform1i(objShader.texUniform[i], i);
		}
	}

	if (objShader.positionAttribute != -1)
	{
		glEnableVertexAttribArray(objShader.positionAttribute);
		glVertexAttribPointer(objShader.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}

	if (objShader.colorAttribute != -1)
	{
		glEnableVertexAttribArray(objShader.colorAttribute);
		glVertexAttribPointer(objShader.colorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) sizeof(Vector3));
	}

	if (objShader.normalAttribute != -1)
	{
		glEnableVertexAttribArray(objShader.normalAttribute);
		glVertexAttribPointer(objShader.normalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Vector3) * 2));
	}

	if (objShader.uvAttribute != -1)
	{
		glEnableVertexAttribArray(objShader.uvAttribute);
		glVertexAttribPointer(objShader.uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Vector3) * 3));
	}

	if (objShader.nmMatrixUniform != -1)
	{
		Matrix MV = m_M * cam->GetView();
		Matrix NM = MV.Inverse().Transpose();
		glUniformMatrix4fv(objShader.nmMatrixUniform, 1, GL_FALSE, reinterpret_cast<GLfloat*>(NM.m));
	}

	if (objShader.vmMatrixUniform != -1)
	{
		Matrix MV = m_M * cam->GetView();
		glUniformMatrix4fv(objShader.vmMatrixUniform, 1, GL_FALSE, reinterpret_cast<GLfloat*>(MV.m));
	}

	if (objShader.matrixUniform != -1)
	{
		Matrix MVP = m_M * cam->GetView() * cam->GetProjection();
		glUniformMatrix4fv(objShader.matrixUniform, 1, GL_FALSE, reinterpret_cast<GLfloat*>(MVP.m));
	}

	const Fog fog = SceneManager::GetInstance()->GetFog();

	if (objShader.fogAlphaUniform != -1)
	{
		float distance = m_position.Distance(cam->GetPosition());
		float alpha = fog.ComputeAlpha(distance);
		glUniform1f(objShader.fogAlphaUniform, alpha);
	}

	if (objShader.fogColorUniform != -1)
	{
		Vector3 color = fog.GetColor();
		glUniform3fv(objShader.fogColorUniform, 1, &color.x);
	}

	const AmbientalLight amb_light = SceneManager::GetInstance()->GetAmbientalLight();

	if (objShader.lightAmbientUniform != -1)
	{
		Vector3 color = amb_light.GetColor();
		glUniform3fv(objShader.lightAmbientUniform, 1, &color.x);
	}

	if (objShader.lightAmbientRatioUniform != -1)
	{
		float ratio = amb_light.GetRatio();
		glUniform1f(objShader.lightAmbientRatioUniform, ratio);
	}

	
	const std::vector<GameLoopObject *> lights = SceneManager::GetInstance()->FindComponents("LightSource");

	if (objShader.lightCountUniform != -1)
	{
		glUniform1i(objShader.lightCountUniform, lights.size());
	}

	uint16_t count = 0;
	for (GameLoopObject *lsObj : lights)
	{

		LightSource *ls = dynamic_cast<LightSource*>(lsObj);

		if (objShader.lightTypeUniform[count] != -1)
		{
			int light_type = static_cast<int>(ls->GetType());
			glUniform1i(objShader.lightTypeUniform[count], light_type);
		}

		if (objShader.lightPositionUniform[count] != -1)
		{
			//Vector3 pos = ls.second->GetPosition();
			Vector3 pos = (LightSource::SPOT_LIGHT == ls->GetType()) ? cam->GetPosition() : ls->GetPosition();
			glUniform3fv(objShader.lightPositionUniform[count], 1, &pos.x);
		}

		if (objShader.lightDirectionUniform[count] != -1)
		{
			Vector3 dir = (LightSource::SPOT_LIGHT == ls->GetType()) ? cam->GetTarget() : ls->GetDirection();
			glUniform3fv(objShader.lightDirectionUniform[count], 1, &dir.x);
		}

		if (objShader.lightSpecularUniform[count] != -1)
		{
			Vector3 color = ls->GetSpecularLight();
			glUniform3fv(objShader.lightSpecularUniform[count], 1, &color.x);
		}

		if (objShader.lightSpecularCoefUniform[count] != -1)
		{
			float coef = ls->GetSpecularCoefficient();
			glUniform1f(objShader.lightSpecularCoefUniform[count], coef);
		}

		if (objShader.lightSpecularShininessUniform[count] != -1)
		{
			float shininess = ls->GetShininess();
			glUniform1f(objShader.lightSpecularShininessUniform[count], shininess);
		}

		if (objShader.lightDiffuseUniform[count] != -1) 
		{
			Vector3 color = ls->GetDiffuseLight();
			glUniform3fv(objShader.lightDiffuseUniform[count], 1, &color.x);
		}

		if (objShader.lightDiffuseCoefUniform[count] != -1)
		{
			float coef = ls->GetDiffuseCoefficient();
			glUniform1f(objShader.lightDiffuseCoefUniform[count], coef);
		}

		if (objShader.lightSpotAngleUniform[count] != -1) 
		{
			float angle = ls->GetSpotAngle();
			glUniform1f(objShader.lightSpotAngleUniform[count], angle);
		}

		++count;
	}

	glDrawElements(
		true == m_is_wired ? GL_LINES : GL_TRIANGLES,      // mode
		m_model->GetIBOCount(m_is_wired),    // count
		GL_UNSIGNED_SHORT,   // type
		(void*)0           // element array buffer offset
	);
}

void SceneObject::GeneralUpdate()
{
	m_M = Matrix().SetScale(m_scale) *
		(Matrix().SetRotationX(m_rotation.x) *
			Matrix().SetRotationY(m_rotation.y) *
			Matrix().SetRotationZ(m_rotation.z)) *
		Matrix().SetTranslation(m_position);
}
