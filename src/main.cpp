#include "wx/wx.h"
#include "wx/sizer.h"

//panel class (canvas) to draw in
class DrawPanel : public wxPanel
{
    
public:
    DrawPanel(wxFrame* parent);
    
    void mouseMoved(wxMouseEvent& event);
    void mouseDown(wxMouseEvent& event);
    void rightClick(wxMouseEvent& event);
    //void erase(wxMouseEvent& event);
    bool drawing = false;
    int oldXPos;
    int oldYPos;
    DECLARE_EVENT_TABLE()
};

//Frame which holds the panel
class myFrame : public wxFrame
{
public:
    myFrame();
    DrawPanel * drawPane;
    void OnReset(wxCommandEvent& event);
};
enum
{
    ID_Reset = 1
};
class MyApp: public wxApp
{
    bool OnInit();

};

IMPLEMENT_APP(MyApp)

//main function
//Runs during initilization
bool MyApp::OnInit()
{
    myFrame* frame = new myFrame();
    frame->Show();
    return true;
} 

//Constructor to create window and generate the menubar
myFrame::myFrame() : wxFrame((wxFrame *)NULL, -1,  wxT("Project 1 Canvas"), wxPoint(50,50), wxSize(800,600))
{

    wxMenu *menuReset = new wxMenu;
    menuReset->Append(ID_Reset,"&Reset");

     wxMenuBar *menuBar = new wxMenuBar;
     menuBar->Append(menuReset, "&Reset");
     SetMenuBar(menuBar);

    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    drawPane = new DrawPanel(this);
    sizer->Add(drawPane, 1, wxEXPAND);
	
    SetSizer(sizer);
    SetAutoLayout(true);

    Bind(wxEVT_MENU, &myFrame::OnReset, this, ID_Reset);
}
//When reset is clicked, the canvas will be cleared
void myFrame::OnReset(wxCommandEvent &event)
{
    Refresh();
}
BEGIN_EVENT_TABLE(DrawPanel, wxPanel)

EVT_LEFT_DOWN(DrawPanel::mouseDown)
EVT_MOTION(DrawPanel::mouseMoved)
EVT_RIGHT_DOWN(DrawPanel::rightClick)
END_EVENT_TABLE()


DrawPanel::DrawPanel(wxFrame* parent) :
wxPanel(parent)
{
}
//When left button is clicked on the panel, drawing will begin when the mouse is moved
void DrawPanel::mouseDown(wxMouseEvent& event)
{
    drawing = true;
    wxClientDC dc(this);
    //stores the first coordinate
    oldXPos = event.GetPosition().x;
    oldYPos = event.GetPosition().y;
    //Circle will be drawn where the click happened to be able to see first point
    dc.SetBrush(*wxBLACK_BRUSH);
    dc.DrawCircle(wxPoint(oldXPos,oldYPos), 5);
}

//When drawing has been enabled, the mouse can draw on the canvas
void DrawPanel::mouseMoved(wxMouseEvent& event)
{
    //using the first coordinate when left button is clicked
    //will draw a line between each of the coordinates
    if(drawing){
        wxClientDC dc(this);
        int x = event.GetPosition().x;
        int y = event.GetPosition().y; 
        dc.SetPen( wxPen( wxColor(0,0,0), 3 ) ); // black line, 3 pixels thick
        dc.DrawLine( oldXPos, oldYPos, x, y );
        oldXPos = x;
        oldYPos = y;
    }
}

//When the right button is clicked, drawing will turn off
void DrawPanel::rightClick(wxMouseEvent& event)
{
    drawing = false;
}
