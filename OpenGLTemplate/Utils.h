//
//  Utils.h
//  Tunnel
//
//  Created by jonathan on 15/02/2014.
//  Copyright (c) 2014 jonathan. All rights reserved.
//

#ifndef __Tunnel__Utils__
#define __Tunnel__Utils__

#include <iostream>
#ifdef __APPLE__
#include <glm/gtc/type_ptr.hpp>
#endif
#ifdef _WIN32
#include <./include/glm/gtc/type_ptr.hpp>
#endif

void glCheckError();


void errorMessage(const char* format, ...);

std::string resourcePath(std::string fileName, std::string type, std::string subtype);

std::string resourceDirectory(std::string type, std::string subtype);

void logVec3 (glm::vec3 vec);

#endif /* defined(__Tunnel__Utils__) */
