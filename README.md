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

# Screenshots #
![Tomate time view](/screenshots/tomate_timeview.png "time view")
![Tomate space view](/screenshots/tomate_spaceview.png "space view")

# Installation #

Install dependencies for compilation: libqt4-dev, libboost-date-time-dev, libboost-filesystem and libboost-test-dev

    git clone http://github.com/MickaelG/tomate_cpp
    cd tomate_cpp
    cmake -G 'Unix Makefiles'
    make tomate_gui
    ./tomate_gui

You can also download packages for version 0.1 here:
http://software.opensuse.org/download.html?project=home%3Aclaudex&package=tomate

# Quick start #
- Add plots using "Edit plots" button in the lower part of the screen.
- Add plants in the "Edit plants" button in the lower part of the screen. You can set a different color for each plant with the "color" button
- Set your first crop data and add it using "add crop" button.
- You can see the state of the garden at any date in the "Space view" tab. By default, this date is today. You can select an other date by clicking in the "months line" in the time view. The red line shows the date selected for space view.

# Roadmap #
- Windows version
- Ability to specify location graphically instead of giving coordinates
- Ability to change dates in time view using mouse drag
- Calendar view to have an overview of what to do over time
- Port to Qt5
- And more...

# License #
Tomate is licensed under GPL.

