#include <iostream>
#include <nOs.h>
#include <nOs/EntryPoint.h>
//#include "GuiLayer.h"

class ExampleLayer : public nOs::Layer {
public:
	ExampleLayer()
		: Layer("Example") {
	}

	void OnUpdate() override {
		NOS_INFO("ExampleLayer::Update");
		if (nOs::Input::IsKeyPressed(NOS_TAB)) {
			NOS_INFO("Tab key is pressed!");
		}
		if (nOs::Input::IsMouseButtonPressed(NOS_LBUTTON)) {
			NOS_INFO("Left mouse button is pressed!");
		}
	}
};

class Boost : public nOs::Application {
public:
	Boost() {
		//PushOverlay(new nOs::GuiLayer());
	}
	~Boost() {

	}
};

nOs::Application* nOs::CreateApplication() {
	return new Boost();
}