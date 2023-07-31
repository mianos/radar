// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.1
// LVGL version: 8.3.6
// Project name: ldx

#include "../ui.h"

void ui_Screen1_screen_init(void)
{
ui_Screen1 = lv_obj_create(NULL);
lv_obj_clear_flag( ui_Screen1, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_times = lv_obj_create(ui_Screen1);
lv_obj_set_width( ui_times, 248);
lv_obj_set_height( ui_times, 23);
lv_obj_set_x( ui_times, 0 );
lv_obj_set_y( ui_times, -54 );
lv_obj_set_align( ui_times, LV_ALIGN_CENTER );
lv_obj_clear_flag( ui_times, LV_OBJ_FLAG_SCROLLABLE );    /// Flags

ui_clock = lv_label_create(ui_times);
lv_obj_set_width( ui_clock, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_clock, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_clock, 7 );
lv_obj_set_y( ui_clock, 0 );
lv_obj_set_align( ui_clock, LV_ALIGN_CENTER );
lv_label_set_text(ui_clock,"12:34:56");

ui_elapsed = lv_label_create(ui_times);
lv_obj_set_width( ui_elapsed, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_elapsed, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_elapsed, 7 );
lv_obj_set_y( ui_elapsed, 1 );
lv_obj_set_align( ui_elapsed, LV_ALIGN_RIGHT_MID );
lv_label_set_text(ui_elapsed,"HH:MM");

ui_mqtton = lv_checkbox_create(ui_times);
lv_checkbox_set_text(ui_mqtton,"");
lv_obj_set_width( ui_mqtton, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_mqtton, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_mqtton, 16 );
lv_obj_set_y( ui_mqtton, -1 );
lv_obj_set_align( ui_mqtton, LV_ALIGN_LEFT_MID );
lv_obj_add_flag( ui_mqtton, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags

lv_obj_set_style_border_side(ui_mqtton, LV_BORDER_SIDE_NONE, LV_PART_INDICATOR| LV_STATE_DEFAULT);

ui_timelabel = lv_label_create(ui_times);
lv_obj_set_width( ui_timelabel, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_timelabel, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_timelabel, -45 );
lv_obj_set_y( ui_timelabel, -1 );
lv_obj_set_align( ui_timelabel, LV_ALIGN_CENTER );
lv_label_set_text(ui_timelabel,"Time:");

ui_uplabel = lv_label_create(ui_times);
lv_obj_set_width( ui_uplabel, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_uplabel, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_uplabel, 51 );
lv_obj_set_y( ui_uplabel, 0 );
lv_obj_set_align( ui_uplabel, LV_ALIGN_CENTER );
lv_label_set_text(ui_uplabel,"Up:");

ui_Label4 = lv_label_create(ui_times);
lv_obj_set_width( ui_Label4, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Label4, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_Label4, -107 );
lv_obj_set_y( ui_Label4, 0 );
lv_obj_set_align( ui_Label4, LV_ALIGN_CENTER );
lv_label_set_text(ui_Label4,"Cq");

ui_messages = lv_textarea_create(ui_Screen1);
lv_obj_set_width( ui_messages, 248);
lv_obj_set_height( ui_messages, 46);
lv_obj_set_x( ui_messages, 0 );
lv_obj_set_y( ui_messages, 40 );
lv_obj_set_align( ui_messages, LV_ALIGN_CENTER );
lv_textarea_set_max_length(ui_messages,1000);
lv_textarea_set_placeholder_text(ui_messages,"Messages");

ui_MDist = lv_bar_create(ui_Screen1);
lv_bar_set_range(ui_MDist, 0,300);
lv_obj_set_width( ui_MDist, 150);
lv_obj_set_height( ui_MDist, 15);
lv_obj_set_x( ui_MDist, -40 );
lv_obj_set_y( ui_MDist, -24 );
lv_obj_set_align( ui_MDist, LV_ALIGN_CENTER );

ui_MEnergy = lv_bar_create(ui_Screen1);
lv_obj_set_width( ui_MEnergy, 60);
lv_obj_set_height( ui_MEnergy, 15);
lv_obj_set_x( ui_MEnergy, 78 );
lv_obj_set_y( ui_MEnergy, -24 );
lv_obj_set_align( ui_MEnergy, LV_ALIGN_CENTER );

ui_SDist = lv_bar_create(ui_Screen1);
lv_bar_set_range(ui_SDist, 0,300);
lv_obj_set_width( ui_SDist, 150);
lv_obj_set_height( ui_SDist, 15);
lv_obj_set_x( ui_SDist, -40 );
lv_obj_set_y( ui_SDist, 3 );
lv_obj_set_align( ui_SDist, LV_ALIGN_CENTER );

ui_SEnergy = lv_bar_create(ui_Screen1);
lv_obj_set_width( ui_SEnergy, 60);
lv_obj_set_height( ui_SEnergy, 15);
lv_obj_set_x( ui_SEnergy, 78 );
lv_obj_set_y( ui_SEnergy, 3 );
lv_obj_set_align( ui_SEnergy, LV_ALIGN_CENTER );

}
