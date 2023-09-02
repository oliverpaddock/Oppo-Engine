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

namespace oppo {
	/*TODO:
	* TextFormat
	* TileMap
	*/
	struct WindowPackage {
		Size2D szScreen = Size2D(); // Screen size on startup, 0 for default
		Size2D szMin = Size2D(); // Minimum screen size, 0 for no min
		Size2D szMax = Size2D(); // Maximum screen size, 0 for no max
		float aspectRatio = 0; // fixed aspect ratio, 0 for unfixed
		float fps = OP_DEFAULTFPS; // Frames per second
		float ups = OP_DEFAULTUPS; // Updates per second
		float aps = OP_DEFAULTAPS; // Animations per second
		Color backgroundColor = Color(); // Default background color
		const char* windowName = ""; // Window name
		int windowID = 0; // sent with events to tell multiple windows apart
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
		friend class ResourceManager;
	};

	class Bitmap {
	protected:
		std::string fileName;
		PCWSTR resource;
		ID2D1Bitmap* pBitmap;
		bool fromResource = false;

		friend class Camera;
		friend class ResourceManager;
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
		friend class ResourceManager;
	};

	class Sprite {
	public:
		RectF rect = RectF();
		Point2F position = Point2F();
		float rotation = 0;
		float opacity = 1;
		Size2F scale = Size2F(1, 1);
		Size2D spriteIndex = Size2D();

	private:
		SpriteSheet* pSpriteSheet;

		friend class Camera;
		friend class ResourceManager;
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

		// box clipping, line spacing
	};

	class TextFormat {
	public:
	private:
		IDWriteTextFormat* pTextFormat;
		friend class Camera;
		friend class ResourceManager;
	};

	class TileMap {};

	class Effect : utility::_BasicNode {};

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

		void SafePushLayer();

		friend class ResourceManager;
		friend class WindowManager;
	};

	using AnimationID = size_t;

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


	class AnimationManager {
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

		bool AnimationExists(AnimationID id);

		Result PauseAnimation(AnimationID id);

		Result ResumeAnimation(AnimationID id);

	private:
		std::vector<std::unique_ptr<_AnimationBase>> animations;
		unsigned long long nextID = 1;
		void NextFrame();

		friend class WindowManager;
	};

	class ResourceManager {
		/*TODO:
		* CreateTileMap()
		* CreateGeometry()
		*/
	public:
		void Init(WindowManager* wm);
		HRESULT CreateWindowResources(HWND hWnd);
		void DestroyWindowResources();
		HRESULT RecreateDDResources(HWND hWnd);

		HRESULT CreateBrush(Brush* pBrush, Color color = Color()); // brush
		HRESULT CreateBitmap(const char* fileName, Bitmap* pBitmap); // filename, bitmap
		HRESULT CreateBitmapFromResource(PCWSTR resource, Bitmap* pBitmap);
		HRESULT CreateSpriteSheet(const char* fileName, Size2D spriteSize, Size2D spriteCount, Rect padding, SpriteSheet* pSpriteSheet); // filename, sprite size, sprite count, padding
		HRESULT CreateSpriteSheetFromResource(PCWSTR resource, Size2D spriteSize, Size2D spriteCount, Rect padding, SpriteSheet* pSpriteSheet);
		HRESULT CreateSprite(Sprite* pSprite, SpriteSheet* pSpriteSheet, RectF spriteRect, Size2D spriteIndex = Size2D()); // sprite, spritesheet, sprite draw rectangle, sprite index
		HRESULT CreateCamera(Camera* pCamera);
		HRESULT CreateTextFormat(TextFormat* pTextFormat, TextFormatProperties properties);

		void DestroyBrush(Brush* pBrush);
		void DestroyBitmap(Bitmap* pBitmap);
		void DestroySpriteSheet(SpriteSheet* pSpriteSheet);
		void DestroySprite(Sprite* pSprite);
		void DestroyCamera(Camera* pCamera);

		~ResourceManager() {
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

	class WindowManager {
	public:
		Result Init(WindowPackage wp);
		Result Run();
		void RegisterGameLoop(std::function<Result(Event)> GameLoopFunc);
		void SetFPS(float fps);
		void SetUPS(float ups);
		void SetAPS(float aps);

		Result CreateBrush(Brush* pBrush); // brush
		Result CreateBitmap(const char* fileName, Bitmap* pBitmap); // filename, bitmap
		Result CreateBitmapFromResource(PCWSTR resource, Bitmap* pBitmap);
		Result CreateSpriteSheet(const char* fileName, Size2D spriteSize, Size2D spriteCount, Rect padding, SpriteSheet* pSpriteSheet); // filename, sprite size, sprite count, padding
		Result CreateSpriteSheetFromResource(PCWSTR resource, Size2D spriteSize, Size2D spriteCount, Rect padding, SpriteSheet* pSpriteSheet);
		Result CreateSprite(Sprite* pSprite, SpriteSheet* pSpriteSheet, RectF spriteRect, Size2D spriteIndex = Size2D()); // sprite, spritesheet, sprite draw rectangle, sprite index
		Result CreateCamera(Camera* pCamera);
		Result CreateTextFormat(TextFormat* pTextFormat, TextFormatProperties properties);

		void DestroyBrush(Brush* pBrush);
		void DestroyBitmap(Bitmap* pBitmap);
		void DestroySpriteSheet(SpriteSheet* pSpriteSheet);
		void DestroySprite(Sprite* pSprite);
		void DestroyCamera(Camera* pCamera);

		template <typename T>
		AnimationID AddAnimation(T* source, std::initializer_list<T> values, int loop = 1, std::function<void()> callback = nullptr) {
			return animationManager.AddAnimation(source, values, loop, callback);
		}
		Result RemoveAnimation(AnimationID& id);
		Result PauseAnimation(AnimationID id);
		Result ResumeAnimation(AnimationID id);
		bool AnimationExists(AnimationID id);

	private:
		ResourceManager resourceManager;
		AnimationManager animationManager;

		HWND hWnd;
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

		std::function<Result(Event)> GameLoop = nullptr;

		LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		void NewClassName();
		void GameLoopTimer();
		KEYS TranslateKeystroke(int vkCode);

		friend class ResourceManager;
	};
}