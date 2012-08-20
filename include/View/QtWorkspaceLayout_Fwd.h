// =====================================================================================
// 
//       Filename:  QtWorkspaceLayout_Fwd.h
// 
//    Description: qt workspace layout forward 
// 
//        Version:  1.0
//        Created:  8/15/2012 6:14:38 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  luoshasha (sand), luoshasha@foxmail.com
//        Company:  
// 
// =====================================================================================

#pragma once

#include <map>
#include <string>

namespace sandgis
{
	class QtWorkspaceLayout;
	class WorkspaceWindow;
	typedef std::map<std::string, WorkspaceWindow*> WindowsCollection;
}
