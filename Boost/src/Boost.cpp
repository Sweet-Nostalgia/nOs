#include <iostream>
#include <nOs.h>

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

#include <nOs/EntryPoint.h>