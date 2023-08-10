#pragma once
#include <Windows.h>
#include <windowsx.h>
#include <d2d1.h>
#pragma comment(lib, "d2d1.lib")
#include <wincodec.h>

#include <memory>
#include <string>
#include <vector>
#include <codecvt>
#include <thread>
#include <random>

#include "optypes.h"
#include "timer.h"

#define OP_DEFAULTFPS 60
#define OP_DEFAULTUPS 100
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
}

namespace oppo {
	/*TODO:
	* WindowManager
	* TextLayout
	* TileMap
	*/
	struct WindowPackage {
		Size2D szScreen; // Screen size on startup, 0 for default
		Size2D szMin; // Minimum screen size, 0 for no min
		Size2D szMax; // Maximum screen size, 0 for no max
		float aspectRatio = 0; // fixed aspect ratio, 0 for unfixed
		float fps = OP_DEFAULTFPS; // Frames per second
		float ups = OP_DEFAULTUPS; // Updates per second
		Color backgroundColor = Color(); // Default background color
		const char* windowName = ""; // Window name
		int windowID = 0; // sent with events to tell multiple windows apart
	};

	class ResourceManager;
	class WindowManager;

	class Brush;
	class Bitmap;
	class Spritesheet;
	class Sprite;
	class TextLayout;
	class TileMap;
	class Camera;

	class Brush {
	public:
		void SetColor(Color);
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
		ID2D1Bitmap* pBitmap;

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
		float opacity = 0;
		Size2F scale = Size2F(1, 1);
		Size2D spriteIndex = Size2D();

	private:
		SpriteSheet* pSpriteSheet;

		friend class Camera;
		friend class ResourceManager;
	};

	class TextLayout {};

	class TileMap {};

	class Camera {
	/*TODO:
	* DrawShape(Arc) - requires path geometry
	* DrawShape(Geometry)
	* FillShape(Geometry)
	* implement DrawShape(Bezier) - requires path geometry
	* implement DrawTileMap
	* implement DrawText
	*/
	public:
		Point2F position = Point2F();
		float rotation = 0;
		Size2F scale = Size2F(1, 1);

		// drawing commands	
		void Fill(Color);
		void Fill(Brush);
		void FillShape(Rect, Brush);
		void FillShape(RectF, Brush);
		void FillShape(RoundedRect, Brush);
		void FillShape(Ellipse, Brush);
		void DrawShape(Rect, Brush);
		void DrawShape(RectF, Brush);
		void DrawShape(RoundedRect, Brush);
		void DrawShape(Ellipse, Brush);
		void DrawShape(Line, Brush);
		void DrawShape(Bezier, Brush);
		void DrawBitmap(Bitmap, RectF, float, RectF);
		void DrawBitmap(Bitmap, RectF, float);
		void DrawSprite(Sprite);
		void DrawTileMap(TileMap);

#ifdef DrawText
#undef DrawText
#endif
		void DrawText(const char*, Point2F, TextLayout);
		void DrawText(const char*, TextLayout);

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

	class ResourceManager {
		/*TODO:
		* CreateTextLayout()
		* CreateTileMap()
		*/
	public:
		void Init(WindowManager*, HWND);
		HRESULT CreateWindowResources();
		void DestroyWindowResources();
		HRESULT RecreateDDResources();

		HRESULT CreateBrush(Brush*, Color); // brush
		HRESULT CreateBitmap(const char*, Bitmap*); // filename, bitmap
		HRESULT CreateSpriteSheet(const char*, Size2D, Size2D, Rect, SpriteSheet*); // filename, sprite size, sprite count, padding
		HRESULT CreateSprite(Sprite*, SpriteSheet*, RectF, Size2D); // sprite, spritesheet, sprite draw rectangle, sprite index
		HRESULT CreateCamera(Camera*);

		void DestroyBrush(Brush*);
		void DestroyBitmap(Bitmap*);
		void DestroySpriteSheet(SpriteSheet*);
		void DestroySprite(Sprite*);
		void DestroyCamera(Camera*);

	private:
		HWND hWnd;
		ID2D1HwndRenderTarget* pRT;
		ID2D1Factory* pFactory;
		IWICImagingFactory* pFactoryWIC;
		
		ID2D1Layer* currentLayer = nullptr;

		std::vector<Brush*> brushes;
		std::vector<Bitmap*> bitmaps;
		std::vector<SpriteSheet*> spriteSheets;
		std::vector<TextLayout*> textLayouts;
		std::vector<TileMap*> tileMaps;
		std::vector<Camera*> cameras;
		
		HRESULT CreateDIResources();
		HRESULT CreateDDResources();
		void DestroyDIResources();
		void DestroyDDResources();
		HRESULT LoadBitmapFromFile(const char*, ID2D1Bitmap**);
	};

	class WindowManager {
	public:
		Result Init(WindowPackage);
		Result Run();
		void RegisterGameLoop(Result(*)(Event));
		void SetFPS(float);
		void SetUPS(float);

		Result CreateBrush(Brush*); // brush
		Result CreateBitmap(const char*, Bitmap*); // filename, bitmap
		Result CreateSpriteSheet(const char*, Size2D, Size2D, Rect, SpriteSheet*); // filename, sprite size, sprite count, padding
		Result CreateSprite(Sprite*, SpriteSheet*, RectF, Size2D); // sprite, spritesheet, sprite draw rectangle, sprite index
		Result CreateCamera(Camera*);

		void DestroyBrush(Brush*);
		void DestroyBitmap(Bitmap*);
		void DestroySpriteSheet(SpriteSheet*);
		void DestroySprite(Sprite*);
		void DestroyCamera(Camera*);


	private:
		ResourceManager resourceManager;

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
		unsigned long long renderCountTarget = -1;
		unsigned long long updateCountTarget = -1;
		std::thread tGameLoopTimer;

		enum class WNDSTATE {
			NONE,
			CREATE,
			RUN,
			DESTROY
		};

		WNDSTATE wndState = WNDSTATE::NONE;

		using Callback = Result(*)(Event);
		Callback GameLoop = nullptr;

		LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
		void NewClassName();
		void GameLoopTimer();
		KEYS TranslateKeystroke(int);

		friend class ResourceManager;
	};
}