#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

const int analogPin = A0;
#define TURBIDITY_PIN A1

#define TDS_THRESHOLD 500 
#define TURBIDITY_THRESHOLD 30 
float calibration_value = 19.34;
unsigned long int avgval; 
int buffer_arr[10],temp;
int pHValue =0;
int sensorValue;
int turbidityValue;


SoftwareSerial espSerial(2, 3);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  espSerial.begin(9600);
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Water Quality ");
  lcd.setCursor(0,1);
  lcd.print("Test ");
 
 

  delay(5000);
}






void loop() {
  int sensorValue = analogRead(analogPin); // read the sensor value
  float voltage = sensorValue * (5.0 / 1023.0); // convert the sensor value to voltage
  float tdsValue = (133.42 * pow(voltage, 3)) - (255.86 * pow(voltage, 2)) + (857.39 * voltage);
  int turbidity = analogRead(TURBIDITY_PIN);
  int turbidityValue = map(turbidity, 0, 750, 100, 0);

  
  for(int i=0;i<10;i++) 
 { 
 buffer_arr[i]=analogRead(A2);
 delay(30);
 }
 for(int i=0;i<9;i++)
 {
 for(int j=i+1;j<10;j++)
 {
 if(buffer_arr[i]>buffer_arr[j])
 {
 temp=buffer_arr[i];
 buffer_arr[i]=buffer_arr[j];
 buffer_arr[j]=temp;
 }
 }
 }
 avgval=0;
 for(int i=2;i<8;i++)
 avgval+=buffer_arr[i];
 float volt=(float)avgval*5.0/1024/6;
 float pHValue = -5.70 * volt + calibration_value;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("TDS:");
  lcd.print(tdsValue);
  lcd.print("PH: ");
  lcd.print(pHValue);
  lcd.setCursor(0,1);
  lcd.print("Turbidity:");
  lcd.print(turbidityValue);

  Serial.print("pH value: ");
  Serial.println(pHValue);
  Serial.print("Turbidity value: ");
  Serial.println(turbidityValue);
  Serial.print("TDS value: ");
  Serial.println(tdsValue);

  espSerial.print("pH value: ");
  espSerial.println(pHValue);
  espSerial.print("Turbidity value: ");
  espSerial.println(turbidityValue);
  espSerial.print("TDS value: ");
  espSerial.println(tdsValue);
  delay(1000);
 
  
  if (tdsValue > TDS_THRESHOLD) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Water Quality");
    lcd.setCursor(0,1);
    lcd.print(":POOR(TDS)");
    Serial.println("Water Quality: Poor");  
     
    
  } 
    else if (pHValue < 6.00 || pHValue > 7.80) {
      lcd.setCursor(0, 0);
      lcd.print("Water Quality");
      lcd.setCursor(0, 1);
      lcd.print(":is Poor(ph))");
     Serial.println("Water Quality: Poor");
     
  }

    
    
    else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Water Quality");
    lcd.setCursor(0,1);
    lcd.print(": Good");
    Serial.println("Water Quality: good"); 
        
       
    
delay(1000);  
}
}
