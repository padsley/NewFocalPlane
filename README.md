# NewFocalPlane
GEANT4 simulations of the new focal-plane detector for the K600 magnetic spectrometer at iThemba LABS.

The structure has been stolen from the GEANT4 examples to make the whole process quicker. I'm not sorry.

At present you should do:

mkdir build
cd build
cmake -G Ninja .. (if you use ninja - or otherwise leave out the "-G Ninja" bit)
ninja -jN or make -jN etc.

Then ./exgps
