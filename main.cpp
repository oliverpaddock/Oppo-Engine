#include "oppo.h"
#include <iostream>

oppo::Brush brush;
oppo::Brush brush2;
oppo::Camera camera;
oppo::Camera cam2;

oppo::Result gameloop(oppo::Event event) {
	switch (event.type) {
	case oppo::EVENTS::CREATE:
		std::cout << "create\n";
		return 0;
	case oppo::EVENTS::CLOSE: // returning 0 closes the window, returning -1 cancels the close
		std::cout << "close\n";
		return 0;
	case oppo::EVENTS::DESTROY:
		std::cout << "destroy\n";
		return 0;
	case oppo::EVENTS::KEYDOWN:
		std::cout << "keydown: " << event.key << std::endl;
		if (event.key == 'A') {
			camera.position.x -= 10;
		}
		else if (event.key == 'D') {
			camera.position.x += 10;
		}
		else if (event.key == 'W') {
			camera.position.y -= 10;
		}
		else if (event.key == 'S') {
			camera.position.y += 10;
		} 
		else if (event.key == oppo::KEYS::LEFT) {
			cam2.position.x -= 10;
		}
		else if (event.key == oppo::KEYS::RIGHT) {
			cam2.position.x += 10;
		}
		else if (event.key == oppo::KEYS::UP) {
			cam2.position.y -= 10;
		}
		else if (event.key == oppo::KEYS::DOWN) {
			cam2.position.y += 10;
		}
		else if (event.key == oppo::KEYS::SPACE) {
			brush.strokeWidth = 3;
		}
		else if (event.key == oppo::KEYS::BACK) {
			brush.strokeWidth = 1;
		}
		return 0;
	case oppo::EVENTS::KEYUP:
		std::cout << "keyup: " << event.aParam << std::endl;
		return 0;
	case oppo::EVENTS::CHAR: // for typing purposes
		std::cout << (char)event.aParam << std::endl;
		return 0;
	case oppo::EVENTS::MOUSEMOVE:
		//std::cout << std::hex << "mouse x: " << event.x << "\t" << "mouse y: " << event.y << std::endl;
		//std::cout << "mouse x: " << (event.aParam & 0xffffffff) << "\t" << "mouse y: " << ((event.aParam >> 32) & 0xffffffff) << std::endl;
		return 0;
	case oppo::EVENTS::MOUSESCROLL:
		std::cout << "scroll:\t" << event.bParam << "\t" << event.aParam << std::endl;
		return 0;
	case oppo::EVENTS::MOUSELEAVE:
		std::cout << "mouseleave\n";
		return 0;
	case oppo::EVENTS::MOUSEDOWN:
		std::cout << "mouse down:\t" << event.key << std::endl;
		return 0;
	case oppo::EVENTS::MOUSEUP:
		std::cout << "mouse up:\t" << event.key << std::endl;
		return 0;
	case oppo::EVENTS::MOUSEDBLCLK:
		std::cout << "mouse dbl click:\t" << event.key << std::endl;
		return 0;
	case oppo::EVENTS::PAINT:
		brush.SetColor(oppo::Color(1., 0., .5));
		camera.DrawShape(oppo::RectF(100, 100, 160, 140), brush);
		brush.SetColor(oppo::Color(.2f, .4, 1.4, .2));
		cam2.FillShape(oppo::Ellipse(oppo::Point2F(120, 130), 30, 20), brush);
		return 0;
	case oppo::EVENTS::UPDATE:
		return 0;
	}
	return 0;
}

int main() {
	// init window
	oppo::WindowManager wm;
	oppo::WindowPackage wp;
	wp.backgroundColor = oppo::Color(D2D1::ColorF::Coral);
	wp.windowName = "this is the name of the window";
	wp.szMin = oppo::Size2D(500, 300);
	wp.szMax = oppo::Size2D(1000, 0);
	wp.szScreen = oppo::Size2D(750, 400);
	wp.ups = 500;
	wm.RegisterGameLoop(gameloop);
	oppo::Result r = wm.Init(wp);

	if (oppo::Succeeded(r)) {
		r = wm.CreateCamera(&camera);
	}
	if (oppo::Succeeded(r)) {
		r = wm.CreateCamera(&cam2);
	}
	if (oppo::Succeeded(r)) {
		r = wm.CreateBrush(&brush);
	}

	if (oppo::Succeeded(r)) {
		wm.Run();
	}

	return 0;
}
