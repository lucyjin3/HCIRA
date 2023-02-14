# Project 1 Part 1: Drawing on a Canvas

## Desciption
This project uses the wxWidgets library in C++ to generate a canvas and allows
the user to draw on the canvas.

#Features
When using the canvas, a left click on the mouse will allow the user to begin drawing.
This can be seen on line 84 of the code in the leftClick function of the DrawPanel class.

The user does not need to hold the left button to draw. In the mouseMoved function of the DrawPanel class on line 92,
if the left button has been clicked, the user can draw on the canvas by moving the mouse.

A right click on the mouse will turn off the drawing ability as seen on line 104 in the rightClick function
of the DrawPanel class. 

To reset the canvas, the user can click on the reset button, which is located on the upper left side of the frame in
th menu bar. This menu bar is generated in the constructor of the myFrame class on line 53. When the reset button is
clicked, the code will call the OnReset function of the myFrame class on line 66, which refreshes the canvas.

### Installation
The wxWidgets library is needed to run the source code. 

### Running on VSCode
Click the Terminal tab -> Run Build Task -> Debug

Open a new terminal and enter the \HCIRA\build\debug directory

Run .\main.exe

### Resources
https://www.wxwidgets.org/downloads/
https://docs.wxwidgets.org/latest/overview_helloworld.html
https://wiki.wxwidgets.org/Drawing_on_a_panel_with_a_DC
https://docs.wxwidgets.org/3.0/classwx_mouse_event.html#aaf32bfc85d51a565b1a9b89189292325
https://docs.wxwidgets.org/stable/classwx_menu.html
https://www.informit.com/articles/article.aspx?p=405047
https://siytek.com/macos-gui-cpp/

### Authors
Sydney McLaughlin

Cameron Vallin

Hyoyoung Jin
