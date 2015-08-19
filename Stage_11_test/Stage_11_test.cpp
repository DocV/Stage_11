// Stage_11_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Gameloop.h>
#include <iostream>
#include <Component.h>
#include <string>

using namespace stage_11;
int serialCount;

class Testprinter : public Component{
public:
	Testprinter(GameObject& owner) : Component(owner){
		serial = serialCount++;
	}
	void doUpdate(){
		std::cout << "Printer " << std::to_string(serial) << " updating" << std::endl;
	}
	void doRender(){
		std::cout << "Printer " << std::to_string(serial) << " rendering" << std::endl;
	}
	int id(){
		return 99;
	}
private:
	int serial;
};

int _tmain(int argc, _TCHAR* argv[])
{
	Gameloop loop(4);

	Scene& sc = loop.createScene();
	loop.setActiveScene(0);
	GameObject& obj1 = sc.createObject();
	GameObject& obj2 = sc.createObject();
	Testprinter* tp1 = new Testprinter(obj1);
	Testprinter* tp2 = new Testprinter(obj2);

	loop.start();
	char c;
	std::cin >> c;
	return 0;
}

