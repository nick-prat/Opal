#include <iostream>

#include "display.h"

int main(int argc, char **argv)
{
	Display* display = new Display();
	display->InitDisplay(500, 500, "OpenGL Game");
	
	
	
	display->Destroy();
	return 0;
}
