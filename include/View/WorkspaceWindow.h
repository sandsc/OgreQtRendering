/// @file IWorkspaceWindow.h
/// @brief  workspace window interface
/// @author luoshasha
/// @version 1.0
/// @date 2012-08-09

#pragma once

namespace sandgis
{

	class ViewContent;
	///@brief child window of workspace
	///@autor luoshasha
	class WorkspaceWindow
	{
	public:
		///constructor
		WorkspaceWindow(ViewContent* content);
		virtual ~WorkspaceWindow(void);

		// -------------------------------------------------------------------------------------
		/// @brief  viewContent gets view content
		///
		/// @returns view content
		// =------------------------------------------------------------------------------------
		ViewContent* viewContent(void) const
		{
			return content_;
		}

		// -------------------------------------------------------------------------------------
		/// @brief  closeWindow 
		// =------------------------------------------------------------------------------------
		virtual void close(void) = 0;

		// -------------------------------------------------------------------------------------
		/// @brief  hideWindow 
		// =------------------------------------------------------------------------------------
		virtual void hide(void) = 0;

		// -------------------------------------------------------------------------------------
		/// @brief  bring window to front 
		// =------------------------------------------------------------------------------------
		virtual void bringToFront(void) = 0;

		// -------------------------------------------------------------------------------------
		/// @brief  selectWindow make window get focus
		// =------------------------------------------------------------------------------------
		virtual void selectWindow(void) = 0;
	private:
		ViewContent* content_;
	};
}
