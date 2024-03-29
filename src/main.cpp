//Authors:
//Hyoyoung Jin
//Sydney McLaughlin
//Cameron Vallin


#include "wx/wx.h"
#include "wx/sizer.h"
#include <wx/textctrl.h>
#include <wx/msgdlg.h>
#include <cmath>
#include "rapidxml_ext.h"
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <unordered_map>
#include <numeric>
#include <windows.h>
//#include "rapidxml/rapidxml_print.hpp"

using namespace std;
using namespace rapidxml;

bool sortDesc(const pair<string, double>& a, const pair<string, double>& b) { return a.second > b.second; };
//Done - To Do: (2a) Create a nested class to store the name of the drawing and the array of coordinate points (use pairs)???
//Done - To Do: (2c) Resampling, Rotation, Scaling + Translation
//Every template/drawing needs to go through these functions to ^
//panel class (canvas) to draw in
//(3) Turn in two logfiles (one for the demo and the other for all of the code)
//(3) E*16 = training set(templates) without replacement (So for E=2, we would randomly choose 2 to be the templates)
//(3) 1*16 = testing set (candidates)
//(3) N-best list is the lable and score for everything test(recogonize) in the training set
//To Do: Update the recognize file with multiple helper functions to store the scores for every template
class DrawPanel : public wxPanel
{

public:
    //Class to return the result of the recognize funtction
    struct Result {
        string Name;
        vector<pair<string, double>> gesScores;
        double Score;
        int Time;
        Result(string name, double score, int time, vector<pair<string,double>> gesScores) :
            Name(name), Score(score), Time(time),gesScores(gesScores) {}
    };
    // one stroke per gesture type
    //(2b)
    class Stroke {

    public:
        //constructor
        Stroke(string name, vector<pair<double, double>>& points) {
            this->name = name;
            this->points = points;
            this->points = Resample(this->points, numPoints);
            double radians = IndicativeAngle(this->points);
            this->points = RotateBy(this->points, -radians);
            this->points = ScaleTo(this->points, sqSize);
            this->points = TranslateTo(this->points, o);
        }
        //pass vectors by reference
        double distance(pair<double, double> a, pair<double, double> b);
        double pathLength(const vector<pair<double, double>>& points);
        vector<pair<double, double>> Resample(vector<pair<double, double>>& points, int n);
        pair<double, double> centroid(const vector<pair<double, double>>& points);
        vector<pair<double, double>> RotateBy(const vector<pair<double, double>>& points, double radians);
        pair<double, double> boundingBox(const vector<pair<double, double>>& points);
        vector<pair<double, double>> ScaleTo(const vector<pair<double, double>>& points, double size);
        vector<pair<double, double>> TranslateTo(const vector<pair<double, double>>& points, const pair<double, double>& pt);
        double IndicativeAngle(const vector<pair<double, double>>& points);
        // double DistanceAtAngle(const vector<pair<double,double>> &points, const Stroke &t, double radians);
        // double DistanceAtBestAngle(const vector<pair<double,double>> &points, const Stroke &t, double a, double b, double threshold);
        string name;
        vector<pair<double, double>> points;
        const double sqSize = 250.0;
        const pair<double, double> o;
        const int numPoints = 64;
        // const double Diagonal = sqrt(sqSize * sqSize + sqSize * sqSize);
        // const double HalfDiagonal = 0.5 * Diagonal;
        // const double AngleRange = (45.0 * M_PI/180);
        // const double AnglePrecision = (2.0 * M_PI/180);
        const double Phi = 0.5 * (-1.0 + sqrt(5.0));
        
    };
    //Part 3 functions
    void extractXML();
    void extracting(const string subject, const string gesture);
    void offlineLoop();
    vector<Stroke> st;
    vector<vector<Stroke>> subGes;
    vector<Stroke> candidate;
    void createCSV();
    void outPutCSV(int user, string gestureType, int i, int examples, vector<Stroke>& templates, Stroke candidate, Result res, int correct);
    string TrainingSetVector(string subject, vector<Stroke>& templates);
    string NBestSortedList(string subject, Result res);

    //Part 4 functions
    void writeXML(string gesture, int num);
    void changePrompt();
    void submitButton(wxCommandEvent& event);
    void OnReset(wxCommandEvent& event);
    vector<string> allGestures{ "arrow", "caret", "check","circle", "delete_mark","left_curly_brace","left_sq_bracket","pigtail","zig-zag","rectangle","right_curly_brace","right_sq_bracket","star","triangle","v","x" };
    int currentGes = 0;
    int samNum = 1;
    wxTextCtrl* gesture;
    wxTextCtrl* sampleNum;

    void ShowResults();
    const int numTemplates = 16;
    DrawPanel(wxFrame* parent);
    void InputTemplates();
    vector<Stroke> templates;
    void mouseMoved(wxMouseEvent& event);
    void leftClick(wxMouseEvent& event);
    void rightClick(wxMouseEvent& event);
    Result Recognize(vector<pair<double, double>>& points, vector<Stroke> Unistrokes);

    double DistanceAtAngle(const vector<pair<double, double>>& points, const Stroke& t, double radians);
    double DistanceAtBestAngle(const vector<pair<double, double>>& points, const Stroke& t, double a, double b, double threshold);
    vector<pair<double, double>> RotateBy(const vector<pair<double, double>>& points, double radians);
    pair<double, double> centroid(const vector<pair<double, double>>& points);
    double PathDistance(vector<pair<double, double>>& pts1, vector<pair<double, double>> pts2);
    const double sqSize = 250.0;
    const double Diagonal = sqrt(sqSize * sqSize + sqSize * sqSize);
    const double HalfDiagonal = 0.5 * Diagonal;
    const double AngleRange = (45.0 * (M_PI / 180));
    const double AnglePrecision = (2.0 * (M_PI / 180));
    const double Phi = 0.5 * (-1.0 + sqrt(5));
    double distance(pair<double, double> a, pair<double, double> b);
    //double distance(pair<double,double> a, pair<double,double> b);
    // double pathLength(const vector<pair<double,double>>& points);
    void TestingDraw(vector<pair<double, double>>& points);
    // vector<pair<double,double>> Resample(const vector<pair<double,double>>& points, int n);
    //void erase(wxMouseEvent& event);
    bool drawing = false;
    int oldXPos;
    int oldYPos;
    //vector stored with points(part a)
    vector<pair<double, double>> points;
    DECLARE_EVENT_TABLE()
};

//vector<vector<Stroke>>
//The first vector is our 11 subjects
// The second is the 160 gestures with points

//Frame which holds the panel
class myFrame : public wxFrame
{
public:
    myFrame();
    DrawPanel* drawPane;
    //void OnReset(wxCommandEvent& event);
};
enum
{
    ID_Reset = 1
};
class MyApp : public wxApp
{
    bool OnInit();

};

IMPLEMENT_APP(MyApp)

//main function
//Runs during initilization
bool MyApp::OnInit()
{
    AllocConsole();
    FILE* fDummy;
    freopen_s(&fDummy, "CONIN$", "r", stdin);
    freopen_s(&fDummy, "CONOUT$", "w", stderr);
    freopen_s(&fDummy, "CONOUT$", "w", stdout);

    myFrame* frame = new myFrame();
    frame->Show();
    return true;
}

//Constructor to create window and generate the menubar
myFrame::myFrame() : wxFrame((wxFrame*)NULL, -1, wxT("Project 1 Canvas"), wxPoint(50, 50), wxSize(800, 600))
{

    /*wxMenu* menuReset = new wxMenu;
    menuReset->Append(ID_Reset, "&Reset");

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuReset, "&Reset");
    SetMenuBar(menuBar);*/

    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    drawPane = new DrawPanel(this);
    sizer->Add(drawPane, 1, wxEXPAND);

    SetSizer(sizer);
    SetAutoLayout(true);

    //Bind(wxEVT_MENU, &myFrame::OnReset, this, ID_Reset);
}
//When reset is clicked, the canvas will be cleared
void DrawPanel::OnReset(wxCommandEvent& event)
{
    cout << "Reset\n";
    //points.clear();
    drawing = false;
    Refresh();
}
BEGIN_EVENT_TABLE(DrawPanel, wxPanel)

EVT_LEFT_DOWN(DrawPanel::leftClick)
EVT_MOTION(DrawPanel::mouseMoved)
EVT_RIGHT_DOWN(DrawPanel::rightClick)
END_EVENT_TABLE()

//constructor for the DrawPanel class (the canvas)
//stores the 16 templates (b)
DrawPanel::DrawPanel(wxFrame* parent) :wxPanel(parent)
{
    wxButton* submitButton = new wxButton(this, wxID_ANY, wxT("Submit"), wxPoint(700, 500));
    wxButton* resetButton = new wxButton(this, wxID_ANY, wxT("Reset"), wxPoint(10, 500));
    submitButton->Bind(wxEVT_BUTTON, &DrawPanel::submitButton, this, submitButton->GetId());
    resetButton->Bind(wxEVT_BUTTON, &DrawPanel::OnReset, this, resetButton->GetId());
    /*string ges = "Gesture: " + allGestures[currentGes];
    string sam = "Sample #: " + to_string(samNum);
    wxTextCtrl* gesture = new wxTextCtrl(this, wxID_ANY, ges, wxPoint(0, 0), wxSize(800, 30),
        wxTE_READONLY);
    wxTextCtrl* sampleNum = new wxTextCtrl(this, wxID_ANY, sam, wxPoint(0, 30), wxSize(800, 30),
        wxTE_READONLY);*/
    //InputTemplates();
    
    // Part 3
    //extractXML();
    //createCSV();
    //offlineLoop();
    //cout << "Done\n";
    changePrompt();
}
void DrawPanel::submitButton(wxCommandEvent& event)
{
    writeXML(allGestures[currentGes], samNum);
    samNum++;
    if (samNum > 10)
    {
        currentGes++;
        samNum = 1;
    }
    //writeXML(allGestures[currentGes], samNum);
    Refresh();
    changePrompt();
}
void DrawPanel::changePrompt()
{
    //vector<string> allGestures{ "arrow", "caret", "check","circle", "delete_mark","left_curly_brace","left_sq_bracket","pigtail","question_mark","rectangle","right_curly_brace","right_sq_bracket","star","triangle","v","x"};
    if (currentGes == allGestures.size())
    {
        wxTextCtrl* gesture = new wxTextCtrl(this, wxID_ANY, "Gesture: Complete! Thank you!", wxPoint(0, 0), wxSize(800, 30),
            wxTE_READONLY);
        wxTextCtrl* sampleNum = new wxTextCtrl(this, wxID_ANY, "Sample #: ", wxPoint(0, 30), wxSize(800, 30),
            wxTE_READONLY);
        return;
    }
    string ges = "Gesture: " + allGestures[currentGes];
    string sam = "Sample #: " + to_string(samNum);
    wxTextCtrl* gesture = new wxTextCtrl(this, wxID_ANY, ges, wxPoint(0, 0), wxSize(800, 30),
        wxTE_READONLY);
    wxTextCtrl* sampleNum = new wxTextCtrl(this, wxID_ANY, sam, wxPoint(0, 30), wxSize(800, 30),
        wxTE_READONLY);
    //string output = ges + "arrow";
    //gesture->ChangeValue(ges);
    //sampleNum->ChangeValue(sam);

  /*  string gesOutput = "";
    string samOutput = "";
    for (int i = 0; i < allGestures.size(); i++)
    {
        for (int j = 1; j <= 10; j++)
        {
            gesOutput = ges + " " + allGestures[i];
            samOutput = sam + " " + to_string(j);
            gesture->ChangeValue(gesOutput);
            sampleNum->ChangeValue(samOutput);

        }
    }*/
    
    
}
void DrawPanel::InputTemplates()
{
    //this->templates = new vector<pair<double,double>> arr(numTemplates);
    //Stroke triangle = new Stroke();
    vector<pair<double, double>> shape1{ make_pair(137,139),make_pair(135,141),make_pair(133,144),make_pair(132,146),make_pair(130,149),make_pair(128,151),make_pair(126,155),make_pair(123,160),make_pair(120,166),make_pair(116,171),make_pair(112,177),make_pair(107,183),make_pair(102,188),make_pair(100,191),make_pair(95,195),make_pair(90,199),make_pair(86,203),make_pair(82,206),make_pair(80,209),make_pair(75,213),make_pair(73,213),make_pair(70,216),make_pair(67,219),make_pair(64,221),make_pair(61,223),make_pair(60,225),make_pair(62,226),make_pair(65,225),make_pair(67,226),make_pair(74,226),make_pair(77,227),make_pair(85,229),make_pair(91,230),make_pair(99,231),make_pair(108,232),make_pair(116,233),make_pair(125,233),make_pair(134,234),make_pair(145,233),make_pair(153,232),make_pair(160,233),make_pair(170,234),make_pair(177,235),make_pair(179,236),make_pair(186,237),make_pair(193,238),make_pair(198,239),make_pair(200,237),make_pair(202,239),make_pair(204,238),make_pair(206,234),make_pair(205,230),make_pair(202,222),make_pair(197,216),make_pair(192,207),make_pair(186,198),make_pair(179,189),make_pair(174,183),make_pair(170,178),make_pair(164,171),make_pair(161,168),make_pair(154,160),make_pair(148,155),make_pair(143,150),make_pair(138,148),make_pair(136,148) };
    Stroke triangle = Stroke("triangle", shape1);
    this->templates.push_back(triangle);

    vector<pair<double, double>> shape2{ make_pair(87,142),make_pair(89,145),make_pair(91,148),make_pair(93,151),make_pair(96,155),make_pair(98,157),make_pair(100,160),make_pair(102,162),make_pair(106,167),make_pair(108,169),make_pair(110,171),make_pair(115,177),make_pair(119,183),make_pair(123,189),make_pair(127,193),make_pair(129,196),make_pair(133,200),make_pair(137,206),make_pair(140,209),make_pair(143,212),make_pair(146,215),make_pair(151,220),make_pair(153,222),make_pair(155,223),make_pair(157,225),make_pair(158,223),make_pair(157,218),make_pair(155,211),make_pair(154,208),make_pair(152,200),make_pair(150,189),make_pair(148,179),make_pair(147,170),make_pair(147,158),make_pair(147,148),make_pair(147,141),make_pair(147,136),make_pair(144,135),make_pair(142,137),make_pair(140,139),make_pair(135,145),make_pair(131,152),make_pair(124,163),make_pair(116,177),make_pair(108,191),make_pair(100,206),make_pair(94,217),make_pair(91,222),make_pair(89,225),make_pair(87,226),make_pair(87,224) };
    Stroke x = Stroke("x", shape2);
    this->templates.push_back(x);

    vector<pair<double, double>> shape3{ make_pair(78,149),make_pair(78,153),make_pair(78,157),make_pair(78,160),make_pair(79,162),make_pair(79,164),make_pair(79,167),make_pair(79,169),make_pair(79,173),make_pair(79,178),make_pair(79,183),make_pair(80,189),make_pair(80,193),make_pair(80,198),make_pair(80,202),make_pair(81,208),make_pair(81,210),make_pair(81,216),make_pair(82,222),make_pair(82,224),make_pair(82,227),make_pair(83,229),make_pair(83,231),make_pair(85,230),make_pair(88,232),make_pair(90,233),make_pair(92,232),make_pair(94,233),make_pair(99,232),make_pair(102,233),make_pair(106,233),make_pair(109,234),make_pair(117,235),make_pair(123,236),make_pair(126,236),make_pair(135,237),make_pair(142,238),make_pair(145,238),make_pair(152,238),make_pair(154,239),make_pair(165,238),make_pair(174,237),make_pair(179,236),make_pair(186,235),make_pair(191,235),make_pair(195,233),make_pair(197,233),make_pair(200,233),make_pair(201,235),make_pair(201,233),make_pair(199,231),make_pair(198,226),make_pair(198,220),make_pair(196,207),make_pair(195,195),make_pair(195,181),make_pair(195,173),make_pair(195,163),make_pair(194,155),make_pair(192,145),make_pair(192,143),make_pair(192,138),make_pair(191,135),make_pair(191,133),make_pair(191,130),make_pair(190,128),make_pair(188,129),make_pair(186,129),make_pair(181,132),make_pair(173,131),make_pair(162,131),make_pair(151,132),make_pair(149,132),make_pair(138,132),make_pair(136,132),make_pair(122,131),make_pair(120,131),make_pair(109,130),make_pair(107,130),make_pair(90,132),make_pair(81,133),make_pair(76,133) };
    Stroke rectangle = Stroke("rectangle", shape3);
    this->templates.push_back(rectangle);

    vector<pair<double, double>> shape4{ make_pair(127,141),make_pair(124,140),make_pair(120,139),make_pair(118,139),make_pair(116,139),make_pair(111,140),make_pair(109,141),make_pair(104,144),make_pair(100,147),make_pair(96,152),make_pair(93,157),make_pair(90,163),make_pair(87,169),make_pair(85,175),make_pair(83,181),make_pair(82,190),make_pair(82,195),make_pair(83,200),make_pair(84,205),make_pair(88,213),make_pair(91,216),make_pair(96,219),make_pair(103,222),make_pair(108,224),make_pair(111,224),make_pair(120,224),make_pair(133,223),make_pair(142,222),make_pair(152,218),make_pair(160,214),make_pair(167,210),make_pair(173,204),make_pair(178,198),make_pair(179,196),make_pair(182,188),make_pair(182,177),make_pair(178,167),make_pair(170,150),make_pair(163,138),make_pair(152,130),make_pair(143,129),make_pair(140,131),make_pair(129,136),make_pair(126,139) };
    Stroke circle = Stroke("circle", shape4);
    this->templates.push_back(circle);

    vector<pair<double, double>> shape5{ make_pair(91,185),make_pair(93,185),make_pair(95,185),make_pair(97,185),make_pair(100,188),make_pair(102,189),make_pair(104,190),make_pair(106,193),make_pair(108,195),make_pair(110,198),make_pair(112,201),make_pair(114,204),make_pair(115,207),make_pair(117,210),make_pair(118,212),make_pair(120,214),make_pair(121,217),make_pair(122,219),make_pair(123,222),make_pair(124,224),make_pair(126,226),make_pair(127,229),make_pair(129,231),make_pair(130,233),make_pair(129,231),make_pair(129,228),make_pair(129,226),make_pair(129,224),make_pair(129,221),make_pair(129,218),make_pair(129,212),make_pair(129,208),make_pair(130,198),make_pair(132,189),make_pair(134,182),make_pair(137,173),make_pair(143,164),make_pair(147,157),make_pair(151,151),make_pair(155,144),make_pair(161,137),make_pair(165,131),make_pair(171,122),make_pair(174,118),make_pair(176,114),make_pair(177,112),make_pair(177,114),make_pair(175,116),make_pair(173,118) };
    Stroke check = Stroke("check", shape5);
    this->templates.push_back(check);

    vector<pair<double, double>> shape6{ make_pair(79,245),make_pair(79,242),make_pair(79,239),make_pair(80,237),make_pair(80,234),make_pair(81,232),make_pair(82,230),make_pair(84,224),make_pair(86,220),make_pair(86,218),make_pair(87,216),make_pair(88,213),make_pair(90,207),make_pair(91,202),make_pair(92,200),make_pair(93,194),make_pair(94,192),make_pair(96,189),make_pair(97,186),make_pair(100,179),make_pair(102,173),make_pair(105,165),make_pair(107,160),make_pair(109,158),make_pair(112,151),make_pair(115,144),make_pair(117,139),make_pair(119,136),make_pair(119,134),make_pair(120,132),make_pair(121,129),make_pair(122,127),make_pair(124,125),make_pair(126,124),make_pair(129,125),make_pair(131,127),make_pair(132,130),make_pair(136,139),make_pair(141,154),make_pair(145,166),make_pair(151,182),make_pair(156,193),make_pair(157,196),make_pair(161,209),make_pair(162,211),make_pair(167,223),make_pair(169,229),make_pair(170,231),make_pair(173,237),make_pair(176,242),make_pair(177,244),make_pair(179,250),make_pair(181,255),make_pair(182,257) };
    Stroke caret = Stroke("caret", shape6);
    this->templates.push_back(caret);

    vector<pair<double, double>> shape7{ make_pair(307,216),make_pair(333,186),make_pair(356,215),make_pair(375,186),make_pair(399,216),make_pair(418,186) };
    Stroke zigzag = Stroke("zigzag", shape7);
    this->templates.push_back(zigzag);

    vector<pair<double, double>> shape8{ make_pair(68,222),make_pair(70,220),make_pair(73,218),make_pair(75,217),make_pair(77,215),make_pair(80,213),make_pair(82,212),make_pair(84,210),make_pair(87,209),make_pair(89,208),make_pair(92,206),make_pair(95,204),make_pair(101,201),make_pair(106,198),make_pair(112,194),make_pair(118,191),make_pair(124,187),make_pair(127,186),make_pair(132,183),make_pair(138,181),make_pair(141,180),make_pair(146,178),make_pair(154,173),make_pair(159,171),make_pair(161,170),make_pair(166,167),make_pair(168,167),make_pair(171,166),make_pair(174,164),make_pair(177,162),make_pair(180,160),make_pair(182,158),make_pair(183,156),make_pair(181,154),make_pair(178,153),make_pair(171,153),make_pair(164,153),make_pair(160,153),make_pair(150,154),make_pair(147,155),make_pair(141,157),make_pair(137,158),make_pair(135,158),make_pair(137,158),make_pair(140,157),make_pair(143,156),make_pair(151,154),make_pair(160,152),make_pair(170,149),make_pair(179,147),make_pair(185,145),make_pair(192,144),make_pair(196,144),make_pair(198,144),make_pair(200,144),make_pair(201,147),make_pair(199,149),make_pair(194,157),make_pair(191,160),make_pair(186,167),make_pair(180,176),make_pair(177,179),make_pair(171,187),make_pair(169,189),make_pair(165,194),make_pair(164,196) };
    Stroke arrow = Stroke("arrow", shape8);
    this->templates.push_back(arrow);

    vector<pair<double, double>> shape9{ make_pair(140,124),make_pair(138,123),make_pair(135,122),make_pair(133,123),make_pair(130,123),make_pair(128,124),make_pair(125,125),make_pair(122,124),make_pair(120,124),make_pair(118,124),make_pair(116,125),make_pair(113,125),make_pair(111,125),make_pair(108,124),make_pair(106,125),make_pair(104,125),make_pair(102,124),make_pair(100,123),make_pair(98,123),make_pair(95,124),make_pair(93,123),make_pair(90,124),make_pair(88,124),make_pair(85,125),make_pair(83,126),make_pair(81,127),make_pair(81,129),make_pair(82,131),make_pair(82,134),make_pair(83,138),make_pair(84,141),make_pair(84,144),make_pair(85,148),make_pair(85,151),make_pair(86,156),make_pair(86,160),make_pair(86,164),make_pair(86,168),make_pair(87,171),make_pair(87,175),make_pair(87,179),make_pair(87,182),make_pair(87,186),make_pair(88,188),make_pair(88,195),make_pair(88,198),make_pair(88,201),make_pair(88,207),make_pair(89,211),make_pair(89,213),make_pair(89,217),make_pair(89,222),make_pair(88,225),make_pair(88,229),make_pair(88,231),make_pair(88,233),make_pair(88,235),make_pair(89,237),make_pair(89,240),make_pair(89,242),make_pair(91,241),make_pair(94,241),make_pair(96,240),make_pair(98,239),make_pair(105,240),make_pair(109,240),make_pair(113,239),make_pair(116,240),make_pair(121,239),make_pair(130,240),make_pair(136,237),make_pair(139,237),make_pair(144,238),make_pair(151,237),make_pair(157,236),make_pair(159,237) };
    Stroke leftsquarebracket = Stroke("left square bracket", shape9);
    this->templates.push_back(leftsquarebracket);

    vector<pair<double, double>> shape10{ make_pair(112,138),make_pair(112,136),make_pair(115,136),make_pair(118,137),make_pair(120,136),make_pair(123,136),make_pair(125,136),make_pair(128,136),make_pair(131,136),make_pair(134,135),make_pair(137,135),make_pair(140,134),make_pair(143,133),make_pair(145,132),make_pair(147,132),make_pair(149,132),make_pair(152,132),make_pair(153,134),make_pair(154,137),make_pair(155,141),make_pair(156,144),make_pair(157,152),make_pair(158,161),make_pair(160,170),make_pair(162,182),make_pair(164,192),make_pair(166,200),make_pair(167,209),make_pair(168,214),make_pair(168,216),make_pair(169,221),make_pair(169,223),make_pair(169,228),make_pair(169,231),make_pair(166,233),make_pair(164,234),make_pair(161,235),make_pair(155,236),make_pair(147,235),make_pair(140,233),make_pair(131,233),make_pair(124,233),make_pair(117,235),make_pair(114,238),make_pair(112,238) };
    Stroke rightsquarebracket = Stroke("right square bracket", shape10);
    this->templates.push_back(rightsquarebracket);

    vector<pair<double, double>> shape11{ make_pair(89,164),make_pair(90,162),make_pair(92,162),make_pair(94,164),make_pair(95,166),make_pair(96,169),make_pair(97,171),make_pair(99,175),make_pair(101,178),make_pair(103,182),make_pair(106,189),make_pair(108,194),make_pair(111,199),make_pair(114,204),make_pair(117,209),make_pair(119,214),make_pair(122,218),make_pair(124,222),make_pair(126,225),make_pair(128,228),make_pair(130,229),make_pair(133,233),make_pair(134,236),make_pair(136,239),make_pair(138,240),make_pair(139,242),make_pair(140,244),make_pair(142,242),make_pair(142,240),make_pair(142,237),make_pair(143,235),make_pair(143,233),make_pair(145,229),make_pair(146,226),make_pair(148,217),make_pair(149,208),make_pair(149,205),make_pair(151,196),make_pair(151,193),make_pair(153,182),make_pair(155,172),make_pair(157,165),make_pair(159,160),make_pair(162,155),make_pair(164,150),make_pair(165,148),make_pair(166,146) };
    Stroke v = Stroke("v", shape11);
    this->templates.push_back(v);

    vector<pair<double, double>> shape12{ make_pair(123,129),make_pair(123,131),make_pair(124,133),make_pair(125,136),make_pair(127,140),make_pair(129,142),make_pair(133,148),make_pair(137,154),make_pair(143,158),make_pair(145,161),make_pair(148,164),make_pair(153,170),make_pair(158,176),make_pair(160,178),make_pair(164,183),make_pair(168,188),make_pair(171,191),make_pair(175,196),make_pair(178,200),make_pair(180,202),make_pair(181,205),make_pair(184,208),make_pair(186,210),make_pair(187,213),make_pair(188,215),make_pair(186,212),make_pair(183,211),make_pair(177,208),make_pair(169,206),make_pair(162,205),make_pair(154,207),make_pair(145,209),make_pair(137,210),make_pair(129,214),make_pair(122,217),make_pair(118,218),make_pair(111,221),make_pair(109,222),make_pair(110,219),make_pair(112,217),make_pair(118,209),make_pair(120,207),make_pair(128,196),make_pair(135,187),make_pair(138,183),make_pair(148,167),make_pair(157,153),make_pair(163,145),make_pair(165,142),make_pair(172,133),make_pair(177,127),make_pair(179,127),make_pair(180,125) };
    Stroke Delete = Stroke("Delete", shape12);
    this->templates.push_back(Delete);

    vector<pair<double, double>> shape13{ make_pair(150,116),make_pair(147,117),make_pair(145,116),make_pair(142,116),make_pair(139,117),make_pair(136,117),make_pair(133,118),make_pair(129,121),make_pair(126,122),make_pair(123,123),make_pair(120,125),make_pair(118,127),make_pair(115,128),make_pair(113,129),make_pair(112,131),make_pair(113,134),make_pair(115,134),make_pair(117,135),make_pair(120,135),make_pair(123,137),make_pair(126,138),make_pair(129,140),make_pair(135,143),make_pair(137,144),make_pair(139,147),make_pair(141,149),make_pair(140,152),make_pair(139,155),make_pair(134,159),make_pair(131,161),make_pair(124,166),make_pair(121,166),make_pair(117,166),make_pair(114,167),make_pair(112,166),make_pair(114,164),make_pair(116,163),make_pair(118,163),make_pair(120,162),make_pair(122,163),make_pair(125,164),make_pair(127,165),make_pair(129,166),make_pair(130,168),make_pair(129,171),make_pair(127,175),make_pair(125,179),make_pair(123,184),make_pair(121,190),make_pair(120,194),make_pair(119,199),make_pair(120,202),make_pair(123,207),make_pair(127,211),make_pair(133,215),make_pair(142,219),make_pair(148,220),make_pair(151,221) };
    Stroke leftcurlybrace = Stroke("left curly brace", shape13);
    this->templates.push_back(leftcurlybrace);

    vector<pair<double, double>> shape14{ make_pair(117,132),make_pair(115,132),make_pair(115,129),make_pair(117,129),make_pair(119,128),make_pair(122,127),make_pair(125,127),make_pair(127,127),make_pair(130,127),make_pair(133,129),make_pair(136,129),make_pair(138,130),make_pair(140,131),make_pair(143,134),make_pair(144,136),make_pair(145,139),make_pair(145,142),make_pair(145,145),make_pair(145,147),make_pair(145,149),make_pair(144,152),make_pair(142,157),make_pair(141,160),make_pair(139,163),make_pair(137,166),make_pair(135,167),make_pair(133,169),make_pair(131,172),make_pair(128,173),make_pair(126,176),make_pair(125,178),make_pair(125,180),make_pair(125,182),make_pair(126,184),make_pair(128,187),make_pair(130,187),make_pair(132,188),make_pair(135,189),make_pair(140,189),make_pair(145,189),make_pair(150,187),make_pair(155,186),make_pair(157,185),make_pair(159,184),make_pair(156,185),make_pair(154,185),make_pair(149,185),make_pair(145,187),make_pair(141,188),make_pair(136,191),make_pair(134,191),make_pair(131,192),make_pair(129,193),make_pair(129,195),make_pair(129,197),make_pair(131,200),make_pair(133,202),make_pair(136,206),make_pair(139,211),make_pair(142,215),make_pair(145,220),make_pair(147,225),make_pair(148,231),make_pair(147,239),make_pair(144,244),make_pair(139,248),make_pair(134,250),make_pair(126,253),make_pair(119,253),make_pair(115,253) };
    Stroke rightcurlybrace = Stroke("right curly brace", shape14);
    this->templates.push_back(rightcurlybrace);

    vector<pair<double, double>> shape15{ make_pair(75,250),make_pair(75,247),make_pair(77,244),make_pair(78,242),make_pair(79,239),make_pair(80,237),make_pair(82,234),make_pair(82,232),make_pair(84,229),make_pair(85,225),make_pair(87,222),make_pair(88,219),make_pair(89,216),make_pair(91,212),make_pair(92,208),make_pair(94,204),make_pair(95,201),make_pair(96,196),make_pair(97,194),make_pair(98,191),make_pair(100,185),make_pair(102,178),make_pair(104,173),make_pair(104,171),make_pair(105,164),make_pair(106,158),make_pair(107,156),make_pair(107,152),make_pair(108,145),make_pair(109,141),make_pair(110,139),make_pair(112,133),make_pair(113,131),make_pair(116,127),make_pair(117,125),make_pair(119,122),make_pair(121,121),make_pair(123,120),make_pair(125,122),make_pair(125,125),make_pair(127,130),make_pair(128,133),make_pair(131,143),make_pair(136,153),make_pair(140,163),make_pair(144,172),make_pair(145,175),make_pair(151,189),make_pair(156,201),make_pair(161,213),make_pair(166,225),make_pair(169,233),make_pair(171,236),make_pair(174,243),make_pair(177,247),make_pair(178,249),make_pair(179,251),make_pair(180,253),make_pair(180,255),make_pair(179,257),make_pair(177,257),make_pair(174,255),make_pair(169,250),make_pair(164,247),make_pair(160,245),make_pair(149,238),make_pair(138,230),make_pair(127,221),make_pair(124,220),make_pair(112,212),make_pair(110,210),make_pair(96,201),make_pair(84,195),make_pair(74,190),make_pair(64,182),make_pair(55,175),make_pair(51,172),make_pair(49,170),make_pair(51,169),make_pair(56,169),make_pair(66,169),make_pair(78,168),make_pair(92,166),make_pair(107,164),make_pair(123,161),make_pair(140,162),make_pair(156,162),make_pair(171,160),make_pair(173,160),make_pair(186,160),make_pair(195,160),make_pair(198,161),make_pair(203,163),make_pair(208,163),make_pair(206,164),make_pair(200,167),make_pair(187,172),make_pair(174,179),make_pair(172,181),make_pair(153,192),make_pair(137,201),make_pair(123,211),make_pair(112,220),make_pair(99,229),make_pair(90,237),make_pair(80,244),make_pair(73,250),make_pair(69,254),make_pair(69,252) };
    Stroke star = Stroke("star", shape15);
    this->templates.push_back(star);

    vector<pair<double, double>> shape16{ make_pair(81,219),make_pair(84,218),make_pair(86,220),make_pair(88,220),make_pair(90,220),make_pair(92,219),make_pair(95,220),make_pair(97,219),make_pair(99,220),make_pair(102,218),make_pair(105,217),make_pair(107,216),make_pair(110,216),make_pair(113,214),make_pair(116,212),make_pair(118,210),make_pair(121,208),make_pair(124,205),make_pair(126,202),make_pair(129,199),make_pair(132,196),make_pair(136,191),make_pair(139,187),make_pair(142,182),make_pair(144,179),make_pair(146,174),make_pair(148,170),make_pair(149,168),make_pair(151,162),make_pair(152,160),make_pair(152,157),make_pair(152,155),make_pair(152,151),make_pair(152,149),make_pair(152,146),make_pair(149,142),make_pair(148,139),make_pair(145,137),make_pair(141,135),make_pair(139,135),make_pair(134,136),make_pair(130,140),make_pair(128,142),make_pair(126,145),make_pair(122,150),make_pair(119,158),make_pair(117,163),make_pair(115,170),make_pair(114,175),make_pair(117,184),make_pair(120,190),make_pair(125,199),make_pair(129,203),make_pair(133,208),make_pair(138,213),make_pair(145,215),make_pair(155,218),make_pair(164,219),make_pair(166,219),make_pair(177,219),make_pair(182,218),make_pair(192,216),make_pair(196,213),make_pair(199,212),make_pair(201,211) };
    Stroke pigtail = Stroke("pigtail", shape16);
    this->templates.push_back(pigtail);
}
//Done: To Do: Parsing the XML files and collecting the data
void DrawPanel::extractXML() {
    
    string gesture;
    string subject;
    
    //extracting xml logs;
    //subjects e.g s02 
    for ( int j = 1; j <= 11; j++ ) {
        if (j == 1) {
            subject = "s01 (pilot)";
        }
        else if (j > 9 ) {
            subject = "s" + to_string(j);
        }
        else {
            subject = "s0" + to_string(j);
        }
        cout << subject << "\n";
        //gestures e.g) arrow01
        for (int i = 1; i <= 10; i++){
            if (i == 10)
            {
                gesture = "arrow10";
            }
            else {
                gesture = "arrow0" + to_string(i);
            }
            //cout << gesture << endl;
            extracting(subject, gesture);
        }
        for (int i = 1; i <= 10; i++) {
            if (i == 10)
            {
                gesture = "caret10";
            }
            else {
                gesture = "caret0" + to_string(i);
            }
            //cout << gesture << endl;
            extracting(subject, gesture);
        }
        for (int i = 1; i <= 10; i++) {
            if (i == 10)
            {
                gesture = "check10";
            }
            else {
                gesture = "check0" + to_string(i);
            }
            //cout << gesture << endl;
            extracting(subject, gesture);
        }
        for (int i = 1; i <= 10; i++) {
            if (i == 10)
            {
                gesture = "circle10";
            }
            else {
                gesture = "circle0" + to_string(i);
            }
            //cout << gesture << endl;
            extracting(subject, gesture);
        }
        for (int i = 1; i <= 10; i++) {
            if (i == 10)
            {
                gesture = "delete_mark10";
            }
            else {
                gesture = "delete_mark0" + to_string(i);
            }
            //cout << gesture << endl;
            extracting(subject, gesture);
        }
        for (int i = 1; i <= 10; i++) {
            if (i == 10)
            {
                gesture = "left_curly_brace10";
            }
            else {
                gesture = "left_curly_brace0" + to_string(i);
            }
            //cout << gesture << endl;
            extracting(subject, gesture);
        }
        for (int i = 1; i <= 10; i++) {
            if (i == 10)
            {
                gesture = "left_sq_bracket10";
            }
            else {
                gesture = "left_sq_bracket0" + to_string(i);
            }
            //cout << gesture << endl;
            extracting(subject, gesture);
        }
        for (int i = 1; i <= 10; i++) {
            if (i == 10)
            {
                gesture = "pigtail10";
            }
            else {
                gesture = "pigtail0" + to_string(i);
            }
            //cout << gesture << endl;
            extracting(subject, gesture);
        }
        for (int i = 1; i <= 10; i++) {
            if (i == 10)
            {
                gesture = "question_mark10";
            }
            else {
                gesture = "question_mark0" + to_string(i);
            }
            //cout << gesture << endl;
            extracting(subject, gesture);
        }
        for (int i = 1; i <= 10; i++) {
            if (i == 10)
            {
                gesture = "rectangle10";
            }
            else {
                gesture = "rectangle0" + to_string(i);
            }
            //cout << gesture << endl;
            extracting(subject, gesture);
        }
        for (int i = 1; i <= 10; i++) {
            if (i == 10)
            {
                gesture = "right_curly_brace10";
            }
            else {
                gesture = "right_curly_brace0" + to_string(i);
            }
            //cout << gesture << endl;
            extracting(subject, gesture);
        }
        for (int i = 1; i <= 10; i++) {
            if (i == 10)
            {
                gesture = "right_sq_bracket10";
            }
            else {
                gesture = "right_sq_bracket0" + to_string(i);
            }
            //cout << gesture << endl;
            extracting(subject, gesture);
        }
        for (int i = 1; i <= 10; i++) {
            if (i == 10)
            {
                gesture = "star10";
            }
            else {
                gesture = "star0" + to_string(i);
            }
            //cout << gesture << endl;
            extracting(subject, gesture);
        }
        for (int i = 1; i <= 10; i++) {
            if (i == 10)
            {
                gesture = "triangle10";
            }
            else {
                gesture = "triangle0" + to_string(i);
            }
            //cout << gesture << endl;
            extracting(subject, gesture);
        }
        for (int i = 1; i <= 10; i++) {
            if (i == 10)
            {
                gesture = "v10";
            }
            else {
                gesture = "v0" + to_string(i);
            }
            //cout << gesture << endl;
            extracting(subject, gesture);
        }
        for (int i = 1; i <= 10; i++) {
            if (i == 10)
            {
                gesture = "x10";
            }
            else {
                gesture = "x0" + to_string(i);
            }
            //cout << gesture << endl;
            extracting(subject,gesture);
        }
        subGes.push_back(st);
        st.clear();
    }
}
//Takes in the subject and gesture to extract from the xml files
void DrawPanel::extracting(const string subject, const string gesture)
{
    
    xml_document<> doc;
    xml_node<> *root_node;
    //read xml file into the vector;
    string filename = "xml_logs/" + subject + "/medium/" + gesture + ".xml";
    ifstream theFile(filename);
    vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
    buffer.push_back('\0');
    //parsing the buffer using xml file
    doc.parse<0>(&buffer[0]);
    //first node
    root_node = doc.first_node("Gesture");
    //iterate over points
    vector <pair<double, double>> p;

    for(xml_node<> * point_node = root_node->first_node("Point"); point_node; point_node = point_node->next_sibling()){
        p.push_back(make_pair(atof(point_node->first_attribute("X")->value()), atof(point_node->first_attribute("Y")->value())));
        /*printf("Point X : %s  and Point Y : %s",
            point_node -> first_attribute("X")->value(),
            point_node->first_attribute("Y")->value());
        cout << endl;*/
    }
    Stroke newStroke = Stroke(gesture, p);
    st.push_back(newStroke);
}
//Creates the csv file
void DrawPanel::createCSV()
{
    string output = "User, GestureType, RandomIteration, #ofTrainingExamples, TotalSizeOfTrainingSet, Training Set Contents, Candidate, Reco Result Gesture, Correct, Reco Result Score, Reco Result Best Match, N-Best Sorted List\n";
    ofstream myCSV;
    myCSV.open("log_file.csv");
    myCSV << output << endl;
    myCSV.close();
}
void DrawPanel::offlineLoop() {
    srand(time(0));
    double avgScore = 0;
    vector<double> userAvg;
    //for each user (subject)
    for (int u = 0; u < subGes.size(); u++)
    {
        vector<double> exampleAvgs;
        cout << u << "\n";
        //for each example
        for (int e = 1; e < 10; e++)
        {
            cout << e << "\n";
            vector<double>repeat;
            //unordered_map<string, int> scoresPerGes = { {"arrow",0},{"caret",0},{"check",0},{"circle",0},{"delete_mark",0},{"left_curly_brace",0},{"left_sq_bracket",0},{"pigtail",0},{"question_mark",0},{"rectangle",0},{"right_curly_brace",0},{"right_sq_bracket",0},{"star",0},{"triangle",0},{"v",0},{"x",0} };
            //test this 10 times
            for (int i = 0; i < 10; i++)
            {
                double score = 0;
                templates.clear();
                int index = 0;
                candidate.clear();
                unordered_map<string, int> scoresPerGes = { {"arrow",0},{"caret",0},{"check",0},{"circle",0},{"delete_mark",0},{"left_curly_brace",0},{"left_sq_bracket",0},{"pigtail",0},{"question_mark",0},{"rectangle",0},{"right_curly_brace",0},{"right_sq_bracket",0},{"star",0},{"triangle",0},{"v",0},{"x",0} };

                //vector<double> scoresPerGes(16,0);
                //unordered_map<string, int> scoresPerGes = { {"arrow",0},{"caret",0},{"check",0},{"circle",0},{"delete_mark",0},{"left_curly_brace",0},{"left_sq_bracket",0},{"pigtail",0},{"question_mark",0},{"rectangle",0},{"right_curly_brace",0},{"right_sq_bracket",0},{"star",0},{"triangle",0},{"v",0},{"x",0};
                //For every gesture, collect the training sets (templates) and the test set(candidate)
                for (int g = 0; g < 16; g++)
                {
                    vector<int> prev(10, 0);
                    for (int c = 0; c < e; c++) {
                        int r = rand() % 10;
                        if (prev[r] == 1)
                        {
                            c--;
                        }
                        else
                        {
                            templates.push_back(subGes[u][r + index]);
                            prev[r] += 1;
                        }
                    }
                    int r = rand() % 10;
                    candidate.push_back(subGes[u][r + index]);

                    index += 10;
                }
                //for every candidate, find the N-best list of the candidate compared to every template
                //Result has a vector<pair<string,double> for the scores of every template compared
                for (int t = 0; t < candidate.size(); t++)
                {
                    int correct = 0;
                    Result res = Recognize(candidate[t].points, templates);
                    if (res.Name.substr(0, res.Name.length() - 2) == candidate[t].name.substr(0, candidate[t].name.length() - 2))
                    {
                        scoresPerGes[res.Name.substr(0, res.Name.length() - 2)] += 1;
                        correct = 1;
                    }
                    outPutCSV(u, candidate[t].name, i, e, templates, candidate[t], res, correct);
                }
                double sum = 0;
                for (auto x : scoresPerGes) {
                    double avg = x.second / 1.0;
                    sum += avg;
                }
                sum = sum / (16.0);
                repeat.push_back(sum);
            }
            double sumRepeat = accumulate(repeat.begin(), repeat.end(), 0.0);
            sumRepeat = sumRepeat / repeat.size();
            exampleAvgs.push_back(sumRepeat);
        }
        double sumOfExamples = accumulate(exampleAvgs.begin(), exampleAvgs.end(), 0.0);
        sumOfExamples /= exampleAvgs.size();
        userAvg.push_back(sumOfExamples);

        ofstream myCSV;
        myCSV.open("log_file.csv", fstream::app);
        myCSV << "Total Average Accuracy for user:,";
        myCSV << to_string(sumOfExamples) << endl;
        myCSV.close();
    }
    double totalAccuracyForAllUsers = accumulate(userAvg.begin(), userAvg.end(), 0.0);
    totalAccuracyForAllUsers /= userAvg.size();
    ofstream myCSV;
    myCSV.open("log_file.csv", fstream::app);
    myCSV << "Total Average Accuracy for All Users:,";
    myCSV << to_string(totalAccuracyForAllUsers) << endl;
    myCSV.close();
    // final average per-user accuracy
    //avgScore = score / 160;
}
//To Do: Write output to a csv file
void DrawPanel::outPutCSV(int user, string gestureType, int i, int examples, vector<Stroke>& templates, Stroke candidate, Result res, int correct)
{
    string subject = "";
    string trainingSet = "";
    string cand = "{";
    string result = "{";
    string nBestList = "";
    string gest = gestureType.substr(0, gestureType.length() - 2);
    int number = stoi(gestureType.substr(gestureType.length() - 2, 2));
    string num = to_string(number);
    int u = user + 1;
    if (u == 10)
    {
        subject = "s" + to_string(u);
    }
    else
    {
        subject = "s0" + to_string(u);
    }

    trainingSet = "\"" + TrainingSetVector(subject, templates) + "\"";
    cand = "\"" + cand;
    cand += subject + "-" + gest + "-" + num + "}" +"\"";
    result = "\"" + result;
    result +=  subject + "-" + res.Name.substr(0, res.Name.length() - 2) + "-" + to_string(stoi(gestureType.substr(gestureType.length() - 2, 2))) + "}" + "\"";
    nBestList = "\"" + NBestSortedList(subject, res) + "\"";

    string output = "";
    output = subject + "," + gest + "," + to_string(i + 1) + "," + to_string(examples) + "," + to_string(examples * 16) + "," + trainingSet + ",";
    output += cand + ",";
    output += res.Name.substr(0, res.Name.length() - 2) + "," + to_string(correct) + "," + to_string(res.Score) + "," + result + "," + nBestList;
    ofstream myCSV;
    myCSV.open("log_file.csv",fstream::app);
    myCSV << output << "\n";
    myCSV.close();
}
string DrawPanel::TrainingSetVector(string subject, vector<Stroke>& templates)
{
    string tempSet = "{";
    for (int i = 0; i < templates.size(); i++)
    {
        string name = templates[i].name.substr(0,templates[i].name.length() - 2);
        int number = stoi(templates[i].name.substr(templates[i].name.length() - 2, 2));
        string num = to_string(number);
        tempSet += subject + "-" + name + "-" + num;
        if (i < templates.size() - 1) {
            tempSet += ",";
        }
    }
    tempSet += "}";

    return tempSet;
}
string DrawPanel::NBestSortedList(string subject, Result res)
{
    sort(res.gesScores.begin(), res.gesScores.end(), sortDesc);
    string nlist = "{";
    int length = 50;
    if (res.gesScores.size() < 50) {
        length = res.gesScores.size();
    }

    for (int i = 0; i < length; i++)
    {
        string name = res.gesScores[i].first.substr(0, res.gesScores[i].first.length() - 2);
        int number = stoi(res.gesScores[i].first.substr(res.gesScores[i].first.length() - 2, 2));
        string num = to_string(number);
        nlist += subject + "-" + name + "-" + num + "," + to_string(res.gesScores[i].second);

        if (i < length - 1)
        {
            nlist += ",";
        }
    }
    nlist += "}";
    return nlist;
}
// DrawPanel::Stroke::Stroke(string name, vector<pair<double,double>> points){
//     this->name = name;
//     this->points = points;
// }
//Helper function to visualize points as they change (resample, rotate, sample, translate)
void DrawPanel::TestingDraw(vector<pair<double, double>>& points)
{
    wxClientDC dc(this);
    int oldX = points[0].first;
    int oldY = points[0].second;
    for (int i = 1; i < points.size(); i++)
    {
        dc.SetPen(wxPen(wxColor(0, 0, 0), 3)); // black line, 3 pixels thick
        dc.DrawLine(oldX, oldY, points[i].first, points[i].second);
        oldX = points[i].first;
        oldY = points[i].second;
    }
}
//When left button is clicked on the panel, drawing will begin when the mouse is moved
void DrawPanel::leftClick(wxMouseEvent& event)
{
    //TestingDraw(templates[0].points);
    points.clear();
    drawing = true;
    wxClientDC dc(this);
    //stores the first coordinate
    oldXPos = event.GetPosition().x;
    oldYPos = event.GetPosition().y;
    //insert points into a vector(a)
    points.push_back(make_pair(oldXPos, oldYPos));
    //Circle will be drawn where the click happened to be able to see first point
    dc.SetBrush(*wxBLACK_BRUSH);
    dc.DrawCircle(wxPoint(oldXPos, oldYPos), 5);
}

//When drawing has been enabled, the mouse can draw on the canvas
//Done: To Do: (1)Store the points into an array and create drawing class for that figure
void DrawPanel::mouseMoved(wxMouseEvent& event)
{
    //using the first coordinate when left button is clicked
    //will draw a line between each of the coordinates
    if (drawing) {
        wxClientDC dc(this);
        int x = event.GetPosition().x;
        int y = event.GetPosition().y;
        //insert points into a vector(a)
        points.push_back(make_pair(x, y));
        dc.SetPen(wxPen(wxColor(0, 0, 0), 3)); // black line, 3 pixels thick
        dc.DrawLine(oldXPos, oldYPos, x, y);
        oldXPos = x;
        oldYPos = y;
    }
}

//When the right button is clicked, drawing will turn off
void DrawPanel::rightClick(wxMouseEvent& event)
{

    drawing = false;
    //TestingDraw(templates[0].points);
    //ShowResults();
    //writeXML();
}
void DrawPanel::ShowResults() 
{
    Result res = Recognize(points, templates);
    string msgOutput = "Result: ";
    msgOutput += res.Name;
    msgOutput += " (";
    msgOutput += to_string(res.Score);
    msgOutput += ") in ";
    msgOutput += to_string(res.Time);
    msgOutput += "ms.";
    int answer = wxMessageBox(msgOutput, "Recognizer Message", wxOK);
}
//  write gesture files
void DrawPanel::writeXML(string gesture, int num) 
{
    //increment subject for each person 
    string subject = "s00";
    string ges = "";
    if (num < 10)
    {
        ges = gesture + "0" + to_string(num);
    }
    else
    {
        ges = gesture + to_string(num);
    }
    string filename = "part4_xml_logs/" + subject + "/" + ges + ".xml";
    //const char* file = filename.c_str();
    ofstream myFile(filename.c_str());
    //myFile.open(filename);
    if (!myFile.is_open())
    {
        cout << "False";
    }
    //std::string newXml = "<?xml version=\"1.0\" encoding=\"UTF - 8\"?><Gesture><Point></Point></Gesture>";

    rapidxml::xml_document<> doc;

    //std::string str = newXml;
    //std::vector<char> buffer(str.begin(), str.end());
    //buffer.push_back('\0');

    //doc.parse<0>(&buffer[0]);
    //vector <pair<double, double>> points = {{1.0, 2.0}, {3.0, 4.0}, {5.0, 6.0}};// just trying out 
    rapidxml::xml_node<>* root_node = doc.allocate_node(node_element,"Gesture");
    root_node->append_attribute(doc.allocate_attribute("Name",ges.c_str()));
    root_node->append_attribute(doc.allocate_attribute("Subject", subject.c_str()));
    //doc.append_node(root_node);

    //rapidxml::xml_node<>* node = root_node->first_node("Point");
    //xml_node<>* node = ("Point");
    //iterate/output into xml
    //points.first = x
    //points.second = y
    //vector of points user puts in
    string x = "";
    string y = "";
    
    for (int i = 0; i < points.size(); i++)
    {
       // xml_node<>* point = root_node->first_node("Point");
        x = to_string(points[i].first);
        y = to_string(points[i].second);
        const char* x_node = doc.allocate_string(x.c_str());
        const char* y_node = doc.allocate_string(y.c_str());
        char* node_name = doc.allocate_string("Point");
        xml_node<>* point_node = doc.allocate_node(node_element, "Point");
        root_node->append_node(point_node);
        xml_attribute<>* xAtt = doc.allocate_attribute("X", x_node);
        xml_attribute<>* yAtt = doc.allocate_attribute("Y", y_node);
        point_node->append_attribute(xAtt);
        point_node->append_attribute(yAtt);
        /*point_node->append_attribute(doc.allocate_attribute("X", x.c_str()));
        point_node->append_attribute(doc.allocate_attribute("Y", y.c_str()));
        root_node->append_node(point_node);*/
        //cout << x + "\n" + y << "\n";
    }
    doc.append_node(root_node);
    //std::cout << doc; // lets see it
    myFile << doc;
    myFile.close();
    doc.clear();
}
//pair.first is x, pair.second is y
double DrawPanel::Stroke::distance(pair<double, double> a, pair<double, double> b) {
    double square = sqrt(abs(((b.first - a.first) * (b.first - a.first)) + ((b.second - a.second) * (b.second - a.second))));
    return square;
}

double DrawPanel::Stroke::pathLength(const vector<pair<double, double>>& points) {
    double length = 0.;
    for (int i = 1; i < points.size(); i++)
        length += distance(points[i - 1], points[i]);
    return length;
}
//need to use double instead of int to get 64 points
//MUST RETURN VECTOR WITH 64 POINTS
vector<pair<double, double>> DrawPanel::Stroke::Resample(vector<pair<double, double>>& points, int n) {
    double I = pathLength(points) / (n - 1);
    double D = 0.0;
    vector<pair<double, double>> newpoints;
    newpoints.push_back(points[0]);
    for (int i = 1; i < points.size(); i++) {
        double d = distance(points[i - 1], points[i]);
        if ((D + d) >= I) {
            double qx = points[i - 1].first + ((I - D) / d) * (points[i].first - points[i - 1].first);
            double qy = points[i - 1].second + ((I - D) / d) * (points[i].second - points[i - 1].second);
            pair<double, double> point(qx, qy);
            newpoints.push_back(point);//append
            points.insert(points.begin() + i, point);
            //points.splice(points[i], 0,point);
            //points[i] = point;

            D = 0.0;
        }
        else D += d;
    }
    //edge case to ensure 64 points
    if (newpoints.size() == n - 1) {
        newpoints.push_back(make_pair(points[points.size() - 1].first, points[points.size() - 1].second));
    }
    return newpoints;
}
//Returns the middle point of the gesture
pair<double, double> DrawPanel::Stroke::centroid(const vector<pair<double, double>>& points) {
    double x = 0, y = 0;
    for (const auto& point : points) {
        x += point.first;
        y += point.second;
    }
    x /= points.size();
    y /= points.size();
    return make_pair(x, y);
}
//Rotates the points around the middle point (the centroid)
vector<pair<double, double>> DrawPanel::Stroke::RotateBy(const vector<pair<double, double>>& points, double radians) {
    pair<double, double> c(centroid(points));
    double cosine = cos(radians);
    double sine = sin(radians);
    vector<pair<double, double>> newpoints;
    for (const auto& point : points) {
        double qx = (point.first - c.first) * cosine - (point.second - c.second) * sine + c.first;
        double qy = (point.first - c.first) * sine + (point.second - c.second) * cosine + c.second;
        newpoints.push_back(make_pair(qx, qy));
    }
    return newpoints;
}
//Keeps the drawing in a box of certain size (250.0)
//Returns width(res.first) and height(res.second)
pair<double, double> DrawPanel::Stroke::boundingBox(const vector<pair<double, double>>& points) {
    double minX = DBL_MAX, maxX = DBL_MIN, minY = DBL_MAX, maxY = DBL_MIN;
    for (const auto& point : points) {
        minX = min(minX, point.first);
        maxX = max(maxX, point.first);
        minY = min(minY, point.second);
        maxY = max(maxY, point.second);
    }
    pair<double, double> res(maxX - minX, maxY - minY);
    return res;
}
//scales the gesture to fit in the bounding box
//points and size must be double, not int
vector<pair<double, double>> DrawPanel::Stroke::ScaleTo(const vector<pair<double, double>>& points, double size) {
    auto B = boundingBox(points);
    vector<pair<double, double>> newPoints;
    for (const auto& point : points) {
        double qx = point.first * (size / B.first);
        double qy = point.second * (size / B.second);
        newPoints.push_back(make_pair(qx, qy));
    }
    return newPoints;
}

//Moves the center of the gesture to the origin (0,0) so it can be compared
vector<pair<double, double>> DrawPanel::Stroke::TranslateTo(const vector<pair<double, double>>& points, const pair<double, double>& pt) {
    auto c = centroid(points);
    vector<pair<double, double>> newPoints;
    for (const auto& point : points) {
        double qx = point.first + pt.first - c.first;
        double qy = point.second + pt.second - c.second;
        newPoints.push_back(make_pair(qx, qy));
    }
    return newPoints;
}

//Done - To Do: (2d) Match drawing to a template
//Compares the gesture drawn to the 16 templates, and returns the template that matches with the score and time
DrawPanel::Result DrawPanel::Recognize(vector<pair<double, double>>& points, vector<Stroke> temp) {
    clock_t t0 = clock();
    Stroke candidate = Stroke("", points);
    vector<pair<string, double>> gesScores;
    int index = -1;
    //double b = DBL_MAX
    double b = numeric_limits<double>::infinity();
    for (int i = 0; i < temp.size(); i++) {
        double d;
        //cout << temp[i].name << "\n";
        d = DistanceAtBestAngle(candidate.points, temp[i], -AngleRange, AngleRange, AnglePrecision);
        gesScores.push_back(make_pair(temp[i].name, 1-0 - d/HalfDiagonal));
        if (d < b) {
            b = d;
            index = i;
        }
    }
    clock_t t1 = clock();
    if (index == -1) {
        return Result("No match.", 0.0, t1 - t0, gesScores);
    }
    else {

        //cout << b << "\n" << HalfDiagonal;
        return Result(temp[index].name, 1.0 - b / HalfDiagonal, t1 - t0, gesScores);

    }
}
//const double Phi = 0.5 * (1 + sqrt(5));

double DrawPanel::DistanceAtBestAngle(const vector<pair<double, double>>& points, const Stroke& t, double a, double b, double threshold)
{
    double x1 = Phi * a + (1.0 - Phi) * b;
    double f1 = DistanceAtAngle(points, t, x1);
    double x2 = (1.0 - Phi) * a + Phi * b;
    double f2 = DistanceAtAngle(points, t, x2);
    while (abs(b - a) > threshold)
    {
        //cout << "B: " << b << "\n";
        //cout << "A: " << a << "\n";
        if (f1 < f2) {
            b = x2;
            x2 = x1;
            f2 = f1;
            x1 = Phi * a + (1.0 - Phi) * b;
            f1 = DistanceAtAngle(points, t, x1);
        }
        else {
            a = x1;
            x1 = x2;
            f1 = f2;
            x2 = (1.0 - Phi) * a + Phi * b;
            f2 = DistanceAtAngle(points, t, x2);
        }
    }
    return min(f1, f2);
}

double DrawPanel::DistanceAtAngle(const vector<pair<double, double>>& points, const Stroke& t, double radians)
{
    vector<pair<double, double>> newpoints = RotateBy(points, radians);
    return PathDistance(newpoints, t.points);
}

double DrawPanel::Stroke::IndicativeAngle(const vector<pair<double, double>>& points)
{
    pair<double, double> c(centroid(points));
    return atan2(c.second - points[0].second, c.first - points[0].first);//arc tangent
}
//Same as rotateby function above but in a different class
vector<pair<double, double>> DrawPanel::RotateBy(const vector<pair<double, double>>& points, double radians) {
    pair<double, double> c(centroid(points));
    double cosine = cos(radians);
    double sine = sin(radians);
    vector<pair<double, double>> newpoints;
    for (const auto& point : points) {
        double qx = (point.first - c.first) * cosine - (point.second - c.second) * sine + c.first;
        double qy = (point.first - c.first) * sine + (point.second - c.second) * cosine + c.second;
        newpoints.push_back(make_pair(qx, qy));
    }
    return newpoints;
}
//same as centroid function above but in a different class
pair<double, double> DrawPanel::centroid(const vector<pair<double, double>>& points) {
    double x = 0, y = 0;
    for (const auto& point : points) {
        x += point.first;
        y += point.second;
    }
    x /= points.size();
    y /= points.size();
    return make_pair(x, y);
}
double DrawPanel::PathDistance(vector<pair<double, double>>& pts1, vector<pair<double, double>> pts2)
{
    double d = 0.0;
    for (int i = 0; i < pts1.size(); i++) // assumes pts1.length == pts2.length
        d += distance(pts1[i], pts2[i]);
    return d / pts1.size();
}
//same as distance function above but in a different class
double DrawPanel::distance(pair<double, double> a, pair<double, double> b) {
    return sqrt(((b.first - a.first) * (b.first - a.first)) + ((b.second - a.second) * (b.second - a.second)));
}
