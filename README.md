QEffects-Pro
============

Improved version of QEffectsGL, an OpenGL proxy mod which adds Screen Space Ambient Occlusion, Depth of Field, HSL color modifications, and more. Mine adds Motion Blur, an improved HSL algorithm and more.

This is a modified version of QEffects that looks better and runs better. Also, I added motion blur. In additon, the contrast issue with color grading is fixed.
A full list of features:
- Motion Blur
- Color Grading (note: this goes based on a 0-100% scale, instead of a 0-255 scale, as per Photoshop)
- Screen Space Ambient Occlusion
- Bloom
- Depth of Field
- Emboss (enabled only through config)
- Multitexture
- Anisotrophic Filtering

PLEASE NOTE: THIS DOES NOT WORK WITH WINDOWED MODE OR WITH DYNAMIC GLOW ON (and dynamic glow is redundant to have on anyway!)

To install, place in your Gamedata folder. You can modify settings per .exe in the QEffectsGL.ini.
If you're using GLDirect as a means to fix ATI drivers, this will not work. Likewise, some nVidia users have reported framerate drops.

Tested on an ATI Radeon HD 6900 on old-ish drivers. Solid 120 FPS with all settings on.
If this does NOT work, do NOT report it as "not working" on JKHub. Please report your video card and your current set of drivers.


CREDITS:
QEffects (foundation)
Scooper (motion blur shader)

Please contact me at my email address if you want to rehost this, or if you have feedback:
eezstreet(at)live(dot)com
