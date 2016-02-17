#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "graphics.h"
#include "simple_logger.h"

SDL_Surface *buffer; /*pointer to the background image buffer*/
SDL_Surface *videobuffer; /*pointer to the draw buffer*/
SDL_Rect Camera; /*x & y are the coordinates for the background map, w and h are of the screen*/
Uint32 NOW;					/*the current time since program started*/

static SDL_Window   *   __gt_graphics_main_window = NULL;
static SDL_Renderer *   __gt_graphics_renderer = NULL;
static SDL_Texture  *   __gt_graphics_texture = NULL;
static SDL_Surface  *   __gt_graphics_surface = NULL;
static SDL_Surface  *   __gt_graphics_temp_buffer = NULL;

/*timing*/
static Uint32 gt_graphics_frame_delay = 30;
static Uint32 gt_graphics_now = 0;
static Uint32 gt_graphics_then = 0;
static Uint8 gt_graphics_print_fps = 1;
static float gt_graphics_fps = 0; 

/*some data on the video settings that can be useful for a lot of functions*/
static int __gt_bitdepth;
static Uint32 __gt_rmask;
static Uint32 __gt_gmask;
static Uint32 __gt_bmask;
static Uint32 __gt_amask;

void gt_graphics_close();

void Init_Graphics(
	char *windowName,
    int viewWidth,
    int viewHeight,
    int renderWidth,
    int renderHeight,
    float bgcolor[4],
    int fullscreen)
{
    Uint32 flags = 0;
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        slog("Unable to initilaize SDL system: %s",SDL_GetError());
        return;
    }
    atexit(SDL_Quit);
    if (fullscreen)
    {
        if (renderWidth == 0)
        {
            flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
        }
        else
        {
            flags |= SDL_WINDOW_FULLSCREEN;
        }
    }
    __gt_graphics_main_window = SDL_CreateWindow(windowName,
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             renderWidth, renderHeight,
                             flags);

    if (!__gt_graphics_main_window)
    {
        slog("failed to create main window: %s",SDL_GetError());
        gt_graphics_close();
        return;
    }
    
    __gt_graphics_renderer = SDL_CreateRenderer(__gt_graphics_main_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    if (!__gt_graphics_renderer)
    {
        slog("failed to create renderer: %s",SDL_GetError());
        gt_graphics_close();
        return;
    }
    
    SDL_SetRenderDrawColor(__gt_graphics_renderer, 0, 0, 0, 255);
    SDL_RenderClear(__gt_graphics_renderer);
    SDL_RenderPresent(__gt_graphics_renderer);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(__gt_graphics_renderer, renderWidth, renderHeight);

    __gt_graphics_texture = SDL_CreateTexture(
        __gt_graphics_renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        renderWidth, renderHeight);
    if (!__gt_graphics_texture)
    {
        slog("failed to create screen texture: %s",SDL_GetError());
        gt_graphics_close();
        return;
    };
    
    SDL_PixelFormatEnumToMasks(SDL_PIXELFORMAT_ARGB8888,
                                    &__gt_bitdepth,
                                    &__gt_rmask,
                                    &__gt_gmask,
                                    &__gt_bmask,
                                    &__gt_amask);

    
    __gt_graphics_surface = SDL_CreateRGBSurface(0, renderWidth, renderHeight, __gt_bitdepth,
                                        __gt_rmask,
                                    __gt_gmask,
                                    __gt_bmask,
                                    __gt_amask);
    buffer = SDL_CreateRGBSurface(0, renderWidth, renderHeight, __gt_bitdepth,
                                                 __gt_rmask,
                                                 __gt_gmask,
                                                 __gt_bmask,
                                                 __gt_amask);    
    if (!__gt_graphics_surface)
    {
        slog("failed to create screen surface: %s",SDL_GetError());
        gt_graphics_close();
        return;
    }
        
    atexit(gt_graphics_close);
    slog("graphics initialized\n");
}

void gt_graphics_render_surface_to_screen(SDL_Surface *surface,SDL_Rect srcRect,int x,int y)
{
    SDL_Rect dstRect;
    SDL_Point point = {1,1};
    int w,h;
    if (!__gt_graphics_texture)
    {
        slog("gt_graphics_render_surface_to_screen: no texture available");
        return;
    }
    if (!surface)
    {
        slog("gt_graphics_render_surface_to_screen: no surface provided");
        return;
    }
    SDL_QueryTexture(__gt_graphics_texture,
                     NULL,
                     NULL,
                     &w,
                     &h);
    /*check if resize is needed*/
    if ((surface->w > w)||(surface->h > h))
    {
        SDL_DestroyTexture(__gt_graphics_texture);
        __gt_graphics_texture = SDL_CreateTexture(__gt_graphics_renderer,
                                                   __gt_graphics_surface->format->format,
                                                   SDL_TEXTUREACCESS_STREAMING, 
                                                   surface->w,
                                                   surface->h);
        if (!__gt_graphics_texture)
        {
            slog("gt_graphics_render_surface_to_screen: failed to allocate more space for the screen texture!");
            return;
        }
    }
    SDL_SetTextureBlendMode(__gt_graphics_texture,SDL_BLENDMODE_BLEND);        
    SDL_UpdateTexture(__gt_graphics_texture,
                      &srcRect,
                      surface->pixels,
                      surface->pitch);
    dstRect.x = x;
    dstRect.y = y;
    dstRect.w = srcRect.w;
    dstRect.h = srcRect.h;
    SDL_RenderCopy(__gt_graphics_renderer,
                     __gt_graphics_texture,
                     &srcRect,
                     &dstRect);
}


void ResetBuffer()
{
    
}

void graphics_frame_delay()
{
    Uint32 diff;
    gt_graphics_then = gt_graphics_now;
    gt_graphics_now = SDL_GetTicks();
    diff = (gt_graphics_now - gt_graphics_then);
    if (diff < gt_graphics_frame_delay)
    {
        SDL_Delay(gt_graphics_frame_delay - diff);
    }
    gt_graphics_fps = 1000.0/MAX(SDL_GetTicks() - gt_graphics_then,0.001);
    if (gt_graphics_print_fps)
    {
        slog("FPS: %f",gt_graphics_fps);
    }
}

void NextFrame()
{
  SDL_RenderPresent(__gt_graphics_renderer);
  graphics_frame_delay();
}

void gt_graphics_close()
{
    if (__gt_graphics_texture)
    {
        SDL_DestroyTexture(__gt_graphics_texture);
    }
    if (__gt_graphics_renderer)
    {
        SDL_DestroyRenderer(__gt_graphics_renderer);
    }
    if (__gt_graphics_main_window)
    {
        SDL_DestroyWindow(__gt_graphics_main_window);
    }
    if (__gt_graphics_surface)
    {
        SDL_FreeSurface(__gt_graphics_surface);
    }
    if (__gt_graphics_temp_buffer)
    {
        SDL_FreeSurface(__gt_graphics_temp_buffer);
    }
    __gt_graphics_surface = NULL;
    __gt_graphics_main_window = NULL;
    __gt_graphics_renderer = NULL;
    __gt_graphics_texture = NULL;
    __gt_graphics_temp_buffer = NULL;
}

SDL_Renderer *gt_graphics_get_active_renderer()
{
    return __gt_graphics_renderer;
}

Uint32 gt_graphics_get_system_time()
{
    return gt_graphics_now;
}
