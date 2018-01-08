#pragma once

#include <map>
#include <memory>
#include <string>
#include <iostream>
#include "rapidxml/rapidxml.hpp"

#include "Model.h"
#include "Texture.h"
#include "Shader.h"

class ResourceManager 
{
public:
	~ResourceManager();

	static ResourceManager *GetInstance();

	Model *GetModel(std::string id);
	Shader *GetShader(std::string id);
	Texture *GetTexture(std::string id);

	bool Init(std::string filepath);
	void SetEngine(engine * eng);
	Model *LoadModel(std::string id);
	Shader *LoadShader(std::string id);
	Texture *LoadTexture(std::string id);

	void CleanUp();

private:
	static ResourceManager *m_instance;

	engine *m_engine;
	std::map<std::string, ModelResource*> m_model_resource_map;
	std::map<std::string, ShaderResource*> m_shader_resource_map;
	std::map<std::string, TextureResource*> m_texture_resource_map;

	std::map<std::string, Model*> m_model_map;
	std::map<std::string, Shader*> m_shader_map;
	std::map<std::string, Texture*> m_texture_map;

	ResourceManager();
};
