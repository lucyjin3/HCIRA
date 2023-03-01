# Project 1 Part 4: Collecting Data from People

## Description
This project implements the $1 algorithm from the $-family of gesture recognition algorithms. This will write the gesture the user draws to a file. The user will be given prompts to draw 10 samples of each gesture type one at a time and write them into a file. 

### General Features
#### Part a) Write Gesture Files
[Lines 836-905] writeXML(), . These methods writes the gesture the user draws to an XML file that is created. The XML file outputs the raw points of each sample in each of the 16 gestures.  
#### Part b) Prompt for Specific Samples
[Lines 201-205, 218-221, 235-246, 248-284] onReset(), submitButton(), changePrompt(). These methods are utilized to display a prompt on the screen indicating what gesture the user should draw next. A button for reset and submit is created so that when the user is finished drawing the gesture, they can press "submit" and if the user wants to redo their gesture, they can press "reset". When the user is done with all 16 gestures, a prompt that writes "complete" pops up. 
#### Part c) Recruit 6 People 
6 people have been recruited under informed consent prior to collecting data and they have been labeled with different subjects(s01, s02, ... etc).
#### Part d) Submit Full Dataset 
The final dataset contains 160 samples per person( 16 gesture types x 10 samples) for each of the 6 people. This is a total of 960 gesture samples. 


### Installation
The wxWidgets library is needed to run the source code. 
VCPKG Library, RapidXML is needed to run the source code. 

### Running on Visual Studio
Click the Debug tab -> Start without debug

### Resources
https://rapidxml.sourceforge.net/manual.html
https://stackoverflow.com/questions/303371/whats-the-easiest-way-to-generate-xml-in-c 
https://www.codeproject.com/Articles/5588/Simple-C-class-for-XML-writing 
https://comp.soft-sys.wxwindows.narkive.com/TJoVjb53/wxtextctrl-and-wxte-readonly 

### Authors
Hyoyoung Jin

Sydney McLaughlin

Cameron Vallin