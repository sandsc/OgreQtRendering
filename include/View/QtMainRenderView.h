// =====================================================================================
// 
//       Filename:  QtMainRenderView.h
// 
//    Description:  qt main render view
// 
//        Version:  1.0
//        Created:  8/17/2012 10:49:24 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  luoshasha (sand), luoshasha@foxmail.com
//        Company:  
// 
// =====================================================================================
#pragma once

#include "view/RenderView.h"
namespace sandgis
{
	class QtOgreWidget;
	// -------------------------------------------------------------------------------------
	/// @brief  qt render view impl. using qt ogre widget
	// =------------------------------------------------------------------------------------
	class QtMainRenderView : public RenderView
	{
	public:
		typedef ViewContent::_ContentType _ContentType;
		///@brief ¹¹Ôìº¯Êý
		///@param parent content parent
		///@param do load file
		///@param window_flags 
		///@content_owner if content_owner is true, render view will be in charge of 
		/// content, that means when view destroyed it will destroy widget also.
		/// otherwise content will manage by it's parent(QT UI Object Tree)
		QtMainRenderView(QWidget* parent=0, 
			bool doLoadFile = false,
			int window_flags=0,
			bool content_owner = false);
		~QtMainRenderView(void);

		Ogre::RenderWindow* renderWindow(void) const;

		void sceneLoaded(void);
		void sceneDestroyed(void);
		void update(void);
		const char* currentSceneName(void) const;

		_ContentType* content(void) const;
	private:
		QtOgreWidget* ogre_widget_;
		bool content_owner_;
	};
}
