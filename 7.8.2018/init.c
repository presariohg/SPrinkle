interrupt [TIM0_OVF] void timer0_ovf_isr(void);
void init_system(){
GLCDINIT_t glcd_init_data;
    {
        
        DDRA=(1<<DDA7) | (1<<DDA6) | (0<<DDA5) | (0<<DDA4) | (0<<DDA3) | (1<<DDA2) | (0<<DDA1) | (0<<DDA0);
        PORTA=(1<<PORTA7) | (1<<PORTA6) | (1<<PORTA5) | (1<<PORTA4) | (1<<PORTA3) | (1<<PORTA2) | (0<<PORTA1) | (0<<PORTA0);


        DDRB=(0<<DDB7) | (1<<DDB6) | (1<<DDB5) | (1<<DDB4) | (1<<DDB3) | (0<<DDB2) | (0<<DDB1) | (0<<DDB0);
        PORTB=(1<<PORTB7) | (1<<PORTB6) | (1<<PORTB5) | (1<<PORTB4) | (1<<PORTB3) | (1<<PORTB2) | (0<<PORTB1) | (0<<PORTB0);

        DDRC=(1<<DDC7) | (1<<DDC6) | (0<<DDC5) | (0<<DDC4) | (0<<DDC3) | (0<<DDC2) | (0<<DDC1) | (0<<DDC0);
        PORTC=(1<<PORTC7) | (0<<PORTC6) | (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (0<<PORTC2) | (0<<PORTC1) | (0<<PORTC0);

        DDRD=(1<<DDD7) | (0<<DDD6) | (0<<DDD5) | (1<<DDD4) | (0<<DDD3) | (0<<DDD2) | (0<<DDD1) | (0<<DDD0);
        PORTD=(1<<PORTD7) | (0<<PORTD6) | (0<<PORTD5) | (1<<PORTD4) | (0<<PORTD3) | (0<<PORTD2) | (0<<PORTD1) | (0<<PORTD0);

        DDRE=(0<<DDE7) | (0<<DDE6) | (0<<DDE5) | (0<<DDE4) | (0<<DDE3) | (0<<DDE2) | (0<<DDE1) | (0<<DDE0);
        PORTE=(0<<PORTE7) | (0<<PORTE6) | (0<<PORTE5) | (0<<PORTE4) | (0<<PORTE3) | (0<<PORTE2) | (0<<PORTE1) | (0<<PORTE0);

        DDRF=(0<<DDF7) | (0<<DDF6) | (0<<DDF5) | (0<<DDF4) | (0<<DDF3) | (0<<DDF2) | (0<<DDF1) | (0<<DDF0);
        PORTF=(0<<PORTF7) | (0<<PORTF6) | (0<<PORTF5) | (0<<PORTF4) | (0<<PORTF3) | (0<<PORTF2) | (0<<PORTF1) | (0<<PORTF0);

        DDRG=(0<<DDG4) | (0<<DDG3) | (0<<DDG2) | (0<<DDG1) | (0<<DDG0);
        PORTG=(0<<PORTG4) | (0<<PORTG3) | (0<<PORTG2) | (0<<PORTG1) | (0<<PORTG0);

        // Timer Period: 20 ms
        ASSR=0<<AS0;
        TCCR0=(0<<WGM00) | (0<<COM01) | (0<<COM00) | (0<<WGM01) | (1<<CS02) | (1<<CS01) | (1<<CS00);
        TCNT0=0x64;
        OCR0=0x00;

        
        TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (0<<TOIE1) | (0<<OCIE0) | (1<<TOIE0);
        ETIMSK=(0<<TICIE3) | (0<<OCIE3A) | (0<<OCIE3B) | (0<<TOIE3) | (0<<OCIE3C) | (0<<OCIE1C);

        // USART1 2400 double
        UCSR1A=(0<<RXC1) | (0<<TXC1) | (0<<UDRE1) | (0<<FE1) | (0<<DOR1) | (0<<UPE1) | (0<<U2X1) | (0<<MPCM1);
        UCSR1B=(1<<RXCIE1) | (1<<TXCIE1) | (0<<UDRIE1) | (1<<RXEN1) | (1<<TXEN1) | (0<<UCSZ12) | (0<<RXB81) | (0<<TXB81);
        UCSR1C=(0<<UMSEL1) | (0<<UPM11) | (0<<UPM10) | (0<<USBS1) | (1<<UCSZ11) | (1<<UCSZ10) | (0<<UCPOL1);
        UBRR1H=0x00;
        UBRR1L=0x33;

        i2c_init(); //100khz
        rtc_init(3,1,0); //32768Hz

        glcd_init_data.font=font5x7;
        glcd_init_data.temp_coef=89;
        glcd_init_data.bias=3;
        glcd_init_data.vlcd=59;  //55 //58
        glcd_init(&glcd_init_data);

        #asm("sei")
    }
}
interrupt[TIM0_OVF] void timer0_ovf_isr() {
    unsigned char prev_sec = second;
    int i = 0;
    char b;
    TCNT0 = 0x64; //20 ms
    milisec += 20; 
    dem++ ;
    if (milisec % 60000 == 0) { //60 secs
    	for (i = 1; i <= 5; i++) {
    		node[i].alarm_time_m--;
    		if (node[i].alarm_time_m == 0) {
    			switch (i) {
    				case 1:
    					relay_1 = !relay_1;
    				break;
    				case 2:
    					relay_2 = !relay_2;
    				break;
    				case 3:
    					relay_3 = !relay_3;
    				break;
    				case 4:
    					relay_4 = !relay_4;
    				break;
    				case 5:
    					relay_5 = !relay_5;
                    break;
                }
                node[i].status = !node[i].status;
            }
        }
      //  gprs_read();
    }  
            
    //4
   /* if(p4!=-1&&p4!=0)
    {
        dem4=0;  
        relay_4=ON;
        a4=p4*50;
        p4=-1;
    }
    if(p4==0){relay_4=OFF;} ;
    if(dem4==a4){
        relay_4=OFF;
        a4=-1;
        }
    if(dem4<a4){
        dem4++;
    };  
    //3 
    if(p3!=-1&&p3!=0)
    {
        dem3=0;  
        relay_3=ON;
        a3=p3*50;
        p3=-1;
    } 
    if(p3==0){relay_3=OFF;} ;
        if(dem3==a3){
            relay_3=OFF;
            //delay_ms(1000); 
            a3=-1;
        }
        if(dem3<a3){
            dem3++;
           // glcd_putchar(final[1]); 
            //delay_ms(20);
        };  
     //2   
     if(p2!=-1&&p2!=0)
    {
        dem2=0;  
        relay_2=ON;
        a2=p2*50;
        p2=-1;
    } 
    if(p2==0){relay_2=OFF;} ;
        if(dem2==a2){
            relay_2=OFF;
            a2=-1;
        }
        if(dem2<a2){
            dem2++;
        }; 
    //1    
      if(p1!=-1&&p1!=0)
    {
        dem1=0;  
        relay_1=ON;
        a1=p1*50;
        p1=-1;
    }
    if(p1==0){relay_1=OFF;} ; 
        if(dem1==a1){
            relay_1=OFF;
            a1=-1;
        }
        if(dem1<a1){
            dem1++;
        };  */    
    // het       
   /*if (dem == 800) { //5 mins
     // gprs_send();          
     is_show_clock = false;    
    glcd_setfont(font5x7);  
    glcd_clear(); 
     glcd_outtextxy(0,30,final);
     a++;
      b=a+48;
      glcd_putchar(b); 
      dem=0; //send cai gi do, sua lai trong ham send
       
     }   
     if(a==5){glcd_outtextxy(0,20,"ok");}*/  
     //if (dem == 800) { //800*20 <=>16s
      //  gprs_read(); 
      //  gprs_send();
      //  dem=0;
       
    // }
     if (is_show_clock) {
        rtc_get_time(&hour, &minute, &second);
        if (second != prev_sec) {
            gui_print_time();
        }
        prev_sec = second; 
        if (milisec % 500 == 0) {
            gui_colon_blink();
        }
    }
    if (milisec > 4000000000) milisec = 0; //gioi han cua unsigned long
    for (i = 1; i <= 5; i++) {
        if ((node[i].active_hour[hour] == true) && (minute == 0)){
            switch (i) {
                case 1:
                    relay_1                 =   ON;
                    node[1].status          =   ON;
                    node[1].alarm_time_m    =   1;
                break;
                case 2:
                    relay_2                 =   ON;
                    node[2].status          =   ON;
                    node[2].alarm_time_m    =   1;
                break;
                case 3:
                    relay_3                 =   ON;
                    node[3].status          =   ON;
                    node[3].alarm_time_m    =   1;
                break;
                case 4:
                    relay_4                 =   ON;
                    node[4].status          =   ON;
                    node[4].alarm_time_m    =   1;
                break;
                case 5:
                    relay_5                 =   ON;
                    node[5].status          =   ON;
                    node[5].alarm_time_m    =   1;
                break; 
            }
        }
    }
    
}
