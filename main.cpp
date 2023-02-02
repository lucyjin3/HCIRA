#include "wx/wx.h"
#include "wx/sizer.h"

class BasicDrawPane : public wxPanel
{
    
public:
    BasicDrawPane(wxFrame* parent);
    
    void mouseMoved(wxMouseEvent& event);
    void mouseDown(wxMouseEvent& event);
    void rightClick(wxMouseEvent& event);
    //void erase(wxMouseEvent& event);
    bool drawing = false;
    int oldXPos;
    int oldYPos;
    DECLARE_EVENT_TABLE()
};

class myFrame : public wxFrame
{
public:
    myFrame();
    BasicDrawPane * drawPane;
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


bool MyApp::OnInit()
{
    myFrame* frame = new myFrame();
    frame->Show();
    return true;
} 

myFrame::myFrame() : wxFrame((wxFrame *)NULL, -1,  wxT("Project 1 Canvas"), wxPoint(50,50), wxSize(800,600))
{

    wxMenu *menuReset = new wxMenu;
    menuReset->Append(ID_Reset,"&Reset");

     wxMenuBar *menuBar = new wxMenuBar;
     menuBar->Append(menuReset, "&Reset");
     SetMenuBar(menuBar);

    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    drawPane = new BasicDrawPane(this);
    sizer->Add(drawPane, 1, wxEXPAND);
	
    SetSizer(sizer);
    SetAutoLayout(true);

    Bind(wxEVT_MENU, &myFrame::OnReset, this, ID_Reset);
}
void myFrame::OnReset(wxCommandEvent &event)
{
    Refresh();
}
BEGIN_EVENT_TABLE(BasicDrawPane, wxPanel)

EVT_LEFT_DOWN(BasicDrawPane::mouseDown)
EVT_MOTION(BasicDrawPane::mouseMoved)
EVT_RIGHT_DOWN(BasicDrawPane::rightClick)
END_EVENT_TABLE()


BasicDrawPane::BasicDrawPane(wxFrame* parent) :
wxPanel(parent)
{
}
void BasicDrawPane::mouseDown(wxMouseEvent& event)
{
    drawing = true;
    wxClientDC dc(this);
    oldXPos = event.GetPosition().x;
    oldYPos = event.GetPosition().y;
    dc.SetBrush(*wxBLACK_BRUSH);
    dc.DrawCircle(wxPoint(oldXPos,oldYPos), 5);
    //render(dc);
}
void BasicDrawPane::mouseMoved(wxMouseEvent& event)
{
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
void BasicDrawPane::rightClick(wxMouseEvent& event)
{
    drawing = false;
}
