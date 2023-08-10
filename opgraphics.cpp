#include "opgraphics.h"
#include <iostream>
std::wstring oppo::utility::StringToWString(const std::string& str)
{
	int count = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), NULL, 0);
	std::wstring wstr(count, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), &wstr[0], count);
	return wstr;
}

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

void oppo::Camera::FillShape(Rect rect, Brush brush) {
	SafePushLayer();
	(*ppRT)->FillRectangle(D2D1::RectF(rect.left, rect.top, rect.right, rect.bottom), brush.pBrush);
}

void oppo::Camera::FillShape(RectF rect, Brush brush) {
	SafePushLayer();
	(*ppRT)->FillRectangle(D2D1::RectF(rect.left, rect.top, rect.right, rect.bottom), brush.pBrush);
}

void oppo::Camera::FillShape(RoundedRect roundedRect, Brush brush) {
	SafePushLayer();
	(*ppRT)->FillRoundedRectangle(D2D1::RoundedRect(D2D1::RectF(roundedRect.rect.left, roundedRect.rect.top, roundedRect.rect.right, roundedRect.rect.bottom), roundedRect.rx, roundedRect.ry), brush.pBrush);
}

void oppo::Camera::FillShape(Ellipse ellipse, Brush brush) {
	SafePushLayer();
	(*ppRT)->FillEllipse(D2D1::Ellipse(D2D1::Point2F(ellipse.center.x, ellipse.center.y), ellipse.rx, ellipse.ry), brush.pBrush);
}

void oppo::Camera::DrawShape(Rect rect, Brush brush) {
	SafePushLayer();
	(*ppRT)->DrawRectangle(D2D1::RectF(rect.left, rect.top, rect.right, rect.bottom), brush.pBrush);
}

void oppo::Camera::DrawShape(RectF rect, Brush brush) {
	SafePushLayer();
	(*ppRT)->FillRectangle(D2D1::RectF(rect.left, rect.top, rect.right, rect.bottom), brush.pBrush);
}

void oppo::Camera::DrawShape(RoundedRect roundedRect, Brush brush) {
	SafePushLayer();
	(*ppRT)->DrawRoundedRectangle(D2D1::RoundedRect(D2D1::RectF(roundedRect.rect.left, roundedRect.rect.top, roundedRect.rect.right, roundedRect.rect.bottom), roundedRect.rx, roundedRect.ry), brush.pBrush);
}

void oppo::Camera::DrawShape(Ellipse ellipse, Brush brush) {
	SafePushLayer();
	(*ppRT)->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(ellipse.center.x, ellipse.center.y), ellipse.rx, ellipse.ry), brush.pBrush);
}

void oppo::Camera::DrawShape(Line line, Brush brush) {
	SafePushLayer();
	(*ppRT)->DrawLine(D2D1::Point2F(line.p0.x, line.p0.y), D2D1::Point2F(line.p1.x, line.p1.y), brush.pBrush, brush.strokeWidth);
}

void oppo::Camera::DrawShape(Bezier bezier, Brush brush) {
	SafePushLayer();
}

void oppo::Camera::DrawBitmap(Bitmap bitmap, RectF destRect, float opacity = 1.f, RectF sourceRect = RectF()) {
	SafePushLayer();
	(*ppRT)->DrawBitmap(bitmap.pBitmap, D2D1::RectF(destRect.left, destRect.top, destRect.right, destRect.bottom), opacity, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, D2D1::RectF(sourceRect.left, sourceRect.top, sourceRect.right, sourceRect.bottom));
}

void oppo::Camera::DrawBitmap(Bitmap bitmap, RectF destRect, float opacity = 1.f) {
	SafePushLayer();
	(*ppRT)->DrawBitmap(bitmap.pBitmap, D2D1::RectF(destRect.left, destRect.top, destRect.right, destRect.bottom), opacity, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
}

void oppo::Camera::DrawSprite(Sprite sprite) {
	SafePushLayer();
	D2D1_RECT_F drawRect = D2D1::RectF(
		sprite.position.x + (sprite.rect.left - sprite.position.x) * sprite.scale.width,
		sprite.position.y + (sprite.rect.top - sprite.position.y) * sprite.scale.height,
		sprite.position.x + (sprite.rect.right - sprite.position.x) * sprite.scale.width,
		sprite.position.y + (sprite.rect.bottom - sprite.position.y) * sprite.scale.height);
	(*ppRT)->DrawBitmap(sprite.pSpriteSheet->pBitmap, drawRect, sprite.opacity, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, sprite.pSpriteSheet->GetSpriteRect(sprite.spriteIndex));
}

void oppo::Camera::DrawTileMap(TileMap tileMap) {
	SafePushLayer();
}

void oppo::Camera::DrawText(const char* text, Point2F position, TextLayout textLayout) {}

void oppo::Camera::DrawText(const char* text, TextLayout textLayout) {}

oppo::RectF oppo::Camera::GetWindowRect() {
	D2D1_SIZE_F sz = (*ppRT)->GetSize();
	return RectF(position.x, position.y, position.x + sz.width, position.y + sz.height);
}

oppo::Size2F oppo::Camera::GetWindowSize() {
	return (*ppRT)->GetSize();
}

void oppo::Camera::SafePushLayer() {
	if (*ppCurrentLayer == pLayer) return;
	if (*ppCurrentLayer != nullptr) {
		(*ppRT)->PopLayer();
		*ppCurrentLayer = nullptr;
	}
	(*ppRT)->PushLayer(layerParams, pLayer);
	*ppCurrentLayer = pLayer;
	(*ppRT)->SetTransform(D2D1::Matrix3x2F::Translation(-position.x, -position.y));
}
#pragma endregion

#pragma region Resource Manager
void oppo::ResourceManager::Init(WindowManager* wm, HWND hWnd) {
	this->hWnd = hWnd;
	wm->ppRT = &pRT;
	wm->ppCurrentLayer = &currentLayer;
}

HRESULT oppo::ResourceManager::CreateWindowResources() {
	HRESULT hr = CreateDIResources();
	if (SUCCEEDED(hr)) {
		hr = CreateDDResources();
	}
	return hr;
}

void oppo::ResourceManager::DestroyWindowResources() {
	DestroyDDResources();
	DestroyDIResources();
}

HRESULT oppo::ResourceManager::CreateDIResources() {
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

	return hr;
}

HRESULT oppo::ResourceManager::CreateDDResources() {
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

HRESULT oppo::ResourceManager::RecreateDDResources() {
	HRESULT hr = CreateDDResources();

	if (SUCCEEDED(hr)) {
		for (auto pBrush : brushes) {
			if (SUCCEEDED(hr)) { hr = CreateBrush(pBrush, pBrush->color); }
		}
		for (auto pBitmap : bitmaps) {
			if (SUCCEEDED(hr)) { hr = CreateBitmap(pBitmap->fileName.c_str(), pBitmap); }
		}
		for (auto pSpriteSheet : spriteSheets) {
			if (SUCCEEDED(hr)) { hr = CreateSpriteSheet(pSpriteSheet->fileName.c_str(), pSpriteSheet->pxSpriteResolution, pSpriteSheet->spriteCount, pSpriteSheet->padding, pSpriteSheet); }
		}
		for (auto pCamera : cameras) {
			if (SUCCEEDED(hr)) { hr = CreateCamera(pCamera); }
		}
	}

	return hr;
}

void oppo::ResourceManager::DestroyDIResources() {
	utility::SafeRelease(&pFactory);
	utility::SafeRelease(&pFactoryWIC);
}

void oppo::ResourceManager::DestroyDDResources() {
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

HRESULT oppo::ResourceManager::CreateBrush(Brush* pBrush, Color color = Color()) {
	HRESULT hr = E_FAIL;
	
	if (pRT) {
		hr = pRT->CreateSolidColorBrush(D2D1::ColorF(color.R, color.G, color.B, color.a), &pBrush->pBrush);
	}
	if (SUCCEEDED(hr) && (std::find(brushes.begin(), brushes.end(), pBrush) == brushes.end())) {
		brushes.push_back(pBrush);
	}

	return hr;
}

HRESULT oppo::ResourceManager::CreateBitmap(const char* fileName, Bitmap* pBitmap) {
	HRESULT hr = S_OK;

	if (pRT) {
		hr = LoadBitmapFromFile(fileName, &pBitmap->pBitmap);
		if (SUCCEEDED(hr)) {
			pBitmap->fileName = fileName;
			if (std::find(bitmaps.begin(), bitmaps.end(), pBitmap) == bitmaps.end()) {
				bitmaps.push_back(pBitmap);
			}
		}
	}

	return hr;
}

HRESULT oppo::ResourceManager::CreateSpriteSheet(const char* fileName, Size2D spriteSize, Size2D spriteCount, Rect padding, SpriteSheet* pSpriteSheet) {
	HRESULT hr = E_FAIL;
	if (pRT) {
		hr = LoadBitmapFromFile(fileName, &pSpriteSheet->pBitmap);
	}

	if (SUCCEEDED(hr)) {
		pSpriteSheet->pxSheetResolution = pSpriteSheet->pBitmap->GetSize();
		pSpriteSheet->pxSpriteResolution = spriteSize;
		pSpriteSheet->spriteCount = spriteCount;
		pSpriteSheet->padding = padding;
		if (std::find(spriteSheets.begin(), spriteSheets.end(), pSpriteSheet) == spriteSheets.end()) {
			spriteSheets.push_back(pSpriteSheet);
		}
	}

	return hr;
}

HRESULT oppo::ResourceManager::CreateSprite(Sprite* pSprite, SpriteSheet* pSpriteSheet, RectF spriteRect, Size2D spriteIndex = Size2D()) {
	HRESULT hr = E_FAIL;

	if (pSpriteSheet->pBitmap) {
		hr = S_OK;
	}

	if (SUCCEEDED(hr)) {
		pSprite->pSpriteSheet = pSpriteSheet;
		pSprite->rect = spriteRect;
		pSprite->spriteIndex = spriteIndex;
	}

	return hr;
}

HRESULT oppo::ResourceManager::CreateCamera(Camera* pCamera) {
	HRESULT hr = E_FAIL;
	
	if (pRT) {
		pCamera->ppRT = &pRT;
		hr = pRT->CreateLayer(&pCamera->pLayer);
	}
	if (SUCCEEDED(hr)) {
		pCamera->ppCurrentLayer = &currentLayer;
		if (std::find(cameras.begin(), cameras.end(), pCamera) == cameras.end()) {
			cameras.push_back(pCamera);
		}
	}

	return hr;

}

void oppo::ResourceManager::DestroyBrush(Brush* pBrush) {
	utility::SafeRelease(&pBrush->pBrush);
	utility::DeleteFromVector(brushes, pBrush);
}

void oppo::ResourceManager::DestroyBitmap(Bitmap* pBitmap) {
	utility::SafeRelease(&pBitmap->pBitmap);
	pBitmap->fileName = "";
	utility::DeleteFromVector(bitmaps, pBitmap);
}

void oppo::ResourceManager::DestroySpriteSheet(SpriteSheet* pSpriteSheet) {
	utility::SafeRelease(&pSpriteSheet->pBitmap);
	pSpriteSheet->fileName = "";
	pSpriteSheet->pxSheetResolution = Size2D();
	pSpriteSheet->pxSpriteResolution = Size2D();
	pSpriteSheet->spriteCount = Size2D();
	pSpriteSheet->padding = Rect();
	utility::DeleteFromVector(spriteSheets, pSpriteSheet);
}

void oppo::ResourceManager::DestroySprite(Sprite* pSprite) {
	pSprite->pSpriteSheet = nullptr;
	pSprite->rect = RectF();
	pSprite->position = Point2F();
	pSprite->rotation = 0;
	pSprite->scale = Size2F(1, 1);
	pSprite->spriteIndex = Size2D();
}

void oppo::ResourceManager::DestroyCamera(Camera* pCamera) {
	utility::SafeRelease(&pCamera->pLayer);
	pCamera->ppCurrentLayer = nullptr;
	pCamera->ppRT = nullptr;
	pCamera->layerParams = D2D1::LayerParameters();
	utility::DeleteFromVector(cameras, pCamera);
}

HRESULT oppo::ResourceManager::LoadBitmapFromFile(const char* fileName, ID2D1Bitmap** ppBitmap) {
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
#pragma endregion

#pragma region Window Manager
oppo::Result oppo::WindowManager::Init(WindowPackage wp) {
	NewClassName();
	WNDCLASS wc = { };
	wc.style = CS_DBLCLKS;
	wc.lpfnWndProc = [](HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT {
		WindowManager* pThis = reinterpret_cast<WindowManager*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		if (uMsg == WM_NCCREATE) {
			CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
			pThis = static_cast<WindowManager*>(pCreate->lpCreateParams);
			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
		}

		if (pThis) {
			return pThis->WindowProc(hwnd, uMsg, wParam, lParam);
		}

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	};

	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = className;
	RegisterClass(&wc);

	if (wp.szScreen.width == 0) wp.szScreen.width = CW_USEDEFAULT;
	if (wp.szScreen.height == 0) wp.szScreen.height = CW_USEDEFAULT;

	wndState = WNDSTATE::CREATE;
	hWnd = CreateWindowEx(
		0,
		className,
		utility::StringToWString(wp.windowName).c_str(),
		WS_OVERLAPPEDWINDOW,
		// Size and position: X, Y, Width, Height
		wp.szScreen.width, wp.szScreen.height, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,       // Parent window    
		NULL,       // Menu
		GetModuleHandle(NULL),  // Instance handle
		this        // Additional application data
	);

	if (hWnd == NULL) {
		return ERRORS::FAIL;
	}

	backgroundColor = wp.backgroundColor;
	szMin = wp.szMin;
	szMax = wp.szMax;
	aspectRatio = wp.aspectRatio;
	SetFPS(wp.fps);
	SetUPS(wp.ups);
	winID = wp.windowID;

	tGameLoopTimer = std::thread([this]() { GameLoopTimer(); });

	return ERRORS::SUCCESS;
}

oppo::Result oppo::WindowManager::Run() {
	if (hWnd == NULL) {
		return ERRORS::FAIL;
	}

	ShowWindow(hWnd, SW_SHOWDEFAULT);

	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	tGameLoopTimer.join();
	return ERRORS::SUCCESS;
}

void oppo::WindowManager::RegisterGameLoop(Result(*Func)(Event)) {
	GameLoop = Func;
}

void oppo::WindowManager::SetFPS(float fps) {
	if (fps == 0) {
		renderCountTarget = -1; // avoid divide by zero error
	}
	else {
		renderCountTarget = (int)(1000000 / fps); // game loop timers are in microseconds
	}
}

void oppo::WindowManager::SetUPS(float ups) {
	if (ups == 0) {
		updateCountTarget = -1; // avoid divide by zero error
	}
	else {
		updateCountTarget = (int)(1000000 / ups); // game loop timers are in microseconds
	}
}

oppo::Result oppo::WindowManager::CreateBrush(Brush* pBrush) {
	HRESULT hr = resourceManager.CreateBrush(pBrush, pBrush->GetColor());
	if (SUCCEEDED(hr)) return ERRORS::SUCCESS;
	return ERRORS::FAIL;
}

oppo::Result oppo::WindowManager::CreateBitmap(const char* fileName, Bitmap* pBitmap) {
	HRESULT hr = resourceManager.CreateBitmap(fileName, pBitmap);
	if (SUCCEEDED(hr)) return ERRORS::SUCCESS;
	return ERRORS::FAIL;
}

oppo::Result oppo::WindowManager::CreateSpriteSheet(const char* fileName, Size2D spriteSize, Size2D spriteCount, Rect padding, SpriteSheet* pSpriteSheet) {
	HRESULT hr = resourceManager.CreateSpriteSheet(fileName, spriteSize, spriteCount, padding, pSpriteSheet);
	if (SUCCEEDED(hr)) return ERRORS::SUCCESS;
	return ERRORS::FAIL;
}

oppo::Result oppo::WindowManager::CreateSprite(Sprite* pSprite, SpriteSheet* pSpriteSheet, RectF spriteRect, Size2D spriteIndex = Size2D()) {
	HRESULT hr = resourceManager.CreateSprite(pSprite, pSpriteSheet, spriteRect, spriteIndex);
	if (SUCCEEDED(hr)) return ERRORS::SUCCESS;
	return ERRORS::FAIL;
}

oppo::Result oppo::WindowManager::CreateCamera(Camera* pCamera) {
	HRESULT hr = resourceManager.CreateCamera(pCamera);
	if (SUCCEEDED(hr)) return ERRORS::SUCCESS;
	return ERRORS::FAIL;
}

void oppo::WindowManager::DestroyBrush(Brush* pBrush) {
	resourceManager.DestroyBrush(pBrush);
}

void oppo::WindowManager::DestroyBitmap(Bitmap* pBitmap) {
	resourceManager.DestroyBitmap(pBitmap);
}

void oppo::WindowManager::DestroySpriteSheet(SpriteSheet* pSpriteSheet) {
	resourceManager.DestroySpriteSheet(pSpriteSheet);
}

void oppo::WindowManager::DestroySprite(Sprite* pSprite) {
	resourceManager.DestroySprite(pSprite);
}

void oppo::WindowManager::DestroyCamera(Camera* pCamera) {
	resourceManager.DestroyCamera(pCamera);
}

void oppo::WindowManager::NewClassName() {
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

void oppo::WindowManager::GameLoopTimer() {
	long long renderCount;
	long long updateCount;
	swRender.Reset();
	swUpdate.Reset();
	while (wndState != WNDSTATE::DESTROY) {
		renderCount = swRender.Elapsed().count();
		updateCount = swUpdate.Elapsed().count();
		if (updateCount >= updateCountTarget && wndState == WNDSTATE::RUN) {
			swUpdate.Reset();
			PostMessage(hWnd, WM_FRAME, updateCount, 0);
		}

		if (renderCount >= renderCountTarget && wndState == WNDSTATE::RUN) {
			swRender.Reset();
			PostMessage(hWnd, WM_FRAME, renderCount, 1);
			InvalidateRect(hWnd, NULL, FALSE);
		}
	}
}

oppo::KEYS oppo::WindowManager::TranslateKeystroke(int vkCode) {
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
		return BACK;
	case VK_TAB: // tab
		return TAB;
	case VK_RETURN: // enter
		return ENTER;
	case VK_SHIFT: // shift
		return SHIFT;
	case VK_CONTROL: // control
		return CTRL;
	case VK_CAPITAL: // capslock
		return CAPS;
	case VK_ESCAPE: // escape
		return ESCAPE;
	case VK_SPACE: // spacebar
		return SPACE;
	case VK_PRIOR: // pg up
		return PGUP;
	case VK_NEXT: // pg down
		return PGDN;
	case VK_END: // end
		return END;
	case VK_HOME: // home
		return HOME;
	case VK_LEFT: // left arrow
		return LEFT;
	case VK_UP: // up arrow
		return UP;
	case VK_RIGHT: // right arrow
		return RIGHT;
	case VK_DOWN: // down arrow
		return DOWN;
	case VK_INSERT: // insert
		return INSERT;
	case VK_DELETE: // delete
		return DELETE;
	case VK_MENU:
		return ALT;
	case VK_MULTIPLY: // numpad multiply
		return NP_MULTIPLY;
	case VK_ADD: // numpad add
		return NP_ADD;
	case VK_SUBTRACT: // numpad subtract
		return NP_SUBTRACT;
	case VK_DECIMAL: // numpad decimal
		return NP_DECIMAL;
	case VK_DIVIDE: // numpad divide
		return NP_DIVIDE;
	case VK_NUMLOCK: // num lock
		return NP_LOCK;
	case VK_SCROLL: // scroll lock
		return SCROLL_LOCK;
	case VK_VOLUME_MUTE: // volume mute
		return MEDIA_VOLUME_MUTE;
	case VK_VOLUME_DOWN: // volume down
		return MEDIA_VOLUME_DOWN;
	case VK_VOLUME_UP: // volume up
		return MEDIA_VOLUME_UP;
	case VK_MEDIA_NEXT_TRACK: // next track, not typically found on keyboards but included for specialized controllers
		return MEDIA_NEXT;
	case VK_MEDIA_PREV_TRACK: // previous track, not typically found on keyboards but included for specialized controllers
		return MEDIA_PREV;
	case VK_MEDIA_STOP: // stop media, not typically found on keyboards but included for specialized controllers
		return MEDIA_STOP;
	case VK_OEM_1: // ;:
		return COLON;
	case VK_OEM_2: // /?
		return QMARK;
	case VK_OEM_3: // ~
		return TILDE;
	case VK_OEM_4: // [{
		return LBRACKET;
	case VK_OEM_5: // \|
		return BSLASH;
	case VK_OEM_6: // ]}
		return RBRACKET;
	case VK_OEM_7: // '"
		return QUOTE;
	case VK_OEM_PLUS: // =+
		return PLUS;
	case VK_OEM_COMMA: // ,<
		return COMMA;
	case VK_OEM_MINUS: // -_
		return MINUS;
	case VK_OEM_PERIOD: // .>
		return PERIOD;
	default:
		return EXTRA;
	}
}

LRESULT CALLBACK oppo::WindowManager::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	Event e = { };
	e.windowID = winID;
	switch (uMsg) {
	case WM_CREATE: { 
		resourceManager.Init(this, hWnd);
		if (FAILED(resourceManager.CreateWindowResources())) {
			return -1;
		}
		if (GameLoop) {
			e.type = EVENTS::CREATE;
			GameLoop(e);
		}
		wndState = WNDSTATE::RUN;
		return 0; 
	}
	case WM_CLOSE: {
		if (GameLoop) {
			e.type = EVENTS::CLOSE;
			Result r = GameLoop(e);
			if (Succeeded(r)) {
				wndState = WNDSTATE::DESTROY;
				DestroyWindow(hWnd);
			}
		}
		else {
			DestroyWindow(hWnd);
		}
		return 0; 
	}
	case WM_DESTROY: { 
		if (GameLoop) {
			e.type = EVENTS::DESTROY;
			GameLoop(e);
		}
		resourceManager.DestroyWindowResources();
		PostQuitMessage(0);
		return 0; 
	}
	case WM_SIZE: {
		(*ppRT)->Resize(D2D1::SizeU(LOWORD(lParam), HIWORD(lParam)));
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;
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
			resourceManager.RecreateDDResources();
			InvalidateRect(hWnd, NULL, FALSE);
		}

		return 0;
	}
	case WM_FRAME: {
		if (GameLoop) {
			long long usCount = (long long)(wParam);
			if (lParam == 0) {
				// update
				e.dt = usCount / 1000000.;
				e.type = EVENTS::UPDATE;
				GameLoop(e);
			}
			else if (lParam == 1) {
				// render
				InvalidateRect(hWnd, NULL, FALSE);
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
			e.key = TranslateKeystroke(vkCode);
			GameLoop(e);
		}

		return 0;
	}
	case WM_KEYUP: {
		WORD vkCode = LOWORD(wParam);

		if (GameLoop) {
			e.type = EVENTS::KEYUP;
			e.key = TranslateKeystroke(vkCode);
			GameLoop(e);
		}

		return 0;
	}
	case WM_CHAR: {
		if (GameLoop) {
			e.type = EVENTS::CHAR;
			e.character = static_cast<char>(wParam);
			e.flags.repeat = LOWORD(lParam);
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
			e.key = TranslateKeystroke(vkCode);
			e.flags.alt = 1;
			GameLoop(e);
		}

		return 0;
	}
	case WM_SYSKEYUP: {
		WORD vkCode = LOWORD(wParam);

		if (GameLoop) {
			e.type = EVENTS::KEYUP;
			e.key = TranslateKeystroke(vkCode);
			e.flags.alt = 1;
			GameLoop(e);
		}

		return 0;
	}
	case WM_SYSCHAR: {
		if (GameLoop) {
			e.type = EVENTS::CHAR;
			e.character = static_cast<char>(wParam);
			e.flags.repeat = LOWORD(lParam);
			e.flags.alt = 1;
			GameLoop(e);
		}

		return 0;
	}

	// mouse
	case WM_MOUSEMOVE: {
		if (GameLoop) {
			e.type = EVENTS::MOUSEMOVE;
			e.mouse.x = GET_X_LPARAM(lParam);
			e.mouse.y = GET_Y_LPARAM(lParam);
			if (GetKeyState(VK_MENU) & 0x8000) {
				e.flags.alt = 1;
			}
			GameLoop(e);
		}
		return 0;
	}
	case WM_MOUSEWHEEL: {
		if (GameLoop) {
			e.type = EVENTS::MOUSESCROLL;
			e.mouse.x = GET_X_LPARAM(lParam);
			e.mouse.y = GET_Y_LPARAM(lParam);
			e.flags.vScroll = 1;
			if (GetKeyState(VK_MENU) & 0x8000) {
				e.flags.alt = 1;
			}
			GameLoop(e);
		}
		return 0;
	}
	case WM_MOUSEHWHEEL: {
		if (GameLoop) {
			e.type = EVENTS::MOUSESCROLL;
			e.mouse.x = GET_X_LPARAM(lParam);
			e.mouse.y = GET_Y_LPARAM(lParam);
			e.flags.hScroll = 1;
			if (GetKeyState(VK_MENU) & 0x8000) {
				e.flags.alt = 1;
			}
			GameLoop(e);
		}
		return 0;
	}
	case WM_LBUTTONDOWN: {
		if (GameLoop) {
			e.type = EVENTS::MOUSEDOWN;
			e.mouse.x = GET_X_LPARAM(lParam);
			e.mouse.y = GET_Y_LPARAM(lParam);
			e.key = KEYS::LMOUSE;
			if (GetKeyState(VK_MENU) & 0x8000) {
				e.flags.alt = 1;
			}
			GameLoop(e);
		}

		return 0;
	}
	case WM_LBUTTONUP: {
		if (GameLoop) {
			e.type = EVENTS::MOUSEDOWN;
			e.mouse.x = GET_X_LPARAM(lParam);
			e.mouse.y = GET_Y_LPARAM(lParam);
			e.key = KEYS::LMOUSE;
			if (GetKeyState(VK_MENU) & 0x8000) {
				e.flags.alt = 1;
			}
			GameLoop(e);
		}

		return 0;
	}
	case WM_LBUTTONDBLCLK: {
		if (GameLoop) {
			e.type = EVENTS::MOUSEDBLCLK;
			e.mouse.x = GET_X_LPARAM(lParam);
			e.mouse.y = GET_Y_LPARAM(lParam);
			e.key = KEYS::LMOUSE;
			if (GetKeyState(VK_MENU) & 0x8000) {
				e.flags.alt = 1;
			}
			GameLoop(e);
		}

		return 0;
	}
	case WM_RBUTTONDOWN: {
		if (GameLoop) {
			e.type = EVENTS::MOUSEDOWN;
			e.mouse.x = GET_X_LPARAM(lParam);
			e.mouse.y = GET_Y_LPARAM(lParam);
			e.key = KEYS::RMOUSE;
			if (GetKeyState(VK_MENU) & 0x8000) {
				e.flags.alt = 1;
			}
			GameLoop(e);
		}

		return 0;
	}
	case WM_RBUTTONUP: {
		if (GameLoop) {
			e.type = EVENTS::MOUSEDOWN;
			e.mouse.x = GET_X_LPARAM(lParam);
			e.mouse.y = GET_Y_LPARAM(lParam);
			e.key = KEYS::RMOUSE;
			if (GetKeyState(VK_MENU) & 0x8000) {
				e.flags.alt = 1;
			}
			GameLoop(e);
		}

		return 0;
	}
	case WM_RBUTTONDBLCLK: {
		if (GameLoop) {
			e.type = EVENTS::MOUSEDBLCLK;
			e.mouse.x = GET_X_LPARAM(lParam);
			e.mouse.y = GET_Y_LPARAM(lParam);
			e.key = KEYS::RMOUSE;
			if (GetKeyState(VK_MENU) & 0x8000) {
				e.flags.alt = 1;
			}
			GameLoop(e);
		}

		return 0;
	}
	case WM_MBUTTONDOWN: {
		if (GameLoop) {
			e.type = EVENTS::MOUSEDOWN;
			e.mouse.x = GET_X_LPARAM(lParam);
			e.mouse.y = GET_Y_LPARAM(lParam);
			e.key = KEYS::MMOUSE;
			if (GetKeyState(VK_MENU) & 0x8000) {
				e.flags.alt = 1;
			}
			GameLoop(e);
		}

		return 0;
	}
	case WM_MBUTTONUP: {
		if (GameLoop) {
			e.type = EVENTS::MOUSEDOWN;
			e.mouse.x = GET_X_LPARAM(lParam);
			e.mouse.y = GET_Y_LPARAM(lParam);
			e.key = KEYS::MMOUSE;
			if (GetKeyState(VK_MENU) & 0x8000) {
				e.flags.alt = 1;
			}
			GameLoop(e);
		}

		return 0;
	}
	case WM_MBUTTONDBLCLK: {
		if (GameLoop) {
			e.type = EVENTS::MOUSEDBLCLK;
			e.mouse.x = GET_X_LPARAM(lParam);
			e.mouse.y = GET_Y_LPARAM(lParam);
			e.key = KEYS::MMOUSE;
			if (GetKeyState(VK_MENU) & 0x8000) {
				e.flags.alt = 1;
			}
			GameLoop(e);
		}

		return 0;
	}
	case WM_XBUTTONDOWN: {
		if (GameLoop) {
			e.type = EVENTS::MOUSEDOWN;
			e.mouse.x = GET_X_LPARAM(lParam);
			e.mouse.y = GET_Y_LPARAM(lParam);
			if (GET_XBUTTON_WPARAM(wParam) & XBUTTON1 == XBUTTON1) {
				e.key = KEYS::XMOUSE1;
			}
			else {
				e.key = KEYS::XMOUSE2;
			}
			if (GetKeyState(VK_MENU) & 0x8000) {
				e.flags.alt = 1;
			}
			GameLoop(e);
		}

		return 0;
	}
	case WM_XBUTTONUP: {
		if (GameLoop) {
			e.type = EVENTS::MOUSEUP;
			e.mouse.x = GET_X_LPARAM(lParam);
			e.mouse.y = GET_Y_LPARAM(lParam);
			if (GET_XBUTTON_WPARAM(wParam) & XBUTTON1 == XBUTTON1) {
				e.key = KEYS::XMOUSE1;
			}
			else {
				e.key = KEYS::XMOUSE2;
			}
			if (GetKeyState(VK_MENU) & 0x8000) {
				e.flags.alt = 1;
			}
			GameLoop(e);
		}

		return 0;
	}
	case WM_XBUTTONDBLCLK: {
		if (GameLoop) {
			e.type = EVENTS::MOUSEDBLCLK;
			e.mouse.x = GET_X_LPARAM(lParam);
			e.mouse.y = GET_Y_LPARAM(lParam);
			if (GET_XBUTTON_WPARAM(wParam) & XBUTTON1 == XBUTTON1) {
				e.key = KEYS::XMOUSE1;
			}
			else {
				e.key = KEYS::XMOUSE2;
			}
			if (GetKeyState(VK_MENU) & 0x8000) {
				e.flags.alt = 1;
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