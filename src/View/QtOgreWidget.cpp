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
#include "presenter/mappresenter.h"
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
	QtOgreWidget::QtOgreWidget(const std::string& name, QWidget *parent,
		Qt::WindowFlags f): RenderView(name), QWidget( parent,  f),
		mRenderWindow(0),
		map_presenter_(new MapPresenter(this)),
		is_scene_loaded_(false),
		mOgreInitialised(false),
		mLastKeyEventTime(0),
		mRenderStop(false), 
		mScreenResize(false), 
		mCursorHidden(false),
		disposed_(false)
	{
		mFrameCounter = 0;
		mTotalFrameTime = 0;
		mSwitchingScene = false;

		setAcceptDrops(true);
		setContextMenuPolicy( Qt::PreventContextMenu );

		setFocusPolicy(Qt::WheelFocus);
		setMouseTracking(true);
		//setAttribute(Qt::WA_NoBackground);
		//These attributes are the same as those use in a QGLWidget
		setAttribute(Qt::WA_PaintOnScreen);
		setAttribute(Qt::WA_NoSystemBackground);

		mOverlayWidget = new OverlayWidget(this);
		QVBoxLayout *layout = new QVBoxLayout();
		layout->setMargin(0);
		layout->addWidget(mOverlayWidget);
		setLayout(layout);
	}
	//----------------------------------------------------------------------------------------
	QtOgreWidget::~QtOgreWidget()
	{
		if(mOgreInitialised)
		{
			Ogre::Root* root = WorkspaceRoot::instance()->ogreContext()->root();
			root->destroyRenderTarget(mRenderWindow);
		}   
		destroy();
		disposed_ = true;
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
#endif
#if defined(Q_WS_X11)
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
		mOgreInitialised = true;

		//
		map_presenter_->initializeScene(QRectF(0, 3000, 3000, 3000), Ogre::ColourValue(0.5, 0.4, 1),
			Ogre::Vector3(1500,1500, 200),
			Ogre::Vector3(1500, 1500, 0));
	}

	//------------------------------------------------------------------------------------
	void QtOgreWidget::timerLoop()
	{
		if(WorkspaceRoot::instance()->workspace()->isMinimized())
			return;

		update();
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
		if(mOgreInitialised && is_scene_loaded_ && !mRenderStop)
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
			if(mOgreInitialised && !is_scene_loaded_)
			{
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

	void QtOgreWidget::sceneLoaded(void)
	{
		mOverlayWidget->hide();
		is_scene_loaded_ = true;
	}

	void QtOgreWidget::refresh(void)
	{
		this->update();
	}

	//------------------------------------------------------------------------------------
	void QtOgreWidget::focusInEvent(QFocusEvent *evt)
	{
		if (!is_scene_loaded_)
			return;

		evt->setAccepted(true);
	}

	//------------------------------------------------------------------------------------
	void QtOgreWidget::focusOutEvent(QFocusEvent *evt)
	{
		if (!is_scene_loaded_)
			return;

		map_presenter_->_onViewFocusOut();

		evt->setAccepted(true);
	}
	//------------------------------------------------------------------------------------
	void QtOgreWidget::keyPressEvent(QKeyEvent *evt)
	{
		if(evt->isAutoRepeat())
			return;

		if (!is_scene_loaded_)
			return;

		unsigned int key = evt->key();
		if(key > 255)
			key = (key & 0xFFF) + 0xFF;

		map_presenter_->_onKeyDown(evt->key());
	}
	//------------------------------------------------------------------------------------
	void QtOgreWidget::keyReleaseEvent(QKeyEvent *evt)
	{
		if(evt->isAutoRepeat())
			return;

		if (!is_scene_loaded_)
			return;

		unsigned int key = evt->key();
		if(key > 255)
			key = (key & 0xFFF) + 0xFF;

		map_presenter_->_onKeyDown((int)key);
	}

	//------------------------------------------------------------------------------------
	bool OgreWidgetMouseMovedSincePress = false;
	void QtOgreWidget::mouseMoveEvent(QMouseEvent *evt)
	{
		if (!is_scene_loaded_)
			return;

		if(!hasFocus())
			setFocus();

		Ogre::Vector2 pos(evt->x(), evt->y());

		map_presenter_->_onMouseMove(evt->buttons(), evt->x(), evt->y());

		OgreWidgetMouseMovedSincePress = true;
	}

	//------------------------------------------------------------------------------------
	void QtOgreWidget::mousePressEvent(QMouseEvent *evt)
	{
		if (!is_scene_loaded_)
			return;

		if(!hasFocus())
			setFocus();

		map_presenter_->_onMouseDown(evt->button(), evt->x(), evt->y());

		OgreWidgetMouseMovedSincePress = false;
	}

	//------------------------------------------------------------------------------------
	void QtOgreWidget::mouseReleaseEvent(QMouseEvent *evt)
	{
		if (!is_scene_loaded_)
			return;

		if(!hasFocus())
			setFocus();

		map_presenter_->_onMouseUp(evt->button(), evt->x(), evt->y());

		if(!OgreWidgetMouseMovedSincePress && evt->button() == Qt::RightButton)
		{
			setContextMenuPolicy( Qt::PreventContextMenu );
			showObjectMenu();
		}
	}

	//------------------------------------------------------------------------------------
	void QtOgreWidget::leaveEvent(QEvent *evt)
	{
		if (!is_scene_loaded_)
			return;

		OgreWidgetMouseMovedSincePress = true;
	}

	//------------------------------------------------------------------------------------
	void QtOgreWidget::wheelEvent(QWheelEvent *evt)
	{
		if (!is_scene_loaded_)
			return;

		if(!hasFocus())
			setFocus();

		map_presenter_->_onMouseWheel(evt->delta());
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
		releaseKeyboard();
	}
	//-------------------------------------------------------------------------------------------
	void QtOgreWidget::dragMoveEvent(QDragMoveEvent *evt)
	{
		void *source = (void*)(evt->source());
		unsigned int modifier = evt->keyboardModifiers();

	}
	//-------------------------------------------------------------------------------------------
	void QtOgreWidget::dropEvent(QDropEvent *evt)
	{
		void *source = (void*)(evt->source());

		evt->setDropAction(Qt::IgnoreAction);

		releaseKeyboard();
	}

	//-------------------------------------------------------------------------------------------
	void QtOgreWidget::showObjectMenu()
	{
	
	}
	//-------------------------------------------------------------------------------------------
	void QtOgreWidget::contextMenu(int id)
	{
	}
	//----------------------------------------------------------------------------------------
}
