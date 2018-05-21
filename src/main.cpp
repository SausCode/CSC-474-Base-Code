// Core libraries
#include <iostream>
#include <cmath>

// Third party libraries
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Local headers
#include "GLSL.h"
#include "Program.h"
#include "WindowManager.h"
#include "Shape.h"
#include "Camera.h"
#include "Platform.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

using namespace std;
using namespace glm;

double get_last_elapsed_time() {
    static double lasttime = glfwGetTime();
    double actualtime = glfwGetTime();
    double difference = actualtime - lasttime;
    lasttime = actualtime;
    return difference;
}

class Application : public EventCallbacks {
public:
    WindowManager *windowManager = nullptr;
    Camera *camera = nullptr;

    Platform* temp;
    Platform* temp2;
    //std::shared_ptr<Shape> shape;
    std::shared_ptr<Program> phongShader, crosshairShader;
    
    double gametime = 0;
    bool wireframeEnabled = false;
    bool mousePressed = false;
    bool mouseCaptured = false;

	double xPosMouse, yPosMouse = 0.0;

	GLuint vbo, vao;

    Application() {
        camera = new Camera();
    }
    
    ~Application() {
        delete camera;
    }

    void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        // Movement
        /*
        if (key == GLFW_KEY_W && action != GLFW_REPEAT) camera->vel.z = (action == GLFW_PRESS) * -0.2f;
        if (key == GLFW_KEY_S && action != GLFW_REPEAT) camera->vel.z = (action == GLFW_PRESS) * 0.2f; */
        if (key == GLFW_KEY_A && action != GLFW_REPEAT) camera->vel.x = (action == GLFW_PRESS) * 2.f;
        if (key == GLFW_KEY_D && action != GLFW_REPEAT) camera->vel.x = (action == GLFW_PRESS) * -2.f; /*
        // Rotation
        if (key == GLFW_KEY_I && action != GLFW_REPEAT) camera->rotVel.x = (action == GLFW_PRESS) * 0.02f;
        if (key == GLFW_KEY_K && action != GLFW_REPEAT) camera->rotVel.x = (action == GLFW_PRESS) * -0.02f;
        if (key == GLFW_KEY_J && action != GLFW_REPEAT) camera->rotVel.y = (action == GLFW_PRESS) * 0.02f;
        if (key == GLFW_KEY_L && action != GLFW_REPEAT) camera->rotVel.y = (action == GLFW_PRESS) * -0.02f;
        if (key == GLFW_KEY_U && action != GLFW_REPEAT) camera->rotVel.z = (action == GLFW_PRESS) * 0.02f;
        if (key == GLFW_KEY_O && action != GLFW_REPEAT) camera->rotVel.z = (action == GLFW_PRESS) * -0.02f;
        // Polygon mode (wireframe vs solid)
        if (key == GLFW_KEY_P && action == GLFW_PRESS) {
            wireframeEnabled = !wireframeEnabled;
            glPolygonMode(GL_FRONT_AND_BACK, wireframeEnabled ? GL_LINE : GL_FILL);
        }
        // Hide cursor (allows unlimited scrolling)
        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
            mouseCaptured = !mouseCaptured;
            glfwSetInputMode(window, GLFW_CURSOR, mouseCaptured ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
            resetMouseMoveInitialValues(window);
        } */
}

    void mouseCallback(GLFWwindow *window, int button, int action, int mods) {
        mousePressed = (action != GLFW_RELEASE);
        if (action == GLFW_PRESS) {
            resetMouseMoveInitialValues(window);
        }
    }
    
    void mouseMoveCallback(GLFWwindow *window, double xpos, double ypos) {
		//convertMousePosition(xpos, ypos);
		xPosMouse = xpos - camera->pos.x;
		yPosMouse = windowManager->getHeight() - ypos - camera->pos.y;

		std::cout << "Camera x" << camera->pos.x << std::endl;
        if (mousePressed || mouseCaptured) {
            //float yAngle = (xpos - mouseMoveOrigin.x) / windowManager->getWidth() * 3.14159f;
            //float xAngle = (ypos - mouseMoveOrigin.y) / windowManager->getHeight() * 3.14159f;
            //camera->setRotation(mouseMoveInitialCameraRot + glm::vec3(-xAngle, -yAngle, 0));
        }
    }

    void resizeCallback(GLFWwindow *window, int in_width, int in_height) { }
    
    // Reset mouse move initial position and rotation
    void resetMouseMoveInitialValues(GLFWwindow *window) {
        //double mouseX, mouseY;
        //glfwGetCursorPos(window, &mouseX, &mouseY);
        //mouseMoveOrigin = glm::vec2(mouseX, mouseY);
        //mouseMoveInitialCameraRot = camera->rot;
    }

    void initGeom(const std::string& resourceDirectory) {
 		temp = new Platform(0, windowManager->getWidth(), 0, windowManager->getHeight()/2.f);
 		temp2 = new Platform(windowManager->getWidth(), 2*windowManager->getWidth(), 0, windowManager->getHeight()/2.f);

		//Vertices for crosshair
		//TODO: Maybe an obj, or something fancier for a nice crosshair
		GLfloat vertices_position[24] = {
			0.0, 0.0,
			0.5, 0.0,
			0.5, 0.5,

			0.0, 0.0,
			0.0, 0.5,
			-0.5, 0.5,

			0.0, 0.0,
			-0.5, 0.0,
			-0.5, -0.5,

			0.0, 0.0,
			0.0, -0.5,
			0.5, -0.5,
		};

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_position), vertices_position, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glEnableVertexAttribArray(0);
    }
    
    void init(const std::string& resourceDirectory) {
        GLSL::checkVersion();

        // Enable z-buffer test.
        glEnable(GL_DEPTH_TEST);
        
        // Initialize the GLSL programs
        phongShader = std::make_shared<Program>();
        phongShader->setShaderNames(resourceDirectory + "/phong.vert", resourceDirectory + "/phong.frag");
        phongShader->init();

		crosshairShader = std::make_shared<Program>();
		crosshairShader->setShaderNames(resourceDirectory + "/crosshair.vert", resourceDirectory + "/crosshair.frag");
		crosshairShader->init();

    }
    
    glm::mat4 getPerspectiveMatrix() {
        float width = windowManager->getWidth();
        float height = windowManager->getHeight();
        return glm::ortho(0.f, width, 0.f, height, 0.01f, 100.f);
    }

    void render() {
        double frametime = get_last_elapsed_time();
        gametime += frametime;

        // Clear framebuffer.
        glClearColor(0.3f, 0.7f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Create the matrix stacks.
        glm::mat4 V, M, P;
        P = getPerspectiveMatrix();
        V = camera->getViewMatrix();
        M = glm::mat4(1);
        
        /**************/
        /* DRAW SHAPE */
        /**************/
        M = glm::translate(glm::mat4(1), glm::vec3(0, 0, -3));

        phongShader->bind();
        phongShader->setMVP(&M[0][0], &V[0][0], &P[0][0]);
        //shape->draw(phongShader, false);
        temp->draw(phongShader, false);
        temp2->draw(phongShader, false);

        phongShader->unbind();

		crosshairShader->bind();

		
		M = glm::translate(glm::mat4(1), glm::vec3(xPosMouse, yPosMouse, -3.f));
		M = glm::scale(M, glm::vec3(100, 100, 1));
		crosshairShader->setMVP(&M[0][0], &V[0][0], &P[0][0]);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 12);
		//shape->draw(crosshairShader, false);
		crosshairShader->unbind();
    }
};

int main(int argc, char **argv) {
    std::string resourceDir = "../resources";
    if (argc >= 2) {
        resourceDir = argv[1];
    }

    Application *application = new Application();

    // Initialize window.
    WindowManager * windowManager = new WindowManager();
    windowManager->init(WINDOW_WIDTH, WINDOW_HEIGHT);
    windowManager->setEventCallbacks(application);
    application->windowManager = windowManager;

    // Initialize scene.
    application->init(resourceDir);
    application->initGeom(resourceDir);
    
    // Loop until the user closes the window.
    while (!glfwWindowShouldClose(windowManager->getHandle())) {

        // Update camera position.
        application->camera->update();
        // Render scene.
        application->render();

        // Swap front and back buffers.
        glfwSwapBuffers(windowManager->getHandle());
        // Poll for and process events.
        glfwPollEvents();
    }

    // Quit program.
    windowManager->shutdown();
    return 0;
}
