/////////////////////////////////////////////////////////////////////////////
// Name:        src/generic/panelg.cpp
// Purpose:     wxPanel and the keyboard handling code
// Author:      Julian Smart, Robert Roebling, Vadim Zeitlin
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "panelg.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/object.h"
    #include "wx/font.h"
    #include "wx/colour.h"
    #include "wx/settings.h"
    #include "wx/log.h"
#endif

#include "wx/containr.h"
#include "wx/panel.h"

// ----------------------------------------------------------------------------
// wxWin macros
// ----------------------------------------------------------------------------

#if wxUSE_EXTENDED_RTTI
IMPLEMENT_DYNAMIC_CLASS_XTI(wxPanel, wxWindow,"wx/panel.h")

WX_BEGIN_PROPERTIES_TABLE(wxPanel)
// style wxTAB_TRAVERSAL
WX_END_PROPERTIES_TABLE()

WX_BEGIN_HANDLERS_TABLE(wxPanel)
WX_END_HANDLERS_TABLE()

WX_CONSTRUCTOR_4( wxPanel , wxWindow* , Parent , wxWindowID , Id , wxPoint , Position , wxSize , Size ) 

#else
IMPLEMENT_DYNAMIC_CLASS(wxPanel, wxWindow)
#endif

BEGIN_EVENT_TABLE(wxPanel, wxWindow)
    EVT_SYS_COLOUR_CHANGED(wxPanel::OnSysColourChanged)

    EVT_SIZE(wxPanel::OnSize)

    WX_EVENT_TABLE_CONTROL_CONTAINER(wxPanel)
END_EVENT_TABLE()

// ============================================================================
// implementation
// ============================================================================

WX_DELEGATE_TO_CONTROL_CONTAINER(wxPanel)

// ----------------------------------------------------------------------------
// wxPanel creation
// ----------------------------------------------------------------------------

void wxPanel::Init()
{
    m_container.SetContainerWindow(this);
}

bool wxPanel::Create(wxWindow *parent, wxWindowID id,
                     const wxPoint& pos,
                     const wxSize& size,
                     long style,
                     const wxString& name)
{
    return wxWindow::Create(parent, id, pos, size, style, name);
}

wxPanel::~wxPanel()
{
}

void wxPanel::InitDialog()
{
    wxInitDialogEvent event(GetId());
    event.SetEventObject(this);
    GetEventHandler()->ProcessEvent(event);
}

// ----------------------------------------------------------------------------
// event handlers
// ----------------------------------------------------------------------------

// Responds to colour changes, and passes event on to children.
void wxPanel::OnSysColourChanged(wxSysColourChangedEvent& event)
{
    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE));
    Refresh();

    // Propagate the event to the non-top-level children
    wxWindow::OnSysColourChanged(event);
}

void wxPanel::OnSize(wxSizeEvent& event)
{
#if wxUSE_CONSTRAINTS
    if (GetAutoLayout())
        Layout();
#if defined(__WXPM__)
    else
    {
        // Need to properly move child windows under OS/2

        PSWP                        pWinSwp = GetSwp();

        if (pWinSwp->cx == 0 && pWinSwp->cy == 0 && pWinSwp->fl == 0)
        {
            // Uninitialized

            ::WinQueryWindowPos(GetHWND(), pWinSwp);
	}
	else
        {
            SWP                     vSwp;
            int                     nYDiff;

            ::WinQueryWindowPos(GetHWND(), &vSwp);
            nYDiff = pWinSwp->cy - vSwp.cy;
            MoveChildren(nYDiff);
            pWinSwp->cx = vSwp.cx;
            pWinSwp->cy = vSwp.cy;
        }
    }
#endif
#endif // wxUSE_CONSTRAINTS

    event.Skip();
}

