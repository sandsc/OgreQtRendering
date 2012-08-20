// =====================================================================================
// 
//       Filename:  Workspace.cpp
// 
//    Description:  workspace source
// 
//        Version:  1.0
//        Created:  8/14/2012 10:31:48 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  luoshasha (sand), luoshasha@foxmail.com
//        Company:  
// 
// =====================================================================================

#include "view/Workspace.h"
#include "view/QtMainWindow.h"
#include "view/ViewContent.h"
#include "view/WorkspaceWindow.h"
#include "view/workspacelayout.h"
namespace sandgis {

	Workspace::Workspace(_ImplType* impl) :
		impl_(impl)
	{
		impl_->workspaceLayout()->attach(this);
	}

	Workspace::~Workspace(void)
	{
		delete impl_;
		impl_ = nullptr;
	}

	unsigned Workspace::handle(void) const
	{
		return impl_->handle();
	}

	Workspace::_ImplType* Workspace::_workspaceImpl(void) const
	{
		return impl_;
	}

	int Workspace::width(void) const
	{
		return impl_->width();
	}

	int Workspace::height(void) const
	{
		return impl_->height();
	}

	void Workspace::displayTriangleNum(int triangle_num)
	{
		impl_->displayTriangleNum(triangle_num);
	}

	void Workspace::displayCameraPos(float x, float y, float z)
	{
		impl_->displayCameraPos(x, y, z);
	}

	void Workspace::displayFPS(float fps)
	{
		impl_->displayFPS(fps);
	}

	bool Workspace::isMinimized(void) const
	{
		return impl_->isMinimized();
	}

	ViewContent* Workspace::activeViewContent(void) const
	{
		return nullptr;
	}

	WorkspaceLayout* Workspace::layout(void) const
	{
		return impl_->workspaceLayout();
	}
}
