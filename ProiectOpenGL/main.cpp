#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"
#include "Model3D.hpp"
#include "Camera.hpp"
#include "SkyBox.hpp"

#include <iostream>

int glWindowWidth = 2400;
int glWindowHeight = 1000;
int retina_width, retina_height;
GLFWwindow* glWindow = NULL;

const unsigned int SHADOW_WIDTH = 3600;
const unsigned int SHADOW_HEIGHT = 3600;

// variables for computeLightSpaceTrMatrix
const GLfloat near_plane = 1.0f;
const GLfloat far_plane = 90.0f;

float margins[] = { 1.0f, 1.0f, 1.0f, 1.0f };

int x, z;
int startX = -40;
int endX = 40;
int passX = 20;
int startZ = -40;
int endZ = 40;
int passZ = 20;


glm::mat4 model;

// rose 
glm::mat4 roseModel1;
glm::mat4 roseModel2;
// swing
glm::mat4 swingModel;

// tree
glm::mat4 treeModel;

// azalea
glm::mat4 azaleaModel;

// tulip
glm::mat4 tulipModel;


// palmtree
glm::mat4 palmtreeModel;


GLuint modelLoc;
glm::mat4 view;
GLuint viewLoc;
glm::mat4 projection;
GLuint projectionLoc;
glm::mat3 normalMatrix;
GLuint normalMatrixLoc;
glm::mat4 lightRotation;

glm::vec3 lightDir;
GLuint lightDirLoc;
glm::vec3 lightColor;
GLuint lightColorLoc;

glm::vec3 cameraPos = glm::vec3(0.0f, 2.0f, 5.5f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// Starting Point
gps::Camera myCamera(glm::vec3(0.0f, 2.0f, 5.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));


// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;


float cameraSpeed = 0.5f;


bool pressedKeys[1024];
float angleY = 0.0f;
GLfloat lightAngle;


// ground
gps::Model3D ground;

//light cube
gps::Model3D lightCube;

// sun 
gps::Model3D sun;

// beagle
gps::Model3D beagle;

// duck
gps::Model3D duck;

// duck number 2
gps::Model3D ducknr2;

// turkey
gps::Model3D turkey;

// trunk
gps::Model3D trunk;

// cabin
gps::Model3D cabin;

// rose1
gps::Model3D rose;

// fence
gps::Model3D fence;

// swing
gps::Model3D swing;

// tree
gps::Model3D tree;

// azalea
gps::Model3D azalea;

// tulip
gps::Model3D tulip;

// palm tree
gps::Model3D palmtree;



gps::Shader myCustomShader;
gps::Shader lightShader;
gps::Shader depthMap;
gps::Shader skyboxShader;
gps::Shader fogShader;



gps::SkyBox mySkyBox;

GLuint shadowMapFBO;
GLuint depthMapTexture;

bool showDepthMap;


bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 400, lastY = 300;
float horizontalAngle = 3.14f;
float verticalAngle = 0.0f;

float newPos;


std::vector<const GLchar*> faces;

bool fogEnabled = false;
float fogDensity = 0.05f;

void LoadSkybox() {

	faces.push_back("skybox/nightsky_rt.tga");
	faces.push_back("skybox/nightsky_lf.tga");
	faces.push_back("skybox/nightsky_up.tga");
	faces.push_back("skybox/nightsky_dn.tga");
	faces.push_back("skybox/nightsky_bk.tga");
	faces.push_back("skybox/nightsky_ft.tga");
}	

void LoadFog() {

}


void dayTime() {
	myCustomShader.useShaderProgram();
	// set light color
	lightColor = glm::vec3(1.0f, 1.0f, 1.0f); // white light
	lightColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor");
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));
}

void nightTime() {
	myCustomShader.useShaderProgram();
	// set light color
	lightColor = glm::vec3(0.0f, 0.0f, 0.0f); // black light
	lightColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor");
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));
}

void sunsetTime() {
	myCustomShader.useShaderProgram();
	// set light color
	lightColor = glm::vec3(1.0f, 0.5f, 0.0f); // orange light
	lightColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor");
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

}

//void generateFog() {
//	fog = 1;
//	myCustomShader.useShaderProgram();
//	glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "fog"), fog);
//}


GLenum glCheckError_(const char* file, int line) {
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH)
{
	glViewport(0, 0, fbW, fbH);
	//initUniforms()
}


void processInput(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			pressedKeys[key] = true;
		else if (action == GLFW_RELEASE)
			pressedKeys[key] = false;
	}
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse) // initially set to true
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;

	if (pitch < -89.0f)
		pitch = -89.0f;

	if (!myCamera.isPresentationMode()) 
	{
		myCamera.rotate(pitch, yaw);
	}
}


void processMovement()
{
	if (pressedKeys[GLFW_KEY_Q]) {
		angleY -= 1.0f;
	}

	if (pressedKeys[GLFW_KEY_E]) {
		angleY += 1.0f;
	}

	if (pressedKeys[GLFW_KEY_J]) {
		lightAngle -= 1.0f;
	}

	if (pressedKeys[GLFW_KEY_L]) {
		lightAngle += 1.0f;
	}

	if (pressedKeys[GLFW_KEY_W]) {
		myCamera.move(gps::MOVE_FORWARD, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_S]) {
		myCamera.move(gps::MOVE_BACKWARD, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_A]) {
		myCamera.move(gps::MOVE_LEFT, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_D]) {
		myCamera.move(gps::MOVE_RIGHT, cameraSpeed);
	}


	if (pressedKeys[GLFW_KEY_I]) {
		myCamera.startPresentationMode();
	}

	if (pressedKeys[GLFW_KEY_H]) {
		myCamera.stopPresentationMode();
	}

	if (pressedKeys[GLFW_KEY_N]) {
		nightTime();
	}

	if (pressedKeys[GLFW_KEY_P]) {
		dayTime();
	}

	if (pressedKeys[GLFW_KEY_O]) {
		sunsetTime();
	}

	// polygon vertices - drawn as points
	if (pressedKeys[GLFW_KEY_C]) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	}
	// wireframe
	// boundary edges of the polygon are drawn as line segments
	if (pressedKeys[GLFW_KEY_V]) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	// solid
	// the interior of the polygon is filled
	if (pressedKeys[GLFW_KEY_B]) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (pressedKeys[GLFW_KEY_F]) {
		fogEnabled = true;

	}

	if (pressedKeys[GLFW_KEY_G]) {
		fogEnabled = false;

	}


	if (pressedKeys[GLFW_KEY_KP_SUBTRACT]) {
		if (fogDensity > 0.0f) {
			fogDensity -= 0.01f;
		}
	}

	if (pressedKeys[GLFW_KEY_KP_ADD]) {
		if (fogDensity < 1.0f) {
			fogDensity += 0.01f;
		}
	}
}

bool initOpenGLWindow()
{
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	glWindow = glfwCreateWindow(glWindowWidth, glWindowHeight, "ProjectOpenGL", NULL, NULL);
	if (!glWindow) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return false;
	}

	glfwSetWindowSizeCallback(glWindow, framebuffer_resize_callback);
	glfwMakeContextCurrent(glWindow);

	// used for rotating the scene using the mouse
	glfwSetInputMode(glWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	
	glfwSetFramebufferSizeCallback(glWindow, framebuffer_resize_callback);

	glfwSwapInterval(1);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	//for RETINA display
	glfwGetFramebufferSize(glWindow, &retina_width, &retina_height);
	glfwSetKeyCallback(glWindow, processInput);
	glfwSetCursorPosCallback(glWindow, mouse_callback);

	return true;
}

void initOpenGLState()
{
	glClearColor(0.3, 0.3, 0.3, 1.0);
	glViewport(0, 0, retina_width, retina_height);

	
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glEnable(GL_FOG);
	glClearColor(0.5f, 0.5f, 0.5f, 1);

	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glEnable(GL_CULL_FACE); // cull face
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CCW); // GL_CCW for counter clock-wise

	glEnable(GL_FRAMEBUFFER_SRGB);
}

void initObjects() {
	ground.LoadModel("objects/ground/ground.obj");
	lightCube.LoadModel("objects/cube/cube.obj");
	sun.LoadModel("objects/sun/13913_Sun_v2_l3.obj");
	beagle.LoadModel("objects/beagle/beagle.obj");
	duck.LoadModel("objects/duck/12248_Bird_v1_L2.obj");
	ducknr2.LoadModel("objects/ducknr2/12249_Bird_v1_L2.obj");
	turkey.LoadModel("objects/turkey/11560_wild_turkey_male_v2_l2.obj");
	trunk.LoadModel("objects/trunk/trunkwood.obj");
	cabin.LoadModel("objects/cabin/20958_Log_Cabin_v1_NEW.obj");
	fence.LoadModel("objects/fence/13078_Wooden_Post_and_Rail_Fence_v1_l3.obj");
	rose.LoadModel("objects/rose/rose.obj");
	swing.LoadModel("objects/woodswing/woodswing.obj");
	tree.LoadModel("objects/tree/Tree.obj");
	azalea.LoadModel("objects/azalea/10433_Azalea_Bush_v1_max2010_it2.obj");
	tulip.LoadModel("objects/tulip/12978_tulip_flower_l3.obj");
	palmtree.LoadModel("objects/palmtree/10446_Palm_Tree_v1_max2010_iteration-2.obj");

}


void initShaders() {

	myCustomShader.loadShader("shaders/shaderStart.vert", "shaders/shaderStart.frag");
	myCustomShader.useShaderProgram();
	lightShader.loadShader("shaders/lightCube.vert", "shaders/lightCube.frag");
	lightShader.useShaderProgram();
	depthMap.loadShader("shaders/depthMap.vert", "shaders/depthMap.frag");
	depthMap.useShaderProgram();

	LoadSkybox();
	mySkyBox.Load(faces);

	skyboxShader.loadShader("shaders/skyboxShader.vert", "shaders/skyboxShader.frag");
	skyboxShader.useShaderProgram();


}

void initUniforms() {

	myCustomShader.useShaderProgram();

	model = glm::mat4(1.0f);
	modelLoc = glGetUniformLocation(myCustomShader.shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	view = myCamera.getViewMatrix();
	viewLoc = glGetUniformLocation(myCustomShader.shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
	normalMatrixLoc = glGetUniformLocation(myCustomShader.shaderProgram, "normalMatrix");
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
	projectionLoc = glGetUniformLocation(myCustomShader.shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//set the light direction (direction towards the light)
	lightDir = glm::vec3(0.0f, 1.0f, 2.0f);
	lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	lightDirLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightDir");
	glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir));

	//set light color
	lightColor = glm::vec3(1.0f, 1.0f, 1.0f); //white light
	lightColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor");
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

	lightShader.useShaderProgram();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	// set fog uniforms
	glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "fogParameters.isEnabled"), fogEnabled);
	glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "fogParameters.density"), fogDensity);

}


void initFBO() {
	
	//generate FBO ID
	glGenFramebuffers(1, &shadowMapFBO);

	//create depth texture for FBO
	glGenTextures(1, &depthMapTexture);
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, margins);

	//attach texture to FBO
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


glm::mat4 computeLightSpaceTrMatrix()
{
	// Return the light-space tranformation matrix
	glm::vec3 lightDirTr = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(lightDir, 1.0f));
	glm::mat4 lightView = glm::lookAt(20.0f * lightDirTr, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));  
	glm::mat4 lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, near_plane, far_plane);

	return lightProjection * lightView;
}


void drawObjects(gps::Shader shader, bool depthPass) {

	shader.useShaderProgram();

	model = glm::rotate(glm::mat4(1.0f), glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	//// do not send the normal matrix if we are rendering in the depth map
	//if (!depthPass) {
	//	normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
	//	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	//}

	//model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	//model = glm::scale(model, glm::vec3(0.5f));
	//glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	//// do not send the normal matrix if we are rendering in the depth map
	//if (!depthPass) {
	//	normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
	//	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	//}

	for (x = startX; x < endX; x += passX) {
		for (z = startZ; z < endZ; z += passZ) {
			model = glm::translate(glm::mat4(1.0f), glm::vec3(x, 0.0f, -z));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			if (!depthPass) {
				normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
				glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
			}
			ground.Draw(shader);
		}
	}
	model = glm::translate(model, glm::vec3(-30.0f, 0.0f, 0.0f));

	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	trunk.Draw(shader);


	model = glm::scale(model, glm::vec3(0.9f));
	model = glm::translate(model, glm::vec3(-10.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	// do not send the normal matrix if we are rendering in the depth map
	if (!depthPass) {
		normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	}

	cabin.Draw(shader);


	// beagle initialization
	// model matrix for beagle
	
	model = glm::translate(model, glm::vec3(12.5f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.03f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));


	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	if (!depthPass) {

		normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	}

	beagle.Draw(shader);


	// fence
	model = glm::scale(model, glm::vec3(2.0f));
	model = glm::translate(model, glm::vec3(10.0f, -50.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	fence.Draw(shader);


	// swing
	swingModel = model;
	swingModel = glm::scale(model, glm::vec3(10.0f));
	swingModel = glm::translate(swingModel, glm::vec3(-50.0f, -50.0f, 0.0f));
	swingModel = glm::rotate(swingModel, glm::radians(-90.0f), glm::vec3(-90.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(swingModel));

	swing.Draw(shader);


	// tree

	treeModel = model;
	treeModel = glm::scale(treeModel, glm::vec3(70.0f));
	treeModel = glm::translate(treeModel, glm::vec3(0.0f, 0.3f, 0.0f));
	treeModel = glm::rotate(treeModel, glm::radians(-90.0f), glm::vec3(-90.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(treeModel));

	tree.Draw(shader);


	// azalea
	azaleaModel = model;
	azaleaModel = glm::scale(azaleaModel, glm::vec3(0.9f));
	azaleaModel = glm::translate(azaleaModel, glm::vec3(-350.0f, -30.0f, 0.0f));
	//azaleaModel = glm::rotate(azaleaModel, glm::radians(-90.0f), glm::vec3(-90.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(azaleaModel));

	azalea.Draw(shader);


	// tulip
	for (int j = -50; j < 270; j += 30) {
		for (int k = -250; k < -150; k += 30) {
			tulipModel = model;
			tulipModel = glm::scale(tulipModel, glm::vec3(2.0f));
			tulipModel = glm::translate(tulipModel, glm::vec3(j, k, 0.0f));
			tulipModel = glm::rotate(tulipModel, glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(tulipModel));

			tulip.Draw(shader);
		}
	}


	// palm tree

	palmtreeModel = model;
	palmtreeModel = glm::scale(palmtreeModel, glm::vec3(0.3f));
	palmtreeModel = glm::translate(palmtreeModel, glm::vec3(-170.0f, -3000.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(palmtreeModel));
	palmtree.Draw(shader);

	// rose1

	//roseModel1 = model;
	//roseModel1 = glm::scale(roseModel1, glm::vec3(0.5f));
	//roseModel1 = glm::translate(roseModel1, glm::vec3(-50.0f, -50.0f, 0.0f));
	//roseModel1 = glm::rotate(roseModel1, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(roseModel1));

	//rose.Draw(shader);

	//roseModel2 = model;
	//roseModel2 = glm::scale(roseModel2, glm::vec3(0.5f));
	//roseModel2 = glm::translate(roseModel2, glm::vec3(-35.0f, -50.0f, 0.0f));
	//roseModel2 = glm::rotate(roseModel2, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(roseModel2));


	//rose2.Draw(shader);

	for (int i = -50; i < 420 ; i += 30) 
	{
		roseModel1 = model;
		roseModel1 = glm::scale(roseModel1, glm::vec3(0.5f));
		roseModel1 = glm::translate(roseModel1, glm::vec3(i, -50.0f, 0.0f));
		roseModel1 = glm::rotate(roseModel1, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(roseModel1));

		rose.Draw(shader);
	
	}


	// duck initialization

	model = glm::translate(model, glm::vec3(-70.0f, -190.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.6f));
	//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	duck.Draw(shader);

	// duck number 2 initialization
	model = glm::scale(model, glm::vec3(0.8f));
	model = glm::translate(model, glm::vec3(-55.0f, 230.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	ducknr2.Draw(shader);


	// turkey
    model = glm::scale(model, glm::vec3(0.7f));
	model = glm::translate(model, glm::vec3(-55.0f, -60.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	turkey.Draw(shader);


}


void renderScene() {

	// first pass: render the scene to the depth buffer
	depthMap.useShaderProgram();

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	glUniformMatrix4fv(glGetUniformLocation(depthMap.shaderProgram, "lightSpaceTrMatrix"), 1, GL_FALSE, glm::value_ptr(computeLightSpaceTrMatrix()));


	drawObjects(depthMap, 1);
	//drawBeagle(depthMapShader, 1);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//// Fog parameters
	//bool FogFlag = true;
	//float FogCol[3] = { 0.8f,0.8f,0.8f };  // define a nice light grey

	//// Turn On Fog
	//glEnable(GL_FOG);
	//glFogfv(GL_FOG_COLOR, FogCol); // Set the fog color
	//glFogf(GL_FOG_DENSITY, 0.1f);  // Thin the fog out a little


	if (showDepthMap) {
		glViewport(0, 0, retina_width, retina_height);

		glClear(GL_COLOR_BUFFER_BIT);

		//bind the depth map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMapTexture);

		
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_DEPTH_TEST);
	}
	else {

		// final scene rendering pass (with shadows)

		glViewport(0, 0, retina_width, retina_height);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		myCustomShader.useShaderProgram();

		view = myCamera.getViewMatrix();
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir));

		//bind the shadow map
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, depthMapTexture);
		glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "shadowMap"), 3);

		glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "lightSpaceTrMatrix"),
			1,
			GL_FALSE,
			glm::value_ptr(computeLightSpaceTrMatrix()));

		// update fog uniforms
		glm::vec3 fogColor = glm::vec3(0.5f, 0.5f, 0.5f);

		glUniform3fv(glGetUniformLocation(myCustomShader.shaderProgram, "fogParameters.color"), 1, glm::value_ptr(fogColor));
		glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "fogParameters.isEnabled"), fogEnabled);
		glUniform1f(glGetUniformLocation(myCustomShader.shaderProgram, "fogParameters.density"), fogDensity);

		//draw a white sun around the light

		lightShader.useShaderProgram();

		glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

		model = lightRotation;
		model = glm::translate(model, 1.0f * lightDir);
		model = glm::translate(model, glm::vec3(0.0f, 20.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.0005f, 0.0005f, 0.0005f));
		glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));


		sun.Draw(lightShader);

		drawObjects(myCustomShader, false);
	}

	//// Fog On or Off
	//if (FogFlag)
	//	glEnable(GL_FOG);
	//else
	//	glDisable(GL_FOG);

	mySkyBox.Draw(skyboxShader, view, projection);

}

void cleanup() {
	glDeleteTextures(1, &depthMapTexture);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &shadowMapFBO);
	glfwDestroyWindow(glWindow);
	//close GL context and any other GLFW resources
	glfwTerminate();
}


int main(int argc, const char* argv[]) {

	if (!initOpenGLWindow()) {
		glfwTerminate();
		return 1;
	}

	initOpenGLState();
	initObjects();
	initShaders();
	initUniforms();
	initFBO();

	glCheckError();


	while (!glfwWindowShouldClose(glWindow)) {
		processMovement();
		renderScene();

		glfwPollEvents();
		glfwSwapBuffers(glWindow);
	}


	cleanup();

	return 0;
}
