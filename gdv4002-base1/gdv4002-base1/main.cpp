#include "Engine.h"

// Function prototypes

// We'll handle our own updates so declare function to do this
void myUpdateScene(GLFWwindow* window, double tDelta);


int main(void) {

	// Initialise the engine (create window, setup OpenGL backend)
	int initResult = engineInit("GDV4002 - Applied Maths for Games", 1024, 1024);

	// If the engine initialisation failed report error and exit
	if (initResult != 0) {

		printf("Cannot setup game window!!!\n");
		return initResult; // exit if setup failed
	}

	// Test object setup
	GameObject2D* player1 = addObject("player", glm::vec2(-2.0f, 0.0f), 0.0f, glm::vec2(1.5f, 1.5f), "Resources\\Textures\\player1_ship.png", TextureProperties::NearestFilterTexture());
	GameObject2D* player2 = addObject("player");

	// Register our own update function
	setUpdateFunction(myUpdateScene);

	// Enter main loop - this handles update and render calls
	engineMainLoop();

	// When we quit (close window for example), clean up engine resources
	engineShutdown();

	// return success :)
	return 0;
}


// Main update function - this is called every iteration of the game loop (run inside the engineMainLoop called above).  We get a reference to the main game window and the amount of time elapsed since the last frame as parameters.
void myUpdateScene(GLFWwindow* window, double tDelta) {

	const float pi = 3.141593f;
	const float thetaVelocity = (pi / 180.0f) * 90.0f; // angle change per second (90 degrees)

	GameObject2D* player1 = getObject("player");

	//player1->position.x = cosf(theta);
	//player1->position.y = sinf(theta);
	player1->orientation = player1->orientation + (thetaVelocity * tDelta);

}


