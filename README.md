# Project 1 Part 2: Online/ Live Recognition

## Description
This project uses the wxWidgets library in C++ to generate a canvas and allows
the user to draw on the canvas.

### General Features

#### Part 1
When using the canvas, a left click on the mouse will allow the user to begin drawing.
This can be seen on line 84 of the code in the leftClick function of the DrawPanel class.

The user does not need to hold the left button to draw. In the mouseMoved function of the DrawPanel class on line 92,
if the left button has been clicked, the user can draw on the canvas by moving the mouse.

A right click on the mouse will turn off the drawing ability as seen on line 104 in the rightClick function
of the DrawPanel class. 

To reset the canvas, the user can click on the reset button, which is located on the upper left side of the frame in
th menu bar. This menu bar is generated in the constructor of the myFrame class on line 53. When the reset button is
clicked, the code will call the OnReset function of the myFrame class on line 66, which refreshes the canvas.

#### Part 2

[Lines 93,249-282] Points are stored in a vector of pairs. To store the points being drawn by the user, the DrawPanel class has a variable vector<pair<int,int>> points that takes in each point drawn in the mouseMoved() function.

[Lines 27-66, 160-227] The templates are created in the DrawPanel constructor. This program has a nested class called Stroke in the DrawPanel class that holds the name and vector of points for each gesture. The DrawPanel class has a vector of Strokes called templates to hold each gesture, which later allows the algorithm to compare user input to each template.

[Line 27-61] The Stroke class implements the four steps of the recognition algorithm, passing the points through the four functions in its constructor class. 

##### 1) Resampling 
[Lines 314-339, 301-305] For resampling, the Resample() function had to return a vector with 64 points. In order to return the exact number of points, we used a double instead of an integer. In addition, we also added an edge case to ensure we had 64 points in the vector.
###### Related Functions: distance()

##### 2) Rotate
[Lines 341-363, 467-471] For rotation, the RotateBy() function rotates the point around the middle point, which is the centroid. In order to find the middle point of the gesture, we created a centroid() function to store the middle point of the gesture.
###### Related Functions: centroid(), IndicativeAngle()

##### 3) Scaling + Translation
[Lines 341-350, 366-398] For scaling, the scaleTo() function scales the gesture to fit in the bounding box. The TranslateTo() function moves the center of the gesture to the origin(0,0) so it can be compared. The bounding box keeps the drawing in the box of a certain size(250.0). The boundingBox() function returns the width(res.first) and height(res.second). The points and size must be double. 
###### Related Functions: boundingBox(), centroid()

##### 4) Matching process
[Lines 404-506] For the matching process, the Recognize() function matches the gesture to one of the 16 templates and returns the template that matches with the score and time. Inside of the DrawPanel class there is stuct called Result that holds the name, score, and time. The Recognize() returns a Result object with the template that best matches, the score, and the time. 
###### Related Functions: DistanceAtBestAngle(),DistanceAtAngle(), PathDistance(), distance()

[Line 285 - 299] When the user is finished drawing and right clicks, the rightClick() function will call the Recognize() function and get the Result. Then, a messagebox is generated with the result of the comparision, displaying the matching template, the score, and the time.

### Installation
The wxWidgets library is needed to run the source code. 

### Running on VSCode
Click the Terminal tab -> Run Build Task -> Debug

Open a new terminal and enter the \HCIRA\build\debug directory

Run .\main.exe

### Resources

##### Part 1
https://www.wxwidgets.org/downloads/
https://docs.wxwidgets.org/latest/overview_helloworld.html
https://wiki.wxwidgets.org/Drawing_on_a_panel_with_a_DC
https://docs.wxwidgets.org/3.0/classwx_mouse_event.html#aaf32bfc85d51a565b1a9b89189292325
https://docs.wxwidgets.org/stable/classwx_menu.html
https://www.informit.com/articles/article.aspx?p=405047
https://siytek.com/macos-gui-cpp/

##### Part 2
https://depts.washington.edu/acelab/proj/dollar/index.html
https://www.cuemath.com/geometry/distance-between-two-points/
https://wiki.wxwidgets.org/WxMessageBox
https://docs.wxwidgets.org/3.0/classwx_message_dialog.html

### Authors
Hyoyoung Jin

Sydney McLaughlin

Cameron Vallin


