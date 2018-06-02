#include "mbed.h"
#include "hcsr04.h"
#include "ESP8266.h"
#include "SDFileSystem.h"
#include  "FATFileSystem.h"
#include "DS3231.h"
#include "stm32_standby.h"
#include <stdio.h>

SDFileSystem sd(D11, D12, D13, D10, "sd");
HCSR04 sensor(D4, D3);
Serial pc(USBTX, USBRX);
DS3231 rtc(D14,D15);
FILE *fp;                                   // File pointer declear

DigitalOut esp8266_reset(D7, 1);

//wifi UART port and baud rate
ESP8266 wifi(D8, D2, 115200);

//buffers for wifi library
char resp[1000];
char http_cmd[300], comm[300];
int dw,d,M,y,h,m,s;  //VARIABLES FOR TIME AND YEAR
int timeout = 8000; //timeout for wifi commands

//SSID and password for connection
#define SSID "Jan iPhone"
#define PASS "mbedcloud"

 //Remote IP
#define IP "184.106.153.149"

//global variable
long distance;

//Update key for thingspeak
const char* Update_Key = "JNP1LEUBKU74MCJ0";

bool check_if_connected(char *resp) {
    return strstr(resp, "WIFI GOT IP") != NULL;
}

void wifi_after_connected();

//Wifi init function
void wifi_initialize(void){

    pc.printf("******** Resetting wifi module ********\r\n");
    wifi.Reset();

    //wait for 5 seconds for response, else display no response receiveed
    if (wifi.RcvReply(resp, 5000)) {
        // pc.printf("%s",resp);
    }
    else {
        pc.printf("No response");
    }

    if (check_if_connected(resp)) {
        pc.printf("Connected\r\n");
        wifi_after_connected();
        return;
    }

    pc.printf("******** Setting Station mode of wifi with AP ********\r\n");
    wifi.SetMode(1);    // set transparent  mode
    if (wifi.RcvReply(resp, timeout))    //receive a response from ESP
        pc.printf("%s",resp);    //Print the response onscreen
    else
        pc.printf("No response while setting mode. \r\n");

    if (check_if_connected(resp)) {
        pc.printf("Connected\r\n");
        wifi_after_connected();
        return;
    }

    pc.printf("******** Joining network with SSID and PASS ********\r\n");
    wifi.Join(SSID, PASS);
    if (wifi.RcvReply(resp, timeout))
        pc.printf("%s",resp);
    else
        pc.printf("No response while connecting to network \r\n");

    wifi_after_connected();
}

void wifi_after_connected() {
    // pc.printf("******** Getting IP and MAC of module ********\r\n");
    // wifi.GetIP(resp);
    // if (wifi.RcvReply(resp, timeout))
    //     pc.printf("%s",resp);
    // else
    //     pc.printf("No response while getting IP \r\n");

    pc.printf("******** Setting WIFI UART passthrough ********\r\n");
    wifi.setTransparent();
    if (wifi.RcvReply(resp, timeout))
        pc.printf("%s",resp);
    else
        pc.printf("No response while setting wifi passthrough. \r\n");
    wait(1);

    pc.printf("******** Setting single connection mode ********\r\n");
    wifi.SetSingle();
    wifi.RcvReply(resp, timeout);
    if (wifi.RcvReply(resp, timeout))
        pc.printf("%s",resp);
    else
        pc.printf("No response while setting single connection \r\n");
    wait(1);
}

void wifi_send() {

 // HERE IS THE CODE FOR OBTAINING TIME FROM THE DS3231


 //END OF CODE USED FOR OBTAINING TIME
    sensor.start();
    wait_ms(100);
    long distance=sensor.get_dist_cm();
     printf("distance is %dcm\n", distance);
     wait(1);
    // SD card logger
       fp = fopen("/sd/mylogger.txt", "a");            // File open for "a"ppend
    if (fp == NULL) {                               // Error!
            pc.printf("Unable to write the file\r\n");
        }
    else {
    rtc.readDateTime(&dw,&d,&M,&y,&h,&m,&s);
    pc.printf("%02d/%02d/%4d\n",d,M,y);
    pc.printf("%02d:%02d:%02d",h,m,s);
    pc.printf(",");

    fprintf(fp, "%02d/%02d/%4d\n",d,M,y);
    fprintf(fp, ",");
    fprintf(fp, "%02d:%02d:%02d",h,m,s);
    fprintf(fp, ",");
    pc.printf("%dcm ",distance);          // Append data to SD card.
    fprintf(fp, "%dcm\r\n",distance);        // Serial monitor.
        }
    fclose(fp);                                     // Cloce file.
    pc.printf("File successfully written!\r\n");    // Serial monitor.


        wait(2);


    pc.printf("******** Starting TCP connection on IP and port ********\r\n");
    wifi.startTCPConn(IP,80);    //cipstart
    wifi.RcvReply(resp, timeout);
    if (wifi.RcvReply(resp, timeout))
        pc.printf("%s",resp);
    else
        pc.printf("No response while starting TCP connection \r\n");
    wait(0.25);

    //create link
    sprintf(http_cmd,"/update?api_key=%s&field1=%d",Update_Key,distance);
    pc.printf(http_cmd);

    pc.printf("******** Sending URL to wifi ********\r\n");
    wifi.sendURL(http_cmd, comm);   //cipsend and get command
}

void update_ThingSpeak()
{
        wifi_send();
        wait(0.25);
}

//to be called when wifi fails to initialize
void use_sdCard() {

}

int main() {
    set_time(0);

    pc.baud(115200);

    pc.printf("******** MCU Woke Up ********\r\n");

    esp8266_reset = 0;
    wait_ms(20);
    esp8266_reset = 1;

    pc.printf("******** ESP8266 Woke Up ********\r\n");

    wifi_initialize();

    while (1) {
        wifi_send();

        pc.printf("******** Done sending, putting to sleep ********\r\n");
        wifi.DeepSleep();

        pc.printf("******** ESP8266 is sleeping, putting MCU to sleep ********\r\n");

        standby(10);
    }

}
