void gui_choose_relay           ();	//setup for relay timer
void gui_relay_setting          (int node_name);

void exe_relay                  (int command, int node_name);

void choose_multiple_hour       (int node_name);
void choose_multiple_week_day   (int node_name);

void gui_choose_relay() {
    int i;

    while (1) {
        i = 1;
        clrscr();
        gui_set_title("SET NODE");
        glcd_setfont(relay_icon_10x14);
        glcd_putcharxy(5, 22, node[1].status);
        glcd_setfont(big_letter_10x14);
        glcd_outtextxyf(25,22, name_relay[i]); 
       // glcd_outtext("@ "); //dau mui ten xuong
        glcd_setfont(font5x7);
        glcd_putcharxy(x - 7,y,' '); //xoa dau > 
        while(1) {
            if (btn_enter) {
                while (btn_enter);
                if (i == 6) { //ALL
                    relay_1 = !relay_1;
                    relay_2 = !relay_2;
                    relay_3 = !relay_3;
                    relay_4 = !relay_4;
                    relay_5 = !relay_5;
                    for (i = 1; i <= 5; i++) {
                        node[i].status = !node[i].status;
                    }
                } else {
                    gui_relay_setting(i);
                }

                break; //break khoi vong while trong thi no se chay lai vong while ngoai tu dau, khong phai init lai gui
            }
            if (btn_back) {
                while (btn_back);
                if (i == 6) {
                    i = 1;
                } else {
                    i++;
                }   

                glcd_setfont(relay_icon_10x14);

                    glcd_putcharxy(5, 22, node[i].status);
                
                glcd_setfont(big_letter_10x14);

                    glcd_outtextxyf(25,22,name_relay[i]);
                    glcd_outtext("@"); //dau mui ten xuong

                glcd_setfont(font5x7);

            }
            if (btn_menu) {        //move to main menu
                while (btn_menu);
                return;
            }
        }
    }
}

void gui_relay_setting(int node_name) {
    int erase_position = 0;
    int command;
    char title[10];
    memset(title, '\0', 10);
    strcpy(title, "NODE ");
    title[6] = '0' + node_name;
    while (1) {
        clrscr();
        gui_set_title(title);
        gui_set_choice(y + NEW_LINE * 0, "On/Off");
        gui_set_choice(y + NEW_LINE * 1, "Set Hour");
        gui_set_choice(y + NEW_LINE * 2, "Set Week Day");
        gui_set_choice(y + NEW_LINE * 3, "Status");
        command = 1;
        while (1) {

            if (btn_enter) {           //excute role menu when press enter button
                while(btn_enter);
                exe_relay(command, node_name);   //in function exe_relay
                break;
            }
            if (btn_back) {            //next command when press back button
                while(btn_back);
                command++;
                y += NEW_LINE;
                if (command > 4) {
                    erase_position  =   4;
                    command         =   1;
                    y               =   15;
                }
                gui_cursor_move(erase_position);    //move cursor position
                erase_position      =   0;
            }    

            if (btn_menu) {         //back to main screen when press menu button
                while (btn_menu);
                return;
            }
        }
    }
}

void exe_relay(int command, int node_name) {
    switch (command) {
        case 1:
            switch (node_name) {
                case 1:
                    relay_1 = !relay_1;
                    node[1].status = '0' + (relay_1 == ON);
                break;

                case 2:
                    relay_2 = !relay_2;
                    node[2].status = '0' + (relay_2 == ON);
                break;

                case 3:
                    relay_3 = !relay_3;
                    node[3].status = '0' + (relay_3 == ON);
                break;

                case 4:
                    relay_4 = !relay_4;
                    node[4].status = '0' + (relay_4 == ON);
                break;

                case 5:
                    relay_5 = !relay_5;
                    node[5].status = '0' + (relay_5 == ON);
                break;
            }
        break;

        case 2:
            choose_multiple_hour(node_name);
        break;

        case 3:
            choose_multiple_week_day(node_name);
        break;

        case 4:
            // gui_set_title("STATUS");

            // glcd_putcharxy(x - 7,y,' '); //xoa dau > 



        break;
    }
}

void choose_multiple_hour(int node_name) {
    int i;

    while (1) {
        i = 0;
        clrscr();
        gui_set_title("HOUR");

        glcd_setfont(relay_icon_10x14);

            glcd_putcharxy(5, 22, '0' + node[node_name].active_hour[0]);


        glcd_setfont(big_letter_10x14);

            glcd_outtextxyf(25,22,name_hour[i]); // 0h
            glcd_outtext("@"); //dau mui ten xuong

        glcd_setfont(font5x7);

        glcd_putcharxy(x - 7,y,' '); //xoa dau > 

        while(1) {
            if (btn_enter) {
                while (btn_enter);
                node[node_name].active_hour[i] = !node[node_name].active_hour[i];

                glcd_setfont(relay_icon_10x14);

                    glcd_putcharxy(5, 22, '0' + node[node_name].active_hour[i]);

            }
            if (btn_back) {
                while (btn_back);
                if (i == 23) {
                    i = 0;
                } else {
                    i++;
                }   

                glcd_setfont(relay_icon_10x14);

                    glcd_putcharxy(5, 22, '0' + node[node_name].active_hour[i]);
                
                glcd_setfont(big_letter_10x14);

                    glcd_outtextxyf(25,22,name_hour[i]);  
                    glcd_outtext("@"); //dau mui ten xuong

                glcd_setfont(font5x7);
            }
            if (btn_menu) {
                while (btn_menu);
                return;
            }
        }
    }
}

void choose_multiple_week_day(int node_name) {
    int i;

    while (1) {
        i = 1;
        clrscr();
        gui_set_title("WEEK DAY");

        glcd_setfont(relay_icon_10x14);

            glcd_putcharxy(5, 22, '0' + node[node_name].active_week_day[1]);

        glcd_setfont(big_letter_10x14);

            glcd_outtextxyf(25,22,name_week_day[i]); //sunday
            glcd_outtext("@"); //dau mui ten xuong

        glcd_setfont(font5x7);

        glcd_putcharxy(x - 7,y,' '); //xoa dau > 

        while(1) {
            if (btn_enter) {
                while (btn_enter);
                node[node_name].active_week_day[i] = !node[node_name].active_week_day[i];

                glcd_setfont(relay_icon_10x14);

                    glcd_putcharxy(5, 22, '0' + node[node_name].active_week_day[i]);
                
            }
            if (btn_back) {
                while (btn_back);
                if (i == 7) {
                    i = 1;
                } else {
                    i++;
                }   

                glcd_setfont(relay_icon_10x14);

                    glcd_putcharxy(5, 22, '0' + node[node_name].active_week_day[i]);
                
                glcd_setfont(big_letter_10x14);

                    glcd_outtextxyf(25,22,name_week_day[i]);
                    glcd_outtext("@"); //dau mui ten xuong

                glcd_setfont(font5x7);

            }
            if (btn_menu) {
                while (btn_menu);
                return;
            }
        }
    }
}

void get_relay_status() {      //show relay status in screen
	node[1].status = (relay_1 == ON) + '0';
	node[2].status = (relay_2 == ON) + '0';
	node[3].status = (relay_3 == ON) + '0';
	node[4].status = (relay_4 == ON) + '0';
	node[5].status = (relay_5 == ON) + '0';
}
