# Voice Repeater on PSoC Microcontroller

This project is a demonstration of a voice repeater system implemented using the PSoC6 microcontroller with a CapSense button.

## Overview

The system captures an audio input, processes the signal, and plays it back. The user interface involves CapSense buttons, providing a touch-sensitive experience.

### Workflow:

#### Input:
- Capture analog signal from a microphone.
- Digitize the received signal.
- Process the audio signal.
- Store in RAM.

#### Output:
- Amplify the audio.
- Output to a speaker.

## Technical Details

### Signal Acquisition and Digitization:
Based on Nyquist-Shannon sampling theorem, we utilize a sample rate of 8 kHz because the frequency of the human voice mostly doesn't exceed 4 kHz.

### ADC:
The Analog-to-Digital Converter (ADC) transforms the input analog signal into a discrete code (digital signal) that quantitatively characterizes the amplitude of the input signal.

### Audio Processing:
- Calculate the average value of digitized data.
- Calculate amplitude (difference between value and average).
- Smoothen the amplitude.
- Scale the amplitude.

### DAC:
The Digital-to-Analog Converter (DAC) is an electronic device that converts a digital signal back to an analog signal.

### Tools Used:
- **PSoC6**: The main microcontroller.
- **Analog Microphone**: For audio input.
- **Sound Amplifier**: To amplify the audio signal.
- **Capacitor**: For the electronic circuit.
- **Analog Speaker**: For audio output.

### Code Overview:
The code starts by setting up the necessary hardware components. Once initialized, it constantly checks for user input via the CapSense buttons. On one button press, it records the voice and processes it. On the other button press, it plays back the recorded voice.

## Project Team:
This project was made as a collaborative effort by a dedicated team.

## Repository Link:
[Github Repository](https://github.com/yurii-zinchuk/VoiceRepeater6.git)

