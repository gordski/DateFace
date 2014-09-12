#include <pebble.h>

Window *my_window;
TextLayer *week_day_layer;
TextLayer *day_of_month_layer;
InverterLayer *inverter_layer;

const char *day_of_month_lookup[] = {
  "first",
  "second",
  "third",
  "fourth",
  "fifth",
  "sixth",
  "seventh",
  "eighth",
  "nineth",
  "tenth",
  "eleventh",
  "twelfth",
  "thirteenth",
  "fourteenth",
  "fifteenth",
  "sixteenth",
  "seventeenth",
  "eighteenth",
  "nineteenth",
  "twentieth",
  "twenty\nfirst",
  "twenty\nsecond",
  "twenty\nthird",
  "twenty\nfourth",
  "twenty\nfifth",
  "twenty\nsixth",
  "twenty\nseventh",
  "twenty\neighth",
  "twenty\nnineth",
  "thirtieth",
  "thirty\nfirst"
};

char week_day[4];
const char *day_of_month;

void set_strings(struct tm *t) {
  strftime(week_day, sizeof(week_day), "%a", t);
  week_day[0] = week_day[0] + ('a' - 'A');
  
  day_of_month = day_of_month_lookup[t->tm_mday-1];
}

void render() {
  // Week Day
  text_layer_set_text(week_day_layer, week_day);
  text_layer_set_font(week_day_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
	text_layer_set_text_alignment(week_day_layer, GTextAlignmentLeft);
  
  // Day of Month
  text_layer_set_text(day_of_month_layer, day_of_month);
  text_layer_set_font(day_of_month_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
	text_layer_set_text_alignment(day_of_month_layer, GTextAlignmentLeft);

}

void handle_time_change(struct tm *t, TimeUnits units) {
  set_strings(t);
  render();
}

void handle_init(void) {
  my_window = window_create();

  week_day_layer = text_layer_create(GRect(0, 10, 144, 42));
  layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(week_day_layer));

  day_of_month_layer = text_layer_create(GRect(0,52,144,95));
  layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(day_of_month_layer));
  
  time_t now = time(NULL);
  set_strings(localtime(&now));
  render();
  
  inverter_layer = inverter_layer_create(GRect(0, 0, 144, 168));
	layer_add_child(window_get_root_layer(my_window), inverter_layer_get_layer(inverter_layer));
  
  window_stack_push(my_window, true);

  tick_timer_service_subscribe(DAY_UNIT, handle_time_change);
}

void handle_deinit(void) {
  text_layer_destroy(week_day_layer);
  text_layer_destroy(day_of_month_layer);
  inverter_layer_destroy(inverter_layer);
  window_destroy(my_window);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
