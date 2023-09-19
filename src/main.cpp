#include "resource.h"
#include <iostream>

#include "oppo.h"

using namespace oppo;

Window wnd;

Result NewScene(Event e) {
	static Camera cam;
	static Brush brush;
	switch (e.type) {
	case EVENTS::SETSCENE: {
		CameraProperties cp;
		cp.referencePoint = CAMERA_REFERENCE::CENTER;
		wnd.CreateCamera(&cam, cp);
		cam.ZoomToHeight(50);
		std::cout << cam.scale.height;
		BrushProperties bp;
		bp.color = COLORS::BLACK;
		bp.strokewidth = 3.f;
		wnd.CreateBrush(&brush, bp);
		break;
	}
	case EVENTS::ENDSCENE: {
		wnd.DestroyCamera(&cam);
		break;
	}
	case EVENTS::PAINT: {
		cam.DrawShape(RectF(-300, -25, 300, 25), brush);
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