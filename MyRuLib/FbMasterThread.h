#ifndef __FBMASTERTHREAD_H__
#define __FBMASTERTHREAD_H__

#include <wx/event.h>
#include "FbThread.h"
#include "FbMasterInfo.h"

class FbMasterThread : public FbThread
{
	public:
		FbMasterThread(wxEvtHandler * owner);
		virtual ~FbMasterThread();
		void Reset(const FbMasterInfo &info);
	    virtual void Close();
	protected:
		virtual void * Entry();
	private:
		FbMasterInfo GetInfo();
		static wxCriticalSection sm_section;
		FbCondition m_condition;
		wxEvtHandler * m_owner;
		FbMasterInfo m_info;
		FbThread * m_thread;
		bool m_exit;
};

#endif // __FBMASTERTHREAD_H__
