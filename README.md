tomate
======

# Introduction #
Tomate is a garden planning and reporting tool for a kitchen garden, developped in C++ with qt4.

# Features #
- Log crops with date, location, specie, variety and notes
- Show timeline of actions for each plot
- Show space view of the garden at any date
- Keep notes for each plant
- Planning

# Installation #
    git clone http://github.com/MickaelG/tomate_cpp
    cd tomate_cpp
    cmake -G 'Unix Makefiles'
    make tomate_gui
    ./tomate_gui

# Quick start #
- Add plots in the "Plots" tab. Each plot is subdivided in subplots, and crops will take place in the subplots. You can create only one subplot in a plot if needed.
- Add plants in the "Plants" tab. You can set a different color for each plant with the "color" button
- Add crops by right-clicking in the "Time view" tab.
- You can see the state of the garden at any date in the "Space view" tab. By default, this date is today. You can select an other date by clicking in the "months line" in the time view. The red line shows the date selected for space view.

# License #
Tomate is licensed under GPL.
