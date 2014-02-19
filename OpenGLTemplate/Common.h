//
//  Common.h
//  TestGL
//
//  Created by jonathan on 02/02/2014.
//  Copyright (c) 2014 jonathan. All rights reserved.
//

#ifndef TestGL_Common_h
#define TestGL_Common_h



#include <ctime>
#include <cstring>
#include <vector>
#include <sstream>



#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

#define LOG 1
#define MYBOX 1
#define PLANE 1
#define FLYTHROUGH 0


//OSX
#ifdef __APPLE__
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <OpenGL/gl3.h>
#include <OpenGL/gl.h>
#include "Compatibilty.h"

typedef int UINT;
typedef unsigned char BYTE;
typedef signed char BOOL;

#endif

//WINDOWS
#ifdef WIN32
#include <windows.h>
#include "./include/glm/gtc/type_ptr.hpp"
#include "./include/glm/gtc/matrix_transform.hpp"
#include "./include/glm/gtx/rotate_vector.hpp"
#include "include/gl/glew.h"
#include <gl/gl.h>
#define snprintf sprintf_s
#endif

#endif
