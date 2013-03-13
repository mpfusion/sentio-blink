Sentio Blink Project
====================

This repository contains a simple application that lets the red LED on the
Sentio platform blink at a frequency of 1Hz. This program can serve as a base
for own developments.


Install
-------

### Clone the project

    git clone --recursive git://github.com/mpfusion/sentio-blink.git

Since the project contains the system kernel and drivers as a submodule, the
parameter `--recursive` is necessary.


### Install the tools

For further installation instructions see the [README file of the Sentio
Framework](https://github.com/mpfusion/sentio-framework/blob/master/README.md).


### Run the code

A simple `make` will compile all necessary code. A `make flash` will launch
the flash loader (which might need root access, depending on the
configuration) and write the blob to the platform.


### Documentation

A `make doc` will create the documentation for this project and the Sentio
system. Doxygen needs to be installed. By default a HTML version is generated.
To view the HTML documentation, point the browser to the file
`doc/html/index.html`. For a PDF version LaTeX needs to be installed. To
generate the PDF file, go to the directory `doc/latex` and run `make`.


### Code style

Tabs are used for indentation and spaces for alignment. To maintain a uniform
code appearance the program `astyle` is used with the following options.

	astyle -T4pbcUDH -z2
