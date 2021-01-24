#pragma once
#include <include/glm.h>
#include <include/math.h>

namespace Player {
	class Player
	{
	public:
		float translateX, translateY, translateZ;
		float scaleX, scaleY;
		float angularStep;

		Player()
		{
		}

		Player(float translateX, float translateY, float translateZ ,float scaleX, float scaleY, float angularStep)
		{
			Set(translateX, translateY, translateZ, scaleX, scaleY, angularStep);
		}

		~Player()
		{ }

		void Set(float translateX, float translateY, float translateZ, float scaleX, float scaleY, float angularStep)
		{
			this->translateX = translateX;
			this->translateY = translateY;
			this->translateZ = translateZ;
			this->scaleX = scaleX;
			this->scaleY = scaleY;
			this->angularStep = angularStep;
		}
	};
}

