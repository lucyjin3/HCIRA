# Project 1 Part 3: Offline/Test Recognition with $1

## Description
This project implements the $1 algorithm from the $-family of gesture recognition algorithms. This will read in a gesture dataset from files, connect to existing $1 recognition methods, go over the gesture dataset to test the recognizer. This will output the result of the recognition tests to a log file. 

### General Features
#### Part a) Read in Dataset
[Lines 274-500] Extracting(), ExtractXML(). These two methods read the XML files and parse them into our vectors. This reads the raw points, the gestures, and the subject. 
#### Part b) Connect to Recognizer
[Lines 498-499 ] This line stores all gestures and preprocesses them into our Stroke class. This allows us to access the preprocessed points later in our data structure. 
#### Part c) Loop over Dataset 
[Line 510-592] This loop is a representation of the pseudocode for the user-dependent random-100 loop. For each user, we looped for each example 10 times. For each gesture, we chose E(example) templates from user, gesture set. Then we chose 1 candidate to test against the templates from user, gesture set. 
#### Part d) Output the Result
[Line 502-509, 562-592, 594-670] CreateCSV(), OutputCSV(), TrainingSetVector(), NBestSortedList(). These methods take in the information that the recognized funciton outputs and outputs them into a csv file. We calculated an output of the average accuracy per user and total accuracy for all users. 


### Installation
The wxWidgets library is needed to run the source code. 
VCPKG Library, RapidXML is needed to run the source code. 

### Running on Visual Studio
Click the Debug tab -> Start without debug

### Resources
https://rapidxml.sourceforge.net/manual.html
https://www.geeksforgeeks.org/csv-file-management-using-c/
https://linuxhint.com/parse_xml_in_cpp/
https://spin.atomicobject.com/2012/05/20/rapidxml-a-lightweight-xml-library-for-c/
https://www.digitalocean.com/community/tutorials/random-number-generator-c-plus-plus


### Authors
Hyoyoung Jin

Sydney McLaughlin

Cameron Vallin


