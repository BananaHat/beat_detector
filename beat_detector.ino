/**
 * Beat Detector
 * Author Banana Hat
 * My attempt to use the anloritham discribed in
 * http://archive.gamedev.net/archive/reference/programming/features/beatdetection/index.html
 */
#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel strip = Adafruit_NeoPixel(50, 6, NEO_GRB + NEO_KHZ800);
double samples[8];
int index = 0;
//double variance[8];
//int vIndex = 0;
//double vTotal = 1.1*8;
//float vAvg;

int pix = 25;
int curPix = 0;
int leftChannel = 0;
float avg = 0;
int pastPix[3];
double total = 0;
// instance average
float iavg = 0;
//current instance total
int citotal = 0;
//last instance total
int litotal = 5;
int debugbtn = 10;
int hm = 0; // hit/miss counter
int hitth = 130; // hit threshhold;
/** 
 * The artical said to use arrays but I think the same could be achived
 * with pure math.
 */
void setup(){
  //Serial.begin(115200);
  pinMode(6, OUTPUT);
  pinMode(0, INPUT);
  //we'll start by getting an overall avrage of noise.
  for(int i = 0; i < 8; i++){
    samples[i] = analogRead(leftChannel);
    total = total + samples[i];
    //variance[i] = 1.1;
    //vTotal = vTotal + variance[i];
  }
  //vAvg = vTotal/8;
  avg = total/8;
}

void loop(){
  //this will be the avarge instance energy for 0.2 of a second.
  //1024 bytes should represent about 0.2 of a second.
  //set the current instance total to 0;
  //debug("avarage:", avg);
  //debug("total:", total);
  total = total - samples[index];
  samples[index] = 0;
  //int time = millis();
  for(int i = 0; i < 128; i++){
    //accumulate the current instance total;
    double in = analogRead(leftChannel);
    if(in > 0 ){
      samples[index] = samples[index] + in;
    }
  }
  //debug("loop execution ti:", iavg);
  iavg = samples[index]/128;
  // if we have more energy in our instance avarage than average we have beat! 
  //debug("loop instance avarage:", iavg);
  //debug("loop instance total:", samples[index]);
  //if(iavg > avg){
    //calVariance(iavg - avg);
  //}
  //debug("avarage variance:", vAvg);
  if(iavg > (avg*130)){
   //debug("Beat", iavg );
   strip.setPixelColor(pix + curPix, makeColor((iavg - (avg*100) )*100));
   strip.setPixelColor(pix - curPix, makeColor((iavg - (avg*100) )*100));
   strip.show();
   curPix++;
   hm + 10;
  }else{
    dimmLEDS();
    hm- 10;
  }
  hitth = hitth + hm/2;
  // magic math!
  // replace the last instance total with the current instance total 
  //total = 0;
  //for(int i = 0; i < 128; i++){
    //total = total + samples[i];
  //}
  samples[index] = iavg;
  total = total + samples[index];
  index++;
  if(index == 8){
    index = 0;
  }
  
  avg = total/1024;
  
  //curPix = curPix * -1;
  if(curPix > pix) curPix = 0;
  
}

void debug(String lable, double value){
  if(digitalRead(debugbtn) == HIGH){
    Serial.print(lable);
    Serial.println(value);
  }
}

void dimmLEDS(){
  uint32_t c;
  int j = 0;
  for(int i = 0; i < 58; i++){
    c = strip.getPixelColor(i);
    uint8_t
    r = ((uint8_t)(c >> 16)),
    g = ((uint8_t)(c >>  8)),
    b = ((uint8_t)c);
    strip.setPixelColor(i, r/1.1, g/1.1, b/1.1);
    j = j + r + g + b;
    if (j < 100){
      curPix = 0;
    }
  }
  strip.show();
}
/*
int calVariance(double newV){
  if(newV < 0){
    newV = 0;
  }
  vTotal = vTotal - variance[vIndex];
  variance[vIndex] = newV;
 // vTotal = vTotal + variance[vIndex];
 // avg = total/8;
 // vIndex++;
 // if(vIndex = 8){
   // vIndex = 0;
  //}
}
*/
uint32_t makeColor(float val){
 //Serial.println("color value");
 //Serial.println(val);
 if(val < 127)return strip.Color(val, 0, 0);
 if(127 < val && val <= 254)return strip.Color(127-(val-127), val-127, 0);
 if(254 < val && val <= 381)return strip.Color(0, 127-(val-254), val- 254);
 if(381 < val && val <= 508)return strip.Color(val- 381, 0, 127);
 if(508 < val)return strip.Color(127, val- 508, 127);
  
}
