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
	if (SUCCEEDED(_resourceManager.CreateBrush(pBrush, properties))) {
		return ERRORS::SUCCESS;
	}
	return ERRORS::FAIL;
}
oppo::Result oppo::Window::CreateBitmap(Bitmap* pBitmap, BitmapProperties properties) {
	if (SUCCEEDED(_resourceManager.CreateBitmap(pBitmap, properties))) {
		return ERRORS::SUCCESS;
	}
	return ERRORS::FAIL;
}
oppo::Result oppo::Window::CreateSpriteSheet(SpriteSheet* pSpriteSheet, SpriteSheetProperties properties) {
	if (SUCCEEDED(_resourceManager.CreateSpriteSheet(pSpriteSheet, properties))) {
		return ERRORS::SUCCESS;
	}
	return ERRORS::FAIL;
}
oppo::Result oppo::Window::CreateSprite(Sprite* pSprite, SpriteProperties properties) {
	if (SUCCEEDED(_resourceManager.CreateSprite(pSprite, properties))) {
		return ERRORS::SUCCESS;
	}
	return ERRORS::FAIL;
}
oppo::Result oppo::Window::CreateTextFormat(TextFormat* pTextFormat, TextFormatProperties properties) {
	if (SUCCEEDED(_resourceManager.CreateTextFormat(pTextFormat , properties))) {
		return ERRORS::SUCCESS;
	}
	return ERRORS::FAIL;
}
oppo::Result oppo::Window::CreateTileMap(TileMap* pTileMap, TileMapProperties properties) {
	if (SUCCEEDED(_resourceManager.CreateTileMap(pTileMap, properties))) {
		return ERRORS::SUCCESS;
	}
	return ERRORS::FAIL;
}
oppo::Result oppo::Window::CreateEffect(Effect* pEffect, EffectProperties properties) {
	if (SUCCEEDED(_resourceManager.CreateEffect(pEffect, properties))) {
		return ERRORS::SUCCESS;
	}
	return ERRORS::FAIL;
}
oppo::Result oppo::Window::CreateCamera(Camera* pCamera, CameraProperties properties) {
	if (SUCCEEDED(_resourceManager.CreateCamera(pCamera, properties))) {
		return ERRORS::SUCCESS;
	}
	return ERRORS::FAIL;
}

// destroy window-dependent resources
void oppo::Window::DestroyBrush(Brush* pBrush) {
	_resourceManager.DestroyBrush(pBrush);
}
void oppo::Window::DestroyBitmap(Bitmap* pBitmap) {
	_resourceManager.DestroyBitmap(pBitmap);
}
void oppo::Window::DestroySpriteSheet(SpriteSheet* pSpriteSheet) {
	_resourceManager.DestroySpriteSheet(pSpriteSheet);
}
void oppo::Window::DestroySprite(Sprite* pSprite) {
	_resourceManager.DestroySprite(pSprite);
}
void oppo::Window::DestroyTextFormat(TextFormat* pTextFormat) { 
	_resourceManager.DestroyTextFormat(pTextFormat);
}
void oppo::Window::DestroyTileMap(TileMap* pTileMap) {}
void oppo::Window::DestroyEffect(Effect* pEffect) {}
void oppo::Window::DestroyCamera(Camera* pCamera) {
	_resourceManager.DestroyCamera(pCamera);
}

// animations
oppo::Result oppo::Window::RemoveAnimation(AnimationID& id) {
	return _animationManager.RemoveAnimation(id);
}
oppo::Result oppo::Window::PauseAnimation(AnimationID id) {
	return _animationManager.PauseAnimation(id);
}
oppo::Result oppo::Window::ResumeAnimation(AnimationID id) {
	return _animationManager.ResumeAnimation(id);
}
oppo::Result oppo::Window::ResetAnimation(AnimationID id) {
	return _animationManager.ResetAnimation(id);
}
bool oppo::Window::AnimationExists(AnimationID id) {
	return _animationManager.AnimationExists(id);
}

// set window properties
void oppo::Window::SetFPS(float fps) {
	if (fps == 0) {
		_renderCountTarget = -1; // avoid divide by zero error
	}
	else {
		_renderCountTarget = (int)(1000000 / fps); // game loop timers are in microseconds
	}
}
void oppo::Window::SetUPS(float ups) {
	if (ups == 0) {
		_updateCountTarget = -1; // avoid divide by zero error
	}
	else {
		_updateCountTarget = (int)(1000000 / ups); // game loop timers are in microseconds
	}
}
void oppo::Window::SetAPS(float aps) {
	if (aps == 0) {
		_animateCountTarget = -1; // avoid divide by zero error
	}
	else {
		_animateCountTarget = (int)(1000000 / aps); // game loop timers are in microseconds
	}
}
void oppo::Window::SetSize(Size2D size) {
	SetWindowPos(_hWnd, HWND_TOPMOST, NULL, NULL, size.width, size.height, SWP_NOMOVE);
}
void oppo::Window::SetPosition(Point2D position) {
	SetWindowPos(_hWnd, HWND_TOPMOST, position.x, position.y, NULL, NULL, SWP_NOSIZE);
}
void oppo::Window::SetMousePosition(Point2D position) {
	SetCursorPos(position.x, position.y);
}
void oppo::Window::SetMouseCapture(bool isCaptured) {
	if (isCaptured) {
		SetCapture(_hWnd);
	}
	else {
		ReleaseCapture();
	}
}
void oppo::Window::SetMaximize(bool isMaximized) {
	if (isMaximized) {
		PostMessage(_hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
	}
	else {
		PostMessage(_hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
	}
}
void oppo::Window::SetMinimize(bool isMinimized) {
	if (isMinimized) {
		PostMessage(_hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
	}
	else {
		PostMessage(_hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
	}
}
void oppo::Window::SetFullscreen(bool isFullscreen) {
	if (isFullscreen && !this->_isFullscreen) {
		_dwStylePrev = GetWindowLong(_hWnd, GWL_STYLE);
		this->_isFullscreen = true;
		MONITORINFO mi = { sizeof(mi) };
		if (GetWindowPlacement(_hWnd, &_wpPrev) && GetMonitorInfo(MonitorFromWindow(_hWnd, MONITOR_DEFAULTTONEAREST), &mi)) {
			SetWindowLong(_hWnd, GWL_STYLE, _dwStylePrev & ~(WS_CAPTION | WS_THICKFRAME));
			SetWindowPos(_hWnd, HWND_TOP,
				mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top,
				SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		}
	}
	else if (!isFullscreen && this->_isFullscreen) {
		this->_isFullscreen = false;
		SetWindowLong(_hWnd, GWL_STYLE, _dwStylePrev);
		SetWindowPlacement(_hWnd, &_wpPrev);
		SetWindowPos(_hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}
}

void oppo::Window::SetScene(std::function<Result(Event)> newScene) {
	Event e;
	e.windowID = _winID;

	if (_GameLoop) {
		e.type = EVENTS::ENDSCENE;
		_GameLoop(e);
	}

	if (newScene) {
		_GameLoop = newScene;
		e.type = EVENTS::SETSCENE;
		_GameLoop(e);
	}
}

// get window properties
oppo::Size2D oppo::Window::GetSize() {
	RECT rc;
	GetWindowRect(_hWnd, &rc);
	return Size2D(rc.right - rc.left, rc.bottom - rc.top);
}
oppo::Point2D oppo::Window::GetPosition() {
	RECT rc;
	GetWindowRect(_hWnd, &rc);
	return Point2D(rc.left, rc.top);
}
#pragma endregion

#pragma region Window::private
void oppo::Window::NewClassName() {
	std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
	std::uniform_int_distribution<int> distribution(0x0041, 0x007A);
	for (int i = 0; i < 8; i++) {
		_className[i] = L'A';
	}
	for (int i = 8; i < 16; i++) {
		_className[i] = static_cast<wchar_t>(distribution(rng));
	}
	_className[16] = L'\0';
}
HWND oppo::Window::Create(WindowProperties properties) {
	// create a new random class name
	NewClassName();

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
	wc.lpszClassName = _className;

	RegisterClass(&wc);

	// window size and position defaults
	if (properties.size.width == 0) properties.size.width = CW_USEDEFAULT;
	if (properties.size.height == 0) properties.size.height = CW_USEDEFAULT;

	// create window
	_GameLoop = properties.GameLoop;
	_wndState = WNDSTATE::CREATE;

	_hWnd = CreateWindowEx(
		0,
		_className,
		utility::StringToWString(properties.name).c_str(),
		properties.style,
		// Size and position: X, Y, Width, Height
		CW_USEDEFAULT, CW_USEDEFAULT, properties.size.width, properties.size.height,
		NULL,       // Parent window    
		NULL,       // Menu
		GetModuleHandle(NULL),  // Instance handle
		this        // Additional application data
	);

	if (_hWnd != NULL) {
		_backgroundColor = properties.backgroundColor;
		_szMin = properties.minSize;
		_szMax = properties.maxSize;
		SetFPS(properties.fps);
		SetUPS(properties.ups);
		SetAPS(properties.aps);
		_winID = properties.id;
		_tGameLoopTimer = std::thread([this]() { GameLoopTimer(); });
	}

	return _hWnd;
}
void oppo::Window::GameLoopTimer() {
	long long renderCount;
	long long updateCount;
	long long animateCount;
	_swRender.Reset();
	_swUpdate.Reset();
	_swAnimate.Reset();

	while (_wndState != WNDSTATE::DESTROY) {
		renderCount = _swRender.Elapsed().count();
		updateCount = _swUpdate.Elapsed().count();
		animateCount = _swAnimate.Elapsed().count();

		if (animateCount >= _animateCountTarget && _wndState == WNDSTATE::RUN) {
			_swAnimate.Reset();
			PostMessage(_hWnd, WM_FRAME, animateCount, 2);
		}

		if (renderCount >= _renderCountTarget && _wndState == WNDSTATE::RUN) {
			_swRender.Reset();
			PostMessage(_hWnd, WM_FRAME, renderCount, 1);
			InvalidateRect(_hWnd, NULL, FALSE);
		}

		if (updateCount >= _updateCountTarget && _wndState == WNDSTATE::RUN) {
			_swUpdate.Reset();
			PostMessage(_hWnd, WM_FRAME, updateCount, 0);
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
	e.windowID = _winID;
	switch (uMsg) {
	case WM_CREATE: {
		_resourceManager.Init(this);
		if (FAILED(_resourceManager.CreateWindowResources(hWnd))) {
			return -1;
		}
		if (_GameLoop) {
			e.type = EVENTS::CREATE;
			_GameLoop(e);
			e.type = EVENTS::SETSCENE;
			_GameLoop(e);
		}
		// set initial fullscreen parameters
		GetWindowPlacement(hWnd, &_wpPrev);
		_dwStylePrev = GetWindowLong(hWnd, GWL_STYLE);
		_wndState = WNDSTATE::RUN;
		return 0;
	}
	case WM_CLOSE: {
		if (_GameLoop) {
			e.type = EVENTS::CLOSE;
			Result r = _GameLoop(e);
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
		_wndState = WNDSTATE::DESTROY;
		if (_GameLoop) {
			e.type = EVENTS::ENDSCENE;
			_GameLoop(e);
			e.type = EVENTS::DESTROY;
			_GameLoop(e);
		}
		_resourceManager.DestroyWindowResources();
		_Engine::GetInstance()->RemoveWindow(this);
		return 0;
	}
	case WM_SIZE: {
		D2D1_SIZE_U size = D2D1::SizeU(LOWORD(lParam), HIWORD(lParam));
		(*_ppRT)->Resize(size);
		if (_GameLoop) {
			e.type = EVENTS::RESIZE;
			e.as.resize.size = size;
			_GameLoop(e);
		}
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;
	}
	case WM_SIZING: {
		if (_aspectRatio != 0) {
			RECT* rc;
			rc = (RECT*)lParam;
			if (wParam != WMSZ_LEFT && wParam != WMSZ_TOPLEFT && wParam != WMSZ_BOTTOMLEFT && wParam != WMSZ_RIGHT) {
				rc->right = (rc->bottom - rc->top) * _aspectRatio + rc->left;
			}
			else if (wParam == WMSZ_LEFT || wParam == WMSZ_RIGHT) {
				rc->bottom = (rc->right - rc->left) * 1 / _aspectRatio + rc->top;
			}
			else {
				rc->left = rc->right - (rc->bottom - rc->top) * _aspectRatio;
			}
		}
		return TRUE;
	}
	case WM_GETMINMAXINFO: {
		LPMINMAXINFO lpMMI = (LPMINMAXINFO)(lParam);
		if (_szMin.width) {
			lpMMI->ptMinTrackSize.x = _szMin.width;
		}
		if (_szMin.height) {
			lpMMI->ptMinTrackSize.y = _szMin.height;
		}
		if (_szMax.width) {
			lpMMI->ptMaxTrackSize.x = _szMax.width;
		}
		if (_szMax.height) {
			lpMMI->ptMaxTrackSize.y = _szMax.height;
		}
		return 0;
	}

	case WM_PAINT: {
		RECT rc;
		GetClientRect(hWnd, &rc);
		ValidateRect(hWnd, &rc);

		(*_ppRT)->BeginDraw();
		(*_ppRT)->SetTransform(D2D1::IdentityMatrix());
		(*_ppRT)->Clear(D2D1::ColorF(_backgroundColor.R, _backgroundColor.G, _backgroundColor.B, _backgroundColor.a));

		if (_GameLoop) {
			e.type = EVENTS::PAINT;
			_GameLoop(e);
		}

		if (*_ppCurrentLayer != nullptr) {
			(*_ppRT)->PopLayer();
			*_ppCurrentLayer = nullptr;
		}

		HRESULT hr = (*_ppRT)->EndDraw();

		if (hr == D2DERR_RECREATE_TARGET) {
			_resourceManager.RecreateDDResources(hWnd);
			InvalidateRect(hWnd, NULL, FALSE);
		}

		return 0;
	}
	case WM_FRAME: {
		if (_GameLoop) {
			long long usCount = (long long)(wParam);
			if (lParam == 0) {
				// update
				e.as.update.dt = usCount / 1000000.;
				e.type = EVENTS::UPDATE;
				_GameLoop(e);
			}
			else if (lParam == 1) {
				// render
				InvalidateRect(hWnd, NULL, FALSE);
			}
			else if (lParam == 2) {
				// animate
				_animationManager._NextFrame();
			}
		}

		return 0;
	}

				 // keyboard
	case WM_KEYDOWN: {
		WORD vkCode = LOWORD(wParam);
		WORD keyFlags = HIWORD(lParam);
		BOOL wasKeyDown = (keyFlags & KF_REPEAT) == KF_REPEAT;

		if (_GameLoop && !wasKeyDown) {
			e.type = EVENTS::KEYDOWN;
			e.as.keyDown.key = TranslateKeystroke(vkCode);
			_GameLoop(e);
		}

		return 0;
	}
	case WM_KEYUP: {
		WORD vkCode = LOWORD(wParam);

		if (_GameLoop) {
			e.type = EVENTS::KEYUP;
			e.as.keyUp.key = TranslateKeystroke(vkCode);
			_GameLoop(e);
		}

		return 0;
	}
	case WM_CHAR: {
		if (_GameLoop) {
			e.type = EVENTS::CHAR;
			e.as.character.character = static_cast<char>(wParam);
			e.as.character.repeat = LOWORD(lParam);
			_GameLoop(e);
		}

		return 0;
	}
	case WM_SYSKEYDOWN: {
		// generated when alt key is down
		WORD vkCode = LOWORD(wParam);
		WORD keyFlags = HIWORD(lParam);
		BOOL wasKeyDown = (keyFlags & KF_REPEAT) == KF_REPEAT;

		if (_GameLoop && !wasKeyDown) {
			e.type = EVENTS::KEYDOWN;
			e.as.keyDown.key = TranslateKeystroke(vkCode);
			e.as.keyDown.alt = true;
			_GameLoop(e);
		}

		return 0;
	}
	case WM_SYSKEYUP: {
		WORD vkCode = LOWORD(wParam);

		if (_GameLoop) {
			e.type = EVENTS::KEYUP;
			e.as.keyUp.key = TranslateKeystroke(vkCode);
			e.as.keyUp.alt = true;
			_GameLoop(e);
		}

		return 0;
	}
	case WM_SYSCHAR: {
		if (_GameLoop) {
			e.type = EVENTS::CHAR;
			e.as.character.character = static_cast<char>(wParam);
			e.as.character.repeat = LOWORD(lParam);
			e.as.character.alt = 1;
			_GameLoop(e);
		}

		return 0;
	}

				   // mouse
	case WM_MOUSEMOVE: {
		if (_GameLoop) {
			e.type = EVENTS::MOUSEMOVE;
			e.as.mouseMove.x = GET_X_LPARAM(lParam);
			e.as.mouseMove.y = GET_Y_LPARAM(lParam);
			if (GetKeyState(VK_MENU) & 0x8000) {
				e.as.mouseMove.alt = true;
			}
			_GameLoop(e);
		}
		return 0;
	}
	case WM_MOUSEWHEEL: {
		if (_GameLoop) {
			e.type = EVENTS::MOUSESCROLL;
			e.as.mouseScroll.vScroll = GET_WHEEL_DELTA_WPARAM(wParam);
			e.as.mouseScroll.x = GET_X_LPARAM(lParam);
			e.as.mouseScroll.y = GET_Y_LPARAM(lParam);
			if (GetKeyState(VK_MENU) & 0x8000) {
				e.as.mouseScroll.alt = true;
			}
			_GameLoop(e);
		}
		return 0;
	}
	case WM_MOUSEHWHEEL: {
		if (_GameLoop) {
			e.type = EVENTS::MOUSESCROLL;
			e.as.mouseScroll.hScroll = GET_WHEEL_DELTA_WPARAM(wParam);
			e.as.mouseScroll.x = GET_X_LPARAM(lParam);
			e.as.mouseScroll.y = GET_Y_LPARAM(lParam);
			if (GetKeyState(VK_MENU) & 0x8000) {
				e.as.mouseScroll.alt = true;
			}
			_GameLoop(e);
		}
		return 0;
	}
	case WM_LBUTTONDOWN: {
		if (_GameLoop) {
			e.type = EVENTS::MOUSEDOWN;
			e.as.mouseDown.x = GET_X_LPARAM(lParam);
			e.as.mouseDown.y = GET_Y_LPARAM(lParam);
			e.as.mouseDown.key = KEYS::LMOUSE;
			if (GetKeyState(VK_MENU) & 0x8000) {
				e.as.mouseDown.alt = true;
			}
			_GameLoop(e);
		}

		return 0;
	}
	case WM_LBUTTONUP: {
		if (_GameLoop) {
			e.type = EVENTS::MOUSEDOWN;
			e.as.mouseUp.x = GET_X_LPARAM(lParam);
			e.as.mouseUp.y = GET_Y_LPARAM(lParam);
			e.as.mouseUp.key = KEYS::LMOUSE;
			if (GetKeyState(VK_MENU) & 0x8000) {
				e.as.mouseUp.alt = true;
			}
			_GameLoop(e);
		}

		return 0;
	}
	case WM_LBUTTONDBLCLK: {
		if (_GameLoop) {
			e.type = EVENTS::MOUSEDBLCLK;
			e.as.mouseDblClk.x = GET_X_LPARAM(lParam);
			e.as.mouseDblClk.y = GET_Y_LPARAM(lParam);
			e.as.mouseDblClk.key = KEYS::LMOUSE;
			if (GetKeyState(VK_MENU) & 0x8000) {
				e.as.mouseDblClk.alt = true;
			}
			_GameLoop(e);
		}

		return 0;
	}
	case WM_RBUTTONDOWN: {
		if (_GameLoop) {
			e.type = EVENTS::MOUSEDOWN;
			e.as.mouseDown.x = GET_X_LPARAM(lParam);
			e.as.mouseDown.y = GET_Y_LPARAM(lParam);
			e.as.mouseDown.key = KEYS::RMOUSE;
			if (GetKeyState(VK_MENU) & 0x8000) {
				e.as.mouseDown.alt = true;
			}
			_GameLoop(e);
		}

		return 0;
	}
	case WM_RBUTTONUP: {
		if (_GameLoop) {
			e.type = EVENTS::MOUSEDOWN;
			e.as.mouseUp.x = GET_X_LPARAM(lParam);
			e.as.mouseUp.y = GET_Y_LPARAM(lParam);
			e.as.mouseUp.key = KEYS::RMOUSE;
			if (GetKeyState(VK_MENU) & 0x8000) {
				e.as.mouseUp.alt = true;
			}
			_GameLoop(e);
		}

		return 0;
	}
	case WM_RBUTTONDBLCLK: {
		if (_GameLoop) {
			e.type = EVENTS::MOUSEDBLCLK;
			e.as.mouseDblClk.x = GET_X_LPARAM(lParam);
			e.as.mouseDblClk.y = GET_Y_LPARAM(lParam);
			e.as.mouseDblClk.key = KEYS::RMOUSE;
			if (GetKeyState(VK_MENU) & 0x8000) {
				e.as.mouseDblClk.alt = true;
			}
			_GameLoop(e);
		}

		return 0;
	}
	case WM_MBUTTONDOWN: {
		if (_GameLoop) {
			e.type = EVENTS::MOUSEDOWN;
			e.as.mouseDown.x = GET_X_LPARAM(lParam);
			e.as.mouseDown.y = GET_Y_LPARAM(lParam);
			e.as.mouseDown.key = KEYS::MMOUSE;
			if (GetKeyState(VK_MENU) & 0x8000) {
				e.as.mouseDown.alt = true;
			}
			_GameLoop(e);
		}

		return 0;
	}
	case WM_MBUTTONUP: {
		if (_GameLoop) {
			e.type = EVENTS::MOUSEDOWN;
			e.as.mouseUp.x = GET_X_LPARAM(lParam);
			e.as.mouseUp.y = GET_Y_LPARAM(lParam);
			e.as.mouseUp.key = KEYS::MMOUSE;
			if (GetKeyState(VK_MENU) & 0x8000) {
				e.as.mouseUp.alt = true;
			}
			_GameLoop(e);
		}

		return 0;
	}
	case WM_MBUTTONDBLCLK: {
		if (_GameLoop) {
			e.type = EVENTS::MOUSEDBLCLK;
			e.as.mouseDblClk.x = GET_X_LPARAM(lParam);
			e.as.mouseDblClk.y = GET_Y_LPARAM(lParam);
			e.as.mouseDblClk.key = KEYS::MMOUSE;
			if (GetKeyState(VK_MENU) & 0x8000) {
				e.as.mouseDblClk.alt = true;
			}
			_GameLoop(e);
		}

		return 0;
	}
	case WM_XBUTTONDOWN: {
		if (_GameLoop) {
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
			_GameLoop(e);
		}

		return 0;
	}
	case WM_XBUTTONUP: {
		if (_GameLoop) {
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
			_GameLoop(e);
		}

		return 0;
	}
	case WM_XBUTTONDBLCLK: {
		if (_GameLoop) {
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
			_GameLoop(e);
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
	this->_color = color;
	if (_pBrush) {
		_pBrush->SetColor(D2D1::ColorF(color.R, color.G, color.B, color.a));
	}
}

oppo::Color oppo::Brush::GetColor() {
	return _color;
}

#pragma endregion

#pragma region Sprite Sheet

oppo::Size2D oppo::SpriteSheet::GetSpriteSize() {
	return _spriteSize;
}

oppo::Size2D oppo::SpriteSheet::GetSpriteCount() {
	return _spriteCount;
}

oppo::Rect oppo::SpriteSheet::GetPadding() {
	return _padding;
}

D2D1_RECT_F oppo::SpriteSheet::GetSpriteRect(Size2D spriteIndex) {
	return D2D1::RectF(
		spriteIndex.width * (_padding.left + _padding.right + _spriteSize.width) + _padding.left,
		spriteIndex.height * (_padding.top + _padding.bottom + _spriteSize.height) + _padding.top,
		(spriteIndex.width + 1) * (_padding.left + _padding.right + _spriteSize.width) - _padding.right,
		(spriteIndex.height + 1) * (_padding.top + _padding.bottom + _spriteSize.height) - _padding.bottom
		);
}
#pragma endregion

#pragma region Camera
void oppo::Camera::Fill(Color color) {
	_SafePushLayer();
	(*_ppRT)->Clear(D2D1::ColorF(color.R, color.G, color.B, color.a));
}
void oppo::Camera::Fill(Brush brush) {
	_SafePushLayer();
	(*_ppRT)->Clear(D2D1::ColorF(brush._color.R, brush._color.G, brush._color.B, brush._color.a));
}
void oppo::Camera::FillShape(Rect rect, Brush brush, Transform tf) {
	_SafePushLayer();
	bool isIdentity = tf.IsIdentity();
	Transform cameraTF;
	if (!isIdentity) {
		(*_ppRT)->GetTransform(&cameraTF);
		(*_ppRT)->SetTransform(tf * cameraTF);
	}
	(*_ppRT)->FillRectangle(D2D1::RectF(rect.left, rect.top, rect.right, rect.bottom), brush._pBrush);
	if (!isIdentity) {
		(*_ppRT)->SetTransform(cameraTF);
	}
}
void oppo::Camera::FillShape(RectF rect, Brush brush, Transform tf) {
	_SafePushLayer();
	bool isIdentity = tf.IsIdentity();
	Transform cameraTF;
	if (!isIdentity) {
		(*_ppRT)->GetTransform(&cameraTF);
		(*_ppRT)->SetTransform(tf * cameraTF);
	}
	(*_ppRT)->FillRectangle(D2D1::RectF(rect.left, rect.top, rect.right, rect.bottom), brush._pBrush);
	if (!isIdentity) {
		(*_ppRT)->SetTransform(cameraTF);
	}
}
void oppo::Camera::FillShape(RoundedRect roundedRect, Brush brush, Transform tf) {
	_SafePushLayer();
	bool isIdentity = tf.IsIdentity();
	Transform cameraTF;
	if (!isIdentity) {
		(*_ppRT)->GetTransform(&cameraTF);
		(*_ppRT)->SetTransform(tf * cameraTF);
	}
	(*_ppRT)->FillRoundedRectangle(D2D1::RoundedRect(D2D1::RectF(roundedRect.rect.left, roundedRect.rect.top, roundedRect.rect.right, roundedRect.rect.bottom), roundedRect.rx, roundedRect.ry), brush._pBrush);
	if (!isIdentity) {
		(*_ppRT)->SetTransform(cameraTF);
	}
}
void oppo::Camera::FillShape(Ellipse ellipse, Brush brush, Transform tf) {
	_SafePushLayer();
	bool isIdentity = tf.IsIdentity();
	Transform cameraTF;
	if (!isIdentity) {
		(*_ppRT)->GetTransform(&cameraTF);
		(*_ppRT)->SetTransform(tf * cameraTF);
	}
	(*_ppRT)->FillEllipse(D2D1::Ellipse(D2D1::Point2F(ellipse.center.x, ellipse.center.y), ellipse.rx, ellipse.ry), brush._pBrush);
	if (!isIdentity) {
		(*_ppRT)->SetTransform(cameraTF);
	}
}
void oppo::Camera::DrawShape(Rect rect, Brush brush, Transform tf) {
	_SafePushLayer();
	bool isIdentity = tf.IsIdentity();
	Transform cameraTF;
	if (!isIdentity) {
		(*_ppRT)->GetTransform(&cameraTF);
		(*_ppRT)->SetTransform(tf * cameraTF);
	}
	(*_ppRT)->DrawRectangle(D2D1::RectF(rect.left, rect.top, rect.right, rect.bottom), brush._pBrush, brush.strokeWidth);
	if (!isIdentity) {
		(*_ppRT)->SetTransform(cameraTF);
	}
}
void oppo::Camera::DrawShape(RectF rect, Brush brush, Transform tf) {
	_SafePushLayer();
	bool isIdentity = tf.IsIdentity();
	Transform cameraTF;
	if (!isIdentity) {
		(*_ppRT)->GetTransform(&cameraTF);
		(*_ppRT)->SetTransform(tf * cameraTF);
	}
	(*_ppRT)->DrawRectangle(D2D1::RectF(rect.left, rect.top, rect.right, rect.bottom), brush._pBrush, brush.strokeWidth);
	if (!isIdentity) {
		(*_ppRT)->SetTransform(cameraTF);
	}
}
void oppo::Camera::DrawShape(RoundedRect roundedRect, Brush brush, Transform tf) {
	_SafePushLayer();
	bool isIdentity = tf.IsIdentity();
	Transform cameraTF;
	if (!isIdentity) {
		(*_ppRT)->GetTransform(&cameraTF);
		(*_ppRT)->SetTransform(tf * cameraTF);
	}
	(*_ppRT)->DrawRoundedRectangle(D2D1::RoundedRect(D2D1::RectF(roundedRect.rect.left, roundedRect.rect.top, roundedRect.rect.right, roundedRect.rect.bottom), roundedRect.rx, roundedRect.ry), brush._pBrush, brush.strokeWidth);
	if (!isIdentity) {
		(*_ppRT)->SetTransform(cameraTF);
	}
}
void oppo::Camera::DrawShape(Ellipse ellipse, Brush brush, Transform tf) {
	_SafePushLayer();
	bool isIdentity = tf.IsIdentity();
	Transform cameraTF;
	if (!isIdentity) {
		(*_ppRT)->GetTransform(&cameraTF);
		(*_ppRT)->SetTransform(tf * cameraTF);
	}
	(*_ppRT)->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(ellipse.center.x, ellipse.center.y), ellipse.rx, ellipse.ry), brush._pBrush, brush.strokeWidth);
	if (!isIdentity) {
		(*_ppRT)->SetTransform(cameraTF);
	}
}
void oppo::Camera::DrawShape(Line line, Brush brush, Transform tf) {
	_SafePushLayer();
	bool isIdentity = tf.IsIdentity();
	Transform cameraTF;
	if (!isIdentity) {
		(*_ppRT)->GetTransform(&cameraTF);
		(*_ppRT)->SetTransform(tf * cameraTF);
	}
	(*_ppRT)->DrawLine(D2D1::Point2F(line.p0.x, line.p0.y), D2D1::Point2F(line.p1.x, line.p1.y), brush._pBrush, brush.strokeWidth);
	if (!isIdentity) {
		(*_ppRT)->SetTransform(cameraTF);
	}
}
void oppo::Camera::DrawShape(Bezier bezier, Brush brush, Transform tf) {
	_SafePushLayer();
}
void oppo::Camera::DrawBitmap(Bitmap bitmap, RectF destRect, float opacity, RectF sourceRect, Transform tf) {
	_SafePushLayer();
	bool isIdentity = tf.IsIdentity();
	Transform cameraTF;
	if (!isIdentity) {
		(*_ppRT)->GetTransform(&cameraTF);
		(*_ppRT)->SetTransform(tf * cameraTF);
	}
	(*_ppRT)->DrawBitmap(bitmap._pBitmap, D2D1::RectF(destRect.left, destRect.top, destRect.right, destRect.bottom), opacity, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, D2D1::RectF(sourceRect.left, sourceRect.top, sourceRect.right, sourceRect.bottom));
	if (!isIdentity) {
		(*_ppRT)->SetTransform(cameraTF);
	}
}
void oppo::Camera::DrawBitmap(Bitmap bitmap, RectF destRect, float opacity, Transform tf) {
	_SafePushLayer();
	bool isIdentity = tf.IsIdentity();
	Transform cameraTF;
	if (!isIdentity) {
		(*_ppRT)->GetTransform(&cameraTF);
		(*_ppRT)->SetTransform(tf * cameraTF);
	}
	(*_ppRT)->DrawBitmap(bitmap._pBitmap, D2D1::RectF(destRect.left, destRect.top, destRect.right, destRect.bottom), opacity, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
	if (!isIdentity) {
		(*_ppRT)->SetTransform(cameraTF);
	}
}
void oppo::Camera::DrawSprite(Sprite sprite, float opacity, Transform tf) {
	_SafePushLayer();
	bool isIdentity = tf.IsIdentity();
	Transform cameraTF;
	if (!isIdentity) {
		(*_ppRT)->GetTransform(&cameraTF);
		(*_ppRT)->SetTransform(tf * cameraTF);
	}
	(*_ppRT)->DrawBitmap(sprite._pSpriteSheet->_pBitmap, D2D1::RectF(sprite.rect.left, sprite.rect.top, sprite.rect.right, sprite.rect.bottom), opacity, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, sprite._pSpriteSheet->GetSpriteRect(sprite.spriteIndex));
	if (!isIdentity) {
		(*_ppRT)->SetTransform(cameraTF);
	}
}
void oppo::Camera::DrawTileMap(TileMap tileMap, Transform tf) {
	_SafePushLayer();
}
void oppo::Camera::DrawText(const char* text, RectF textBox, TextFormat textFormat, Brush brush, TEXT_CLIPPING clipOptions, Transform tf) {
	_SafePushLayer();
	bool isIdentity = tf.IsIdentity();
	Transform cameraTF;
	if (!isIdentity) {
		(*_ppRT)->GetTransform(&cameraTF);
		(*_ppRT)->SetTransform(tf * cameraTF);
	}
	(*_ppRT)->DrawTextW(
		utility::StringToWString(text).c_str(),
		strlen(text),
		textFormat._pTextFormat,
		D2D1::RectF(textBox.left, textBox.top, textBox.right, textBox.bottom),
		brush._pBrush,
		static_cast<D2D1_DRAW_TEXT_OPTIONS>(clipOptions)
	);
	if (!isIdentity) {
		(*_ppRT)->SetTransform(cameraTF);
	}
}

void oppo::Camera::ZoomToWidth(float width) {
	Size2F sz = (*_ppRT)->GetSize();
	float newScale = sz.width / width;
	scale = Size2F(newScale, newScale);
}
void oppo::Camera::ZoomToHeight(float height) {
	Size2F sz = (*_ppRT)->GetSize();
	float newScale = sz.height / height;
	scale = Size2F(newScale, newScale);
}
void oppo::Camera::ZoomToFit(Size2F size) {
	Size2F currentSize = (*_ppRT)->GetSize();
	float scaleX = currentSize.width / size.width;
	float scaleY = currentSize.height / size.height;
	float newScale = min(scaleX, scaleY);
	scale = Size2F(newScale, newScale);
}
void oppo::Camera::ZoomToFill(Size2F size) {
	Size2F currentSize = (*_ppRT)->GetSize();
	float scaleX = currentSize.width / size.width;
	float scaleY = currentSize.height / size.height;
	float newScale = max(scaleX, scaleY);
	scale = Size2F(newScale, newScale);
}

oppo::Point2F oppo::Camera::ScreenToWorld(Point2D point) {
	D2D1_SIZE_F sz = (*_ppRT)->GetSize();
	D2D1_POINT_2F offset = D2D1::Point2F();
	if (_refPoint == CAMERA_REFERENCE::CENTER) {
		offset.x = sz.width / 2;
		offset.y = sz.height / 2;
	}
	else {
		if (_refPoint == CAMERA_REFERENCE::TOP_RIGHT || _refPoint == CAMERA_REFERENCE::BOTTOM_RIGHT) {
			offset.x = sz.width;
		}
		if (_refPoint == CAMERA_REFERENCE::BOTTOM_LEFT || _refPoint == CAMERA_REFERENCE::BOTTOM_RIGHT) {
			offset.y = sz.height;
		}
	}
	D2D1::Matrix3x2F m =
		D2D1::Matrix3x2F::Translation(position.x - offset.x, position.y - offset.y)
		* D2D1::Matrix3x2F::Rotation(-rotation, D2D1::Point2F(position.x, position.y))
		* D2D1::Matrix3x2F::Scale(D2D1::SizeF(1 / scale.width, 1 / scale.height), D2D1::Point2F(position.x, position.y));
	return Point2F(
		(m._11 * point.x) + (m._21 * point.y) + m._31,
		(m._12 * point.x) + (m._22 * point.y) + m._32
	);
}
oppo::Point2D oppo::Camera::WorldToScreen(Point2F point) {
	D2D1_SIZE_F sz = (*_ppRT)->GetSize();
	D2D1_POINT_2F offset = D2D1::Point2F();
	if (_refPoint == CAMERA_REFERENCE::CENTER) {
		offset.x = sz.width / 2;
		offset.y = sz.height / 2;
	}
	else {
		if (_refPoint == CAMERA_REFERENCE::TOP_RIGHT || _refPoint == CAMERA_REFERENCE::BOTTOM_RIGHT) {
			offset.x = sz.width;
		}
		if (_refPoint == CAMERA_REFERENCE::BOTTOM_LEFT || _refPoint == CAMERA_REFERENCE::BOTTOM_RIGHT) {
			offset.y = sz.height;
		}
	}
	D2D1::Matrix3x2F m =
		D2D1::Matrix3x2F::Rotation(rotation, D2D1::Point2F(position.x, position.y))
		* D2D1::Matrix3x2F::Translation(offset.x - position.x, offset.y - position.y)
		* D2D1::Matrix3x2F::Scale(D2D1::SizeF(scale.width, scale.height), offset);
	return Point2D(
		(m._11 * point.x) + (m._21 * point.y) + m._31,
		(m._12 * point.x) + (m._22 * point.y) + m._32
	);
}

oppo::RectF oppo::Camera::GetRect() {
	Size2F sz = (*_ppRT)->GetSize();
	Point2F topLeft = ScreenToWorld(Point2D(0, 0));
	Point2F topRight = ScreenToWorld(Point2D(sz.width, 0));
	Point2F bottomLeft = ScreenToWorld(Point2D(0, sz.height));
	Point2F bottomRight = ScreenToWorld(Point2D(sz.width, sz.height));
	RectF rc;
	rc.left = min(topLeft.x, min(topRight.x, min(bottomLeft.x, bottomRight.x)));
	rc.top = min(topLeft.y, min(topRight.y, min(bottomLeft.y, bottomRight.y)));
	rc.right = max(topLeft.x, max(topRight.x, max(bottomLeft.x, bottomRight.x)));
	rc.bottom = max(topLeft.y, max(topRight.y, max(bottomLeft.y, bottomRight.y)));
	return rc;
}
oppo::Size2F oppo::Camera::GetSize() {
	RectF rc = GetRect();
	return Size2F(rc.right - rc.left, rc.bottom - rc.top);
}

void oppo::Camera::_SafePushLayer() {
	if (*_ppCurrentLayer == _pLayer) return;
	if (*_ppCurrentLayer != nullptr) {
		(*_ppRT)->PopLayer();
		*_ppCurrentLayer = nullptr;
	}
	(*_ppRT)->PushLayer(_layerParams, _pLayer);
	*_ppCurrentLayer = _pLayer;
	D2D1_SIZE_F sz = (*_ppRT)->GetSize();
	
	
	D2D1_POINT_2F offset = D2D1::Point2F();
	if (_refPoint == CAMERA_REFERENCE::CENTER) {
		offset.x = sz.width / 2;
		offset.y = sz.height / 2;
	}
	else {
		if (_refPoint == CAMERA_REFERENCE::TOP_RIGHT || _refPoint == CAMERA_REFERENCE::BOTTOM_RIGHT) {
			offset.x = sz.width;
		}
		if (_refPoint == CAMERA_REFERENCE::BOTTOM_LEFT || _refPoint == CAMERA_REFERENCE::BOTTOM_RIGHT) {
			offset.y = sz.height;
		}
	}
	D2D1_MATRIX_3X2_F transform;
	(*_ppRT)->SetTransform(
		D2D1::Matrix3x2F::Scale(D2D1::SizeF(scale.width, scale.height), D2D1::Point2F(position.x, position.y))
		* D2D1::Matrix3x2F::Rotation(rotation, D2D1::Point2F(position.x, position.y)) 
		* D2D1::Matrix3x2F::Translation(offset.x - position.x, offset.y - position.y)
	);
}
#pragma endregion

#pragma region Animation Manager
void oppo::_AnimationManager::_NextFrame() {
	std::vector<std::function<void()>> callbacks;
	for (auto a = _animations.begin(); a != _animations.end();) {
		if (!(*a)->isPaused) {
			(*a)->NextFrame();
			if ((*a)->iFrame == (*a)->size) {
				(*a)->iFrame = 0;
				if ((*a)->loop > 0) {
					(*a)->loop--;
				}
				if ((*a)->loop == 0) {
					if ((*a)->callback) {
						callbacks.push_back((*a)->callback);
					}
					a = _animations.erase(a);
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
	for (auto a = _animations.begin(); a != _animations.end(); a++) {
		if ((*a)->id == id) {
			_animations.erase(a);
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
	for (auto a = _animations.begin(); a != _animations.end(); a++) {
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
	for (auto a = _animations.begin(); a != _animations.end(); a++) {
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
	for (auto a = _animations.begin(); a != _animations.end(); a++) {
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
	for (auto a = _animations.begin(); a != _animations.end(); a++) {
		if ((*a)->id == id) {
			(*a)->iFrame = 0;
			return oppo::ERRORS::SUCCESS;
		}
	}
	return oppo::ERRORS::FAIL; // id not found
}

#pragma endregion

#pragma region Resource Manager
void oppo::_ResourceManager::Init(Window* wnd) {
	wnd->_ppRT = &pRT;
	wnd->_ppCurrentLayer = &currentLayer;
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
				properties.color = pBrush->_color;
				properties.strokewidth = pBrush->strokeWidth;
				hr = CreateBrush(pBrush, properties); 
			}
		}
		for (auto pBitmap : bitmaps) {
			if (SUCCEEDED(hr)) { 
				BitmapProperties properties;
				properties.fileName = pBitmap->_fileName.c_str();
				hr = CreateBitmap(pBitmap, properties);
			}
		}
		for (auto pSpriteSheet : spriteSheets) {
			if (SUCCEEDED(hr)) { 
				SpriteSheetProperties properties;
				properties.fileName = pSpriteSheet->_fileName.c_str();
				properties.padding = pSpriteSheet->_padding;
				properties.spriteCount = pSpriteSheet->_spriteCount;
				properties.spriteSize = pSpriteSheet->_spriteSize;
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
		utility::SafeRelease(&textFormat->_pTextFormat);
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
		hr = pRT->CreateSolidColorBrush(D2D1::ColorF(color.R, color.G, color.B, color.a), &pBrush->_pBrush);
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
		hr = LoadBitmapFromFile(properties.fileName, &pBitmap->_pBitmap);
		if (SUCCEEDED(hr)) {
			pBitmap->_fileName = properties.fileName;
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
		hr = LoadBitmapFromFile(properties.fileName, &pSpriteSheet->_pBitmap);
	}

	if (SUCCEEDED(hr)) {
		pSpriteSheet->_fileName = properties.fileName;
		pSpriteSheet->_spriteSize = properties.spriteSize;
		pSpriteSheet->_spriteCount = properties.spriteCount;
		pSpriteSheet->_padding = properties.padding;
		if (std::find(spriteSheets.begin(), spriteSheets.end(), pSpriteSheet) == spriteSheets.end()) {
			spriteSheets.push_back(pSpriteSheet);
		}
	}

	return hr;
}
HRESULT oppo::_ResourceManager::CreateSprite(Sprite* pSprite, SpriteProperties properties) {
	HRESULT hr = E_FAIL;

	if (properties.pSpriteSheet->_pBitmap) {
		hr = S_OK;
	}

	if (SUCCEEDED(hr)) {
		pSprite->_pSpriteSheet = properties.pSpriteSheet;
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
			&pTextFormat->_pTextFormat
		);
	}

	if (SUCCEEDED(hr)) {
		hr = pTextFormat->_pTextFormat->SetTextAlignment(static_cast<DWRITE_TEXT_ALIGNMENT>(properties.hAlignment));
	}

	if (SUCCEEDED(hr)) {
		hr = pTextFormat->_pTextFormat->SetParagraphAlignment(static_cast<DWRITE_PARAGRAPH_ALIGNMENT>(properties.vAlignment));
	}

	if (SUCCEEDED(hr)) {
		hr = pTextFormat->_pTextFormat->SetWordWrapping(static_cast<DWRITE_WORD_WRAPPING>(properties.wordWrapping));
	}

	if (SUCCEEDED(hr)) {
		float lineSpacing = properties.fontSize * (properties.lineSpacing);
		hr = pTextFormat->_pTextFormat->SetLineSpacing(DWRITE_LINE_SPACING_METHOD_UNIFORM, lineSpacing, .8 * lineSpacing);
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
		pCamera->_ppRT = &pRT;
		hr = pRT->CreateLayer(&pCamera->_pLayer);
	}
	if (SUCCEEDED(hr)) {
		pCamera->_ppCurrentLayer = &currentLayer;
		pCamera->_refPoint = properties.referencePoint;
		if (std::find(cameras.begin(), cameras.end(), pCamera) == cameras.end()) {
			cameras.push_back(pCamera);
		}
	}

	return hr;

}

void oppo::_ResourceManager::DestroyBrush(Brush* pBrush) {
	utility::SafeRelease(&pBrush->_pBrush);
	utility::DeleteFromVector(brushes, pBrush);
}
void oppo::_ResourceManager::DestroyBitmap(Bitmap* pBitmap) {
	utility::SafeRelease(&pBitmap->_pBitmap);
	pBitmap->_fileName = "";
	utility::DeleteFromVector(bitmaps, pBitmap);
}
void oppo::_ResourceManager::DestroySpriteSheet(SpriteSheet* pSpriteSheet) {
	utility::SafeRelease(&pSpriteSheet->_pBitmap);
	pSpriteSheet->_fileName = "";
	pSpriteSheet->_spriteSize = Size2D();
	pSpriteSheet->_spriteCount = Size2D();
	pSpriteSheet->_padding = Rect();
	utility::DeleteFromVector(spriteSheets, pSpriteSheet);
}
void oppo::_ResourceManager::DestroySprite(Sprite* pSprite) {
	pSprite->_pSpriteSheet = nullptr;
	pSprite->rect = RectF();
	pSprite->spriteIndex = Size2D();
}
void oppo::_ResourceManager::DestroyTextFormat(TextFormat* pTextFormat) {
	utility::SafeRelease(&pTextFormat->_pTextFormat);
	utility::DeleteFromVector(textFormats, pTextFormat);
}
void oppo::_ResourceManager::DestroyCamera(Camera* pCamera) {
	utility::SafeRelease(&pCamera->_pLayer);
	pCamera->_ppCurrentLayer = nullptr;
	pCamera->_ppRT = nullptr;
	pCamera->_layerParams = D2D1::LayerParameters();
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
oppo::_Engine* oppo::_Engine::_pInstance = nullptr;

oppo::Result oppo::_Engine::AddWindow(Window* wnd, WindowProperties properties) {
	HWND hWnd = wnd->Create(properties);
	if (hWnd == NULL) {
		return ERRORS::FAIL;
	}
	_windows.push_back(hWnd);
	return ERRORS::SUCCESS;
}

void oppo::_Engine::RemoveWindow(Window* wnd) {
	wnd->_tGameLoopTimer.join();
	utility::DeleteFromVector(_windows, wnd->_hWnd);
	if (_windows.size() == 0) {
		PostQuitMessage(0);
	}
}

void oppo::_Engine::Terminate() {
	for (auto hWnd : _windows) {
		DestroyWindow(hWnd);
	}
}

void oppo::_Engine::Run() {
	for (auto hWnd : _windows) {
		ShowWindow(hWnd, SW_SHOWDEFAULT);
	}

	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

oppo::_Engine* oppo::_Engine::GetInstance() {
	if (_pInstance == nullptr) {
		_pInstance = new _Engine();
	}
	return _pInstance;
}

oppo::Result oppo::CreateWindow(Window* wnd, WindowProperties properties) {
	return _Engine::GetInstance()->AddWindow(wnd, properties);
}

void oppo::Run() {
	_Engine::GetInstance()->Run();
}

void oppo::Terminate() {
	_Engine::GetInstance()->Terminate();
}

#pragma endregion