#include "sandgis.h"
#include "view/workspace.h"
#include "View/qtmainwindow.h"
#include "presenter/WorkspaceRoot.h"

//qt
#include <QApplication>
#include <QWidget>
#include <QSettings>
#include <QTranslator>
#include <QLibraryInfo>
#include <QLocale>
#include <QSplashScreen>
#include <QVBoxLayout>

//ogre
#include "ogreroot.h"

using namespace sandgis;
///@brief main function
int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	app.setOrganizationName("SandSoft");
	app.setApplicationName(QString("SandGis ") + QString("%1.%2").arg(MajorVersion).arg(MinorVersion));

	//international settings
	QTranslator qtranslator;
	QString qt_lang_res  = "qt_" + QLocale::system().name();
	if (qtranslator.load(qt_lang_res, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
	{
		app.installTranslator(&qtranslator);
	}

	QPixmap pixmap(":/images/splash.png");
	QSplashScreen *splash = new QSplashScreen(pixmap);
	splash->show();

	std::unique_ptr<WorkspaceRoot> root(new WorkspaceRoot());
	root->initialise();

	QWidget* mainwindow = root->workspace()->_workspaceImpl();
	root->workspace()->_workspaceImpl()->setApplicationObject(&app);
	mainwindow->show();

	//end splash screen
	splash->finish(mainwindow);
	delete splash;

	int retval = app.exec();
	//notify workspace unload
	root->_workspaceUnloaded();
	return retval;
}