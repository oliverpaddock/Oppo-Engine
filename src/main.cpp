#include "resource.h"
#include <iostream>

#include "oppo.h"

using namespace oppo;

Window wnd;

Result NewScene(Event e) {
	static Camera camera;
	switch (e.type) {
	case EVENTS::SETSCENE: {
		CameraProperties cp;
		cp.referencePoint = CAMERA_REFERENCE::CENTER;
		wnd.CreateCamera(&camera, cp);
		break;
	}
	case EVENTS::ENDSCENE: {
		wnd.DestroyCamera(&camera);
		break;
	}
	}
	return 0;
}

int main() {
	WindowProperties wp;
	wp.GameLoop = NewScene; // set the game event handler function
	wp.id = 0; // id can be any number
	wp.backgroundColor = COLORS::CORAL; // default background color
	CreateWindow(&wnd, wp);
	Run();
}