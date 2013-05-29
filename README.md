# Autohome Arduino

This repository contains all libraries and sketches used with the
[Autohome Project](http://autohomeproject.org), a Rails-based home automation system.

This repository is strictly for [Arduino](http://arduino.cc)-based libraries.
When we write libraries for other boards, other repositories will be created.

All sketches are written with the [Arduino Uno](http://arduino.cc/en/Main/arduinoBoardUno)
in mind to keep costs and memory usage down. They should work on other boards
and 3rd party variations, but these have not been tested.

# File Structure

* `libraries/` contains any shared libraries that need to be added to
your Arduino build environment prior to compiling any sketches.
* `sketches/` contains a set of Arduino sketches. Each one serves a
specific purpose (data collection, security door access, I/O panel, etc). Obviously,
only one should (and can) be installed per Arduino.

# Installation/Usage

For now, there is no automated build process for these files. Installation and
compilation is a manual process.

1. Download and unzip the Arduino installation package from http://www.arduino.cc
2. Merge the `libraries/` folders together into the Arduino installation
package. Most operating systems let you copy one folder over top of the other.
3. Open the sketch you want to use in the Arduino editor.
4. Change any `#define` blocks that are required for that sketch.
5. Compile and program to your Arduino!

# License

All Autohome components are dual-licenced. For most people, Autohome and all of
its components are licenced under the _GNU GPL v3_. For use in commercial
products, another license applies. Please contact us for more information.

    Autohome Arduino Components
    Copyright (C) 2013 Brian Turchyn, Tyler Seredynski, Justin Blagden

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
