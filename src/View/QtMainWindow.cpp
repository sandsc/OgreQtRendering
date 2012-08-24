#include "view/qtmainwindow.h"
#include "view/qtogrewidget.h"
#include "view/workspacelayout.h"
#include "view/QtMainRenderView.h"
#include "presenter/WorkspaceRoot.h"
#include "presenter/OgreContext.h"

#include "Ogre.h"

namespace sandgis
{
	//------------------------------------------------------------------------------
	QtMainWindow::QtMainWindow(QString args, QWidget *parent)
		: QMainWindow(parent),
		menubar_(0),
		statusbar_(0),
		last_time_(0), 
		app_object_(0),
		args_file_(args),
		has_file_args_(false),
		last_loaded_scene_(""), 
		subwindow_visible_(true),
		workspace_layout_(new WorkspaceLayout())
	{
		target_render_count_ = 30;
		setMinimumSize(400,300);
		if(!args_file_.isEmpty())
		{
			has_file_args_ = true;
		}
		QIcon icon;
		icon.addPixmap(QPixmap(":/icons/mainwindow.png"), QIcon::Normal, QIcon::Off);
		setWindowIcon(icon);
		if(objectName().isEmpty())
			setObjectName(QString::fromUtf8("this"));
		resize(1024, 768);
		setDockNestingEnabled(true);

		createDockWidgets();
		setupLog();
		createMenus();
		createStatusBar();
		createToolbars();

		retranslateUi();

		createRenderWindow();

		timer_ = new QTimer(this);
		timer_->setInterval(0);
		connect(timer_, SIGNAL(timeout()), this, SLOT(timerLoop()));
		timer_->start();

		initHiddenRenderWindow();

		readSettings();
		//Looks better on OSX to always be shown
#if OGRE_PLATFORM != OGRE_PLATFORM_APPLE
		hideSubWindows();
#endif
	}
	//------------------------------------------------------------------------------
	QtMainWindow::~QtMainWindow()
	{
		timer_->stop();
		delete timer_;
		delete workspace_layout_;
	}
	//------------------------------------------------------------------------------
	void QtMainWindow::setApplicationObject(QObject *obj)
	{
		app_object_ = obj;
		app_active_ = true;
		obj->installEventFilter(this);
	}

	void QtMainWindow::createRenderWindow(void)
	{
		render_views_.append(new QtMainRenderView());
	}

	//------------------------------------------------------------------------------
	void QtMainWindow::initHiddenRenderWindow()
	{
#if defined(Q_WS_MAC) || defined(Q_WS_WIN)
		Ogre::RenderWindow* pPrimary = WorkspaceRoot::instance()->ogreContext()->initRenderWindowFromExternal(
			"QtPrimaryHiddenWindow",
			(unsigned)this->winId(),
			1,
			1, 0, true);
#else
		const QX11Info info = this->x11Info();
		Ogre::RenderWindow* pPrimary = WorkspaceRoot::instance()->ogreContext()->initRenderWindowFromExternal("QtPrimaryHiddenWindow",
			(unsigned)this->winId(), 
			1, 
			1, 
			&info, true);
#endif
		pPrimary->setVisible(false);
		pPrimary->setAutoUpdated(false);

		Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
		//Load resources
		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	}
	//------------------------------------------------------------------------------------
	void QtMainWindow::showSubWindows()
	{
#ifdef OPT_SHOW_HIDE_WINDOWS

		if(subwindow_visible_)
			return;

		mEditorTab->setVisible(true);

		for(unsigned int i = 0;i < mSubWindowStateSave.size();i++)
		{
			mSubWindowStateSave[i]->setVisible(true);
		}
		mSubWindowStateSave.clear();

		menubar_->insertAction(menuHelp->menuAction(), menuEdit->menuAction());
		menubar_->insertAction(menuHelp->menuAction(), menuView->menuAction());
		menubar_->insertAction(menuHelp->menuAction(), menuLog->menuAction());
		menubar_->insertAction(menuHelp->menuAction(), menuCamera->menuAction());
		menubar_->insertAction(menuHelp->menuAction(), menuTools->menuAction());
		menubar_->insertAction(menuHelp->menuAction(), menuTerrainTools->menuAction());

		actFullScreen->setEnabled(true);
		actSuperFullScreen->setEnabled(true);

		mCamPosToolBar->setVisible(true);
		mStatusViewToolBar->setVisible(true);
		mStatusShowHideToolBar->setVisible(true);

		subwindow_visible_ = true;
#endif
	}
	//------------------------------------------------------------------------------
	void QtMainWindow::hideSubWindows()
	{
#ifdef OPT_SHOW_HIDE_WINDOWS

		if(!subwindow_visible_)
			return;

		mSubWindowStateSave.clear();

		const QObjectList& childwins = children();
		for(int i = 0;i < childwins.size();i++)
		{
			if(childwins[i]->isWidgetType())
			{
				QWidget *widget = static_cast<QWidget*>(childwins[i]);
				QString objName = widget->objectName();
				// Check if the widget is a dockwidget and if its not already defined as hidden
				// Qt defines widget attribute WA_WState_Hidden if a widget will be created but not displayed on screen
				// restoreLayout also restores this attribute, so we get the correct state,
				// testing this attribute instead of isVisible() since isVisible will always
				// incorrectly return false when the window is first created but not updated yet...
				if(objName.endsWith("DockWidget", Qt::CaseInsensitive) && !widget->testAttribute(Qt::WA_WState_Hidden))
				{
					widget->setVisible(false);
					mSubWindowStateSave.push_back(widget);
				}
			}
		}

		mEditorTab->setVisible(false);

		menubar_->removeAction(menuTools->menuAction());
		menubar_->removeAction(menuEdit->menuAction());
		menubar_->removeAction(menuCamera->menuAction());
		menubar_->removeAction(menuView->menuAction());
		menubar_->removeAction(menuLog->menuAction());
		menubar_->removeAction(menuTerrainTools->menuAction());

		actFullScreen->setEnabled(false);
		actSuperFullScreen->setEnabled(false);

		mCamPosToolBar->setVisible(false);
		mStatusViewToolBar->setVisible(false);
		mStatusShowHideToolBar->setVisible(false);

		subwindow_visible_ = false;
#endif
	}
	//------------------------------------------------------------------------------
	void QtMainWindow::readSettings(const char* filename)
	{
		//QSettings *settings;
		//bool invalid_win_rect = false;

		//if(filename.isEmpty())
		//{
		//	settings = new QSettings();
		//}
		//else
		//	settings = new QSettings(filename, QSettings::IniFormat);

		//settings->beginGroup("session");
		//restoreState(settings->value("Layout").toByteArray());
		//bool maximized = settings->value("MainWindowMaximized", false).toBool();
		//QRect rect = settings->value("MainWindowGeometry").toRect();
		//if(rect.isEmpty())
		//	invalid_win_rect = true;
		//settings->endGroup();

		///*   settings->beginGroup("messagefilters");
		//actLogShowWarnings->setChecked(settings->value("FilterWarnings", 1).toBool());
		//actLogShowErrors->setChecked(settings->value("FilterErrors", 1).toBool());
		//actLogShowInfo->setChecked(settings->value("FilterInfo", 1).toBool());
		//actLogShowDebug->setChecked(settings->value("FilterDebug", 1).toBool());
		//settings->endGroup();*/
		////toggleLogMessages();

		//if(maximized)
		//	setWindowState(Qt::WindowMaximized);
		//else
		//{
		//	move(rect.topLeft());
		//	resize(rect.size());
		//}

		//delete settings;

		//if(invalid_win_rect)
		//	return readSettings(":/layouts/initial.oglayout");
	}
	//------------------------------------------------------------------------------
	void QtMainWindow::writeSettings(const char* filename)
	{
		QSettings *settings;

		if(!filename)
			settings = new QSettings();
		else
			settings = new QSettings(filename, QSettings::IniFormat);

		settings->beginGroup("session");
		settings->setValue("MainWindowGeometry", geometry());

		bool maximized = isMaximized();
		settings->remove("Layout");
		settings->setValue("Layout", saveState());
		settings->setValue("MainWindowMaximized", maximized);
		settings->endGroup();
		//settings->beginGroup("messagefilters");
		//settings->setValue("FilterWarnings", actLogShowWarnings->isChecked());
		//settings->setValue("FilterErrors", actLogShowErrors->isChecked());
		//settings->setValue("FilterInfo", actLogShowInfo->isChecked());
		//settings->setValue("FilterDebug", actLogShowDebug->isChecked());
		//settings->endGroup();
		settings->setValue("preferences/lastLoadedScene", last_loaded_scene_.c_str());

		delete settings;
	}
	//------------------------------------------------------------------------------
	void QtMainWindow::closeEvent(QCloseEvent *event)
	{
		bool isdirty = false;
		if(!isdirty)
		{
			//Ogre::LogManager::getSingleton().getDefaultLog()->removeListener(this);

			//if(actFullScreen->isChecked())
			//{
			//    actFullScreen->setChecked(false);
			//    //toggleFullScreen();
			//}
			//else if(actSuperFullScreen->isChecked())
			//{
			//    actSuperFullScreen->setChecked(false);
			//    //toggleSuperFullScreen();
			//}

			showSubWindows();

			writeSettings();

			event->setAccepted(true);
		}
		else
		{
		    event->setAccepted(false);
		    return;
		}
	}
	//------------------------------------------------------------------------------
	void QtMainWindow::retranslateUi()
	{
		QString appTitle = "SandGis ";
		setWindowTitle(appTitle);
	}

	//------------------------------------------------------------------------------
	void QtMainWindow::createDockWidgets()
	{

	}
	
	//------------------------------------------------------------------------------
	void QtMainWindow::createToolbars()
	{
		QToolBar* file_toolbar = new QToolBar(tr("File"));
		file_toolbar->setObjectName("FileToolBar");
		file_toolbar->setIconSize(QSize(24,24));
		file_toolbar->setToolButtonStyle(Qt::ToolButtonIconOnly);

		//mFileToolBar->addAction(actNew);
		//mFileToolBar->addAction(actOpen);
		//mFileToolBar->addAction(actSave);
		//mFileToolBar->addAction(actSaveAs);
		//mFileToolBar->addAction(actClose);

		QToolBar* edit_toolbar = new QToolBar(tr("Edit"));
		edit_toolbar->setObjectName("EditToolBar");
		edit_toolbar->setIconSize(QSize(24,24));
		edit_toolbar->setToolButtonStyle(Qt::ToolButtonIconOnly);

		/*  mEditToolBar->addAction(actUndo);
		mEditToolBar->addAction(actRedo);
		mEditToolBar->addSeparator();
		mEditToolBar->addAction(actSceneOptions);
		mEditToolBar->addAction(actOpenPreferences);*/

		QToolBar* help_toolbar = new QToolBar(tr("Help"));
		help_toolbar->setObjectName("HelpToolBar");
		help_toolbar->setIconSize(QSize(24,24));
		help_toolbar->setToolButtonStyle(Qt::ToolButtonIconOnly);

		/* mHelpToolBar->addAction(actHelp);
		mHelpToolBar->addAction(actAbout);*/
		this->toolbars_.append(file_toolbar);
		this->toolbars_.append(edit_toolbar);
		this->toolbars_.append(help_toolbar);
	}

	//------------------------------------------------------------------------------
	void QtMainWindow::createMenus(void)
	{
		menubar_ = new QMenuBar(this);
		menubar_->setObjectName(QString::fromUtf8("menubar_"));
		menubar_->setGeometry(QRect(0, 0, 616, 22));

		menuFile = new QMenu(tr("File"), menubar_);
		menuFile->setObjectName(QString::fromUtf8("menuFile"));
		menubar_->addAction(menuFile->menuAction());
		//menuFile->addAction(actNew);
		//menuFile->addAction(actOpen);
		//menuFile->addAction(actSave);
		//menuFile->addAction(actSaveAs);
		//menuFile->addAction(actClose);
		menuFile->addSeparator();
		menuImport = menuFile->addMenu(tr("Import"));
		menuImport->setIcon(QIcon(":/icons/import.svg"));
		menuExport = menuFile->addMenu(tr("Export"));
		menuExport->setIcon(QIcon(":/icons/export.svg"));

		Ogre::StringVector theList;
		QSignalMapper *importMapper = new QSignalMapper(this);
		QSignalMapper *exportMapper = new QSignalMapper(this);

		//OgitorsRoot::getSingletonPtr()->GetImportSerializerList(theList);

		for(unsigned int i = 0;i < theList.size();i++)
		{
			QAction *item = menuImport->addAction(QString(theList[i].c_str()));
			connect(item, SIGNAL(triggered()), importMapper, SLOT(map()));
			importMapper->setMapping(item, QString(theList[i].c_str()));
		}

		//OgitorsRoot::getSingletonPtr()->GetExportSerializerList(theList);

		for(unsigned int i = 0;i < theList.size();i++)
		{
			QAction *item = menuExport->addAction(QString(theList[i].c_str()));
			connect(item, SIGNAL(triggered()), exportMapper, SLOT(map()));
			exportMapper->setMapping(item, QString(theList[i].c_str()));
		}

		connect(importMapper, SIGNAL(mapped( const QString & )), this, SLOT(importSerializer(  const QString & )));
		connect(exportMapper, SIGNAL(mapped( const QString & )), this, SLOT(exportSerializer(  const QString & )));

		menuRecentFiles = new QMenu(tr("Recent Files"), menubar_);
		menuRecentFiles->setObjectName(QString::fromUtf8("menuRecentFiles"));
		menuRecentFiles->setIcon(QIcon(":/icons/files.svg"));
		menuRecentFiles->setEnabled(false);

		//updateRecentFiles();

		menuFile->addSeparator();
		menuFile->addAction(menuRecentFiles->menuAction());
		menuFile->addSeparator();
		//menuFile->addAction(actExit);

		menuEdit = new QMenu(tr("Edit"), menubar_);
		menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
		menubar_->addAction(menuEdit->menuAction());
		/*menuEdit->addAction(actUndo);
		menuEdit->addAction(actRedo);*/
		menuEdit->addSeparator();
		/*menuEdit->addAction(actEditCopy);
		menuEdit->addAction(actEditCut);
		menuEdit->addAction(actEditPaste);
		menuEdit->addAction(actEditDelete);
		menuEdit->addAction(actEditRename);
		menuEdit->addAction(actEditCopyToTemplate);
		menuEdit->addAction(actEditCopyToTemplateWithChildren);
		menuEdit->addSeparator();
		menuEdit->addAction(actDecreaseGizmoScale);
		menuEdit->addAction(actIncreaseGizmoScale);
		menuEdit->addSeparator();
		menuEdit->addAction(actSceneOptions);
		menuEdit->addAction(actOpenPreferences);*/

		menuTools = new QMenu(tr("Tools"), menubar_);
		menuTools->setObjectName(QString::fromUtf8("menuTools"));
		menubar_->addAction(menuTools->menuAction());
	/*	menuTools->addAction(actSelect);
		menuTools->addAction(actMove);
		menuTools->addAction(actRotate);
		menuTools->addAction(actScale);*/
		menuDefineSelectionList = menuTools->addMenu(tr("Define Selection List"));
		menuSelectSelectionList = menuTools->addMenu(tr("Activate Selection List"));
		//for(int sa = 0;sa < 10;sa++)
		//{
		//	menuDefineSelectionList->addAction(mSelectActions[sa]);
		//	menuSelectSelectionList->addAction(mSelectActions[sa + 10]);
		//}

		menuView = new QMenu(tr("View"),menubar_);
		menuView->setObjectName(QString::fromUtf8("menuView"));
		menubar_->addAction(menuView->menuAction());
		/*menuView->addAction(actToggleExplorer);
		menuView->addAction(actToggleProjectFiles);
		menuView->addAction(actToggleLayer);
		menuView->addAction(actToggleProperties);
		menuView->addAction(actToggleTools);
		menuView->addAction(actToggleResources);
		menuView->addAction(actToggleLog);
		menuView->addSeparator();
		menuView->addAction(actToggleGrid);
		menuView->addAction(actToggleToolBar);
		menuView->addAction(actFullScreen);
		menuView->addAction(actSuperFullScreen);
		menuView->addSeparator();
		menuView->addAction(actLoadLayout);
		menuView->addAction(actSaveLayout);
		menuView->addSeparator();
		menuView->addAction(actHideMenuBar);*/

		menuLog = new QMenu(tr("Messages"),menubar_);
		menuLog->setObjectName(QString::fromUtf8("menuLog"));
		menubar_->addAction(menuLog->menuAction());
	/*	menuLog->addAction(actSearchLog);
		menuLog->addAction(actClearLog);
		menuLog->addAction(actLogShowErrors);
		menuLog->addAction(actLogShowWarnings);
		menuLog->addAction(actLogShowInfo);
		menuLog->addAction(actLogShowDebug);
*/
	/*	mLogWidget->addAction(actSearchLog);
		mLogWidget->addAction(actClearLog);
		mLogWidget->addAction(actLogShowErrors);
		mLogWidget->addAction(actLogShowWarnings);
		mLogWidget->addAction(actLogShowInfo);
		mLogWidget->addAction(actLogShowDebug);*/

		menuCamera = new QMenu(tr("Camera"), menubar_);
		menuCamera->setObjectName(QString::fromUtf8("menuCamera"));
		menubar_->addAction(menuCamera->menuAction());
		//menuCamera->addAction(actCamSave);
		menuView->addSeparator();
		//menuCamera->addAction(actCamSpeedPlus);
		//menuCamera->addAction(actCamSpeedMinus);

		menuTerrainTools = new QMenu(tr("Terrain Tools"), menubar_);
		menuTerrainTools->setObjectName(QString::fromUtf8("menuTerrainTools"));
		menubar_->addAction(menuTerrainTools->menuAction());
		/*menuTerrainTools->addAction(actDeform);
		menuTerrainTools->addAction(actSmooth);
		menuTerrainTools->addAction(actSplat);
		menuTerrainTools->addAction(actPaint);
		menuTerrainTools->addAction(actReLight);*/

		menuHelp = new QMenu(tr("Help"), menubar_);
		menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
		menubar_->addAction(menuHelp->menuAction());
		/*menuHelp->addAction(actHelp);
		menuHelp->addAction(actAbout);*/
	}

	//------------------------------------------------------------------------------
	void QtMainWindow::createStatusBar()
	{
		statusbar_ = new QStatusBar(this);
		statusbar_->setObjectName(QString::fromUtf8("statusbar"));

		mFPSLabel = new QLabel(tr("FPS: "));
		mFPSLabel->setMinimumWidth(87);
		//
		mFPSSlider = new QSlider(Qt::Horizontal);
		mFPSSlider->setRange(0, 19);
		mFPSSlider->setTickInterval(1);
		mFPSSlider->setTickPosition(QSlider::TicksBelow);
		mFPSSlider->setMaximumWidth(100);
		mFPSSliderLabel = new QLabel(tr("FPS (30)"));
		mFPSSlider->setSliderPosition(5);

		mSelectedObjectsCountLabel = new QLabel();
		mSelectedObjectsCountLabel->setMinimumWidth(150);
		mTriangleCountLabel = new QLabel(tr("Triangles visible: %1").arg(0));
		mTriangleCountLabel->setMinimumWidth(120);
		mCamPosLabel = new QLabel(tr("Camera Position:"));
		mCamPosLabel->setMinimumWidth(300);
		mCamPosToolBar = new QToolBar();
		mCamPosToolBar->setIconSize(QSize(16, 16));
		mCamPosToolBar->addWidget(mSelectedObjectsCountLabel);
		mCamPosToolBar->addSeparator();
#if OGRE_MEMORY_TRACKER
		mMemoryUsageLabel = new QLabel("MU : 0000MB");
		mMemoryUsageLabel->setMinimumWidth(87);
		mCamPosToolBar->addWidget(mMemoryUsageLabel);
		mCamPosToolBar->addSeparator();
#endif
		mCamPosToolBar->addWidget(mTriangleCountLabel);
		mCamPosToolBar->addSeparator();
		mCamPosToolBar->addWidget(mCamPosLabel);
		mCamPosToolBar->addSeparator();
		mCamPosToolBar->addWidget(mFPSLabel);
		mCamPosToolBar->addSeparator();
		mCamPosToolBar->addWidget(mFPSSliderLabel);
		mCamPosToolBar->addWidget(mFPSSlider);

		mStatusViewToolBar = new QToolBar();
	/*	mStatusViewToolBar->setIconSize(QSize(16, 16));
		mStatusViewToolBar->addAction(actTogglePolymode);*/
		/*mStatusViewToolBar->addAction(actFullScreen);
		mStatusViewToolBar->addAction(actSuperFullScreen);
		*/
		mStatusShowHideToolBar = new QToolBar();
		mStatusShowHideToolBar->setIconSize(QSize(16, 16));
		/* mStatusShowHideToolBar->addAction(actToggleExplorer);
		mStatusShowHideToolBar->addAction(actToggleProjectFiles);
		mStatusShowHideToolBar->addAction(actToggleLayer);
		mStatusShowHideToolBar->addAction(actToggleProperties);
		mStatusShowHideToolBar->addAction(actToggleTools);
		mStatusShowHideToolBar->addAction(actToggleResources);
		mStatusShowHideToolBar->addAction(actToggleLog);*/

		//for(unsigned int dw = 0; dw < mCustomToggleActions.size();dw++)
		//{
		//	mStatusShowHideToolBar->addAction(mCustomToggleActions[dw]);
		//}

		statusbar_->insertPermanentWidget(0, mCamPosToolBar);
		statusbar_->addPermanentWidget(mStatusViewToolBar);
		statusbar_->addPermanentWidget(mStatusShowHideToolBar);

		connect(mFPSSlider, SIGNAL(valueChanged( int )), this, SLOT(fpsValueChanged( int )));
	}
	//------------------------------------------------------------------------------
	//static LogBuffer LOGBUFFER;
	//------------------------------------------------------------------------------
	void QtMainWindow::messageLogged(const Ogre::String &message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String &logName, bool &skipThisMessage)
	{
		//LOGBUFFER.append(lml, message);
	}
	//------------------------------------------------------------------------------
	void QtMainWindow::setupLog()
	{
		/*mLogDockWidget = new QDockWidget(this);
		mLogDockWidget->setAllowedAreas(Qt::BottomDockWidgetArea);
		mLogDockWidget->setObjectName(QString::fromUtf8("logDockWidget"));
		mLogDockWidget->setWindowTitle(QApplication::translate("SandGis", "SandGis Console", 0, QApplication::UnicodeUTF8));

		mLogWidget = new QListWidget();
		mLogWidget->setObjectName(QString::fromUtf8("logWidget"));
		mLogWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
		mLogWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
		mLogWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
		mLogWidget->setWordWrap(true);
		QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		sizePolicy.setHeightForWidth(mLogWidget->sizePolicy().hasHeightForWidth());
		mLogWidget->setSizePolicy(sizePolicy);

		mScriptConsoleWidget = new QWidget();
		QVBoxLayout* verticalLayout = new QVBoxLayout(mScriptConsoleWidget);
		verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
		QVBoxLayout* verticalLayout_2 = new QVBoxLayout();
		verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
		QHBoxLayout* horizontalLayout = new QHBoxLayout();
		horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
		mTxtScriptInput = new QLineEdit(mScriptConsoleWidget);
		mTxtScriptInput->setObjectName(QString::fromUtf8("txtScriptInput"));
		horizontalLayout->addWidget(mTxtScriptInput);
		QPushButton* btnRunScript = new QPushButton(tr("Execute"), mScriptConsoleWidget);
		btnRunScript->setObjectName(QString::fromUtf8("btnRunScript"));
		connect(btnRunScript, SIGNAL(clicked()), this, SLOT(runScriptClicked()));
		connect(mTxtScriptInput, SIGNAL(returnPressed()), this, SLOT(runScriptClicked()));
		horizontalLayout->addWidget(btnRunScript);
		verticalLayout_2->addLayout(horizontalLayout);
		mListScriptOutput = new QListWidget(mScriptConsoleWidget);
		mListScriptOutput->setObjectName(QString::fromUtf8("listScriptOutput"));
		mListScriptOutput->setContextMenuPolicy(Qt::ActionsContextMenu);
		mListScriptOutput->setSelectionMode(QAbstractItemView::ExtendedSelection);
		mListScriptOutput->setSelectionBehavior(QAbstractItemView::SelectRows);
		mListScriptOutput->setWordWrap(true);
		mListScriptOutput->setSizePolicy(sizePolicy);
		verticalLayout_2->addWidget(mListScriptOutput);
		verticalLayout->addLayout(verticalLayout_2);
		mLogTabs = new QTabWidget(mLogDockWidget);
		mLogTabs->addTab(mLogWidget, tr("Log"));
		mLogTabs->addTab(mScriptConsoleWidget, tr("Script Console"));

		mLogDockWidget->setWidget(mLogTabs);
		mLogDockWidget->setVisible(false);

		addDockWidget(static_cast<Qt::DockWidgetArea>(8), mLogDockWidget);

		Ogre::LogManager::getSingleton().getDefaultLog()->addListener(this);*/
	}
	//------------------------------------------------------------------------------
	void QtMainWindow::updateLog(QListWidgetItem* item)
	{
		Q_ASSERT(item);
		item->setBackgroundColor(QColor("#000000"));
		//mLogWidget->addItem(item);
		switch( item->type() )
		{
		case 4:
			// Warning
			item->setText(item->text());
			item->setTextColor(QColor("#FFCC00"));
			//item->setHidden(!actLogShowWarnings->isChecked());
			break;
		case 3:
			// Error
			item->setText(item->text());
			item->setTextColor(QColor("#EE1100"));
			//item->setHidden(!actLogShowErrors->isChecked());
			break;
		case 2:
			// Info
			item->setText(item->text());
			item->setTextColor(QColor("#C0C0C0"));
			//item->setHidden(!actLogShowInfo->isChecked());
			break;
		case 1:
			// Debug
			item->setText(item->text());
			item->setTextColor(QColor("#C0C0C0"));
			//item->setHidden(!actLogShowDebug->isChecked());
			break;
		default:
			break;
		}
	}

	///@brief display fps on status bar
	///@param visible trangle numbers
	void QtMainWindow::displayTriangleNum(int triangle_num)
	{
		mTriangleCountLabel->setText(QString("Triangles visible: %1").arg(triangle_num));
	}

	///@brief display fps
	///@param fps value
	void QtMainWindow::displayFPS(float fps)
	{
		mFPSLabel->setText(QString("FPS:%1").arg(fps));
	}

	///@brief display camera position
	void QtMainWindow::displayCameraPos(float x, float y, float z)
	{
		mCamPosLabel->setText(QString("Camera Position:%1,%2,%3").arg(x).arg(y).arg(z));
	}

	void QtMainWindow::fpsValueChanged ( int value )
	{
		target_render_count_ = (value + 1) * 5;
		mFPSSliderLabel->setText(QString("FPS (%1)").arg(target_render_count_));
	}

	//------------------------------------------------------------------------------
	unsigned int renderDelta = 0;
	unsigned int totalMemoryUsage = 0;
	unsigned int lastConsolePos = 0;
	void QtMainWindow::timerLoop()
	{
		if(isMinimized() || !app_active_)
		{
			if(timer_->interval() != 200)
				timer_->setInterval(200);
		}
		else
		{
			if(timer_->interval() != 0)
				timer_->setInterval(0);
		}

	/*	LogDataVector messages;
		LOGBUFFER.getBuffer(messages);*/

		//for(unsigned int i = 0;i < messages.size();i++)
		//{
		//	updateLog(new QListWidgetItem(messages[i].mMessage, 0, messages[i].mLevel) );
		//}

		//if(messages.size() > 0)
		//	mLogWidget->scrollToBottom();

#if OGRE_MEMORY_TRACKER
		unsigned int total = Ogre::MemoryTracker::get().getTotalMemoryAllocated() / (1024 * 1024);
		if(total != totalMemoryUsage)
		{
			totalMemoryUsage = total;
			mMemoryUsageLabel->setText(QString("MU : %1MB").arg(total));
		}
#endif


		//limit fps to target_render_count_
		//default value is 30 fps
		QTime time = QTime::currentTime();
		unsigned int curtime = time.hour() * 60 + time.minute();
		curtime = (curtime * 60) + time.second();
		curtime = (curtime * 1000) + time.msec();

		unsigned int timediff = curtime - last_time_;
		last_time_ = curtime;

		renderDelta += timediff;

		unsigned int TargetDelta;

		//targetdelta is the value how many milliseconds need to update frame at most!
		if(target_render_count_ > 0)
			TargetDelta = 1000 / target_render_count_;
		else
			TargetDelta = 1000000;

		if(renderDelta >= TargetDelta)
		{
			renderDelta = renderDelta % TargetDelta;
			this->updateRenderViews();
		}

	}

	///update all render views
	void QtMainWindow::updateRenderViews(void)
	{
		auto it = render_views_.begin();
		auto it_end = render_views_.end();
		for (; it != it_end; ++it)
		{
			(*it)->update();
		}
	}

	//------------------------------------------------------------------------------------
	bool in_active_event = false;
	bool QtMainWindow::eventFilter(QObject *obj,  QEvent *event)
	{
		if (obj == app_object_  &&  !in_active_event)
		{
			if (event->type() == QEvent::ApplicationActivate)
			{
				// This gets called when the application starts, and when you switch back.
				in_active_event = true;
				app_active_ = true;
				in_active_event = false;
			}

			else if (event->type() == QEvent::ApplicationDeactivate)
			{
				// This gets called when the application closes, and when you switch away.
				in_active_event = true;
				app_active_ = false;
				in_active_event = false;
			}
		}

		return QMainWindow::eventFilter(obj, event);
	}

}
//------------------------------------------------------------------------------------