#pragma once
#include <Windows.h>
#include <windowsx.h>
#include <d2d1_3.h>
#pragma comment(lib, "d2d1.lib")
#include <dwrite_3.h>
#pragma comment(lib, "dwrite.lib")
#include <wincodec.h>

#include <memory>
#include <string>
#include <vector>
#include <codecvt>
#include <thread>
#include <random>
#include <map>
#include <functional>

#include "optypes.h"
#include "timer.h"

#define OP_DEFAULTFPS 60
#define OP_DEFAULTUPS 100
#define OP_DEFAULTAPS 30
#define WM_FRAME WM_USER + 1

namespace oppo::utility {

	std::wstring StringToWString(const std::string& str);

	template <typename T>
	void DeleteFromVector(std::vector<T>& vec, const T& value) {
		auto it = std::find(vec.begin(), vec.end(), value);
		if (it != vec.end()) {
			vec.erase(it);
		}
	}

	template <typename T>
	void SafeRelease(T** ppT) {
		if (*ppT) {
			(*ppT)->Release();
			*ppT = NULL;
		}
	}

	class _BasicNode {
	public:
		_BasicNode(int nInputs) : nInputs(nInputs) {
			inputs.resize(nInputs);
		}

		_BasicNode() {
			nInputs = 0;
		}

		void AddInput(int i, _BasicNode& node) {
			if (i < nInputs) {
				inputs[i] = &node;
			}
		}

		template <typename... T>
		_BasicNode& operator() (T&&... nodes) {
			int n = sizeof...(nodes);
			int i = 0;
			//static_assert(((std::is_same_v<T, Node&> || std::is_same_v<T, int>) && ...), "Must pass valid node or 0 as argument");
			(AddInput(i++, nodes), ...);
			return *this;
		}

	protected:
		std::vector<_BasicNode*> inputs;
		int nInputs;

		void AddInput(int i, int n) {}
	};
}

#pragma region User Graphics
namespace oppo {
	struct BrushProperties {
		Color color = Color();
		float strokewidth = 1.f;
	};
	class Brush {
	public:
		void SetColor(Color color);
		Color GetColor();
		float strokeWidth = 1.f;
	private:
		Color color = Color();
		ID2D1SolidColorBrush* pBrush = nullptr;

		friend class Camera;
		friend class _ResourceManager;
	};

	struct BitmapProperties {
		const char* fileName;
	};
	class Bitmap {
	protected:
		std::string fileName;
		ID2D1Bitmap* pBitmap;

		friend class Camera;
		friend class _ResourceManager;
	};

	struct SpriteSheetProperties {
		const char* fileName;
		Size2D spriteSize;
		Size2D spriteCount = Size2D(1,1);
		Rect padding = Rect();
	};
	class SpriteSheet : Bitmap {
	public:
		Size2D SheetResolution();
		Size2D SpriteResolution();
		Size2D SpriteCount();
		Rect   Padding();

	private:
		Size2D pxSheetResolution;
		Size2D pxSpriteResolution;
		Size2D spriteCount;
		Rect padding;

		D2D1_RECT_F GetSpriteRect(Size2D spriteIndex);

		friend class Camera;
		friend class _ResourceManager;
	};

	struct SpriteProperties {
		SpriteSheet* pSpriteSheet;
		RectF spriteRect;
		Size2D spriteIndex;
		float rotation = 0.f;
		float opacity = 1.f;
		Size2F scale = Size2F(1, 1);
		Point2F position = Point2F();
	};
	class Sprite {
	public:
		RectF rect = RectF();
		Point2F position = Point2F();
		float rotation = 0.f;
		float opacity = 1.f;
		Size2F scale = Size2F(1, 1);
		Size2D spriteIndex = Size2D();

	private:
		SpriteSheet* pSpriteSheet;

		friend class Camera;
		friend class _ResourceManager;
	};

	struct TextFormatProperties {
		const char* fontName = "";
		float fontSize = 12.f;
		FONT_WEIGHTS fontWeight = FONT_WEIGHTS::NORMAL;
		FONT_STYLES fontStyle = FONT_STYLES::NORMAL;
		FONT_STRETCHES fontStretch = FONT_STRETCHES::NORMAL;
		TEXT_HORIZONTAL_ALIGNMENT hAlignment = TEXT_HORIZONTAL_ALIGNMENT::LEFT;
		TEXT_VERTICAL_ALIGNMENT vAlignment = TEXT_VERTICAL_ALIGNMENT::TOP;
		TEXT_WRAPPING wordWrapping = TEXT_WRAPPING::WRAP;
		float lineSpacing = 1.f;
	};
	class TextFormat {
	public:
	private:
		IDWriteTextFormat* pTextFormat;
		friend class Camera;
		friend class _ResourceManager;
	};

	struct TileMapProperties {};
	class TileMap {};

	struct EffectProperties {};
	class Effect {};

	enum class CAMERA_REFERENCE {
		CENTER,
		TOP_LEFT,
		TOP_RIGHT,
		BOTTOM_LEFT,
		BOTTOM_RIGHT,

	};
	struct CameraProperties {
		CAMERA_REFERENCE referencePoint;
	};
	class Camera {
		/*TODO:
		* DrawShape(Arc) - requires path geometry
		* DrawShape(Geometry)
		* FillShape(Geometry)
		* implement DrawShape(Bezier) - requires path geometry
		* implement DrawTileMap
		*/
	public:
		Point2F position = Point2F();
		float rotation = 0;
		Size2F scale = Size2F(1, 1);

		// drawing commands	
		void Fill(Color color);
		void Fill(Brush brush);
		void FillShape(Rect rect, Brush brush);
		void FillShape(RectF rect, Brush brush);
		void FillShape(RoundedRect roundedRect, Brush brush);
		void FillShape(Ellipse ellipse, Brush brush);
		void DrawShape(Rect rect, Brush brush);
		void DrawShape(RectF rect, Brush brush);
		void DrawShape(RoundedRect roundedRect, Brush brush);
		void DrawShape(Ellipse ellipse, Brush brush);
		void DrawShape(Line line, Brush brush);
		void DrawShape(Bezier bezier, Brush brush);
		void DrawBitmap(Bitmap bitmap, RectF destRect, float opacity, RectF sourceRect);
		void DrawBitmap(Bitmap bitmap, RectF destRect, float opacity = 1.f);
		void DrawSprite(Sprite sprite);
		void DrawTileMap(TileMap tileMap);
#ifdef DrawText
#undef DrawText
#endif
		void DrawText(const char* text, RectF textBox, TextFormat textFormat, Brush brush, TEXT_CLIPPING clipOptions = TEXT_CLIPPING::NO_CLIP);

		// read only
		RectF GetWindowRect();
		Size2F GetWindowSize();

	private:
		ID2D1HwndRenderTarget** ppRT = nullptr;
		ID2D1Layer* pLayer = nullptr;
		ID2D1Layer** ppCurrentLayer = nullptr;
		D2D1_LAYER_PARAMETERS layerParams = D2D1::LayerParameters(); // for future geomety masks

		CAMERA_REFERENCE refPoint;

		void SafePushLayer();

		friend class _ResourceManager;
		friend class Window;
		friend class _WindowManager;
	};

	using AnimationID = size_t;
}

#pragma endregion

#pragma region Backend Graphics
namespace oppo {
#ifdef CreateWindow
#undef CreateWindow
#endif // CreateWindow
	class _AnimationBase {
	public:
		AnimationID id = 0;
		int size = 0;
		int i = 0;
		int loop = 0;
		bool isPaused = false;
		virtual void NextFrame() {}
		virtual ~_AnimationBase() {}
		std::function<void()> callback = nullptr;
	};

	template <typename T>
	class _Animation : public _AnimationBase {
	public:
		T* source;
		std::vector<T> values;
		void NextFrame() {
			*source = values[i++];
		}
	};

	class _AnimationManager {
	public:
		template <typename T>
		AnimationID AddAnimation(T* source, std::initializer_list<T> values, int loop, std::function<void()> callback) {
			std::unique_ptr<_Animation<T>> a = std::make_unique<_Animation<T>>();
			a->source = source;
			a->values = values;
			a->size = values.size();
			a->callback = callback;
			a->loop = loop;
			AnimationID id = nextID++;
			if (id == 0) id = nextID++; // in case 18 quintillion animations have been used
			a->id = id;
			animations.push_back(std::move(a));
			return id;
		}

		Result RemoveAnimation(AnimationID& id);
		Result PauseAnimation(AnimationID id);
		Result ResumeAnimation(AnimationID id);
		Result ResetAnimation(AnimationID id);
		bool AnimationExists(AnimationID id);

	private:
		std::vector<std::unique_ptr<_AnimationBase>> animations;
		unsigned long long nextID = 1;

		void NextFrame();

		friend class _WindowManager;
		friend class Window;
	};
	class _ResourceManager {
		/*TODO:
		* CreateTileMap()
		* CreateGeometry()
		*/
	public:
		void Init(Window* wm);
		HRESULT CreateWindowResources(HWND hWnd);
		void DestroyWindowResources();
		HRESULT RecreateDDResources(HWND hWnd);

		HRESULT CreateBrush(Brush* pBrush, BrushProperties properties = BrushProperties());
		HRESULT CreateBitmap(Bitmap* pBitmap, BitmapProperties properties);
		HRESULT CreateSpriteSheet(SpriteSheet* pSpriteSheet, SpriteSheetProperties properties); // filename, sprite size, sprite count, padding
		HRESULT CreateSprite(Sprite* pSprite, SpriteProperties properties); // sprite, spritesheet, sprite draw rectangle, sprite index
		HRESULT CreateTextFormat(TextFormat* pTextFormat, TextFormatProperties properties);
		HRESULT CreateTileMap(TileMap* pTileMap, TileMapProperties properties);
		HRESULT CreateEffect(Effect* pEffect, EffectProperties properties);
		HRESULT CreateCamera(Camera* pCamera, CameraProperties properties);

		void DestroyBrush(Brush* pBrush);
		void DestroyBitmap(Bitmap* pBitmap);
		void DestroySpriteSheet(SpriteSheet* pSpriteSheet);
		void DestroySprite(Sprite* pSprite);
		void DestroyTextFormat(TextFormat* pTextFormat);
		void DestroyCamera(Camera* pCamera);

		~_ResourceManager() {
			DestroyWindowResources();
		}

	private:
		ID2D1HwndRenderTarget* pRT;
		ID2D1Factory* pFactory;
		IWICImagingFactory* pFactoryWIC;
		IDWriteFactory* pFactoryWrite;


		ID2D1Layer* currentLayer = nullptr;

		std::vector<Brush*> brushes;
		std::vector<Bitmap*> bitmaps;
		std::vector<SpriteSheet*> spriteSheets;
		std::vector<TextFormat*> textFormats;
		std::vector<TileMap*> tileMaps;
		std::vector<Camera*> cameras;

		HRESULT CreateDIResources();
		HRESULT CreateDDResources(HWND hWnd);
		void DestroyDIResources();
		void DestroyDDResources();
		HRESULT LoadBitmapFromFile(const char* fileName, ID2D1Bitmap** ppBitmap);
		HRESULT LoadBitmapFromResource(PCWSTR resource, ID2D1Bitmap** ppBitmap);
	};

	enum WNDSTYLE {
		DEFAULT = WS_OVERLAPPEDWINDOW, // Default window with a title bar and resize and close buttons
		BORDER = WS_BORDER, // Window has a title bar but no resize or close buttons
		DISABLED = WS_DISABLED, // Window doesn't accept user input
		MINIMIZE = WS_MINIMIZE, // Window is initially minimized
		MAXIMIZE = WS_MAXIMIZE, // Window is initially maximized
		MAXBOX = WS_MAXIMIZEBOX | WS_SYSMENU | WS_CAPTION, // Window maximize box and tiling are enabled
		MINBOX = WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION, // Window minimize box and minimizing are enabled
		OVERLAPPED = WS_OVERLAPPEDWINDOW, // Same as DEFAULT
		RESIZE = WS_SIZEBOX, // Window can be resized
		POPUP
	};
	struct WindowProperties {
		const char* name = ""; // window name
		int id = 0; // user-specified id included in events to identify window
		int style = WNDSTYLE::DEFAULT; // window style
		std::function<Result(Event)> GameLoop = nullptr; // game loop function
		Size2D size = Size2D(); // starting size of the window in pixels
		Size2D minSize = Size2D(); // minimum allowable window size in pixels, 0 for default
		Size2D maxSize = Size2D(); // maximum allowable window size in pixels, 0 for default
		float fps = 60; // frames per second (render speed)
		float ups = 100; // updates per second
		float aps = 30; // animations per second
		Color backgroundColor = Color(); // default background color
	};
	class Window {
	public:
		// create window-dependent resources
		Result CreateBrush(Brush* pBrush, BrushProperties properties);
		Result CreateBitmap(Bitmap* pBitmap, BitmapProperties properties);
		Result CreateSpriteSheet(SpriteSheet* pSpriteSheet, SpriteSheetProperties properties);
		Result CreateSprite(Sprite* pSprite, SpriteProperties properties);
		Result CreateTextFormat(TextFormat* pTextFormat, TextFormatProperties properties);
		Result CreateTileMap(TileMap* pTileMap, TileMapProperties properties);
		Result CreateEffect(Effect* pEffect, EffectProperties properties);
		Result CreateCamera(Camera* pCamera, CameraProperties properties);

		// destroy window-dependent resources
		void DestroyBrush(Brush* pBrush);
		void DestroyBitmap(Bitmap* pBitmap);
		void DestroySpriteSheet(SpriteSheet* pSpriteSheet);
		void DestroySprite(Sprite* pSprite);
		void DestroyTextFormat(TextFormat* pTextFormat);
		void DestroyTileMap(TileMap* pTileMap);
		void DestroyEffect(Effect* pEffect);
		void DestroyCamera(Camera* pCamera);

		// animations
		template <typename T>
		AnimationID AddAnimation(T* source, std::initializer_list<T> values, int loop, std::function<void()> callback) {
			animationManager.AddAnimation(source, values, loop, callback);
		}
		Result RemoveAnimation(AnimationID& id);
		Result PauseAnimation(AnimationID id);
		Result ResumeAnimation(AnimationID id);
		Result ResetAnimation(AnimationID id);
		bool AnimationExists(AnimationID id);

		// set window properties
		void SetFPS(float fps);
		void SetUPS(float ups);
		void SetAPS(float aps);
		void SetSize(Size2D size);
		void SetPosition(Point2D position);
		void SetMousePosition(Point2D position);
		void SetMouseCapture(bool isCaptured);
		void SetMaximize(bool isMaximized);
		void SetMinimize(bool isMinimized);
		void SetFullscreen(bool isFullscreen);

		void SetScene(std::function<Result(Event)> newScene);

		// get window properties
		Size2D GetSize();
		Point2D GetPosition();

	private:
		_ResourceManager resourceManager;
		_AnimationManager animationManager;

		HWND hWnd;

		// fullscreen
		WINDOWPLACEMENT wpPrev = { sizeof(wpPrev) };
		DWORD dwStylePrev;
		bool isFullscreen = false;

		ID2D1HwndRenderTarget** ppRT;
		ID2D1Layer** ppCurrentLayer;
		TRACKMOUSEEVENT tme;
		wchar_t className[17];
		Color backgroundColor;
		Size2D szMin; // Minimum screen size, 0 for no min
		Size2D szMax; // Maximum screen size, 0 for no max
		float aspectRatio = 0; // fixed aspect ratio, 0 for unfixed
		float fps = OP_DEFAULTFPS;
		float ups = OP_DEFAULTUPS;
		int winID = 0;


		Stopwatch<std::chrono::microseconds> swRender; // controls framerate
		Stopwatch<std::chrono::microseconds> swUpdate; // controls update messages
		Stopwatch<std::chrono::microseconds> swAnimate; // controls animation frames
		unsigned long long renderCountTarget = -1;
		unsigned long long updateCountTarget = -1;
		unsigned long long animateCountTarget = -1;
		std::thread tGameLoopTimer;

		enum class WNDSTATE {
			NONE,
			CREATE,
			RUN,
			DESTROY
		};

		WNDSTATE wndState = WNDSTATE::NONE;
		void SetWndStateCreate() {
			wndState = WNDSTATE::CREATE;
		}

		std::function<Result(Event)> GameLoop = nullptr;

		void NewClassName();
		void GameLoopTimer();
		KEYS TranslateKeystroke(int vkCode);
		LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		friend class _ResourceManager;
		friend class _Engine;
	};

	class _Engine {
	public:
		_Engine(const _Engine& obj) = delete;
		static Result AddWindow(Window* wnd, WindowProperties properties);
		static void RemoveWindow(Window* wnd);
		static void Terminate();

		static void Run();

	private:
		static std::vector<HWND> windows;
		_Engine();
	};

	Result CreateWindow(Window* wnd, WindowProperties properties);

	void Run();

	void Terminate();
}
#pragma endregion
