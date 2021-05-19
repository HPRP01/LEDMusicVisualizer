# LED Music Visualizer
Arduino based LED strip music visualizer

Currently in the process of creating an Arduino/ESP32 based music visualizer. This device will take audio in through an aux cable from a phone or other device and output to a 15 x 20 LED matrix created outof WS2812b LED strips.

#### Goals
- Controllable brightness/colors/patterns
- Automatically adapt to different to ensure similar output 
- Ability to select from preset patterns

The code is loosely based on examples created by [s-marley](https://github.com/s-marley/ESP32_FFT_VU) and [mrme88](https://github.com/mrme88/Arduino-Audio-Visualizer).

### Update 5/18
After getting the matrix to function correctly I began noticing a few problems. One of the main problems I noticed was that while the visualizer was picking up the low end frequencies, the high end were not displayed as well or as often. Another problem was that in order for the visualizer to function properly, I had to have the music turned up to full volume. This was something that I wanted to address as I do not want to have the speakers outputting at full volume all the time. 

After some research, I came to realize that one of the reasons this might be happening is because the Arduino Mega that I was using did not have enough computing power. After looking around at similar projects I decided that I would purchase a ESP32 microcontroller as it had a higher clock speed and overall more computing power. This would mean that there should be fewer problems when processing the audio input. 

I attempted to upload the code that I had written for the Arduino and it does not appear to be working so I am now in the process of debugging and rewriting the current code. 

-hp
