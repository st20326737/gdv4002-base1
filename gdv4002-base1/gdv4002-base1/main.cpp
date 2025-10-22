#include "Engine.h"

// Function prototypes
void myUpdateScene(GLFWwindow* window, double tDelta);


int main(void) {

	int initResult = engineInit("GDV4002 - Applied Maths for Games", 1024, 1024);

	if (initResult != 0)
		return initResult; // exit if setup failed

	// Test object setup
	GameObject2D* player1 = addObject("player", glm::vec2(-2.0f, 0.0f), 0.0f, glm::vec2(1.5f, 1.5f), "Resources\\Textures\\player1_ship.png", TextureProperties::NearestFilterTexture());
	GameObject2D* player2 = addObject("player", glm::vec2(2.0f, 1.0f), 0.0f, glm::vec2(0.25f, 0.25f), "Resources\\Textures\\bumblebee.png");

	listObjectCounts();
	listGameObjectKeys();

	setUpdateFunction(myUpdateScene);

	engineMainLoop();

	engineShutdown();

	return 0;
}


void myUpdateScene(GLFWwindow* window, double tDelta) {

	static float theta = 0.0f; // angle - in radians

	const float pi = 3.141593f;
	const float thetaVelocity = (pi / 180.0f) * 90.0f; // angle change per second (90 degrees)

	GameObject2D* player1 = getObject("player");

	//player1->position.x = cosf(theta);
	//player1->position.y = sinf(theta);
	player1->orientation = theta;

	theta += thetaVelocity * tDelta;
}


