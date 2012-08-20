/// @file MainWindow.h
/// @brief  mainwindow of sangis app
//			original codes from proted from ogitor
/// @author luoshasha
/// @version 1.0
/// @date 2012-08-09
#ifndef MAINWINDOW_HXX
#define MAINWINDOW_HXX

#include <QtGui/QtGui>
#include <QtCore/QtCore>

#include "OgreLog.h"

QT_BEGIN_NAMESPACE
class QProcess;
QT_END_NAMESPACE

namespace sandgis
{
	class QtOgreWidget;
	///@brief Mainwindow of sandgis application
	///       original code is ported from Ogitor, thanks very much
	///@seealso Ogitor website: www.ogitor.org
	///@autor luoshasha
	class WorkspaceLayout;
	class RenderView;
	class QtMainWindow : public QMainWindow, Ogre::LogListener
	{
		Q_OBJECT
	private:
		//-----------------------------------------
		//menu toolbars statubar
		QMenuBar* menubar_;
		QStatusBar* statusbar_;
		QList<QToolBar*> toolbars_;

		//----------------------------------------------------------------------
		//  menus
		//----------------------------------------------------------------------
		QMenu*    menuCompactMenuBar;
		QMenu*    menuFile;
		QMenu*    menuImport;
		QMenu*    menuExport;
		QMenu*    menuTools;
		QMenu*    menuEdit;
		QMenu*    menuCamera;
		QMenu*    menuView;
		QMenu*    menuLog;
		QMenu*    menuPlugins;
		QMenu*    menuCamTools;
		QMenu*    menuHelp;
		QMenu*    menuCamPolyMode;
		QMenu*    menuTerrainTools;
		QMenu*    menuCameraPositionMain;
		QMenu*    menuCameraPositions[10];
		QMenu*    menuRecentFiles;
		QMenu*    menuDefineSelectionList;
		QMenu*    menuSelectSelectionList;

		//----------------------------------------------------------------------
		//  status bar widgets
		//----------------------------------------------------------------------
		QSlider*  mCameraSpeedSlider;
		QSlider*  mFPSSlider;
		QLabel*   mFPSSliderLabel;
#if OGRE_MEMORY_TRACKER
		QLabel*   mMemoryUsageLabel;
#endif
		QLabel*   mSelectedObjectsCountLabel;
		QLabel*   mTriangleCountLabel;
		QLabel*   mFPSLabel;
		QLabel*   mCamPosLabel;
		QToolBar* mCamPosToolBar;
		QToolBar* mStatusShowHideToolBar;
		QToolBar* mStatusViewToolBar;


		//----------------------------------------------------------------------
		//  actions
		//----------------------------------------------------------------------
		QAction* actTogglePolymode;
	public:
		QtMainWindow(QString args = "", QWidget *parent = 0);
		~QtMainWindow();

		//gets menu and toolbars etc.
		QMenuBar* topMenu(void) const
		{
			return menubar_;
		}

		QStatusBar* statusBar(void) const
		{
			return statusbar_;
		}

		const QList<QToolBar*>& toolBars(void) const
		{
			return toolbars_;
		}

		const QList<RenderView*>& renderViews(void) const
		{
			return render_views_;
		}

		void readSettings(const char* = "");
		void writeSettings(const char* = "");

		void setApplicationObject(QObject *obj);

		bool getAppActive() const { return app_active_;}

		///@brief gets win handle
		unsigned handle(void) const
		{
			return (unsigned)this->winId();
		}
		///@brief display fps on status bar
		///@param visible trangle numbers
		void displayTriangleNum(int triangle_num);

		///@brief display fps
		///@param fps value
		void displayFPS(float fps);

		///@brief display camera position
		void displayCameraPos(float x, float y, float z);

		WorkspaceLayout* workspaceLayout(void) const
		{
			return workspace_layout_;
		}
	private:
		// -------------------------------------------------------------------------------------
		/// @brief  createActions <++>create actions
		// =------------------------------------------------------------------------------------
		void updateLog(QListWidgetItem* item);
	
		void showSubWindows();
		void hideSubWindows();
		void retranslateUi();

		void closeEvent(QCloseEvent *event);
		bool eventFilter(QObject *obj,  QEvent *event);
		void initHiddenRenderWindow();

		void createDockWidgets(void);
		void createRenderWindow(void);
		void createToolbars(void);
		void createActions(void);
		void createMenus(void);
		void createStatusBar(void);
		void updateActions(void);
		void setupLog(void);
		void updateRenderViews(void);

		void messageLogged (const Ogre::String &message, 
			Ogre::LogMessageLevel lml,
			bool maskDebug, 
			const Ogre::String &logName, 
			bool &skipThisMessage);

		public Q_SLOTS:
			void timerLoop();
			void fpsValueChanged ( int value );

	private:
		QTimer                          *timer_;
		unsigned int                   last_time_;
		QString                         args_file_;
		bool                           subwindow_visible_;
		int                            target_render_count_;
		QObject                         *app_object_;
		bool                           app_active_;
		bool has_file_args_;
		std::string last_loaded_scene_;
		WorkspaceLayout* workspace_layout_;
		QList<RenderView*> render_views_;
	};

}
#endif // MAINWINDOW_HXX

//-----------------------------------------------------------------------------------------
