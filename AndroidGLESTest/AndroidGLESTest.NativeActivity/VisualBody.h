#pragma once

#include <vector>
#include <string>
#include "Model.h"
#include "Shader.h"
#include "Texture.h"
#include "Math.h"
#include "GameLoopObject.h"
#include "BoundingBox.h"

class VisualBody : public GameLoopObject
{
public:
	VisualBody(Vector3 pos, Vector3 rot, Vector3 scale, std::string name, bool depth_test);
	virtual ~VisualBody();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void Destroy() override;
	virtual std::string ToString() override;
	virtual std::string GetClassName() override;

	void SetWired(bool is_wired);
	void SetBlend(bool use_blend);
	void SetModel(Model *model);
	void SetShader(Shader *shader);

	void AddTexture(Texture *texture);
	//void AddLightID(std::string id);

	inline const std::string & GetName() const
	{
		return m_name;
	}


	virtual void FixedUpdate() override;

protected:
	bool m_depth_test;
	bool m_is_wired;

	Model *m_model;
	Shader *m_shader;

	std::vector<Texture *> m_textures;

	std::string m_name;

	void SharedDrawElements();
};
