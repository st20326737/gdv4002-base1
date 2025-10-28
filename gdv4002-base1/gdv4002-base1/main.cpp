#include "Engine.h"

//global veriables
const float pi = 3.14159265359f;
const float maxSpeed = 200.0f;
const float maxTurn = 250.0f;

static bool aPressed = false;//left
static bool dPressed = false;//right
static bool wPressed = false;//forward
static bool sPressed = false;//backward

float turnVelocity = 0.0f;//- = left, + = right
float turnAcceleration = 5.0f; // degrees per second squared
float forwardVelocity = 0.0f;//- = back, + = front
float forwardAcceleration = 0.5f; // degrees per second squared
float dx = 0.0f;
float dy = 0.0f;

// Function prototypes
void myUpdateScene(GLFWwindow* window, double tDelta);
void myKeyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods);
void keepOnScreen(float viewWidth, float viewHight, GameObject2D*);


int main(void) 
{
	const float pi = 3.14159265359f;

	// Initialise the engine (create window, setup OpenGL backend)
	int initResult = engineInit("GDV4002 - Applied Maths for Games", 1024, 1024, 100);

	// If the engine initialisation failed report error and exit
	if (initResult != 0) 
	{

		printf("Cannot setup game window!!!\n");
		return initResult; // exit if setup failed
	}

	//
	// Setup game scene objects here
	//
	
	addObject("player", glm::vec2(0.0f, 0.0f), glm::radians(90.0f), glm::vec2(5.0f, 5.0f), "Resources\\Textures\\player1_ship.png", TextureProperties::NearestFilterTexture());
	setUpdateFunction(myUpdateScene);

	setKeyboardHandler(myKeyboardHandler);

	// Enter main loop - this handles update and render calls
	engineMainLoop();

	// When we quit (close window for example), clean up engine resources
	engineShutdown();

	// return success :)
	return 0;
}


void myUpdateScene(GLFWwindow* window, double tDelta) 
{
	GameObject2D* player1 = getObject("player");
	
	// Update game objects here
	keepOnScreen(getViewplaneWidth()/2.0f, getViewplaneHeight()/2.0f, player1);

	// Update player rotation based on key presses
	float thetaVelocity; // radians per second
	

	
	if (aPressed) 
	{
		turnVelocity += turnAcceleration;
	}
	if (dPressed) 
	{
		turnVelocity -= turnAcceleration;
	}

	if (fabs(turnVelocity) > maxTurn)
	{
		turnVelocity = (turnVelocity / fabs(turnVelocity)) * maxTurn;
	}

	thetaVelocity = (pi / 180.0f) * turnVelocity;

	player1->orientation += thetaVelocity * (float)tDelta;

	// Update player speed based on key presses
	if (wPressed)
	{
		forwardVelocity += forwardAcceleration;
	}
	if (sPressed)
	{
		forwardVelocity -= forwardAcceleration;
	}

	if (fabs(forwardVelocity) > maxSpeed)
	{
		forwardVelocity = (forwardVelocity / fabs(forwardVelocity)) * maxSpeed;//scale to max speed with sign
	}

	dx = forwardVelocity * cos(player1->orientation) * (float)tDelta;
	dy = forwardVelocity * sin(player1->orientation) * (float)tDelta;



	player1->position.x += dx;
	player1->position.y += dy;

}


void myKeyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check if a key is pressed
	if (action == GLFW_PRESS) 
	{

		// check which key was pressed...
		switch (key)
		{
		case GLFW_KEY_A:
			// If escape is pressed tell GLFW we want to close the window (and quit)
			aPressed = true;
			break;

		case GLFW_KEY_D:
			// If escape is pressed tell GLFW we want to close the window (and quit)
			dPressed = true;
			break;

		case GLFW_KEY_W:
			// If escape is pressed tell GLFW we want to close the window (and quit)
			wPressed = true;
			break;

		case GLFW_KEY_S:
			// If escape is pressed tell GLFW we want to close the window (and quit)
			sPressed = true;
			break;

		default:
		{
		}
		}
	}
	// If not check a key has been released
	else if (action == GLFW_RELEASE) 
	{

		// handle key release events
		switch (key)
		{
		case GLFW_KEY_A:
			// If escape is pressed tell GLFW we want to close the window (and quit)
			aPressed = false;
			break;

		case GLFW_KEY_D:
			// If escape is pressed tell GLFW we want to close the window (and quit)
			dPressed = false;
			break;

		case GLFW_KEY_W:
			// If escape is pressed tell GLFW we want to close the window (and quit)
			wPressed = false;
			break;

		case GLFW_KEY_S:
			// If escape is pressed tell GLFW we want to close the window (and quit)
			sPressed = false;
			break;
		default:
		{
		}
		}
	}
}

void keepOnScreen(float viewWidth, float viewHight, GameObject2D* player1)
{
	if (player1->position.x < viewWidth && player1->position.x > -viewWidth)
	{
		//do nothing
	}
	else
	{
		player1->position.x *= -1.0f;
	}


	if (player1->position.y < viewHight && player1->position.y > -viewHight)
	{
		//do nothing
	}
	else
	{
		player1->position.y *= -1.0f;
	}
}