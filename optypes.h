#pragma once
#include <Windows.h>
#include <utility>
#include <d2d1.h>

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
		FAIL    = -1
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
	using Rect		  = utility::_Rect<int>;
	using RectF		  = utility::_Rect<float>;
	using Ellipse	  = utility::_Ellipse<float, Point2F>;
	using RoundedRect = utility::_RoundedRect<float, RectF>;
	using Line		  = utility::_Line<Point2F>;
	using Bezier	  = utility::_Bezier<Point2F>;

	// colors
	struct Color {
		float R = 0., G = 0., B = 0., a = 1.;
		Color(float R = 0, float G = 0, float B = 0, float a = 1) {
			this->R = R;
			this->G = G;
			this->B = B;
			this->a = a;
		}
		Color(int d2dColor) {
			D2D1_COLOR_F converter = D2D1::ColorF(d2dColor);
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
		int windowID;

		union {
			long long aParam = 0; // general purpose
			double dt; // used for timer events
			long long key; // oppo::KEYS key
			char character; // key characters
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