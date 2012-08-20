/// @file ogrewidget.cpp
/// @brief  ogre renderwindow using qt 
// original code is ported from Ogitor
// see Ogitor website www.ogitor.org
//
/// @author luoshasha
/// @version 1.0
/// @date 2012-08-10
#include "view/qtogrewidget.h"
#include "view/Workspace.h"
#include "presenter/WorkspaceRoot.h"
#include "presenter/OgreContext.h"

#include "Ogre.h"

#include <QtGui>

namespace sandgis
{
	//----------------------------------------------------------------------------------------
	void OverlayWidget::paintEvent(QPaintEvent* evt)
	{
		QPainter painter(this);
		painter.setClipRect(0,0,width(),height());
		painter.setBrush(QBrush(QColor(0,0,0)));
		painter.fillRect(QRectF(0,0,width(),height()), QColor(0,0,0));
		painter.setPen(QColor(210,210,210));
		painter.drawText(QRectF(0,0,width(),height()),msgstr,QTextOption(Qt::AlignVCenter | Qt::AlignHCenter));
	}
	//----------------------------------------------------------------------------------------
	QtOgreWidget::QtOgreWidget(QWidget *parent, bool doLoadFile, Qt::WindowFlags f): QWidget( parent,  f /*| Qt::MSWindowsOwnDC*/ ),
		mRenderWindow(0), 
		mOgreInitialised(false),
		mLastKeyEventTime(0),
		mRenderStop(false), 
		mScreenResize(false), 
		mCursorHidden(false),
		disposed_(false),
		mDoLoadFile(doLoadFile)
	{
		mFrameCounter = 0;
		mTotalFrameTime = 0;
		mSwitchingScene = false;

		/*for(int i = 0;i < 1024;i++)
			ViewKeyboard[i] = false;*/

		setAcceptDrops(true);
		setContextMenuPolicy( Qt::PreventContextMenu );

		setFocusPolicy(Qt::WheelFocus);
		setMouseTracking(true);
		setAttribute(Qt::WA_NoBackground);
		setAttribute(Qt::WA_PaintOnScreen);

		mOverlayWidget = new OverlayWidget(this);
		QVBoxLayout *layout = new QVBoxLayout();
		layout->setMargin(0);
		layout->addWidget(mOverlayWidget);
		setLayout(layout);
	}
	//----------------------------------------------------------------------------------------
	QtOgreWidget::~QtOgreWidget()
	{
		/*if(mOgreInitialised)
		{
			Ogre::Root* root = WorkspaceRoot::instance()->ogreContext()->root();
			root->removeFrameListener(this);
			root->getRenderSystem()->removeListener(this);
		} */   
		destroy();
		disposed_ = true;
	}
	//----------------------------------------------------------------------------------------
	static Ogre::Vector3 oldCamPos = Ogre::Vector3::ZERO;
	static int oldTris = 0;

	bool QtOgreWidget::frameStarted(const Ogre::FrameEvent& evt)
	{
		float fps = mRenderWindow->getAverageFPS();//this->caculateFPS(evt.timeSinceLastFrame);
		WorkspaceRoot::instance()->workspace()->displayFPS(fps);

		const Ogre::Vector3& campos = mRenderWindow->getViewport(0)->getCamera()->getPosition();
		if(oldCamPos != campos)
		{
			WorkspaceRoot::instance()->workspace()->displayCameraPos(campos.x, campos.y, campos.z);
			oldCamPos = campos;
		}

		int tris = mRenderWindow->getTriangleCount();
		if(oldTris != tris)
		{
			WorkspaceRoot::instance()->workspace()->displayTriangleNum(tris);
			oldTris = tris;
		}

		//------------------------------test--------------------------------------
		Ogre::AnimationState* robotWalkState = WorkspaceRoot::instance()->ogreContext()->getFirstSceneManager()->getEntity("robot")->getAnimationState("Walk");
		robotWalkState->addTime(evt.timeSinceLastFrame);

		Ogre::AnimationState* ninjaWalkState = WorkspaceRoot::instance()->ogreContext()->getFirstSceneManager()->getEntity("ninja")->getAnimationState("Walk");
		ninjaWalkState->addTime(evt.timeSinceLastFrame);

		return true;
	}

	void QtOgreWidget::eventOccurred (const Ogre::String &eventName, const Ogre::NameValuePairList *parameters /*=0*/ )
	{
		//rendersystem listener
	}

	//----------------------------------------------------------------------------------------
	void QtOgreWidget::initializeRenderWindow()
	{
		//create ogre render window
#if defined(Q_WS_MAC) || defined(Q_WS_WIN)
		mRenderWindow = WorkspaceRoot::instance()->ogreContext()->initRenderWindowFromExternal(
			"QtSandGisRenderWindow",
			(unsigned)this->winId(),
			this->width(),
			this->height(), 
			0, 
			false);
#else
		const QX11Info info = this->x11Info();
		mRenderWindow = WorkspaceRoot::instance()->ogreContext()->initRenderWindowFromExternal("QtSandGisRenderWindow",
			(unsigned)this->winId(), 
			this->width(),
			this->height(),
			&info, false);
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
		mRenderWindow->windowMovedOrResized();
		//resizeEvent(0);
#endif
#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
		mRenderWindow->resize(width(), height());
#endif
		Ogre::Root* proot = WorkspaceRoot::instance()->ogreContext()->root();
		proot->getRenderSystem()->addListener(this);
		proot->addFrameListener(this);
		Ogre::MeshManager::getSingletonPtr()->setListener(this);
		mOgreInitialised = true;

		//--------------test load scene--------------------
		OgreContext* pOgreContext = WorkspaceRoot::instance()->ogreContext();
		QRectF rect(0, 3000, 3000, 3000);
		pOgreContext->createScene("MainRenderScene", rect, Ogre::ColourValue(0.4F, 0.5F, 0.3F), 
			Ogre::Vector3(1500, 1500, 200), Ogre::Vector3(1500, 1500, 0), mRenderWindow);
		mOverlayWidget->hide();
	}

	//------------------------------------------------------------------------------------
	void QtOgreWidget::timerLoop()
	{
		if(WorkspaceRoot::instance()->workspace()->isMinimized())
			return;

		update();
	}
	//------------------------------------------------------------------------------------
	void QtOgreWidget::setDoLoadFile(bool doLoad)
	{
		mDoLoadFile = doLoad;
	}
	//------------------------------------------------------------------------------------
	bool adjustFrameTime = false;

	void QtOgreWidget::paintEvent(QPaintEvent* evt)
	{
		if(!isVisible())
			return;

		if(!mRenderWindow)
			initializeRenderWindow();

		Ogre::Root* proot = WorkspaceRoot::instance()->ogreContext()->root();
		bool is_scene_loaded = WorkspaceRoot::instance()->ogreContext()->isSceneLoaded();
		if(mOgreInitialised && is_scene_loaded && !mRenderStop)
		{
			if(this->width() > 0 && this->height() > 0)
			{
				if(adjustFrameTime)
				{
					proot->renderOneFrame(0.01f);
					adjustFrameTime = false;
				}
				else
				{
					proot->renderOneFrame();
				}

#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
				mRenderWindow->update();
#endif
			}
		}
		else
		{
			QString msgstr = tr("Initializing OGRE...");
			if(mOgreInitialised && !is_scene_loaded)
			{
				if(mDoLoadFile)
					msgstr = tr("Loading Scene...");
				else
					msgstr = tr("Scene is not load...");
			}

			if(mRenderStop)
				msgstr = tr("Rendering stopped...");

			mOverlayWidget->setMessageString(msgstr);

			adjustFrameTime = true;
		}
	}
	//------------------------------------------------------------------------------------
	void QtOgreWidget::resizeEvent(QResizeEvent* evt)
	{
		if(!mRenderWindow)
			return;

		mScreenResize = true;

#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
		mRenderWindow->resize(width(), height());
#endif

		mRenderWindow->windowMovedOrResized();

		mScreenResize = false;
	}


	//----------------------------------------------------------------------
	//  
	//----------------------------------------------------------------------
	void QtOgreWidget::sceneLoaded(void)
	{
		mOverlayWidget->hide();
	}

	void QtOgreWidget::sceneDestroyed(void)
	{

	}
	
	//------------------------------------------------------------------------------------
	void QtOgreWidget::focusInEvent(QFocusEvent *evt)
	{
		//if(!OgitorsRoot::getSingletonPtr()->IsSceneLoaded())
		//    return;

	/*	for(int i = 0;i < 1024;i++)
			ViewKeyboard[i] = false;*/

		evt->setAccepted(true);
	}
	//------------------------------------------------------------------------------------
	void QtOgreWidget::focusOutEvent(QFocusEvent *evt)
	{
		/*if(!OgitorsRoot::getSingletonPtr()->IsSceneLoaded())
		return;*/

		/*for(int i = 0;i < 1024;i++)
			ViewKeyboard[i] = false;*/

		/* if(OgitorsRoot::getSingletonPtr()->GetTerrainEditor())
		OgitorsRoot::getSingletonPtr()->GetTerrainEditor()->stopEdit();*/

		evt->setAccepted(true);
	}
	//------------------------------------------------------------------------------------
	void QtOgreWidget::keyPressEvent(QKeyEvent *evt)
	{
		if(evt->isAutoRepeat())
			return;

		/* if(!OgitorsRoot::getSingletonPtr()->IsSceneLoaded())
		return;*/

		unsigned int key = evt->key();
		if(key > 255)
			key = (key & 0xFFF) + 0xFF;

		//OgitorsRoot::getSingletonPtr()->OnKeyDown(key);
	}
	//------------------------------------------------------------------------------------
	void QtOgreWidget::keyReleaseEvent(QKeyEvent *evt)
	{
		if(evt->isAutoRepeat())
			return;

		/*   if(!OgitorsRoot::getSingletonPtr()->IsSceneLoaded())
		return;*/

		unsigned int key = evt->key();
		if(key > 255)
			key = (key & 0xFFF) + 0xFF;

		//OgitorsRoot::getSingletonPtr()->OnKeyUp(key);
	}
	//------------------------------------------------------------------------------------
	unsigned int QtOgreWidget::getMouseButtons(Qt::MouseButtons buttons, Qt::MouseButton button)
	{
		unsigned int flags = 0;
		buttons |= button;

		/* if(buttons.testFlag(Qt::LeftButton))
		flags |= OMB_LEFT;
		if(buttons.testFlag(Qt::RightButton))
		flags |= OMB_RIGHT;
		if(buttons.testFlag(Qt::MidButton))
		flags |= OMB_MIDDLE;*/

		return flags;
	}
	//------------------------------------------------------------------------------------
	bool OgreWidgetMouseMovedSincePress = false;
	void QtOgreWidget::mouseMoveEvent(QMouseEvent *evt)
	{
		/* if(!OgitorsRoot::getSingletonPtr()->IsSceneLoaded())
		return;*/

		if(!hasFocus())
			setFocus();

		Ogre::Vector2 pos(evt->x(), evt->y());

		//OgitorsRoot::getSingletonPtr()->OnMouseMove(pos, getMouseButtons(evt->buttons(), evt->button()));

		OgreWidgetMouseMovedSincePress = true;
	}
	//------------------------------------------------------------------------------------
	void QtOgreWidget::mousePressEvent(QMouseEvent *evt)
	{
		/*  if(!OgitorsRoot::getSingletonPtr()->IsSceneLoaded())
		return;*/

		if(!hasFocus())
			setFocus();

		Ogre::Vector2 pos1(evt->x(), evt->y());

		/*if(evt->button() == Qt::LeftButton)
		OgitorsRoot::getSingletonPtr()->OnMouseLeftDown(pos1, getMouseButtons(evt->buttons(), evt->button()));
		else if(evt->button() == Qt::RightButton)
		OgitorsRoot::getSingletonPtr()->OnMouseRightDown(pos1, getMouseButtons(evt->buttons(), evt->button()));
		else if(evt->button() == Qt::MidButton)
		OgitorsRoot::getSingletonPtr()->OnMouseMiddleDown(pos1, getMouseButtons(evt->buttons(), evt->button()));*/

		OgreWidgetMouseMovedSincePress = false;
	}
	//------------------------------------------------------------------------------------
	void QtOgreWidget::mouseReleaseEvent(QMouseEvent *evt)
	{
		/* if(!OgitorsRoot::getSingletonPtr()->IsSceneLoaded())
		return;*/

		if(!hasFocus())
			setFocus();

		Ogre::Vector2 pos(evt->x(), evt->y());

		/* if(evt->button() == Qt::LeftButton)
		OgitorsRoot::getSingletonPtr()->OnMouseLeftUp(pos, getMouseButtons(evt->buttons(), evt->button()));
		else if(evt->button() == Qt::RightButton)
		OgitorsRoot::getSingletonPtr()->OnMouseRightUp(pos, getMouseButtons(evt->buttons(), evt->button()));
		else if(evt->button() == Qt::MidButton)
		OgitorsRoot::getSingletonPtr()->OnMouseMiddleUp(pos, getMouseButtons(evt->buttons(), evt->button()));
		*/
		if(!OgreWidgetMouseMovedSincePress && evt->button() == Qt::RightButton)
		{
			setContextMenuPolicy( Qt::PreventContextMenu );
			showObjectMenu();
		}
	}
	//------------------------------------------------------------------------------------
	void QtOgreWidget::leaveEvent(QEvent *evt)
	{
		/* if(!OgitorsRoot::getSingletonPtr()->IsSceneLoaded())
		return;*/

		Ogre::Vector2 pos(-1, -1);

		//OgitorsRoot::getSingletonPtr()->OnMouseLeave(pos, 0);
		OgreWidgetMouseMovedSincePress = true;
	}
	//------------------------------------------------------------------------------------
	void QtOgreWidget::wheelEvent(QWheelEvent *evt)
	{
		/* if(!OgitorsRoot::getSingletonPtr()->IsSceneLoaded())
		return;*/

		if(!hasFocus())
			setFocus();

		Ogre::Vector2 pos(evt->x(), evt->y());

		//OgitorsRoot::getSingletonPtr()->OnMouseWheel(pos, evt->delta(), getMouseButtons(evt->buttons(), Qt::NoButton));
	}

	//------------------------------------------------------------------------------------
	QPaintEngine* QtOgreWidget::paintEngine() const
	{
		// We don't want another paint engine to get in the way for our Ogre based paint engine.
		// So we return nothing.
		return NULL;
	}

	//------------------------------------------------------------------------------------
	void QtOgreWidget::_processKeyActions(void)
	{
		QTime time = QTime::currentTime();
		unsigned int curtime = time.hour() * 60 + time.minute();
		curtime = (curtime * 60) + time.second();
		curtime = (curtime * 1000) + time.msec();

		unsigned int iTimeDiff = curtime - mLastKeyEventTime;
		mLastKeyEventTime = curtime;
	}

	//------------------------------------------------------------------------------------
	float QtOgreWidget::caculateFPS(float time)
	{
		mFrameCounter++;
		mTotalFrameTime += time;
		mFrameRate = (float)mFrameCounter / (float)mTotalFrameTime ;
		if(mTotalFrameTime > 1.0f)
		{
			mTotalFrameTime = 0;
			mFrameCounter = 0;
		}
		return mFrameRate;
	}
	//----------------------------------------------------------------------------------
	void QtOgreWidget::processMaterialName(Ogre::Mesh *mesh, Ogre::String *name)
	{
		//Ogre::NameValuePairList *modelMaterialMap = OgitorsRoot::getSingletonPtr()->GetModelMaterialMap();
		//modelMaterialMap->insert(Ogre::NameValuePairList::value_type(mesh->getName(), name->c_str()));
	}
	//-------------------------------------------------------------------------------------------
	void QtOgreWidget::dragEnterEvent(QDragEnterEvent *evt)
	{
		void *source = (void*)(evt->source());

		/* if(OgitorsRoot::getSingletonPtr()->OnDragEnter(source))
		{
		evt->setDropAction(Qt::IgnoreAction);
		evt->acceptProposedAction();

		setFocus();
		grabKeyboard();
		}*/
	}
	//-------------------------------------------------------------------------------------------
	void QtOgreWidget::dragLeaveEvent(QDragLeaveEvent *evt)
	{
		//OgitorsRoot::getSingletonPtr()->OnDragLeave();
		releaseKeyboard();
	}
	//-------------------------------------------------------------------------------------------
	void QtOgreWidget::dragMoveEvent(QDragMoveEvent *evt)
	{
		void *source = (void*)(evt->source());
		unsigned int modifier = evt->keyboardModifiers();

		//evt->setAccepted(OgitorsRoot::getSingletonPtr()->OnDragMove(source, modifier, evt->pos().x(), evt->pos().y()));
	}
	//-------------------------------------------------------------------------------------------
	void QtOgreWidget::dropEvent(QDropEvent *evt)
	{
		void *source = (void*)(evt->source());

		evt->setDropAction(Qt::IgnoreAction);

		//OgitorsRoot::getSingletonPtr()->OnDragDropped(source, evt->pos().x(), evt->pos().y());

		releaseKeyboard();
	}
	//-------------------------------------------------------------------------------------------
	void QtOgreWidget::showObjectMenu()
	{
		/* CBaseEditor *e = 0;

		if(!OgitorsRoot::getSingletonPtr()->GetSelection()->isEmpty())
		e = OgitorsRoot::getSingletonPtr()->GetSelection()->getAsSingle();*/

		//QMenu* contextMenu = new QMenu(this);

		//if(e != 0)
		//{
		//    contextMenu->setTitle(tr("Object Menu : ") + QString(e->getName().c_str()));
		//    QSignalMapper *signalMapper = 0;
		//    QSignalMapper *pasteSignalMapper = 0;

		//    contextMenu->addAction(mOgitorMainWindow->actEditCopy);
		//    contextMenu->addAction(mOgitorMainWindow->actEditCut);
		//    contextMenu->addAction(mOgitorMainWindow->actEditDelete);
		//    contextMenu->addAction(mOgitorMainWindow->actEditRename);
		//    contextMenu->addSeparator();
		//    contextMenu->addAction(mOgitorMainWindow->actEditCopyToTemplate);
		//    contextMenu->addAction(mOgitorMainWindow->actEditCopyToTemplateWithChildren);

		//    //UTFStringVector menuList;
		//    if(e->getObjectContextMenu(menuList))
		//    {
		//        mOgitorMainWindow->parseAndAppendContextMenuList(contextMenu, menuList, this);
		//    }
		//    contextMenu->exec(QCursor::pos());
		//    delete signalMapper;
		//}
		//else
		//{
		//    contextMenu->addAction(mOgitorMainWindow->menuFile->menuAction());
		//    contextMenu->addAction(mOgitorMainWindow->menuEdit->menuAction());
		//    contextMenu->addAction(mOgitorMainWindow->menuView->menuAction());
		//    contextMenu->addAction(mOgitorMainWindow->menuCamera->menuAction());
		//    contextMenu->addAction(mOgitorMainWindow->menuTools->menuAction());
		//    contextMenu->addAction(mOgitorMainWindow->menuTerrainTools->menuAction());
		//    contextMenu->exec(QCursor::pos());
		//}

		//delete contextMenu;
	}
	//-------------------------------------------------------------------------------------------
	void QtOgreWidget::contextMenu(int id)
	{
		/*if(!OgitorsRoot::getSingletonPtr()->GetSelection()->isEmpty())
		OgitorsRoot::getSingletonPtr()->GetSelection()->getAsSingle()->onObjectContextMenu(id);*/
	}
	//----------------------------------------------------------------------------------------
}