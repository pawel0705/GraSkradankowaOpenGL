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
#include <map>
#include <stack>
#include <functional>
#include <utility>
#include <array>

// additional dependencies
#include "glad/glad.h"

#include "GLFW/glfw3.h"

#include "GLM/include/glm.hpp"
#include "GLM/include/gtc/matrix_transform.hpp"
#include "GLM/include/gtc/type_ptr.hpp"
#include "GLM/include/gtx/norm.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

// this project
#include "../Main/config.h"
#include "../Window/windowMode.h"
#include "../Rendering system/Shader/shaderProgram.h"
#include "../Rendering system/OpenGL Objects/vao.h"
#include "../Rendering system/OpenGL Objects/vbo.h"
#include "../Rendering system/OpenGL Objects/ebo.h"
#include "../Rendering system/OpenGL Objects/fbo.h"

#include "../Event system/mouse.h"
#include "../Event system/keyboard.h"
#include "../Event system/eventType.h"
#include "../Event system/eventManager.h"

#include "../Utilities/timer.h"
#include "../Utilities/resourceManager.h"

#endif