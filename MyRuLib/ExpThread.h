#ifndef __EXPTHREAD_H__
#define __EXPTHREAD_H__

#include <wx/wx.h>
#include <wx/thread.h>
#include <wx/arrimpl.cpp>
#include <wx/filename.h>
#include <wx/process.h>
#include "BaseThread.h"
#include "FbWindow.h"
#include "FbConst.h"
#include "FbBookEvent.h"

class ExportFileItem
{
	public:
		ExportFileItem(const wxFileName &n, const int i): filename(n), id(i) {};
	public:
		wxFileName filename;
		int id;
};

WX_DECLARE_OBJARRAY(ExportFileItem, ExportFileArray);

class FbExportDlg : public FbDialog
{
	private:
		class ExportLog: public wxLog
		{
			public:
				ExportLog(FbExportDlg * parent);
				virtual ~ExportLog();
			protected:
				void DoLog(wxLogLevel level, const wxChar *szString, time_t t);
				void DoLogString(const wxChar *szString, time_t t) {}
			private:
				FbExportDlg * m_parent;
				wxLog * m_old;
				DECLARE_NO_COPY_CLASS(ExportLog)
		};
		class ExportThread: public wxThread
		{
			public:
				ExportThread(FbExportDlg * parent, int format, const ExportFileItem &item)
					: wxThread(wxTHREAD_JOINABLE), m_parent(parent), m_format(format), m_id(item.id), m_filename(item.filename.GetFullPath()) {}
			protected:
				virtual void * Entry();
			private:
				FbExportDlg * m_parent;
				int m_format;
				int m_id;
				wxString m_filename;
		};
		class GzipThread: public wxThread
		{
			public:
				GzipThread(FbExportDlg * parent, const wxArrayString &args);
				virtual void * Entry();
			private:
				FbExportDlg * m_parent;
				wxArrayString m_filelist;
		};
		class ZipThread: public wxThread
		{
			public:
				ZipThread(FbExportDlg * parent, const wxArrayString &args);
				virtual void * Entry();
			private:
				FbExportDlg * m_parent;
				wxArrayString m_filelist;
		};
		class DelThread: public wxThread
		{
			public:
				DelThread(FbExportDlg * parent, const wxArrayString &args);
				virtual void * Entry();
			private:
				FbExportDlg * m_parent;
				wxArrayString m_filelist;
		};
		class ExportProcess: public wxProcess
		{
			public:
				ExportProcess(FbExportDlg * parent): m_parent(parent) { Redirect(); }
				virtual void OnTerminate(int pid, int status);
				virtual bool HasInput();
				#ifdef __WXMSW__
				bool m_dos;
				#endif // __WXMSW__
			protected:
				FbExportDlg * m_parent;
		};
	public:
		FbExportDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER );
		~FbExportDlg();
		void Execute();
		void LogMessage(const wxString &msg);
		ExportFileArray m_filelist;
		int m_format;
	private:
		void Start();
		void Finish();
		void GzipFiles(const wxArrayString &args);
		void ZipFiles(const wxArrayString &args);
		void DelFiles(const wxArrayString &args);
		wxString GetScript(int format);
		wxString GetCommand(const wxString &script, const wxFileName &filename);
		void ExportFile(size_t index, const ExportFileItem &item);
		void ExecScript(size_t index, const wxFileName &filename);
		wxArrayString m_scripts;
		size_t m_index;
		size_t m_script;
		ExportProcess m_process;
		ExportLog m_log;
		bool m_closed;
		int m_errors;
	private:
		wxStaticText m_info;
		wxListBox m_text;
		wxGauge m_gauge;
		wxButton m_button;
	private:
		void OnIdle(wxIdleEvent& event);
		void OnProcessTerm(wxProcessEvent& event);
		void OnCancelBtn(wxCommandEvent& event);
		void OnCloseBtn(wxCommandEvent& event);
		void OnScriptRun(wxCommandEvent& event);
		void OnScriptLog(wxCommandEvent& event);
		void OnScriptError(wxCommandEvent& event);
		void OnCloseDlg(wxCloseEvent& event);
		DECLARE_EVENT_TABLE()
		DECLARE_CLASS(FbExportDlg);
};

class ExportThread : public BaseThread
{
	public:
		ExportThread(int format): BaseThread(), m_format(format) {};
		virtual void *Entry();
	public:
		ExportFileArray m_filelist;
		bool Execute();
	private:
		void WriteFileItem(ExportFileItem &item);
		wxString GetScript();
		wxString GetCommand(const wxString &script, const wxFileName &filename);
	private:
		int m_format;
		wxArrayString m_scripts;
};

#endif // __EXPTHREAD_H__
