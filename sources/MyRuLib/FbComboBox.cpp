/////////////////////////////////////////////////////////////////////////////
// Source:      src/generic/odcombo.cpp
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include <wx/wx.h>
#include <wx/combo.h>
#include "FbComboBox.h"
#include "FbTreeModel.h"

// ============================================================================
// implementation
// ============================================================================

//-----------------------------------------------------------------------------
//  FbSearchCombo
//-----------------------------------------------------------------------------

static int GetMultiplier()
{
#ifdef __WXWINCE__
    // speed up bitmap generation by using a small bitmap
    return 3;
#else
    int depth = ::wxDisplayDepth();

    if  ( depth >= 24 )
    {
        return 8;
    }
    return 6;
#endif
}

wxBitmap FbSearchCombo::RenderButtonBitmap()
{
	wxSize sizeText = GetBestSize();
	int y  = sizeText.y - 4;
	int x = sizeText.y - 4;

    //===============================================================================
    // begin drawing code
    //===============================================================================
    // image stats

    // force width:height ratio
    if ( 14*x > y*20 )
    {
        // x is too big
        x = y*20/14;
    }
    else
    {
        // y is too big
        y = x*14/20;
    }

    // glass 11x11, top left corner
    // handle (9,9)-(13,13)
    // drop (13,16)-(19,6)-(16,9)

    int multiplier = GetMultiplier();
    int penWidth = multiplier * 1;

    penWidth = penWidth * x / 20;

    wxBitmap bitmap( multiplier*x, multiplier*y );
	wxMemoryDC mem;

	wxColour bg = GetBackgroundColour();
    wxColour fg = mem.GetTextForeground();

	bg = wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE);

	mem.SelectObject(bitmap);

    // clear background
    mem.SetBrush( wxBrush(bg) );
    mem.SetPen( wxPen(bg) );
    mem.DrawRectangle(0,0,bitmap.GetWidth(),bitmap.GetHeight());

    // draw drop glass
    mem.SetBrush( wxBrush(fg) );
    mem.SetPen( wxPen(fg) );
    int glassBase = 5 * x / 20;
    int glassFactor = 2*glassBase + 1;
    int radius = multiplier*glassFactor/2;
    mem.DrawCircle(radius,radius,radius);
    mem.SetBrush( wxBrush(bg) );
    mem.SetPen( wxPen(bg) );
    mem.DrawCircle(radius,radius,radius-penWidth);

	penWidth = penWidth * 2;

    // draw handle
    int lineStart = radius + (radius-penWidth/2) * 707 / 1000; // 707 / 1000 = 0.707 = 1/sqrt(2);

    mem.SetPen( wxPen(fg) );
    mem.SetBrush( wxBrush(fg) );
    int handleCornerShift = penWidth * 707 / 1000 / 2; // 707 / 1000 = 0.707 = 1/sqrt(2);
	handleCornerShift = handleCornerShift > 0 ? handleCornerShift : 1;
    int handleBase = 4 * x / 20;
    int handleLength = 2*handleBase+1;
    wxPoint handlePolygon[] =
    {
        wxPoint(-handleCornerShift,+handleCornerShift),
        wxPoint(+handleCornerShift,-handleCornerShift),
        wxPoint(multiplier*handleLength/2+handleCornerShift,multiplier*handleLength/2-handleCornerShift),
        wxPoint(multiplier*handleLength/2-handleCornerShift,multiplier*handleLength/2+handleCornerShift),
    };
    mem.DrawPolygon(WXSIZEOF(handlePolygon),handlePolygon,lineStart,lineStart);

    //===============================================================================
    // end drawing code
    //===============================================================================

    if ( multiplier != 1 )
    {
        wxImage image = bitmap.ConvertToImage();
        image.Rescale(x,y);
        bitmap = wxBitmap( image );
    }

    mem.SelectObject(wxNullBitmap);

    // Finalize transparency with a mask
    wxMask *mask = new wxMask(bitmap, bg);
    bitmap.SetMask(mask);

	bitmap = bitmap.GetSubBitmap(wxRect(0,0, y,y));

    return bitmap;
}

//-----------------------------------------------------------------------------
//  FbComboPopup
//-----------------------------------------------------------------------------

// time in milliseconds before partial completion buffer drops
#define fbODCB_PARTIAL_COMPLETION_TIME 1000

BEGIN_EVENT_TABLE(FbComboPopup, wxVListBox)
    EVT_MOTION(FbComboPopup::OnMouseMove)
    EVT_KEY_DOWN(FbComboPopup::OnKey)
    EVT_LEFT_UP(FbComboPopup::OnLeftClick)
END_EVENT_TABLE()


void FbComboPopup::Init()
{
    m_itemHeight = 0;
    m_value = -1;
    m_itemHover = -1;
    m_clientDataItemsType = wxClientData_None;
    m_partialCompletionString = wxEmptyString;
	m_model = NULL;
}

bool FbComboPopup::Create(wxWindow* parent)
{
    if ( !wxVListBox::Create(parent,
                             wxID_ANY,
                             wxDefaultPosition,
                             wxDefaultSize,
                             wxBORDER_SIMPLE | wxLB_INT_HEIGHT | wxWANTS_CHARS) )
        return false;

    m_useFont = m_combo->GetFont();

    wxVListBox::SetItemCount(GetCount());

    // TODO: Move this to SetFont
    m_itemHeight = GetCharHeight() + 0;

    return true;
}

FbComboPopup::~FbComboPopup()
{
	wxDELETE(m_model);
}

bool FbComboPopup::LazyCreate()
{
    // NB: There is a bug with wxVListBox that can be avoided by creating
    //     it later (bug causes empty space to be shown if initial selection
    //     is at the end of a list longer than the control can show at once).
    return true;
}

// paint the control itself
void FbComboPopup::PaintComboControl( wxDC& dc, const wxRect& rect )
{
    if ( !(m_combo->GetWindowStyle() & wxODCB_STD_CONTROL_PAINT) )
    {
        int flags = wxODCB_PAINTING_CONTROL;

        if ( m_combo->ShouldDrawFocus() )
            flags |= wxODCB_PAINTING_SELECTED;

        OnDrawBg(dc, rect, m_value, flags);

        if ( m_value >= 0 )
        {
            OnDrawItem(dc,rect,m_value,flags);
            return;
        }
    }

    wxComboPopup::PaintComboControl(dc,rect);
}

void FbComboPopup::OnDrawItem(wxDC& dc, const wxRect& rect, size_t n) const
{
    // TODO: Maybe this code could be moved to wxVListBox::OnPaint?
    dc.SetFont(m_useFont);

    int flags = 0;

    // Set correct text colour for selected items
    if ( wxVListBox::GetSelection() == (int) n )
    {
        dc.SetTextForeground( wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHTTEXT) );
        flags |= wxODCB_PAINTING_SELECTED;
    }
    else
    {
        dc.SetTextForeground( wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT) );
    }

    OnDrawItem(dc,rect,(int)n,flags);
}

void FbComboPopup::OnDrawBg( wxDC& dc, const wxRect& rect, int item, int flags ) const
{
    FbComboBox* combo = (FbComboBox*) m_combo;

    wxASSERT_MSG( combo->IsKindOf(CLASSINFO(FbComboBox)),
                  wxT("you must subclass FbComboPopup for drawing and measuring methods") );

    if ( IsCurrent((size_t)item) && !(flags & wxODCB_PAINTING_CONTROL) )
        flags |= wxODCB_PAINTING_SELECTED;

    combo->OnDrawBackground(dc,rect,item,flags);
}

void FbComboPopup::OnDrawBackground(wxDC& dc, const wxRect& rect, size_t n) const
{
    OnDrawBg(dc,rect,(int)n,0);
}

// This is called from FbComboPopup::OnDrawItem, with text colour and font prepared
void FbComboPopup::OnDrawItem( wxDC& dc, const wxRect& rect, int item, int flags ) const
{
    FbComboBox* combo = (FbComboBox*) m_combo;

    wxASSERT_MSG( combo->IsKindOf(CLASSINFO(FbComboBox)),
                  wxT("you must subclass FbComboPopup for drawing and measuring methods") );

    combo->OnDrawItem(dc,rect,item,flags);
}

void FbComboPopup::DismissWithEvent()
{
    StopPartialCompletion();

    int selection = wxVListBox::GetSelection();

    Dismiss();

	wxString valStr = GetString(selection);

    m_value = selection;

    if ( valStr != m_combo->GetValue() )
        m_combo->SetValueWithEvent(valStr);

    SendComboBoxEvent(selection);
}

void FbComboPopup::SendComboBoxEvent( int selection )
{
    wxCommandEvent evt(wxEVT_COMMAND_COMBOBOX_SELECTED, m_combo->GetId());

    evt.SetEventObject(m_combo);

    evt.SetInt(selection);

    m_combo->GetEventHandler()->AddPendingEvent(evt);
}

// returns true if key was consumed
bool FbComboPopup::HandleKey( int keycode, bool saturate, wxChar unicode )
{
    const int itemCount = GetCount();

    // keys do nothing in the empty control and returning immediately avoids
    // using invalid indices below
    if ( !itemCount )
        return false;

    int value = m_value;
    int comboStyle = m_combo->GetWindowStyle();

    // this is the character equivalent of the code
    wxChar keychar = 0;
    if ( keycode < WXK_START )
    {
#if wxUSE_UNICODE
        if ( unicode > 0 )
        {
            if ( wxIsprint(unicode) )
                keychar = unicode;
        }
        else
#else
        wxUnusedVar(unicode);
#endif
        if ( wxIsprint(keycode) )
        {
            keychar = (wxChar) keycode;
        }
    }

    if ( keycode == WXK_DOWN || keycode == WXK_RIGHT )
    {
        value++;
        StopPartialCompletion();
    }
    else if ( keycode == WXK_UP || keycode == WXK_LEFT )
    {
        value--;
        StopPartialCompletion();
    }
    else if ( keycode == WXK_PAGEDOWN )
    {
        value+=10;
        StopPartialCompletion();
    }
    else if ( keycode == WXK_PAGEUP )
    {
        value-=10;
        StopPartialCompletion();
    }
    else if ( keychar && (comboStyle & wxCB_READONLY) )
    {
        // Try partial completion

        // find the new partial completion string
#if wxUSE_TIMER
        if (m_partialCompletionTimer.IsRunning())
            m_partialCompletionString+=wxString(keychar);
        else
#endif // wxUSE_TIMER
            m_partialCompletionString=wxString(keychar);

        // now search through the values to see if this is found
        int found = -1;
        unsigned int length=m_partialCompletionString.length();
        int i;
        for (i=0; i<itemCount; i++)
        {
            wxString item=GetString(i);
            if (( item.length() >= length) && (!  m_partialCompletionString.CmpNoCase(item.Left(length))))
            {
                found=i;
                break;
            }
        }

        if (found<0)
        {
            StopPartialCompletion();
            ::wxBell();
            return true; // to stop the first value being set
        }
        else
        {
            value=i;
#if wxUSE_TIMER
            m_partialCompletionTimer.Start(fbODCB_PARTIAL_COMPLETION_TIME, true);
#endif // wxUSE_TIMER
        }
    }
    else
        return false;

    if ( saturate )
    {
        if ( value >= itemCount )
            value = itemCount - 1;
        else if ( value < 0 )
            value = 0;
    }
    else
    {
        if ( value >= itemCount )
            value -= itemCount;
        else if ( value < 0 )
            value += itemCount;
    }

    if ( value == m_value )
        // Even if value was same, don't skip the event
        // (good for consistency)
        return true;

    m_value = value;

    if ( value >= 0 )
		m_combo->SetValue(GetString(value));

    SendComboBoxEvent(m_value);

    return true;
}

// stop partial completion
void FbComboPopup::StopPartialCompletion()
{
    m_partialCompletionString = wxEmptyString;
#if wxUSE_TIMER
    m_partialCompletionTimer.Stop();
#endif // wxUSE_TIMER
}

void FbComboPopup::OnComboDoubleClick()
{
    // Cycle on dclick (disable saturation to allow true cycling).
    if ( !::wxGetKeyState(WXK_SHIFT) )
        HandleKey(WXK_DOWN,false);
    else
        HandleKey(WXK_UP,false);
}

void FbComboPopup::OnComboKeyEvent( wxKeyEvent& event )
{
    // Saturated key movement on
    if ( !HandleKey(event.GetKeyCode(),true,
#if wxUSE_UNICODE
        event.GetUnicodeKey()
#else
        0
#endif
        ) )
        event.Skip();
}

void FbComboPopup::OnPopup()
{
    // *must* set value after size is set (this is because of a vlbox bug)
    wxVListBox::SetSelection(m_value);
}

void FbComboPopup::OnMouseMove(wxMouseEvent& event)
{
    event.Skip();

    // Move selection to cursor if it is inside the popup

    int y = event.GetPosition().y;
    int fromBottom = GetClientSize().y - y;

    // Since in any case we need to find out if the last item is only
    // partially visible, we might just as well replicate the HitTest
    // loop here.
    const size_t lineMax = GetVisibleEnd();
    for ( size_t line = GetVisibleBegin(); line < lineMax; line++ )
    {
        y -= OnGetLineHeight(line);
        if ( y < 0 )
        {
            // Only change selection if item is fully visible
            if ( (y + fromBottom) >= 0 )
            {
                wxVListBox::SetSelection((int)line);
                return;
            }
        }
    }
}

void FbComboPopup::OnLeftClick(wxMouseEvent& WXUNUSED(event))
{
    DismissWithEvent();
}

void FbComboPopup::OnKey(wxKeyEvent& event)
{
    // Hide popup if certain key or key combination was pressed
    if ( m_combo->IsKeyPopupToggle(event) )
    {
        StopPartialCompletion();
        Dismiss();
    }
    else if ( event.AltDown() )
    {
        // On both wxGTK and wxMSW, pressing Alt down seems to
        // completely freeze things in popup (ie. arrow keys and
        // enter won't work).
        return;
    }
    // Select item if ENTER is pressed
    else if ( event.GetKeyCode() == WXK_RETURN || event.GetKeyCode() == WXK_NUMPAD_ENTER )
    {
        DismissWithEvent();
    }
    else
    {
        int comboStyle = m_combo->GetWindowStyle();
        int keycode = event.GetKeyCode();
        // Process partial completion key codes here, but not the arrow keys as the base class will do that for us
        if ((comboStyle & wxCB_READONLY) &&
            (keycode >= WXK_SPACE) && (keycode <=255) && (keycode != WXK_DELETE) && wxIsprint(keycode))
        {
            OnComboKeyEvent(event);
            SetSelection(m_value); // ensure the highlight bar moves
        }
        else
            event.Skip();
    }
}

wxString FbComboPopup::GetString( int item ) const
{
	return m_model ? m_model->GetData(item + 1) : wxString();
}

wxString FbComboPopup::GetStringValue() const
{
    return wxEmptyString;
}

void FbComboPopup::SetSelection( int item )
{
    wxCHECK_RET( item == wxNOT_FOUND || ((unsigned int)item < GetCount()),
                 wxT("invalid index in FbComboPopup::SetSelection") );

    m_value = item;

    if ( IsCreated() )
        wxVListBox::SetSelection(item);
}

int FbComboPopup::GetSelection() const
{
    return m_value;
}

wxSize FbComboPopup::GetAdjustedSize( int minWidth, int prefHeight, int maxHeight )
{
    int height = 250;

    maxHeight -= 2;  // Must take borders into account

    if ( GetCount() )
    {
        if ( prefHeight > 0 )
            height = prefHeight;

        if ( height > maxHeight )
            height = maxHeight;

        int totalHeight = GetTotalHeight(); // + 3;

#if defined(__WXMAC__)
        // Take borders into account, or there will be scrollbars even for one or two items.
        totalHeight += 2;
#endif
        if ( height >= totalHeight )
        {
            height = totalHeight;
        }
        else
        {
            // Adjust height to a multiple of the height of the first item
            // NB: Calculations that take variable height into account
            //     are unnecessary.
            int fih = GetLineHeight(0);
            height -= height % fih;
        }
    }
    else
        height = 50;

    // Take scrollbar into account in width calculations
    return wxSize(minWidth, height + 2);
}

void FbComboPopup::AssignModel(FbListModel * model)
{
	wxDELETE(m_model);
	m_model = model;
	wxVListBox::SetItemCount(GetCount());
}

unsigned int FbComboPopup::GetCount() const
{
	return m_model ? m_model->GetRowCount() : 0;
}

// ----------------------------------------------------------------------------
// FbComboBox
// ----------------------------------------------------------------------------


BEGIN_EVENT_TABLE(FbComboBox, wxComboCtrl)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC_CLASS(FbComboBox, wxComboCtrl)

void FbComboBox::Init()
{
	m_initModel = NULL;
}

FbComboBox::~FbComboBox()
{
	wxDELETE(m_initModel);
}

void FbComboBox::DoSetPopupControl(wxComboPopup* popup)
{
    if ( popup == NULL) popup = new FbComboPopup();

    wxComboCtrl::DoSetPopupControl(popup);

    wxASSERT(popup);

    // Add initial choices to the wxVListBox
    if ( m_initModel )
    {
        GetVListBoxComboPopup()->AssignModel(m_initModel);
        m_initModel = NULL;
    }
}

// ----------------------------------------------------------------------------
// FbComboBox item drawing and measuring default implementations
// ----------------------------------------------------------------------------

void FbComboBox::OnDrawItem( wxDC& dc,
                                       const wxRect& rect,
                                       int item,
                                       int flags ) const
{
    if ( flags & wxODCB_PAINTING_CONTROL )
    {
        dc.DrawText( GetValue(),
                     rect.x + GetTextIndent(),
                     (rect.height-dc.GetCharHeight())/2 + rect.y );
    }
    else
    {
        dc.DrawText( GetVListBoxComboPopup()->GetString(item), rect.x + 2, rect.y );
    }
}

void FbComboBox::OnDrawBackground(wxDC& dc,
                                            const wxRect& rect,
                                            int WXUNUSED(item),
                                            int flags) const
{
    // We need only to explicitly draw background for items
    // that should have selected background. Also, call PrepareBackground
    // always when painting the control so that clipping is done properly.

    if ( (flags & wxODCB_PAINTING_SELECTED) ||
         ((flags & wxODCB_PAINTING_CONTROL) && HasFlag(wxCB_READONLY)) )
    {
        int bgFlags = wxCONTROL_SELECTED;

        if ( !(flags & wxODCB_PAINTING_CONTROL) )
            bgFlags |= wxCONTROL_ISSUBMENU;

        PrepareBackground(dc, rect, bgFlags);
    }
}

void FbComboBox::AssignModel(FbListModel * model)
{
	FbComboPopup * popup = GetVListBoxComboPopup();
	if (popup) {
		popup->AssignModel(model);
	} else {
		wxDELETE(m_initModel);
		m_initModel = model;
	}
}
