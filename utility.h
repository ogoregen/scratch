
#pragma once

#include "glm/glm.hpp"

glm::vec2 toReal(float x, float y){

	glm::vec2 output(x, y);
	output.x -= output.y;
	output.y = (x + y) / 2;
	return output;
}

glm::vec2 toIsometricUnit(float x, float y){

	glm::vec2 output(x, y);
	output.x = (x + 2 * y) / 2;
	output.y = (2 * y - x) / 2;
	return output;
}

int roundToMultiple(int target, int input){

	int output = input;
	if(output % target >= target / 2) output += target - (output % target);
	else output -= (output % target);
	return output;
}
