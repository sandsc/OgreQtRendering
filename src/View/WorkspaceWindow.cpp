// =====================================================================================
// 
//       Filename:  WorkspaceWindow.cpp
// 
//    Description:  workspace window source
// 
//        Version:  1.0
//        Created:  8/15/2012 6:45:14 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  luoshasha (sand), luoshasha@foxmail.com
//        Company:  
// 
// =====================================================================================
#include "view/WorkspaceWindow.h"

namespace sandgis
{
	WorkspaceWindow::WorkspaceWindow(ViewContent* content) :
		content_(content)
	{

	}

	WorkspaceWindow::~WorkspaceWindow(void)
	{
		delete content_;
	}
}
