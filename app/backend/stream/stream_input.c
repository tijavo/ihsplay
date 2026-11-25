#include "app.h"
#include "ui/app_ui.h"

#include "stream_input.h"
#include "stream_manager_internal.h"
#include "backend/input_manager.h"

bool stream_input_handle_key_event(stream_manager_t *manager, const SDL_KeyboardEvent *event) {
#ifdef __WEBOS__
    SDL_Keysym keysym = event->keysym;
    switch ((int) keysym.scancode) {
        case SDL_SCANCODE_WEBOS_EXIT: {
            if (event->state == SDL_RELEASED) {
                stream_manager_set_overlay_opened(manager, true);
            }
            return true;
        }
        case SDL_SCANCODE_WEBOS_BACK: {
            // TODO: Send ESC key
            return true;
        }
    }
#endif
    if (event->keysym.sym == SDLK_ESCAPE) {
        if (event->state == SDL_RELEASED) {
            stream_manager_set_overlay_opened(manager, true);
        }
        return true;
    }
    if (!manager->app->settings->enable_input) {
        return true;
    }
    if (event->state == SDL_PRESSED) {
        IHS_SessionSendKeyDown(manager->session, event->keysym.scancode);
    } else {
        IHS_SessionSendKeyUp(manager->session, event->keysym.scancode);
    }
    return true;
}

/**
 * Map a window position to the host's capture area (0..1 on both axes), accounting for the letterboxing
 * applied when the stream's aspect ratio differs from the screen's.
 */
static void stream_input_map_to_capture(stream_manager_t *manager, int wx, int wy, float *x, float *y) {
    int vw = manager->viewport_width, vh = manager->viewport_height;
    if (vw <= 0 || vh <= 0) {
        SDL_GetWindowSize(manager->app->ui->window, &vw, &vh);
    }
    float dst_x = 0, dst_y = 0, dst_w = (float) vw, dst_h = (float) vh;
    if (manager->capture_width > 0 && manager->capture_height > 0) {
        float scale = SDL_min((float) vw / (float) manager->capture_width,
                              (float) vh / (float) manager->capture_height);
        dst_w = (float) manager->capture_width * scale;
        dst_h = (float) manager->capture_height * scale;
        dst_x = ((float) vw - dst_w) / 2.0f;
        dst_y = ((float) vh - dst_h) / 2.0f;
    }
    *x = SDL_clamp(((float) wx - dst_x) / dst_w, 0.0f, 1.0f);
    *y = SDL_clamp(((float) wy - dst_y) / dst_h, 0.0f, 1.0f);
}

bool stream_input_handle_mouse_event(stream_manager_t *manager, const SDL_Event *event) {
    if (!manager->app->settings->enable_input) {
        return true;
    }
    switch (event->type) {
        case SDL_MOUSEMOTION: {
            if (input_manager_get_and_reset_mouse_movement(manager->app->input_manager)) {
                break;
            }
            if (manager->app->settings->relmouse) {
                IHS_SessionSendMouseMovement(manager->session, event->motion.xrel, event->motion.yrel);
            } else {
                float x, y;
                stream_input_map_to_capture(manager, event->motion.x, event->motion.y, &x, &y);
                IHS_SessionSendMousePosition(manager->session, x, y);
            }
            return true;
        }
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP: {
            IHS_StreamInputMouseButton button = 0;
            switch (event->button.button) {
                case SDL_BUTTON_LEFT:
                    button = IHS_MOUSE_BUTTON_LEFT;
                    break;
                case SDL_BUTTON_RIGHT:
                    button = IHS_MOUSE_BUTTON_RIGHT;
                    break;
                case SDL_BUTTON_MIDDLE:
                    button = IHS_MOUSE_BUTTON_MIDDLE;
                    break;
                case SDL_BUTTON_X1:
                    button = IHS_MOUSE_BUTTON_X1;
                    break;
                case SDL_BUTTON_X2:
                    button = IHS_MOUSE_BUTTON_X2;
                    break;
            }
            if (button != 0) {
                if (event->button.state == SDL_RELEASED) {
                    IHS_SessionSendMouseUp(manager->session, button);
                } else {
                    IHS_SessionSendMouseDown(manager->session, button);
                }
            }
            return true;
        }
        case SDL_MOUSEWHEEL: {
            Sint32 x = event->wheel.x, y = event->wheel.y;
            if (event->wheel.direction == SDL_MOUSEWHEEL_FLIPPED) {
                x *= -1;
                y *= -1;
            }
            if (x != 0) {
                IHS_SessionSendMouseWheel(manager->session, x < 0 ? IHS_MOUSE_WHEEL_LEFT : IHS_MOUSE_WHEEL_RIGHT);
            }
            if (y != 0) {
                IHS_SessionSendMouseWheel(manager->session, y > 0 ? IHS_MOUSE_WHEEL_UP : IHS_MOUSE_WHEEL_DOWN);
            }
            return true;
        }
    }
    return false;
}