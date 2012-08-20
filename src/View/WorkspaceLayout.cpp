// =====================================================================================
// 
//       Filename:  WorkspaceLayout.cpp
// 
//    Description:  workspace layout source
// 
//        Version:  1.0
//        Created:  8/15/2012 6:50:21 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  luoshasha (sand), luoshasha@foxmail.com
//        Company:  
// 
// =====================================================================================
#include "view/WorkspaceLayout.h"
#include "view/Workspace.h"
#include "view/QtWorkspaceLayout.h"
#include "view/QtMainWindow.h"
#include "view/QtWorkspaceWindow.h"
#include "view/renderview.h"
#include "view/viewcontent.h"


namespace sandgis
{
	//-------------------------------------------------------
	//qt layout impl
	//-----------------------------------------------------
	QtWorkspaceLayout::QtWorkspaceLayout(void) : active_window_(0),
		workspace_(0), main_tabwidget_(0)
	{
	}

	QtWorkspaceLayout::~QtWorkspaceLayout(void)
	{
		//workspace windows will destroyed when its parent desctroyed
		//then wrappred view content will destroyed
	}

	void QtWorkspaceLayout::tabIndexChanged(int index)
	{
		//gets active window
		QWidget* current = main_tabwidget_->currentWidget();
		active_window_ = dynamic_cast<QTabbedWindow*>(current);
	}

	void QtWorkspaceLayout::attach(Workspace* workspace)
	{
		workspace_ = workspace;
		auto pimpl = workspace_->_workspaceImpl();
		main_tabwidget_ = new QTabWidget(pimpl);
		main_tabwidget_->setObjectName("mainwindowtab");
		connect(main_tabwidget_, SIGNAL(currentChanged(int)), this, SLOT(tabIndexChanged(int)));
		pimpl->setCentralWidget(main_tabwidget_);

		//layout render view
		auto renderviews = pimpl->renderViews();
		auto itview = renderviews.begin();
		for (; itview != renderviews.end(); ++itview)
		{
			this->showViewContent((*itview));
		}
		
		//layout menus and toolbars
		QMenuBar* top_menu = pimpl->topMenu();
		pimpl->setMenuBar(top_menu);
		pimpl->setStatusBar(pimpl->statusBar());
		const QList<QToolBar*> toolbars = pimpl->toolBars();
		auto it = toolbars.begin();
		for (; it != toolbars.end(); ++it)
		{
			pimpl->addToolBar(Qt::TopToolBarArea, (*it));
		}
	}

	void QtWorkspaceLayout::detach(void)
	{
		if (!workspace_)
			return;
	}

	void QtWorkspaceLayout::loadConfiguration(void)
	{

	}

	void QtWorkspaceLayout::storeConfiguration(void)
	{

	}

	WorkspaceWindow* QtWorkspaceLayout::showViewContent(ViewContent* viewcontent, bool switchImmediately)
	{
		//view name can't be empty
		const std::string& viewname = viewcontent->name();
		if (viewname.empty())
			return nullptr;

		auto it = window_list_.find(viewname);
		if (it != window_list_.end())
		{
			it->second->bringToFront();

			if (switchImmediately)
			{
				QTabbedWindow* tabwindow = dynamic_cast<QTabbedWindow*>(it->second);
				main_tabwidget_->setCurrentIndex(tabwindow->tabIndex());
			}
			return it->second;
		}
		QTabbedWindow* tabbed_window = new QTabbedWindow(viewcontent, main_tabwidget_);

		QString title = QString::fromLocal8Bit(viewcontent->title().c_str());
		QString name = QString::fromAscii(viewcontent->name().c_str());
		tabbed_window->setObjectName(name);

		//add tab
		if (title.isEmpty())
		{
			title = QString("[defaultview%1]").arg(window_list_.size());
		}
		int index = main_tabwidget_->addTab(tabbed_window, title);
		window_list_[viewname] = tabbed_window;

		//save tab index
		tabbed_window->setTabIndex(index);
		//switch to index
		if (switchImmediately)
			main_tabwidget_->setCurrentIndex(index);
		return tabbed_window;
	}

	WorkspaceWindow* QtWorkspaceLayout::showDockContent(DockContent* dockcontent)
	{
		//view's name can not be null
		const std::string& viewname = dockcontent->name();
		if (viewname.empty())
			return nullptr;

		auto it = window_list_.find(viewname);
		if (it != window_list_.end())
		{
			it->second->bringToFront();
			return it->second;
		}
		QDockWindow* dock_window = new QDockWindow(dockcontent, workspace_->_workspaceImpl());
		dock_window->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);

		QString title = QString::fromLocal8Bit(dockcontent->title().c_str());
		QString name = QString::fromAscii(dockcontent->name().c_str());
		dock_window->setObjectName(name);

		workspace_->_workspaceImpl()->addDockWidget(dock_window->dockArea(), dock_window);

		window_list_[viewname] = dock_window;

		return dock_window;
	}


	//-------------------------------------------------------
	//workspace layout
	//-----------------------------------------------------
	WorkspaceLayout::WorkspaceLayout(void) :
	layout_(new QtWorkspaceLayout())
	{

	}

	WorkspaceLayout::~WorkspaceLayout(void)
	{
		delete layout_;
	}

	const WindowsCollection& WorkspaceLayout::workspaceWindows(void) const
	{
		return layout_->workspaceWindows();
	}
	WorkspaceWindow* WorkspaceLayout::activeWindow(void) const
	{
		return layout_->activeWindow();
	}
	void WorkspaceLayout::attach(Workspace* workspace)
	{
		return layout_->attach(workspace);
	}
	void WorkspaceLayout::detach(void)
	{
		return layout_->detach();
	}
	void WorkspaceLayout::loadConfiguration(void)
	{
		return layout_->loadConfiguration();
	}
	void WorkspaceLayout::storeConfiguration(void)
	{
		return layout_->storeConfiguration();
	}
	WorkspaceWindow* WorkspaceLayout::showViewContent(ViewContent* viewcontent, bool switchImmediately/* = true*/)
	{
		return layout_->showViewContent(viewcontent, switchImmediately);
	}
	WorkspaceWindow* WorkspaceLayout::showDockContent(DockContent* dockcontent)
	{
		return layout_->showDockContent(dockcontent);
	}
}
