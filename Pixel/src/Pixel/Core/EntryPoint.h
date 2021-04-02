#ifndef ENTRY_POINT_H
#define ENTRY_POINT_H

#include "Application.h"
#include "Core/Logger.h"

int main(int argc, char** argv)
{
	Pixel::LogImpl::Init();
	auto app = Pixel::CreateApplication();

	app->Run();

	delete app;
}

#endif // !ENTRY_POINT_H
