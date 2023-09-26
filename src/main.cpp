#include "resource.h"
#include <iostream>

#include "oppo.h"

using namespace oppo;

Window wnd;
Cursor cursor;

Result NewScene(Event e) {
	static Camera cam;
	static Brush brush;
	switch (e.type) {
	case EVENTS::SETSCENE: {
		CameraProperties cp;
		cp.referencePoint = CAMERA_REFERENCE::CENTER;
		wnd.CreateCamera(&cam, cp);
		cam.ZoomToHeight(50);
		BrushProperties bp;
		bp.color = COLORS::BLACK;
		bp.strokewidth = 3.f;
		wnd.CreateBrush(&brush, bp);
		break;
	}
	case EVENTS::ENDSCENE: {
		wnd.DestroyCamera(&cam);
		wnd.DestroyBrush(&brush);
		break;
	}
	case EVENTS::PAINT: {
		cam.DrawShape(RectF(-300, -25, 300, 25), brush);
		break;
	}
	case EVENTS::MOUSEMOVE: {
		SetCursor(cursor);
		break;
	}
	case EVENTS::KEYDOWN: {
		if (e.as.keyDown.key == KEYS::LEFT) {
			cam.rotation -= 10;
		}
		else if (e.as.keyDown.key == KEYS::RIGHT) {
			cam.rotation += 10;
		}
		break;
	}
	}
	return 0;
}

Result MainMenu(Event e) {
	static Camera cam;
	static Brush brush;
	switch (e.type) {
	case EVENTS::SETSCENE: {
		CameraProperties cp;
		cp.referencePoint = CAMERA_REFERENCE::CENTER;
		wnd.CreateCamera(&cam, cp);
		cam.scale = { 2, 1 };
		cam.rotation = 45;
		BrushProperties bp;
		bp.color = COLORS::BLACK;
		bp.strokewidth = 3.f;
		wnd.CreateBrush(&brush, bp);
		break;
	}
	case EVENTS::ENDSCENE: {
		wnd.DestroyCamera(&cam);
		wnd.DestroyBrush(&brush);
		break;
	}
	case EVENTS::PAINT: {
		cam.DrawShape(RectF(-100, -100, 100, 100), brush);
		break;
	}
	case EVENTS::MOUSEDOWN: {
		if (e.as.mouseDown.key == KEYS::LMOUSE) {
			Point2F worldmouse = cam.ScreenToWorld(Point2D(e.as.mouseDown.x, e.as.mouseDown.y));
			float x = worldmouse.x;
			float y = worldmouse.y;
			std::cout << x << " " << y << "\n";
			if (x > -100 && x < 100 && y > -100 && y < 100) {
				wnd.SetScene(NewScene);
			}
		}
		break;
	}
	}
	return 0;
}

int main() {
	WindowProperties wp;
	wp.GameLoop = MainMenu; // set the game event handler function
	wp.id = 0; // id can be any number
	wp.backgroundColor = COLORS::CORAL; // default background color
	CreateWindow(&wnd, wp);
	cursor = LoadCursor(NULL, IDC_SIZEALL);
	Run();
}