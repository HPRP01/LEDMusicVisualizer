#define SAMPLING_FREQUENCY 40000
#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>
#include <arduinoFFT.h>


#define LED_PIN     33
#define AUDIO_IN      35


#define NUM_LEDS    150
int BRIGHTNESS = 150;
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB

#define SAMPLES 1024
#define xres 15     //Cols
#define yres 10     //Rows

double vReal[SAMPLES];
double vImag[SAMPLES];

//int Intensity[xres] = {0 };
int Intensity[yres] = {0};
int PrevIntensity[yres]= {0};
int Displacement = 1;

CRGB leds[NUM_LEDS];
arduinoFFT FFT = arduinoFFT(vReal, vImag, SAMPLES, SAMPLING_FREQUENCY);

unsigned int sampling_period;

void setup() {
  //analogReference(EXTERNAL);
  pinMode(AUDIO_IN, INPUT);
  Serial.begin(115200);
  // put your setup code here, to run once:
  delay( 3000 );
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );

  sampling_period = round(1000000 * (1.0 / SAMPLING_FREQUENCY));
}

void loop() {
  // put your main code here, to run repeatedly:
  
  visualizer();
}

void visualizer(){
  getSamples();
  displayUpdate();
  //delay(1000);
  FastLED.show();
}

void displayUpdate(){
  //int color = 0;
  int color = map(analogRead(32), 0, 4095, 0, 255);
  int bright = map(analogRead(34), 0, 4095, 0, 240);
  //int bright = 240;
  // i is columns
  
  for(int i=0; i<yres; i++){
    // j is rows
    //color = 240;
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
       //color -= 255/xres;
    }
   
  }
}

void getSamples(){

  //Take in samples from aux input
  for(int i=0; i<SAMPLES; i++){
    
    unsigned long newTime = micros();
    vReal[i] = analogRead(AUDIO_IN);
    //erial.println(vReal[i]);
    vImag[i] = 0;
    while((micros()-newTime) < sampling_period){}
  }

  // Preform fft on recorded samples
  FFT.DCRemoval();
  FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(FFT_FORWARD);
  FFT.ComplexToMagnitude();

  
  for(int i=2; i < 96; i++){
    Serial.println(vReal[i]/1000);
    unsigned int vr = vReal[i];
    //vReal[i] = constrain(vReal[i], 0, 4096);
    //vReal[i] = map(vReal[i], 0, 4096, 0, xres);
    int band;
    
      if(i<=3           )  band = 0;
      if(i>3   && i<=4  )  band = 1; 
      if(i>4   && i<=6  )  band = 2;
      if(i>6   && i<=9 )  band = 3;
      if(i>9   && i<=13 )  band = 4;
      if(i>13  && i<=19 )  band = 5;
      if(i>19  && i<=29 )  band = 6;
      if(i>29  && i<=43)  band = 7;
      if(i>43  && i<=64)  band = 8;
      if(i>64  && i<=96)  band = 9;
      if(i>96) band = 10;

      //if(band<10) Intensity[band]--;
      

    
    if(vReal[i]/1000 > Intensity[band])



      Intensity[band]+=vReal[i]/1000;
       

    
  }

  //delay(100);
  
  
}
