#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/mouse.h>

//rozmiary ekranu
const int screen_width = 950;
const int screen_higth = 540;
//new
#define blockWidth  70 //szerokosc blokow
#define blockHeight  20 //wysokosc blokow
#define ROWS 5
#define COLS 13
#define BLOCK_SPACING 2;
//new
//new
typedef struct {
	int x, y; // pozycja klocka
	int w, h; // wymiary klocka
	int destroyed; // czy klocek zostal zniszczony
} Block;

typedef struct {
	Block blocks[ROWS][COLS]; // tablica klockow w planszy
} Board;
//new
void build_block() {
	Board board;
	for (int row = 0; row < ROWS; row++) {
		for (int col = 0; col < COLS; col++) {
			Block* block = &board.blocks[row][col];
			block->x = (col * (blockWidth + 2) + 7);
			block->y = (row * (blockHeight + 2) + 40);
			block->w = blockWidth;
			block->h = blockHeight;
			block->destroyed = false;
		}
	}
	for (int row = 0; row < ROWS; row++) {
		for (int col = 0; col < COLS; col++) {
			Block* block = &board.blocks[row][col];
			if (!block->destroyed) {
				al_draw_filled_rectangle(block->x, block->y,
					block->x + block->w, block->y + block->h,
					al_map_rgb(215, 93, 93));
			}
		}
	}
}
int main() {
	//deklaracja wszystkiego na swiecie
	al_init();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_keyboard();
	al_install_mouse();

	// typowe zmienne
	bool running = true;
	float x = 0 , y = 0;
	bool key;
	ALLEGRO_DISPLAY* display = al_create_display(screen_width, screen_higth);
	ALLEGRO_FONT* font = al_load_ttf_font("Raleway.ttf", 24, 0);
	ALLEGRO_FONT* easter_egg = al_load_ttf_font("Raleway.ttf", 12, 0);
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60);
	ALLEGRO_BITMAP* bitmap = NULL;

	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_timer_event_source(timer));
	al_register_event_source(queue, al_get_mouse_event_source());
	al_init_image_addon();
	al_set_window_title(display, "Arkanoid");

	// dodajemy element platformy
	bitmap = al_load_bitmap("platform.jpg");
	assert(bitmap != NULL);
	// cos tam prymitywne
	al_init_primitives_addon();

	// poczatek dzialania
	al_start_timer(timer);
	puts("Game started");
	while (running)
	{
		ALLEGRO_EVENT event;
		al_wait_for_event(queue, &event);
		//jak wylaczyc program?
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE || event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			running = false;

		// mysz
		if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
			x = event.mouse.x;
			
		}

		if (event.type == ALLEGRO_EVENT_TIMER) {
			al_clear_to_color(al_map_rgb(255, 255, 255));
			al_draw_text(easter_egg, al_map_rgb(203, 203, 203), 0, 0, 0, "made by Michal Mlodawski");
			al_draw_bitmap(bitmap, x, 500, 0);
			build_block();
			al_draw_text(font, al_map_rgb(0, 0, 0), 415, 0, 0, "Arkanoid!");
			al_flip_display();
			al_hide_mouse_cursor(display);
		}
		
	}
	//koniec gry
	puts("Game over");
	// usuwamy wszystko
	al_destroy_display(display);
	al_destroy_font(font);
	al_uninstall_keyboard();
	al_destroy_timer(timer);
	al_destroy_bitmap(bitmap);

	al_uninstall_mouse();
	return 0;
}