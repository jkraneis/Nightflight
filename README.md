# Nightflight
Library to controle LEDs through a Teensy with using acccelerometer and RC receiver input


* LEDHardwareAbstraction for creating virtual LED strips from different LED strips controlled over different pins on the Teensy/Arduino
* LEDEffect for implementation of various effects on LEDs
** LEDEffects can be stacked. If stacked, they always render in additive mode. If you implement new effects, make sure this is taken care of.
** KnightRider
*** ToDo: function to clear background with extra color or extra effect. Or: make a pure form effect: Color value for detecting a "set" led and just clear all others.
** Line
*** ToDo: function to clear background with extra color or extra effect. Or: make a pure form effect: Color value for detecting a "set" led and just clear all others.
** Fade
** Fire
** Brightness
** ColorReset
** Rainbow
** Effects to Do:
*** Pattern (take a bit mask to define which LEDs are on)
*** Area (2D across several LED strips)
* ParameterChange for an abstraction of Parameters to use in LEDEffects
* RenderTimer for a block free timer implementation that allows you to run different timers for different purposes and different delay times
** BPMTimer
** Repeating Timer
* Nightflight for implementing a controller class to put all together