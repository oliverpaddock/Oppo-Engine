#include "opgraphics.h"
#include <iostream>
#include <cmath>
#pragma region Utility
std::wstring oppo::utility::StringToWString(const std::string& str)
{
	int count = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), NULL, 0);
	std::wstring wstr(count, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), &wstr[0], count);
	return wstr;
}
#pragma endregion

#pragma region Window
#pragma region Window::public
// create window-dependent resources
oppo::Result oppo::Window::CreateBrush(Brush* pBrush, BrushProperties properties) {
	if (SUCCEEDED(resourceManager.CreateBrush(pBrush, properties))) {
		return ERRORS::SUCCESS;
	}
	return ERRORS::FAIL;
}
oppo::Result oppo::Window::CreateBitmap(Bitmap* pBitmap, BitmapProperties properties) {
	if (SUCCEEDED(resourceManager.CreateBitmap(pBitmap, properties))) {
		return ERRORS::SUCCESS;
	}
	return ERRORS::FAIL;
}
oppo::Result oppo::Window::CreateSpriteSheet(SpriteSheet* pSpriteSheet, SpriteSheetProperties properties) {
	if (SUCCEEDED(resourceManager.CreateSpriteSheet(pSpriteSheet, properties))) {
		return ERRORS::SUCCESS;
	}
	return ERRORS::FAIL;
}
oppo::Result oppo::Window::CreateSprite(Sprite* pSprite, SpriteProperties properties) {
	if (SUCCEEDED(resourceManager.CreateSprite(pSprite, properties))) {
		return ERRORS::SUCCESS;
	}
	return ERRORS::FAIL;
}
oppo::Result oppo::Window::CreateTextFormat(TextFormat* pTextFormat, TextFormatProperties properties) {
	if (SUCCEEDED(resourceManager.CreateTextFormat(pTextFormat , properties))) {
		return ERRORS::SUCCESS;
	}
	return ERRORS::FAIL;
}
oppo::Result oppo::Window::CreateTileMap(TileMap* pTileMap, TileMapProperties properties) {
	if (SUCCEEDED(resourceManager.CreateTileMap(pTileMap, properties))) {
		return ERRORS::SUCCESS;
	}
	return ERRORS::FAIL;
}
oppo::Result oppo::Window::CreateEffect(Effect* pEffect, EffectProperties properties) {
	if (SUCCEEDED(resourceManager.CreateEffect(pEffect, properties))) {
		return ERRORS::SUCCESS;
	}
	return ERRORS::FAIL;
}
oppo::Result oppo::Window::CreateCamera(Camera* pCamera, CameraProperties properties) {
	if (SUCCEEDED(resourceManager.CreateCamera(pCamera, properties))) {
		return ERRORS::SUCCESS;
	}
	return ERRORS::FAIL;
}

// destroy window-dependent resources
void oppo::Window::DestroyBrush(Brush* pBrush) {
	resourceManager.DestroyBrush(pBrush);
}
void oppo::Window::DestroyBitmap(Bitmap* pBitmap) {
	resourceManager.DestroyBitmap(pBitmap);
}
void oppo::Window::DestroySpriteSheet(SpriteSheet* pSpriteSheet) {
	resourceManager.DestroySpriteSheet(pSpriteSheet);
}
void oppo::Window::DestroySprite(Sprite* pSprite) {
	resourceManager.DestroySprite(pSprite);
}
void oppo::Window::DestroyTextFormat(TextFormat* pTextFormat) { 
	resourceManager.DestroyTextFormat(pTextFormat);
}
void oppo::Window::DestroyTileMap(TileMap* pTileMap) {}
void oppo::Window::DestroyEffect(Effect* pEffect) {}
void oppo::Window::DestroyCamera(Camera* pCamera) {}

// animations
oppo::Result oppo::Window::RemoveAnimation(AnimationID& id) {
	return animationManager.RemoveAnimation(id);
}
oppo::Result oppo::Window::PauseAnimation(AnimationID id) {
	return animationManager.PauseAnimation(id);
}
oppo::Result oppo::Window::ResumeAnimation(AnimationID id) {
	return animationManager.ResumeAnimation(id);
}
oppo::Result oppo::Window::ResetAnimation(AnimationID id) {
	return animationManager.ResetAnimation(id);
}
bool oppo::Window::AnimationExists(AnimationID id) {
	return animationManager.AnimationExists(id);
}

// set window properties
void oppo::Window::SetFPS(float fps) {
	if (fps == 0) {
		renderCountTarget = -1; // avoid divide by zero error
	}
	else {
		renderCountTarget = (int)(1000000 / fps); // game loop timers are in microseconds
	}
}
void oppo::Window::SetUPS(float ups) {
	if (ups == 0) {
		updateCountTarget = -1; // avoid divide by zero error
	}
	else {
		updateCountTarget = (int)(1000000 / ups); // game loop timers are in microseconds
	}
}
void oppo::Window::SetAPS(float aps) {
	if (aps == 0) {
		animateCountTarget = -1; // avoid divide by zero error
	}
	else {
		animateCountTarget = (int)(1000000 / aps); // game loop timers are in microseconds
	}
}
void oppo::Window::SetSize(Size2D size) {
	SetWindowPos(hWnd, HWND_TOPMOST, NULL, NULL, size.width, size.height, SWP_NOMOVE);
}
void oppo::Window::SetPosition(Point2D position) {
	SetWindowPos(hWnd, HWND_TOPMOST, position.x, position.y, NULL, NULL, SWP_NOSIZE);
}
void oppo::Window::SetMousePosition(Point2D position) {
	SetCursorPos(position.x, position.y);
}
void oppo::Window::SetMouseCapture(bool isCaptured) {
	if (isCaptured) {
		SetCapture(hWnd);
	}
	else {
		ReleaseCapture();
	}
}
void oppo::Window::SetMaximize(bool isMaximized) {
	if (isMaximized) {
		PostMessage(hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
	}
	else {
		PostMessage(hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
	}
}
void oppo::Window::SetMinimize(bool isMinimized) {
	if (isMinimized) {
		PostMessage(hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
	}
	else {
		PostMessage(hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
	}
}
void oppo::Window::SetFullscreen(bool isFullscreen) {
	if (isFullscreen && !this->isFullscreen) {
		dwStylePrev = GetWindowLong(hWnd, GWL_STYLE);
		this->isFullscreen = true;
		MONITORINFO mi = { sizeof(mi) };
		if (GetWindowPlacement(hWnd, &wpPrev) && GetMonitorInfo(MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST), &mi)) {
			SetWindowLong(hWnd, GWL_STYLE, dwStylePrev & ~(WS_CAPTION | WS_THICKFRAME));
			SetWindowPos(hWnd, HWND_TOP,
				mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top,
				SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		}
	}
	else if (!isFullscreen && this->isFullscreen) {
		this->isFullscreen = false;
		SetWindowLong(hWnd, GWL_STYLE, dwStylePrev);
		SetWindowPlacement(hWnd, &wpPrev);
		SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}
}

void oppo::Window::SetScene(std::function<Result(Event)> newScene) {
	Event e;
	e.windowID = winID;

	if (GameLoop) {
		e.type = EVENTS::ENDSCENE;
		GameLoop(e);
	}

	if (newScene) {
		GameLoop = newScene;
		e.type = EVENTS::SETSCENE;
		GameLoop(e);
	}
}

// get window properties
oppo::Size2D oppo::Window::GetSize() {
	RECT rc;
	GetWindowRect(hWnd, &rc);
	return Size2D(rc.right - rc.left, rc.bottom - rc.top);
}
oppo::Point2D oppo::Window::GetPosition() {
	RECT rc;
	GetWindowRect(hWnd, &rc);
	return Point2D(rc.left, rc.top);
}
#pragma endregion

#pragma region Window::private
void oppo::Window::NewClassName() {
	std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
	std::uniform_int_distribution<int> distribution(0x0041, 0x007A);
	for (int i = 0; i < 8; i++) {
		className[i] = L'A';
	}
	for (int i = 8; i < 16; i++) {
		className[i] = static_cast<wchar_t>(distribution(rng));
	}
	className[16] = L'\0';
}
void oppo::Window::GameLoopTimer() {
	long long renderCount;
	long long updateCount;
	long long animateCount;
	swRender.Reset();
	swUpdate.Reset();
	swAnimate.Reset();

	while (wndState != WNDSTATE::DESTROY) {
		renderCount = swRender.Elapsed().count();
		updateCount = swUpdate.Elapsed().count();
		animateCount = swAnimate.Elapsed().count();

		if (animateCount >= animateCountTarget && wndState == WNDSTATE::RUN) {
			swAnimate.Reset();
			PostMessage(hWnd, WM_FRAME, animateCount, 2);
		}

		if (renderCount >= renderCountTarget && wndState == WNDSTATE::RUN) {
			swRender.Reset();
			PostMessage(hWnd, WM_FRAME, renderCount, 1);
			InvalidateRect(hWnd, NULL, FALSE);
		}

		if (updateCount >= updateCountTarget && wndState == WNDSTATE::RUN) {
			swUpdate.Reset();
			PostMessage(hWnd, WM_FRAME, updateCount, 0);
		}

	}
}
oppo::KEYS oppo::Window::TranslateKeystroke(int vkCode) {
	if (vkCode >= '0' && vkCode <= '9') {
		return (KEYS)vkCode;
	}
	if (vkCode >= 'A' && vkCode <= 'Z') {
		return (KEYS)vkCode;
	}
	if (vkCode >= VK_NUMPAD0 && vkCode <= VK_NUMPAD9) {
		return (KEYS)(vkCode + 31);
	}
	if (vkCode >= VK_F1 && vkCode <= VK_F24) {
		return (KEYS)(vkCode + 31);
	}
	switch (vkCode) {
	case VK_BACK: // backspace
		return KEYS::BACK;
	case VK_TAB: // tab
		return KEYS::TAB;
	case VK_RETURN: // enter
		return KEYS::ENTER;
	case VK_SHIFT: // shift
		return KEYS::SHIFT;
	case VK_CONTROL: // control
		return KEYS::CTRL;
	case VK_CAPITAL: // capslock
		return KEYS::CAPS;
	case VK_ESCAPE: // escape
		return KEYS::ESCAPE;
	case VK_SPACE: // spacebar
		return KEYS::SPACE;
	case VK_PRIOR: // pg up
		return KEYS::PGUP;
	case VK_NEXT: // pg down
		return KEYS::PGDN;
	case VK_END: // end
		return KEYS::END;
	case VK_HOME: // home
		return KEYS::HOME;
	case VK_LEFT: // left arrow
		return KEYS::LEFT;
	case VK_UP: // up arrow
		return KEYS::UP;
	case VK_RIGHT: // right arrow
		return KEYS::RIGHT;
	case VK_DOWN: // down arrow
		return KEYS::DOWN;
	case VK_INSERT: // insert
		return KEYS::INSERT;
	case VK_DELETE: // delete
		return KEYS::DELETE;
	case VK_MENU:
		return KEYS::ALT;
	case VK_MULTIPLY: // numpad multiply
		return KEYS::NP_MULTIPLY;
	case VK_ADD: // numpad add
		return KEYS::NP_ADD;
	case VK_SUBTRACT: // numpad subtract
		return KEYS::NP_SUBTRACT;
	case VK_DECIMAL: // numpad decimal
		return KEYS::NP_DECIMAL;
	case VK_DIVIDE: // numpad divide
		return KEYS::NP_DIVIDE;
	case VK_NUMLOCK: // num lock
		return KEYS::NP_LOCK;
	case VK_SCROLL: // scroll lock
		return KEYS::SCROLL_LOCK;
	case VK_VOLUME_MUTE: // volume mute
		return KEYS::MEDIA_VOLUME_MUTE;
	case VK_VOLUME_DOWN: // volume down
		return KEYS::MEDIA_VOLUME_DOWN;
	case VK_VOLUME_UP: // volume up
		return KEYS::MEDIA_VOLUME_UP;
	case VK_MEDIA_NEXT_TRACK: // next track, not typically found on keyboards but included for specialized controllers
		return KEYS::MEDIA_NEXT;
	case VK_MEDIA_PREV_TRACK: // previous track, not typically found on keyboards but included for specialized controllers
		return KEYS::MEDIA_PREV;
	case VK_MEDIA_STOP: // stop media, not typically found on keyboards but included for specialized controllers
		return KEYS::MEDIA_STOP;
	case VK_OEM_1: // ;:
		return KEYS::COLON;
	case VK_OEM_2: // /?
		return KEYS::QMARK;
	case VK_OEM_3: // ~
		return KEYS::TILDE;
	case VK_OEM_4: // [{
		return KEYS::LBRACKET;
	case VK_OEM_5: // \|
		return KEYS::BSLASH;
	case VK_OEM_6: // ]}
		return KEYS::RBRACKET;
	case VK_OEM_7: // '"
		return KEYS::QUOTE;
	case VK_OEM_PLUS: // =+
		return KEYS::PLUS;
	case VK_OEM_COMMA: // ,<
		return KEYS::COMMA;
	case VK_OEM_MINUS: // -_
		return KEYS::MINUS;
	case VK_OEM_PERIOD: // .>
		return KEYS::PERIOD;
	default:
		return KEYS::EXTRA;
	}
}
LRESULT CALLBACK oppo::Window::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	Event e;
	e.windowID = winID;
	switch (uMsg) {
	case WM_CREATE: {
		resourceManager.Init(this);
		if (FAILED(resourceManager.CreateWindowResources(hWnd))) {
			return -1;
		}
		if (GameLoop) {
			e.type = EVENTS::CREATE;
			GameLoop(e);
			e.type = EVENTS::SETSCENE;
			GameLoop(e);
		}
		// set initial fullscreen parameters
		GetWindowPlacement(hWnd, &wpPrev);
		dwStylePrev = GetWindowLong(hWnd, GWL_STYLE);
		wndState = WNDSTATE::RUN;
		return 0;
	}
	case WM_CLOSE: {
		if (GameLoop) {
			e.type = EVENTS::CLOSE;
			Result r = GameLoop(e);
			if (Succeeded(r)) {
				DestroyWindow(hWnd);
			}
		}
		else {
			DestroyWindow(hWnd);
		}
		return 0;
	}
	case WM_DESTROY: {
		wndState = WNDSTATE::DESTROY;
		if (GameLoop) {
			e.type = EVENTS::ENDSCENE;
			GameLoop(e);
			e.type = EVENTS::DESTROY;
			GameLoop(e);
		}
		resourceManager.DestroyWindowResources();
		_Engine::RemoveWindow(this);
		return 0;
	}
	case WM_SIZE: {
		(*ppRT)->Resize(D2D1::SizeU(LOWORD(lParam), HIWORD(lParam)));
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;
	}
	case WM_SIZING: {
		if (aspectRatio != 0) {
			RECT* rc;
			rc = (RECT*)lParam;
			if (wParam != WMSZ_LEFT && wParam != WMSZ_TOPLEFT && wParam != WMSZ_BOTTOMLEFT && wParam != WMSZ_RIGHT) {
				rc->right = (rc->bottom - rc->top) * aspectRatio + rc->left;
			}
			else if (wParam == WMSZ_LEFT || wParam == WMSZ_RIGHT) {
				rc->bottom = (rc->right - rc->left) * 1 / aspectRatio + rc->top;
			}
			else {
				rc->left = rc->right - (rc->bottom - rc->top) * aspectRatio;
			}
		}
		return TRUE;
	}
	case WM_GETMINMAXINFO: {
		LPMINMAXINFO lpMMI = (LPMINMAXINFO)(lParam);
		if (szMin.width) {
			lpMMI->ptMinTrackSize.x = szMin.width;
		}
		if (szMin.height) {
			lpMMI->ptMinTrackSize.y = szMin.height;
		}
		if (szMax.width) {
			lpMMI->ptMaxTrackSize.x = szMax.width;
		}
		if (szMax.height) {
			lpMMI->ptMaxTrackSize.y = szMax.height;
		}
		return 0;
	}

	case WM_PAINT: {
		RECT rc;
		GetClientRect(hWnd, &rc);
		ValidateRect(hWnd, &rc);

		(*ppRT)->BeginDraw();
		(*ppRT)->SetTransform(D2D1::IdentityMatrix());
		(*ppRT)->Clear(D2D1::ColorF(backgroundColor.R, backgroundColor.G, backgroundColor.B, backgroundColor.a));

		if (GameLoop) {
			e.type = EVENTS::PAINT;
			GameLoop(e);
		}

		if (*ppCurrentLayer != nullptr) {
			(*ppRT)->PopLayer();
			*ppCurrentLayer = nullptr;
		}

		HRESULT hr = (*ppRT)->EndDraw();

		if (hr == D2DERR_RECREATE_TARGET) {
			resourceManager.RecreateDDResources(hWnd);
			InvalidateRect(hWnd, NULL, FALSE);
		}

		return 0;
	}
	case WM_FRAME: {
		if (GameLoop) {
			long long usCount = (long long)(wParam);
			if (lParam == 0) {
				// update
				e.as.update.dt = usCount / 1000000.;
				e.type = EVENTS::UPDATE;
				GameLoop(e);
			}
			else if (lParam == 1) {
				// render
				InvalidateRect(hWnd, NULL, FALSE);
			}
			else if (lParam == 2) {
				// animate
				animationManager.NextFrame();
			}
		}

		return 0;
	}

				 // keyboard
	case WM_KEYDOWN: {
		WORD vkCode = LOWORD(wParam);
		WORD keyFlags = HIWORD(lParam);
		BOOL wasKeyDown = (keyFlags & KF_REPEAT) == KF_REPEAT;

		if (GameLoop && !wasKeyDown) {
			e.type = EVENTS::KEYDOWN;
			e.as.keyDown.key = TranslateKeystroke(vkCode);
			GameLoop(e);
		}

		return 0;
	}
	case WM_KEYUP: {
		WORD vkCode = LOWORD(wParam);

		if (GameLoop) {
			e.type = EVENTS::KEYUP;
			e.as.keyUp.key = TranslateKeystroke(vkCode);
			GameLoop(e);
		}

		return 0;
	}
	case WM_CHAR: {
		if (GameLoop) {
			e.type = EVENTS::CHAR;
			e.as.character.character = static_cast<char>(wParam);
			e.as.character.repeat = LOWORD(lParam);
			GameLoop(e);
		}

		return 0;
	}
	case WM_SYSKEYDOWN: {
		// generated when alt key is down
		WORD vkCode = LOWORD(wParam);
		WORD keyFlags = HIWORD(lParam);
		BOOL wasKeyDown = (keyFlags & KF_REPEAT) == KF_REPEAT;

		if (GameLoop && !wasKeyDown) {
			e.type = EVENTS::KEYDOWN;
			e.as.keyDown.key = TranslateKeystroke(vkCode);
			e.as.keyDown.alt = true;
			GameLoop(e);
		}

		return 0;
	}
	case WM_SYSKEYUP: {
		WORD vkCode = LOWORD(wParam);

		if (GameLoop) {
			e.type = EVENTS::KEYUP;
			e.as.keyUp.key = TranslateKeystroke(vkCode);
			e.as.keyUp.alt = true;
			GameLoop(e);
		}

		return 0;
	}
	case WM_SYSCHAR: {
		if (GameLoop) {
			e.type = EVENTS::CHAR;
			e.as.character.character = static_cast<char>(wParam);
			e.as.character.repeat = LOWORD(lParam);
			e.as.character.alt = 1;
			GameLoop(e);
		}

		return 0;
	}

				   // mouse
	case WM_MOUSEMOVE: {
		if (GameLoop) {
			e.type = EVENTS::MOUSEMOVE;
			e.as.mouseMove.x = GET_X_LPARAM(lParam);
			e.as.mouseMove.y = GET_Y_LPARAM(lParam);
			if (GetKeyState(VK_MENU) & 0x8000) {
				e.as.mouseMove.alt = true;
			}
			GameLoop(e);
		}
		return 0;
	}
	case WM_MOUSEWHEEL: {
		if (GameLoop) {
			e.type = EVENTS::MOUSESCROLL;
			e.as.mouseScroll.vScroll = GET_WHEEL_DELTA_WPARAM(wParam);
			e.as.mouseScroll.x = GET_X_LPARAM(lParam);
			e.as.mouseScroll.y = GET_Y_LPARAM(lParam);
			if (GetKeyState(VK_MENU) & 0x8000) {
				e.as.mouseScroll.alt = true;
			}
			GameLoop(e);
		}
		return 0;
	}
	case WM_MOUSEHWHEEL: {
		if (GameLoop) {
			e.type = EVENTS::MOUSESCROLL;
			e.as.mouseScroll.hScroll = GET_WHEEL_DELTA_WPARAM(wParam);
			e.as.mouseScroll.x = GET_X_LPARAM(lParam);
			e.as.mouseScroll.y = GET_Y_LPARAM(lParam);
			if (GetKeyState(VK_MENU) & 0x8000) {
				e.as.mouseScroll.alt = true;
			}
			GameLoop(e);
		}
		return 0;
	}
	case WM_LBUTTONDOWN: {
		if (GameLoop) {
			e.type = EVENTS::MOUSEDOWN;
			e.as.mouseDown.x = GET_X_LPARAM(lParam);
			e.as.mouseDown.y = GET_Y_LPARAM(lParam);
			e.as.mouseDown.key = KEYS::LMOUSE;
			if (GetKeyState(VK_MENU) & 0x8000) {
				e.as.mouseDown.alt = true;
			}
			GameLoop(e);
		}

		return 0;
	}
	case WM_LBUTTONUP: {
		if (GameLoop) {
			e.type = EVENTS::MOUSEDOWN;
			e.as.mouseUp.x = GET_X_LPARAM(lParam);
			e.as.mouseUp.y = GET_Y_LPARAM(lParam);
			e.as.mouseUp.key = KEYS::LMOUSE;
			if (GetKeyState(VK_MENU) & 0x8000) {
				e.as.mouseUp.alt = true;
			}
			GameLoop(e);
		}

		return 0;
	}
	case WM_LBUTTONDBLCLK: {
		if (GameLoop) {
			e.type = EVENTS::MOUSEDBLCLK;
			e.as.mouseDblClk.x = GET_X_LPARAM(lParam);
			e.as.mouseDblClk.y = GET_Y_LPARAM(lParam);
			e.as.mouseDblClk.key = KEYS::LMOUSE;
			if (GetKeyState(VK_MENU) & 0x8000) {
				e.as.mouseDblClk.alt = true;
			}
			GameLoop(e);
		}

		return 0;
	}
	case WM_RBUTTONDOWN: {
		if (GameLoop) {
			e.type = EVENTS::MOUSEDOWN;
			e.as.mouseDown.x = GET_X_LPARAM(lParam);
			e.as.mouseDown.y = GET_Y_LPARAM(lParam);
			e.as.mouseDown.key = KEYS::RMOUSE;
			if (GetKeyState(VK_MENU) & 0x8000) {
				e.as.mouseDown.alt = true;
			}
			GameLoop(e);
		}

		return 0;
	}
	case WM_RBUTTONUP: {
		if (GameLoop) {
			e.type = EVENTS::MOUSEDOWN;
			e.as.mouseUp.x = GET_X_LPARAM(lParam);
			e.as.mouseUp.y = GET_Y_LPARAM(lParam);
			e.as.mouseUp.key = KEYS::RMOUSE;
			if (GetKeyState(VK_MENU) & 0x8000) {
				e.as.mouseUp.alt = true;
			}
			GameLoop(e);
		}

		return 0;
	}
	case WM_RBUTTONDBLCLK: {
		if (GameLoop) {
			e.type = EVENTS::MOUSEDBLCLK;
			e.as.mouseDblClk.x = GET_X_LPARAM(lParam);
			e.as.mouseDblClk.y = GET_Y_LPARAM(lParam);
			e.as.mouseDblClk.key = KEYS::RMOUSE;
			if (GetKeyState(VK_MENU) & 0x8000) {
				e.as.mouseDblClk.alt = true;
			}
			GameLoop(e);
		}

		return 0;
	}
	case WM_MBUTTONDOWN: {
		if (GameLoop) {
			e.type = EVENTS::MOUSEDOWN;
			e.as.mouseDown.x = GET_X_LPARAM(lParam);
			e.as.mouseDown.y = GET_Y_LPARAM(lParam);
			e.as.mouseDown.key = KEYS::MMOUSE;
			if (GetKeyState(VK_MENU) & 0x8000) {
				e.as.mouseDown.alt = true;
			}
			GameLoop(e);
		}

		return 0;
	}
	case WM_MBUTTONUP: {
		if (GameLoop) {
			e.type = EVENTS::MOUSEDOWN;
			e.as.mouseUp.x = GET_X_LPARAM(lParam);
			e.as.mouseUp.y = GET_Y_LPARAM(lParam);
			e.as.mouseUp.key = KEYS::MMOUSE;
			if (GetKeyState(VK_MENU) & 0x8000) {
				e.as.mouseUp.alt = true;
			}
			GameLoop(e);
		}

		return 0;
	}
	case WM_MBUTTONDBLCLK: {
		if (GameLoop) {
			e.type = EVENTS::MOUSEDBLCLK;
			e.as.mouseDblClk.x = GET_X_LPARAM(lParam);
			e.as.mouseDblClk.y = GET_Y_LPARAM(lParam);
			e.as.mouseDblClk.key = KEYS::MMOUSE;
			if (GetKeyState(VK_MENU) & 0x8000) {
				e.as.mouseDblClk.alt = true;
			}
			GameLoop(e);
		}

		return 0;
	}
	case WM_XBUTTONDOWN: {
		if (GameLoop) {
			e.type = EVENTS::MOUSEDOWN;
			e.as.mouseDown.x = GET_X_LPARAM(lParam);
			e.as.mouseDown.y = GET_Y_LPARAM(lParam);
			if (GET_XBUTTON_WPARAM(wParam) & XBUTTON1 == XBUTTON1) {
				e.as.mouseDown.key = KEYS::XMOUSE1;
			}
			else {
				e.as.mouseDown.key = KEYS::XMOUSE2;
			}
			if (GetKeyState(VK_MENU) & 0x8000) {
				e.as.mouseDown.alt = true;
			}
			GameLoop(e);
		}

		return 0;
	}
	case WM_XBUTTONUP: {
		if (GameLoop) {
			e.type = EVENTS::MOUSEUP;
			e.as.mouseUp.x = GET_X_LPARAM(lParam);
			e.as.mouseUp.y = GET_Y_LPARAM(lParam);
			if (GET_XBUTTON_WPARAM(wParam) & XBUTTON1 == XBUTTON1) {
				e.as.mouseUp.key = KEYS::XMOUSE1;
			}
			else {
				e.as.mouseUp.key = KEYS::XMOUSE2;
			}
			if (GetKeyState(VK_MENU) & 0x8000) {
				e.as.mouseUp.alt = true;
			}
			GameLoop(e);
		}

		return 0;
	}
	case WM_XBUTTONDBLCLK: {
		if (GameLoop) {
			e.type = EVENTS::MOUSEDBLCLK;
			e.as.mouseDblClk.x = GET_X_LPARAM(lParam);
			e.as.mouseDblClk.y = GET_Y_LPARAM(lParam);
			if (GET_XBUTTON_WPARAM(wParam) & XBUTTON1 == XBUTTON1) {
				e.as.mouseDblClk.key = KEYS::XMOUSE1;
			}
			else {
				e.as.mouseDblClk.key = KEYS::XMOUSE2;
			}
			if (GetKeyState(VK_MENU) & 0x8000) {
				e.as.mouseDblClk.alt = 1;
			}
			GameLoop(e);
		}

		return 0;
	}

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

}
#pragma endregion
#pragma endregion

#pragma region Brush
void oppo::Brush::SetColor(Color color) {
	this->color = color;
	if (pBrush) {
		pBrush->SetColor(D2D1::ColorF(color.R, color.G, color.B, color.a));
	}
}

oppo::Color oppo::Brush::GetColor() {
	return color;
}

#pragma endregion

#pragma region Sprite Sheet
oppo::Size2D oppo::SpriteSheet::SheetResolution() {
	return pxSheetResolution;
}

oppo::Size2D oppo::SpriteSheet::SpriteResolution() {
	return pxSpriteResolution;
}

oppo::Size2D oppo::SpriteSheet::SpriteCount() {
	return spriteCount;
}

oppo::Rect oppo::SpriteSheet::Padding() {
	return padding;
}

D2D1_RECT_F oppo::SpriteSheet::GetSpriteRect(Size2D spriteIndex) {
	return D2D1::RectF(
		spriteIndex.width * (padding.left + padding.right + pxSpriteResolution.width) + padding.left,
		spriteIndex.height * (padding.top + padding.bottom + pxSpriteResolution.height) + padding.top,
		(spriteIndex.width + 1) * (padding.left + padding.right + pxSpriteResolution.width) - padding.right,
		(spriteIndex.height + 1) * (padding.top + padding.bottom + pxSpriteResolution.height) - padding.bottom
		);
}
#pragma endregion

#pragma region Camera
void oppo::Camera::Fill(Color color) {
	SafePushLayer();
	(*ppRT)->Clear(D2D1::ColorF(color.R, color.G, color.B, color.a));
}
void oppo::Camera::Fill(Brush brush) {
	SafePushLayer();
	(*ppRT)->Clear(D2D1::ColorF(brush.color.R, brush.color.G, brush.color.B, brush.color.a));
}
void oppo::Camera::FillShape(Rect rect, Brush brush, Transform tf) {
	SafePushLayer();
	bool isIdentity = tf.IsIdentity();
	Transform cameraTF;
	if (!isIdentity) {
		(*ppRT)->GetTransform(&cameraTF);
		(*ppRT)->SetTransform(tf * cameraTF);
	}
	(*ppRT)->FillRectangle(D2D1::RectF(rect.left, rect.top, rect.right, rect.bottom), brush.pBrush);
	if (!isIdentity) {
		(*ppRT)->SetTransform(cameraTF);
	}
}
void oppo::Camera::FillShape(RectF rect, Brush brush, Transform tf) {
	SafePushLayer();
	bool isIdentity = tf.IsIdentity();
	Transform cameraTF;
	if (!isIdentity) {
		(*ppRT)->GetTransform(&cameraTF);
		(*ppRT)->SetTransform(tf * cameraTF);
	}
	(*ppRT)->FillRectangle(D2D1::RectF(rect.left, rect.top, rect.right, rect.bottom), brush.pBrush);
	if (!isIdentity) {
		(*ppRT)->SetTransform(cameraTF);
	}
}
void oppo::Camera::FillShape(RoundedRect roundedRect, Brush brush, Transform tf) {
	SafePushLayer();
	bool isIdentity = tf.IsIdentity();
	Transform cameraTF;
	if (!isIdentity) {
		(*ppRT)->GetTransform(&cameraTF);
		(*ppRT)->SetTransform(tf * cameraTF);
	}
	(*ppRT)->FillRoundedRectangle(D2D1::RoundedRect(D2D1::RectF(roundedRect.rect.left, roundedRect.rect.top, roundedRect.rect.right, roundedRect.rect.bottom), roundedRect.rx, roundedRect.ry), brush.pBrush);
	if (!isIdentity) {
		(*ppRT)->SetTransform(cameraTF);
	}
}
void oppo::Camera::FillShape(Ellipse ellipse, Brush brush, Transform tf) {
	SafePushLayer();
	bool isIdentity = tf.IsIdentity();
	Transform cameraTF;
	if (!isIdentity) {
		(*ppRT)->GetTransform(&cameraTF);
		(*ppRT)->SetTransform(tf * cameraTF);
	}
	(*ppRT)->FillEllipse(D2D1::Ellipse(D2D1::Point2F(ellipse.center.x, ellipse.center.y), ellipse.rx, ellipse.ry), brush.pBrush);
	if (!isIdentity) {
		(*ppRT)->SetTransform(cameraTF);
	}
}
void oppo::Camera::DrawShape(Rect rect, Brush brush, Transform tf) {
	SafePushLayer();
	bool isIdentity = tf.IsIdentity();
	Transform cameraTF;
	if (!isIdentity) {
		(*ppRT)->GetTransform(&cameraTF);
		(*ppRT)->SetTransform(tf * cameraTF);
	}
	(*ppRT)->DrawRectangle(D2D1::RectF(rect.left, rect.top, rect.right, rect.bottom), brush.pBrush, brush.strokeWidth);
	if (!isIdentity) {
		(*ppRT)->SetTransform(cameraTF);
	}
}
void oppo::Camera::DrawShape(RectF rect, Brush brush, Transform tf) {
	SafePushLayer();
	bool isIdentity = tf.IsIdentity();
	Transform cameraTF;
	if (!isIdentity) {
		(*ppRT)->GetTransform(&cameraTF);
		(*ppRT)->SetTransform(tf * cameraTF);
	}
	(*ppRT)->DrawRectangle(D2D1::RectF(rect.left, rect.top, rect.right, rect.bottom), brush.pBrush, brush.strokeWidth);
	if (!isIdentity) {
		(*ppRT)->SetTransform(cameraTF);
	}
}
void oppo::Camera::DrawShape(RoundedRect roundedRect, Brush brush, Transform tf) {
	SafePushLayer();
	bool isIdentity = tf.IsIdentity();
	Transform cameraTF;
	if (!isIdentity) {
		(*ppRT)->GetTransform(&cameraTF);
		(*ppRT)->SetTransform(tf * cameraTF);
	}
	(*ppRT)->DrawRoundedRectangle(D2D1::RoundedRect(D2D1::RectF(roundedRect.rect.left, roundedRect.rect.top, roundedRect.rect.right, roundedRect.rect.bottom), roundedRect.rx, roundedRect.ry), brush.pBrush, brush.strokeWidth);
	if (!isIdentity) {
		(*ppRT)->SetTransform(cameraTF);
	}
}
void oppo::Camera::DrawShape(Ellipse ellipse, Brush brush, Transform tf) {
	SafePushLayer();
	bool isIdentity = tf.IsIdentity();
	Transform cameraTF;
	if (!isIdentity) {
		(*ppRT)->GetTransform(&cameraTF);
		(*ppRT)->SetTransform(tf * cameraTF);
	}
	(*ppRT)->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(ellipse.center.x, ellipse.center.y), ellipse.rx, ellipse.ry), brush.pBrush, brush.strokeWidth);
	if (!isIdentity) {
		(*ppRT)->SetTransform(cameraTF);
	}
}
void oppo::Camera::DrawShape(Line line, Brush brush, Transform tf) {
	SafePushLayer();
	bool isIdentity = tf.IsIdentity();
	Transform cameraTF;
	if (!isIdentity) {
		(*ppRT)->GetTransform(&cameraTF);
		(*ppRT)->SetTransform(tf * cameraTF);
	}
	(*ppRT)->DrawLine(D2D1::Point2F(line.p0.x, line.p0.y), D2D1::Point2F(line.p1.x, line.p1.y), brush.pBrush, brush.strokeWidth);
	if (!isIdentity) {
		(*ppRT)->SetTransform(cameraTF);
	}
}
void oppo::Camera::DrawShape(Bezier bezier, Brush brush, Transform tf) {
	SafePushLayer();
}
void oppo::Camera::DrawBitmap(Bitmap bitmap, RectF destRect, float opacity, RectF sourceRect, Transform tf) {
	SafePushLayer();
	bool isIdentity = tf.IsIdentity();
	Transform cameraTF;
	if (!isIdentity) {
		(*ppRT)->GetTransform(&cameraTF);
		(*ppRT)->SetTransform(tf * cameraTF);
	}
	(*ppRT)->DrawBitmap(bitmap.pBitmap, D2D1::RectF(destRect.left, destRect.top, destRect.right, destRect.bottom), opacity, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, D2D1::RectF(sourceRect.left, sourceRect.top, sourceRect.right, sourceRect.bottom));
	if (!isIdentity) {
		(*ppRT)->SetTransform(cameraTF);
	}
}
void oppo::Camera::DrawBitmap(Bitmap bitmap, RectF destRect, float opacity, Transform tf) {
	SafePushLayer();
	bool isIdentity = tf.IsIdentity();
	Transform cameraTF;
	if (!isIdentity) {
		(*ppRT)->GetTransform(&cameraTF);
		(*ppRT)->SetTransform(tf * cameraTF);
	}
	(*ppRT)->DrawBitmap(bitmap.pBitmap, D2D1::RectF(destRect.left, destRect.top, destRect.right, destRect.bottom), opacity, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
	if (!isIdentity) {
		(*ppRT)->SetTransform(cameraTF);
	}
}
void oppo::Camera::DrawSprite(Sprite sprite, float opacity, Transform tf) {
	SafePushLayer();
	bool isIdentity = tf.IsIdentity();
	Transform cameraTF;
	if (!isIdentity) {
		(*ppRT)->GetTransform(&cameraTF);
		(*ppRT)->SetTransform(tf * cameraTF);
	}
	(*ppRT)->DrawBitmap(sprite.pSpriteSheet->pBitmap, D2D1::RectF(sprite.rect.left, sprite.rect.top, sprite.rect.right, sprite.rect.bottom), opacity, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, sprite.pSpriteSheet->GetSpriteRect(sprite.spriteIndex));
	if (!isIdentity) {
		(*ppRT)->SetTransform(cameraTF);
	}
}
void oppo::Camera::DrawTileMap(TileMap tileMap, Transform tf) {
	SafePushLayer();
}
void oppo::Camera::DrawText(const char* text, RectF textBox, TextFormat textFormat, Brush brush, TEXT_CLIPPING clipOptions, Transform tf) {
	SafePushLayer();
	bool isIdentity = tf.IsIdentity();
	Transform cameraTF;
	if (!isIdentity) {
		(*ppRT)->GetTransform(&cameraTF);
		(*ppRT)->SetTransform(tf * cameraTF);
	}
	(*ppRT)->DrawTextW(
		utility::StringToWString(text).c_str(),
		strlen(text),
		textFormat.pTextFormat,
		D2D1::RectF(textBox.left, textBox.top, textBox.right, textBox.bottom),
		brush.pBrush,
		static_cast<D2D1_DRAW_TEXT_OPTIONS>(clipOptions)
	);
	if (!isIdentity) {
		(*ppRT)->SetTransform(cameraTF);
	}
}

void oppo::Camera::ZoomToWidth(float width) {
	Size2F sz = (*ppRT)->GetSize();
	float newScale = sz.width / width;
	scale = Size2F(newScale, newScale);
}
void oppo::Camera::ZoomToHeight(float height) {
	Size2F sz = (*ppRT)->GetSize();
	float newScale = sz.height / height;
	scale = Size2F(newScale, newScale);
}
void oppo::Camera::ZoomToFit(Size2F size) {
	Size2F currentSize = (*ppRT)->GetSize();
	float scaleX = currentSize.width / size.width;
	float scaleY = currentSize.height / size.height;
	float newScale = min(scaleX, scaleY);
	scale = Size2F(newScale, newScale);
}
void oppo::Camera::ZoomToFill(Size2F size) {
	Size2F currentSize = (*ppRT)->GetSize();
	float scaleX = currentSize.width / size.width;
	float scaleY = currentSize.height / size.height;
	float newScale = max(scaleX, scaleY);
	scale = Size2F(newScale, newScale);
}

oppo::RectF oppo::Camera::GetRect() {
	D2D1_SIZE_F sz = (*ppRT)->GetSize();
	sz.width /= scale.width;
	sz.height /= scale.height;
	RectF rc;
	if (refPoint == CAMERA_REFERENCE::CENTER) {
		rc.left = position.x - sz.width / 2;
		rc.top = position.y - sz.height / 2;
		rc.right = position.x + sz.width / 2;
		rc.bottom = position.y + sz.height / 2;
	}
	else {
		if (refPoint == CAMERA_REFERENCE::TOP_LEFT || refPoint == CAMERA_REFERENCE::BOTTOM_LEFT) {
			rc.left = position.x;
			rc.right = position.x + sz.width;
		}
		else {
			rc.left = position.x - sz.width;
			rc.right = position.x;
		}
		if (refPoint == CAMERA_REFERENCE::TOP_LEFT || refPoint == CAMERA_REFERENCE::TOP_RIGHT) {
			rc.top = position.y;
			rc.bottom = position.y + sz.height;
		}
		else {
			rc.top = position.y - sz.height;
			rc.bottom = position.y;
		}
	}
	return rc;
}
oppo::Size2F oppo::Camera::GetSize() {
	Size2F size = (*ppRT)->GetSize();
	size.width /= scale.width;
	size.height /= scale.height;
	return size;
}

void oppo::Camera::SafePushLayer() {
	if (*ppCurrentLayer == pLayer) return;
	if (*ppCurrentLayer != nullptr) {
		(*ppRT)->PopLayer();
		*ppCurrentLayer = nullptr;
	}
	(*ppRT)->PushLayer(layerParams, pLayer);
	*ppCurrentLayer = pLayer;
	D2D1_SIZE_F sz = (*ppRT)->GetSize();
	
	
	D2D1_POINT_2F offset = D2D1::Point2F();
	if (refPoint == CAMERA_REFERENCE::CENTER) {
		offset.x = sz.width / 2;
		offset.y = sz.height / 2;
	}
	else {
		if (refPoint == CAMERA_REFERENCE::TOP_RIGHT || refPoint == CAMERA_REFERENCE::BOTTOM_RIGHT) {
			offset.x = sz.width;
		}
		if (refPoint == CAMERA_REFERENCE::BOTTOM_LEFT || refPoint == CAMERA_REFERENCE::BOTTOM_RIGHT) {
			offset.y = sz.height;
		}
	}
	D2D1_MATRIX_3X2_F transform;
	(*ppRT)->SetTransform(
		D2D1::Matrix3x2F::Rotation(rotation, D2D1::Point2F(position.x, position.y)) 
		* D2D1::Matrix3x2F::Translation(-position.x + offset.x, -position.y + offset.y)
		* D2D1::Matrix3x2F::Scale(D2D1::SizeF(scale.width, scale.height), offset)
	);
}
#pragma endregion

#pragma region Animation Manager
void oppo::_AnimationManager::NextFrame() {
	std::vector<std::function<void()>> callbacks;
	for (auto a = animations.begin(); a != animations.end();) {
		if (!(*a)->isPaused) {
			(*a)->NextFrame();
			if ((*a)->i == (*a)->size) {
				(*a)->i = 0;
				if ((*a)->loop > 0) {
					(*a)->loop--;
				}
				if ((*a)->loop == 0) {
					if ((*a)->callback) {
						callbacks.push_back((*a)->callback);
					}
					a = animations.erase(a);
				}
				else {
					a++;
				}
			}
			else {
				a++;
			}
		}
		else {
			a++;
		}
	}
	for (auto cb : callbacks) {
		cb();
	}
}

oppo::Result oppo::_AnimationManager::RemoveAnimation(AnimationID& id) {
	if (!id) {
		return oppo::ERRORS::FAIL;
	}
	for (auto a = animations.begin(); a != animations.end(); a++) {
		if ((*a)->id == id) {
			animations.erase(a);
			id = 0;
			return oppo::ERRORS::SUCCESS;
		}
	}
	return oppo::ERRORS::FAIL; // id not found
}
bool oppo::_AnimationManager::AnimationExists(AnimationID id) {
	if (!id) {
		return false;
	}
	for (auto a = animations.begin(); a != animations.end(); a++) {
		if ((*a)->id == id) {
			return true;
		}
	}
	return false; // id not found
}
oppo::Result oppo::_AnimationManager::PauseAnimation(AnimationID id) {
	if (!id) {
		return oppo::ERRORS::FAIL;
	}
	for (auto a = animations.begin(); a != animations.end(); a++) {
		if ((*a)->id == id) {
			(*a)->isPaused = true;
			return oppo::ERRORS::SUCCESS;
		}
	}
	return oppo::ERRORS::FAIL; // id not found
}
oppo::Result oppo::_AnimationManager::ResumeAnimation(AnimationID id) {
	if (!id) {
		return oppo::ERRORS::FAIL;
	}
	for (auto a = animations.begin(); a != animations.end(); a++) {
		if ((*a)->id == id) {
			(*a)->isPaused = false;
			return oppo::ERRORS::SUCCESS;
		}
	}
	return oppo::ERRORS::FAIL; // id not found
}
oppo::Result oppo::_AnimationManager::ResetAnimation(AnimationID id) {
	if (!id) {
		return oppo::ERRORS::FAIL;
	}
	for (auto a = animations.begin(); a != animations.end(); a++) {
		if ((*a)->id == id) {
			(*a)->i = 0;
			return oppo::ERRORS::SUCCESS;
		}
	}
	return oppo::ERRORS::FAIL; // id not found
}

#pragma endregion

#pragma region Resource Manager
void oppo::_ResourceManager::Init(Window* wnd) {
	wnd->ppRT = &pRT;
	wnd->ppCurrentLayer = &currentLayer;
}

HRESULT oppo::_ResourceManager::CreateWindowResources(HWND hWnd) {
	HRESULT hr = CreateDIResources();
	if (SUCCEEDED(hr)) {
		hr = CreateDDResources(hWnd);
	}
	return hr;
}
void oppo::_ResourceManager::DestroyWindowResources() {
	DestroyDDResources();
	DestroyDIResources();
}

HRESULT oppo::_ResourceManager::CreateDIResources() {
	HRESULT hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		&pFactory
	);

	if (SUCCEEDED(hr)) {
		hr = CoInitialize(nullptr);
	}

	if (SUCCEEDED(hr)) {
		hr = CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&pFactoryWIC)
		);
	}

	if (SUCCEEDED(hr)) {
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&pFactoryWrite)
		);
	}

	return hr;
}
HRESULT oppo::_ResourceManager::CreateDDResources(HWND hWnd) {
	RECT rc;
	GetClientRect(hWnd, &rc);
	HRESULT hr = pFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			hWnd,
			D2D1::SizeU(
				rc.right - rc.left,
				rc.bottom - rc.top)
		),
		&pRT
	);
	return hr;
}
HRESULT oppo::_ResourceManager::RecreateDDResources(HWND hWnd) {
	HRESULT hr = CreateDDResources(hWnd);

	if (SUCCEEDED(hr)) {
		for (auto pBrush : brushes) {
			if (SUCCEEDED(hr)) { 
				BrushProperties properties;
				properties.color = pBrush->color;
				properties.strokewidth = pBrush->strokeWidth;
				hr = CreateBrush(pBrush, properties); 
			}
		}
		for (auto pBitmap : bitmaps) {
			if (SUCCEEDED(hr)) { 
				BitmapProperties properties;
				properties.fileName = pBitmap->fileName.c_str();
				hr = CreateBitmap(pBitmap, properties);
			}
		}
		for (auto pSpriteSheet : spriteSheets) {
			if (SUCCEEDED(hr)) { 
				SpriteSheetProperties properties;
				properties.fileName = pSpriteSheet->fileName.c_str();
				properties.padding = pSpriteSheet->padding;
				properties.spriteCount = pSpriteSheet->spriteCount;
				properties.spriteSize = pSpriteSheet->pxSpriteResolution;
				hr = CreateSpriteSheet(pSpriteSheet, properties); 
			}
		}
		for (auto pCamera : cameras) {
			if (SUCCEEDED(hr)) { 
				CameraProperties properties = { };
				hr = CreateCamera(pCamera, properties); 
			}
		}
	}

	return hr;
}

void oppo::_ResourceManager::DestroyDIResources() {
	for (auto textFormat : textFormats) {
		utility::SafeRelease(&textFormat->pTextFormat);
	}
	utility::SafeRelease(&pFactory);
	utility::SafeRelease(&pFactoryWIC);
	utility::SafeRelease(&pFactoryWrite);
}
void oppo::_ResourceManager::DestroyDDResources() {
	for (auto pBrush : brushes) {
		DestroyBrush(pBrush);
	}
	for (auto pBitmap : bitmaps) {
		DestroyBitmap(pBitmap);
	}
	for (auto pSpriteSheet : spriteSheets) {
		DestroySpriteSheet(pSpriteSheet);
	}
	for (auto pCamera : cameras) {
		DestroyCamera(pCamera);
	}
	utility::SafeRelease(&pRT);
}

HRESULT oppo::_ResourceManager::CreateBrush(Brush* pBrush, BrushProperties properties) {
	HRESULT hr = E_FAIL;
	if (pRT) {
		Color color = properties.color;
		hr = pRT->CreateSolidColorBrush(D2D1::ColorF(color.R, color.G, color.B, color.a), &pBrush->pBrush);
	}
	if (SUCCEEDED(hr)) {
		pBrush->strokeWidth = properties.strokewidth;
		if ((std::find(brushes.begin(), brushes.end(), pBrush) == brushes.end())) {
			brushes.push_back(pBrush);
		}
	}

	return hr;
}
HRESULT oppo::_ResourceManager::CreateBitmap(Bitmap* pBitmap, BitmapProperties properties) {
	HRESULT hr = S_OK;

	if (pRT) {
		hr = LoadBitmapFromFile(properties.fileName, &pBitmap->pBitmap);
		if (SUCCEEDED(hr)) {
			pBitmap->fileName = properties.fileName;
			if (std::find(bitmaps.begin(), bitmaps.end(), pBitmap) == bitmaps.end()) {
				bitmaps.push_back(pBitmap);
			}
		}
	}

	return hr;
}
HRESULT oppo::_ResourceManager::CreateSpriteSheet(SpriteSheet* pSpriteSheet, SpriteSheetProperties properties) {
	HRESULT hr = E_FAIL;
	if (pRT) {
		hr = LoadBitmapFromFile(properties.fileName, &pSpriteSheet->pBitmap);
	}

	if (SUCCEEDED(hr)) {
		pSpriteSheet->fileName = properties.fileName;
		pSpriteSheet->pxSheetResolution = pSpriteSheet->pBitmap->GetSize();
		pSpriteSheet->pxSpriteResolution = properties.spriteSize;
		pSpriteSheet->spriteCount = properties.spriteCount;
		pSpriteSheet->padding = properties.padding;
		if (std::find(spriteSheets.begin(), spriteSheets.end(), pSpriteSheet) == spriteSheets.end()) {
			spriteSheets.push_back(pSpriteSheet);
		}
	}

	return hr;
}
HRESULT oppo::_ResourceManager::CreateSprite(Sprite* pSprite, SpriteProperties properties) {
	HRESULT hr = E_FAIL;

	if (properties.pSpriteSheet->pBitmap) {
		hr = S_OK;
	}

	if (SUCCEEDED(hr)) {
		pSprite->pSpriteSheet = properties.pSpriteSheet;
		pSprite->rect = properties.spriteRect;
		pSprite->spriteIndex = properties.spriteIndex;
	}

	return hr;
}
HRESULT oppo::_ResourceManager::CreateTextFormat(TextFormat* pTextFormat, TextFormatProperties properties) {
	HRESULT hr = E_FAIL;
	if (pFactoryWrite) {
		hr = S_OK;
	}
	
	if (SUCCEEDED(hr)) {
		hr = pFactoryWrite->CreateTextFormat(
			utility::StringToWString(properties.fontName).c_str(),
			NULL,
			static_cast<DWRITE_FONT_WEIGHT>(properties.fontWeight),
			static_cast<DWRITE_FONT_STYLE>(properties.fontStyle),
			static_cast<DWRITE_FONT_STRETCH>(properties.fontStretch),
			properties.fontSize,
			L"en-us",
			&pTextFormat->pTextFormat
		);
	}

	if (SUCCEEDED(hr)) {
		hr = pTextFormat->pTextFormat->SetTextAlignment(static_cast<DWRITE_TEXT_ALIGNMENT>(properties.hAlignment));
	}

	if (SUCCEEDED(hr)) {
		hr = pTextFormat->pTextFormat->SetParagraphAlignment(static_cast<DWRITE_PARAGRAPH_ALIGNMENT>(properties.vAlignment));
	}

	if (SUCCEEDED(hr)) {
		hr = pTextFormat->pTextFormat->SetWordWrapping(static_cast<DWRITE_WORD_WRAPPING>(properties.wordWrapping));
	}

	if (SUCCEEDED(hr)) {
		float lineSpacing = properties.fontSize * (properties.lineSpacing);
		hr = pTextFormat->pTextFormat->SetLineSpacing(DWRITE_LINE_SPACING_METHOD_UNIFORM, lineSpacing, .8 * lineSpacing);
	}

	return hr;
}
HRESULT oppo::_ResourceManager::CreateTileMap(TileMap* pTileMap, TileMapProperties properties) {
	return ERRORS::FAIL;
}
HRESULT oppo::_ResourceManager::CreateEffect(Effect* pEffect, EffectProperties properties) {
	return ERRORS::FAIL;
}
HRESULT oppo::_ResourceManager::CreateCamera(Camera* pCamera, CameraProperties properties) {
	HRESULT hr = E_FAIL;
	
	if (pRT) {
		pCamera->ppRT = &pRT;
		hr = pRT->CreateLayer(&pCamera->pLayer);
	}
	if (SUCCEEDED(hr)) {
		pCamera->ppCurrentLayer = &currentLayer;
		pCamera->refPoint = properties.referencePoint;
		if (std::find(cameras.begin(), cameras.end(), pCamera) == cameras.end()) {
			cameras.push_back(pCamera);
		}
	}

	return hr;

}

void oppo::_ResourceManager::DestroyBrush(Brush* pBrush) {
	utility::SafeRelease(&pBrush->pBrush);
	utility::DeleteFromVector(brushes, pBrush);
}
void oppo::_ResourceManager::DestroyBitmap(Bitmap* pBitmap) {
	utility::SafeRelease(&pBitmap->pBitmap);
	pBitmap->fileName = "";
	utility::DeleteFromVector(bitmaps, pBitmap);
}
void oppo::_ResourceManager::DestroySpriteSheet(SpriteSheet* pSpriteSheet) {
	utility::SafeRelease(&pSpriteSheet->pBitmap);
	pSpriteSheet->fileName = "";
	pSpriteSheet->pxSheetResolution = Size2D();
	pSpriteSheet->pxSpriteResolution = Size2D();
	pSpriteSheet->spriteCount = Size2D();
	pSpriteSheet->padding = Rect();
	utility::DeleteFromVector(spriteSheets, pSpriteSheet);
}
void oppo::_ResourceManager::DestroySprite(Sprite* pSprite) {
	pSprite->pSpriteSheet = nullptr;
	pSprite->rect = RectF();
	pSprite->spriteIndex = Size2D();
}
void oppo::_ResourceManager::DestroyTextFormat(TextFormat* pTextFormat) {
	utility::SafeRelease(&pTextFormat->pTextFormat);
	utility::DeleteFromVector(textFormats, pTextFormat);
}
void oppo::_ResourceManager::DestroyCamera(Camera* pCamera) {
	utility::SafeRelease(&pCamera->pLayer);
	pCamera->ppCurrentLayer = nullptr;
	pCamera->ppRT = nullptr;
	pCamera->layerParams = D2D1::LayerParameters();
	utility::DeleteFromVector(cameras, pCamera);
}

HRESULT oppo::_ResourceManager::LoadBitmapFromFile(const char* fileName, ID2D1Bitmap** ppBitmap) {
	IWICBitmapDecoder* pDecoder = NULL;
	IWICBitmapFrameDecode* pSource = NULL;
	IWICStream* pStream = NULL;
	IWICFormatConverter* pConverter = NULL;
	IWICBitmapScaler* pScaler = NULL;

	std::wstring wFileName = utility::StringToWString(fileName);

	HRESULT hr = pFactoryWIC->CreateDecoderFromFilename(
		wFileName.c_str(),
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder
	);

	if (SUCCEEDED(hr)) {
		hr = pDecoder->GetFrame(0, &pSource);
	}

	if (SUCCEEDED(hr)) {
		hr = pFactoryWIC->CreateFormatConverter(&pConverter);
	}

	if (SUCCEEDED(hr)) {
		hr = pConverter->Initialize(
			pSource,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeMedianCut
		);
	}

	if (SUCCEEDED(hr)) {
		hr = pRT->CreateBitmapFromWicBitmap(
			pConverter,
			NULL,
			ppBitmap
		);
	}

	utility::SafeRelease(&pDecoder);
	utility::SafeRelease(&pSource);
	utility::SafeRelease(&pStream);
	utility::SafeRelease(&pConverter);
	utility::SafeRelease(&pScaler);

	return hr;
}
HRESULT oppo::_ResourceManager::LoadBitmapFromResource(PCWSTR resource, ID2D1Bitmap** ppBitmap) {
	IWICBitmapDecoder* pDecoder = NULL;
	IWICBitmapFrameDecode* pSource = NULL;
	IWICStream* pStream = NULL;
	IWICFormatConverter* pConverter = NULL;
	IWICBitmapScaler* pScaler = NULL;

	HRSRC imageResHandle = NULL;
	HGLOBAL imageResDataHandle = NULL;
	void* pImageFile = NULL;
	DWORD imageFileSize = 0;

	HMODULE hModule = GetModuleHandle(nullptr);

	// locate resource
	imageResHandle = FindResource(hModule, resource, MAKEINTRESOURCE(RT_BITMAP));
	HRESULT hr = imageResHandle ? S_OK : E_FAIL;

	// load resource
	if (SUCCEEDED(hr)) {
		imageResDataHandle = LoadResource(hModule, imageResHandle);

		hr = imageResDataHandle ? S_OK : E_FAIL;
	}

	// lock to system memory pointer
	if (SUCCEEDED(hr)) {
		pImageFile = LockResource(imageResDataHandle);

		hr = pImageFile ? S_OK : E_FAIL;
	}

	// calculate size
	if (SUCCEEDED(hr)) {
		imageFileSize = SizeofResource(hModule, imageResHandle);

		hr = imageFileSize ? S_OK : E_FAIL;
	}

	// create WIC stream
	if (SUCCEEDED(hr)) {
		hr = pFactoryWIC->CreateStream(&pStream);
	}

	// initialize stream
	if (SUCCEEDED(hr)) {
		hr = pStream->InitializeFromMemory(
			reinterpret_cast<BYTE*>(pImageFile),
			imageFileSize
		);
	}

	// create stream decoder
	if (SUCCEEDED(hr)) {
		hr = pFactoryWIC->CreateDecoderFromStream(
			pStream,
			NULL,
			WICDecodeMetadataCacheOnLoad,
			&pDecoder
		);
	}

	// create initial frame
	if (SUCCEEDED(hr)) {
		hr = pDecoder->GetFrame(0, &pSource);
	}

	if (SUCCEEDED(hr)) {
		hr = pFactoryWIC->CreateFormatConverter(&pConverter);
	}

	if (SUCCEEDED(hr)) {
		hr = pConverter->Initialize(
			pSource,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeMedianCut
		);
	}

	if (SUCCEEDED(hr)) {
		hr = pRT->CreateBitmapFromWicBitmap(
			pConverter,
			NULL,
			ppBitmap
		);
	}
	utility::SafeRelease(&pDecoder);
	utility::SafeRelease(&pSource);
	utility::SafeRelease(&pStream);
	utility::SafeRelease(&pConverter);
	utility::SafeRelease(&pScaler);

	return hr;
}
#pragma endregion

#pragma region _Engine
std::vector<HWND> oppo::_Engine::windows;

oppo::Result oppo::_Engine::AddWindow(Window* wnd, WindowProperties properties) {
	// create a new random class name
	wnd->NewClassName();

	// set win32 window class properties
	WNDCLASS wc = { };
	wc.style = CS_DBLCLKS;
	wc.lpfnWndProc = [](HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT {
		Window* pThis = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		if (uMsg == WM_NCCREATE) {
			CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
			pThis = static_cast<Window*>(pCreate->lpCreateParams);
			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
		}

		if (pThis) {
			return pThis->WindowProc(hwnd, uMsg, wParam, lParam);
		}

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	};

	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = wnd->className;

	RegisterClass(&wc);

	// window size and position defaults
	if (properties.size.width == 0) properties.size.width = CW_USEDEFAULT;
	if (properties.size.height == 0) properties.size.height = CW_USEDEFAULT;

	// create window
	wnd->GameLoop = properties.GameLoop;
	wnd->SetWndStateCreate();

	wnd->hWnd = CreateWindowEx(
		0,
		wnd->className,
		utility::StringToWString(properties.name).c_str(),
		properties.style,
		// Size and position: X, Y, Width, Height
		CW_USEDEFAULT, CW_USEDEFAULT, properties.size.width, properties.size.height,
		NULL,       // Parent window    
		NULL,       // Menu
		GetModuleHandle(NULL),  // Instance handle
		wnd        // Additional application data
	);

	// check for errors
	if (wnd->hWnd == NULL) {
		return ERRORS::FAIL;
	}

	// set window properties
	wnd->backgroundColor = properties.backgroundColor;
	wnd->szMin = properties.minSize;
	wnd->szMax = properties.maxSize;
	wnd->SetFPS(properties.fps);
	wnd->SetUPS(properties.ups);
	wnd->SetAPS(properties.aps);
	wnd->winID = properties.id;

	// initialize timer thread
	wnd->tGameLoopTimer = std::thread([wnd]() { wnd->GameLoopTimer(); });

	// add hWnd for running
	windows.push_back(wnd->hWnd);

	return ERRORS::SUCCESS;
}

void oppo::_Engine::RemoveWindow(Window* wnd) {
	wnd->tGameLoopTimer.join();
	utility::DeleteFromVector(windows, wnd->hWnd);
	if (windows.size() == 0) {
		PostQuitMessage(0);
	}
}

void oppo::_Engine::Terminate() {
	for (auto hWnd : windows) {
		DestroyWindow(hWnd);
	}
}

void oppo::_Engine::Run() {
	for (auto hWnd : windows) {
		ShowWindow(hWnd, SW_SHOWDEFAULT);
	}

	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

oppo::Result oppo::CreateWindow(Window* wnd, WindowProperties properties) {
	return _Engine::AddWindow(wnd, properties);
}

void oppo::Run() {
	_Engine::Run();
}

void oppo::Terminate() {
	_Engine::Terminate();
}


#pragma endregion