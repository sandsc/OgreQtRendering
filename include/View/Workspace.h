/// @file IWorkspace.h
/// @brief  workspace interface
/// @author luoshasha
/// @version 1.0
/// @date 2012-08-09


#pragma once

#include "Noncopyable.h"

//forward declare
namespace Ogre
{
	class SceneManager;
}

namespace sandgis
{
	class QtMainWindow;
	//forward declare
	class ViewContent;
	class DockContent;
	class WorkspaceWindow;
	class WorkspaceLayout;

	///workspace impletation type
	///using qt mainwindow currently
	typedef QtMainWindow WokspaceImpl;
	///@brief workspace interface
	///@autor luoshasha
	///@remark workspace represents whole workspace, generally it's the entire main window.
	///		including menus, toolbars, statusbar, all tabbed views¡¢dockable views and floatting windows.
	///		tab views must be subclass of IViewContent interface
	///		dockable view must be subclass of IDockContent interface
	///		float window is also dockable content, which is just in different dock state
	///		ViewContent and DockContent will finally wrapped to WorkspaceWindow;
	///		workspace manages all workspace windows
	class Workspace : Noncopyable
	{
	public:
		///workspace imple
		typedef WokspaceImpl _ImplType;
		typedef WorkspaceLayout _LayoutType;
		// -------------------------------------------------------------------------------------
		/// @brief  Workspace cont.
		///
		/// @param impl workspace impl type
		///  @note workspace will take owner of impl instance, impltype should allocate in heap
		///  workspace will delete impl when it desctroyed
		// =------------------------------------------------------------------------------------
		Workspace(_ImplType* impl);

		// -------------------------------------------------------------------------------------
		/// @brief  ~Workspace dest.
		// =------------------------------------------------------------------------------------
		~Workspace(void);

		///@brief gets win handle
		unsigned handle(void) const;
		 
		///@brief gets window's width
		int width(void) const;

		///@brief gets window's height
		int height(void) const;

		///@brief display fps on status bar
		///@param visible trangle numbers
		void displayTriangleNum(int triangle_num);

		///@brief display fps
		///@param fps value
		void displayFPS(float fps);

		///@brief display camera position
		void displayCameraPos(float x, float y, float z);

		///@brief gets if workspace is minimized
		bool isMinimized(void) const;

		///@brief gets workspace implementation instance
		/// internal using
		_ImplType* _workspaceImpl(void) const;
		// -------------------------------------------------------------------------------------
		/// @brief  activeViewContent gets active view content
		///
		/// @returns   
		// =------------------------------------------------------------------------------------
		ViewContent* activeViewContent(void) const;

		// -------------------------------------------------------------------------------------
		/// @brief  layout gets the workspace layout
		///
		/// @returns  workspace layout instance 
		// =------------------------------------------------------------------------------------
		WorkspaceLayout* layout(void) const;
	private:
		_ImplType* impl_;
	};
}
