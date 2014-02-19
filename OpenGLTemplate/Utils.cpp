//
//  Utils.cpp
//  Tunnel
//
//  Created by jonathan on 15/02/2014.
//  Copyright (c) 2014 jonathan. All rights reserved.
//

#include "Utils.h"
#include "Common.h"
#include <stdarg.h>
#include <string>
#ifdef __APPLE__
#include <Foundation/Foundation.h>
#include <CoreFoundation/CoreFoundation.h>
#include "glm/gtx/string_cast.hpp"
#endif
#ifdef _WIN32
#include "./include/glm/gtx/string_cast.hpp"
#endif

std::string glErrorCode (GLenum error);


void glCheckError() {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        cout << "glError: " << glErrorCode(error) << endl;
    }
}

void errorMessage(const char* format, ...) {
    va_list args;
    va_start (args, format);
    char string[1024];
    vsnprintf(string, 1024, format, args);
    va_end (args);
#if defined(WIN32)
    MessageBox(NULL,string,"Error", MB_ICONHAND);
#else
    printf("%s",string);
#endif
}


std::string glErrorCode (GLenum error) {
    std::string result;
    switch (error) {
        case GL_INVALID_ENUM:
            result = "GL_INVALID_ENUM GLenum argument out of range.";
            break;
        case GL_INVALID_VALUE:
            result = "GL_INVALID_VALUE Numeric argument out of range.";
            break;
        case GL_INVALID_OPERATION:
            result = "GL_INVALID_OPERATION Operation illegal in current state.";
            break;
        case GL_STACK_OVERFLOW:
            result = "GL_STACK_OVERFLOW Function would cause a stack overflow.";
            break;
        case GL_STACK_UNDERFLOW:
            result = "GL_STACK_UNDERFLOW Function would cause a stack underflow.";
            break;
        case GL_OUT_OF_MEMORY:
            result = "GL_OUT_OF_MEMORY Not enough memory left to execute function.";
            break;
        default:
            result = "GL Error not known";
            break;
    }
    return result;
}


#ifdef __APPLE__

std::string resourcePath(std::string fileName) {
    NSString* fname = [NSString stringWithCString:fileName.c_str() encoding:NSUTF8StringEncoding];
    NSString* path;
    if (fileName.length()>0) {
     path = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:fname];
    } else {
     path = [[[NSBundle mainBundle] resourcePath] stringByAppendingString:@"/"];
    }
    std::string result = std::string([path cStringUsingEncoding:NSUTF8StringEncoding]);
    return result;
}

std::string resourcePath(std::string fileName, std::string type, std::string subtype) {
        return resourcePath(fileName);
}



#endif


#ifdef _WIN32


std::string resourcePath(std::string fileName, std::string type, std::string subtype) {
    std::string result = "resources\\";
    std::string pathSeparator = "\\";

    result.append(type);
    result.append(pathSeparator);

    if (subtype.length() > 0) {
        result.append(subtype);
        result.append(pathSeparator);
    }
    if (fileName.length()>0) {
        result.append(fileName);
    }
    return result;
}
#endif



void logVec3 (glm::vec3 vec) {
    std::cout<<glm::to_string(vec)<<std::endl;
}

