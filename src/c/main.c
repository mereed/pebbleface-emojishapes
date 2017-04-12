/*

Copyright (C) 2017 Mark Reed

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

-------------------------------------------------------------------

*/

#include <pebble.h>
#include "main.h"
#include "hands.h"

#define ANTIALIASING true

//WINDOW
Window* window;

//LAYER
Layer* layer;

//BACKGROUND
GBitmap* background;
static BitmapLayer *background_layer;

//TOP MARKER
static GBitmap* topm;
static BitmapLayer *topm_layer;

//SETTINGS
bool seconds = true;
bool date = true;

GBitmap *img_battery_8;
GBitmap *img_battery_7;
GBitmap *img_battery_6;
GBitmap *img_battery_5;
GBitmap *img_battery_4;
GBitmap *img_battery_3;
GBitmap *img_battery_2;
GBitmap *img_battery_1;
GBitmap *img_battery_charge;
BitmapLayer *layer_batt_img;

int charge_percent = 0;
int cur_day = -1;

static GBitmap *bluetooth_image;
static BitmapLayer *bluetooth_layer;    

// A struct for our specific settings (see main.h)
ClaySettings settings;

int stringToInt(char *str);

static GPath *s_minute_arrow, *s_hour_arrow, *s_second_arrow;


// Initialize the default settings
static void prv_default_settings() {	
  settings.background = 0;
  settings.topmarker = 0;
  settings.date = false;
  settings.secs = false;
  settings.batt = true;
  settings.hrcol = GColorWhite;
  settings.mincol = GColorWhite;
  settings.seccol = GColorWhite;
  settings.dcol = GColorWhite;
  settings.dtext = GColorBlack;
  settings.bgcol = GColorBlack;
}

static void tick(struct tm *tick_time, TimeUnits units_changed);

void update_layer(Layer *me, GContext* ctx) 
{
	//watchface drawing
	
	char text[8];

	  graphics_context_set_antialiased(ctx, ANTIALIASING);

	//get tick_time
	time_t temp = time(NULL); 
  	struct tm *tick_time = localtime(&temp);
	
	// get date - number
	strftime(text, 8, "%d", tick_time);
	graphics_context_set_text_color(ctx, settings.dtext);
	graphics_context_set_fill_color(ctx, settings.dcol);
	
	//draw hands
#ifdef PBL_PLATFORM_CHALK
	GPoint center = GPoint(90,90);
#else
	GPoint center = GPoint(72,84);
#endif
	
	if((settings.date) && (date == 1)) {
#ifdef PBL_PLATFORM_CHALK
		graphics_fill_circle(ctx, center, 10);
	  graphics_draw_text(ctx, text, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD), GRect(77,81,20,15), GTextOverflowModeTrailingEllipsis, GTextAlignmentRight, NULL);
#else
		graphics_fill_circle(ctx,center, 10);
	  graphics_draw_text(ctx, text, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD), GRect(58,75,20,15), GTextOverflowModeTrailingEllipsis, GTextAlignmentRight, NULL);
#endif
	} else {
		
	}
	
  graphics_context_set_stroke_width(ctx, 2);	

	if((settings.secs) && (seconds == 1)) {
	
		tick_timer_service_subscribe(SECOND_UNIT, tick);
		gpath_rotate_to(s_second_arrow, TRIG_MAX_ANGLE * tick_time->tm_sec / 60);
		graphics_context_set_stroke_width(ctx, 2);
		graphics_context_set_stroke_color(ctx, settings.seccol);
		gpath_draw_outline(ctx, s_second_arrow);
				
	} else {
		tick_timer_service_subscribe(MINUTE_UNIT, tick);
	}		
	
	// minute hand
	graphics_context_set_stroke_color(ctx, settings.mincol);
	gpath_rotate_to(s_minute_arrow, TRIG_MAX_ANGLE * tick_time->tm_min / 60);
	gpath_draw_outline(ctx, s_minute_arrow);

	// hour hand
	graphics_context_set_stroke_color(ctx, settings.hrcol);
	gpath_rotate_to(s_hour_arrow, (TRIG_MAX_ANGLE * (((tick_time->tm_hour % 12) * 6) + (tick_time->tm_min / 10))) / (12 * 6)); // from Pebble SDK example
	gpath_draw_outline(ctx, s_hour_arrow);
	
}

// Read settings from persistent storage
static void prv_load_settings() {
  // Load the default settings
  prv_default_settings();
  // Read settings from persistent storage, if they exist
  persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
}

// Save the settings to persistent storage
static void prv_save_settings() {
  persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
  // Update the display based on new settings
  prv_update_display();
}

// Update the display elements
static void prv_update_display() {
	
	if (settings.batt){
		  layer_set_hidden(bitmap_layer_get_layer(layer_batt_img), false);
	} else {
		  layer_set_hidden(bitmap_layer_get_layer(layer_batt_img), true);		
	}
	
	switch(settings.background) {
		
	case 0:  // hour nums

	    if (background) {
		gbitmap_destroy(background);
		background = NULL;
	    }
		background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
		
 	    if (background != NULL) {
		bitmap_layer_set_bitmap(background_layer, background);
		layer_set_hidden(bitmap_layer_get_layer(background_layer), false);
		layer_mark_dirty(bitmap_layer_get_layer(background_layer));
        }

	    break;		
		
	case 1:  // min nums

	    if (background) {
		gbitmap_destroy(background);
		background = NULL;
	    }
		background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND1);
		
 	    if (background != NULL) {
		bitmap_layer_set_bitmap(background_layer, background);
		layer_set_hidden(bitmap_layer_get_layer(background_layer), false);
		layer_mark_dirty(bitmap_layer_get_layer(background_layer));
        }
		
	case 2:  // min and hour nums

	    if (background) {
		gbitmap_destroy(background);
		background = NULL;
	    }
		background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND8);
		
 	    if (background != NULL) {
		bitmap_layer_set_bitmap(background_layer, background);
		layer_set_hidden(bitmap_layer_get_layer(background_layer), false);
		layer_mark_dirty(bitmap_layer_get_layer(background_layer));
        }
	    break;		
			
	case 3:  // blank

	    if (background) {
		gbitmap_destroy(background);
		background = NULL;
	    }
		background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BLANK);
		
 	    if (background != NULL) {
		bitmap_layer_set_bitmap(background_layer, background);
		layer_set_hidden(bitmap_layer_get_layer(background_layer), false);
		layer_mark_dirty(bitmap_layer_get_layer(background_layer));
        }
		window_set_background_color(window,settings.bgcol);

		break;		
			
	case 4:  // min and hour dots
	
	    if (background) {
		gbitmap_destroy(background);
		background = NULL;
	    }
		background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND3);
		
 	    if (background != NULL) {
		bitmap_layer_set_bitmap(background_layer, background);
		layer_set_hidden(bitmap_layer_get_layer(background_layer), false);
		layer_mark_dirty(bitmap_layer_get_layer(background_layer));
        }
	    break;	
		
	case 5:  // lines
	
	    if (background) {
		gbitmap_destroy(background);
		background = NULL;
	    }
		background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND4);
		
 	    if (background != NULL) {
		bitmap_layer_set_bitmap(background_layer, background);
		layer_set_hidden(bitmap_layer_get_layer(background_layer), false);
		layer_mark_dirty(bitmap_layer_get_layer(background_layer));
        }
	    break;	
		
	case 6:  // lines
	
	    if (background) {
		gbitmap_destroy(background);
		background = NULL;
	    }
		background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND5);
		
 	    if (background != NULL) {
		bitmap_layer_set_bitmap(background_layer, background);
		layer_set_hidden(bitmap_layer_get_layer(background_layer), false);
		layer_mark_dirty(bitmap_layer_get_layer(background_layer));
        }
	    break;	
		
	case 7:  // rings
	
	    if (background) {
		gbitmap_destroy(background);
		background = NULL;
	    }
		background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND6);
		
 	    if (background != NULL) {
		bitmap_layer_set_bitmap(background_layer, background);
		layer_set_hidden(bitmap_layer_get_layer(background_layer), false);
		layer_mark_dirty(bitmap_layer_get_layer(background_layer));
        }
	    break;	

	case 8:  // lines and min dots
	
	    if (background) {
		gbitmap_destroy(background);
		background = NULL;
	    }
		background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND7);
		
 	    if (background != NULL) {
		bitmap_layer_set_bitmap(background_layer, background);
		layer_set_hidden(bitmap_layer_get_layer(background_layer), false);
		layer_mark_dirty(bitmap_layer_get_layer(background_layer));
        }
	    break;	
	}
	
	
	switch(settings.topmarker) {
		
	case 0:  // pebble

	    if (topm) {
		gbitmap_destroy(topm);
		topm = NULL;
	    }
		topm = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PEBBLE);
		
 	    if (topm != NULL) {
		bitmap_layer_set_bitmap(topm_layer, topm);
		layer_set_hidden(bitmap_layer_get_layer(topm_layer), false);
		layer_mark_dirty(bitmap_layer_get_layer(topm_layer));
        }
#ifdef PBL_PLATFORM_CHALK
		layer_set_frame(bitmap_layer_get_layer(topm_layer), GRect(65, 12, 50,13));
#else
		layer_set_frame(bitmap_layer_get_layer(topm_layer), GRect(48, 5, 52,13));
#endif
		break;		
		
		
	case 1:  // p

	    if (topm) {
		gbitmap_destroy(topm);
		topm = NULL;
	    }
		topm = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_P);
		
 	    if (topm != NULL) {
		bitmap_layer_set_bitmap(topm_layer, topm);
		layer_set_hidden(bitmap_layer_get_layer(topm_layer), false);
		layer_mark_dirty(bitmap_layer_get_layer(topm_layer));
        }
#ifdef PBL_PLATFORM_CHALK
		layer_set_frame(bitmap_layer_get_layer(topm_layer), GRect(86, 10, 10,13));
#else
		layer_set_frame(bitmap_layer_get_layer(topm_layer), GRect(68, 6, 10,13));
#endif
		break;		
		
	case 2:  // 0

	    if (topm) {
		gbitmap_destroy(topm);
		topm = NULL;
	    }
		topm = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_0);
		
 	    if (topm != NULL) {
		bitmap_layer_set_bitmap(topm_layer, topm);
		layer_set_hidden(bitmap_layer_get_layer(topm_layer), false);
		layer_mark_dirty(bitmap_layer_get_layer(topm_layer));
        }
#ifdef PBL_PLATFORM_CHALK
		layer_set_frame(bitmap_layer_get_layer(topm_layer), GRect(84, 6, 15,15));
#else
		layer_set_frame(bitmap_layer_get_layer(topm_layer), GRect(65, 4, 15,15));
#endif
		break;		
			
	case 3:  // white square

	    if (topm) {
		gbitmap_destroy(topm);
		topm = NULL;
	    }
		topm = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_SQUARE);
		
 	    if (topm != NULL) {
		bitmap_layer_set_bitmap(topm_layer, topm);
		layer_set_hidden(bitmap_layer_get_layer(topm_layer), false);
		layer_mark_dirty(bitmap_layer_get_layer(topm_layer));
        }
#ifdef PBL_PLATFORM_CHALK
		layer_set_frame(bitmap_layer_get_layer(topm_layer), GRect(86, 9, 9,9));
#else
		layer_set_frame(bitmap_layer_get_layer(topm_layer), GRect(67, 12, 9,9));
#endif
		break;		

	case 4:  // blank

	    if (topm) {
		gbitmap_destroy(topm);
		topm = NULL;
	    }
		topm = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BLANK);
		
 	    if (topm != NULL) {
		bitmap_layer_set_bitmap(topm_layer, topm);
		layer_set_hidden(bitmap_layer_get_layer(topm_layer), false);
		layer_mark_dirty(bitmap_layer_get_layer(topm_layer));
        }
#ifdef PBL_PLATFORM_CHALK
		layer_set_frame(bitmap_layer_get_layer(topm_layer), GRect(89, 9, 2,2));
#else
		layer_set_frame(bitmap_layer_get_layer(topm_layer), GRect(72, 12, 2,2));
#endif
		break;		
	}
}

int stringToInt(char *str){
    int i=0,sum=0;
    while(str[i]!='\0'){
         if(str[i]< 48 || str[i] > 57){
            // if (DEBUG) APP_LOG(APP_LOG_LEVEL_ERROR, "Unable to convert it into integer.");
          //   return 0;
         }
         else{
             sum = sum*10 + (str[i] - 48);
             i++;
         }
    }
    return sum;
}

// Handle the response from AppMessage
static void prv_inbox_received_handler(DictionaryIterator *iter, void *context) {
	
  // face background style
  Tuple *bg_t = dict_find(iter, MESSAGE_KEY_background);
  if (bg_t) {
    settings.background = stringToInt((char*) bg_t->value->data);
  }

 // date
  Tuple *date_t = dict_find(iter, MESSAGE_KEY_date);
  if (date_t) {
    settings.date = date_t->value->int32 == 1;
  }
	
// secs
  Tuple *secs_t = dict_find(iter, MESSAGE_KEY_secs);
  if (secs_t) {
    settings.secs = secs_t->value->int32 == 1;
  }

 // batt
  Tuple *batt_t = dict_find(iter, MESSAGE_KEY_batt);
  if (batt_t) {
    settings.batt = batt_t->value->int32 == 1;
  }
	
 // Hour hand Color
  Tuple *hr_color_t = dict_find(iter, MESSAGE_KEY_hrcol);
  if (hr_color_t) {
    settings.hrcol = GColorFromHEX(hr_color_t->value->int32);
  }
	
 // Minute hand Color
  Tuple *min_color_t = dict_find(iter, MESSAGE_KEY_mincol);
  if (min_color_t) {
    settings.mincol = GColorFromHEX(min_color_t->value->int32);
  }
	
 // Second hand Color
  Tuple *sec_color_t = dict_find(iter, MESSAGE_KEY_seccol);
  if (sec_color_t) {
    settings.seccol = GColorFromHEX(sec_color_t->value->int32);
  }
	
	// date circle Color
  Tuple *date_color_t = dict_find(iter, MESSAGE_KEY_dcol);
  if (date_color_t) {
    settings.dcol = GColorFromHEX(date_color_t->value->int32);
  }
	
	// date text Color
  Tuple *datet_color_t = dict_find(iter, MESSAGE_KEY_dtext);
  if (datet_color_t) {
    settings.dtext = GColorFromHEX(datet_color_t->value->int32);
  }

	// bg canvas Color
  Tuple *bgcol_color_t = dict_find(iter, MESSAGE_KEY_bgcol);
  if (bgcol_color_t) {
    settings.bgcol = GColorFromHEX(bgcol_color_t->value->int32);
  }

	// top marker
  Tuple *tm_t = dict_find(iter, MESSAGE_KEY_topmarker);
  if (tm_t) {
    settings.topmarker = stringToInt((char*) tm_t->value->data);
  }
	
	// Save the new settings to persistent storage
  prv_save_settings();
}

void update_battery_state(BatteryChargeState charge_state) {

    if (charge_state.is_charging) {
        bitmap_layer_set_bitmap(layer_batt_img, img_battery_charge);
		
    } else {

        if (charge_state.charge_percent <= 12) {
            bitmap_layer_set_bitmap(layer_batt_img, img_battery_1);
        } else if (charge_state.charge_percent <= 25) {
            bitmap_layer_set_bitmap(layer_batt_img, img_battery_2);
		} else if (charge_state.charge_percent <= 37) {
            bitmap_layer_set_bitmap(layer_batt_img, img_battery_3);
		} else if (charge_state.charge_percent <= 50) {
            bitmap_layer_set_bitmap(layer_batt_img, img_battery_4);
		} else if (charge_state.charge_percent <= 62) {
            bitmap_layer_set_bitmap(layer_batt_img, img_battery_5);
		} else if (charge_state.charge_percent <= 75) {
            bitmap_layer_set_bitmap(layer_batt_img, img_battery_6);
		} else if (charge_state.charge_percent <= 87) {
            bitmap_layer_set_bitmap(layer_batt_img, img_battery_7);
		} else if (charge_state.charge_percent <= 100) {
            bitmap_layer_set_bitmap(layer_batt_img, img_battery_8);

		} else {
            bitmap_layer_set_bitmap(layer_batt_img, img_battery_8);
        } 
    } 
    charge_percent = charge_state.charge_percent; 
} 

static void toggle_bluetooth_icon(bool connected) {

  layer_set_hidden(bitmap_layer_get_layer(bluetooth_layer), !connected);
	  
}

void bluetooth_connection_callback(bool connected) {
  toggle_bluetooth_icon(connected);
}

void force_update(void) {
    update_battery_state(battery_state_service_peek());
    toggle_bluetooth_icon(bluetooth_connection_service_peek());
}

void tick(struct tm *tick_time, TimeUnits units_changed) {	

	//redraw every tick
	layer_mark_dirty(layer);

}
	
void init() {
	
  prv_load_settings();
	
// Listen for AppMessages
  app_message_register_inbox_received(prv_inbox_received_handler);
  app_message_open(256, 256);

// international support
  setlocale(LC_ALL, "");
	
//create window
  window = window_create();
  window_set_background_color(window,settings.bgcol);
  window_stack_push(window, true);
  Layer* window_layer = window_get_root_layer(window);	
  GRect bounds = layer_get_bounds(window_layer);

  background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
#if PBL_PLATFORM_CHALK	
  background_layer  = bitmap_layer_create(GRect(0, 0, 180, 180)); 
#else
  background_layer  = bitmap_layer_create(GRect(0, 0, 144, 168));
#endif
  bitmap_layer_set_bitmap(background_layer, background);
  bitmap_layer_set_compositing_mode(background_layer, GCompOpSet);
  layer_add_child(window_layer, bitmap_layer_get_layer(background_layer));	

	
//load top marker
  topm = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
#if PBL_PLATFORM_CHALK	
  topm_layer  = bitmap_layer_create(GRect(65, 8, 50, 13)); 
#else
  topm_layer  = bitmap_layer_create(GRect(58, 8, 50, 13));
#endif
  bitmap_layer_set_bitmap(topm_layer, topm);
  bitmap_layer_set_compositing_mode(topm_layer, GCompOpSet);
  layer_add_child(window_layer, bitmap_layer_get_layer(topm_layer));
		
	
  img_battery_8    = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_8);
  img_battery_7    = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_7);
  img_battery_6    = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_6);
  img_battery_5    = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_5);
  img_battery_4    = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_4);
  img_battery_3    = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_3);
  img_battery_2    = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_2);
  img_battery_1    = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_1);
  img_battery_charge = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_CHARGING);
		
#if PBL_PLATFORM_CHALK	
  layer_batt_img  = bitmap_layer_create(GRect(77, 153, 25, 24)); 
#else
  layer_batt_img  = bitmap_layer_create(GRect(59, 144, 25, 24));
#endif
  bitmap_layer_set_bitmap(layer_batt_img, img_battery_3);
  bitmap_layer_set_compositing_mode(layer_batt_img, GCompOpSet);
  layer_add_child(window_layer, bitmap_layer_get_layer(layer_batt_img));
	
  bluetooth_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BLUETOOTH);
  GRect bitmap_bounds_bt_on = gbitmap_get_bounds(bluetooth_image);
#if PBL_PLATFORM_CHALK	
  GRect frame_bt = GRect(86, 160, bitmap_bounds_bt_on.size.w, bitmap_bounds_bt_on.size.h); 
#else
  GRect frame_bt = GRect(68, 152, bitmap_bounds_bt_on.size.w, bitmap_bounds_bt_on.size.h);
#endif
  bluetooth_layer = bitmap_layer_create(frame_bt);
  bitmap_layer_set_bitmap(bluetooth_layer, bluetooth_image);
  bitmap_layer_set_compositing_mode(bluetooth_layer, GCompOpSet);
  layer_add_child(window_layer, bitmap_layer_get_layer(bluetooth_layer));
	
//create layer for hands and date
  layer = layer_create(bounds);
  layer_set_update_proc(layer, update_layer);
  layer_add_child(window_layer, layer);
	
  prv_update_display();
	
  toggle_bluetooth_icon(bluetooth_connection_service_peek());
  update_battery_state(battery_state_service_peek());

//subscribe to seconds tick event
  tick_timer_service_subscribe(MINUTE_UNIT, tick);
	
// init hand paths
  s_second_arrow = gpath_create(&SECOND_HAND_POINTS);
  s_minute_arrow = gpath_create(&MINUTE_HAND_POINTS);
  s_hour_arrow = gpath_create(&HOUR_HAND_POINTS);
  GPoint center = grect_center_point(&bounds);
  gpath_move_to(s_second_arrow, center);
  gpath_move_to(s_minute_arrow, center);
  gpath_move_to(s_hour_arrow, center);
	
  bluetooth_connection_service_subscribe(bluetooth_connection_callback);
  battery_state_service_subscribe(&update_battery_state);
	
// draw first frame
  force_update();

}

static void deinit(void) {

  bluetooth_connection_service_unsubscribe();
  battery_state_service_unsubscribe();
	
  layer_destroy(layer);
	
  layer_remove_from_parent(bitmap_layer_get_layer(background_layer));
  bitmap_layer_destroy(background_layer);
  gbitmap_destroy(background);

  layer_remove_from_parent(bitmap_layer_get_layer(topm_layer));
  bitmap_layer_destroy(topm_layer);

	if (topm != NULL) {
		gbitmap_destroy(topm);
    }
	
  layer_remove_from_parent(bitmap_layer_get_layer(bluetooth_layer));
  bitmap_layer_destroy(bluetooth_layer);
  gbitmap_destroy(bluetooth_image);
  bluetooth_image = NULL;
	
  layer_remove_from_parent(bitmap_layer_get_layer(layer_batt_img));
  bitmap_layer_destroy(layer_batt_img);
  gbitmap_destroy(img_battery_1);
  gbitmap_destroy(img_battery_2);
  gbitmap_destroy(img_battery_3);
  gbitmap_destroy(img_battery_4);
  gbitmap_destroy(img_battery_5);
  gbitmap_destroy(img_battery_6);
  gbitmap_destroy(img_battery_7);
  gbitmap_destroy(img_battery_8);
  gbitmap_destroy(img_battery_charge);	
	
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}