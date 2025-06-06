#define GLEW_STATIC

#include "error.h"
#include "Light.h"
#include "freeLookCamera.h"
#include "events.h"
#include "openglFunctions.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../ParticleGenerator.h"
#include <algorithm>


using namespace std;

freeCamera Camera;

Light scene_light;

Mouse_input Manager;

bool is_perspective = true;

void Uinput(float ts)
{
	if (Manager.mouse_scroll_return().increase)
	{
		Camera.MovementSpeed += 1.25f;
	}

	else if (Manager.mouse_scroll_return().decrease)
	{
		Camera.MovementSpeed -= 1.25f;
	}

	if (Manager.mouseMoved())
	{
		Camera.ProcessMouseMovement(Manager.getCusorPosition()[0], Manager.getCusorPosition()[1]);
	}
}


void display(GLsizei count)
{
	GLcall(glDrawArrays(GL_TRIANGLES, 0, count));
}

void displayIndex(GLsizei count)
{
	GLcall(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr));
}

void display(GLsizei start, GLsizei count)
{
	// draw triangles starting from index 0 and
	// using 3 indices
	GLcall(glDrawArrays(GL_TRIANGLE_STRIP, start, count));
}

// main function
int main(int argc, char** argv)
{
	//............................................................................
	if (!glfwInit()) {
		std::cout << "initialization failed!" << std::endl;
		return 0;
	}
	//................................................................................
	GLFWwindow* m_window = glfwCreateWindow(1280, 720, "FIRE", nullptr, nullptr);

	int max, min, r;
	glfwGetVersion(&max, &min, &r);
	//..............................................................................
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, max);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, min);

	std::cout << "version >> " << max << "." << min << std::endl;
	//...............................................................
	glfwMakeContextCurrent(m_window);

	glfwSetErrorCallback((GLFWerrorfun)[](int error_code, const char* description) {
		fprintf(stderr, "Error: %s\n", description);
		});

	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//.............................................................
	if (glewInit() != GLEW_OK)
	{
		std::cout << "error opengl loader not initialized" << std::endl;
	}

	//...........................................................
	//std::cout << glGetString(GL_VERSION) << std::endl;
	Manager = Mouse_input(m_window);
	init();

	Camera.Position = glm::vec3(15.0f, 3.0f, 45.0f);

	//...........................................................
	glm::mat4 projection = glm::infinitePerspective(glm::radians(60.0f), 16.0f / 9.0f, 0.1f);
	glm::mat4 model = glm::mat4(1.0f), view = glm::mat4(1.0f), spotModel = glm::mat4(1.0f);
	//.............................................................

	//.........................................................
	scene_light = Light(glm::vec3(0.5f, 0.4f, 0.2f), "light[0]", 25500.0f, 0.0925f, 15250.51f);

	scene_light.set_position(glm::vec3(0.0f, 200.0f, 0.0f));
	scene_light.set_Spotdirection(glm::vec3(0.0f, -200.0f, 0.0f));
	scene_light.set_spotlight_cutoff(glm::cos(3.14f / 15.0f));
	scene_light.set_spotlight_innercutoff(glm::cos(3.14f / 100.0f));
	scene_light.set_attenuation_constants(4.0f, 1.0f, 0.5f, 0.44f);

	//set s projection /////////////////////////////////////////////
	general_program.setUniformMat4("projection", projection);
	general_program.Bind();

	float deltaTime, currentFrame, lastFrame = 0;

	float theta = 0.0f;
	glm::vec3 lightPos = glm::vec3(glm::sin(theta) * 150, 100, glm::cos(theta) * 150);

	openglShader ParticleShader = openglShader("asset/vs.glsl", "asset/fs.glsl", "ParticleShader");
	ParticleShader.Bind();
	ParticleShader.setUniformMat4("projection", projection);

	ParticleGenerator Generator(ParticleShader, opengl2Dtexture("asset/fire.jpg"), 10, 4.0f, 5.0f, glm::vec3(0.5f));
	Generator.CenterPos = glm::vec3(0.0f, 3.0f, 0.0f);
	Generator.GeneratorConstSpeed = glm::vec3(1.0f, 2.0f, 1.0f);


	ParticleGenerator Generator2(ParticleShader, opengl2Dtexture("asset/fire.png"), 10, 4.50f, 4.50f, glm::vec3(1.0f, 0.8f, 0.4f));
	Generator2.CenterPos = glm::vec3(-.5f, 3.0f, 0);
	Generator2.GeneratorConstSpeed = glm::vec3(0.1f, 0.1f, 0.1f);


	ParticleGenerator Generator3(ParticleShader, opengl2Dtexture("asset/fire.png"), 10, 4.50f, 4.50f, glm::vec3(1.0f, 0.8f, 0.4f));
	Generator3.CenterPos = glm::vec3(0.5f, 3.0f, 0);
	Generator3.GeneratorConstSpeed = glm::vec3(0.1f, 0.1f, 0.1f);

	float flicker = 2550.0f;
	float factor = sqrt((float)rand() / (float)RAND_MAX);

	while (!glfwWindowShouldClose(m_window))
	{
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//Mouse_input////////////////////////////////////////
		Uinput(deltaTime);


		float camera_speed = 3.0f;

		if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
		{
			Camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime * camera_speed);
		}
		else if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
		{
			Camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime * camera_speed);
		}

		else if (glfwGetKey(m_window, GLFW_KEY_W))
		{
			Camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime * camera_speed);
		}

		else if (glfwGetKey(m_window, GLFW_KEY_S))
		{
			Camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime * camera_speed);
		}

		else if (glfwGetKey(m_window, GLFW_KEY_Q))
		{
			Camera.ProcessKeyboard(Camera_Movement::UP, deltaTime * camera_speed);
		}

		else if (glfwGetKey(m_window, GLFW_KEY_E))
		{
			Camera.ProcessKeyboard(Camera_Movement::DOWN, deltaTime * camera_speed);
		}

		else if (glfwGetKey(m_window, GLFW_KEY_P))
		{
			is_perspective = !is_perspective;
		}


		//sets view///////////////////////////
		view = Camera.GetViewMatrix();
		projection = (is_perspective ? glm::infinitePerspective(glm::radians(60.0f), 16.0f / 9.0f, 0.1f) : glm::orthoRH(-105.0f, 105.0f, -105.0f, 105.0f, 0.01f, 10000.0f));


		if (glfwGetKey(m_window, GLFW_KEY_ESCAPE))
		{
			break;
		}
		factor = (sqrt((float)rand() / (float)RAND_MAX) + factor) / 2;

		//flicker = (flicker + clamp(2550.0f * factor , 100.0f, 2550.0f));
		flicker = 2550.0f * factor;
		scene_light.set_intensity(25500.0f + flicker);
		scene_light.set_spotlight_cutoff(glm::cos(3.14f / (15.0f + 3.0f * factor)));
		scene_light.set_spotlight_innercutoff(glm::cos(3.14f / (100.0f + 10.0f * factor)));
		//set light uniforms
		scene_light.render(Camera.Position, general_program.getRendererId(), false, true);

		//sets view matrix ///////////////////////////
		general_program.Bind();
		general_program.setUniformMat4("view", view);
		general_program.setUniformMat4("projection", projection);


		GLcall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		GLcall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));

		GLcall(glViewport(0, 0, 1280, 720));
		{
			//sets texture sampler
			general_program.Bind();
			floorTexture.bindTextureToSlot(1);
			general_program.setTextureSampler("_MainTex", floorTexture.GetTextureSlot());
			general_program.setTextureSampler("texture_2", floorTexture.GetTextureSlot());

			//sets model matrix ////////////////////////////
			//model transformation/////////////////////////////////////				

			//sets model and color uiniforms
			general_program.setUniformMat4("projection", projection);
			general_program.setUniformMat4("model", glm::scale(glm::mat4(1.0f), glm::vec3(400.0f, 1.0f, 400.0f)));
			general_program.setUniformMat4("view", view);

			//ParticleShader.Bind();

			planeBuffer.Bind();
			planeVao.Bind();


			display(36);
		}


		model = glm::mat4(1.0f);

		//float scale = 10.0f;

		ParticleShader.setUniformMat4("view", view);

		Generator.spawnparticles(15, glm::vec3(-3.0f), glm::vec3(3.0f));

		Generator.Update(deltaTime, 0, glm::vec3(0.1f, 0.1f, 0.0f));

		Generator.Draw(Camera.Position, glm::vec3(0.2f));


		//second set of particles

		ParticleShader.setUniformMat4("view", view);

		Generator2.spawnparticles(6, glm::vec3(-0.3f), glm::vec3(0.30f, 0.4f, 0.3f));

		Generator2.Update(deltaTime, 0, glm::vec3(0.f, 0.1f, 0.0f));

		Generator2.Draw(Camera.Position, glm::vec3(1.4f));


		//third set of particles

		//ParticleShader.setUniformMat4("view", view);

		/*Generator3.spawnparticles(6, glm::vec3(-0.3f), glm::vec3(0.30f, 0.4f, 0.3f));
		Generator3.Update(deltaTime, 0, glm::vec3(0.1f, 0.1f, 0.0f));

		Generator3.Draw(Camera.Position, glm::vec3(1.4f));*/



		glfwSwapBuffers(m_window);
		//polls all Mouse_input recorded
		glfwPollEvents();
	}

	return 0;
}

