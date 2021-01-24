#pragma once
#include <Component/SimpleScene.h>
#include "LabCamera.h"
#include "Player.h"
#include "Platform.h"
#include "FuelBar.h"

class Tema2 : public SimpleScene
{
	public:
		Tema2();
		~Tema2();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color = glm::vec3(1));

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

		void drawFuelBarFull();
		void drawFuelBarCurrent();
		void drawLives(int numberLives);
		void verifyJumping(float deltaTimeSeconds);
		void verifyFalling(float deltaTimeSeconds);
		void drawPlatforms(int i);
		void saveOldPlatforms(int old_start, int old_end);
		void generateNewPlatforms(int new_start, int new_end);
		void isFalling();

	protected:
		Laborator::Camera *camera;
		glm::mat4 projectionMatrix;
		bool renderCameraTarget;
		Player::Player* player;
		Platform::Platform* platform1;
		Platform::Platform* platform2;
		Platform::Platform* platform3;
		FuelBar::FuelBar* fuelBarFull;
		FuelBar::FuelBar* fuelBarCurrent;
		FuelBar::FuelBar* life1;
		FuelBar::FuelBar* life2;
		FuelBar::FuelBar* life3;
		bool distortPlayer = false;
		bool UI = false;
		bool RedPowerUpOn = false;
		bool GreenPowerUpOn = false;
		bool YellowPowerUpOn = false;
		bool OrangePowerUpOn = false;
		bool WhitePowerUpOn = false;
		bool GrayPowerUpOn = false;
		bool powerUpOn = false;
		float waveLength;
};
