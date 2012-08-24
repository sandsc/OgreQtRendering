// =====================================================================================
// 
//       Filename:  RenderingPresenter.cpp
// 
//    Description:  rendering presenter for render view
// 
//        Version:  1.0
//        Created:  8/22/2012 9:15:57 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  luoshasha (sand), luoshasha@foxmail.com
//        Company:  
// 
// =====================================================================================

#include "presenter/MapPresenter.h"
#include "view/QtMainRenderView.h"
#include "presenter/WorkspaceRoot.h"
#include "presenter/SdkCameraMan.h"
#include "presenter/OgreContext.h"

using namespace OgreBites;
using namespace Ogre;
namespace sandgis
{
	MapPresenter::MapPresenter(ViewPtr view)
		: view_(view),
		camera_man_(0),
		scene_(0)
	{

	}

	MapPresenter::~MapPresenter(void)
	{
		if (camera_man_)
			delete camera_man_;
	}

	void  MapPresenter::_onViewClosed(void)
	{
		this->dispose();
	}

	void MapPresenter::_onMouseDown(int button, int x, int y)
	{
		camera_man_->injectMouseDown(button, x, y);
	}

	void MapPresenter::_onMouseMove(int button, int x, int y)
	{
		camera_man_->injectMouseMove(button, x, y);
	}

	void MapPresenter::_onMouseUp(int button, int x, int y)
	{
		camera_man_->injectMouseUp(button, x, y);
	}

	void MapPresenter::_onKeyDown(int key)
	{
		camera_man_->injectKeyDown(key);
	}

	void MapPresenter::_onKeyUp(int key)
	{
		camera_man_->injectKeyUp(key);
	}

	void MapPresenter::_onViewFocusIn(void)
	{
	}

	void MapPresenter::_onViewFocusOut(void)
	{
	}

	void MapPresenter::_onMouseWheel(int delta)
	{

	}

	void MapPresenter::initializeScene(const QRectF& rect,
			const Ogre::ColourValue& bkcolor, 
			const Ogre::Vector3& eye_pos,
			const Ogre::Vector3& target_pos)
	{
		if (!view_->renderWindow())
			return;
		OgreContext* pOgreContext = WorkspaceRoot::instance()->ogreContext();
		scene_ = pOgreContext->createScene("MainRenderScene");

		//----------------------------------scene rendering test-------------------
		// Setup animation default
		Ogre::Animation::setDefaultInterpolationMode(Ogre::Animation::IM_LINEAR);
		Ogre::Animation::setDefaultRotationInterpolationMode(Ogre::Animation::RIM_LINEAR);

		// Set ambient light
		scene_->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 1));

		//create camera
		active_camera_ = scene_->createCamera("MainRenderScene.Camera");
		// Position it at 500 in Z direction
		active_camera_->setPosition(eye_pos);
		// Look back along -Z
		active_camera_->lookAt(target_pos);
		//camera->setPolygonMode(Ogre::PM_WIREFRAME);

		//set aspect ratio determine left and right
		active_camera_->setAspectRatio(rect.width() / rect.height());
		//set fovy determine top and bottom
		float near_distance = 10;
		float far_distance = 10000;
		float fov = 2 * ::atan(rect.height() * 0.5F / near_distance);
		//camera->setFOVy(Ogre::Radian(fov));
		//set near and far distance
		active_camera_->setNearClipDistance(near_distance);
		active_camera_->setFarClipDistance(far_distance);

		//initialise camera man
		camera_man_= new SdkCameraMan(active_camera_);

		//add viewport
		Ogre::RenderWindow* pRenderWindow = view_->renderWindow();
		Viewport* vp = pRenderWindow->addViewport(active_camera_);
		vp->setBackgroundColour(bkcolor);

		//------------------------------rendering test---------------------
		// Create the robot scene
		Entity* robotEntity = scene_->createEntity("robot", "robot.mesh");

		// Add entity to the scene node
		// Place and rotate to face the Z direction
		Vector3 robotLoc(1500, 1480, 0);
		Quaternion robotRot(Degree(-90), Vector3(0, 1, 0));

		scene_->getRootSceneNode()->createChildSceneNode(robotLoc, robotRot)->attachObject(robotEntity);

		AnimationState* robotWalkState = robotEntity->getAnimationState("Walk");
		robotWalkState->setEnabled(true);

		// Create the ninja entity
		Entity *ent = scene_->createEntity("ninja", "ninja.mesh");

		// Add entity to the scene node
		// Place and rotate to face the Z direction
		Vector3 ninjaLoc(1460, 1470, 0);
		Quaternion ninjaRot(Degree(180), Vector3(0, 1, 0));

		SceneNode *ninjaNode = scene_->getRootSceneNode()->createChildSceneNode(ninjaLoc, ninjaRot);
		ninjaNode->scale(0.5, 0.5, 0.5);        // He's twice as big as our robot...
		ninjaNode->attachObject(ent);

		AnimationState* ninjaWalkState = ent->getAnimationState("Walk");
		ninjaWalkState->setEnabled(true);

		// Give it a little ambience with lights
		Ogre::Light* l;
		l = scene_->createLight("BlueLight");
		l->setPosition(1500,1500,100);
		l->setDiffuseColour(0.5, 0.5, 1.0);

		l = scene_->createLight("GreenLight");
		l->setPosition(1460,1450,-100);
		l->setDiffuseColour(0.5, 1.0, 0.5);
		//---------------------------------------------rendering test-------------------------
	}

	void MapPresenter::dispose(void)
	{
		if (scene_)
		{
			WorkspaceRoot::instance()->ogreContext()->root()->destroySceneManager(scene_);
		}
	}
}
