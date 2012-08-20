// =====================================================================================
// 
//       Filename:  QtWorkspaceWindow.cpp
// 
//    Description:  qt workspace window implementation
// 
//        Version:  1.0
//        Created:  8/15/2012 6:25:47 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  luoshasha (sand), luoshasha@foxmail.com
//        Company:  
// 
// =====================================================================================

#include "view/QtWorkspaceWindow.h"
#include "view/ViewContent.h"

#include <QVBoxLayout>

namespace sandgis
{
	QTabbedWindow::QTabbedWindow(ViewContent* content, QWidget* parent) : WorkspaceWindow(content),
		QWidget(parent), tabbed_index_(-1)
	{
		QString title = QString::fromLocal8Bit(content->title().c_str());
		this->setWindowTitle(title);
		QVBoxLayout *layout = new QVBoxLayout();
		layout->setSpacing(0);
		layout->setMargin(0);
		layout->addWidget(content->content(), 1);
		this->setLayout(layout);
	}

	QTabbedWindow::~QTabbedWindow(void)
	{
	}

	void QTabbedWindow::close(void)
	{
		QWidget::close();
	}

	void QTabbedWindow::hide(void)
	{
		QWidget::hide();
	}

	void QTabbedWindow::bringToFront(void)
	{
		QWidget::showNormal();
	}

	void QTabbedWindow::selectWindow(void)
	{
		QWidget::setFocus(Qt::ActiveWindowFocusReason);
	}

	//--------------------------------------------------------------
	//dock window
	//--------------------------------------------------------------
	QDockWindow::QDockWindow(DockContent* content, QWidget* parent) : WorkspaceWindow(content),
		QDockWidget(parent)
	{
		QString title = QString::fromLocal8Bit(content->title().c_str());
		this->setWindowTitle(title);
		this->setWidget(content->content());

	/*LeftDockWidgetArea = 0x1,
        RightDockWidgetArea = 0x2,
        TopDockWidgetArea = 0x4,
        BottomDockWidgetArea = 0x8,

        DockWidgetArea_Mask = 0xf,
        AllDockWidgetAreas = DockWidgetArea_Mask,
        NoDockWidgetArea = 0*/

		dockstate::DockState ds = content->dockstate();
		switch (ds)
		{
		case dockstate::None:
			qdock_area_ = Qt::NoDockWidgetArea;
			break;
		case dockstate::DockLeft:
			qdock_area_ = Qt::LeftDockWidgetArea;
			break;
		case dockstate::DockRight:
			qdock_area_ = Qt::RightDockWidgetArea;
			break;
		case dockstate::DockTop:
			qdock_area_ = Qt::TopDockWidgetArea;
			break;
		case dockstate::DockBottom:
			qdock_area_ = Qt::BottomDockWidgetArea;
			break;
		case dockstate::DockCenter:
			qdock_area_ = Qt::AllDockWidgetAreas;
			break;
		default:
			qdock_area_ = Qt::NoDockWidgetArea;
			break;
		}
	}

	QDockWindow::~QDockWindow(void)
	{
	}

	void QDockWindow::close(void)
	{
		QWidget::close();
	}

	void QDockWindow::hide(void)
	{
		QWidget::hide();
	}

	void QDockWindow::bringToFront(void)
	{
		QDockWidget::show();
	}

	void QDockWindow::selectWindow(void)
	{
		QWidget::setFocus(Qt::ActiveWindowFocusReason);
	}
}
