#include <iostream>
#include <nOs.h>
#include <nOs/EntryPoint.h>

class Boost : public nOs::Application {
public:
	Boost() {

	}
	~Boost() {

	}
};

nOs::Application* nOs::CreateApplication() {
	return new Boost();
}