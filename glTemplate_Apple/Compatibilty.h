//
//  Compatibilty.h
//  Tunnel
//
//  Created by jonathan on 15/02/2014.
//  Copyright (c) 2014 jonathan. All rights reserved.
//

/*
 provides drop-in replacement for some windows-specific 'embrace and extend' C++ functions
 
 */

#ifndef __Tunnel__Compatibilty__
#define __Tunnel__Compatibilty__

#include <iostream>



int sprintf_s(
              char *buffer,
              size_t sizeOfBuffer,
              const char *format
              ,...
              );


int sprintf_s(
              char *buffer,
              const char *format
              ,...
              );

#endif /* defined(__Tunnel__Compatibilty__) */
