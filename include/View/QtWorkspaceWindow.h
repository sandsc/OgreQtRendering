// =====================================================================================
// 
//       Filename:  QtWorkspaceWindow.h
// 
//    Description:  qt workspace window
// 
//        Version:  1.0
//        Created:  8/15/2012 6:18:21 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  luoshasha (sand), luoshasha@foxmail.com
//        Company:  
// 
// =====================================================================================
#pragma once

#include "view/WorkspaceWindow.h"

#include <QWidget>
#include <QDockWidget>

namespace sandgis
{
	class DockContent;
	class QTabbedWindow : public WorkspaceWindow, public QWidget
	{
	public:
		QTabbedWindow(ViewContent* content, QWidget* parent = 0);
		~QTabbedWindow(void);

		//----------------------------------------------------------------------
		//  workspace window override
		//----------------------------------------------------------------------
		void close(void);
		void hide(void);
		void bringToFront(void);
		void selectWindow(void);

		int tabIndex(void) const
		{
			return tabbed_index_;
		}

		void setTabIndex(int index)
		{
			tabbed_index_ = index;
		}
	private:
		int tabbed_index_;
	};

	class QDockWindow : public WorkspaceWindow, public QDockWidget
	{
	public:
		QDockWindow(DockContent* content, QWidget* parent = 0);
		~QDockWindow(void);
		//----------------------------------------------------------------------
		//  workspace window override
		//----------------------------------------------------------------------
		void close(void);
		void hide(void);
		void bringToFront(void);
		void selectWindow(void);

		///gets qt dockarea
		Qt::DockWidgetArea dockArea(void) const
		{
			return static_cast<Qt::DockWidgetArea>(qdock_area_);
		}

	private:
		int qdock_area_;
	};
}
