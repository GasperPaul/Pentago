#include "UserInterface.h"

#define __MainUserInterfaceFile

#ifdef _PENTAGO_CONSOLE_
	#include "UserInterfaceConsole.cpp"
#elif defined _PENTAGO_OPENGL_
	#include "UserInterfaceOpenGL.cpp"
#else
	#error Interface type not specified
#endif

#undef __MainUserInterfaceFile
