#ifndef ENTRY_POINT_H
#define ENTRY_POINT_H

#include "Application.h"

int main(int argc, char** argv)
{
	auto app = Pixel::CreateApplication();

	delete app;
}

#endif // !ENTRY_POINT_H