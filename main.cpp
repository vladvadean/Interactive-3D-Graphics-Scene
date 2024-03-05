#if defined (__APPLE__)
#define GLFW_INCLUDE_GLCOREARB
#define GL_SILENCE_DEPRECATION
#else
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include <glm/glm.hpp> //core glm functionality
#include <glm/gtc/matrix_transform.hpp> //glm extension for generating common transformation matrices
#include <glm/gtc/matrix_inverse.hpp> //glm extension for computing inverse matrices
#include <glm/gtc/type_ptr.hpp> //glm extension for accessing the internal data structure of glm types

#include "Window.h"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Model3D.hpp"
#include "SkyBox.hpp"

#include <iostream>


float yaw = -90.0f; // Initial yaw angle
float pitch = 0.0f; // Initial pitch angle
float lastX = 0.0f;
float lastY = 0.0f;
bool firstMouse = true;

// window
gps::Window myWindow;

// matrices
glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;
glm::mat3 normalMatrix;

// light parameters
glm::vec3 lightDir;
glm::vec3 lightColor;

// shader uniform locations
GLint modelLoc;
GLint viewLoc;
GLint projectionLoc;
GLint normalMatrixLoc;
GLint lightDirLoc;
GLint lightColorLoc;

// camera
gps::Camera myCamera(
	glm::vec3(0.0f, 0.0f, 3.0f),
	glm::vec3(0.0f, 0.0f, -10.0f),
	glm::vec3(0.0f, 1.0f, 0.0f));

GLfloat cameraSpeed = 0.1f;

GLboolean pressedKeys[1024];

// models
gps::Model3D heli;
gps::Model3D scene;
GLfloat angle;

// shaders
gps::Shader myBasicShader;

// to change color and background
bool setLightColor = false;
bool setSunsetSkyBox = false;
bool setNight = false;

// to change the fog
bool fogOn = false;
GLfloat fogDensity = 0.0f;
GLfloat lastFogDensity = 0.0f;
glm::vec4 fogColor;
GLuint fogLoc;
GLuint fogColorLoc;

// skybox
gps::SkyBox mySkyBox;
gps::Shader skyboxShader;

// camera animation
bool animationActive = false;

// spotlight
glm::mat4 lightRotation;
GLfloat constant = 1.0f;
GLfloat linear = 0.1f;
GLfloat quadratic = 0.1f;
glm::vec3 position = glm::vec3(-10.757727f, 1.983271f, -1.666334f);
glm::vec3 lampColor;

GLuint lampColorLoc;
GLuint constantLoc;
GLuint linearLoc;
GLuint quadraticLoc;
GLuint positionLoc;

// angles
GLfloat spotCutoff = 12.0f;
GLfloat spotOuterCutoff = 15.0f;

// view mode
int mode = 0;

GLenum glCheckError_(const char* file, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR) {
		std::string error;
		switch (errorCode) {
		case GL_INVALID_ENUM:
			error = "INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			error = "INVALID_VALUE";
			break;
		case GL_INVALID_OPERATION:
			error = "INVALID_OPERATION";
			break;
		case GL_OUT_OF_MEMORY:
			error = "OUT_OF_MEMORY";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			error = "INVALID_FRAMEBUFFER_OPERATION";
			break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void initSkybox() {
	std::vector<const GLchar*> faces;
	if (setSunsetSkyBox == false) {
		faces.push_back("skybox/right.tga");
		faces.push_back("skybox/left.tga");
		faces.push_back("skybox/top.tga");
		faces.push_back("skybox/bottom.tga");
		faces.push_back("skybox/back.tga");
		faces.push_back("skybox/front.tga");
	}
	else {
		faces.push_back("sunsetskybox/marslike01rt.tga");
		faces.push_back("sunsetskybox/marslike01lf.tga");
		faces.push_back("sunsetskybox/marslike01up.tga");
		faces.push_back("sunsetskybox/marslike01dn.tga");
		faces.push_back("sunsetskybox/marslike01bk.tga");
		faces.push_back("sunsetskybox/marslike01ft.tga");
	}
	mySkyBox.Load(faces);
}

void windowResizeCallback(GLFWwindow* window, int width, int height) {
	fprintf(stdout, "Window resized! New width: %d , and height: %d\n", width, height);
	//TODO
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	// Toggle the light color
	if (key == GLFW_KEY_O && action == GLFW_PRESS) {
		setLightColor = !setLightColor;
		setSunsetSkyBox = !setSunsetSkyBox;
	}

	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			pressedKeys[key] = true;
		}
		else if (action == GLFW_RELEASE) {
			pressedKeys[key] = false;
		}
	}

	if (pressedKeys[GLFW_KEY_F])
	{
		if (!fogOn) {
			fogOn = true;
			fogDensity = lastFogDensity;
		}
		else {
			fogOn = false;
			lastFogDensity = fogDensity;
			fogDensity = 0.0f;
		}
	}

	if (pressedKeys[GLFW_KEY_G]) {
		if (fogOn && fogDensity < 1.0f) {
			fogDensity += 0.02f;
		}
	}

	if (pressedKeys[GLFW_KEY_H]) {
		if (fogOn && fogDensity > 0.02f) {
			fogDensity -= 0.02f;
		}
	}

	if (pressedKeys[GLFW_KEY_L]) {
		animationActive = !animationActive;

		if (animationActive == true) {
			gps::Camera aux(
				glm::vec3(11.516719f, 11.220733f, -13.750375f),
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 1.0f, 0.0f));
			myCamera = aux;
			view = myCamera.getViewMatrix();
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
		}
	}

	if (pressedKeys[GLFW_KEY_N]) {
		setNight = !setNight;
	}

	if (pressedKeys[GLFW_KEY_1]) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if (pressedKeys[GLFW_KEY_2]) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	}

	if (pressedKeys[GLFW_KEY_3]) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	

}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {

	if (animationActive == false) {
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		double xoffset = xpos - lastX;
		double yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		double sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		myCamera.rotate(pitch, yaw);
		view = myCamera.getViewMatrix();
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
	}

}

void processMovement() {
	if (animationActive == false) {
		if (pressedKeys[GLFW_KEY_W]) {
			myCamera.move(gps::MOVE_FORWARD, cameraSpeed);
			//update view matrix
			view = myCamera.getViewMatrix();
			myBasicShader.useShaderProgram();
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			// compute normal matrix for mainscene
			normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
		}

		if (pressedKeys[GLFW_KEY_S]) {
			myCamera.move(gps::MOVE_BACKWARD, cameraSpeed);
			//update view matrix
			view = myCamera.getViewMatrix();
			myBasicShader.useShaderProgram();
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			// compute normal matrix for mainscene
			normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
		}

		if (pressedKeys[GLFW_KEY_A]) {
			myCamera.move(gps::MOVE_LEFT, cameraSpeed);
			//update view matrix
			view = myCamera.getViewMatrix();
			myBasicShader.useShaderProgram();
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			// compute normal matrix for mainscene
			normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
		}

		if (pressedKeys[GLFW_KEY_D]) {
			myCamera.move(gps::MOVE_RIGHT, cameraSpeed);
			//update view matrix
			view = myCamera.getViewMatrix();
			myBasicShader.useShaderProgram();
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			// compute normal matrix for mainscene
			normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
		}

		if (pressedKeys[GLFW_KEY_Q]) {
			angle -= 1.0f;
			// update model matrix for mainscene
			model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0));
			// update normal matrix for mainscene
			normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
		}

		if (pressedKeys[GLFW_KEY_E]) {
			angle += 1.0f;
			// update model matrix for mainscene
			model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0));
			// update normal matrix for mainscene
			normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
		}
	}
	else
	{
		angle += 1.0f;
		// update model matrix for mainscene
		model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0));
		// update normal matrix for mainscene
		normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
	}
}

void initOpenGLWindow() {
	myWindow.Create(1920, 1080, "Scene");
}

void setWindowCallbacks() {
	glfwSetWindowSizeCallback(myWindow.getWindow(), windowResizeCallback);
	glfwSetKeyCallback(myWindow.getWindow(), keyboardCallback);
	glfwSetCursorPosCallback(myWindow.getWindow(), mouseCallback);
}

void initOpenGLState() {
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	glViewport(0, 0, myWindow.getWindowDimensions().width, myWindow.getWindowDimensions().height);
	glEnable(GL_FRAMEBUFFER_SRGB);
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glEnable(GL_CULL_FACE); // cull face
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CCW); // GL_CCW for counter clock-wise
}

void initModels() {
	scene.LoadModel("models/materiale proiect/scene.obj", "models/materiale proiect/");
	heli.LoadModel("models/materiale proiect/heli.obj", "models/materiale proiect/");
}

void initShaders() {
	myBasicShader.loadShader("shaders/basic.vert", "shaders/basic.frag");
	myBasicShader.useShaderProgram();
	skyboxShader.loadShader("shaders/skyboxShader.vert", "shaders/skyboxShader.frag");
	skyboxShader.useShaderProgram();
}

glm::mat4 computeLightSpaceTrMatrix() {
	glm::mat4 lightView = glm::lookAt(glm::inverseTranspose(glm::mat3(lightRotation)) * lightDir, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	const GLfloat near_plane = -50.0f, far_plane = 50.0f;
	glm::mat4 lightProjection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, near_plane, far_plane);
	glm::mat4 lightSpaceTrMatrix = lightProjection * lightView;

	return lightSpaceTrMatrix;
}

void initUniforms() {

	myBasicShader.useShaderProgram();

	// create model matrix for scene
	model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	modelLoc = glGetUniformLocation(myBasicShader.shaderProgram, "model");

	// get view matrix for current camera
	view = myCamera.getViewMatrix();
	viewLoc = glGetUniformLocation(myBasicShader.shaderProgram, "view");
	// send view matrix to shader
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	// compute normal matrix for scene
	normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
	normalMatrixLoc = glGetUniformLocation(myBasicShader.shaderProgram, "normalMatrix");

	// create projection matrix
	projection = glm::perspective(glm::radians(45.0f),
		(float)myWindow.getWindowDimensions().width / (float)myWindow.getWindowDimensions().height,
		0.1f, 1000.0f);
	projectionLoc = glGetUniformLocation(myBasicShader.shaderProgram, "projection");
	// send projection matrix to shader
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//set the light direction (direction towards the light)
	lightDir = glm::vec3(0.0f, 1.0f, 1.0f);
	lightDirLoc = glGetUniformLocation(myBasicShader.shaderProgram, "lightDir");
	// send light dir to shader
	glUniform3fv(lightDirLoc, 1, glm::value_ptr(lightDir));

	//set light color
	if (setLightColor == false)
		lightColor = glm::vec3(1.0f, 1.0f, 0.9686f); //white light
	else
		lightColor = glm::vec3(1.0f, 0.459f, 0.098f); //orange light
	if (setNight == true)
		lightColor = glm::vec3(0.0f, 0.0f, 0.0f);// black light

	lightColorLoc = glGetUniformLocation(myBasicShader.shaderProgram, "lightColor");
	// send light color to shader
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

	//set fog
	fogLoc = glGetUniformLocation(myBasicShader.shaderProgram, "fogDensity");
	glUniform1f(fogLoc, fogDensity);//fog

	//set fog color
	if (setLightColor == false)
		fogColor = glm::vec4(1.0f, 1.0f, 0.784f, 1.0f);//white color
	else
		fogColor = glm::vec4(1.0f, 0.459f, 0.098f, 1.0f); //orange color
	if (setNight == true)
		fogColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);// black color


	fogColorLoc = glGetUniformLocation(myBasicShader.shaderProgram, "fogColor");
	glUniform4fv(fogColorLoc, 1, glm::value_ptr(fogColor));

	//set spotlight
	lampColor = glm::vec3(1.0f, 0.9372549019607843f, 0.9686274f); //orange light 06 02
	lampColorLoc = glGetUniformLocation(myBasicShader.shaderProgram, "lampColor");
	// send light color to shader
	glUniform3fv(lampColorLoc, 1, glm::value_ptr(lampColor));

	//send constants loc
	constantLoc = glGetUniformLocation(myBasicShader.shaderProgram, "constant");
	glUniform1f(constantLoc, constant);//constant
	linearLoc = glGetUniformLocation(myBasicShader.shaderProgram, "linear");
	glUniform1f(linearLoc, linear);//linear
	quadraticLoc = glGetUniformLocation(myBasicShader.shaderProgram, "quadratic");
	glUniform1f(quadraticLoc, quadratic);//quadratic
	positionLoc = glGetUniformLocation(myBasicShader.shaderProgram, "position");
	glUniform3fv(positionLoc, 1, glm::value_ptr(position));
	glUniformMatrix4fv(glGetUniformLocation(myBasicShader.shaderProgram, "lightSpaceTrMatrix"),
		1,
		GL_FALSE,
		glm::value_ptr(computeLightSpaceTrMatrix()));
}
float angleHeli = 15.0f;
float delta;
float movementSpeed = 2; // units per second

void updateDelta(double elapsedSeconds) {
	delta = delta + elapsedSeconds;
}

double lastTimeStamp = glfwGetTime();

void renderMainScene(gps::Shader shader) {
	// select active shader program
	shader.useShaderProgram();

	model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	//send teapot model matrix data to shader
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	//send teapot normal matrix data to shader
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	// draw scene
	scene.Draw(shader);

	// heli flying
	model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	angleHeli += 0.3f * delta;
	glm::mat4 origin = glm::translate(glm::mat4(1.0f), glm::vec3(-10.3509f, 5.97882f, -2.70866f));
	glm::mat4 rot = glm::rotate(origin, glm::radians(angleHeli), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 trans = glm::translate(rot, glm::vec3(10.3509f, -5.97882f, 2.70866f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model * trans));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	heli.Draw(shader);

}

void renderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	mySkyBox.Draw(skyboxShader, view, projection);
	//render the scene
	renderMainScene(myBasicShader);

}

void cleanup() {
	myWindow.Delete();
	//cleanup code for your own data
}

int main(int argc, const char* argv[]) {

	try {
		initOpenGLWindow();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	initOpenGLState();
	initModels();
	initSkybox();
	initShaders();
	initUniforms();
	setWindowCallbacks();
	fogDensity = 0.0f;

	glCheckError();
	// application loop
	while (!glfwWindowShouldClose(myWindow.getWindow())) {

		processMovement();
		renderScene();
		double currentTimeStamp = glfwGetTime();
		updateDelta(currentTimeStamp - lastTimeStamp);
		lastTimeStamp = currentTimeStamp;

		if (pressedKeys[GLFW_KEY_O] || pressedKeys[GLFW_KEY_F] || pressedKeys[GLFW_KEY_H] || pressedKeys[GLFW_KEY_G] || pressedKeys[GLFW_KEY_N] || pressedKeys[GLFW_KEY_M]) {
			renderScene();
			initUniforms();
			initSkybox();
		}

		glfwPollEvents();
		glfwSwapBuffers(myWindow.getWindow());

		glCheckError();
	}

	cleanup();

	return EXIT_SUCCESS;
}
