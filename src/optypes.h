#pragma once
#include <Windows.h>
#include <utility>
#include <d2d1.h>
#include <map>

/*TODO:
* Geometry
* Arc segment
* More error conditions
*/

#pragma region Type Templates
namespace oppo::utility {
	// 2d point
	template <typename T>
	struct _Point2d {
		T x = 0, y = 0;
		_Point2d(T x = 0, T y = 0) {
			this->x = x;
			this->y = y;
		}
	};

	// operator overloads for _Point2d
	template <typename T1, typename T2>
	auto operator+(const _Point2d<T1>& p1, const _Point2d<T2>& p2)
		-> _Point2d<decltype(std::declval<T1>()* std::declval<T2>())> {
		return { p1.x + p2.x, p1.y + p2.y };
	}

	template <typename T1, typename T2>
	auto operator+(const _Point2d<T1>& p, const T2& k)
		-> _Point2d<decltype(std::declval<T1>()* std::declval<T2>())> {
		return { p.x + k, p.y + k };
	}

	template <typename T1, typename T2>
	auto operator+(const T2& k, const _Point2d<T1>& p)
		-> _Point2d<decltype(std::declval<T1>()* std::declval<T2>())> {
		return p + k;
	}

	template <typename T1, typename T2>
	auto operator-(const _Point2d<T1>& p1, const _Point2d<T2>& p2)
		-> _Point2d<decltype(std::declval<T1>()* std::declval<T2>())> {
		return { p1.x - p2.x, p1.y - p2.y };
	}

	template <typename T1, typename T2>
	auto operator*(const _Point2d<T1>& p1, const _Point2d<T2>& p2)
		-> _Point2d<decltype(std::declval<T1>()* std::declval<T2>())> {
		return { p1.x * p2.x, p1.y * p2.y };
	}

	template <typename T1, typename T2>
	auto operator*(const _Point2d<T1>& p, const T2& k)
		-> _Point2d<decltype(std::declval<T1>()* std::declval<T2>())> {
		return { p.x * k, p.y * k };
	}

	template <typename T1, typename T2>
	auto operator*(const T2& k, const _Point2d<T1>& p)
		-> _Point2d<decltype(std::declval<T1>()* std::declval<T2>())> {
		return p * k;
	}

	template <typename T1, typename T2>
	auto operator/(const _Point2d<T1>& p1, const _Point2d<T2>& p2)
		-> _Point2d<decltype(std::declval<T1>()* std::declval<T2>())> {
		return { p1.x / p2.x, p1.y / p2.y };
	}

	template <typename T1, typename T2>
	auto operator/(const _Point2d<T1>& p, const T2& k)
		-> _Point2d<decltype(std::declval<T1>()* std::declval<T2>())> {
		return { p.x / k, p.y / k };
	}

	// 2d size
	template <typename T>
	struct _Size2d {
		T width = 0, height = 0;
		_Size2d(T width = 0, T height = 0) {
			this->width = width;
			this->height = height;
		}
		_Size2d(const D2D1_SIZE_U& size) : width(static_cast<T>(size.width)), height(static_cast<T>(size.height)) {}
		_Size2d(const D2D1_SIZE_F& size) : width(static_cast<T>(size.width)), height(static_cast<T>(size.height)) {}
	};

	// operator overloading for _Size2d
	template <typename T>
	bool operator==(const _Size2d<T>& thisSz, const _Size2d<T>& other) {
		return (thisSz.width == other.width) && (thisSz.height == other.height);
	}

	template <typename T1, typename T2>
	auto operator+(const _Size2d<T1>& p1, const _Size2d<T2>& p2)
		-> _Size2d<decltype(std::declval<T1>()* std::declval<T2>())> {
		return { p1.x + p2.x, p1.y + p2.y };
	}

	template <typename T1, typename T2>
	auto operator+(const _Size2d<T1>& p, const T2& k)
		-> _Size2d<decltype(std::declval<T1>()* std::declval<T2>())> {
		return { p.x + k, p.y + k };
	}

	template <typename T1, typename T2>
	auto operator+(const T2& k, const _Size2d<T1>& p)
		-> _Size2d<decltype(std::declval<T1>()* std::declval<T2>())> {
		return p + k;
	}

	template <typename T1, typename T2>
	auto operator-(const _Size2d<T1>& p1, const _Size2d<T2>& p2)
		-> _Size2d<decltype(std::declval<T1>()* std::declval<T2>())> {
		return { p1.x - p2.x, p1.y - p2.y };
	}

	template <typename T1, typename T2>
	auto operator*(const _Size2d<T1>& p1, const _Size2d<T2>& p2)
		-> _Size2d<decltype(std::declval<T1>()* std::declval<T2>())> {
		return { p1.x * p2.x, p1.y * p2.y };
	}

	template <typename T1, typename T2>
	auto operator*(const _Size2d<T1>& p, const T2& k)
		-> _Size2d<decltype(std::declval<T1>()* std::declval<T2>())> {
		return { p.x * k, p.y * k };
	}

	template <typename T1, typename T2>
	auto operator*(const T2& k, const _Size2d<T1>& p)
		-> _Size2d<decltype(std::declval<T1>()* std::declval<T2>())> {
		return p * k;
	}

	template <typename T1, typename T2>
	auto operator/(const _Size2d<T1>& p1, const _Size2d<T2>& p2)
		-> _Size2d<decltype(std::declval<T1>()* std::declval<T2>())> {
		return { p1.x / p2.x, p1.y / p2.y };
	}

	template <typename T1, typename T2>
	auto operator/(const _Size2d<T1>& p, const T2& k)
		-> _Size2d<decltype(std::declval<T1>()* std::declval<T2>())> {
		return { p.x / k, p.y / k };
	}

	template <typename T>
	struct _Rect {
		T left = 0, top = 0, right = 0, bottom = 0;
		_Rect(T left = 0, T top = 0, T right = 0, T bottom = 0) {
			this->left = left;
			this->top = top;
			this->right = right;
			this->bottom = bottom;
		}
	};

	template <typename T, typename TP>
	struct _Ellipse {
		TP center = { 0, 0 };
		T rx = 0, ry = 0;
		_Ellipse(TP center = TP(), T rx = 0, T ry = 0) {
			this->center = center;
			this->rx = rx;
			this->ry = ry;
		}
	};

	template <typename T, typename TR>
	struct _RoundedRect {
		TR rect = { 0, 0, 0, 0 };
		T rx = 0, ry = 0;
		_RoundedRect(TR rect = TR(), T rx = 0, T ry = 0) {
			this->rect = rect;
			this->rx = rx;
			this->ry = ry;
		}
	};

	template <typename T>
	struct _Line {
		T p0 = { 0, 0 }, p1 = { 0, 0 };
		_Line(T p0 = T(), T p1 = T()) {
			this->p0 = p0;
			this->p1 = p1;
		}
	};

	template <typename T>
	struct _Bezier {
		T p0 = { 0, 0 }, p1 = { 0, 0 }, p2 = { 0, 0 };
		_Bezier(T p0 = T(), T p1 = T(), T p2 = T()) {
			this->p0 = p0;
			this->p1 = p1;
			this->p2 = p2;
		}
	};
}
#pragma endregion

#pragma region Base Types
namespace oppo {
	// errors: Success >=0, Fail < 0
	using Result = long;
	enum ERRORS : Result {
		SUCCESS = 0,
		FAIL = -1,
		WARNING = 1 // warning is a success, but might indicate unexpected behavior
	};
	inline bool Succeeded(Result r) { return r >= 0; }
	inline bool Failed(Result r) { return r < 0; }

	// points
	using Point2D = utility::_Point2d<int>;
	using Point2F = utility::_Point2d<float>;
	using Point2U = utility::_Point2d<unsigned int>;
	using Point2L = utility::_Point2d<long long>;


	// sizes
	using Size2D = utility::_Size2d<int>;
	using Size2F = utility::_Size2d<float>;
	using Size2U = utility::_Size2d<unsigned int>;
	using Size2L = utility::_Size2d<long long>;

	// vectors
	using Vector2D = utility::_Point2d<int>;
	using Vector2F = utility::_Point2d<float>;
	using Vector2U = utility::_Point2d<unsigned int>;
	using Vector2L = utility::_Point2d<long long>;

	// shapes
	using Rect = utility::_Rect<int>;
	using RectF	= utility::_Rect<float>;
	using Ellipse = utility::_Ellipse<float, Point2F>;
	using RoundedRect = utility::_RoundedRect<float, RectF>;
	using Line = utility::_Line<Point2F>;
	using Bezier = utility::_Bezier<Point2F>;

	// colors
	enum class COLORS { // direct2d color codes
		ALICEBLUE = 0XF0F8FF,
		ANTIQUEWHITE = 0XFAEBD7,
		AQUA = 0X00FFFF,
		AQUAMARINE = 0X7FFFD4,
		AZURE = 0XF0FFFF,
		BEIGE = 0XF5F5DC,
		BISQUE = 0XFFE4C4,
		BLACK = 0X000000,
		BLANCHEDALMOND = 0XFFEBCD,
		BLUE = 0X0000FF,
		BLUEVIOLET = 0X8A2BE2,
		BROWN = 0XA52A2A,
		BURLYWOOD = 0XDEB887,
		CADETBLUE = 0X5F9EA0,
		CHARTREUSE = 0X7FFF00,
		CHOCOLATE = 0XD2691E,
		CORAL = 0XFF7F50,
		CORNFLOWERBLUE = 0X6495ED,
		CORNSILK = 0XFFF8DC,
		CRIMSON = 0XDC143C,
		CYAN = 0X00FFFF,
		DARKBLUE = 0X00008B,
		DARKCYAN = 0X008B8B,
		DARKGOLDENROD = 0XB8860B,
		DARKGRAY = 0XA9A9A9,
		DARKGREEN = 0X006400,
		DARKKHAKI = 0XBDB76B,
		DARKMAGENTA = 0X8B008B,
		DARKOLIVEGREEN = 0X556B2F,
		DARKORANGE = 0XFF8C00,
		DARKORCHID = 0X9932CC,
		DARKRED = 0X8B0000,
		DARKSALMON = 0XE9967A,
		DARKSEAGREEN = 0X8FBC8F,
		DARKSLATEBLUE = 0X483D8B,
		DARKSLATEGRAY = 0X2F4F4F,
		DARKTURQUOISE = 0X00CED1,
		DARKVIOLET = 0X9400D3,
		DEEPPINK = 0XFF1493,
		DEEPSKYBLUE = 0X00BFFF,
		DIMGRAY = 0X696969,
		DODGERBLUE = 0X1E90FF,
		FIREBRICK = 0XB22222,
		FLORALWHITE = 0XFFFAF0,
		FORESTGREEN = 0X228B22,
		FUCHSIA = 0XFF00FF,
		GAINSBORO = 0XDCDCDC,
		GHOSTWHITE = 0XF8F8FF,
		GOLD = 0XFFD700,
		GOLDENROD = 0XDAA520,
		GRAY = 0X808080,
		GREEN = 0X008000,
		GREENYELLOW = 0XADFF2F,
		HONEYDEW = 0XF0FFF0,
		HOTPINK = 0XFF69B4,
		INDIANRED = 0XCD5C5C,
		INDIGO = 0X4B0082,
		IVORY = 0XFFFFF0,
		KHAKI = 0XF0E68C,
		LAVENDER = 0XE6E6FA,
		LAVENDERBLUSH = 0XFFF0F5,
		LAWNGREEN = 0X7CFC00,
		LEMONCHIFFON = 0XFFFACD,
		LIGHTBLUE = 0XADD8E6,
		LIGHTCORAL = 0XF08080,
		LIGHTCYAN = 0XE0FFFF,
		LIGHTGOLDENRODYELLOW = 0XFAFAD2,
		LIGHTGREEN = 0X90EE90,
		LIGHTGRAY = 0XD3D3D3,
		LIGHTPINK = 0XFFB6C1,
		LIGHTSALMON = 0XFFA07A,
		LIGHTSEAGREEN = 0X20B2AA,
		LIGHTSKYBLUE = 0X87CEFA,
		LIGHTSLATEGRAY = 0X778899,
		LIGHTSTEELBLUE = 0XB0C4DE,
		LIGHTYELLOW = 0XFFFFE0,
		LIME = 0X00FF00,
		LIMEGREEN = 0X32CD32,
		LINEN = 0XFAF0E6,
		MAGENTA = 0XFF00FF,
		MAROON = 0X800000,
		MEDIUMAQUAMARINE = 0X66CDAA,
		MEDIUMBLUE = 0X0000CD,
		MEDIUMORCHID = 0XBA55D3,
		MEDIUMPURPLE = 0X9370DB,
		MEDIUMSEAGREEN = 0X3CB371,
		MEDIUMSLATEBLUE = 0X7B68EE,
		MEDIUMSPRINGGREEN = 0X00FA9A,
		MEDIUMTURQUOISE = 0X48D1CC,
		MEDIUMVIOLETRED = 0XC71585,
		MIDNIGHTBLUE = 0X191970,
		MINTCREAM = 0XF5FFFA,
		MISTYROSE = 0XFFE4E1,
		MOCCASIN = 0XFFE4B5,
		NAVAJOWHITE = 0XFFDEAD,
		NAVY = 0X000080,
		OLDLACE = 0XFDF5E6,
		OLIVE = 0X808000,
		OLIVEDRAB = 0X6B8E23,
		ORANGE = 0XFFA500,
		ORANGERED = 0XFF4500,
		ORCHID = 0XDA70D6,
		PALEGOLDENROD = 0XEEE8AA,
		PALEGREEN = 0X98FB98,
		PALETURQUOISE = 0XAFEEEE,
		PALEVIOLETRED = 0XDB7093,
		PAPAYAWHIP = 0XFFEFD5,
		PEACHPUFF = 0XFFDAB9,
		PERU = 0XCD853F,
		PINK = 0XFFC0CB,
		PLUM = 0XDDA0DD,
		POWDERBLUE = 0XB0E0E6,
		PURPLE = 0X800080,
		RED = 0XFF0000,
		ROSYBROWN = 0XBC8F8F,
		ROYALBLUE = 0X4169E1,
		SADDLEBROWN = 0X8B4513,
		SALMON = 0XFA8072,
		SANDYBROWN = 0XF4A460,
		SEAGREEN = 0X2E8B57,
		SEASHELL = 0XFFF5EE,
		SIENNA = 0XA0522D,
		SILVER = 0XC0C0C0,
		SKYBLUE = 0X87CEEB,
		SLATEBLUE = 0X6A5ACD,
		SLATEGRAY = 0X708090,
		SNOW = 0XFFFAFA,
		SPRINGGREEN = 0X00FF7F,
		STEELBLUE = 0X4682B4,
		TAN = 0XD2B48C,
		TEAL = 0X008080,
		THISTLE = 0XD8BFD8,
		TOMATO = 0XFF6347,
		TURQUOISE = 0X40E0D0,
		VIOLET = 0XEE82EE,
		WHEAT = 0XF5DEB3,
		WHITE = 0XFFFFFF,
		WHITESMOKE = 0XF5F5F5,
		YELLOW = 0XFFFF00,
		YELLOWGREEN = 0X9ACD32,
	};

	struct Color {
		float R = 0., G = 0., B = 0., a = 1.;
		Color(float R = 0, float G = 0, float B = 0, float a = 1) {
			this->R = R;
			this->G = G;
			this->B = B;
			this->a = a;
		}
		Color(COLORS d2dColor) {
			D2D1_COLOR_F converter = D2D1::ColorF(static_cast<int>(d2dColor));
			this->R = converter.r;
			this->G = converter.g;
			this->B = converter.b;
			this->a = converter.a;
		}
	};
}
#pragma endregion

#pragma region Events
namespace oppo {
	// window
	enum class EVENTS {
		NONE = 0,

		// window events
		CREATE,
		CLOSE,
		DESTROY,

		// user input
		KEYDOWN,
		KEYUP,
		CHAR,

		MOUSEMOVE,
		MOUSELEAVE,
		MOUSEDOWN,
		MOUSEDBLCLK,
		MOUSEUP,
		MOUSESCROLL,

		// graphics
		PAINT,
		UPDATE,
	};

	struct Event {
		EVENTS type = EVENTS::NONE;
		int windowID = 0;

		union {
			long long aParam = 0; // general purpose
			double dt; // used for timer events
			long long key; // oppo::KEYS key
			char character; // key characters
			struct {
				long x; // horizontal mousewheel
				long y; // vertical mousewheel
			} mouseWheel;
		};

		union {
			long long bParam = 0; // general purpose
			struct {
				long x; // mouse x position
				long y; // mouse y position
			} mouse;
		};

		struct {
			unsigned int repeat : 16; // character repeat count
			unsigned int vScroll : 1; // vertical scroll flag
			unsigned int hScroll : 1; // horizontal scroll flag
			unsigned int alt : 1; // alt key down
		} flags;
	};
}
#pragma endregion

#pragma region Keys
namespace oppo {
#ifdef DELETE
#undef DELETE
#endif

	enum KEYS {
		// reserved
		EXTRA,

		// mouse 
		LMOUSE,
		MMOUSE,
		RMOUSE,
		XMOUSE1,
		XMOUSE2,

		// media
		MEDIA_VOLUME_MUTE,
		MEDIA_VOLUME_DOWN,
		MEDIA_VOLUME_UP,
		MEDIA_NEXT,
		MEDIA_PREV,
		MEDIA_STOP,

		// arrow keys
		LEFT,
		UP,
		RIGHT,
		DOWN,

		// other
		BACK,
		TAB,
		ENTER,
		SHIFT,
		CTRL,
		CAPS,
		ESCAPE,
		PGUP,
		PGDN,
		END,
		HOME,
		INSERT,
		DELETE,
		ALT,
		SCROLL_LOCK,

		// with associated chars
		SPACE = ' ',
		// !
		QUOTE = '"',
		// #
		// $
		// %
		// &
		// '
		// (
		// )
		// *
		PLUS = '+',
		COMMA,
		MINUS,
		PERIOD,
		// /
		ZERO = '0',
		ONE,
		TWO,
		THREE,
		FOUR,
		FIVE,
		SIX,
		SEVEN,
		EIGHT,
		NINE,
		COLON,
		// ;
		// <
		// =
		// >
		QMARK = '?',
		AT,
		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		LBRACKET,
		BSLASH,
		RBRACKET,
		// ^
		// _
		// `
		// lowercase alphabet
		// {
		// |
		// }
		TILDE = '~',

		// numpad
		NP_0, NP_1, NP_2, NP_3, NP_4, NP_5, NP_6, NP_7, NP_8, NP_9,
		NP_MULTIPLY,
		NP_ADD,
		NP_SUBTRACT,
		NP_DECIMAL,
		NP_DIVIDE,
		NP_LOCK,

		// function keys
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24,
	};
}
#pragma endregion

#pragma region Fonts
namespace oppo {
	enum class FONTS {
		ARIAL,
		ARIAL_BLACK,
		BAHNSCHRIFT,
		CALIBRI,
		CAMBRIA,
		CAMBRIA_MATH,
		CANDARA,
		CASCADIA_CODE,
		CASCADIA_MONO,
		COMIC_SANS_MS,
		CONSOLAS,
		CONSTANTIA,
		CORBEL,
		COURIER_NEW,
		EBRIMA,
		FRANKLIN_GOTHIC_MEDIUM,
		GABRIOLA,
		GADUGI,
		GEORGIA,
		HOLOLENS_MDL2_ASSETS,
		IMPACT,
		INK_FREE,
		JAVANESE_TEXT,
		LEELAWADEE_UI,
		LUCIDA_CONSOLE,
		LUCIDA_SANS_UNICODE,
		MALGUN_GOTHIC,
		MARLETT,
		MICROSOFT_HIMALAYA,
		MICROSOFT_JHENGHEI,
		MICROSOFT_NEW_TAI_LUE,
		MICROSOFT_PHAGSPA,
		MICROSOFT_SANS_SERIF,
		MICROSOFT_TAI_LE,
		MICROSOFT_YAHEI,
		MICROSOFT_YI_BAITI,
		MINGLIU_EXTB,
		MONGOLIAN_BAITI,
		MS_GOTHIC,
		MV_BOLI,
		MYANMAR_TEXT,
		NIRMALA_UI,
		PALATINO_LINOTYPE,
		SEGOE_FLUENT_ICONS,
		SEGOE_MDL2_ASSETS,
		SEGOE_PRINT,
		SEGOE_SCRIPT,
		SEGOE_UI,
		SEGOE_UI_EMOJI,
		SEGOE_UI_HISTORIC,
		SEGOE_UI_SYMBOL,
		SEGOE_UI_VARIABLE,
		SIMSUN,
		SITKA,
		SYLFAEN,
		SYMBOL,
		TAHOMA,
		TIMES_NEW_ROMAN,
		TREBUCHET_MS,
		VERDANA,
		WEBDINGS,
		WINGDINGS,
		YU_GOTHIC,
	};


	inline std::map<FONTS, const char*> FONTNAMES = {
		{FONTS::ARIAL,					"Arial"					},
		{FONTS::ARIAL_BLACK,			"Arial Black"			},
		{FONTS::BAHNSCHRIFT,			"Bahnschrift"			},
		{FONTS::CALIBRI,				"Calibri"				},
		{FONTS::CAMBRIA,				"Cambria"				},
		{FONTS::CAMBRIA_MATH,			"Cambria Math"			},
		{FONTS::CANDARA,				"Candara"				},
		{FONTS::CASCADIA_CODE,			"Cascadia Code"			},
		{FONTS::CASCADIA_MONO,			"Cascadia Mono"			},
		{FONTS::COMIC_SANS_MS,			"Comic Sans MS"			},
		{FONTS::CONSOLAS,				"Consolas"				},
		{FONTS::CONSTANTIA,				"Constantia"			},
		{FONTS::CORBEL,					"Corbel"				},
		{FONTS::COURIER_NEW,			"Courier New"			},
		{FONTS::EBRIMA,					"Ebrima"				},
		{FONTS::FRANKLIN_GOTHIC_MEDIUM,	"Franklin Gothic Medium"},
		{FONTS::GABRIOLA,				"Gabriola"				},
		{FONTS::GADUGI,					"Gadugi"				},
		{FONTS::GEORGIA,				"Georgia"				},
		{FONTS::HOLOLENS_MDL2_ASSETS,	"HoloLens MDL2 Assets"	},
		{FONTS::IMPACT,					"Impact"				},
		{FONTS::INK_FREE,				"Ink Free"				},
		{FONTS::JAVANESE_TEXT,			"Javanese Text"			},
		{FONTS::LEELAWADEE_UI,			"Leelawadee UI"			},
		{FONTS::LUCIDA_CONSOLE,			"Lucida Console"		},
		{FONTS::LUCIDA_SANS_UNICODE,	"Lucida Sans Unicode"	},
		{FONTS::MALGUN_GOTHIC,			"Malgun Gothic"			},
		{FONTS::MARLETT,				"Marlett"				},
		{FONTS::MICROSOFT_HIMALAYA,		"Microsoft Himalaya"	},
		{FONTS::MICROSOFT_JHENGHEI,		"Microsoft JhengHei"	},
		{FONTS::MICROSOFT_NEW_TAI_LUE,	"Microsoft New Tai Lue"	},
		{FONTS::MICROSOFT_PHAGSPA,		"Microsoft PhagsPa"		},
		{FONTS::MICROSOFT_SANS_SERIF,	"Microsoft Sans Serif"	},
		{FONTS::MICROSOFT_TAI_LE,		"Microsoft Tai Le"		},
		{FONTS::MICROSOFT_YAHEI,		"Microsoft YaHei"		},
		{FONTS::MICROSOFT_YI_BAITI,		"Microsoft Yi Baiti"	},
		{FONTS::MINGLIU_EXTB,			"MingLiU-ExtB"			},
		{FONTS::MONGOLIAN_BAITI,		"Mongolian Baiti"		},
		{FONTS::MS_GOTHIC,				"MS Gothic"				},
		{FONTS::MV_BOLI,				"MV Boli"				},
		{FONTS::MYANMAR_TEXT,			"Myanmar Text"			},
		{FONTS::NIRMALA_UI,				"Nirmala UI"			},
		{FONTS::PALATINO_LINOTYPE,		"Palatino Linotype"		},
		{FONTS::SEGOE_FLUENT_ICONS,		"Segoe Fluent Icons"	},
		{FONTS::SEGOE_MDL2_ASSETS,		"Segoe MDL2 Assets"		},
		{FONTS::SEGOE_PRINT,			"Segoe Print"			},
		{FONTS::SEGOE_SCRIPT,			"Segoe Script"			},
		{FONTS::SEGOE_UI,				"Segoe UI"				},
		{FONTS::SEGOE_UI_EMOJI,			"Segoe UI Emoji"		},
		{FONTS::SEGOE_UI_HISTORIC,		"Segoe UI Historic"		},
		{FONTS::SEGOE_UI_SYMBOL,		"Segoe UI Symbol"		},
		{FONTS::SEGOE_UI_VARIABLE,		"Segoe UI Variable"		},
		{FONTS::SIMSUN,					"SimSun"				},
		{FONTS::SITKA,					"Sitka"					},
		{FONTS::SYLFAEN,				"Sylfaen"				},
		{FONTS::SYMBOL,					"Symbol"				},
		{FONTS::TAHOMA,					"Tahoma"				},
		{FONTS::TIMES_NEW_ROMAN,		"Times New Roman"		},
		{FONTS::TREBUCHET_MS,			"Trebuchet MS"			},
		{FONTS::VERDANA,				"Verdana"				},
		{FONTS::WEBDINGS,				"Webdings"				},
		{FONTS::WINGDINGS,				"Wingdings"				},
		{FONTS::YU_GOTHIC,				"Yu Gothic"				},
	};

	enum FONT_WEIGHTS {
		// value between 1 and 999
		EXTRA_THIN = 50,
		THIN = 100,
		EXTRA_LIGHT = 200,
		LIGHT = 300,
		NORMAL = 400,
		MEDIUM = 500,
		SEMI_BOLD = 600,
		BOLD = 700,
		EXTRA_BOLD = 800,
		BLACK = 900,
		EXTRA_BLACK = 950,
	};
	enum class FONT_STYLES {
		NORMAL = 0,
		OBLIQUE = 1,
		ITALIC = 2,
	};
	enum class FONT_STRETCHES {
		// value between 1 and 9
		ULTRA_CONDENSED = 1,
		EXTRA_CONDENSED = 2,
		CONDENSED = 3,
		SEMI_CONDENSED = 4,
		NORMAL = 5,
		MEDIUM = 5,
		SEMI_EXPANDED = 6,
		EXPANDED = 7,
		EXTRA_EXPANDED = 8,
		ULTRA_EXPANDED = 9
	};
	enum class TEXT_FLOW_DIRECTION {
		TOP_TO_BOTTOM = 0,
		BOTTOM_TO_TOP = 1,
		LEFT_TO_RIGHT = 2,
		RIGHT_TO_LEFT = 3,
	};
	enum class TEXT_HORIZONTAL_ALIGNMENT {
		LEFT,
		RIGHT,
		CENTER,
	};
	enum class TEXT_VERTICAL_ALIGNMENT {
		TOP,
		BOTTOM,
		CENTER,
	};
	enum class TEXT_WRAPPING {
		WRAP = 0,
		NO_WRAP = 1,
		EMERGENCY_BREAK = 2,
		WHOLE_WORD_WRAP = 3,
		CHARACTER_WRAP = 4,
	};
	enum class TEXT_CLIPPING {
		NO_CLIP = 0,
		CLIP = 2,
	};
}

#pragma endregion