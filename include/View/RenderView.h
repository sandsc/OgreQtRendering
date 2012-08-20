/// @file IRenderView.h
/// @brief  represent a view for ogre rendering
/// @author luoshasha
/// @version 1.0
/// @date 2012-08-09

#pragma once

#include "view\ViewContent.h"

//----------------------------------------------------------------------
//  forward declaration
//----------------------------------------------------------------------
namespace Ogre
{
	class RenderWindow;
}

namespace sandgis
{
	///@brief IRenderView
	///@autor luoshasha
	class RenderView : public ViewContent
	{
	public:
		// -------------------------------------------------------------------------------------
		/// @brief  ~RenderView virtual dest.
		// =------------------------------------------------------------------------------------
		virtual ~RenderView(void) {}

		// === FUNCTION ======================================================================
		/// @brief  renderWindow gets render window
		/// @author luoshasha
		/// @returns  ogre render window 
		// =====================================================================================
		virtual Ogre::RenderWindow* renderWindow(void) const = 0;

		// -------------------------------------------------------------------------------------
		/// @brief  sceneLoaded notify when a scene has loaded
		// =------------------------------------------------------------------------------------
		virtual void sceneLoaded(void) = 0;

		// -------------------------------------------------------------------------------------
		/// @brief  sceneDestroyed notify when a scene has been destroyed
		// =------------------------------------------------------------------------------------
		virtual void sceneDestroyed(void) = 0; 

		// -------------------------------------------------------------------------------------
		/// @brief  currentSceneName gets current scene name
		///
		/// @returns   
		// =------------------------------------------------------------------------------------
		virtual const char* currentSceneName(void) const = 0;
	};
}
