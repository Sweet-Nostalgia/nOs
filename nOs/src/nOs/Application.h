#pragma once

#include "Core.h"

namespace nOs {
	class NOS_API Application {
	public:
		Application();
		virtual ~Application();
		void Run();
	};

	//To be defined in client
	Application* CreateApplication();
}
