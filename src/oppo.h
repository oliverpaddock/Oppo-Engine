#pragma once

/* Naming conventions 
 * 
 * Classes, functions, and types: Pascal
 * Variables: Camel
 * Enums and constants: capital
 * 
 * Use namespace oppo::win32 with win32 conventions and variables for win32/DirectX functionality that user doesn't interact with
 * Use oppo namespace and wrappers of win32 elements for user interface with win32 components
 * Use oppo::utility for things used by oppo that user doesn't need
 */

// windows libraries
//#include <Windows.h>
//#include <windowsx.h>
//
//// project headers
//#include "optypes.h"
//#include "opgraphics.h"
//
///*** MACROS ***/
//// define PRODUCTION to get rid of console
//#ifdef PRODUCTION
//#	pragma comment(linker, "/subsystem:windows /entry:mainCRTStartup")
//#endif
#include "opgraphics.h"
#include "optypes.h"

