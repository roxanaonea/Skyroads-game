#include "Tema2.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;
bool ortho = false;
bool persp = false;
float o_left, o_right, o_bottom, o_top, o_zNear, o_zFar;
float p_fovy, p_aspect, p_zNear, p_zFar;

float speed = 3.0f;

bool jump = false;
bool positive = true;
float jumpSpeed = 2.5;
bool falling = false;
float fallSpeed = 3;

float dist = -9;

glm::vec3 violet_color = glm::vec3(0.25, 0, 0.25); //rgb(128,0,128)
glm::vec3 blue_color = glm::vec3(0, 0, 0.75);
glm::vec3 red_color = glm::vec3(0.5, 0, 0);
glm::vec3 yellow_color = glm::vec3(0.75, 0.75, 0); //rgb(255,255,0)
glm::vec3 orange_color = glm::vec3(0.75, 0.25, 0);//rgb(255,140,0)
glm::vec3 green_color = glm::vec3(0, 0.5, 0);
vector<glm::vec3> colors;
int min_color = 0, max_color = 6;
glm::vec3 white_color = glm::vec3(1, 1, 1);
glm::vec3 gray_color = glm::vec3(0.5, 0.5, 0.5);

vector<Platform::Platform*> platforms;
bool initial_state = true;
int current_platforms = 0;
int max_lengthZ = 8;
int min_lengthZ = 5;

vector<int> randomPlatforms;
vector<int> old_randomPlatforms;
int numberPlatforms;
int old_numberPlatforms;
int platformGroups = 0;

int minSpeed = 2;
int maxSpeed = 8;
bool gameOver = false;
bool fullSpeed = false;
float endTimeOrange = 0;
float startTimeOrange = 0;
float timeOrange = 0;

float fuelPlatformYellow = false;
float fuelPlatformGreen = false;
int fuelPlatformIndex;

bool toggleCamera = false;
glm::vec3 position_camera;
glm::vec3 center_camera;
glm::vec3 up_camera;

float initialZ = 0;
vector<FuelBar::FuelBar*> lives;
int numberLives;
bool modify_lives = false;
int livesPlatformIndex;

Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}

// Check if the element is one of the platforms(old_randomPlatforms or randomPlatforms)
bool containsElement(vector<int> platforms, int element, int numberPlatforms) {
	for (int i = 0; i < numberPlatforms; i++) {
		if (platforms[i] == element) {
			return true;
		}
	}
	return false;
}

// Check on which side is the player -> 0 for left; 1 for center; 2 for right and -1 if the player is between sides
int DirectionPlatform(Player::Player* player) {
	if (player->translateX > -2.5 && player->translateX < -1.5)
		return 0;
	if (player->translateX > -0.5 && player->translateX < 0.5)
		return 1;
	if (player->translateX > 1.5 && player->translateX < 2.5)
		return 2;
	return -1;
}

// Check if the player is on the left platform
bool OnLeftPlatform(Player::Player* player, int i) {
	bool condition1 = false, condition2 = false;
	if (i == 0 || i == 3) {
		if (platforms[i]->translateZ < 0 && platforms[i]->translateZ + platforms[i]->scaleZ / 2 > 0) {
			condition1 = true;
		}
		else if (platforms[i]->translateZ > 0 && platforms[i]->translateZ - platforms[i]->scaleZ / 2 < 0) {
			condition1 = true;
		}
		if (player->translateX > -2.5 && player->translateX < -1.5)
			condition2 = true;
	}
	if (condition1 && condition2) {
		return true;
	}
	return false;
}

// Check if the player is on the center platform
bool OnCenterPlatform(Player::Player* player, int i) {
	bool condition1 = false, condition2 = false;
	if (i == 1 || i == 4) {
		if (platforms[i]->translateZ < 0 && platforms[i]->translateZ + platforms[i]->scaleZ / 2 > 0) {
			condition1 = true;
		}
		else if (platforms[i]->translateZ > 0 && platforms[i]->translateZ - platforms[i]->scaleZ / 2 < 0) {
			condition1 = true;
		}
		if (player->translateX > -0.5 && player->translateX < 0.5)
			condition2 = true;
	}
	if (condition1 && condition2) {
		return true;
	}



	return false;
}

// Check if the player is on the right platform
bool OnRightPlatform(Player::Player* player, int i) {
	bool condition1 = false, condition2 = false;
	if (i == 2 || i == 5) {
		if (platforms[i]->translateZ < 0 && platforms[i]->translateZ + platforms[i]->scaleZ / 2 > 0) {
			condition1 = true;
		}
		else if (platforms[i]->translateZ > 0 && platforms[i]->translateZ - platforms[i]->scaleZ / 2 < 0) {
			condition1 = true;
		}

		if (player->translateX > 1.5 && player->translateX < 2.5)
			condition2 = true;
	}
	if (condition1 && condition2) {
		return true;
	}
	return false;
}

// Return the index of the platform on which the player is(0,1,2 or -1 if it is not on any platform)
int PlayerOnPlatform(Player::Player* player) {
	for (int i = 0; i < 6; i++) {
		if (OnLeftPlatform(player, i) && player->translateY < 0.6 && (i == 0 || i == 3))
			return i;
		else if (OnCenterPlatform(player, i) && player->translateY < 0.6 && (i == 1 || i == 4))
			return i;
		else if (OnRightPlatform(player, i) && player->translateY < 0.6 && (i == 2 || i == 5))
			return i;
	}
	return -1;
}

// Return true if the player is on any platform, false otherwise
bool OnPlatform(Player::Player* player) {
	if ((OnLeftPlatform(player, 0) || OnLeftPlatform(player, 3)) && player->translateY < 0.6)
		return true;
	else if ((OnCenterPlatform(player, 1) || OnCenterPlatform(player, 4)) && player->translateY < 0.6)
		return true;
	else if ((OnRightPlatform(player, 2) || OnRightPlatform(player, 5)) && player->translateY < 0.6)
		return true;
	return false;
}

// Check if one of the current platforms is red
bool containsRed(vector<int> old_randomPlatforms) {
	for (int i = 0; i < old_numberPlatforms; i++) {
		int index = old_randomPlatforms[i];
		if (platforms[index]->color == red_color) {
			return true;
		}
	}
	return false;
}

// Platforms can be from 0->2 or 3->5
vector<int> getPlatforms(int start, int end, int numberPlatforms) {
	vector<int> allPlatforms;
	vector<int> randomPlatforms;
	// Save in allPlatforms all indices from start to end
	for (int i = start; i < end; i++) {
		allPlatforms.push_back(i);
	}
	int n = 3;
	int first_platform;
	/*bool correct_platform = true;*/
	for (int i = 0; i < numberPlatforms; i++) {
		bool correct_platform = true;
		// Get random_index random[0-n]
		int random_index = rand() % (n - 0) + 0;
		// Save first platform selected
		if (i == 0)
			first_platform = allPlatforms[random_index];
		// If we have 2 random platforms -> they should be adjacent
		if (numberPlatforms == 2 && i == 1) {
			// If the difference between the current platform and the first one is 2 -> they are too far 
			if ((allPlatforms[random_index] - first_platform) == 2 || (first_platform - allPlatforms[random_index]) == 2) {
				correct_platform = false;
			}
		}

		// If the number of current platforms and the number of next platforms is 1 -> check if they are too far
		if (numberPlatforms == 1 && old_numberPlatforms == 1) {
			if (allPlatforms[random_index] == 2 && old_randomPlatforms[0] == 3) {
				correct_platform = false;
			}
			if (allPlatforms[random_index] == 0 && old_randomPlatforms[0] == 5) {
				correct_platform = false;
			}
			if (allPlatforms[random_index] == 3 && old_randomPlatforms[0] == 2) {
				correct_platform = false;
			}
			if (allPlatforms[random_index] == 5 && old_randomPlatforms[0] == 0) {
				correct_platform = false;
			}
		}

		// If all conditions for the platform are true -> add the platform to random platforms
		if (correct_platform) {
			// Add platform to randomPlatforms
			randomPlatforms.push_back(allPlatforms[random_index]);
		}

		// Eliminate current platform from allPlatforms
		for (int j = 0; j < n; j++) {
			if (allPlatforms[j] == allPlatforms[random_index]) {
				allPlatforms.erase(allPlatforms.begin() + j);
				n--;
			}
		}

		if (!correct_platform) {
			// Add the other platform to randomPlatforms(if the current platform is wrong -> any of the others is correct)
			randomPlatforms.push_back(allPlatforms[0]);
		}

	}

	return randomPlatforms;
}

// Return the maximum length of te current platforms
float maxPlatformLength(vector<Platform::Platform*> platforms, int start, int end) {
	float longestPlatform = 0;
	for (int i = start; i < end; i++) {
		if (platforms[i]->scaleZ > longestPlatform) {
			longestPlatform = platforms[i]->scaleZ;
		}
	}
	return longestPlatform;
}

// Draw current fuel bar
void Tema2::drawFuelBarCurrent() {
	UI = true;
	// If all lives are lost -> set speed to 0 -> game overs
	if (numberLives == 0) {
		speed = 0;
	}
	else {
		// Consuming fuel
		if (fuelBarCurrent->scaleX > 0) {
			fuelBarCurrent->scaleX -= speed * 0.0002;
			fuelBarCurrent->translateX -= speed * 0.0001;
		}
		else {
			// Verify conditions for losing lives
			numberLives--;
			fuelBarCurrent->scaleX = fuelBarFull->scaleX;
			fuelBarCurrent->translateX = fuelBarFull->translateX;
		}
	}
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(fuelBarCurrent->translateX, fuelBarCurrent->translateY, fuelBarCurrent->translateZ));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(fuelBarCurrent->scaleX, fuelBarCurrent->scaleY, fuelBarCurrent->scaleZ));
	RenderSimpleMesh(meshes["fuel_bar"], shaders["MyShader"], modelMatrix, fuelBarCurrent->color);
	UI = false;
}

// Draw full fuel bar
void Tema2::drawFuelBarFull() {
	UI = true;
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(fuelBarFull->translateX, fuelBarFull->translateY, fuelBarFull->translateZ));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(fuelBarFull->scaleX, fuelBarFull->scaleY, fuelBarFull->scaleZ));
	RenderSimpleMesh(meshes["fuel_bar"], shaders["MyShader"], modelMatrix, fuelBarFull->color);
	UI = false;
}

// Draw lives
void Tema2::drawLives(int numberLives) {
	for (int i = 0; i < numberLives; i++) {
		UI = true;
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(lives[i]->translateX, lives[i]->translateY, lives[i]->translateZ));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(lives[i]->scaleX, lives[i]->scaleY, lives[i]->scaleZ));
		RenderSimpleMesh(meshes["fuel_bar"], shaders["MyShader"], modelMatrix, lives[i]->color);
		UI = false;
	}
}

// Verify conditions for jumping
void Tema2::verifyJumping(float deltaTimeSeconds) {
	if (jump) {
		if (positive) {
			jumpSpeed -= 2 * deltaTimeSeconds;
			player->translateY += jumpSpeed * deltaTimeSeconds;

			// If maximum height is reached -> descendig
			if (player->translateY > 1.8) {
				positive = false;
			}

		}

		if (!positive) {
			jumpSpeed += 2.5 * deltaTimeSeconds;
			player->translateY -= jumpSpeed * deltaTimeSeconds;

			// If minimum height is reached and the player is on one of the platforms -> stop from jumping
			if (player->translateY < 0.6 && OnPlatform(player)) {
				positive = true;
				jump = false;
				jumpSpeed = 3;
			}

			// Otherwise -> fall from platform
		}
	}
}

// Verify conditions for falling
void Tema2::verifyFalling(float deltaTimeSeconds) {
	if (falling) {

		// Update player position considering fallSpeed
		fallSpeed += 2 * deltaTimeSeconds;
		player->translateY -= fallSpeed * deltaTimeSeconds;

		// Condition to return to initial position
		if (player->translateY < -3) {
			// Set parameters to initial value
			{
				falling = false;
				initial_state = true;
				powerUpOn = false;

				positive = true;
				jump = false;
				jumpSpeed = 3;

				speed = 3;
				fuelBarCurrent->scaleX = fuelBarFull->scaleX;
				fuelBarCurrent->translateX = fuelBarFull->translateX;
				player->translateX = 0;
				player->translateY = 0.5;
				camera->Set(position_camera, center_camera, up_camera);

				current_platforms = 0;
				randomPlatforms.clear();
				old_randomPlatforms.clear();
				numberPlatforms = 3;
				old_numberPlatforms = 3;

				numberLives = 3;
			}
			// Set current platforms and next platforms to platforms[0-3]
			for (int i = 0; i < 3; i++) {
				randomPlatforms.push_back(i);
				old_randomPlatforms.push_back(i);
			}

			// For platforms[0-3] set color to blue
			for (int i = 0; i < 3; i++) {
				platforms[i]->translateZ = -3;
				platforms[i]->scaleZ = 8;
				platforms[i]->color = blue_color;
			}

			// For platforms[3-6] set color to blue
			for (int i = 3; i < 6; i++) {
				platforms[i]->translateZ = -dist;
				platforms[i]->scaleZ = 8;
				platforms[i]->color = blue_color;
			}

		}
	}
}

// Draw current platforms
void Tema2::drawPlatforms(int i) {
	glm::mat4 modelMatrix = glm::mat4(1);
	// Move the platform towards the player on Z axis
	modelMatrix = glm::translate(modelMatrix, glm::vec3(platforms[i]->translateX, platforms[i]->translateY, platforms[i]->translateZ));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(platforms[i]->scaleX, platforms[i]->scaleY, platforms[i]->scaleZ));

	// Verify if the player is on the left platform
	if (OnLeftPlatform(player, i) && player->translateY < 0.6) {
		RenderSimpleMesh(meshes["platform"], shaders["MyShader"], modelMatrix, violet_color);
	}
	// Verify if the player is on the center platform
	else if (OnCenterPlatform(player, i) && player->translateY < 0.6) {
		RenderSimpleMesh(meshes["platform"], shaders["MyShader"], modelMatrix, violet_color);
	}
	// Verify if the player is on the right platform
	else if (OnRightPlatform(player, i) && player->translateY < 0.6) {
		RenderSimpleMesh(meshes["platform"], shaders["MyShader"], modelMatrix, violet_color);
	}
	else {
		RenderSimpleMesh(meshes["platform"], shaders["MyShader"], modelMatrix, platforms[i]->color);
	}
}

// Save current platforms and generate random new platform number
void Tema2::saveOldPlatforms(int old_start, int old_end) {
	old_randomPlatforms.clear();
	old_numberPlatforms = numberPlatforms;
	for (int j = 0; j < numberPlatforms; j++) {
		int index = randomPlatforms[j];
		old_randomPlatforms.push_back(index);
	}

	randomPlatforms.clear();
	// Generate random new platform number
	numberPlatforms = rand() % (3 - 1 + 1) + 1;
	vector<int> randomPlatformsIndices;
	// If we saved from [0-2] -> save possible new random indices from [3-5]
	if (old_start == 0)
		randomPlatformsIndices = getPlatforms(old_start + 3, old_end + 3, numberPlatforms);
	// If we saved from [3-5] -> save possible new random indices from [0-2]
	else
		randomPlatformsIndices = getPlatforms(old_start - 3, old_end - 3, numberPlatforms);

	for (int j = 0; j < numberPlatforms; j++) {
		int index = randomPlatformsIndices[j];
		randomPlatforms.push_back(index);
	}
}

// Generate random qualities for new platforms
void Tema2::generateNewPlatforms(int new_start, int new_end) {
	// Generate random platform length and change Z value of platforms[new_start-new_end] to dist
	for (int j = new_start; j < new_end; j++) {
		{
			platforms[j]->scaleZ = rand() % (max_lengthZ - min_lengthZ + 1) + min_lengthZ;
			platforms[j]->translateZ = dist;
			current_platforms = new_start / 3;// [0/3 -> 0] or [3/3 -> 1]
		}
	}
	// Generate random platform color
	for (int j = 0; j < numberPlatforms; j++) {
		int color_idx;
		int first_index = randomPlatforms[0];
		int index = randomPlatforms[j];
		// If only one platform is generated or one of the current platforms is red or is a middle platform -> ignore red color
		if (numberPlatforms == 1 || containsRed(old_randomPlatforms) || index % 3 == 1)
			color_idx = rand() % max_color + 1;
		else
			color_idx = rand() % (max_color - min_color + 1) + min_color;
		platforms[index]->color = colors[color_idx];
	}
}

void Tema2::Init()
{

	camera = new Laborator::Camera();
	position_camera = glm::vec3(0, 1.5f, 3);
	center_camera = glm::vec3(0, 1, 0);
	up_camera = glm::vec3(0, 1, 0);
	camera->Set(position_camera, center_camera, up_camera);

	{
		Mesh* mesh = new Mesh("platform");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("player");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader* shader = new Shader("MyShader");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	// Initialize a player object
	player = new Player::Player(0, 0.5, 0, 0.75, 0.75, 0);

	// Initialize 6 platform objects
	{
		float initial_scaleZ = 8;

		platforms.push_back(new Platform::Platform(-2, 0, -4, 1, 0.1, initial_scaleZ, 0, blue_color));
		platforms.push_back(new Platform::Platform(0, 0, -4, 1, 0.1, initial_scaleZ, 0, blue_color));
		platforms.push_back(new Platform::Platform(2, 0, -4, 1, 0.1, initial_scaleZ, 0, blue_color));
		platforms.push_back(new Platform::Platform(-2, 0, -dist, 1, 0.1, initial_scaleZ, 0, blue_color));
		platforms.push_back(new Platform::Platform(0, 0, -dist, 1, 0.1, initial_scaleZ, 0, blue_color));
		platforms.push_back(new Platform::Platform(2, 0, -dist, 1, 0.1, initial_scaleZ, 0, blue_color));

	}

	// Initialize color vector
	{
		colors.push_back(red_color);
		colors.push_back(yellow_color);
		colors.push_back(orange_color);
		colors.push_back(green_color);
		colors.push_back(blue_color);
		colors.push_back(white_color);
		colors.push_back(gray_color);
	}

	{
		Mesh* mesh = new Mesh("fuel_bar");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	fuelBarFull = new FuelBar::FuelBar(-0.5, 0.75, 0, 0.4, 0.09, 0, 0, white_color);
	fuelBarCurrent = new FuelBar::FuelBar(-0.5, 0.75, 0, 0.4, 0.09, 0, 0, gray_color);

	{
		life1 = new FuelBar::FuelBar(0.5, 0.75, 0, 0.03, 0.07, 0, 0, green_color);
		life2 = new FuelBar::FuelBar(0.6, 0.75, 0, 0.03, 0.07, 0, 0, green_color);
		life3 = new FuelBar::FuelBar(0.7, 0.75, 0, 0.03, 0.07, 0, 0, green_color);
		lives.push_back(life1);
		lives.push_back(life2);
		lives.push_back(life3);
	}

	numberLives = 3;

	{
		numberPlatforms = 3;
		old_numberPlatforms = 3;
		for (int i = 0; i < 3; i++) {
			randomPlatforms.push_back(i);
			old_randomPlatforms.push_back(i);
		}
	}
}

void Tema2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds)
{
	if (!gameOver) {

		if (toggleCamera) {
			glm::vec3 position_new = glm::vec3(player->translateX, player->translateY, player->translateZ);
			glm::vec3 center_new = glm::vec3(player->translateX, player->translateY, player->translateZ - 1);
			camera->Set(position_new, center_new, up_camera);

		}

		// Draw fuelBarCurrent
		drawFuelBarCurrent();

		// Draw fuelBarFull
		drawFuelBarFull();

		// Draw lives
		drawLives(numberLives);

		// Platforms moving animation
		{
			for (int i = 0; i < 6; i++) {
				platforms[i]->translateZ += deltaTimeSeconds * speed;
			}
			camera->MoveForward(deltaTimeSeconds * speed);
		}

		// Verify conditions for falling
		{
			// If the player is currently on the left side(0) and falls from the platform 0 or 3 -> falling
			if (DirectionPlatform(player) == 0 && !OnLeftPlatform(player, 0) && !OnLeftPlatform(player, 3) && player->translateY < 0.6) {
				falling = true;
			}
			// If the player is currently on the center side(1) and falls from the platform 1 or 4 -> falling
			else if (DirectionPlatform(player) == 1 && !OnCenterPlatform(player, 1) && !OnCenterPlatform(player, 4) && player->translateY < 0.6) {
				falling = true;
			}
			// If the player is currently on the right side(2) and falls from the platform 2 or 5 -> falling
			else if (DirectionPlatform(player) == 2 && !OnRightPlatform(player, 2) && !OnRightPlatform(player, 5) && player->translateY < 0.6) {
				falling = true;
			}

			// If te platform is not shown -> falling
			for (int i = 0; i < 6; i++) {
				bool cond1 = containsElement(randomPlatforms, i, numberPlatforms);
				bool cond2 = containsElement(old_randomPlatforms, i, old_numberPlatforms);

				if (DirectionPlatform(player) == 0 && OnLeftPlatform(player, i) && (!cond1 && !cond2)) {
					falling = true;
				}
				if (DirectionPlatform(player) == 1 && OnCenterPlatform(player, i) && (!cond1 && !cond2)) {
					falling = true;
				}
				if (DirectionPlatform(player) == 2 && OnRightPlatform(player, i) && (!cond1 && !cond2)) {
					falling = true;
				}
			}

			// If the player is between platforms
			if (DirectionPlatform(player) == -1 && player->translateY < 0.6) {
				falling = true;
			}
		}

		// Verify jumping
		verifyJumping(deltaTimeSeconds);
		// Check for falling -> falling animation
		verifyFalling(deltaTimeSeconds);

		// Initial state of the game -> 3 blue platforms
		if (initial_state) {

			for (int i = 0; i < 3; i++)
			{
				// Generate initial platforms
				drawPlatforms(i);
			}
			{
				//Generate new platform
				float longestPlatform = platforms[0]->translateZ - maxPlatformLength(platforms, 0, 3) / 2;
				float dist_gen = -(max_lengthZ / 2);

				if (longestPlatform > dist_gen)
				{

					// Save current platforms -> [0,1,2]
					saveOldPlatforms(0, 3);

					// Generate new random platforms
					generateNewPlatforms(3, 6);
					initial_state = false;
				}
			}
		}
		// The rest of the game
		else {


			// Verify conditions for red power-up
			{

				int playerPlatform = PlayerOnPlatform(player);
				if (playerPlatform != -1 && platforms[playerPlatform]->color == red_color) {
					speed = 0;
					printf("Game Over!\n");
					gameOver = true;
					RedPowerUpOn = true;
				}


			}

			// Verify conditions for orange power-up
			{
				int playerplatform = PlayerOnPlatform(player);


				if (!fullSpeed && playerplatform != -1 && platforms[playerplatform]->color == orange_color) {
					fullSpeed = true;
					speed = 9;
					endTimeOrange = 10 * deltaTimeSeconds;
					startTimeOrange = 10 * deltaTimeSeconds;
					OrangePowerUpOn = true;
				}

				// Verify stop condition for orange power-up
				if (fullSpeed)
				{
					OrangePowerUpOn = true;
					endTimeOrange += 10 * deltaTimeSeconds;
					timeOrange = endTimeOrange - startTimeOrange;
					if (timeOrange > 20 || falling) {
						fullSpeed = false;
						speed = 3;
						OrangePowerUpOn = false;
					}

				}
			}

			// Verify conditions for yellow power-up
			{

				int playerPlatform = PlayerOnPlatform(player);
				// If the player is on yellow platform
				if (!fuelPlatformYellow && playerPlatform != -1 && platforms[playerPlatform]->color == yellow_color) {
					// Verify not to decrease past fuelBarFull inferior limit
					if (fuelBarCurrent->scaleX - 2 * 0.5 > 0) {
						fuelBarCurrent->scaleX -= 2 * 0.5;
						fuelBarCurrent->translateX -= 0.5;
					}
					// Otherwise scale to 0
					else {
						fuelBarCurrent->scaleX = 0;
						fuelBarCurrent->translateX -= fuelBarCurrent->scaleX / 2;
					}
					// Save current platform index and set fuelPlatformYellow to true
					fuelPlatformYellow = true;
					fuelPlatformIndex = playerPlatform;
					YellowPowerUpOn = true;
				}
				// If the player is no longer on yellow platform -> set fuelPlatformYellow to false
				else if (fuelPlatformYellow && playerPlatform != -1 && playerPlatform != fuelPlatformIndex) {
					fuelPlatformYellow = false;
					YellowPowerUpOn = false;
				}
			}

			// Verify conditions for green power-up
			{
				int playerPlatform = PlayerOnPlatform(player);
				// If the player is on green platform
				if (!fuelPlatformGreen && playerPlatform != -1 && platforms[playerPlatform]->color == green_color) {
					// Verify not to increase fuel past fuelBarFull superior limit
					if (fuelBarCurrent->scaleX + 2 * 0.5 < fuelBarFull->scaleX) {
						fuelBarCurrent->scaleX += 2 * 0.5;
						fuelBarCurrent->translateX += 0.5;
					}
					// Otherwise scale to fuelBarFull scale
					else {
						fuelBarCurrent->scaleX = fuelBarFull->scaleX;
						fuelBarCurrent->translateX = fuelBarFull->translateX;
					}
					// Save current platform index and set fuelPlatformYellow to true
					fuelPlatformIndex = playerPlatform;
					fuelPlatformGreen = true;
					GreenPowerUpOn = true;
				}
				// If the player is no longer on green platform -> set fuelPlatformGreen to false
				else if (fuelPlatformGreen && playerPlatform != -1 && fuelPlatformIndex != playerPlatform) {
					fuelPlatformGreen = false;
					GreenPowerUpOn = false;
				}
			}

			// Verify condition for white power-up() -> gain one life
			{
				int playerPlatform = PlayerOnPlatform(player);
				if (!modify_lives && playerPlatform != -1 && platforms[playerPlatform]->color == white_color) {
					if (numberLives < 3) {
						numberLives++;
					}
					livesPlatformIndex = playerPlatform;
					modify_lives = true;
					WhitePowerUpOn = true;
				}
				else if (modify_lives && playerPlatform != -1 && livesPlatformIndex != playerPlatform) {
					WhitePowerUpOn = false;
					modify_lives = false;
				}

			}

			// Verify condition for gray power-up() -> lose one life
			{
				int playerPlatform = PlayerOnPlatform(player);
				if (!modify_lives && playerPlatform != -1 && platforms[playerPlatform]->color == gray_color) {
					if (numberLives > 0) {
						numberLives--;
					}
					livesPlatformIndex = playerPlatform;
					modify_lives = true;
					GrayPowerUpOn = true;
				}
				else if (modify_lives && playerPlatform != -1 && livesPlatformIndex != playerPlatform) {
					GrayPowerUpOn = false;
					modify_lives = false;
				}
			}


			if (RedPowerUpOn || OrangePowerUpOn || YellowPowerUpOn || GreenPowerUpOn || WhitePowerUpOn || GrayPowerUpOn) {
				powerUpOn = true;
			}
			// If te platform is blue -> no power-up
			else {
				powerUpOn = false;
			}
			// If the player is no longer on the platform or the orange power-up expired -> no power-up
			if (player->translateY > 0.6 && !OrangePowerUpOn) {
				powerUpOn = false;
			}

			// Verify blue platform
			{
				int playerPlatform = PlayerOnPlatform(player);
				if (playerPlatform != -1 && platforms[playerPlatform]->color == blue_color) {
					powerUpOn = false;
				}
			}

			// Drawing platforms
			{
				// Currently on platforms [0-2]
				if (current_platforms == 0)
				{
					for (int i = 0; i < 6; i++) {
						// If i is the index of one of the next platforms and i is in range [0-2] -> draw platform
						if (containsElement(randomPlatforms, i, numberPlatforms) && i >= 0 && i < 3) {
							drawPlatforms(i);
						}

						// If i is the index of one of the current platforms and i is in range [3-6] -> draw platform
						if (containsElement(old_randomPlatforms, i, old_numberPlatforms) && i >= 3 && i < 6) {
							drawPlatforms(i);
						}


					}
				}
				// Currently on platforms [3-5]
				if (current_platforms == 1) {
					for (int i = 0; i < 6; i++) {
						// If i is the index of one of the next platforms and i is in range [3-6] -> draw platform
						if (containsElement(randomPlatforms, i, numberPlatforms) && i >= 3 && i < 6) {
							drawPlatforms(i);
						}

						// If i is the index of one of the current platforms and i is in range [0-2] -> draw platform
						if (containsElement(old_randomPlatforms, i, old_numberPlatforms) && i >= 0 && i < 3) {
							drawPlatforms(i);
						}


					}
				}
			}

			// Generating new platforms
			{
				float longestPlatform;
				float dist_gen = -(max_lengthZ / 2);

				//If the player is currently on platforms[3-5]
				if (current_platforms == 1)
				{
					// Compute the farmost limit of the longest platform between platforms[3-5]
					{
						int index = randomPlatforms[0];
						longestPlatform = platforms[index]->translateZ - maxPlatformLength(platforms, 3, 6) / 2;
					}

					// Verify if the longest platform reaches the limit
					if (longestPlatform > dist_gen) {

						// Save current platforms and generate random new platform number
						saveOldPlatforms(3, 6);

						// Generate random qualitie for new platforms
						generateNewPlatforms(0, 3);


					}
				}
				//If the player is currently on platforms[0-2]
				else
				{
					// Compute the farmost limit of the longest platform between platforms[0-3]
					{
						int index = randomPlatforms[0];
						longestPlatform = platforms[index]->translateZ - maxPlatformLength(platforms, 0, 3) / 2;
					}

					// Verify if the longest platform reaches the limit
					if (longestPlatform > dist_gen) {
						// Save current platforms and generate random new platform number
						saveOldPlatforms(0, 3);

						// Generate random qualitie for new platforms
						generateNewPlatforms(3, 6);
					}
				}

			}

		}

		// Player
		{
			glm::mat4 modelMatrix = glm::mat4(1);
			// Player stays in the same positio on Z axis
			modelMatrix = glm::translate(modelMatrix, glm::vec3(player->translateX, player->translateY, 0));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.75));
			if (!toggleCamera) {
				distortPlayer = true;
				if (powerUpOn) {
					modelMatrix = glm::scale(modelMatrix, glm::vec3(1.5));
				}
				RenderSimpleMesh(meshes["player"], shaders["MyShader"], modelMatrix, glm::vec3(1, 0.5, 1));
				distortPlayer = false;
			}
		}

	}
}

void Tema2::FrameEnd()
{
	//DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	glUniform3fv(glGetUniformLocation(shader->program, "object_color"), 1, glm::value_ptr(color));

	// TODO : get shader location for uniform mat4 "Model"
	int model_location = glGetUniformLocation(shader->program, "Model");

	// TODO : set shader uniform "Model" to modelMatrix
	glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// TODO : get shader location for uniform mat4 "View"
	int view_location = glGetUniformLocation(shader->program, "View");

	// TODO : set shader uniform "View" to viewMatrix
	if (!toggleCamera) {
		glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
		glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	}
	if (toggleCamera) {
		glm::mat4 viewMatrix = camera->GetViewMatrix();
		glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	}
	// TODO : get shader location for uniform mat4 "Projection"
	int projection_location = glGetUniformLocation(shader->program, "Projection");

	// TODO : set shader uniform "Projection" to projectionMatrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	// Send distortPlayer to shader as uniform
	int distort_position = glGetUniformLocation(shader->program, "distortPlayer");
	glUniform1f(distort_position, distortPlayer);

	// Send UI to shader as uniform
	int UI_position = glGetUniformLocation(shader->program, "UI");
	glUniform1f(UI_position, UI);

	// Send powerUpOn to shader as uniform
	int powerUp_position = glGetUniformLocation(shader->program, "powerUpOn");
	glUniform1f(powerUp_position, powerUpOn);

	// Send wavelengt to shader as uniform
	waveLength = 0.75;
	int wave_position = glGetUniformLocation(shader->program, "waveLength");
	glUniform1f(wave_position, waveLength);

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	if (window->KeyHold(GLFW_KEY_W)) {
		if (speed < maxSpeed && !fullSpeed) {
			speed += 10 * deltaTime;

		}
	}
	if (window->KeyHold(GLFW_KEY_S)) {
		if (speed > minSpeed && !fullSpeed)
			speed -= 10 * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_A)) {
		// If the player moves left and is no longer on the platform -> falling
		if (player->translateY < 0.6 && !OnPlatform(player)) {
			falling = true;
		}
		// Otherwise, move normally
		else {
			player->translateX -= deltaTime * 2;
		}
	}
	if (window->KeyHold(GLFW_KEY_D)) {
		// If the player moves right and is no longer on the platform -> falling
		if (player->translateY < 0.6 && !OnPlatform(player)) {
			falling = true;
		}
		// Otherwise, move normally
		else {
			player->translateX += deltaTime * 2;
		}
	}
}

void Tema2::OnKeyPress(int key, int mods)
{
	// add key press event

	if (key == GLFW_KEY_SPACE)
	{
		jump = true;
	}

	if (key == GLFW_KEY_C) {
		toggleCamera = !toggleCamera;
		if (!toggleCamera) {
			camera->Set(position_camera, center_camera, up_camera);
		}
	}
}

void Tema2::OnKeyRelease(int key, int mods)
{
	// add key release event

}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event


}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}
