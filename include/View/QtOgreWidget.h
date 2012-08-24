/// @file ogrewidget.h
/// @brief  qt render window
//		original code is ported from Ogitor
/// @author luoshasha
/// @version 1.0
/// @date 2012-08-09
#ifndef __OGREWIDGET_HXX__
#define __OGREWIDGET_HXX__

#include <QTimer>
#include <QApplication>
#include <QWidget>
#if defined(Q_WS_X11)
#include <QX11Info>
#endif

//ogre 
#include "OgrePrerequisites.h"
#include "view/RenderView.h"

namespace sandgis
{
	// -------------------------------------------------------------------------------------
	/// @brief  overlay widget use to display message 
	//when scene is not rendering into render window
	// =------------------------------------------------------------------------------------
	class OverlayWidget : public QWidget
	{
		Q_OBJECT;

	public:
		OverlayWidget(QWidget *parent=0) : QWidget(parent) 
		{
			msgstr = QApplication::translate("OgreWidget","Initializing OGRE...");
			setContextMenuPolicy( Qt::PreventContextMenu );
		};

		virtual ~OverlayWidget() {};

		void setMessageString(QString msg)
		{
			if(msgstr != msg)
			{
				msgstr = msg;
				this->repaint();
			}
		};

	protected:
		QString msgstr;

		void paintEvent(QPaintEvent* evt);
	};

	class QtOgreWidget;


	class MapPresenter;
	// -------------------------------------------------------------------------------------
	///@brief ogre render window combined with qt!
	///       original code is ported from Ogitor, thanks very much
	///@seealso Ogitor website: www.ogitor.org
	///@autor luoshasha
	// =------------------------------------------------------------------------------------
	class QtOgreWidget : public QWidget, public RenderView
	{
		Q_OBJECT;
	public:
		QtOgreWidget(QWidget *parent=0, Qt::WindowFlags f=0);
		virtual ~QtOgreWidget();

		///Override QWidget::paintEngine to return NULL
		///Turn off QTs paint engine for the Ogre widget.
		QPaintEngine* paintEngine() const;

		void showCursorEx(bool bShow)
		{
			if(mCursorHidden == bShow)
			{
				if(!bShow)
					setCursor(Qt::BlankCursor);
				else
					setCursor(Qt::ArrowCursor);

				mCursorHidden = !bShow;
			}
		};

		void setTimerInterval(int value);

		void setOverlayMessage(QString msg)
		{
			mOverlayWidget->setMessageString(msg);
			QWidget::update();
		};

		bool isSizing()
		{
			return mScreenResize;
		}

		///@brief callback method for internal using
		bool _notifyFrameStarted(const Ogre::FrameEvent& evt);

		///@brief proccess key action invorked by mainwindow
		///       don't call this method directly
		void _processKeyActions(void);
		//----------------------------------------------------------------------
		//  irenderview interface
		//----------------------------------------------------------------------
		Ogre::RenderWindow* renderWindow(void) const 
		{
			return mRenderWindow;
		}

		// -------------------------------------------------------------------------------------
		/// @brief  sceneLoaded callback method when scene loaded
		// =------------------------------------------------------------------------------------
		void sceneLoaded(void);

		// -------------------------------------------------------------------------------------
		/// @brief  sceneDestroyed notify method called when scene destroyed
		// =------------------------------------------------------------------------------------
		void sceneDestroyed(void);

		//----------------
		void stopRendering(bool stop) 
		{
			mRenderStop = stop;
		};
	public Q_SLOTS:
		void timerLoop();
		void contextMenu(int id);

	private:
		bool  mOgreInitialised;
		bool  mRenderStop;
		float mFrameRate;
		bool  is_scene_loaded_;
		bool disposed_;
		std::unique_ptr<MapPresenter> map_presenter_;
		Ogre::RenderWindow *mRenderWindow;

	protected:
		bool           mSwitchingScene;
		unsigned int  mLastKeyEventTime;
		volatile bool mScreenResize;
		int           mFrameCounter;
		double        mTotalFrameTime;
		bool          mCursorHidden;
		OverlayWidget *mOverlayWidget;

		// -------------------------------------------------------------------------------------
		/// @brief  initializeOGRE intialise ogre render window
		// =------------------------------------------------------------------------------------
		void initializeRenderWindow();

		///---------------------------------------------------
		// overrided qwidget's input event
		///---------------------------------------------------
		void resizeEvent(QResizeEvent* evt);
		void paintEvent(QPaintEvent* evt);
		void focusOutEvent(QFocusEvent *evt);
		void focusInEvent(QFocusEvent *evt);
		void keyPressEvent(QKeyEvent *evt);
		void keyReleaseEvent(QKeyEvent *evt);
		void mouseMoveEvent(QMouseEvent *evt);
		void mousePressEvent(QMouseEvent *evt);
		void mouseReleaseEvent(QMouseEvent *evt);
		void leaveEvent(QEvent *evt);
		void wheelEvent(QWheelEvent *evt);
		void dragEnterEvent(QDragEnterEvent *evt);
		void dragLeaveEvent(QDragLeaveEvent *evt);
		void dragMoveEvent(QDragMoveEvent *evt);
		void dropEvent(QDropEvent *evt);

		float caculateFPS(float time);
		/// show objects context menu
		void showObjectMenu();
	};

}
//-----------------------------------------------------------------------------------------
#endif // __OGREWIDGET_HXX__
//-----------------------------------------------------------------------------------------
