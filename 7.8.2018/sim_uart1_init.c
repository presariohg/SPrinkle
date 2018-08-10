unsigned char phone_number[13] = "+84983712941";
unsigned char sim = 0;   //0 la sim vietnam mobile, 1 la sim viettel
unsigned char money_code[20];
char money[10];
bool is_get_money = false;
char final[10]="em";

char ahi[]="fgvjhjbhgndxfcid=69#{'lenh':'*1off#'}{'lenh':'*2off#'}{'lenh':'*3on8#'}{'lenh':'*4on15#'} ";   
int p4=-1,dem4,a4,p3=-1,dem3,a3,p2=-1,dem2,a2,p1=-1,dem1,a1; 
int search                             ( char nguon[], char *dau);


void put_string                         (unsigned char *s);
void put_command                        (unsigned char *s, int time_out_ms, int max_retry);
void sms_read                           ();
bool read_command                       (unsigned char *s);
bool wait_until                         (unsigned char *s, int time_out_ms);
void clear_buffer                       ();
void decrypt                            (unsigned char *s);
void toggle_relay                       (bool status, unsigned char *s);
void sms_send                           (unsigned char *s);

void init_sms                           ();
void init_gprs                          ();
void gprs_read                          ();
void gprs_send                          ();
void get_money_request                  ();
void call                               ();
void get_money_response_vietnammobile   ();
void get_money_response_viettel         ();
void put_money                          ();

const int   BUFFER_SIZE         =   500,
            DEFAULT_MAX_RETRY   =   5,
            DEFAULT_TIME_OUT_MS =   50;

int buffer_index = 0;
char receive_buffer[BUFFER_SIZE];
// USART1 Receiver buffer
#define RX_BUFFER_SIZE1 64
char rx_buffer1[RX_BUFFER_SIZE1]; 

#if RX_BUFFER_SIZE1 <= 256
    unsigned char rx_wr_index1=0,rx_rd_index1=0;
#else
    unsigned int rx_wr_index1=0,rx_rd_index1=0;
#endif

#if RX_BUFFER_SIZE1 < 256
    unsigned char rx_counter1=0;
#else
    unsigned int rx_counter1=0;
#endif

// This flag is set on USART1 Receiver buffer overflow
bit rx_buffer_overflow1;

// USART1 Receiver interrupt service routine
interrupt [USART1_RXC] void usart1_rx_isr(void) {
    char status,data;
    status=UCSR1A;
    data=UDR1;

    if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0) {
        rx_buffer1[rx_wr_index1++]=data;
        #if RX_BUFFER_SIZE1 == 256
        // special case for receiver buffer size=256
            if (++rx_counter1 == 0) rx_buffer_overflow1=1;
        #else
            if (rx_wr_index1 == RX_BUFFER_SIZE1) rx_wr_index1=0;
            if (++rx_counter1 == RX_BUFFER_SIZE1) {
                rx_counter1=0;
                rx_buffer_overflow1=1;
            }
        #endif
    }

    receive_buffer[buffer_index] = data;
    if (read_command("CMTI")) sms_read();
    if (buffer_index == BUFFER_SIZE) buffer_index = 0;
    buffer_index++;
}

// Get a character from the USART1 Receiver buffer
#pragma used+
    char getchar1(void) {
        char data;
        while (rx_counter1==0);
        data=rx_buffer1[rx_rd_index1++];
    #if RX_BUFFER_SIZE1 != 256
        if (rx_rd_index1 == RX_BUFFER_SIZE1) rx_rd_index1=0;
    #endif
    #asm("cli")
        --rx_counter1;
    #asm("sei")
        return data;
    }
#pragma used-
// USART1 Transmitter buffer
#define TX_BUFFER_SIZE1 64
char tx_buffer1[TX_BUFFER_SIZE1];

#if TX_BUFFER_SIZE1 <= 256
unsigned char tx_wr_index1=0,tx_rd_index1=0;
#else
unsigned int tx_wr_index1=0,tx_rd_index1=0;
#endif

#if TX_BUFFER_SIZE1 < 256
unsigned char tx_counter1=0;
#else
unsigned int tx_counter1=0;
#endif

// USART1 Transmitter interrupt service routine
interrupt [USART1_TXC] void usart1_tx_isr(void) {
    if (tx_counter1) {
        --tx_counter1;
        UDR1=tx_buffer1[tx_rd_index1++];
#if TX_BUFFER_SIZE1 != 256
        if (tx_rd_index1 == TX_BUFFER_SIZE1) tx_rd_index1=0;
#endif
    }
}

// Write a character to the USART1 Transmitter buffer
#pragma used+
void putchar1(char c) {
    while (tx_counter1 == TX_BUFFER_SIZE1);
#asm("cli")
    if (tx_counter1 || ((UCSR1A & DATA_REGISTER_EMPTY)==0)) {
        tx_buffer1[tx_wr_index1++]=c;
#if TX_BUFFER_SIZE1 != 256
        if (tx_wr_index1 == TX_BUFFER_SIZE1) tx_wr_index1=0;
#endif
        ++tx_counter1;
    } else
        UDR1=c;
#asm("sei")
}
#pragma used-

void put_string(unsigned char *s) {
    while (*s) {
        putchar1(*s);
        delay_ms(50);
        s++;
    }
}

void put_command(unsigned char *s, int time_out_ms, int max_retry) {
    char    command[50];
    int     i       =   0;

    memset(command,'\0',50);
    while (*s) {
        command[i] = *s;
        s++;
        i++;    
    }
    for (i = 0; i < max_retry; i++) {
        put_string(command);
        if (wait_until("OK", time_out_ms)) return; //neu khong tra ve "OK" thi lap lai
    }

    is_sim_init_ok = false;
    return;
}

bool wait_until(unsigned char *s, int time_out_ms) {
    char keyword[20];
    int i               =   0,
        time_start_ms   =   milisec;

    memset(keyword,'\0',20);
    while (*s) {
        keyword[i] = *s;
        s++;
        i++;
    }

    while (milisec - time_start_ms < time_out_ms) {
        if (strstr(receive_buffer, keyword)) {
            return true;
        }
    }

    return false;
}

bool read_command(unsigned char *s) {
    char command[4];
    int i               =   0;
    memset(command,'\0',4);

    while (*s) {
        command[i] = *s;
        s++;
        i++;
    }

    for (i = 0; i < 4; i++) {
        if (command[3 - i] != receive_buffer[buffer_index - i]) return false;
    }

    return true;
}


void clear_buffer() {
    memset(receive_buffer, '\0', BUFFER_SIZE);
    buffer_index = 0;
}

void init_gprs() {
    put_command("AT+CIPMODE=0\n\r", DEFAULT_TIME_OUT_MS, DEFAULT_MAX_RETRY);
 
    put_command("AT+CIPCSGP=1,\"internet\",\"\",\"\"\n\r", DEFAULT_TIME_OUT_MS, DEFAULT_MAX_RETRY);
 
    put_command("AT+CIPHEAD=0\n\r", DEFAULT_TIME_OUT_MS, DEFAULT_MAX_RETRY);
 
    put_command("AT+CIPSPRT=1\n\r", DEFAULT_TIME_OUT_MS, DEFAULT_MAX_RETRY); 

    put_command("AT+CIPSRIP=0\n\r", DEFAULT_TIME_OUT_MS, DEFAULT_MAX_RETRY);
 
    put_command("AT+CIPSCONT\n\r", DEFAULT_TIME_OUT_MS, DEFAULT_MAX_RETRY);       
    
    put_command("AT+CIPMUX=0\n\r", DEFAULT_TIME_OUT_MS, DEFAULT_MAX_RETRY);   
    
}

/*void init_gprs_m() {
    put_command("AT+CIPSHUT\n\r", DEFAULT_TIME_OUT_MS, DEFAULT_MAX_RETRY);
    put_command("AT+CIPMUX=0\n\r", DEFAULT_TIME_OUT_MS, DEFAULT_MAX_RETRY);
    put_command("AT+CSTT= \"internet\", \"\", \"\" \n\r", DEFAULT_TIME_OUT_MS, DEFAULT_MAX_RETRY);
    put_command("AT+CIICR\n\r", DEFAULT_TIME_OUT_MS, DEFAULT_MAX_RETRY);
    put_command("AT+CGATT?\n\r", DEFAULT_TIME_OUT_MS, DEFAULT_MAX_RETRY);
    put_command("AT+CIFSR\n\r", DEFAULT_TIME_OUT_MS, DEFAULT_MAX_RETRY);
}  */
void init_sms() {
    put_command("AT+CCALR?\n\r", DEFAULT_TIME_OUT_MS, DEFAULT_MAX_RETRY);

    put_command("ATE0\n\r", DEFAULT_TIME_OUT_MS, DEFAULT_MAX_RETRY);
 
    put_command("AT+CMGF=1\n\r", DEFAULT_TIME_OUT_MS, DEFAULT_MAX_RETRY);
 
    put_command("AT+CNMI=2,1,0,0,0\n\r", DEFAULT_TIME_OUT_MS, DEFAULT_MAX_RETRY);
 
    put_command("AT+CSAS\n\r", DEFAULT_TIME_OUT_MS, DEFAULT_MAX_RETRY);

    put_command("AT+CMGD=1\n\r", DEFAULT_TIME_OUT_MS, DEFAULT_MAX_RETRY);
}


void sms_read() {
    int i = 0, j = 0, quote_count = 0;
    char    command[10],
            phone_number[13];
    const int QUOTES_BEFORE_PHONE_NUMBER = 3;

    memset(command, '\0', 10);

    clear_buffer();

    is_show_clock = false;
    glcd_setfont(font5x7);
    glcd_clear(); 
    glcd_outtextxy(10,0,"Got new SMS\n");

    put_command("AT+CMGR=1\n\r", DEFAULT_TIME_OUT_MS, DEFAULT_MAX_RETRY);
 
    put_command("AT+CMGD=1\n\r", DEFAULT_TIME_OUT_MS, DEFAULT_MAX_RETRY);
 
    for (i = 0; receive_buffer[i] != '\0'; i++) {
        if (receive_buffer[i] == '\"') {
            quote_count++;
            j = 0;
            continue;
        }
        if (quote_count == QUOTES_BEFORE_PHONE_NUMBER) {
            phone_number[j++] = receive_buffer[i];
        }
        if (receive_buffer[i] == '*') {
            j = 0;
            while(receive_buffer[i - 1] != '#') {
                command[j++] = receive_buffer[i++];
                if ((receive_buffer[i] == '\r') && (receive_buffer[i+1] == '\n')) break; //co ve dong nay bo di cung khong sao?              
            }
            decrypt(command); 
            memset(command, '\0', 10); 
            break;
        }
    }
    glcd_outtextxy(0,10,phone_number);
    glcd_outtextxy(0,20,command);
    memset(command, '\0', 10); 
    delay_ms(2000);
    clear_buffer();
    glcd_clear();
    gui_init();    
    is_show_clock = true;
    clear_buffer();
}

void sms_send(unsigned char *s) {
    is_show_clock = false;
    glcd_setfont(font5x7);
    glcd_clear(); 
    glcd_outtextxy(0,0,"Sending SMS...\n");
    put_command("AT+CMGF=1\r\n", DEFAULT_TIME_OUT_MS, DEFAULT_MAX_RETRY);
    wait_until("OK", 2000);
    put_string("AT+CMGS=");
    putchar1(34);
    put_string(phone_number); 
    glcd_outtextxy(0,10,phone_number);
    putchar1(34);
    put_string("\r\n");    
    wait_until("> ", 5000); 
    while (*s) {
        putchar1(*s);
        s++;
    }
    putchar1(0x1A);
    glcd_outtextxy(15,30,"... done!");
    delay_ms(2000);
    glcd_clear();
    gui_init();    
    is_show_clock = true;
    clear_buffer();
}

void decrypt(unsigned char *s) {
    char message[160], command[10];
    int i = 0;
    memset(command, '\0', 10);
    memset(message, '\0', 160);
     while (*s) {
        command[i] = *s;
        s++;
        i++;
    }
    if (strstr(command, "on")) toggle_relay(ON, command);     //find string "on" in command and excute relay
    if (strstr(command, "off")) toggle_relay(OFF, command);
    if (strstr(command, "info")) {
        strcat(message, "1: ");
        if (relay_1 == ON) {
            strcat(message, "on");
        } else {
            strcat(message, "off");
        }
         strcat(message, "\r2: ");
        if (relay_2 == ON) {
            strcat(message, " on");
        } else {
            strcat(message, " off");
        }
        strcat(message, "\r3: ");
        if (relay_3 == ON) {
            strcat(message, " on");
        } else {
            strcat(message, " off");
        }
        strcat(message, "\r4: ");
        if (relay_4 == ON) {
            strcat(message, " on");
        } else {
            strcat(message, " off");
        }
        strcat(message, "\r5: ");
        if (relay_5 == ON) {
            strcat(message, " on");
        } else {
            strcat(message, " off");
        }
         sms_send(message);
    }
}

void toggle_relay (bool status, unsigned char *s) {
	int DEFAULT_ALARM_DURATION = 1;
    bool get_time = false;
    char command[10], duration_in_minute[4];
    int alarm_time_m = DEFAULT_ALARM_DURATION;
    int i = 0, j = 0;
    memset(command, '\0', 10);
    memset(duration_in_minute, '\0', 10);
     while (*s) {
        command[i] = *s;
        s++;
        i++;
    }
    /*
    for (i = 1; command[i] != '#'; i++) {
        if ((command[i - 1] == 'n') || (command[i - 1] == 'f')) { //on hoac off
            duration_in_minute[j] = command[i];
            j++;
        }
    }
    */
    for (i = 1; command[i] != '#'; i++) {
        if ((command[i - 1] == 'n') || (command[i - 1] == 'f')) { //on hoac off
            get_time = true;
        }
        if(get_time){
            duration_in_minute[j] = command[i];
            j++;
        }
    }
    alarm_time_m = atoi(duration_in_minute); 
    glcd_outtextxy(0,30,duration_in_minute); 
    if (alarm_time_m == 0) alarm_time_m = DEFAULT_ALARM_DURATION; //neu khong co thi cho no bang mac dinh
    switch (command[1]) {
        case '1':
            relay_1                 =   status;
            node[1].status          =   status;
            node[1].alarm_time_m    =   alarm_time_m;
        break;

        case '2':
            relay_2                 =   status;
            node[2].status          =   status;
            node[2].alarm_time_m    =   alarm_time_m;
        break;

        case '3':
            relay_3                 =   status;
            node[3].status          =   status;
            node[3].alarm_time_m    =   alarm_time_m;
        break;

        case '4':
            relay_4                 =   status;
            node[4].status          =   status;
            node[4].alarm_time_m    =   alarm_time_m;
        break;

        default:
            relay_1                 =   status;
            relay_2                 =   status;
            relay_3                 =   status;
            relay_4                 =   status;
            for (i = 1; i < 5; i++) 
                node[i].status      =   status;
        break;
    }
}


void gprs_read() {  
    int i=0;   
    is_show_clock = false;
    glcd_setfont(font5x7);
    glcd_clear();
    glcd_outtextxy(15,0,"Got GPRS CMD \n");
    put_string("AT+CIPSTART=\"TCP\",\"103.18.6.134\",80\r");
    wait_until("CONNECT OK", 15000);
    delay_ms(200);
    put_string("AT+CIPSEND\r");
    wait_until("> ", 4000);
    put_string("GET /irrigation/php/receivedata.php?value=70 HTTP/1.1\r\n"); 
    put_string("HOST: www.openrobohus.com\r\n");
    delay_ms(100);
    put_string("Connection: close\r\n\r\n");
    putchar1(0x1A);
    wait_until("OK", 1000);   
    //put_command("AT+CIPCLOSE\r\n",DEFAULT_TIME_OUT_MS,DEFAULT_MAX_RETRY);   
    
    for (i = 0; receive_buffer[i] != '\0'; i++) {
         glcd_putchar(receive_buffer[i]); 
         if (i % 20 == 0) {
            delay_ms(800);
            glcd_clear();}
    }
    //delay_ms(2000);    
     //glcd_outtextxy(15,0,receive_buffer);
  
    //p4=search(receive_buffer,"*4on");  
    //p3=search(receive_buffer,"*3on");   
    //p2=search(receive_buffer,"*2on");  
   // p1=search(receive_buffer,"*1on");  
        
    glcd_outtextxy(0,10,"ok");
    delay_ms(2000);
    clear_buffer();
    glcd_clear();
    delay_ms(100);
    gui_init();   
    delay_ms(100); 
    is_show_clock = true;
        
}
 

 

void gprs_send() {
   // PORTC.6=!PORTC.6;    
   int i=0;
    is_show_clock = false;
    glcd_setfont(font5x7);
    glcd_clear();
    glcd_outtextxy(15,0,"Send Data\n");
    put_string("AT+CIPSTART=\"TCP\",\"103.18.6.134\",80\r");
    wait_until("CONNECT OK", 20000);
    delay_ms(1000);
    put_string("AT+CIPSEND\r");
    wait_until("> ", 4000);
    put_string("GET /irrigation/php/senddata.php?value=1,5,2,6,22 HTTP/1.1\r\n"); 
    put_string("HOST: www.openrobohus.com\r\n");
    delay_ms(100);
    put_string("Connection: close\r\n\r\n");
    putchar1(0x1A);
    wait_until("OK", 1000);   
    //put_command("AT+CIPCLOSE\r\n",DEFAULT_TIME_OUT_MS,DEFAULT_MAX_RETRY);   
    
    for (i = 0; receive_buffer[i] != '\0'; i++) {
         glcd_putchar(receive_buffer[i]); 
         if (i % 20 == 0) {
            delay_ms(800);
            glcd_clear();}
    }
    delay_ms(2000);  
    
    glcd_outtextxy(0,10,"done");
    delay_ms(2000);
    clear_buffer();
    glcd_clear();
    delay_ms(100);
    gui_init();   
    delay_ms(100); 
    is_show_clock = true;
}

void call(){
    put_string("ATD+84983712941;\r\n");
}
void get_money_request(){
    is_get_money = true;
    glcd_clear();
    glcd_outtext("start get money\n");    
    put_string("AT+CUSD=1,");
    putchar1('"');
    put_string("*101#");
    putchar1('"');
    put_string("\r\n"); 
    if(sim == 0)
        get_money_response_vietnammobile();
    else if(sim == 1)
        get_money_response_viettel();           
}

void get_money_response_vietnammobile(){
    bool is_money = true;
    unsigned char buff_money;
    int count=0, j=0;
    glcd_clear();
    glcd_outtext("get money response"); 
    while(is_money){
        buff_money = getchar1();
        if(buff_money == ':'){
            count++; 
        }           
        if(count == 2){
            money[j] = buff_money;
            j++;    
        }       
        if(count == 2 && buff_money == 'd')
            is_money = false;
    }        
    sms_send(money);  
    delay_ms(2000);
    is_get_money = false;  
}

void get_money_response_viettel(){
    bool is_money = true;
    unsigned char buff_money;
    int count=0, j=0;
    glcd_clear();
    glcd_outtext("get money response"); 
    while(is_money){
        buff_money = getchar1();
        if(buff_money == '"'){
            count++; 
        }           
        if(count == 1 && buff_money > 45 && buff_money < 58){
            money[j] = buff_money;
            j++;    
        }       
        if(count == 1 && buff_money == 'd')
            is_money = false;
    }        
    /*glcd_clear();                    
    glcd_outtext("money: ");    
    glcd_outtext(money);*/
    sms_send(money);  
    delay_ms(2000);
    is_get_money = false;  
}

void put_money(){
    glcd_clear();
    glcd_outtext("put money on phone\n");
    glcd_outtext(money_code);
    put_string("AT+CUSD=1,");
    putchar1('"');
    put_string("*100*");
    put_string(money_code);
    putchar1('#');  
    putchar1('"');
    put_string("\r\n");
    delay_ms(4000);
    get_money_request(); 
}

   
   void test(){         
    int 
    i,d;

    //char ahi[]="fgvjhjbhgndxfcid=69#{'lenh':'*1off#'}{'lenh':'*2off#'}{'lenh':'*3on8#'}{'lenh':'*4on15#'} ";   
   
    p4=search(ahi,"*4on");  
    p3=search(ahi,"*3on");   
    p2=search(ahi,"*2on");  
    p1=search(ahi,"*1on");  
    
    for(d=0;d<strlen(ahi);d++){
    ahi[d]='\0';
    }
 }



int search( char nguon[], char *dau){
    int i,a,b=0,c=0;    
    char so;
    char begin[], cuoi[]="#";     
    strcpy(begin,strstr(nguon,dau)); 
    a=(strlen(begin)-strlen(strstr(begin,cuoi))); 
    for (i = 4; i<a ; i++) {  
         //glcd_putchar(begin[i]);
        c=begin[i]-48;  
         b=b*10+c;    
    }         
    delay_ms(2000); 
    if(strlen(begin)==0){b=-1;}; 
    //if(strlen(strstr(dau,"off"))!=0){b=0;};
     return b;  
   }  
   