#include <FastLED.h>
#include <arduinoFFT.h>

#define LED_PIN     14
#define MIC_IN      35

#define NUM_LEDS    150
int BRIGHTNESS = 150;
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB

#define SAMPLES 64
#define xres 15     //Cols
#define yres 10     //Rows

double vReal[SAMPLES];
double vImag[SAMPLES];

//int Intensity[xres] = {0 };
int Intensity[yres] = {0};
int Displacement = 1;

CRGB leds[NUM_LEDS];
arduinoFFT FFT = arduinoFFT();



void setup() {
  //analogReference(EXTERNAL);
  pinMode(MIC_IN, INPUT);
  Serial.begin(115200);
  // put your setup code here, to run once:
  delay( 3000 );
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );
  
}

void loop() {
  // put your main code here, to run repeatedly:
  visualizer();
}

void visualizer(){
  //getSamples();
  displayUpdate();
  delay(10);
  FastLED.show();
}

void displayUpdate(){
  int color = 0;
  //int color = map(analogRead(A2), 40, 1000, 0, 255);
  int bright = map(analogRead(A3), 0, 1000, 0, 240);
  // i is columns
  /*for(int i=0; i<xres; i++){
    // j is rows
    for(int j=0; j<yres; j++){
      if(j<= Intensity[i]){
        if(j%2 != 0){
          leds[(xres*(j+1))-i-1] = CHSV(color, 255, bright);
        }
        else {
          leds[(xres*j)+i] = CHSV(color, 255, bright);
        }
      }
      else {
        if(j%2!=0){
          leds[(xres*(j+1))-i-1] = CHSV(color, 255, 0);
        }
        else {
          leds[(xres*j)+i] = CHSV(color, 255, 0);
        }
      }
    }
    //color += 255/xres;
  }*/
  for(int i=0; i<yres; i++){
    // j is rows
    color = 240;
    for(int j=0; j<xres; j++){
      if(j<= Intensity[i]){
        if(i%2 == 0){
          leds[i*xres+j] = CHSV(color, 255, bright);
        }
        else {
          leds[xres*(i+1)-j-1] = CHSV(color, 255, bright);
        }
      }
      else {
        if(i%2==0){
          leds[i*xres+j] = CHSV(color, 255, 0);
        }
        else {
          leds[xres*(i+1)-j-1] = CHSV(color, 255, 0);
        }
      }
       color -= 255/xres;
    }
   
  }
}

void getSamples(){

  for(int i=0; i<SAMPLES; i++){
    vReal[i] = analogRead(MIC_IN);
    //Serial.println(vReal[i]);
    vImag[i] = 0;
  }

  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);

  /*for(int i=2; i < (xres*Displacement)+2; i+=Displacement){
    vReal[i] = constrain(vReal[i], 0, 2047);
    vReal[i] = map(vReal[i], 0, 2047, 0, yres);

    Intensity[(i/Displacement)-2]--;
    if(vReal[i] > Intensity[(i/Displacement)-2])
      Intensity[(i/Displacement)-2] = vReal[i];
  }*/
  for(int i=2; i < (yres*Displacement)+2; i+=Displacement){
    vReal[i] = constrain(vReal[i], 0, 2047);
    vReal[i] = map(vReal[i], 0, 2047, 0, yres);

    Intensity[(i/Displacement)-2]--;
    if(vReal[i] > Intensity[(i/Displacement)-2])
      Intensity[(i/Displacement)-2] = vReal[i]*1.5;
      Serial.println(Intensity[(i/Displacement)-2]);
      //Serial.println(Intensity[i]);
  }
  
  
}
