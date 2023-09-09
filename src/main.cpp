#include "oppo.h"
#include "resource.h"
#include <iostream>

using namespace oppo;

Window wnd;
Window wnd2;

Result GameLoop(Event e) {
	if (e.type == EVENTS::KEYDOWN) {
		if (e.as.keyDown.key == KEYS::SPACE) {
			wnd.SetFullscreen(true);
		}
		else if (e.as.keyDown.key == KEYS::ESCAPE) {
			wnd.SetFullscreen(false);
		}
	}
	return 0;
}

int main() {
	WindowProperties wp;
	wp.style = WNDSTYLE::POPUP;
	wp.GameLoop = GameLoop; // set the game event handler function
	wp.id = 0; // id can be any number
	wp.name = "window name";
	wp.backgroundColor = Color(COLORS::CORAL); // default background color
	CreateWindow(&wnd, wp);
	Run();
	return 0;
}
