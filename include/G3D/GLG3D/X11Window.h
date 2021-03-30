/**
 @file X11Window.h
  
 A OSWindow that uses the X11 API.

 @created 	  2005-06-04
 @edited  	  2005-06-04
    
 Copyright 2000-2005, Morgan McGuire.
 All rights reserved.
*/

#ifndef G3D_X11WINDOW_H
#define G3D_X11WWINDOW_H

#include <C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\G3D/platform.h>

#ifndef G3D_LINUX
#endif

// Current implementation: X11Window *is* SDLWindow
#include "C:\Users\Mary Dull\Desktop\xenimus\xps\include\G3D\GLG3D/SDLWindow.h"

#define X11Window SDLWindow

#endif
