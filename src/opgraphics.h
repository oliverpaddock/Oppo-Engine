#pragma once
#include <Windows.h>
#include <windowsx.h>
#include <d2d1_3.h>
#include <d2d1helper.h>
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
		Color _color = Color();
		ID2D1SolidColorBrush* _pBrush = nullptr;

		friend class Camera;
		friend class _ResourceManager;
	};

	struct BitmapProperties {
		const char* fileName;
	};
	class Bitmap {
	protected:
		std::string _fileName;
		ID2D1Bitmap* _pBitmap;

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
		Size2D GetSpriteSize();
		Size2D GetSpriteCount();
		Rect   GetPadding();

	private:
		Size2D _spriteSize;
		Size2D _spriteCount;
		Rect _padding;

		D2D1_RECT_F GetSpriteRect(Size2D spriteIndex);

		friend class Camera;
		friend class _ResourceManager;
	};

	struct SpriteProperties {
		SpriteSheet* pSpriteSheet;
		RectF spriteRect;
		Size2D spriteIndex;
	};
	class Sprite {
	public:
		RectF rect = RectF();
		Size2D spriteIndex = Size2D();

	private:
		SpriteSheet* _pSpriteSheet;

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
		IDWriteTextFormat* _pTextFormat;
		friend class Camera;
		friend class _ResourceManager;
	};

	struct TileMapProperties {};
	class TileMap {};

	struct EffectProperties {};
	class Effect {};
	
	/// <summary>
	/// Reference position on the window for camera position.
	/// </summary>
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
		
		/// <summary>
		/// Fill the screen with the specified color.
		/// </summary>
		void Fill(Color color);
		/// <summary>
		/// Fill the screen with the color of the brush.
		/// </summary>
		void Fill(Brush brush);
		/// <summary>
		/// Draw a filled shape.
		/// </summary>
		void FillShape(Rect rect, Brush brush, Transform tf = Transform::Identity());
		/// <summary>
		/// Draw a filled shape.
		/// </summary>
		void FillShape(RectF rect, Brush brush, Transform tf = Transform::Identity());
		/// <summary>
		/// Draw a filled shape.
		/// </summary>
		void FillShape(RoundedRect roundedRect, Brush brush, Transform tf = Transform::Identity());
		/// <summary>
		/// Draw a filled shape.
		/// </summary>
		void FillShape(Ellipse ellipse, Brush brush, Transform tf = Transform::Identity());
		/// <summary>
		/// Draw an outlined shape.
		/// </summary>
		void DrawShape(Rect rect, Brush brush, Transform tf = Transform::Identity());
		/// <summary>
		/// Draw an outlined shape.
		/// </summary>
		void DrawShape(RectF rect, Brush brush, Transform tf = Transform::Identity());
		/// <summary>
		/// Draw an outlined shape.
		/// </summary>
		void DrawShape(RoundedRect roundedRect, Brush brush, Transform tf = Transform::Identity());
		/// <summary>
		/// Draw an outlined shape.
		/// </summary>
		void DrawShape(Ellipse ellipse, Brush brush, Transform tf = Transform::Identity());
		/// <summary>
		/// Draw an outlined shape.
		/// </summary>
		void DrawShape(Line line, Brush brush, Transform tf = Transform::Identity());
		/// <summary>
		/// Draw an outlined shape.
		/// </summary>
		void DrawShape(Bezier bezier, Brush brush, Transform tf = Transform::Identity());
		/// <summary>
		/// Draw a bitmap to the specified rectangle.
		/// </summary>
		void DrawBitmap(Bitmap bitmap, RectF destRect, float opacity, RectF sourceRect, Transform tf = Transform::Identity());
		/// <summary>
		/// Draw a bitmap to the specified rectangle.
		/// </summary>
		void DrawBitmap(Bitmap bitmap, RectF destRect, float opacity = 1.f, Transform tf = Transform::Identity());
		/// <summary>
		/// Draw a sprite.
		/// </summary>
		void DrawSprite(Sprite sprite, float opacity = 1.f, Transform tf = Transform::Identity());
		/// <summary>
		/// Draw a tile map
		/// </summary>
		void DrawTileMap(TileMap tileMap, Transform tf = Transform::Identity());
#ifdef DrawText
#undef DrawText
#endif
		/// <summary>
		/// Draw text to the screen.
		/// </summary>
		void DrawText(const char* text, RectF textBox, TextFormat textFormat, Brush brush, TEXT_CLIPPING clipOptions = TEXT_CLIPPING::NO_CLIP, Transform tf = Transform::Identity());

		/// <summary>
		/// Set the camera zoom to fit the specified width in pixels on the screen. 
		/// Does not account for camera rotation.
		/// </summary>
		void ZoomToWidth(float width);
		/// <summary>
		/// Set the camera zoom to fit the specified height in pixels on the screen. 
		/// Does not account for camera rotation.
		/// </summary>
		void ZoomToHeight(float height);
		/// <summary>
		/// Set the camera zoom to fit within the specified size in pixels on the screen.
		/// Does not account for camera rotation.
		/// </summary>
		void ZoomToFit(Size2F size);
		/// <summary>
		/// Set the camera zoom to fill the specified size in pixels on the screen.
		/// Does not account for camera rotation.
		/// </summary>
		void ZoomToFill(Size2F size);

		/// <summary>
		/// Get the world coordinates of the specified window coordinates.
		/// </summary>
		Point2F ScreenToWorld(Point2D point);
		/// <summary>
		/// Get the window coordinates of the specified world coordinates.
		/// </summary>
		Point2D WorldToScreen(Point2F point);

		/// <summary>
		/// Returns a rectangle specifying the viewport of the camera. 
		/// If the camera is rotated, the rectangle is the minimum size required to fit all of the view in frame.
		/// </summary>
		RectF GetRect();
		/// <summary>
		/// Returns the width and height of the viewport of the camera.
		/// If the camera is rotated, returns the width and height of the rectangle required to fit all of the view in frame.
		/// </summary>
		Size2F GetSize();

	private:
		ID2D1HwndRenderTarget** _ppRT = nullptr;
		ID2D1Layer* _pLayer = nullptr;
		ID2D1Layer** _ppCurrentLayer = nullptr;
		D2D1_LAYER_PARAMETERS _layerParams = D2D1::LayerParameters(); // for future geomety masks
		CAMERA_REFERENCE _refPoint;

		/// <summary>
		/// Helper function to manage render target layers and transformations
		/// </summary>
		void _SafePushLayer();

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
		int iFrame = 0;
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
			*source = values[iFrame++];
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
			AnimationID id = _nextID++;
			if (id == 0) id = _nextID++; // in case 18 quintillion animations have been used
			a->id = id;
			_animations.push_back(std::move(a));
			return id;
		}

		Result RemoveAnimation(AnimationID& id);
		Result PauseAnimation(AnimationID id);
		Result ResumeAnimation(AnimationID id);
		Result ResetAnimation(AnimationID id);
		bool AnimationExists(AnimationID id);

	private:
		std::vector<std::unique_ptr<_AnimationBase>> _animations;
		unsigned long long _nextID = 1;

		void _NextFrame();

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
			return _animationManager.AddAnimation(source, values, loop, callback);
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
		_ResourceManager _resourceManager;
		_AnimationManager _animationManager;

		HWND _hWnd;

		// fullscreen
		WINDOWPLACEMENT _wpPrev = { sizeof(_wpPrev) };
		DWORD _dwStylePrev;
		bool _isFullscreen = false;

		ID2D1HwndRenderTarget** _ppRT;
		ID2D1Layer** _ppCurrentLayer;
		TRACKMOUSEEVENT _tme;
		wchar_t _className[17];
		Color _backgroundColor;
		Size2D _szMin; // Minimum screen size, 0 for no min
		Size2D _szMax; // Maximum screen size, 0 for no max
		float _aspectRatio = 0; // fixed aspect ratio, 0 for unfixed
		float _fps = OP_DEFAULTFPS;
		float _ups = OP_DEFAULTUPS;
		int _winID = 0;


		Stopwatch<std::chrono::microseconds> _swRender; // controls framerate
		Stopwatch<std::chrono::microseconds> _swUpdate; // controls update messages
		Stopwatch<std::chrono::microseconds> _swAnimate; // controls animation frames
		unsigned long long _renderCountTarget = -1;
		unsigned long long _updateCountTarget = -1;
		unsigned long long _animateCountTarget = -1;
		std::thread _tGameLoopTimer;

		enum class WNDSTATE {
			NONE,
			CREATE,
			RUN,
			DESTROY
		};

		WNDSTATE _wndState = WNDSTATE::NONE;

		std::function<Result(Event)> _GameLoop = nullptr;

		void NewClassName();
		HWND Create(WindowProperties properties);
		void GameLoopTimer();
		KEYS TranslateKeystroke(int vkCode);
		LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		friend class _ResourceManager;
		friend class _Engine;
	};

	class _Engine {
	public:
		_Engine(const _Engine& obj) = delete;
		Result AddWindow(Window* wnd, WindowProperties properties);
		void RemoveWindow(Window* wnd);
		void Terminate();

		void Run();
		static _Engine* GetInstance();
	private:
		static _Engine* _pInstance;
		std::vector<HWND> _windows;
		_Engine() {};
	};

	Result CreateWindow(Window* wnd, WindowProperties properties);

	void Run();

	void Terminate();
}
#pragma endregion
