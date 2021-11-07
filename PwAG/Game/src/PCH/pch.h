#ifndef PCH_H
#define PCH_H

// C++
#include <iostream>
#include <cstdint>
#include <queue>
#include <fstream>
#include <unordered_map>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>
#include <stdio.h>
#include <vector>

// additional dependencies
#include "glad/glad.h"

#include "GLFW/glfw3.h"

#include "GLM/include/glm.hpp"
#include "GLM/include/gtc/matrix_transform.hpp"
#include "GLM/include/gtc/type_ptr.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

// podobno (tak ludzie ze stacka pisz¹) to musi byæ definiowane w pliku .cpp lub .c, bo nie da siê skompilowaæ przy próbie u¿ycia funkcji stbi_load lub free
// da³em to na razie w texture.cpp
/*
#define STB_IMAGE_IMPLEMENTATION
#include "../SourceDep/stb_image.h"
*/

// this project
#include "../Main/config.h"
#include "../Window/windowMode.h"
#include "../Rendering system/Shader/shaderProgram.h"
#include "../Rendering system/ABE Objects/vao.h"
#include "../Rendering system/ABE Objects/vbo.h"
#include "../Rendering system/ABE Objects/ebo.h"

#include "../Event system/mouse.h"
#include "../Event system/keyboard.h"
#include "../Event system/eventType.h"
#include "../Event system/eventManager.h"

#include "../Utilities/timer.h"


#endif