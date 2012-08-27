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

#include "noncopyable.h"

namespace Ogre
{
	class SceneManager;
	class Camera;
	class Vector3;
	class ColourValue;
	struct FrameEvent;
}

namespace OgreBites
{
	class SdkCameraMan;
}

//qt forward declaration
class QMouseEvent;
class QKeyEvent;
class QWheelEvent;
class QRectF;

namespace sandgis
{
	class QtOgreWidget;
	class MapPresenter : private Noncopyable
	{
	public:
		typedef QtOgreWidget ViewType;
		typedef QtOgreWidget* ViewPtr;

		// -------------------------------------------------------------------------------------
		/// @brief  RenderingPresenter cont. & dest.
		///
		/// @param view point to render view
		// =------------------------------------------------------------------------------------
		MapPresenter(ViewPtr view);
		~MapPresenter(void);

		ViewPtr view(void) const
		{
			return view_;
		}
		// -------------------------------------------------------------------------------------
		//view input events
		// =------------------------------------------------------------------------------------
		void _onMouseDown(int button, int x, int y);
		void _onMouseMove(int button, int x, int y);
		void _onMouseUp(int button, int x, int y);
		void _onMouseWheel(int delta);

		void _onKeyDown(int key);
		void _onKeyUp(int key);

		//view focus events
		void _onViewFocusIn(void);
		void _onViewFocusOut(void);

		// -------------------------------------------------------------------------------------
		/// @brief  createScene create scene for render view
		///@param rect:scene initial extents
		///@param bkcolor scene background color
		///@param eye_pos initial eye position
		///@param target_pos initial target position
		// =------------------------------------------------------------------------------------
		void initializeScene(const QRectF& rect,
			const Ogre::ColourValue& bkcolor, 
			const Ogre::Vector3& eye_pos,
			const Ogre::Vector3& target_pos);

		// -------------------------------------------------------------------------------------
		/// @brief  activeCamera gets active Camera
		///
		/// @returns   
		// =------------------------------------------------------------------------------------
		Ogre::Camera* activeCamera(void) const
		{
			return active_camera_;
		}

		// -------------------------------------------------------------------------------------
		/// @brief  sceneManager gets scene manager
		///
		/// @returns  scene manager create by create scene  
		// =------------------------------------------------------------------------------------
		Ogre::SceneManager* sceneManager(void) const
		{
			return scene_;
		}

		///@brief is scene initialized
		bool isSceneInitialized(void) const
		{
			return (scene_ != 0);
		}
	private:
		// -------------------------------------------------------------------------------------
		/// @brief  dispose dispose current presenter
		// =------------------------------------------------------------------------------------
		void dispose(void);

		// -------------------------------------------------------------------------------------
		//ogre frame events
		// =------------------------------------------------------------------------------------
		bool frameStarted(const Ogre::FrameEvent& evt);
		bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	private:
		ViewPtr view_;
		Ogre::SceneManager* scene_;
		Ogre::Camera* active_camera_;

		OgreBites::SdkCameraMan* camera_man_;

		struct EventListener* event_listener_;
		friend struct EventListener;
	};
}
