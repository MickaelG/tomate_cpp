
lupdate src/main.cpp src/gui/gui_*.cpp src/gui/*/*.cpp -ts tomate_fr.ts
linguist tomate_fr.ts
lrelease tomate_fr.ts
mv tomate_fr.qm build/

