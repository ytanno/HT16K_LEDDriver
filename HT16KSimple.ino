//ref
//https://www.digikey.com/htmldatasheets/production/2070902/0/0/1/ht16k33.html#pfd
//https://www.mgo-tec.com/blog-entry-ada-ledmatrix01.html

#include <Wire.h>
#define addr (0x70)

#define com 16
#define A 8
int a[com][A];//com x A
int incomingByte;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();
  LED_Driver_Setup(addr, 1); //on 1, off 0
  LED_Driver_Blink(addr, 1, 0); //blink_Hz=0 点滅off, 1は2Hz, 2は1Hz, 3は0.5Hz, on_off=0は消灯、1は点灯
  LED_Driver_Brightness( addr, 15 ); // brightness= 0～15 ? 仕様書に書いてない
  delay(1000);
  //a[0][0] = 0;
}

void init_led_array(){
  for (int i = 0; i < com; i++){
    for(int j = 0; j < A; j++){
      a[i][j] = 0;
    }
  }
}

void set_all_one(){
  for (int i = 0; i < com; i++){
    for(int j = 0; j < A; j++){
      a[i][j] = 1;
    }
  }
}



void loop() {

  for (int   i = 0; i < 3; i++){
    for (int j = 0; j < 3; j++){
       a[i][j] = 1;
       update_LED();
       delay(50);
       init_led_array();
    }
  }
  
}

void update_LED(){
  for (int i = 0; i < com; i++)
  {
    Wire.beginTransmission(addr);
    Wire.write(i*2); //2個制御のため、１個制御の時はアドレスを飛ばす必要がある。

    int bit8 = 0b00000000;
    for (int j = 0; j < A; j++)
    {
      bit8 = a[i][j]<<j | bit8;
    }
    //Serial.println(bit8);
    Wire.write(bit8);
    Wire.endTransmission();
  }

}

void LED_Driver_DisplayInt(byte LD_addrs)
{
  Wire.beginTransmission(LD_addrs);
  Wire.write(0x00);
  for(int i=0;i<16;i++){  
        Wire.write(B00000000);                   //1つ目の8x8LED初期化
  }
  Wire.endTransmission();
}

void LED_Driver_Brightness(byte LD_addrs, byte brightness)
{
  // brightness= 0～15
  Wire.beginTransmission(LD_addrs);
  Wire.write(0xE0 | brightness);
  Wire.endTransmission();
}

void LED_Driver_Setup(byte LD_addrs, byte on_off)
{
  Wire.beginTransmission(LD_addrs);
  Wire.write(0x20 | on_off); 
  Wire.endTransmission();
}

void LED_Driver_Blink(byte LD_addrs, byte on_off, byte blink_Hz)
{
  //blink_Hz=0 点滅off, 1は2Hz, 2は1Hz, 3は0.5Hz, on_off=0は消灯、1は点灯　
  Wire.beginTransmission(LD_addrs);
  Wire.write(0x80 | (blink_Hz<<1) | on_off); 
  Wire.endTransmission();
}
