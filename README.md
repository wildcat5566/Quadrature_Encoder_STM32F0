# STM32F0 Quadrature Encoder

## Rotary encoders
![alt text](./src/rotary.jpg)  </br >
</br >
An optical encoder "channel" is composed of a pair of light (optical signal) emitter and receiver. </br >
The stationary codewheel has alternating opaque and transparent sections (stripes) on it. When optical signals are able to be received (i.e. between the emitter and receiver is a transparent section), the channel outputs a HIGH (1) digital signal. Otherwise the channel outputs a LOW (0) one. </br >
When our motor shaft starts rotating, the optical channels start to output HIGH and LOW signals alternatively, hence generates a square wave signal, shown in the image above. </br >
We detect triggers (both falling and rising edges) to determine occurance of a state transition.
</br >

## Encoder logic (1): State
![alt text](./src/logic.jpg)  </br >
</br >
A "Quadrature" encoder means that there are 2 channels (A, B) * 2 states (1, 0) = 4 possible combinations (states). </br >
Here I represent the encoder state with an XOR logic analogy, shown in the table in above image. </br >
</br >

## Encoder logic (2): Direction
![alt text](./src/direction.jpg)  </br >
</br >
What differs a quadrature encoder from a regular one (or with only one channel) is that with the extra channel, rotation direction of motor shaft can now be determined. </br >
We compare the current state with the previous one to determine the rotation direction. Details are described in the image above. </br >
</br >

## Maxon encoder observation
![alt text](./src/observation.jpg)  </br >
</br >
The motor we use is Maxon RE30. Installed encoder is with 256 lines resolution and a gear ratio of 110. </br >
Hence we should detect 256 * 4 (quadrature) * 110 = 112640 triggers per revolution. </br >
Before moving on to our microcontroller implementation, we should make an observation of our encoder output pins. </br >
According to product specs, shown in the image above, there are 2 output pins for optical channels A and B each, the "differential signaling" transmission technique is used. Details are described in next paragraph. </br >
</br >

## Differential signaling and corresponding IC SN75175
![alt text](./src/SN75175.jpg)  </br >
</br >
The pair of "complementary" differential signals have same wave profile but opposite magnitudes. </br >
By using the SN75175 IC illustrated above, we integrate the pair of complements by subtraction, hence gaining following benefits: </br >
- Amplify low voltage signals, i.e. V - (-V) = 2V </br >
- Robust against white and electromagnetic noise, since we subtract them out when combining the differential signals. </br >
</br >

## STM32F0 EXTI
EXTI stands for external interrupt and that's the library we use for the square wave edge/trigger detection. </br >
After reading the signal via a GPIO pin, we connect it to an event handler "line".
It's worth mentioning that for external interrupt handling, all pins sharing the same number share the same line, i.e. PA0, PB0, ... PG0 share line0. Therefore we need to choose separate lines for each square wave input.

## Hardware setup
![alt text](./src/hardwaresetup.jpg)  </br >
</br >
FINALLY we are ready to move on to our circuit & hardware setup. </br >
Wiring of the motor encoder and SN75175 is illustrated abov. </br >
The two outputs (original A and B channel outputs) are connected to PA0 and PA3 on the STM32F0. </br >
</br >

## STM studio monitoring
![alt text](./src/studio.JPG)  </br >
