/**
 * Beat Detector
 * Author Banana Hat
 * My attempt to use the anloritham discribed in
 * http://archive.gamedev.net/archive/reference/programming/features/beatdetection/index.html
 */
#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel strip = Adafruit_NeoPixel(50, 6, NEO_GRB + NEO_KHZ800);

#define SAMPLE_SIZE 12
#define INSTANCE_SAMPLE 128


int pMed = 70;

int pHigh = 120;

long samples[SAMPLE_SIZE];
int index = 0;
//double variance[8];
//int vIndex = 0;
//double vTotal = 1.1*8;
//float vAvg;

int pix = 25;
int curPix = 0;
int leftChannel = 5;
float avg = 0;
int pastPix[3];
long total = 0;
// instance average
float iavg = 0;
//current instance total
//int citotal = 0;
//last instance total
//int litotal = 5;
int debugbtn = 10;
int hm = 0; // hit/miss counter
int hitth = 1; // hit threshhold;
int base = 0;

int lastColor = 0;

/** 
 * The artical said to use arrays but I think the same could be achived
 * with pure math.
 */
void setup(){
  //Serial.begin(115200);
  pinMode(6, OUTPUT);
  pinMode(0, INPUT);
  //we'll start by getting an overall avrage of noise.
  for(int i = 0; i < SAMPLE_SIZE; i++){
    samples[i] = analogRead(leftChannel);
    total = total + samples[i];
    //variance[i] = 1.1;
    //vTotal = vTotal + variance[i];
  }
  //vAvg = vTotal/8;
  avg = total/SAMPLE_SIZE;
  base = avg;
  debug("base:", base);
}

void loop(){
  //this will be the avarge instance energy for 0.2 of a second.
  //1024 bytes should represent about 0.2 of a second.
  //set the current instance total to 0;
  debug("avarage:", avg);
  debug("total:", total);
  total = total - samples[index];
  samples[index] = 0;
  //samples[index] = getSample();
  samples[index] = getSample();
  iavg = samples[index]/float(INSTANCE_SAMPLE);
  // if we have more energy in our instance avarage than average we have beat! 
  debug("loop instance avarage:", iavg);
  debug("loop instance total:", samples[index]);
  //if(iavg > avg){
    //calVariance(iavg - avg);
  //}
  //debug("avarage variance:", vAvg);
   publishLEDs();
  // magic math!
  // replace the last instance total with the current instance total 
  //total = 0;
  //for(int i = 0; i < 128; i++){
    //total = total + samples[i];
  //}
  //samples[index] = iavg;
  total = total + samples[index];
    
   
  debug("value is:", samples[index]);
   
  index++;
  if(index == SAMPLE_SIZE){
    index = 0;
  }
  debug("index is:", index);
  debug("value is:", samples[index]);
  
  avg = total/float(SAMPLE_SIZE * INSTANCE_SAMPLE);
  
  //curPix = curPix * -1;
  //delay(30);
  if(curPix > pix) curPix = 0;
}

double getSample(){
    for(int i = 0; i < 128; i++){
      //accumulate the current instance total;
      double in = analogRead(leftChannel);
      //delay(1);
      in = abs(in - base);
      if(in > 0 ){
        samples[index] = samples[index] + in;
      }
    }
  return samples[index];
}

void debug(String lable, double value){
  /*if(digitalRead(debugbtn) == HIGH){
    Serial.print(lable);
    Serial.println(value);
  }*/
}

void publishLEDs(){
  
   if(avg > pHigh){
     theaterChase(makeColor((iavg - (avg*1) )*7));
     strip.show();     
     curPix = 0;
     return;
   }
  
  if((iavg + hitth) > (avg*1.10)){
    debug("Beat", iavg );
 
    if(avg > pHigh){
       theaterChase(makeColor((iavg - (avg*1) )*7));
       strip.show();     
       curPix = 0;
       return;
    }   
    strip.setPixelColor(pix + curPix, makeColor((iavg - (avg*1) )*7));
    strip.setPixelColor(pix - curPix, makeColor((iavg - (avg*1) )*7));
    strip.show();
    curPix++;
    hm + 10;
  }else{
    
    if(avg > pHigh){
       //theaterChase(makeColor(0));
       //strip.show();     
       //curPix = 0;
       return;
    }   
    
    dimmLEDS();
    hm- 10;
    
    
    
  }
  hitth = hitth + hm/2;

}
int q = 0;
long nextTime;
void theaterChase(uint32_t c) {
  //for (int j=0; j<1; j++) {  //do 10 cycles of chasing
    //for (int q=0; q < 3; q++) {

      if(millis() < nextTime){
        return;
      } 
      
      for (int i = q; i < strip.numPixels(); i=i+3) {
        uint32_t last = strip.getPixelColor(i);
        if (last < makeColor(50)){
          last = c;
        }
        strip.setPixelColor(pix + i, c);    //turn every third pixel on        
        strip.setPixelColor(pix - i, c);    //turn every third pixel on
        c = last;
        
      }
      //strip.show();
      
      for (int i = q-1; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(pix + i, 0);    //turn every third pixel on        
        strip.setPixelColor(pix - i, 0);    //turn every third pixel on
      }
      q++;
      if(q == 4){
        q = 0;
      }
      nextTime = millis() + 50;
    //}
  //}
}
int j = 0;
void coolPattern0(uint32_t c){
    //for (int j=0; j<1; j++) {  //do 10 cycles of chasing
    //for (int q=0; q < 3; q++) {
      for (int i = j; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(pix + i, c);    //turn every third pixel on        
        strip.setPixelColor(pix - i, c);    //turn every third pixel on
      }
      strip.show();
      
      for (int i = j-1; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(pix + i, 0);    //turn every third pixel on        
        strip.setPixelColor(pix - i, 0);    //turn every third pixel on
      }
      q++;
      if(j == pix){
        j = 0;
      }
    //}
  //}
}

void dimmLEDS(){
  
  if(avg > pHigh){
        /*lastColor = strip.getPixelColor(curPix);
        strip.setPixelColor(pix + curPix - 2, 0);
        strip.setPixelColor(pix + curPix - 3, lastColor);
        strip.setPixelColor(pix - curPix + 2, 0);
        strip.setPixelColor(pix - curPix + 3, lastColor);*/
    return;
  }
  
  uint32_t c;
  int j = 0;
  for(int i = 0; i < strip.numPixels(); i++){
    c = strip.getPixelColor(i);
    uint8_t
    r = ((uint8_t)(c >> 16)),
    g = ((uint8_t)(c >>  8)),
    b = ((uint8_t)c);
    strip.setPixelColor(i, r/1.1, g/1.1, b/1.1);
    j = j + r + g + b;
    if(avg < pMed){
      if (j < 100){
        curPix = 0;
      }
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
