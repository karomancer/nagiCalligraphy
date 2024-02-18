# Naginata Calligraphy
Using OpenFrameworks to detect naginata (Japanese glaives) movement and visualize them as Japanese calligraphy-styled brushstrokes.

This project uses a Kinect V2's infrared camera to detect the reflective tape we wrapped over the sendanmaki (the point at which the blade is attached to the pole) of the naginata and paints it in as black without refreshing the frame.

I tried many methods ranging from 
- Using OpenCV to detect blobs so that multiple naginatas can be detected for competitions. The problem is the movement is too quick and includes too many orientations for OpenCV to track accurately.
- Detecting only infrared and painting it 1:1, but this causes the shape to be unlike a Japanese brush
- Detecing the infrared and painting it in circles, but it ends up looking like a splatter with quick movements
- Detecting the infrared and storing the center and lerping between the two with a brush shaped polygon

Never got it quite how I wanted it, but it was a fun exploratory process that I learned a lot in! Hoping to get back to it and back into the dojo and see this through.

Here's a compilation of my struggles thus far:

https://github.com/karomancer/nagiCalligraphy/assets/482817/e18d94c3-756a-431d-8179-744634145117


