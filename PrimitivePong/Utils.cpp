#include "Utils.h"
#include <gl\glew.h>
#include <glfw/glfw3.h>

float Utils::toRadians(float degrees)
{
    return degrees * 2.0f * 3.14159f / 360.0f;
}