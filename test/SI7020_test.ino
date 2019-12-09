/* DFrobot SI7020 Source: https://wiki.dfrobot.com/SI7021_Temperature_and_humidity_sensor_SKU_TOY0054 */
/* Robotistan.com the product sale link: https://www.robotistan.com/si7021-sicaklik-ve-nem-sensor-karti 
 Robotistan.com SI7021 Sıcaklık ve Nem Sensör Kartı: https://www.robotistan.com/si7021-sicaklik-ve-nem-sensor-karti 
 Robotistan Ürün Kodu:15131 


SI7021 sıcaklık ve nem sensörü, analog-dijital çevirici, sinyal işleme devresi ve I2C haberleşme devresini bir arada barındıran bir entegredir.

Özellikler:
Çalışma gerilimi: 1.9 - 3.6V (tavsiye edilen: 3.3V)
Boşta çektiği akım: 60nA
Sıcaklık ölçüm aralığı: -40 - 85 Celsius (tavsiye edilen:  -10 - 60 Celsius)
Sıcaklık hassasiyeti: ±0.4℃ (-10 - 85C arasında)
Nem ölçüm aralığı: %0 - %100 bağıl nem (tavsiye edilen: %20-%80 bağıl nem)
Nem ölçüm hassasiyeti: ± %3 (%20-%80 aralığında)
Sıcaklık ölçümü çevrim süresi: 7ms
Nem ölçümü çevrim süresi: 17ms (nem ölçümü yapıldığında otomatik olarak sıcaklık ölçümü de yapılır)
Haberleşme arayüzü: I2C
Boyutlar: 13mm x 10mm
*/
#include <Wire.h>

const int ADDR =0x40;
int X0,X1,Y0,Y1,Y2,Y3;
double X,Y,X_out,Y_out1,Y_out2;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  delay(100);
  Wire.beginTransmission(ADDR);
  Wire.endTransmission();
  pinMode(13,OUTPUT);
}
void loop()
{
  /**Send command of initiating temperature measurement**/
  Wire.beginTransmission(ADDR);
  Wire.write(0xE3);
  Wire.endTransmission();

  Serial.print("Temp");
  Serial.print("\t");
  Serial.println("RH");

  /**Read data of temperature**/
  digitalWrite(13,HIGH);
  Wire.requestFrom(ADDR,2);

  if(Wire.available()<=2);
  {
    X0 = Wire.read();
    X1 = Wire.read();
    X0 = X0<<8;
    X_out = X0+X1;
  }

  /**Calculate and display temperature**/
  X=(175.72*X_out)/65536;
  X=X-46.85;
  Serial.print(X);
  Serial.print("C");
  Serial.print("\t");

  /**Send command of initiating relative humidity measurement**/
  Wire.beginTransmission(ADDR);
  Wire.write(0xE5);
  Wire.endTransmission();

  /**Read data of relative humidity**/
  Wire.requestFrom(ADDR,2);
  if(Wire.available()<=2);
  {
    Y0 = Wire.read();
    Y2=Y0/100;
    Y0=Y0%100;
    Y1 = Wire.read();
    Y_out1 = Y2*25600;
    Y_out2 = Y0*256+Y1;
  }

  /**Calculate and display relative humidity**/
  Y_out1 = (125*Y_out1)/65536;
  Y_out2 = (125*Y_out2)/65536;
  Y = Y_out1+Y_out2;
  Y=Y-6;
  Serial.print(Y);
  Serial.println("%");

  digitalWrite(13,LOW);
  delay(300);
}