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

void AnimationCallback() {
	std::cout << "animation callback\n";
}

oppo::Result gameloop(oppo::Event event) {
	switch (event.type) {
	case oppo::EVENTS::CREATE:
		return 0;
	case oppo::EVENTS::CLOSE: // returning 0 closes the window, returning -1 cancels the close
		return 0;
	case oppo::EVENTS::DESTROY:
		return 0;
	case oppo::EVENTS::KEYDOWN:
		if (event.key == oppo::KEYS::SPACE) {
			wm.AddAnimation(&sprite.spriteIndex, { {0,1}, {0,2}, {0,3}, {1,0}, {1,1}, {1,2}, {1,3}, {0,0} }, 2, AnimationCallback);
		}
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
		else if (event.key == 'Q') {
			camera.rotation += 10;
		}
		else if (event.key == 'E') {
			camera.rotation -= 10;
		}
		return 0;
	case oppo::EVENTS::KEYUP:
		return 0;
	case oppo::EVENTS::CHAR: // for typing purposes
		return 0;
	case oppo::EVENTS::MOUSEMOVE:
		//std::cout << "mouse x: " << event.mouse.x << "\t" << "mouse y: " << event.mouse.y << std::endl;
		return 0;
	case oppo::EVENTS::MOUSESCROLL:
		return 0;
	case oppo::EVENTS::MOUSELEAVE:
		return 0;
	case oppo::EVENTS::MOUSEDOWN:
		return 0;
	case oppo::EVENTS::MOUSEUP:
		return 0;
	case oppo::EVENTS::MOUSEDBLCLK:
		return 0;
	case oppo::EVENTS::PAINT:
		brush.SetColor(oppo::Color(1., 0., .5));
		camera.DrawShape(oppo::RectF(100, 100, 160, 140), brush);
		brush.SetColor(oppo::Color(.2f, .4, 1.4, .2));
		camera.FillShape(oppo::Ellipse(oppo::Point2F(120, 130), 30, 20), brush);
		camera.DrawSprite(sprite);
		brush.SetColor(oppo::Color(oppo::COLORS::BLACK));
		camera.DrawText("testing this is text this is text this is text this is text text text text text", oppo::RectF(-100, -100, 100, 100), textFormat, brush, oppo::TEXT_CLIPPING::CLIP);
		camera.DrawShape(oppo::RectF(-100, -100, 100, 100), brush);
		return 0;
	case oppo::EVENTS::UPDATE:
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
		r = wm.CreateSpriteSheet("test spritesheet.png", oppo::Size2D(4, 4), oppo::Size2D(4, 4), oppo::Rect(), &SH);
		//r = wm.CreateSpriteSheetFromResource(MAKEINTRESOURCE(IDB_PNG1), oppo::Size2D(4, 4), oppo::Size2D(4, 4), oppo::Rect(), &SH);
	}
	if (oppo::Succeeded(r)) {
		r = wm.CreateSprite(&sprite, &SH, oppo::RectF(-50, -50, 50, 50), oppo::Size2D(0,0));
		sprite.position = oppo::Point2F(100, 100);
		sprite.rotation = 30;
		sprite.scale = { 2, 1 };
	}
	if (oppo::Succeeded(r)) {
		//r = wm.CreateBitmapFromResource(MAKEINTRESOURCE(IDB_PNG1), &bitmap);
		r = wm.CreateBitmap("test spritesheet.png", &bitmap);
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
