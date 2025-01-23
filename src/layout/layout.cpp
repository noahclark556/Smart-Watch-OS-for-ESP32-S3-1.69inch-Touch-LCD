#include <lvgl.h>
#include "layout.h"
#include "../resources/IMG_NOAH.h"
#include <time.h>
#include <stdio.h>
#include "../util/util.h"

static lv_obj_t *clock_label;
void update_clock(lv_timer_t *);

// declaration
static void create_page1(lv_obj_t *parent);
static void create_page2(lv_obj_t *parent);
static void create_page3(lv_obj_t *parent);
void lt_btn_handler(lv_event_t *e);

void initialize_layout(void)
{
    // main horiz scroll container setup
    lv_obj_t *main_cont = lv_obj_create(lv_scr_act());
    lv_obj_set_size(main_cont, lv_pct(100), lv_pct(100));
    lv_obj_set_scroll_dir(main_cont, LV_DIR_HOR);
    lv_obj_set_scroll_snap_x(main_cont, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_style_pad_row(main_cont, 0, 0);
    lv_obj_set_style_pad_column(main_cont, 0, 0);
    lv_obj_set_style_pad_all(main_cont, 0, 0);
    lv_obj_set_flex_flow(main_cont, LV_FLEX_FLOW_ROW); // side by side pages

    // dashboard page
    lv_obj_t *page1 = lv_obj_create(main_cont);
    lv_obj_set_size(page1, lv_pct(100), lv_pct(100)); // w,h
    lv_obj_set_scroll_dir(page1, LV_DIR_VER);         // vertical scroll
    // lv_obj_set_scroll_snap_y(page1, LV_SCROLL_SNAP_NONE); // enables snapping
    // lv_obj_set_style_pad_all(page1, 0, 0);
    lv_obj_set_style_border_width(page1, 0, 0);
    // lv_obj_clear_flag(page1, LV_OBJ_FLAG_SCROLLABLE); // not needed, does opposite
    create_page1(page1);

    // agenda page
    lv_obj_t *page2 = lv_obj_create(main_cont);
    lv_obj_set_size(page2, lv_pct(100), lv_pct(100));
    lv_obj_set_scroll_dir(page2, LV_DIR_VER);
    // lv_obj_set_scroll_snap_y(page2, LV_SCROLL_SNAP_NONE);
    lv_obj_set_style_pad_all(page2, 0, 0);
    // lv_obj_clear_flag(page2, LV_OBJ_FLAG_SCROLLABLE);
    create_page2(page2);

    // control page
    lv_obj_t *page3 = lv_obj_create(main_cont);
    lv_obj_set_size(page3, lv_pct(100), lv_pct(100));
    lv_obj_set_scroll_dir(page3, LV_DIR_VER);
    // lv_obj_set_scroll_snap_y(page2, LV_SCROLL_SNAP_NONE);
    lv_obj_set_style_pad_all(page3, 0, 0);
    // lv_obj_clear_flag(page2, LV_OBJ_FLAG_SCROLLABLE);
    create_page3(page3);
}

static void create_page1(lv_obj_t *parent)
{
    lv_obj_set_style_pad_top(parent, 30, 0);

    clock_label = lv_label_create(parent);
    lv_label_set_text(clock_label, "");
    lv_obj_set_style_text_font(clock_label, &lv_font_montserrat_24, 0);
    lv_obj_set_style_pad_top(clock_label, 5, 0);
    lv_obj_align(clock_label, LV_ALIGN_CENTER, 0, -90);

    lv_obj_t *welcome_label = lv_label_create(parent);
    lv_label_set_text(welcome_label, "Welcome, Noah!");
    lv_obj_set_style_text_font(welcome_label, &lv_font_montserrat_20, 0);
    lv_obj_align_to(welcome_label, clock_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    lv_obj_t *container = lv_obj_create(parent);
    lv_obj_set_size(container, 100, 100);
    lv_obj_set_style_radius(container, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_clip_corner(container, true, 0);
    lv_obj_clear_flag(container, LV_OBJ_FLAG_SCROLLABLE); // Disable scrolling inside this container
    lv_obj_align_to(container, welcome_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    lv_obj_t *img = lv_img_create(container);
    lv_img_set_src(img, &IMG_NOAH);
    lv_img_set_zoom(img, 128);
    lv_obj_center(img);

    lv_obj_t *emergency_label = lv_label_create(parent);
    lv_label_set_text(emergency_label, "Emergency Data");
    lv_obj_set_style_text_font(emergency_label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(emergency_label, lv_color_hex(0xFF0000), 0);
    lv_obj_align_to(emergency_label, container, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_obj_set_style_pad_top(emergency_label, 80, 0); // pad so its on a new (page)

    lv_obj_t *bt_label = lv_label_create(parent);
    lv_label_set_text(bt_label, "Blood Type: O+");
    lv_obj_set_style_text_font(bt_label, &lv_font_montserrat_18, 0);
    lv_obj_align_to(bt_label, emergency_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);

    lv_obj_t *ct_label = lv_label_create(parent);
    lv_label_set_text(ct_label, "Emergency Contact:");
    lv_obj_set_style_text_font(ct_label, &lv_font_montserrat_18, 0);
    lv_obj_align_to(ct_label, bt_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 15);

    lv_obj_t *ct_name_label = lv_label_create(parent);
    lv_label_set_text(ct_name_label, "Jacob Clark (Brother)");
    lv_obj_set_style_text_font(ct_name_label, &lv_font_montserrat_14, 0);
    lv_obj_align_to(ct_name_label, ct_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);

    lv_obj_t *ct_phone_label = lv_label_create(parent);
    lv_label_set_text(ct_phone_label, "724-422-8688");
    lv_obj_set_style_text_font(ct_phone_label, &lv_font_montserrat_14, 0);
    lv_obj_align_to(ct_phone_label, ct_name_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);

    lv_obj_set_style_pad_bottom(ct_phone_label, 70, 0);

    // Update the clock every second
    lv_timer_create(update_clock, 1000, NULL);
}

static void create_page2(lv_obj_t *parent)
{
    lv_obj_set_style_pad_top(parent, 30, 0);

    lv_obj_t *agenda_label = lv_label_create(parent);
    lv_label_set_text(agenda_label, "Noah's Agenda");
    lv_obj_set_style_text_font(agenda_label, &lv_font_montserrat_22, 0);
    lv_obj_set_style_text_align(agenda_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(agenda_label, LV_ALIGN_CENTER, 0, -110);
    lv_obj_set_style_text_color(agenda_label, lv_color_hex(0xFB8F47), 0);

    // Formatted this way for possible db communication later
    struct
    {
        const char *time;
        const char *items[5];
    } agenda[] = {
        {"9:00 AM", {"Drink Coffee", "Check Calendar", NULL}},
        {"10:30 AM", {"Check Emails", NULL}},
        {"12:00 PM", {"Team Lunch", "Call Vendor", NULL}},
        {"2:00 PM", {"Status Meeting", "Create Report", NULL}},
        {"4:00 PM", {"Gym Session", NULL}}};

    // maybe remove this so the whole page scrolls
    lv_obj_t *agenda_cont = lv_obj_create(parent);
    lv_obj_set_width(agenda_cont, lv_pct(90));
    lv_obj_set_height(agenda_cont, lv_pct(100));
    lv_obj_set_flex_flow(agenda_cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_border_width(agenda_cont, 0, 0);
    lv_obj_align_to(agenda_cont, agenda_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);

    for (int i = 0; i < (int)(sizeof(agenda) / sizeof(agenda[0])); i++)
    {
        lv_obj_t *time_label = lv_label_create(agenda_cont);
        char time_buf[64];
        snprintf(time_buf, sizeof(time_buf), "• %s", agenda[i].time);
        lv_label_set_text(time_label, time_buf);
        lv_obj_set_style_text_font(time_label, &lv_font_montserrat_18, 0);
        lv_obj_set_style_text_color(time_label, lv_color_hex(0x3091F1), 0);
        lv_obj_set_style_pad_bottom(time_label, 5, 0);

        for (int j = 0; agenda[i].items[j] != NULL; j++)
        {
            lv_obj_t *item_label = lv_label_create(agenda_cont);
            lv_label_set_text(item_label, agenda[i].items[j]);
            lv_obj_set_style_text_font(item_label, &lv_font_montserrat_16, 0);
            lv_obj_set_style_text_color(item_label, lv_color_black(), 0);
            lv_obj_set_style_pad_left(item_label, 20, 0);
            lv_obj_set_style_pad_bottom(item_label, 3, 0);
        }
    }
}

void lt_btn_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e); // Get the event code
    lv_obj_t *btn = lv_event_get_target(e);      // Get the object that triggered the event

    if (code == LV_EVENT_CLICKED) // Check if the button was clicked
    {
        USBSerial.println("Button tapped!");
    }
}

static void create_page3(lv_obj_t *parent)
{
    lv_obj_set_style_pad_top(parent, 30, 0);

    lv_obj_t *cpanel_label = lv_label_create(parent);
    lv_label_set_text(cpanel_label, "Control Panel");
    lv_obj_set_style_text_font(cpanel_label, &lv_font_montserrat_22, 0);
    lv_obj_set_style_text_align(cpanel_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(cpanel_label, LV_ALIGN_CENTER, 0, -110);
    lv_obj_set_style_text_color(cpanel_label, lv_color_hex(0xFB8F47), 0);

    lv_obj_t *btn = lv_btn_create(parent);
    lv_obj_set_size(btn, 120, 50);
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, "Click");
    lv_obj_center(label);

    lv_obj_add_event_cb(btn, lt_btn_handler, LV_EVENT_ALL, NULL);
}


void update_clock(lv_timer_t *timer)
{
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);

    char time_str[16];
    strftime(time_str, sizeof(time_str), "%I:%M %p", current_time);

    lv_label_set_text(clock_label, time_str);
}
