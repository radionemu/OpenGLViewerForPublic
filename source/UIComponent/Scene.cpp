#include "Scene.hpp"
#include "source/incls.h"
#include "general/loadshader.hpp"
#include "stb_image.h"

namespace UIComponent
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	void Scene::init(std::shared_ptr<GLVGLTF::FileManager> fManager)
	{
		SceneView::renderer::init(fManager, "shaders/Scene/Vert.vert", "shaders/Scene/Frag.frag");

		// skybox area
		this->skyboxShader = std::make_unique<shaders::Shader>();
		this->skyboxVAO = std::make_unique<VertexArray>();
		this->skyboxVBO = std::make_unique<VertexBuffer>();
		this->skyboxUBO = std::make_unique<UniformBuffer>();
		// skybox shader
		skyboxShader->init();
		skyboxShader->LoadVF_GLSL("./shaders/Scene/skyVert.vert", "./shaders/Scene/skyFrag.frag");
		// skybox vertices
		this->skyboxVAO->Bind();
		this->skyboxVBO->Bind();
		this->skyboxVBO->FillData(&skyboxVertices[0], sizeof(float) * 108);
		BufferLayout pos;
		pos.DirectPush(0, GL_FLOAT, 3, 0);
		this->skyboxVAO->AttribBuffer(*skyboxVBO, pos);
		// skybox textures
		std::vector<std::string> faces = {"./skybox/right.jpg", "./skybox/left.jpg", "./skybox/top.jpg", "./skybox/bottom.jpg", "./skybox/front.jpg", "./skybox/back.jpg"};
		glGenTextures(1, &cubemap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
		int width, height, nrChannels;
		for (size_t i = 0; i < faces.size(); i++)
		{
			uint8_t *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				int rgb = GL_RGBA;
				if (nrChannels == 3)
					rgb = GL_RGB;
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, rgb, width, height, 0, rgb, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
			}
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		}
	}

	void Scene::render()
	{
		size_t flags = ImGuiWindowFlags_NoTitleBar;
		bool is_open;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
		ImGui::Begin("Scene", NULL, flags);
		ImGui::PopStyleVar();

		// update panel size
		ImVec2 PanelSize = ImGui::GetContentRegionAvail();
		this->mPanelSize = {PanelSize.x, PanelSize.y};
		this->mScenePixelSize = {static_cast<int>(mPanelSize.x), static_cast<int>(mPanelSize.y)};

		// config flags
		size_t enFlags = EnableFlags_DEPTHTEST;

		setColor(glm::vec4(1.0f));
		enable(enFlags);
		if (mScenePixelSize.first > 0 && mScenePixelSize.second > 0)
		{
			predraw(mScenePixelSize.first, mScenePixelSize.second);
			// draw skybox
			glDepthMask(GL_FALSE);
			skyboxShader->useProgram();
			glm::mat4 view = glm::mat4(glm::mat3(mCamera->getView()));
			glm::mat4 proj = mCamera->getProj();
			glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(5.0f, 5.0f, 5.0f));
			UniformBuffer ubo, ubo2, ubo3;
			ubo.Bind();
			ubo.AddData(&view[0], sizeof(glm::mat4));
			ubo.FillData(0);
			ubo2.Bind();
			ubo2.AddData(&proj[0], sizeof(glm::mat4));
			ubo2.FillData(1);
			ubo3.Bind();
			ubo3.AddData(&model[0], sizeof(glm::mat4));
			ubo3.FillData(2);
			// skybox cube
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
			skyboxVAO->Bind();
			skyboxVBO->Bind();
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
			glDepthMask(GL_TRUE);

			//
			this->mShader->useProgram();
			UniformBuffer ubo4;
			ubo4.Bind();
			ubo4.AddData(&(mCamera->getProj())[0], sizeof(glm::mat4));
			ubo4.AddData(&(mCamera->getView())[0], sizeof(glm::mat4));
			ubo4.AddData(&(mCamera->getWorld())[0], sizeof(glm::mat4));
			ubo4.AddData(&(mCamera->getModel())[0], sizeof(glm::mat4));
			ubo4.FillData(0);

			glm::vec3 cp = mCamera->getcamPos();
			glUniform3f(9, cp.x, cp.y, cp.z); // campos
			static float ti = 0.01f;
			glm::mat4 rotmat = glm::rotate(glm::mat4(1.0f), ti, glm::vec3(0.0f, 1.0f, 0.0f));
			glm::vec4 lpos = rotmat * glm::vec4(10.0f, -1.0f, 0.0f, 1.0f);
			glUniform3f(15, lpos.x, lpos.y, lpos.z); // lights pos
			glUniform3f(16, 400.0f, 400.0f, 400.0f); // lights color

			glUniform3f(17, lpos.y, -lpos.z, -lpos.x); // lights pos
			glUniform3f(18, 200.0f, 200.0f, 200.0f);   // lights color

			ti += 0.01f;

			draw();
			postdraw();
		}

		disable(enFlags);

		ImGui::End();
	}

	void Scene::onMouseDown(unsigned int MSG, int X, int Y)
	{
		switch (MSG)
		{
		case WM_LBUTTONDOWN:
			this->mKeyInput.isLeft = true;
			break;
		case WM_RBUTTONDOWN:
			this->mKeyInput.isRight = true;
			break;
		default:
			break;
		}
	}

	void Scene::onMouseClick()
	{
	}

	void Scene::onMouseUp(unsigned int MSG, int X, int Y)
	{
		switch (MSG)
		{
		case WM_LBUTTONUP:
			this->mKeyInput.isLeft = false;
			break;
		case WM_RBUTTONUP:
			this->mKeyInput.isRight = false;
			break;
		default:
			break;
		}
	}

	void Scene::onMouseMove(int X, int Y)
	{
		if (!mKeyInput.IsWindowHovered)
		{
			return;
		}

		if (mKeyInput.isLeft)
		{
			mCamera->rotateViewFPRot(glm::vec3((float)(-X + mKeyInput.mX), (float)(mKeyInput.mY - Y), 0));
		}
		else if (mKeyInput.isRight)
		{
			mCamera->translateViewFPTransform(glm::vec3((float)(X - mKeyInput.mX) / 30, (float)(mKeyInput.mY - Y) / 30, 0));
		}

		// update keyInput
		mKeyInput.mX = X;
		mKeyInput.mY = Y;
	}

	void Scene::onMouseWheel(int Z)
	{
		if (!mKeyInput.IsWindowHovered)
		{
			return;
		}

		mCamera->translateViewFPTransform(glm::vec3(0, 0, (float)Z));
	}
}
