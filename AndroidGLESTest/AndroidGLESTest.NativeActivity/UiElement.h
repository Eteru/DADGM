#pragma once
#include "GameLoopObject.h"
#include "Shader.h"

class UiElement :
	public GameLoopObject
{
public:
	struct Design
	{
		std::string tex_id;
		Vector4 text_color;
	};

	UiElement();
	UiElement(float top, float left, float w, float h, Design d);
	virtual ~UiElement();

	virtual void Init() override;
	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void DebugDraw() override;
	virtual void Destroy() override;

	virtual std::string ToString() override;
	virtual std::string GetClassName() override;

	virtual void OnTouchDown(const int x, const int y) override;
	virtual void OnTouchUp(const int x, const int y) override;
	virtual void OnTouchDrag(const int xPrev, const int yPrev, const int x, const int y) override;

	void ConvertToScreenValues();
	void SetTexCoords(const std::vector<GLfloat> & coords);
	void SetShader(const std::string & shader_id);
	inline const Shader *GetShader() const { return m_shader; }
	inline GLuint GetVBO() const { return m_vbo; };
	inline GLuint GetTexVBO() const { return m_tex_vbo; };

protected:
	float m_width;
	float m_height;
	float m_top_offset;
	float m_left_offset;

	GLuint m_vbo;
	GLuint m_tex_vbo;

	Design m_design;
	Shader *m_shader;
};

