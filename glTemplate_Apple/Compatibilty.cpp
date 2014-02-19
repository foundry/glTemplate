//
//  Compatibilty.cpp
//  Tunnel
//
//  Created by jonathan on 15/02/2014.
//  Copyright (c) 2014 jonathan. All rights reserved.
//

#include "Compatibilty.h"
#include "Common.h"


int sprintf_s(
              char *buffer,
              size_t sizeOfBuffer,
              const char *format
              ,...
              ) {
    return snprintf(buffer,sizeOfBuffer,format);
}

int sprintf_s(
              char *buffer,
              const char *format
              ,...
              ) {
    return sprintf_s(buffer, sizeof(buffer)/8,format);
    
}