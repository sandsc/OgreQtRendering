/// @file IWorkspaceLayout.h
/// @brief  workspace layout
/// @author luoshasha
/// @version 1.0
/// @date 2012-08-09

#pragma once

#include "view/QtWorkspaceLayout_Fwd.h"
#include "Noncopyable.h"

namespace sandgis
{
	//----------------------------------------------------------------------
	//  forward declaration
	//----------------------------------------------------------------------
	class ViewContent;
	class DockContent;
	class Workspace;
	///@brief layout interface manage all layout of worspace's subwindow
	///@autor luoshasha
	class WorkspaceLayout : Noncopyable
	{
	public:
		typedef QtWorkspaceLayout _LayoutImplType;
		// -------------------------------------------------------------------------------------
		/// @brief  WorkspaceLayout cont. & dest.
		// =------------------------------------------------------------------------------------
		WorkspaceLayout(void);
		~WorkspaceLayout(void);

		// -------------------------------------------------------------------------------------
		/// @brief  workspaceWindows gets workspace windows
		///
		/// @returns  worksace windows collection 
		// =------------------------------------------------------------------------------------
		const WindowsCollection& workspaceWindows(void) const;

		// -------------------------------------------------------------------------------------
		/// @brief  activeWindow gets active workspace window
		///
		/// @returns  gets active workspace window 
		// =------------------------------------------------------------------------------------
		WorkspaceWindow* activeWindow(void) const;

		// -------------------------------------------------------------------------------------
		/// @brief  attach attach workspace
		///
		/// @param workspace 
		// =------------------------------------------------------------------------------------
		void attach(Workspace* workspace);

		// -------------------------------------------------------------------------------------
		/// @brief  dettach detach workspace
		// =------------------------------------------------------------------------------------
		void detach(void);

		// -------------------------------------------------------------------------------------
		/// @brief  loadconfigration load configration such as layout config
		// =------------------------------------------------------------------------------------
		void loadConfiguration(void);

		// -------------------------------------------------------------------------------------
		/// @brief  storeconfiguration save layout configuration
		// =------------------------------------------------------------------------------------
		void storeConfiguration(void);

		// -------------------------------------------------------------------------------------
		/// @brief  showViewContent show view content
		/// @param viewcontent view content to show
		// =------------------------------------------------------------------------------------
		WorkspaceWindow* showViewContent(ViewContent* viewcontent, bool switchImmediately = true);

		// -------------------------------------------------------------------------------------
		/// @brief  showDockContent show docktent
		///
		/// @param dockcontent DockContent
		// =------------------------------------------------------------------------------------
		WorkspaceWindow* showDockContent(DockContent* dockcontent);

	private:
		_LayoutImplType* layout_;
	};
}
