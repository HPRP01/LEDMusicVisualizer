#define SAMPLING_FREQUENCY 40000
#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>
#include <arduinoFFT.h>


#define LED_PIN     33
#define AUDIO_IN      35


#define NUM_LEDS    300
int BRIGHTNESS = 150; 
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB

#define SAMPLES 1024
#define xres 15     //Cols
#define yres 20     //Rows

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
  Serial.println(color);
  int bright = map(analogRead(34), 0, 4095, 0, 240);
  //int bright = 240;
  // i is columns
  //int color = 0;
  for(int i=0; i<yres; i++){
    // j is rows
    
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
       //color += 255/xres;
    }
   //color+=255/5;
   
  }
}

void getSamples(){
  int amplitude = map(analogRead(25), 0, 4095, 500, 2000);
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

  
  for(int i=2; i < 256; i++){
    //Serial.println(vReal[i]/1000);
    unsigned int vr = vReal[i];
    
    int band;
    
    if(i<=2           )  band = 19;
    if(i>2   && i<=3  )  band = 18; 
    if(i>3   && i<=4  )  band = 17;
    if(i>4   && i<=5  )  band = 16;
    if(i>5   && i<=6  )  band = 15;
    if(i>6   && i<=8  )  band = 14;
    if(i>8   && i<=10 )  band = 13;
    if(i>10  && i<=13 )  band = 12;
    if(i>13  && i<=16 )  band = 11;
    if(i>16  && i<=21 )  band = 10;
    
    if(i>21  && i<=26 )  band = 9;
    if(i>26  && i<=33 )  band = 8; 
    if(i>33  && i<=43 )  band = 7;
    if(i>43  && i<=54 )  band = 6;
    if(i>54  && i<=69 )  band = 5;
    if(i>69  && i<=88 )  band = 4;
    if(i>88  && i<=113)  band = 3;
    if(i>113 && i<=143)  band = 2;
    if(i>143 && i<=183)  band = 1;
    if(i>183 && i<=233)  band = 0;
    

    //if(band<10) Intensity[band]--;
      
    if(vReal[i]/amplitude > Intensity[band])
    {
      Intensity[band]+= min((int)(vReal[i]/amplitude), 15);
    }

    Intensity[band] = min((int)Intensity[band], 20);

    Intensity[band] = (PrevIntensity[band] + Intensity[band])/2;
    PrevIntensity[band] = Intensity[band];

    EVERY_N_MILLISECONDS(30) {
      for(int b = 0; b<20; b++)
        if(Intensity[b] > 0) Intensity[b]--;
    }
    }
}
