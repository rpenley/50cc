#include <SDL3/SDL.h>

// Application
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define PLATFORM_COUNT 10

// Physics
#define GRAVITY 0.6f
#define JUMP_FORCE -16.0f
#define MOVE_SPEED 6.0f
#define MAX_FALL_SPEED 15.0f
#define ACCELERATION 1.0f
#define DECELERATION 0.8f
#define AIR_CONTROL 0.6f

// Globals
SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gHelloWorld = NULL;

typedef struct {
    float x, y;
    float target_x, target_y;
} Camera;

typedef enum {
    ENTITY_PLAYER,
    ENTITY_PLATFORM,
    // Add more types as needed
} EntityType;

typedef struct {
    EntityType type;
    float x, y;
    float width, height;
    // Common entity properties

    // Type-specific data pointers
    union {
        struct {
            float vel_x, vel_y;
            bool is_jumping;
            bool is_grounded;
        } player;

        struct {
            // Platform-specific data
        } platform;
    } data;
} Entity;

typedef struct {
    float x, y;
    float width, height;
} Platform;

typedef struct {
    float x, y;
    float width, height;
    float vel_x, vel_y;
    bool is_jumping;
    bool is_grounded;
} Player;

bool init() {
    bool success = SDL_Init(SDL_INIT_VIDEO);
    if (!success) {
        SDL_Log("Initialization failed! Cuz: %s\n", SDL_GetError());
    }

    return success;
}

bool makeWindow() {
    const int kScreenWidth = 640;
    const int kScreenHeight = 480;

    gWindow = SDL_CreateWindow("SDL3 Tutorial: Hello SDL3!", kScreenWidth, kScreenHeight, 0);
    if (gWindow == NULL) {
        SDL_Log("Window creation failed: %s\n", SDL_GetError());
        return false;
    }

    // get draw surface
    gScreenSurface = SDL_GetWindowSurface(gWindow);

    return true;
}

bool loadMedia() {
    char* imagePath = "hello.bmp";

    gHelloWorld = SDL_LoadBMP(imagePath);

    if (gHelloWorld == NULL) {
        SDL_Log("Unable to load image %s! Cuz: %s\n", imagePath, SDL_GetError());
        return false;
    }

    return true;
}

void end() {
    // Destroy surface
    SDL_DestroySurface(gHelloWorld);
    gHelloWorld = NULL;

    // Destroy window
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gScreenSurface = NULL;

    SDL_Quit();
}

void loop() {
    // Main game loop
    bool quit = false;
    SDL_Event e;
    SDL_zero(e); // ensure that we're dealing with zeroed out memory

    while (!quit) {
        // start by processing events
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }

        // then, perform updates
        SDL_FillSurfaceRect(gScreenSurface,
                            NULL,
                            SDL_MapSurfaceRGB(gScreenSurface, 0xFF, 0xFF, 0xFF));

        SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);

        // update the surface
        SDL_UpdateWindowSurface(gWindow);
    }
}

void init_camera(Camera* camera, Player* player) {
    camera->x = player->x - WINDOW_WIDTH / 2;
    camera->y = player->y - WINDOW_HEIGHT / 2;
    camera->target_x = camera->x;
    camera->target_y = camera->y;
}

void update_camera(Camera* camera, Player* player) {
    // Set target to keep player centered
    camera->target_x = player->x - WINDOW_WIDTH / 2 + player->width / 2;
    camera->target_y = player->y - WINDOW_HEIGHT / 2 + player->height / 2;

    // Smoothly interpolate camera position (lerp)
    const float lerp_factor = 0.1f;
    camera->x += (camera->target_x - camera->x) * lerp_factor;
    camera->y += (camera->target_y - camera->y) * lerp_factor;
}

void init_platforms(Platform* platforms) {
    // Ground
    platforms[0].x = 0.0f;
    platforms[0].y = 500.0f;
    platforms[0].width = 500.0f;
    platforms[0].height = 40.0f;

    // Scattered platforms of different sizes
    platforms[1].x = 600.0f;
    platforms[1].y = 500.0f;
    platforms[1].width = 200.0f;
    platforms[1].height = 40.0f;

    platforms[2].x = 900.0f;
    platforms[2].y = 400.0f;
    platforms[2].width = 150.0f;
    platforms[2].height = 30.0f;

    platforms[3].x = 1150.0f;
    platforms[3].y = 300.0f;
    platforms[3].width = 100.0f;
    platforms[3].height = 30.0f;

    platforms[4].x = 1350.0f;
    platforms[4].y = 400.0f;
    platforms[4].width = 200.0f;
    platforms[4].height = 30.0f;

    platforms[5].x = 1650.0f;
    platforms[5].y = 350.0f;
    platforms[5].width = 120.0f;
    platforms[5].height = 30.0f;

    platforms[6].x = 1850.0f;
    platforms[6].y = 280.0f;
    platforms[6].width = 100.0f;
    platforms[6].height = 30.0f;

    platforms[7].x = 400.0f;
    platforms[7].y = 350.0f;
    platforms[7].width = 120.0f;
    platforms[7].height = 30.0f;

    platforms[8].x = 200.0f;
    platforms[8].y = 250.0f;
    platforms[8].width = 150.0f;
    platforms[8].height = 30.0f;

    platforms[9].x = 100.0f;
    platforms[9].y = 400.0f;
    platforms[9].width = 100.0f;
    platforms[9].height = 30.0f;
}

bool check_collision(float x1, float y1, float w1, float h1, float x2, float y2,
                     float w2, float h2) {
    return (x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2);
}

void init_player(Player* player) {
    player->x = 100.0f;
    player->y = 100.0f;
    player->width = 32.0f;
    player->height = 32.0f;
    player->vel_x = 0.0f;
    player->vel_y = 0.0f;
    player->is_jumping = false;
    player->is_grounded = false;
}

void update_player(Player* player, Platform* platforms,
                   const Uint8* keyboard_state) {
    // Store previous grounded state for jump detection
    bool was_grounded = player->is_grounded;

    // Reset grounded state - will be set true if standing on something
    player->is_grounded = false;

    // Apply gravity
    player->vel_y += GRAVITY;
    if (player->vel_y > MAX_FALL_SPEED) {
        player->vel_y = MAX_FALL_SPEED;
    }

    // Calculate horizontal movement with acceleration and deceleration
    bool moving = false;
    float acceleration_factor = player->is_grounded ? 1.0f : AIR_CONTROL;

    if (keyboard_state[SDL_SCANCODE_A] || keyboard_state[SDL_SCANCODE_LEFT]) {
        player->vel_x -= ACCELERATION * acceleration_factor;
        if (player->vel_x < -MOVE_SPEED) {
            player->vel_x = -MOVE_SPEED;
        }
        moving = true;
    }

    if (keyboard_state[SDL_SCANCODE_D] || keyboard_state[SDL_SCANCODE_RIGHT]) {
        player->vel_x += ACCELERATION * acceleration_factor;
        if (player->vel_x > MOVE_SPEED) {
            player->vel_x = MOVE_SPEED;
        }
        moving = true;
    }

    // Apply deceleration when not actively moving
    if (!moving) {
        if (player->vel_x > 0) {
            player->vel_x -= DECELERATION;
            if (player->vel_x < 0)
                player->vel_x = 0;
        } else if (player->vel_x < 0) {
            player->vel_x += DECELERATION;
            if (player->vel_x > 0)
                player->vel_x = 0;
        }
    }

    // Store original position for collision resolution
    float original_x = player->x;
    float original_y = player->y;

    // Apply both velocities
    player->x += player->vel_x;
    player->y += player->vel_y;

    // Check for horizontal collisions first
    for (int i = 0; i < PLATFORM_COUNT; i++) {
        if (check_collision(player->x, original_y, player->width, player->height,
                            platforms[i].x, platforms[i].y, platforms[i].width,
                            platforms[i].height)) {
            // Resolve horizontal collision
            if (player->vel_x > 0) {
                player->x = platforms[i].x - player->width;
            } else if (player->vel_x < 0) {
                player->x = platforms[i].x + platforms[i].width;
            }
            player->vel_x = 0;
        }
    }

    // Check for vertical collisions
    for (int i = 0; i < PLATFORM_COUNT; i++) {
        if (check_collision(player->x, player->y, player->width, player->height,
                            platforms[i].x, platforms[i].y, platforms[i].width,
                            platforms[i].height)) {
            // Resolve vertical collision
            if (player->vel_y > 0) { // Falling down
                player->y = platforms[i].y - player->height;
                player->is_grounded = true;
                player->vel_y = 0;
            } else if (player->vel_y < 0) { // Moving up
                player->y = platforms[i].y + platforms[i].height;
                player->vel_y = 0;
            }
        }
    }

    // Only reset jumping state when landing
    if (player->is_grounded && !was_grounded) {
        player->is_jumping = false;
    }

    // Jumping - only allow if grounded and not already jumping
    if ((keyboard_state[SDL_SCANCODE_W] ||
         keyboard_state[SDL_SCANCODE_UP] ||
         keyboard_state[SDL_SCANCODE_SPACE]) &&
        player->is_grounded && !player->is_jumping) {
        player->vel_y = JUMP_FORCE;
        player->is_jumping = true;
        player->is_grounded = false; // No longer grounded as soon as we jump
    }

    // Variable jump height (if jump key released during upward movement)
    if (!(keyboard_state[SDL_SCANCODE_W] ||
          keyboard_state[SDL_SCANCODE_UP] ||
          keyboard_state[SDL_SCANCODE_SPACE]) &&
        player->vel_y < 0 && player->is_jumping) {
        player->vel_y *= 0.5f; // Cut the jump short
    }
}

void render(SDL_Renderer* renderer, Player* player, Platform* platforms,
            Camera* camera) {

    // Clear screen
    SDL_SetRenderDrawColor(renderer, 25, 25, 25, 255);
    SDL_RenderClear(renderer);

    // Draw platforms
    SDL_SetRenderDrawColor(renderer, 100, 200, 100, 255);
    for (int i = 0; i < PLATFORM_COUNT; i++) {
        SDL_FRect rect = {platforms[i].x - camera->x, platforms[i].y - camera->y,
                          platforms[i].width, platforms[i].height};
        SDL_RenderFillRect(renderer, &rect);
    }

    // Draw player
    SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
    SDL_FRect player_rect = {player->x - camera->x, player->y - camera->y,
                             player->width, player->height};
    SDL_RenderFillRect(renderer, &player_rect);

    // Present renderer
    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n",
                SDL_GetError());
        return 1;
    }

    // Create window
    SDL_Window* window =
        SDL_CreateWindow("Platformer Game", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!window) {
        fprintf(stderr, "Window could not be created! SDL_Error: %s\n",
                SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n",
                SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Enable VSync
    SDL_SetRenderVSync(renderer, 1);

    // Initialize game objects
    Player player;
    Platform platforms[PLATFORM_COUNT];
    Camera camera;

    init_player(&player);
    init_platforms(platforms);
    init_camera(&camera, &player);

    // Main loop
    bool quit = false;
    SDL_Event e;
    Uint64 last_time = SDL_GetTicks();
    const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);

    while (!quit) {
        // Process events
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
            if (e.type == SDL_EVENT_KEY_DOWN && e.key.scancode == SDLK_ESCAPE) {
                quit = true;
            }
        }

        // Update game state
        update_player(&player, platforms, keyboard_state);
        update_camera(&camera, &player);

        // Render
        render(renderer, &player, platforms, &camera);

        // Cap the frame rate
        Uint64 current_time = SDL_GetTicks();
        Uint64 elapsed = current_time - last_time;
        if (elapsed < 16) {
            SDL_Delay(16 - elapsed);
        }
        last_time = SDL_GetTicks();
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
