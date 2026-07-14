#pragma once

#ifdef NOS_PLATFORM_WINDOWS

extern nOs::Application* nOs::CreateApplication();

int main(int argc, char** argv) {

	nOs::Log::Init();

	auto app = nOs::CreateApplication();
	app->Run();
	delete app;
}

#endif