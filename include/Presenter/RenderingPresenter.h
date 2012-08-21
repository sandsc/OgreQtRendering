// =====================================================================================
// 
//       Filename:  RenderingPresenter.h
// 
//    Description:  render view presenter
// 
//        Version:  1.0
//        Created:  8/21/2012 11:07:56 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  luoshasha (sand), luoshasha@foxmail.com
//        Company:  
// 
// =====================================================================================
#pragma once

namespace Ogre
{
	class SceneManager;
}

namespace sandgis
{
	class QtMainRenderView;
	class RenderingPresenter
	{
	public:
		typedef QtMainRenderView ViewType;
		typedef QtMainRenderView* ViewPtr;

		// -------------------------------------------------------------------------------------
		/// @brief  RenderingPresenter cont. & dest.
		///
		/// @param view point to render view
		// =------------------------------------------------------------------------------------
		RenderingPresenter(ViewPtr view);
		~RenderingPresenter(void);

		ViewPtr view(void) const
		{
			return view_;
		}

		// -------------------------------------------------------------------------------------
		/// @brief  _onViewClosed called when view closed
		// =------------------------------------------------------------------------------------
		void _onViewClosed(void);

		// -------------------------------------------------------------------------------------
		/// @brief  createScene create scene for render view
		// =------------------------------------------------------------------------------------
		void createScene(void);

	private:
		ViewPtr view_;
		

	};
}
