int choose_week_day             (); //setup current week
int choose_day                  (); //setup current day
int choose_month                ();	//setup current month
int choose_year                 (); //setup current year
int choose_hour                 (); //setup current hour
int choose_minute               ();	//setup current minute
int choose_second               ();	//setup current second
void gui_current_time           (); //setup time function
void gui_current_time_set_date  ();	//setup date function
void gui_current_time_set_time  ();	//setup time function
void exe_current_time           (int command);	//excute choosen command	
void exe_set_date               (int command);	//put current setup date to ds1307 register
void exe_set_time               (int command);	//put current setup time to ds1307 register

int choose_week_day() {
    int i;
    while (1) {
        i = 1;
        clrscr();
        gui_set_title("WEEK DAY");
        glcd_setfont(big_letter_10x14);
        glcd_outtextxyf(25,22,name_week_day[i]); //sunday
        glcd_outtext("@"); //dau mui ten xuong
        glcd_setfont(font5x7);
        glcd_putcharxy(x - 7,y,' '); //xoa dau > 
        while(1) {
            if (btn_enter) {
                while (btn_enter);
                return i;
            }
            if (btn_back) {
                while (btn_back);
                if (i == 7) {
                    i = 1;
                } else {
                    i++;
                }   
                glcd_setfont(big_letter_10x14);
                glcd_outtextxyf(25,22,name_week_day[i]);
                glcd_outtext("@"); //dau mui ten xuong
                glcd_setfont(font5x7);
            }
            if (btn_menu) {
                while (btn_menu);
                return week_day;
            }
        }
    }
}
int choose_day() {
    int i;
    while (1) {
        i = 1;
        clrscr();
        gui_set_title("DAY");
        glcd_setfont(big_letter_10x14);
            glcd_outtextxyf(25,22,name_day[i]); //01
            glcd_outtext("@"); //dau mui ten xuong
        glcd_setfont(font5x7);
        glcd_putcharxy(x - 7,y,' '); //xoa dau > 
        while(1) {
            if (btn_enter) {
                while (btn_enter);
                return i;
            }
            if (btn_back) {
                while (btn_back);
                if (i == 31) {
                    i = 1;
                } else {
                    i++;
                }   
                glcd_setfont(big_letter_10x14);
                    glcd_outtextxyf(25,22,name_day[i]);  
                    glcd_outtext("@"); //dau mui ten xuong
                glcd_setfont(font5x7);
            }
            if (btn_menu) {
                while (btn_menu);
                return day;
            }
        }
    }
}
int choose_month() {
    int i;
    while (1) {
        i = 1;
        clrscr();
        gui_set_title("MONTH");
        glcd_setfont(big_letter_10x14);
            glcd_outtextxyf(25,22,name_month[i]); //01
            glcd_outtext("@"); //dau mui ten xuong
        glcd_setfont(font5x7);
        glcd_putcharxy(x - 7,y,' '); //xoa dau > 
        while(1) {
            if (btn_enter) {
                while (btn_enter);
                return i;
            }
            if (btn_back) {
                while (btn_back);
                if (i == 12) {
                    i = 1;
                } else {
                    i++;
                }   
                glcd_setfont(big_letter_10x14);
                    glcd_outtextxyf(25,22,name_month[i]);  
                    glcd_outtext("@"); //dau mui ten xuong
                glcd_setfont(font5x7);
            }
            if (btn_menu) {
                while (btn_menu);
                return month;
            }
            
        }
    }
}
int choose_year() {
    int i;
    while (1) {
        i = 0;
        clrscr();
        gui_set_title("YEAR");
        glcd_setfont(big_letter_10x14);
            glcd_outtextxyf(15,22,name_year[i]); //2017
            glcd_outtext("@"); //dau mui ten xuong
        glcd_setfont(font5x7);
        glcd_putcharxy(x - 7,y,' '); //xoa dau > 
        while(1) {
            if (btn_enter) {
                while (btn_enter);
                return i;
            }
            if (btn_back) {
                while (btn_back);
                if (i == (2032 - 2017)) {
                    i = 0;
                } else {
                    i++;
                }   
                glcd_setfont(big_letter_10x14);
                    glcd_outtextxyf(15,22,name_year[i]);  
                    glcd_outtext("@"); //dau mui ten xuong                
                glcd_setfont(font5x7);
            }
            if (btn_menu) {
                while (btn_menu);
                return year;
            }
        }
    }
}
int choose_hour() {
    int i;
    while (1) {
        i = 0;
        clrscr();
        gui_set_title("HOUR");
        glcd_setfont(big_letter_10x14);
            glcd_outtextxyf(25,22,name_hour[i]); // 0h
            glcd_outtext("@"); //dau mui ten xuong
        glcd_setfont(font5x7);
        glcd_putcharxy(x - 7,y,' '); //xoa dau > 
        while(1) {
            if (btn_enter) {
                while (btn_enter);
                return i;
            }
            if (btn_back) {
                while (btn_back);
                if (i == 23) {
                    i = 0;
                } else {
                    i++;
                }   
                glcd_setfont(big_letter_10x14);
                    glcd_outtextxyf(25,22,name_hour[i]);  
                    glcd_outtext("@"); //dau mui ten xuong
                glcd_setfont(font5x7);
            }
            if (btn_menu) {
                while (btn_menu);
                return hour;
            }
        }
    }
}
int choose_minute() {
    int i;
    while (1) {
        i = 0;
        clrscr();
        gui_set_title("MINUTE");
        glcd_setfont(big_letter_10x14);
            glcd_outtextxyf(25,22,name_minute[i]); // 0h
            glcd_outtext("@"); //dau mui ten xuong
        glcd_setfont(font5x7);
        glcd_putcharxy(x - 7,y,' '); //xoa dau > 
        while(1) {
            if (btn_enter) {
                while (btn_enter);
                return i;
            }
            if (btn_back) {
                while (btn_back);
                if (i == 59) {
                    i = 0;
                } else {
                    i++;
                }   
                glcd_setfont(big_letter_10x14);
                    glcd_outtextxyf(25,22,name_minute[i]);  
                    glcd_outtext("@"); //dau mui ten xuong
                glcd_setfont(font5x7);
            }
            if (btn_menu) {
                while (btn_menu);
                return minute;
            }
        }
    }
}
int choose_second() {
    int i;
    while (1) {
        i = 0;
        clrscr();
        gui_set_title("SECOND");
        glcd_setfont(big_letter_10x14);
            glcd_outtextxyf(25,22,name_second[i]); // 0s
            glcd_outtext("@"); //dau mui ten xuong
        glcd_setfont(font5x7);
        glcd_putcharxy(x - 7,y,' '); //xoa dau > 
        while(1) {
            if (btn_enter) {
                while (btn_enter);
                return i;
            }
            if (btn_back) {
                while (btn_back);
                if (i == 59) {
                    i = 0;
                } else {
                    i++;
                }   
                glcd_setfont(big_letter_10x14);
                    glcd_outtextxyf(25,22,name_second[i]);  
                    glcd_outtext("@"); //dau mui ten xuong
                glcd_setfont(font5x7);
            }
            if (btn_menu) {
                while (btn_menu);
                return second;
            }
        }
    }
}
void gui_current_time() {
    int erase_position = 0;
    int command;
    while (1) {
        command = 1;
        clrscr();
        gui_set_title("SET TIME");
        gui_set_choice(y + NEW_LINE * 0, "Set Date");
        gui_set_choice(y + NEW_LINE * 1, "Set Time");
        while (1) {
            if (btn_enter) {
                while (btn_enter);
                exe_current_time(command);
                break;
            }
            if (btn_back) {
                while (btn_back);
                command++;
                y += NEW_LINE;

                if (command > 2) {
                    erase_position  =   2;
                    command         =   1;
                    y               =   15;
                }
                gui_cursor_move(erase_position);
                erase_position      =   0;
            }
            if (btn_menu) {
                while(btn_menu);
                return;
            }
        }

    }
}
void gui_current_time_set_date() {
    int erase_position = 0;
    int command;
    while (1) {
        command = 1;
        clrscr();
        gui_set_title("SET DATE");
        gui_set_choice(y + NEW_LINE * 0, "Week day");
        gui_set_choice(y + NEW_LINE * 1, "Day");
        gui_set_choice(y + NEW_LINE * 2, "Month");
        gui_set_choice(y + NEW_LINE * 3, "Year");
        while (1) {
            if (btn_enter) {
                while(btn_enter);
                exe_set_date(command);
                break;
            }
            if (btn_back) {
                while(btn_back);

                command++;
                y += NEW_LINE;
                if (command > 4) {
                    erase_position      =   4;
                    command             =   1;
                    y                   =   15;
                }
                gui_cursor_move(erase_position);
                erase_position          =   0;
            }
            if (btn_menu) {
                while(btn_menu);
                return;
            }
        }
    }
}
void gui_current_time_set_time() {
    int erase_position = 0;
    int command;
    while (1) {
        command = 1;
        clrscr();
        gui_set_title("SET TIME");
        gui_set_choice(y + NEW_LINE * 0, "Hour");
        gui_set_choice(y + NEW_LINE * 1, "Minute");
        gui_set_choice(y + NEW_LINE * 2, "Second");
        while (1) {
            if (btn_enter) {
                while(btn_enter);
                exe_set_time(command);
                break;
            }
            if (btn_back) {
                while(btn_back);
                command++;
                y += NEW_LINE;
                if (command > 3) {
                    erase_position = 3;
                    command = 1;
                    y = 15;
                }
                gui_cursor_move(erase_position);
                erase_position = 0;
            }
            if (btn_menu) {
                while(btn_menu);
                return;
            }
        }
    }
}
void exe_current_time(int command) {
    switch (command) {
        case 1:
            gui_current_time_set_date();
            return;
        case 2:
            gui_current_time_set_time();
            return;
    }
}
void exe_set_date(int command) {
    switch (command) {
        case 1:
            week_day    =   choose_week_day();
            rtc_set_date(week_day, day, month, year);
            return;
        case 2:
            day         =   choose_day();
            rtc_set_date(week_day, day, month, year);
            return;
        case 3:
            month       =   choose_month();
            rtc_set_date(week_day, day, month, year);
            return;
        case 4:
            year        =   choose_year();
            rtc_set_date(week_day, day, month, year);
            return;
    }
}
void exe_set_time(int command) {
    switch (command) {
        case 1:
            hour        =   choose_hour();	//renew hour to ds1307 register
            rtc_set_time(hour, minute, second);
            return;
        case 2:
            minute      =   choose_minute();
            rtc_set_time(hour, minute, second);
            return;
        case 3:
            second      =   choose_second();
            rtc_set_time(hour, minute, second);
            return;
    }
}