#include "oppo.h"
#include "resource.h"
#include <iostream>

oppo::WindowManager wm;
oppo::Brush brush;
oppo::Camera camera;
oppo::SpriteSheet SH;
oppo::Sprite sprite;
oppo::Bitmap bitmap;
oppo::TextFormat textFormat;

bool right = false;
bool left = false;
bool running = false;

oppo::AnimationID idleID = 0;
oppo::AnimationID movingID = 0;

void AnimationCallback() {
	if (running) {
		movingID = wm.AddAnimation(&sprite.spriteIndex, { {0,1},{0,1},{0,1},{0,1},{1,1},{1,1},{1,1},{1,1} }, 1, AnimationCallback);
	}
}

oppo::Result gameloop(oppo::Event event) {
	switch (event.type) {
	case oppo::EVENTS::KEYUP: {
		if (event.key == oppo::KEYS::LEFT) {
			left = false;
		}
		if (event.key == oppo::KEYS::RIGHT) {
			right = false;
		}
		if (!left && !right) {
			running = false;
			wm.RemoveAnimation(movingID);
		}
		return 0;
	}
	case oppo::EVENTS::KEYDOWN:
		switch (event.key) {
		case oppo::KEYS::RIGHT: {
			if (!wm.AnimationExists(movingID)) {
				movingID = wm.AddAnimation(&sprite.spriteIndex, { {0,1},{0,1},{0,1},{0,1},{1,1},{1,1},{1,1},{1,1} }, 1, AnimationCallback);
			}
			running = true;
			right = true;
			return 0;
		}
		case oppo::KEYS::LEFT: {
			if (!wm.AnimationExists(movingID)) {
				movingID = wm.AddAnimation(&sprite.spriteIndex, { {0,1},{0,1},{0,1},{0,1},{1,1},{1,1},{1,1},{1,1} }, 1, AnimationCallback);
			}
			running = true;
			left = true;
			return 0;
		}
		case oppo::KEYS::SPACE: {
			wm.ResumeAnimation(idleID);
			std::cout << idleID;
			return 0;
		}
		case oppo::KEYS::ESCAPE: {
			wm.PauseAnimation(idleID);
			return 0;
		}
		}
		return 0;
	case oppo::EVENTS::PAINT:
		brush.SetColor(oppo::Color(1., 0., .5));
		camera.DrawShape(oppo::RectF(-20, -20, 20, 20), brush);
		camera.DrawSprite(sprite);
		return 0;
	case oppo::EVENTS::UPDATE:
		if (running) {
			if (right) sprite.position.x += 200 * event.dt;
			else sprite.position.x -= 200 * event.dt;
		}
		return 0;
	}
	return 0;
}

int main() {
	// init window
	oppo::WindowPackage wp;
	wp.backgroundColor = oppo::Color(oppo::COLORS::CORAL);
	wp.windowName = "this is the name of the window";
	wp.szMin = oppo::Size2D(600, 300);
	wp.szMax = oppo::Size2D(0, 0);
	wp.szScreen = oppo::Size2D(600, 300);
	//wp.aspectRatio = 2;
	wp.ups = 100;
	wp.aps = 30;
	wm.RegisterGameLoop(gameloop);
	oppo::Result r = wm.Init(wp);

	if (oppo::Succeeded(r)) {
		r = wm.CreateCamera(&camera);
	}
	if (oppo::Succeeded(r)) {
		r = wm.CreateBrush(&brush);
	}
	if (oppo::Succeeded(r)) {
		r = wm.CreateSpriteSheet("test spritesheet.png", oppo::Size2D(8, 8), oppo::Size2D(2, 2), oppo::Rect(), &SH);
		//r = wm.CreateSpriteSheetFromResource(MAKEINTRESOURCE(IDB_PNG1), oppo::Size2D(4, 4), oppo::Size2D(4, 4), oppo::Rect(), &SH);
	}
	if (oppo::Succeeded(r)) {
		r = wm.CreateSprite(&sprite, &SH, oppo::RectF(-50, -50, 50, 50), oppo::Size2D(0,0));
		idleID = wm.AddAnimation(&sprite.spriteIndex, { {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{1,0},{1,0},{1,0},{1,0},{1,0},{1,0},{1,0}, }, -1);
		sprite.position = oppo::Point2F(100, 100);
	}
	if (oppo::Succeeded(r)) {
		oppo::TextFormatProperties tfp;
		tfp.fontSize = 80.f;
		tfp.fontName = oppo::FONTNAMES[oppo::FONTS::GABRIOLA];
		tfp.fontWeight = oppo::FONT_WEIGHTS::NORMAL;
		tfp.hAlignment = oppo::TEXT_HORIZONTAL_ALIGNMENT::RIGHT;
		tfp.vAlignment = oppo::TEXT_VERTICAL_ALIGNMENT::CENTER;
		tfp.wordWrapping = oppo::TEXT_WRAPPING::CHARACTER_WRAP;
		tfp.fontStretch = oppo::FONT_STRETCHES::NORMAL;
		tfp.lineSpacing = 1;
		r = wm.CreateTextFormat(&textFormat, tfp);
	}
	if (oppo::Succeeded(r)) {
		wm.Run();
	}

	return 0;
}
