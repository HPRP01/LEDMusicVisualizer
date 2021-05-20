# LED Music Visualizer
Arduino based LED strip music visualizer

Currently in the process of creating an Arduino/ESP32 based music visualizer. This device will take audio in through an aux cable from a phone or other device and output to a 15 x 20 LED matrix created outof WS2812b LED strips.

#### Goals
- Controllable brightness/colors/patterns
- Automatically adapt to different volumes/input to ensure similar output 
- Ability to select from preset patterns

The code is based on examples created by [s-marley](https://github.com/s-marley/ESP32_FFT_VU) and [mrme88](https://github.com/mrme88/Arduino-Audio-Visualizer).

### Update 5/19
After a couple hours of troubleshooting and testing it appears that the visualizer is now working. I ended up having some code that was specific to the arduino and have since removed it. Other things I had to modify include adding a sampling delay as the clock speed on the ESP32 is much higher. 

There is a new problem that I have noticed now and that is that there seems to be quite a bit of noise coming into the visualizer. This appears to have been due to a few things, one of those being the length of the data wire going into the WS2812 strip. The wire was too long and was picking up noise and I found that shortening it reduced this noise significantly. I also modified the circuit from the aux in by changing resistor values and that seems to have helped the problem some as well. 

The next steps are going to be to add some sort of noise filter in the code as suggester by s-marley then I will begin to implement some of the other features such as color and brightness control.

-hp

### Update 5/18
After getting the matrix to function correctly I began noticing a few problems. One of the main problems I noticed was that while the visualizer was picking up the low end frequencies, the high end were not displayed as well or as often. Another problem was that in order for the visualizer to function properly, I had to have the music turned up to full volume. This was something that I wanted to address as I do not want to have the speakers outputting at full volume all the time. 

After some research, I came to realize that one of the reasons this might be happening is because the Arduino Mega that I was using did not have enough computing power. After looking around at similar projects I decided that I would purchase a ESP32 microcontroller as it had a higher clock speed and overall more computing power. This would mean that there should be fewer problems when processing the audio input. 

I attempted to upload the code that I had written for the Arduino and it does not appear to be working so I am now in the process of debugging and rewriting the current code. 

-hp
