#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0xA3, 0x86, 0x84, 0xD7, 0x77, 0x86, 0x43, 0xE5, 0xB2, 0xA4, 0x7F, 0x90, 0xED, 0xB0, 0xC8, 0x3C }
PBL_APP_INFO(MY_UUID,
             "Stopwatch App", "daharro",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_STANDARD_APP);

Window window;
TextLayer timeLayer;
TextLayer stopWatchLayer;

static char hourText[] = "00:00"; //variable for storing time
static char stopWatchText[] = "00:00:00";
int elapsedSeconds = 0;

void setTime(PblTm *t) {
 
	//If user selects '24hr' in Settings on the watch
	if(clock_is_24h_style())
		string_format_time(hourText, sizeof(hourText), "%H:%M", t);
	else
		string_format_time(hourText, sizeof(hourText), "%I:%M", t);
	//Set the TextLayer text
	
	text_layer_set_text(&timeLayer, hourText);
	
}


void setStopWatch(elapsedTime) {

//elapsed time into human readable
text_layer_set_text(&stopWatchLayer, elapsedTime);
}

void handle_second_tick(AppContextRef ctx, PebbleTickEvent *t)
{
	(void)ctx;
	PblTm time;
	get_time(&time);
	int seconds = time.tm_sec;

	if (seconds == 0)
		setTime(t->tick_time);

	elapsedSeconds++;
	setStopWatch(elapsedSeconds)
}

void handle_init(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "StopWatch");
  window_stack_push(&window, true /* Animated */);
	window_set_background_color(&window, GColorBlack);
	
	//text layers
	//Time layer
	text_layer_init(&timeLayer, GRect(30, 30, 150, 50));
	text_layer_set_background_color(&timeLayer, GColorClear);
	text_layer_set_text_color(&timeLayer, GColorWhite);
	text_layer_set_font(&timeLayer,
	fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
	text_layer_set_text_alignment(&timeLayer, GTextAlignmentLeft);

	//Stopwatch layer
	text_layer_init(&stopWatchLayer, GRect(30, 65, 150, 50));
	text_layer_set_background_color(&stopWatchLayer, GColorClear);
	text_layer_set_text_color(&stopWatchLayer, GColorWhite);
	text_layer_set_font(&stopWatchLayer,
	fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
	text_layer_set_text_alignment(&stopWatchLayer, GTextAlignmentLeft);
	
	
	//Add to window
	layer_add_child(&window.layer, &timeLayer.layer);
	layer_add_child(&window.layer, &stopWatchLayer.layer);
	//Set initial time so display isn't blank
	PblTm time;
	get_time(&time);
	setTime(&time);
	setStopWatch(elapsedSeconds);
}





void pbl_main(void *params) {
	PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
	.tick_info = {
		.tick_handler = &handle_second_tick,
		.tick_units = SECOND_UNIT
	}		
  };
  app_event_loop(params, &handlers);
}
