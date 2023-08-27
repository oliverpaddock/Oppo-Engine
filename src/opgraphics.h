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
		void DrawBitmap(Bitmap bitmap, RectF destRect, float opacity, RectF sourceRect);
		void DrawBitmap(Bitmap bitmap, RectF destRect, float opacity = 1.f);
		void DrawSprite(Sprite);
		void DrawTileMap(TileMap);

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

		void SafePushLayer(D2D1_MATRIX_3X2_F preTransform = D2D1::IdentityMatrix(), D2D1_MATRIX_3X2_F postTransform = D2D1::IdentityMatrix());

		friend class ResourceManager;
		friend class WindowManager;
	};
	
	struct AnimationFrame {
		Size2D spriteIndex = Size2D(); // sprite to display
		Vector2F dPosition = Vector2F(); // change in position
		float dRot = 0; // change in rotation
		Size2F dScale = Size2F(); // change in scale, {0, 0} for no change
	};

	class Animation {
	public:
		std::vector<AnimationFrame> frames;
		int repeatCount = 1; // repeatCount < 0 = infinite loop

		void NewFrames(int); // create empty frames
		void AddFrames(int);

		Result FrameFromVector(std::vector<AnimationFrame>);
		Result IndexFromVector(std::vector<Size2D>);
		Result PosFromVector(std::vector<Vector2F>);
		Result RotFromVector(std::vector<float>);
		Result ScaleFromVector(std::vector<Size2F>);

		Result IndexConstValue(Size2D);
		Result PosConstValue(Vector2F);
		Result RotConstValue(float);
		Result ScaleConstValue(Size2F);

		Result IndexLinValue(Size2D, Size2D);
		Result PosLinValue(Vector2F, Vector2F);
		Result RotLinValue(float, float);
		Result ScaleLinValue(Size2F, Size2F);
	};

	class AnimationManager {};

	class ResourceManager {
		/*TODO:
		* CreateTextFormat()
		* CreateTileMap()
		*/
	public:
		void Init(WindowManager*);
		HRESULT CreateWindowResources(HWND);
		void DestroyWindowResources();
		HRESULT RecreateDDResources(HWND);

		HRESULT CreateBrush(Brush*, Color); // brush
		HRESULT CreateBitmap(const char*, Bitmap*); // filename, bitmap
		HRESULT CreateBitmapFromResource(PCWSTR, Bitmap*);
		HRESULT CreateSpriteSheet(const char*, Size2D, Size2D, Rect, SpriteSheet*); // filename, sprite size, sprite count, padding
		HRESULT CreateSpriteSheetFromResource(PCWSTR, Size2D, Size2D, Rect, SpriteSheet*);
		HRESULT CreateSprite(Sprite*, SpriteSheet*, RectF, Size2D); // sprite, spritesheet, sprite draw rectangle, sprite index
		HRESULT CreateCamera(Camera*);

		HRESULT CreateTextFormat(TextFormat*, TextFormatProperties);

		void DestroyBrush(Brush*);
		void DestroyBitmap(Bitmap*);
		void DestroySpriteSheet(SpriteSheet*);
		void DestroySprite(Sprite*);
		void DestroyCamera(Camera*);

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
		HRESULT CreateDDResources(HWND);
		void DestroyDIResources();
		void DestroyDDResources();
		HRESULT LoadBitmapFromFile(const char*, ID2D1Bitmap**);
		HRESULT LoadBitmapFromResource(PCWSTR hResource, ID2D1Bitmap** pBitmap);
	};

	class WindowManager {
	public:
		Result Init(WindowPackage);
		Result Run();
		void RegisterGameLoop(Result(*Func)(Event));
		void SetFPS(float);
		void SetUPS(float);
		void SetAPS(float);

		Result CreateBrush(Brush*); // brush
		Result CreateBitmap(const char*, Bitmap*); // filename, bitmap
		Result CreateBitmapFromResource(PCWSTR, Bitmap*);
		Result CreateSpriteSheet(const char*, Size2D, Size2D, Rect, SpriteSheet*); // filename, sprite size, sprite count, padding
		Result CreateSpriteSheetFromResource(PCWSTR, Size2D, Size2D, Rect, SpriteSheet*);
		Result CreateSprite(Sprite*, SpriteSheet*, RectF, Size2D); // sprite, spritesheet, sprite draw rectangle, sprite index
		Result CreateCamera(Camera*);
		Result CreateTextFormat(TextFormat*, TextFormatProperties);

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

		using Callback = Result(*)(Event);
		Callback GameLoop = nullptr;

		LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
		void NewClassName();
		void GameLoopTimer();
		KEYS TranslateKeystroke(int);

		friend class ResourceManager;
	};
}