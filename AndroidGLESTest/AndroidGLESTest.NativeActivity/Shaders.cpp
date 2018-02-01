
#include "Shaders.h"
#include "SceneManager.h"
#include "PrintUtils.h"
#include <string>
#include <sstream>
#include <EGL\egl.h>

void esLogMessage(const char *formatStr, ...)
{
	va_list params;
	char buf[2048];

	va_start(params, formatStr);
	vsprintf(buf, formatStr, params);

	//printf("%s", buf);
	PrintUtils::PrintD(buf);
	//OutputDebugString(buf);

	va_end(params);
}

/// \brief Load a shader, check for compile errors, print error messages to output log
/// \param type Type of shader (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)
/// \param shaderSrc Shader source string
/// \return A new shader object on success, 0 on failure
//
GLuint esLoadShader(GLenum type, char * filename)
{
	GLuint shader;
	GLint compiled;

	// Create the shader object
	shader = glCreateShader(type);

	if (shader == 0)
		return 0;

	// Load the shader source
	const engine *eng = SceneManager::GetInstance()->GetEngine();
	AAssetManager* mgr = eng->app->activity->assetManager;
	AAsset* file = AAssetManager_open(mgr, filename, AASSET_MODE_BUFFER);

	if (NULL == file)
		return false;

	long fsize = AAsset_getLength(file);
	char *shaderSrc = new char[fsize + 1];

	AAsset_read(file, shaderSrc, fsize);
	AAsset_close(file);

	shaderSrc[fsize] = '\0';

	glShaderSource(shader, 1, (const char **)&shaderSrc, NULL);
	delete[] shaderSrc;

	// Compile the shader
	glCompileShader(shader);

	// Check the compile status
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (!compiled)
	{
		GLint infoLen = 0;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1)
		{
			char* infoLog = new char[infoLen];


			glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
			esLogMessage("Error compiling shader <%s>:\n%s\n", filename, infoLog);

			delete[] infoLog;
		}

		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

/// \brief Create a program object, link program.
//         Errors output to log.
/// \param vertShaderSrc Vertex shader source code
/// \param fragShaderSrc Fragment shader source code
/// \return A new program object linked with the vertex/fragment shader pair, 0 on failure

GLuint esLoadProgram(GLuint vertexShader, GLuint fragmentShader)
{
	GLuint programObject;
	GLint linked;

	// Create the program object
	programObject = glCreateProgram();

	if (programObject == 0)
		return 0;

	glAttachShader(programObject, vertexShader);
	glAttachShader(programObject, fragmentShader);

	// Link the program
	glLinkProgram(programObject);

	// Check the link status
	glGetProgramiv(programObject, GL_LINK_STATUS, &linked);

	if (!linked)
	{
		GLint infoLen = 0;

		glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1)
		{
			char* infoLog = new char[sizeof(char) * infoLen];


			glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
			esLogMessage("Error linking program:\n%s\n", infoLog);

			delete[] infoLog;
		}

		glDeleteProgram(programObject);
		return 0;
	}

	return programObject;
}

int Shaders::Init(char * fileVertexShader, char * fileFragmentShader)
{
	vertexShader = esLoadShader(GL_VERTEX_SHADER, fileVertexShader);

	if ( vertexShader == 0 )
		return -1;

	fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, fileFragmentShader);

	if ( fragmentShader == 0 )
	{
		glDeleteShader( vertexShader );
		return -2;
	}

	program = esLoadProgram(vertexShader, fragmentShader);

	//finding location of uniforms / attributes
	positionAttribute = glGetAttribLocation(program, "a_posL");
	colorAttribute = glGetAttribLocation(program, "a_color");
	normalAttribute = glGetAttribLocation(program, "a_normal");
	uvAttribute = glGetAttribLocation(program, "a_uv");
	uvBlendAttribute = glGetAttribLocation(program, "a_uvBlend");
	texHeightsUniform = glGetUniformLocation(program, "u_heights");
	uvOffsetUniform = glGetUniformLocation(program, "u_tex_offset");
	vmMatrixUniform = glGetUniformLocation(program, "u_vm");
	nmMatrixUniform = glGetUniformLocation(program, "u_nm");
	colorUniform = glGetUniformLocation(program, "u_color");
	matrixUniform = glGetUniformLocation(program, "u_matrix");
	texUniform[0] = glGetUniformLocation(program, "u_texture_0");
	texUniform[1] = glGetUniformLocation(program, "u_texture_1");
	texUniform[2] = glGetUniformLocation(program, "u_texture_2");
	texUniform[3] = glGetUniformLocation(program, "u_texture_3");
	cubeTexUniform = glGetUniformLocation(program, "u_cubeTexture");
	timeUniform = glGetUniformLocation(program, "u_time");
	displacementMaxUniform = glGetUniformLocation(program, "u_disp_max");
	fogAlphaUniform = glGetUniformLocation(program, "u_fog_alpha");
	fogColorUniform = glGetUniformLocation(program, "u_fog_color");
	lightAmbientUniform = glGetUniformLocation(program, "u_lightA");
	lightAmbientRatioUniform = glGetUniformLocation(program, "u_lightA_ratio");
	lightCountUniform = glGetUniformLocation(program, "u_lights_count");

	std::stringstream ss;
	for (int i = 0; i < MAX_LIGHTS; ++i) {
		ss << i;
		std::string index = ss.str();
		ss.clear();

		lightPositionUniform[i] = glGetUniformLocation(program, ("u_light_pos[" + index + "]").c_str());
		lightDirectionUniform[i] = glGetUniformLocation(program, ("u_light_dir[" + index + "]").c_str());
		lightSpecularUniform[i] = glGetUniformLocation(program, ("u_lightS[" + index + "]").c_str());
		lightSpecularCoefUniform[i] = glGetUniformLocation(program, ("u_lightS_ratio[" + index + "]").c_str());
		lightSpecularShininessUniform[i] = glGetUniformLocation(program, ("u_shininess[" + index + "]").c_str());
		lightDiffuseUniform[i] = glGetUniformLocation(program, ("u_lightD[" + index + "]").c_str());
		lightDiffuseCoefUniform[i] = glGetUniformLocation(program, ("u_lightD_ratio[" + index + "]").c_str());
		lightTypeUniform[i] = glGetUniformLocation(program, ("u_light_type[" + index + "]").c_str());
		lightSpotAngleUniform[i] = glGetUniformLocation(program, ("u_cone_angle[" + index + "]").c_str());

	}
	
	return 0;
}

Shaders::~Shaders()
{
	//glDeleteProgram(program);
	//glDeleteShader(vertexShader);
	//glDeleteShader(fragmentShader);
}