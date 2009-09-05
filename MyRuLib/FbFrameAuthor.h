#ifndef __FBFRAMEAUTHOR_H__
#define __FBFRAMEAUTHOR_H__

#include <wx/wx.h>
#include <wx/toolbar.h>
#include <wx/aui/tabmdi.h>
#include <wx/listbox.h>
#include <wx/splitter.h>
#include <wx/html/htmlwin.h>
#include "FbAuthorList.h"
#include "BookListCtrl.h"
#include "FbManager.h"
#include "BooksPanel.h"

class FbFrameAuthor : public wxAuiMDIChildFrame
{
public:
	FbFrameAuthor();
	FbFrameAuthor(wxAuiMDIParentFrame * parent, wxWindowID id = wxID_ANY, const wxString & title = wxEmptyString);
	bool Create(wxAuiMDIParentFrame * parent, wxWindowID id = wxID_ANY, const wxString & title = wxEmptyString);
	virtual wxToolBar* CreateToolBar(long style, wxWindowID winid, const wxString& WXUNUSED(name)) { return NULL; };
    virtual wxToolBar *GetToolBar() const  { return NULL; };
private:
	void CreateControls();
	wxMenuBar * CreateMenuBar();
	wxToolBar * CreateAlphaBar(wxWindow * parent, const wxString & alphabet, const int &toolid, long style);
	void ToggleAlphabar(const int &idLetter);
	void CreateBookInfo();
	void SelectFirstAuthor();
	BookTreeItemData * GetSelectedBook();
private:
	wxSplitterWindow * m_BooksSplitter;
	FbAuthorList * m_AuthorsListBox;
    wxToolBar * m_RuAlphabar;
    wxToolBar * m_EnAlphabar;
    BooksPanel * m_BooksPanel;
private:
    void OnFindTextEnter(wxCommandEvent& event);
    void OnFindTool(wxCommandEvent& event);
	void OnToolButton(wxCommandEvent & event);
	void OnToggleToolbar(wxCommandEvent & event);
	void OnLetterClicked(wxCommandEvent& event);
	void OnAuthorsListBoxSelected(wxCommandEvent & event);
	void OnChangeViewUpdateUI(wxUpdateUIEvent & event);
    void OnExternal(wxCommandEvent& event);
    void OnSubmenu(wxCommandEvent& event);
	DECLARE_EVENT_TABLE()
};

#endif // __FBFRAMEAUTHOR_H__
