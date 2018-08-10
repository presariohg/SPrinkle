#include <mega128.h>
#include <i2c.h>
#include <ds1307.h>
#include <glcd.h>
#include <eeprom.h>
#include <font5x7.h>
#include <big_letter_10x14.c>
#include <tiny_letter.c>
#include <relay_icon_7x7.c>
#include <relay_icon_10x14.c>
#include <phone_signal_10x5.c>
#include <delay.h>
#include <string.h>
#include <stdlib.h>
#include <define.c>
#include <rf.c>

typedef struct {
	bool 	status;
	bool 	active_hour[24];
	bool 	active_week_day[8];
    bool    is_set_time;
    int     time_left_m;
    int     time_left_s;
    int 	alarm_time_m;
} relay_type;
eeprom relay_type node[6];

#include <menu.c>
#include <sim_uart1_init.c>

#include <relay_setting.c>
#include <time_date_setting.c>
#include <init.c>

void main(void) {
    init_system();
    glcd_clear();
    milisec = 0;
    gui_init();
    is_show_clock = true; 
    
    init_sms();
	init_gprs();
     
    Address1 = 0xA1;
    Address2 = 0xA2;
    Address3 = 0xA3;
    Address4 = 0xA4;
    
    Common_Config();
    RX_Mode();
    RX_Config_4();

    
    while (1) { 
    gprs_send();
    delay_ms(200);
    gprs_read(); 
        if (IRQ == 0) {
			RX_Read();                     
			node_info_examine(station_receive);
           // gprs_send();
		} 		
        if(btn_menu) {
            while(btn_menu);
            is_show_clock = false;
            gui_main_menu();
        }
        if(btn_back) {
         //  gprs_send();  
         sms_send("test");
        }
        if(btn_enter) {
            while(btn_enter);
            call();
        }
        //gprs_send();
       // delay_ms(1000);
       // gprs_read();
    }
}



