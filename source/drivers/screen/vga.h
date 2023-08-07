#ifndef VGA_H_
#define VGA_H_
#include <stdbool.h>
#include <stdint.h>

#define VGA3 ((vga3_cell_t*)0xB8000)
#define VGA3_ROW (24)
#define VGA3_COL (80)

typedef enum {
	VGA3_BLACK,
	VGA3_BLUE,
	VGA3_GREEN,
	VGA3_CYAN,
	VGA3_RED,
	VGA3_MAGENTA,
	VGA3_BROWN,
	VGA3_LIGHT_GREY,
	VGA3_DARK_GREY,
	VGA3_LIGHT_BLUE,
	VGA3_LIGHT_GREEN,
	VGA3_LIGHT_CYAN,
	VGA3_LIGHT_RED,
	VGA3_LIGHT_MAGENTA,
	VGA3_WHITE,
} vga3_color_t;

typedef struct {
	char symbol;
	uint8_t color;
} vga3_cell_t;

typedef struct {
	uint8_t row;
	uint8_t col;
} vga3_cursor_t;

#define VGA3_FG(S) ((S) << 0)
#define VGA3_BG(S) ((S) << 4)
#define VGA3_AT(R, C) VGA3[(C)+(R)*VGA3_ROW]

void vga3_clear(void);
void vga3_scroll(uint8_t row);

void vga3_cursor(bool enable);
void vga3_cursor_set(vga3_cursor_t cursor);
vga3_cursor_t vga3_cursor_get(void);

#endif//VGA_H_
