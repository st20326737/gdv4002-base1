#include "Engine.h"

class MyObject : public GameObject2D {

public:
	float	 thetaVelocity = (3.141593f / 180.0f) * 90.0f;

	MyObject(glm::vec2 initPosition, float initOrientation, glm::vec2 initSize, GLuint initTextureID) : GameObject2D(initPosition, initOrientation, initSize, initTextureID) {
	}

	void update(double tDelta) {

		orientation = orientation + (thetaVelocity * tDelta);
	}
};


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

	//
	// Setup game scene objects here
	//

	GameObject2D* player1 = addObject("player", glm::vec2(-2.0f, 0.0f), 0.0f, glm::vec2(1.5f, 1.5f), "Resources\\Textures\\player1_ship.png", TextureProperties::NearestFilterTexture());
	
	MyObject* player2 = new MyObject(glm::vec2(0.0f, 0.0f), 0.0f, glm::vec2(0.25f, 0.25f), 0);
	addObject("player", player2);

	//GameObject2D* player2 = addObject("player");
	//player2->size = glm::vec2(0.25f, 0.25f);

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

	GameObjectCollection objects = getObjectCollection("player");

	for (int i = 0; i < objects.objectCount; i++) {

		GameObject2D* player = objects.objectArray[i];
		player->orientation = player->orientation + (thetaVelocity * tDelta);
	}
}


