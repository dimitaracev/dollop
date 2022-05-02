#include <stdio.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#define SIZE 200
#define PIXEL_SIZE 15
#define HEIGHT 600
#define WIDTH 600
#define WAIT 250000

static short m[SIZE][SIZE];
static short n[SIZE][SIZE];
static short turn = 1;
static short (*eval)[SIZE][SIZE];

void setup(short array[SIZE][SIZE])
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            array[i][j] = 0;
        }
    }
}

int count_neighbours(int i, int j, short array[SIZE][SIZE])
{
    return 
    ((i - 1 < 0) ? 0 : array[i - 1][j]) +
    ((i + 1 > (SIZE - 1)) ? 0 : array[i + 1][j]) +
    (((i - 1 < 0) || (j - 1 < 0)) ? 0 : array[i - 1][j - 1]) +
    (((i + 1 > (SIZE - 1)) || (j + 1 > (SIZE - 1))) ? 0 : array[i + 1][j + 1]) +
    (((i - 1 < 0) || (j + 1 > (SIZE - 1))) ? 0 : array[i - 1][j + 1]) +
    (((i + 1 > (SIZE - 1)) || (j - 1 < 0)) ? 0 : array[i + 1][j - 1]) +
    ((j - 1 < 0) ? 0 : array[i][j - 1]) +
    ((j + 1 > (SIZE - 1)) ? 0 : array[i][j + 1]);
}

void perform_swap(int neighbour_count, int i, int j, short prev_generation[SIZE][SIZE], short next_generation[SIZE][SIZE])
{
    if ((neighbour_count == 2 || neighbour_count == 3) && prev_generation[i][j] == 1)
    {
        next_generation[i][j] = 1;
    }
    else if (neighbour_count > 3 || neighbour_count < 2)
    {
        next_generation[i][j] = 0;
    }
    else if ((neighbour_count == 3 && prev_generation[i][j] == 0))
    {
        next_generation[i][j] = 1;
    }
}

void next_generation()
{
    if (turn)
    {
        eval = &n;
        setup(n);
        for (int i = 0; i < SIZE; i++)
        {
            for (int j = 0; j < SIZE; j++)
            {
                int count = count_neighbours(i, j, m);
                perform_swap(count, i, j, m ,n);   
            }
        }
    }
    else
    {
        eval = &m;
        setup(m);
        for (int i = 0; i < SIZE; i++)
        {
            for (int j = 0; j < SIZE; j++)
            {
                int count = count_neighbours(i, j, n);
                perform_swap(count, i, j, n, m);
            }
        }
    }
    turn = !turn;
}

void loop(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    next_generation();
 
    int r = (rand() * (rand() + 100)) % 255;
    int g = (rand() * (rand() + 100)) % 255;
    int b = (rand() * (rand() + 100)) % 255;
    int a = rand() % 255;
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if ((*eval)[i][j] == 1)
            {
                SDL_Rect rect;
                rect.h = 1;
                rect.w = 1;
                rect.x = i + PIXEL_SIZE;
                rect.y = j + PIXEL_SIZE;
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
    SDL_RenderPresent(renderer);
    usleep(WAIT);
}

int main()
{
    setup(m);
    setup(n);

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    SDL_Window *win = SDL_CreateWindow("Game of Life",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       WIDTH, HEIGHT, 0);

    SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderSetScale(renderer, 5, 5);
    SDL_RenderClear(renderer);

    SDL_RenderPresent(renderer);
    loop(renderer);
    while((*eval)[SIZE - 1][SIZE - 1] == 0)
    {
        loop(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}
