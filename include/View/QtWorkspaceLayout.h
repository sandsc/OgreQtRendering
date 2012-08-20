// =====================================================================================
// 
//       Filename:  QtWorkspaceLayout.h
// 
//    Description:  qt worksapce layout impl
// 
//        Version:  1.0
//        Created:  8/17/2012 10:38:27 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  luoshasha (sand), luoshasha@foxmail.com
//        Company:  
// 
// =====================================================================================
#pragma once
#include "view/QtWorkspaceLayout_Fwd.h"

#include <QObject>
QT_BEGIN_NAMESPACE
class QTabWidget;
QT_END_NAMESPACE


namespace sandgis
{
	class Workspace;
	class ViewContent;
	class DockContent;
	///----------------------------------------------------
	///@brief qt workspace layout implementation
	class QtWorkspaceLayout : public QObject
	{
		Q_OBJECT
	public:
		QtWorkspaceLayout(void);
		~QtWorkspaceLayout(void);
		const WindowsCollection& workspaceWindows(void) const
		{
			return window_list_;
		}
		WorkspaceWindow* activeWindow(void) const
		{
			return active_window_;
		}
		void attach(Workspace* workspace);
		void detach(void);
		void loadConfiguration(void);
		void storeConfiguration(void);
		WorkspaceWindow* showViewContent(ViewContent* viewcontent, bool switchImmediately = true);
		WorkspaceWindow* showDockContent(DockContent* dockcontent);

		private slots:
			void tabIndexChanged(int index);

	private:
		WindowsCollection window_list_;
		WorkspaceWindow* active_window_;
		Workspace* workspace_;
		QTabWidget* main_tabwidget_;
	};
	
}
