#pragma once
#include "Core.h"

namespace Velvet {

	class VELVET_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}