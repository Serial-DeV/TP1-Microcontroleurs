#include "mbed.h"

void initLCD();
void write_command(char c);
void write_data(char c);
void print_LCD_char(char c);
void print_LCD_String(char *s);
void initLED();
void configLED(char b, char g, char r, char a);
void print_LCD_int(int en);
char* itoa(int i, char b[]);

I2C i2c(I2C_SDA , I2C_SCL );
 
int LCD_addr = 0x7C;
int LED_addr = 0xC4;
int i = 0;
char*str;
char ch[36];
 
int main() {
    initLED();
    configLED(0x00, 0xFF, 0xFF, 0xFF);
    
    initLCD();
    while(1){
        wait(0.5);
        write_command(0x01);
        wait(0.00153);
        write_command(0xC0);
        if(i%5 == 0) configLED(0xFF, 0xFF, 0xFF, 0xFF);//Noir
        else if(i%4 == 0) configLED(0x00, 0xFF, 0xFF, 0xFF);//Bleu
        else if(i%3 == 0) configLED(0xFF, 0x00, 0xFF, 0xFF);//Vert
        else if(i%2 == 0) configLED(0xFF, 0xFF, 0x00, 0xFF);//Rouge
        else configLED(0x00, 0x00, 0x00, 0x00);//Blanc
        str = itoa(i,ch);
        print_LCD_String(str);
        i++;
        
    }
    
    //print_LCD_String("Hello w80");
    //print_LCD_char(99);
}

void initLED(){
    
    char cmd[3];
    wait(0.04);
    cmd[0] = 0x80;
    cmd[1] = 0xFF;
    cmd[2] = 0xFF;
    i2c.write(LED_addr, cmd, 3, false);
    cmd[0] = 0x08;
    cmd[1] = 0xAA;
    i2c.write(LED_addr, cmd, 2, false);
    cmd[0] = 0x80;
    cmd[1] = 0xAA;
    i2c.write(LED_addr, cmd, 2, false);
}

void configLED(char b, char g, char r, char a){
    
    char cmd[5];
    cmd[0] = 0xA2;
    cmd[1] = b;
    cmd[2] = g;
    cmd[3] = r;
    cmd[4] = a;
    i2c.write(LED_addr, cmd, 5, false);
    
}
    

void initLCD(){
    wait(0.04);
    write_command(0x3C);//4 pour mode 1 ligne(Celle du haut), C pour 2
    write_command(0x0C);
    write_command(0x01);
    wait(0.0016);
    write_command(0x06);
    
    
}

void write_command(char c){
    char cmd[2];
    cmd[0] = 0x80;
    cmd[1] = c;
    i2c.write(LCD_addr, cmd, 2, false);//false car répété
}

void write_data(char c){
    char cmd[2];
    cmd[0] = 0x40;
    cmd[1] = c;
    i2c.write(LCD_addr, cmd, 2, false);
}

void print_LCD_char(char c){
    char cmd[2];
    cmd[0] = 0x40;
    cmd[1] = c;
    i2c.write(LCD_addr, cmd, 2, false);
}

void print_LCD_String(char *s){
    char cmd[36];
    cmd[0] = 0x40;
    for(int n = 1; n < strlen(s)+1; n++){
        cmd[n] = s[n-1];
    }
    
    
    i2c.write(LCD_addr, cmd, strlen(s)+1, false);
}

char* itoa(int i, char b[]){
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}
