#pragma once
#include <GLES2\gl2.h>

class Shaders 
{
public:
	static constexpr int MAX_LIGHTS = 20;

	GLuint program, vertexShader, fragmentShader;
	char fileVS[260];
	char fileFS[260];

	GLint positionAttribute;
	GLint colorAttribute;
	GLint normalAttribute;
	GLint uvAttribute;
	GLint uvBlendAttribute;
	GLint texHeightsUniform;
	GLint uvOffsetUniform;
	GLint matrixUniform;
	GLint nmMatrixUniform;
	GLint vmMatrixUniform;
	GLint colorUniform;
	GLint hasTextureUniform;
	// texture blending
	GLint texUniform[4];
	// skybox texture
	GLint cubeTexUniform;
	// fire
	GLint timeUniform;
	GLint displacementMaxUniform;
	//fog
	GLint fogAlphaUniform;
	GLint fogColorUniform;
	// light
	GLint lightCountUniform;
	GLint lightAmbientUniform;
	GLint lightAmbientRatioUniform;

	GLint lightTypeUniform[MAX_LIGHTS];
	GLint lightPositionUniform[MAX_LIGHTS];
	GLint lightDirectionUniform[MAX_LIGHTS];
	GLint lightSpecularUniform[MAX_LIGHTS];
	GLint lightSpecularCoefUniform[MAX_LIGHTS];
	GLint lightSpecularShininessUniform[MAX_LIGHTS];
	GLint lightDiffuseUniform[MAX_LIGHTS];
	GLint lightDiffuseCoefUniform[MAX_LIGHTS];
	GLint lightSpotAngleUniform[MAX_LIGHTS];

	int Init(char * fileVertexShader, char * fileFragmentShader);
	~Shaders();
};