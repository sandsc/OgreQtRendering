/// @file IViewContent.h
/// @brief  tabbed view interface
/// @author luoshasha
/// @version 1.0
/// @date 2012-08-09

#pragma once

#include <string>
class QWidget;

namespace sandgis
{
	namespace dockstate
	{
		// -------------------------------------------------------------------------------------
		/// @brief  _DockState 
		// =------------------------------------------------------------------------------------
		enum DockState
		{
			///unknown dockstate
			None,
			///dock to the left of parent window
			DockLeft,
			///dock to the right of parent window
			DockRight,
			///dock to top
			DockTop,
			///dock to bottom
			DockBottom,
			///dock to center
			DockCenter,
			///dock as floatting window
			Floatting
		};
	}
	///@brief ViewContent is a tabbed view of workspace
	///@author luoshasha
	class ViewContent
	{
	public:
		typedef QWidget _ContentType;

		ViewContent(const std::string& name);
		virtual ~ViewContent(void);
		// -------------------------------------------------------------------------------------
		/// @brief  content gets content of this view
		///
		/// @returns content of this view 
		// =------------------------------------------------------------------------------------
		virtual _ContentType* content(void) = 0;

		///@brief update view content
		virtual void refresh(void) {}

		// -------------------------------------------------------------------------------------
		/// @brief  title gets view content title
		///
		/// @returns   
		// =------------------------------------------------------------------------------------
		std::string title(void) const
		{
			return title_;
		}

		// -------------------------------------------------------------------------------------
		/// @brief  setTitle set view content title
		///
		/// @param title 
		// =------------------------------------------------------------------------------------
		void setTitle(const std::string& title);

		///@brief gets the name of this view content
		std::string name(void) const
		{
			return name_;
		}

	protected:
		// -------------------------------------------------------------------------------------
		/// @brief  onTitleChanged called when title changed
		// =------------------------------------------------------------------------------------
		virtual void onTitleChanged(void) {}

		///@brief sets the name of view content
		///@param name param name
		void setName(const std::string& name)
		{
			name_ = name;
		}

	private:
		std::string title_;
		std::string name_;
	};

	///@brief dockcontent is dockable or floatting window in workspace
	///author luoshasha
	class DockContent : public ViewContent
	{
	public:
		DockContent(const std::string& name);
		virtual ~DockContent(void);

		// -------------------------------------------------------------------------------------
		/// @brief  dockstate gets dock state
		///
		/// @returns   
		// =------------------------------------------------------------------------------------
		dockstate::DockState dockstate(void) const
		{
			return dock_state_;
		}

		// -------------------------------------------------------------------------------------
		/// @brief  setDockState sets docksate
		///
		/// @param state dockstate
		// =------------------------------------------------------------------------------------
		void setDockState(dockstate::DockState state);

	protected:

		// -------------------------------------------------------------------------------------
		/// @brief  onDockStateChanged called when dock state changed
		// =------------------------------------------------------------------------------------
		virtual void onDockStateChanged(void) 
		{

		}

	private:
		dockstate::DockState dock_state_;	
	};
}
