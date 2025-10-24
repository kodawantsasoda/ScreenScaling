#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <math.h>

//To be used for determining scaling
#define MAX(a, b) ((a)>(b)? (a) : (b))
#define MIN(a, b) ((a)<(b)? (a) : (b))

//dimensions for out virtual screen resolution. We chose this to play well with scaling to 1080p and 4k resolutions
const int VIRTUAL_SCREEN_WIDTH = 320;
const int VIRTUAL_SCREEN_HEIGHT = 180;

//this is the "canvas" we will be rendering to, and the initial design resolution for our application
RenderTexture2D virtualCanvas;

//scaling for determining size and position for textures
float scale = 1.0f;

//positions
const Vector2 ZERO_POS{ 0,0 };
const Vector2 VIRTUAL_RES_BOUNDS = { (float)VIRTUAL_SCREEN_WIDTH, (float)VIRTUAL_SCREEN_HEIGHT };
Vector2 mousePos{ 0, 0 };
Vector2 virtualMousePos{ 0, 0 };

const Rectangle VIRTUAL_RES_RECT = { 0.0f, 0.0f, (float)VIRTUAL_SCREEN_WIDTH, (float)-VIRTUAL_SCREEN_HEIGHT };

//initializing the window and preparing our virtual canvas to be drawn into
void Init() 
{
	//window initialization
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(VIRTUAL_SCREEN_WIDTH, VIRTUAL_SCREEN_HEIGHT, "SPACE INVADERS");
	SetWindowMinSize(VIRTUAL_SCREEN_WIDTH, VIRTUAL_SCREEN_HEIGHT);

	virtualCanvas = LoadRenderTexture(VIRTUAL_SCREEN_WIDTH, VIRTUAL_SCREEN_HEIGHT);
	//Preparing our virtual texture to be drawn onto
	SetTextureFilter(virtualCanvas.texture, TEXTURE_FILTER_POINT);
}

void DrawVirtualResolution() 
{
	Rectangle drawnVirtualRect =
	{
		(GetScreenWidth() - ((float)VIRTUAL_SCREEN_WIDTH * scale)) * 0.5f,
		(GetScreenHeight() - ((float)VIRTUAL_SCREEN_HEIGHT * scale)) * 0.5f,
		(float)VIRTUAL_SCREEN_WIDTH * scale,
		(float)VIRTUAL_SCREEN_HEIGHT * scale
	};

	Rectangle fullScreenRect = { 0, 0, GetScreenWidth(), GetScreenHeight() };

	DrawTexturePro(virtualCanvas.texture, VIRTUAL_RES_RECT, drawnVirtualRect, ZERO_POS, 0.0f, WHITE);
}

//determining 
void DetermineScale()
{
	//for testing purposes only, adjust/delete if needed, just make sure to set window size whenever toggling to full screen mode
	if (GetKeyPressed() == KEY_F11)
	{
		ToggleFullscreen();
		if (IsWindowFullscreen())
		{
			SetWindowSize(1920, 1080);
		}
	}
	scale = MIN((float)GetScreenWidth() / VIRTUAL_SCREEN_WIDTH, (float)GetScreenHeight() / VIRTUAL_SCREEN_HEIGHT);
}

//calculating mouse position in relation to the virtual resolution with the actual resolution
void DetermineVirtualMouse()
{
	mousePos = GetMousePosition();
	//offsetting the mouse to adjust for black bars on the side of screen in case of aspect ratio mismatch
	virtualMousePos.x = (mousePos.x - (GetScreenWidth() - (VIRTUAL_SCREEN_WIDTH * scale)) * 0.5f) / scale;
	virtualMousePos.y = (mousePos.y - (GetScreenHeight() - (VIRTUAL_SCREEN_HEIGHT * scale)) * 0.5f) / scale;
	virtualMousePos = Vector2Clamp(virtualMousePos, ZERO_POS, VIRTUAL_RES_BOUNDS);
}

void MouseScaleHandler()
{
	DetermineScale();
	DetermineVirtualMouse();
}

int main() 
{
	Init();
	
	//demoing with a texture
	Texture2D playerDemo = LoadTexture("Assets/Alien.png");

	while (!WindowShouldClose()) 
	{
		MouseScaleHandler();
		/*****************************************************
		VIRTUAL SCREEN DRAWING*/
		BeginTextureMode(virtualCanvas);
		ClearBackground(BLACK);

		//ADD STUFF TO DRAW HERE!
		DrawTextureEx(playerDemo, ZERO_POS, 0, 1.0f, WHITE);
		EndTextureMode();
		/*END VIRTUAL RESOLUTION DRAWING
		*****************************************************/

		/*****************************************************
		DRAWING VIRTUAL CANVAS ONTO CURRENT SCREEN RESOLUTION*/
		BeginDrawing();
		ClearBackground(WHITE);

		DrawVirtualResolution();

		EndDrawing();
		/*END DRAWING
		*****************************************************/
	}
	UnloadRenderTexture(virtualCanvas);
	CloseWindow();
}