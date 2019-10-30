#pragma once

#include "Crow/Math/Vector/Vec2.h"

#define MAX_KEYS 512

#define MAX_MOUSE_BUTTONS 32

namespace Crow {

	class Input
	{
	public:

		static void Init();

		inline static short int GetKeyState(int key) { return s_Keys[key]; }
		inline static short int GetKeyState(short int key) { return s_MouseKeys[key]; }
		inline static Math::Vec2<int> GetMousePosition() { return s_MousePos; }

		static bool IsKeyPressed(short int key) { return s_Keys[key] > 0; } // if key action is more than KEY_RELEASE

		static void KeyCallback(Math::Vec2<int> key);
		static void MouseCallback(Math::Vec2<int> key);
		static void MousePosCallback(Math::Vec2<int> key);

	private:
		static short int s_Keys[MAX_KEYS];
		static short int s_MouseKeys[MAX_KEYS];
		static Math::Vec2<int> s_MousePos;
	};

}

// Key actions
#define CROW_KEY_RELEASE  0
#define CROW_KEY_PRESS 1
#define CROW_KEY_REPEAT 2

// Key codes
#define CROW_KEY_UNKNOWN            -1

/* Printable keys */
#define CROW_KEY_SPACE              32
#define CROW_KEY_APOSTROPHE         39  /* ' */
#define CROW_KEY_COMMA              44  /* , */
#define CROW_KEY_MINUS              45  /* - */
#define CROW_KEY_PERIOD             46  /* . */
#define CROW_KEY_SLASH              47  /* / */
#define CROW_KEY_0                  48
#define CROW_KEY_1                  49
#define CROW_KEY_2                  50
#define CROW_KEY_3                  51
#define CROW_KEY_4                  52
#define CROW_KEY_5                  53
#define CROW_KEY_6                  54
#define CROW_KEY_7                  55
#define CROW_KEY_8                  56
#define CROW_KEY_9                  57
#define CROW_KEY_SEMICOLON          59  /* ; */
#define CROW_KEY_EQUAL              61  /* = */
#define CROW_KEY_A                  65
#define CROW_KEY_B                  66
#define CROW_KEY_C                  67
#define CROW_KEY_D                  68
#define CROW_KEY_E                  69
#define CROW_KEY_F                  70
#define CROW_KEY_G                  71
#define CROW_KEY_H                  72
#define CROW_KEY_I                  73
#define CROW_KEY_J                  74
#define CROW_KEY_K                  75
#define CROW_KEY_L                  76
#define CROW_KEY_M                  77
#define CROW_KEY_N                  78
#define CROW_KEY_O                  79
#define CROW_KEY_P                  80
#define CROW_KEY_Q                  81
#define CROW_KEY_R                  82
#define CROW_KEY_S                  83
#define CROW_KEY_T                  84
#define CROW_KEY_U                  85
#define CROW_KEY_V                  86
#define CROW_KEY_W                  87
#define CROW_KEY_X                  88
#define CROW_KEY_Y                  89
#define CROW_KEY_Z                  90
#define CROW_KEY_LEFT_BRACKET       91  /* [ */
#define CROW_KEY_BACKSLASH          92  /* \ */
#define CROW_KEY_RIGHT_BRACKET      93  /* ] */
#define CROW_KEY_GRAVE_ACCENT       96  /* ` */
#define CROW_KEY_WORLD_1            161 /* non-US #1 */
#define CROW_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define CROW_KEY_ESCAPE             256
#define CROW_KEY_ENTER              257
#define CROW_KEY_TAB                258
#define CROW_KEY_BACKSPACE          259
#define CROW_KEY_INSERT             260
#define CROW_KEY_DELETE             261
#define CROW_KEY_RIGHT              262
#define CROW_KEY_LEFT               263
#define CROW_KEY_DOWN               264
#define CROW_KEY_UP                 265
#define CROW_KEY_PAGE_UP            266
#define CROW_KEY_PAGE_DOWN          267
#define CROW_KEY_HOME               268
#define CROW_KEY_END                269
#define CROW_KEY_CAPS_LOCK          280
#define CROW_KEY_SCROLL_LOCK        281
#define CROW_KEY_NUM_LOCK           282
#define CROW_KEY_PRINT_SCREEN       283
#define CROW_KEY_PAUSE              284
#define CROW_KEY_F1                 290
#define CROW_KEY_F2                 291
#define CROW_KEY_F3                 292
#define CROW_KEY_F4                 293
#define CROW_KEY_F5                 294
#define CROW_KEY_F6                 295
#define CROW_KEY_F7                 296
#define CROW_KEY_F8                 297
#define CROW_KEY_F9                 298
#define CROW_KEY_F10                299
#define CROW_KEY_F11                300
#define CROW_KEY_F12                301
#define CROW_KEY_F13                302
#define CROW_KEY_F14                303
#define CROW_KEY_F15                304
#define CROW_KEY_F16                305
#define CROW_KEY_F17                306
#define CROW_KEY_F18                307
#define CROW_KEY_F19                308
#define CROW_KEY_F20                309
#define CROW_KEY_F21                310
#define CROW_KEY_F22                311
#define CROW_KEY_F23                312
#define CROW_KEY_F24                313
#define CROW_KEY_F25                314
#define CROW_KEY_KP_0               320
#define CROW_KEY_KP_1               321
#define CROW_KEY_KP_2               322
#define CROW_KEY_KP_3               323
#define CROW_KEY_KP_4               324
#define CROW_KEY_KP_5               325
#define CROW_KEY_KP_6               326
#define CROW_KEY_KP_7               327
#define CROW_KEY_KP_8               328
#define CROW_KEY_KP_9               329
#define CROW_KEY_KP_DECIMAL         330
#define CROW_KEY_KP_DIVIDE          331
#define CROW_KEY_KP_MULTIPLY        332
#define CROW_KEY_KP_SUBTRACT        333
#define CROW_KEY_KP_ADD             334
#define CROW_KEY_KP_ENTER           335
#define CROW_KEY_KP_EQUAL           336
#define CROW_KEY_LEFT_SHIFT         340
#define CROW_KEY_LEFT_CONTROL       341
#define CROW_KEY_LEFT_ALT           342
#define CROW_KEY_LEFT_SUPER         343
#define CROW_KEY_RIGHT_SHIFT        344
#define CROW_KEY_RIGHT_CONTROL      345
#define CROW_KEY_RIGHT_ALT          346
#define CROW_KEY_RIGHT_SUPER        347
#define CROW_KEY_MENU               348

#define CROW_KEY_LAST               CROW_KEY_MENU

/* Mouse Buttons */
#define CROW_MOUSE_BUTTON_1         0
#define CROW_MOUSE_BUTTON_2         1
#define CROW_MOUSE_BUTTON_3         2
#define CROW_MOUSE_BUTTON_4         3
#define CROW_MOUSE_BUTTON_5         4
#define CROW_MOUSE_BUTTON_6         5
#define CROW_MOUSE_BUTTON_7         6
#define CROW_MOUSE_BUTTON_8         7
#define CROW_MOUSE_BUTTON_LAST      CROW_MOUSE_BUTTON_8
#define CROW_MOUSE_BUTTON_LEFT      CROW_MOUSE_BUTTON_1
#define CROW_MOUSE_BUTTON_RIGHT     CROW_MOUSE_BUTTON_2
#define CROW_MOUSE_BUTTON_MIDDLE    CROW_MOUSE_BUTTON_3
