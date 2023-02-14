# Project 1 Part 2: Online/ Live Recognition

## Description
This project uses the wxWidgets library in C++ to generate a canvas and allows
the user to draw on the canvas.

### General Features
When using the canvas, a left click on the mouse will allow the user to begin drawing.
This can be seen on line 84 of the code in the leftClick function of the DrawPanel class.

The user does not need to hold the left button to draw. In the mouseMoved function of the DrawPanel class on line 92,
if the left button has been clicked, the user can draw on the canvas by moving the mouse.

A right click on the mouse will turn off the drawing ability as seen on line 104 in the rightClick function
of the DrawPanel class. 

To reset the canvas, the user can click on the reset button, which is located on the upper left side of the frame in
th menu bar. This menu bar is generated in the constructor of the myFrame class on line 53. When the reset button is
clicked, the code will call the OnReset function of the myFrame class on line 66, which refreshes the canvas.

### 4 components 
1) [Line314-339,504-506] For resampling, the Resample() function had to return a vector with 64 points. In order to return the exact number of points, we used a double instead of an integer. In addition, we also added an edge case to ensure we had 64 points in the vector.
# Related Functions: distance()

2) [Line341-363,467-471] For rotation, the RotateBy() function rotates the point around the middle point, which is the centroid. In order to find the middle point of the gesture, we created a centroid() function to store the middle point of the gesture.
# Related Functions: centroid(), IndicativeAngle()

3) [Line341-350,366-398] For scaling, the scaleTo() function scales the gesture to fit in the bounding box. The TranslateTo function moves the center of the gesture to the origin(0,0) so it can be compared. The bounding box keeps the drawing in the box of a certain size(250.0). The boundingBox function returns the width(res.first) and height(res.second). Thhe points and size must be double. 
# Related Functions: boundingBox()

4) [Line404-465,496-506] For the matching process, the Recognize() function matches the gesture to one of the 16 templates and returns the template that matches with the score and time. 
# Related Functions: DistanceAtBestAngle(),DistanceAtAngle(), PathDistance(), distance()

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
Hyoyoung Jin

Sydney McLaughlin

Cameron Vallin


