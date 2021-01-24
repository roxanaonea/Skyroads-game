#pragma once
#include <include/glm.h>
#include <include/math.h>

namespace FuelBar {
	class FuelBar
	{
		public:
			float translateX, translateY, translateZ;
			float scaleX, scaleY, scaleZ;
			float angularStep;
			glm::vec3 color;

			FuelBar()
			{
			}

			FuelBar(float translateX, float translateY, float translateZ, float scaleX, float scaleY, float scaleZ, float angularStep, glm::vec3 color)
			{
				Set(translateX, translateY, translateZ, scaleX, scaleY, scaleZ, angularStep, color);
			}

			~FuelBar()
			{ }

			void Set(float translateX, float translateY, float translateZ, float scaleX, float scaleY, float scaleZ, float angularStep, glm::vec3 color)
			{
				this->translateX = translateX;
				this->translateY = translateY;
				this->translateZ = translateZ;
				this->scaleX = scaleX;
				this->scaleY = scaleY;
				this->scaleZ = scaleZ;
				this->angularStep = angularStep;
				this->color = color;
			}
	};
}

