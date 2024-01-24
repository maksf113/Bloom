#include "Model.h"
#include "PointLight.h"
#include "FBO.h"
#include "Cubemap.h"
#include "PickFBO.h"
#include "Floor.h"

#define numPointLights 3

GLuint width = 1920;
GLuint height = 1080;
float prevTime = 0.0f;
float crntTime = 0.0f;
float dt = 0.0f;

int main(void) 
{
	// Initialize GLFW
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	// Tell GLFW what version of OpenGL we are using 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Create a GLFWwindow object
	GLFWwindow* window = glfwCreateWindow(width, height, "Chapter 2 - program1", NULL, NULL);
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);
	// Specify the viewport of OpenGL in the Window

	glViewport(0, 0, width, height);
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);


	// backpack
	Model model("backpack/backpack.obj");
	Floor floor(10.0f, "planks.png", "planksSpec.png");
	ShaderProgram shader("default.vert", "default.frag");
	ShaderProgram lightShader("light.vert", "light.frag");
	ShaderProgram framebufferShader("framebuffer.vert", "framebuffer.frag");
	ShaderProgram pointShadowShader("pointShadowMap.vert", "pointShadowMap.frag", "pointShadowMap.geom");
	ShaderProgram pickShader("pick.vert", "pick.frag");

	framebufferShader.Activate();
	int screenTexUnit = 0;
	framebufferShader.setInt("screenTexture", screenTexUnit);
	framebufferShader.setInt("brightTexture", screenTexUnit + 1);
	framebufferShader.setInt("width", width);
	framebufferShader.setInt("height", height);

	// light

	PointLight light0(0, glm::vec3(1.0f,  1.5f, 3.0f), glm::vec3(0.9f, 0.5f, 0.4f) * 3.0f);
	PointLight light1(1, glm::vec3(-2.0f, 1.5f, 1.0f), glm::vec3(0.8f, 0.8f, 0.8f) * 3.0f);
	PointLight light2(2, glm::vec3(2.0f,  1.5f, -1.0f), glm::vec3(0.4f, 0.7f, 0.4f) * 3.0f);

	PointLight lights[numPointLights] = { light0, light1, light2 };

	glm::vec3 floorPos = glm::vec3(0.0f, -1.0f, 0.0f);
	glm::vec3 bPackPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 floorModel = glm::mat4(1.0f);
	glm::mat4 bPackModel = glm::mat4(1.0f);
	float scale = 0.1f;
	bPackModel = glm::translate(bPackModel, bPackPos);
	bPackModel = glm::scale(bPackModel, glm::vec3(scale));
	glm::mat3 trInvBpack = glm::mat3(transpose(inverse(glm::mat3(bPackModel))));
	floorModel = glm::translate(floorModel, floorPos);
	
	 
	for (int i = 0; i < numPointLights; i++)
	{
		lights[i].SetLightUniforms(lightShader);
		lights[i].SetModelUniforms(shader);
	}

	Camera camera(width, height, glm::vec3(2.0f, 1.5f, 2.0f));

	int counter = 0;
	FBO primaryFBO(width, height);
	primaryFBO.GenTexture();
	primaryFBO.Attach16F();
	primaryFBO.GenRBO();
	//primaryFBO.GenRectVAO();

	FBO extractBrightFBO(width, height);
	extractBrightFBO.GenTexture();
	extractBrightFBO.Attach16F();
	extractBrightFBO.GenRectVAO();
	ShaderProgram extractBrightShader("framebuffer.vert", "extractBright.frag");

	FBO blurIterFBO[2] = { FBO(width, height), FBO(width, height) };
	for (unsigned int i = 0; i < 2; i++)
	{
		blurIterFBO[i].GenTexture();
		blurIterFBO[i].Attach16F();
		blurIterFBO[i].GenRectVAO();
	}
	ShaderProgram shaderBlur("framebuffer.vert", "framebufferGauss.frag");

	FBO finalBloomFBO(width, height);
	finalBloomFBO.GenTexture();
	finalBloomFBO.Attach16F();
	//finalBloomFBO.GenRBO();
	finalBloomFBO.GenRectVAO();

	ShaderProgram finaleBloomShader("framebuffer.vert", "framebufferBloom.frag");


	// SHADOW POINT LIGHT
	
	GLuint firstShadowCubeUnit = 3;
	GLuint shadowCubeSize = 1024;
	float pointLightFarPlane = 100.0;
	std::vector<Cubemap> shadowCubemap;

	glm::mat4 shadowProj;;
	glm::vec3 lightPos;
	
	for (int i = 0; i < numPointLights; i++)
		shadowCubemap.push_back(Cubemap(GL_DEPTH_ATTACHMENT, shadowCubeSize, lights[i].position));
		
	
	for (int i = 0; i < numPointLights; i++)
	{
		shadowCubemap[i].Bind();
		shadowCubemap[i].DisableColor();
		shadowCubemap[i].SetPosition(lights[i].position);

		shadowProj = glm::perspective(glm::radians(90.0f), 1.0f, 0.05f, pointLightFarPlane);
		lightPos = lights[i].position;
		pointShadowShader.Activate();
		for (GLuint j = 0; j < 6; j++)
		{
			shadowCubemap[i].SwitchToFace(j);

			glm::mat4 shadowMatrix = shadowProj * shadowCubemap[i].GetCamera().GetView();
			pointShadowShader.setMat4("shadowMatrices[" + std::to_string(j) + "]", shadowMatrix);
		}

		shadowCubemap[i].SetFaceToDraw(0);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_DEPTH_BUFFER_BIT);
		glDepthFunc(GL_LESS);
		

		pointShadowShader.Activate();
		pointShadowShader.setVec3("lightPos", lights[i].position);

		pointShadowShader.setFloat("pointLightFarPlane", shadowCubemap[i].GetCamera().GetFarPlane());
		pointShadowShader.setMat4("model", bPackModel);

		model.Draw(pointShadowShader, shadowCubemap[i].GetCamera());
		pointShadowShader.setMat4("model", floorModel);

		floor.Draw(pointShadowShader, shadowCubemap[i].GetCamera());
		lights[i].SetShadowMap(shadowCubemap[i].GetTexID());
	}
	// PICKING FBO

	PickFBO pickFBO(width, height);
	bool picked = false;
	GLint id = -1;

	// MAIN LOOP
	while (!glfwWindowShouldClose(window)) 
	{
		crntTime = glfwGetTime();
		dt = crntTime - prevTime;
		counter++;
		if (dt >= 1.0 / 30.0)
		{ 
			std::string FPS = std::to_string(1.0 / dt * counter);
			std::string ms = std::to_string(dt / counter * 1000);
			std::string newTitle = "Project 8 - " + FPS + "FPS / " + ms + "ms";
			glfwSetWindowTitle(window, newTitle.c_str());
			counter = 0;
			prevTime = crntTime;
		}
		// PICKING
		GLclearError();
		pickFBO.Bind();
		pickFBO.Viewport();
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		pickShader.Activate();
		pickShader.setMat4("camMatrix", camera.GetVP());
		for (GLuint i = 0; i < numPointLights; i++)
		{
			glm::mat4 posMat = glm::translate(glm::mat4(1.0f), lights[i].position);
			pickShader.setMat4("model", posMat);
			pickShader.setUint("objectIndex", i + 1);
			lights[i].Draw(pickShader, camera);
		}
		
		glm::vec2 cursor = camera.CursorPosition(window);
		PickFBO::PixelInfo pixelInfo;
		if (!camera.IsLocked() && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && camera.IsFirstClickLeft())
		{
			pixelInfo = pickFBO.ReadPixel(GLuint(cursor.x), height - GLuint(cursor.y) - 1);	
			if (pixelInfo.objectID > 0)
			{
				id = pixelInfo.objectID - 1;
				picked = true;
			}
		}
		if (!camera.IsLocked() && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
		{
			id = -1;
			picked = false;
		}
		if (picked)
		{
			if (id >= 0)
				lights[id].position = pickFBO.UpdateObjectPosition(window, lights[id].position, cursor, camera);
		}
		GLcheckError();
		// SHADOWS
		for (int i = 0; i < numPointLights; i++)
		{
			
			shadowCubemap[i].Bind();
			shadowCubemap[i].SetPosition(lights[i].position);

			lightPos = lights[i].position;
			pointShadowShader.Activate();
			for (GLuint j = 0; j < 6; j++)
			{
				shadowCubemap[i].SwitchToFace(j);
				glm::mat4 shadowMatrix = shadowProj * shadowCubemap[i].GetCamera().GetView();   
				pointShadowShader.setMat4("shadowMatrices[" + std::to_string(j) + "]", shadowMatrix);
			}

			shadowCubemap[i].SetFaceToDraw(0);
			glEnable(GL_DEPTH_TEST);
			glClear(GL_DEPTH_BUFFER_BIT);
			glDepthFunc(GL_LESS);
			
			
			pointShadowShader.Activate();
			pointShadowShader.setVec3("lightPos", lights[i].position);
			pointShadowShader.setFloat("pointLightFarPlane", shadowCubemap[i].GetCamera().GetFarPlane());
			pointShadowShader.setMat4("model", bPackModel);
			model.Draw(pointShadowShader, shadowCubemap[i].GetCamera());
			pointShadowShader.setMat4("model", floorModel);
			floor.Draw(pointShadowShader, shadowCubemap[i].GetCamera()); 
			lights[i].SetShadowMap(shadowCubemap[i].GetTexID());
			
		}
		// PRIMARY
		primaryFBO.Bind();
		primaryFBO.Viewport();

		// Specify the color of the background
		// glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		
		camera.updateMatrix(45.0f, 0.1f, 100.0f);
		camera.Input(window, dt);

		shader.Activate();
		shader.setVec3("camDir", camera.GetDirection());
		shader.setMat4("model", floorModel);
		shader.setBool("flashLight", camera.GetFlashlight());

		
		shader.setFloat("pointLightFarPlane", pointLightFarPlane);
		
		for (GLuint i = 0; i < numPointLights; i++)
		{
			
			glActiveTexture(GL_TEXTURE0 + firstShadowCubeUnit + i);
			GLclearError();
			lights[i].BindShadowTexture();
			lights[i].SetModelUniforms(shader);
			shader.setInt("pointLights[" + std::to_string(i) + "].shadowMap", 
				firstShadowCubeUnit + i);
		}
		
		floor.Draw(shader, camera);

		shader.setMat4("model", bPackModel);
		model.Draw(shader, camera);

		for (int i = 0; i < numPointLights; i++)
		{
			lightShader.Activate();
			lights[i].SetLightUniforms(lightShader);
			lights[i].Draw(lightShader, camera);
		}
		// BRIGHT EXTRACTION
		extractBrightFBO.Bind();
		extractBrightShader.Activate();
		extractBrightShader.setInt("primaryTexture", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, primaryFBO.GetTexID());
		extractBrightFBO.DrawRect();


		// BLUR
		bool horizontal = true, first_iteration = true;
		int amount = 10;
		shaderBlur.Activate();
		shaderBlur.setInt("texToBlur", 0);
		//GLclearError();
		for (unsigned int i = 0; i < amount; i++)
		{
			blurIterFBO[horizontal].Bind();
			shaderBlur.setInt("horizontal", horizontal);
			shaderBlur.setBool("odd", (GLboolean)i % 2);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture
			(
				GL_TEXTURE_2D, first_iteration ? extractBrightFBO.GetTexID() : blurIterFBO[!horizontal].GetTexID()
			);
			
			//GLclearError();
			blurIterFBO[horizontal].DrawRect();
			GLcheckError();
			horizontal = !horizontal;
			if (first_iteration)
				first_iteration = false;
		}
		
		// FINAL
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		finaleBloomShader.Activate();
		finaleBloomShader.setInt("primaryTexture", 0);
		finaleBloomShader.setInt("bloomBlur", 1);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, primaryFBO.GetTexID());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, blurIterFBO[!horizontal].GetTexID());
		//GLclearError();
		finalBloomFBO.DrawRect();
		
		
		
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
		
		GLcheckError();
	}
	shader.Delete();
	lightShader.Delete();
	framebufferShader.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}



