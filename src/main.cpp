// Core libraries
#include <iostream>
#include <cmath>
#include <limits>

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
#include "Player.h"
#include "Water.h"
#include "Fire.h"
#include "Healthbar.h"
#include "Waterbar.h"
#include "Background.h"
#include "Hydrant.h"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

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

    Player* player;

    std::vector<Platform> platforms;
	std::vector<Water> waterDroplets;
    std::vector<Fire> fires;
    std::vector<Hydrant> hydrants;
    std::shared_ptr<Program> crosshairShader, waterShader, fireShader, playerShader, healthbarShader, imageShader;

    Healthbar *healthbar;
    Waterbar *waterbar;

    Background *background;
    
    double gametime = 0;
    double time_since_waterdrop = 0;
    bool wireframeEnabled = false;
    bool mousePressed = false;
    bool mouseCaptured = false;

    bool jetPack = false;

	double xPosMouse, yPosMouse = 0.0;
    double raw_xPosMouse, raw_yPosMouse = 0.0;

	GLuint vbo, vao;

	GLuint waterTexture;

	std::shared_ptr<Shape> raindrop;

    Application() {
        camera = new Camera();
    }
    
    ~Application() {
        delete camera;
    }

    void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_A && action == GLFW_PRESS) {player->left = true;}
        if (key == GLFW_KEY_A && action == GLFW_RELEASE) {player->left = false;}

        if (key == GLFW_KEY_D && action == GLFW_PRESS) {player->right = true;}
        if (key == GLFW_KEY_D && action == GLFW_RELEASE) {player->right = false;}

		if (key == GLFW_KEY_W && action == GLFW_PRESS) { player->jump = true; player->justJumped = true; }
        if (key == GLFW_KEY_W && action == GLFW_RELEASE) {player->jump = false;}

        if (key == GLFW_KEY_G && action == GLFW_PRESS) {jetPack = !jetPack;}

		if (key == GLFW_KEY_R && action == GLFW_PRESS) { 
            player->reset = true;
            camera->pos = glm::vec2(0.f);
        }
		if (key == GLFW_KEY_R && action == GLFW_RELEASE) { player->reset = false; }

		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) { addWaterDrops(); }
    }

    void mouseCallback(GLFWwindow *window, int button, int action, int mods) {
        mousePressed = (action != GLFW_RELEASE);
    }

	void addWaterDrops() {
		waterDroplets.push_back(Water(player->pos, vec2(xPosMouse, yPosMouse), player->vel, raindrop, platforms));
	}
    
    void mouseMoveCallback(GLFWwindow *window, double xpos, double ypos) {
		raw_xPosMouse = xpos;
		raw_yPosMouse = windowManager->getHeight() - ypos;
    }

    void resizeCallback(GLFWwindow *window, int in_width, int in_height) {
        background = new Background(windowManager->getWidth(), windowManager->getHeight());
    }

    void initGeom(const std::string& resourceDirectory) {
		//Create Platforms
        platforms.push_back(Platform(0, windowManager->getWidth(), 0, windowManager->getHeight()/2.f));
 		
		//Create Player
        player = new Player(100, windowManager->getHeight()/2.f + 200);
		player->platforms = platforms;
		player->loadAnimations(resourceDirectory + "/Animations");

		//Load Raindrop
		raindrop = std::make_shared<Shape>();
		raindrop->loadMesh(resourceDirectory + "/raindrop.obj");
		raindrop->resize();
		raindrop->init();

        fires.push_back(Fire(1000, windowManager->getHeight()/2.f));
		fires.push_back(Fire(1500, windowManager->getHeight()/2.f));

        hydrants.push_back(Hydrant(glm::vec2(windowManager->getWidth()/2.f, windowManager->getHeight()/2.f + 50)));

        healthbar = new Healthbar(windowManager->getWidth() - 100);

        waterbar = new Waterbar(windowManager->getWidth() - 100);

        background = new Background(windowManager->getWidth(), windowManager->getHeight());

		//Vertices for crosshair
		GLfloat vertices_position[24] = {
			0.0, 0.0,
			0.5, 0.1,
			0.5, -0.1,

			0.0, 0.0,
			0.1, 0.5,
			-0.1, 0.5,

			0.0, 0.0,
			-0.5, 0.1,
			-0.5, -0.1,

			0.0, 0.0,
			0.1, -0.5,
			-0.1, -0.5,
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
        imageShader = std::make_shared<Program>();
        imageShader->setShaderNames(resourceDirectory + "/image.vert", resourceDirectory + "/image.frag");
        imageShader->init();

		crosshairShader = std::make_shared<Program>();
		crosshairShader->setShaderNames(resourceDirectory + "/crosshair.vert", resourceDirectory + "/crosshair.frag");
		crosshairShader->init();

		waterShader = std::make_shared<Program>();
		waterShader->setShaderNames(resourceDirectory + "/water.vert", resourceDirectory + "/water.frag");
		waterShader->init();


        fireShader = std::make_shared<Program>();
        fireShader->setShaderNames(resourceDirectory + "/fire.vert", resourceDirectory + "/fire.frag");
        fireShader->init();

		playerShader = std::make_shared<Program>();
		playerShader->setShaderNames(resourceDirectory + "/player.vert", resourceDirectory + "/player.frag");
		playerShader->init();
		playerShader->addUniform("Manim");

        healthbarShader = std::make_shared<Program>();
        healthbarShader->setShaderNames(resourceDirectory + "/healthbar.vert", resourceDirectory + "/healthbar.frag");
        healthbarShader->init();
    }   
    
    glm::mat4 getPerspectiveMatrix() {
        float width = windowManager->getWidth();
        float height = windowManager->getHeight();
        return glm::ortho(0.f, width, 0.f, height, -10000.f, 10000.f);
    }

    void update() {
        double frametime = get_last_elapsed_time();
        gametime += frametime;
        time_since_waterdrop += frametime;

        camera->update(player->pos, player->vel, windowManager->getWidth());

        xPosMouse = raw_xPosMouse - camera->pos.x;
        yPosMouse = raw_yPosMouse - camera->pos.y;

        background->update(camera->pos);

        player->update(frametime);

        if (mousePressed && player->water > 0.f) {
            if (time_since_waterdrop > (1.f / 40.f)) {
                addWaterDrops();
                time_since_waterdrop = 0;
                player->water -= 0.2f;
                if (jetPack) {
                    player->water -= 0.2f;
                }
            }

            if (jetPack) {
                player->addWaterVelocity(xPosMouse, yPosMouse, frametime);
            }
        }

		for (unsigned int i = 0; i < waterDroplets.size(); i++) {
			waterDroplets[i].update(frametime);
            if (waterDroplets[i].shouldRemove) {
                waterDroplets.erase(waterDroplets.begin() + i);
                continue;
            }
		}

        for (unsigned int i = 0; i < fires.size(); i++) {
            fires[i].update(frametime, waterDroplets);
            fires[i].checkPlayerDamage(player);
            if (fires[i].size < 20.f) {
                fires.erase(fires.begin() + i);
                continue;
            }
        }

        for (unsigned int i = 0; i < hydrants.size(); i++) {
            hydrants[i].update(player);
        }

		player->updatePlayerAnimation(frametime);
        healthbar->update(player->health);
        waterbar->update(player->water);
    }

    void render() {
        // Clear framebuffer.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Create the matrix stacks.
        glm::mat4 V, V_flat, V_nahh, M, P;
        P = getPerspectiveMatrix();
        V = camera->getViewMatrix();
        V_flat = camera->getFlatViewMatrix();
        V_nahh = glm::mat4(1);
        M = glm::mat4(1);

		//Draw PLAYER SKELETON

		playerShader->bind();
		playerShader->setMVP(&M[0][0], &V[0][0], &P[0][0]);
		player->draw(playerShader, false);

		playerShader->unbind();
        
        /**************/
        /* DRAW SHAPE */
        /**************/
        
        glDisable(GL_BLEND);

        imageShader->bind();
        imageShader->setMVP(&M[0][0], &V[0][0], &P[0][0]);

        // Draw platforms
        for (unsigned int i = 0; i < platforms.size(); i++) {
            platforms[i].draw(imageShader);
        }
        background->draw(imageShader);
		imageShader->unbind();

		waterShader->bind();
		waterShader->setMVP(&M[0][0], &V[0][0], &P[0][0]);
		// Draw Water Droplets
		for (unsigned int i = 0; i < waterDroplets.size(); i++) {
			if (waterDroplets[i].pos.y < 0) {
				//Get rid of the ones we can't see!!!!!
				waterDroplets.erase(waterDroplets.begin() + i);
				continue;
			}
			waterDroplets[i].draw(waterShader);
		}
		waterShader->unbind();


        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        imageShader->bind();
        imageShader->setMVP(&M[0][0], &V[0][0], &P[0][0]);
        for (unsigned int i = 0; i < hydrants.size(); i++) {
            hydrants[i].draw(imageShader);
        }
        imageShader->unbind();

        fireShader->bind();
        fireShader->setMVP(&M[0][0], &V[0][0], &P[0][0]);

        for (unsigned int i = 0; i < fires.size(); i++) {
            fires[i].draw(fireShader);
        }

        fireShader->unbind();

        healthbarShader->bind();
        healthbarShader->setMVP(&M[0][0], &V_nahh[0][0], &P[0][0]);
        healthbar->draw(healthbarShader);

        waterbar->draw(healthbarShader);
        healthbarShader->unbind();

		crosshairShader->bind();
		M = glm::translate(glm::mat4(1), glm::vec3(xPosMouse + camera->pos.x, yPosMouse + camera->pos.y, 99.f));
		M = glm::scale(M, glm::vec3(50, 50, 1));
		crosshairShader->setMVP(&M[0][0], &V_flat[0][0], &P[0][0]);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 12);
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

	glfwSetInputMode(application->windowManager->getHandle(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    
    // Loop until the user closes the window.
    while (!glfwWindowShouldClose(windowManager->getHandle())) {

        application->update();
        // Render scene.
        application->render();

        // Swap front and back buffers.
        glfwSwapBuffers(windowManager->getHandle());
        // Poll for and process events.
        glfwPollEvents();

        if (application->player->health <= 0) {
            // You ded
            std::cout << "You died! Try again" << std::endl;
            windowManager->shutdown();
            return 0;
        }
    }

    // Quit program.
    windowManager->shutdown();
    return 0;
}