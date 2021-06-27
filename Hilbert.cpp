#include <cmath>
#include <cstdio>
#include <cstdint>
#include <SDL2/SDL.h>

using namespace std;

enum {
	CURVED,
	SQUARE,
	THORNY,
	ANGLED,
	SLANTED,
	SEMISLANTED,
	AERODYNAMIC,
};

enum {
	UP,
	DOWN,
	LEFT,
	RIGHT,
};

int WIDTH = 720;
int HEIGHT = 1280;

int edge = SLANTED;
int colors = true;

int stroke = 4;
int step = 10;
int leval = 6;

uint8_t R,G,B;
float slope;
int xPos = 2;
int yPos = 2;
int newX = xPos;
int newY = yPos;

void DrawLine(SDL_Renderer* renderer, int direction, int x1, int y1, int x2, int y2)
{
	if (colors == true)
	{
		R = 255 - abs(x1*255) / WIDTH;
		G = 255 - abs(y2*255) / HEIGHT;
		B = 255 - abs(pow(x2,2) - pow(y1,2));
		SDL_SetRenderDrawColor(renderer, R, G, B, 255);
	}
	else SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	
	switch (edge)
	{
	case SLANTED:
		for (int i = 0 ; i <= stroke ; i++)
		{
			SDL_RenderDrawLine(renderer, x1+i, y1+i, x2+i, y2+i);
		}
		break;
	
	case SEMISLANTED:
		for (int i = 0 ; i <= stroke ; i++)
		{
			SDL_RenderDrawLine(renderer, x1+i, y1-i, x2+i, y2-i);
		}
		break;
	
	case CURVED:			// WIP //
		switch(direction)
		{
		case UP:
			for (int i = 0 ; i <= stroke ; i++)
			{
				//SDL_RenderDrawLine(renderer, x1+i, y1+stroke-1, x2+i, y2+i);
			}
			break;
		
		case DOWN:
			for (int i = 0 ; i <= stroke ; i++)
			{
				//SDL_RenderDrawLine(renderer, x1+i, y1+stroke-1, x2+i, y2+i);
			}
			break;
		
		case RIGHT:
			for (int i = 0 ; i < stroke-1 ; i++)
			{
				SDL_RenderDrawLine(renderer, x1+i, y1+i, x2+stroke-stroke/(stroke-i), y2+i);
			}
			break;
		
		case LEFT:
			for (int i = 0 ; i <= stroke ; i++)
			{
				//SDL_RenderDrawLine(renderer, x1+i, y1+i, x2+stroke-1, y2+i);
			}
			break;
		}
		break;
	
	case ANGLED:	
		switch(direction)
		{
		case UP:
			for (int i = 0 ; i <= stroke ; i++)
			{
				SDL_RenderDrawLine(renderer, x1+i, y1+stroke-i, x2+i, y2+i);
			}
			break;
		
		case DOWN:
			for (int i = 0 ; i <= stroke ; i++)
			{
				SDL_RenderDrawLine(renderer, x1+i, y1+stroke-i, x2+i, y2+i);
			}
			break;
		
		case RIGHT:
			for (int i = 0 ; i <= stroke ; i++)
			{
				SDL_RenderDrawLine(renderer, x1+i, y1+i, x2+stroke-i, y2+i);
			}
			break;
		
		case LEFT:
			for (int i = 0 ; i <= stroke ; i++)
			{
				SDL_RenderDrawLine(renderer, x1+i, y1+i, x2+stroke-i, y2+i);
			}
			break;
		}
		break;
	
	case SQUARE:
		switch(direction)
		{
		case DOWN:
			for (int i = 0 ; i <= stroke ; i++)
			{
				SDL_RenderDrawLine(renderer, x1+i, y1, x2+i, y2+stroke);
			}
			break;
		
		case UP:
			for (int i = 0 ; i <= stroke ; i++)
			{
				SDL_RenderDrawLine(renderer, x1+i, y1, x2+i, y2);
			}
			break;
		
		case LEFT:
			for (int i = 0 ; i <= stroke ; i++)
			{
				SDL_RenderDrawLine(renderer, x1, y1+i, x2, y2+i);
			}
			break;
		
		case RIGHT:
			for (int i = 0 ; i <= stroke ; i++)
			{
				SDL_RenderDrawLine(renderer, x1, y1+i, x2+stroke, y2+i);
			}
			break;
		}
		break;
	
	case THORNY:
		for (int i = 0 ; i <= stroke ; i++)
		{
			SDL_RenderDrawLine(renderer, x1+i, y1+i, x2-i, y2-i);
		}
		break;
	
	case AERODYNAMIC:
		for (int i = 0 ; i <= stroke ; i++)
		{
			SDL_RenderDrawLine(renderer, x1+i, y1+i, x2-i, y2+i);
		}
		break;
	}//switch
}

void move(SDL_Renderer* renderer, SDL_Texture* backbuff, int direction)
	{
		//* Setting Render Target to BackBuffer *//
		SDL_SetRenderTarget(renderer, backbuff);
		
		switch (direction)
		{
		case UP:
			newY = yPos - step;
			DrawLine(renderer, direction, xPos, yPos, newX, newY);
			xPos = newX; yPos = newY; break;
		
		case DOWN:
			newY = yPos + step;
			DrawLine(renderer, direction, xPos, yPos, newX, newY);
			xPos = newX; yPos = newY; break;
		
		case LEFT:
			newX = xPos - step;
			DrawLine(renderer, direction, xPos, yPos, newX, newY);
			xPos = newX; yPos = newY; break;
		
		case RIGHT:
			newX = xPos + step;
			DrawLine(renderer, direction, xPos, yPos, newX, newY);
			xPos = newX; yPos = newY; break;
		}
		
		//* Rendering from BackBuffer to Screen *//
		SDL_SetRenderTarget(renderer, NULL);
		SDL_RenderCopy(renderer, backbuff, NULL, NULL);
		SDL_RenderPresent(renderer);
		
		//* Clearing ScreenBuffer with Black Color *//
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);// usleep(1);
	}
	
	void draw_hilbert(SDL_Renderer* renderer, SDL_Texture* backbuff, int level, int direction=UP)
	{
		if (level==1)
		{
			switch (direction) {
			case LEFT:
				move(renderer, backbuff, RIGHT);
				move(renderer, backbuff, DOWN);
				move(renderer, backbuff, LEFT);
				break;
			case RIGHT:
				move(renderer, backbuff, LEFT);
				move(renderer, backbuff, UP);
				move(renderer, backbuff, RIGHT);
				break;
			case UP:
				move(renderer, backbuff, DOWN);
				move(renderer, backbuff, RIGHT);
				move(renderer, backbuff, UP);
				break;
			case DOWN:
				move(renderer, backbuff, UP);
				move(renderer, backbuff, LEFT);
				move(renderer, backbuff, DOWN);
				break;
			} /* switch */
		}
		else
		{
			level -= 1;
			switch (direction)
			{
			case LEFT:
				draw_hilbert(renderer, backbuff, level, UP);
				move(renderer, backbuff, RIGHT);
				draw_hilbert(renderer, backbuff, level, LEFT);
				move(renderer, backbuff, DOWN);
				draw_hilbert(renderer, backbuff, level, LEFT);
				move(renderer, backbuff, LEFT);
				draw_hilbert(renderer, backbuff, level, DOWN);
				break;
				
			case RIGHT:
				draw_hilbert(renderer, backbuff, level, DOWN);
				move(renderer, backbuff, LEFT);
				draw_hilbert(renderer, backbuff, level, RIGHT);
				move(renderer, backbuff, UP);
				draw_hilbert(renderer, backbuff, level, RIGHT);
				move(renderer, backbuff, RIGHT);
				draw_hilbert(renderer, backbuff, level, UP);
				break;
				
			case UP:
				draw_hilbert(renderer, backbuff, level, LEFT);
				move(renderer, backbuff, DOWN);
				draw_hilbert(renderer, backbuff, level, UP);
				move(renderer, backbuff, RIGHT);
				draw_hilbert(renderer, backbuff, level, UP);
				move(renderer, backbuff, UP);
				draw_hilbert(renderer, backbuff, level, RIGHT);
				break;
				
			case DOWN:
				draw_hilbert(renderer, backbuff, level, RIGHT);
				move(renderer, backbuff, UP);
				draw_hilbert(renderer, backbuff, level, DOWN);
				move(renderer, backbuff, LEFT);
				draw_hilbert(renderer, backbuff, level, DOWN);
				move(renderer, backbuff, DOWN);
				draw_hilbert(renderer, backbuff, level, LEFT);
				break;
			} /* switch */
		} /* else */
	}
	

int main()
{
	SDL_Event event;
	SDL_Renderer* renderer;
	SDL_Window* window;
	
	SDL_Init(SDL_INIT_VIDEO);
	SDL_SetHint(SDL_HINT_RENDER_BATCHING, "1");
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"1");		// Linear Filtering
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);	// 4x MSAA
	SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);
	SDL_Texture* backbuff = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WIDTH, HEIGHT);
	//SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	
	//* Draw Hilbert *//
	draw_hilbert(renderer, backbuff, leval);
	
	while (true)	//EventLoop
	if (SDL_WaitEvent(&event) && event.type == SDL_QUIT)
	{
		//* Cleanup & Exiting *//
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		puts("exiting...");
		return EXIT_SUCCESS;
	}
}
