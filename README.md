# DracoQuest
A tribute to the absolute legend, Akira Toriyama. A work-in-progress, which I started in March 2024.


This was developed on the Waveshare Esp32S3 Touch LCD 4.3B, though the configs file abstracted LovyanGFX in such a way that you can select a generic "Esp32S3 Dev Module" as the board in your IDE, 
so I suspect that you'll be able to make a few simple modifications to support your hardware of choice. I find it important to note that I am in no way a senior dev, and this is probably memory-intensive, and may not run on anything less-powerful than the esp32s3 n16r8 modules, especially considering the intended scope.

IDE: Arduino v2.3.6
Esp32 core v3.0.7
LovyanGFX v1.2.7

Everything *should* work with up to date libraries, please let me know if you have issues or questions.

I drew the tiles on my phone using an app called 'Pixilart', and I created the map files with the map editor 'Tiled'.
I have every intent of filling this entire world with content, and will gradually be updating this as it progresses.

The world map is a 6x6 grid of "Zones"
Each of these zones is composed of a similar but smaller grid, 5x5 Areas.
Each of those Areas is then composed of a 25x25 tile "area map", which is what is visible to the player in worldspace within the viewport.
