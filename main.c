#include "raylib.h"
#include <complex.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

unsigned int MAX_ITER = 100;
#define MOVE_DELTA 0.5
#define ZOOM_DELTA 0.5

void almondBread(Texture2D* texture, Color* pixels, Vector2 position, float zoom){
	Vector2 minExtent = {position.x - zoom, position.y + zoom};
	Vector2 maxExtent = {position.x + zoom, position.y - zoom};
	for(int y = 0; y < texture->height; y++)
	for(int x = 0; x < texture->width;  x++){

		double xf = (double)x / texture->width;
		double yf = (double)y / texture->height;

		complex double z = 0 + 0 * I;
		complex double c = 
			xf * (maxExtent.x - minExtent.x) + minExtent.x +
	    (yf * (maxExtent.y - minExtent.y) - minExtent.y) * I;
		int iter;
		for(iter = 0; iter < MAX_ITER; iter++){
			z = z * z + c;
			if(cabs(z) > 2){
				break;
			}
		}
		pixels[y*texture->width + x] = 
			(Color){0,0, iter * 255 / MAX_ITER,255};
	}
	UpdateTexture(*texture, pixels);
}

int main(){
	int width = 800, height = 600;

	InitWindow(width, height, "Hello World!");


	Image im = (Image){
		.width = width,
		.height = height,
		.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
		.data = MemAlloc(sizeof(Color) * width * height),
		.mipmaps = 1
	};
	Texture2D texture = LoadTextureFromImage(im);

	Vector2 position = {0,0};
	double zoom = 1;


	//almondBread(&texture, im.data, position, zoom);


	double t = GetTime();
	while(!WindowShouldClose()){
		double dt = GetTime() - t;
		if (IsKeyDown(KEY_W)){
			position.y -= MOVE_DELTA * zoom * dt;
		}
		if (IsKeyDown(KEY_S)){
			position.y += MOVE_DELTA * zoom * dt;
		}
		if (IsKeyDown(KEY_A)){
			position.x -= MOVE_DELTA * zoom * dt;
		}
		if (IsKeyDown(KEY_D)){
			position.x += MOVE_DELTA * zoom * dt;
		}
		if (IsKeyDown(KEY_Q)){
			zoom *= 1 + (ZOOM_DELTA * dt);
		}
		if (IsKeyDown(KEY_E)){
			zoom /= 1 + (ZOOM_DELTA * dt);
		}
		if (IsKeyDown(KEY_I)){
			if (IsKeyPressed(KEY_KP_ADD))
				MAX_ITER += 5;
			if (IsKeyPressed(KEY_KP_SUBTRACT)){
				MAX_ITER -= 5;
				if(MAX_ITER<5) MAX_ITER = 5;
			}
		}
		t = GetTime();

		almondBread(&texture, im.data, position, zoom);

		BeginDrawing();
		ClearBackground(BLACK);

		DrawTexture(texture, 0, 0, WHITE);
		char bottomText[64];
		sprintf(bottomText, "%d Iterations", MAX_ITER);
		DrawText(bottomText, 0, height - 32, 24, WHITE);

		EndDrawing();
	};

	CloseWindow();
	return 0;
}
