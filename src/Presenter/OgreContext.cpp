/// @file OgreContext.cpp
/// @brief  ogre context holds ogre objects and operations
/// @author luoshasha
/// @version 1.0
/// @date 2012-08-09
#include "presenter/OgreContext.h"

// Last Change:  2012-08-09 17:15:04
//qt
#include <QSettings>
#include <QRectF>

//ogre
#include "Ogre.h"

#include <cmath>

#define _Self OgreContext

using namespace Ogre;

namespace sandgis
{
	/// @brief  OgreContext cont.
	_Self::OgreContext(void) :
			_ogre_root(nullptr)
	{

	}

	_Self::~OgreContext(void)
	{
		_ogre_root->shutdown();
		OGRE_DELETE _ogre_root;
	}

	/// @brief  initialiseOgre 
	///
	/// @param ogre_plugin plug in config file
	/// @param ogre_config ogre config file
	/// @param orge_log ogre log file
	void _Self::initialiseOgre(const QString& ogre_plugin, const QString& ogre_config, const QString& orge_log)
	{
		if (this->_ogre_root != nullptr)
		{
			return;
		}
		Ogre::Root *root = OGRE_NEW Ogre::Root(ogre_plugin.toUtf8().constData());
		this->_ogre_root = root;

		QSettings settings;
		settings.beginGroup("preferences");
		QString renderer = settings.value("renderSystem", "").toString();
		if (renderer.isEmpty())
		{
			renderer = "OpenGL Rendering Subsystem";
		}

		// setup a renderer
		Ogre::RenderSystemList::const_iterator prenderer = root->getAvailableRenderers().begin();
		while(prenderer != root->getAvailableRenderers().end())
		{
			Ogre::String rName = (*prenderer)->getName();
			if (rName == renderer.toStdString())
				break;
			prenderer++;
		}
		Ogre::RenderSystem *rsys = *prenderer;

		int antialias = settings.value("antiAliasing", 0).toInt();
		bool vsync = settings.value("useVSync", false).toBool();
		bool azerty = settings.value("useAZERTY", false).toBool();
		settings.endGroup();

		// Some standard rendering system configurations
		if(vsync)
			rsys->setConfigOption("VSync", "Yes");
		else
			rsys->setConfigOption("VSync", "No");

		rsys->setConfigOption("FSAA", Ogre::StringConverter::toString(antialias));
		// Set the rendering system and initialise OGRE
		root->setRenderSystem(rsys);

		// initialize without creating window
		root->getRenderSystem()->setConfigOption("Full Screen", "No");
		//mOgreRoot->saveConfig();
		root->initialise(false); // don't create a window
	}

	void _Self::loadResourcesConfig(const QString& cfg_file)
	{
		// Load resource paths from config file
		Ogre::ConfigFile cf;
		cf.load(cfg_file.toUtf8().constData());

		// Go through all sections & settings in the file
		Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
		Ogre::String secName, typeName, archName;
		while (seci.hasMoreElements())
		{
			secName = seci.peekNextKey();
			Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
			Ogre::ConfigFile::SettingsMultiMap::iterator i;
			for (i = settings->begin(); i != settings->end(); ++i)
			{
				typeName = i->first;
				archName = i->second;
				Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
					archName, typeName, secName);
			}
		}
	}

	Ogre::RenderWindow* _Self::initRenderWindowFromExternal(const QString& name,
		unsigned hwnd, 
		int width, 
		int height,
		const QX11Info* x11info/*= 0*/,
		bool borderless/*= false*/)
	{
		//Creating Ogre render Window
		Ogre::NameValuePairList params;
#if defined(Q_WS_MAC) || defined(Q_WS_WIN)
		params["externalWindowHandle"] = Ogre::StringConverter::toString((size_t)hwnd);
#else
		const QX11Info info = *x11info;
		//QX11Info info = x11Info();
		Ogre::String winHandle;
		winHandle  = Ogre::StringConverter::toString((unsigned long)(info.display()));
		winHandle += ":";
		winHandle += Ogre::StringConverter::toString((unsigned int)(info.screen()));
		winHandle += ":";
		winHandle += Ogre::StringConverter::toString((unsigned long)(this->winId()));
		winHandle += ":";
		winHandle += Ogre::StringConverter::toString((unsigned long)(info.visual()));

		params["externalWindowHandle"] = winHandle;
#endif

#if defined(Q_WS_MAC)
		params["macAPI"] = "cocoa";   
		params["macAPICocoaUseNSView"] = "true";
#endif
		if (borderless)
			params["border"] = "none";

		Ogre::Root* root = Ogre::Root::getSingletonPtr();
		return root->createRenderWindow(name.toAscii().constData(),
			width,
			height,
			false,
			&params);
	}


	Ogre::SceneManager* _Self::createScene(const QString& name)
	{
		if (name.isEmpty())
			return 0;
		Ogre::SceneManager* pSceneManager = _ogre_root->createSceneManager(Ogre::ST_GENERIC, name.toAscii().constData());
		_scenemanagers[name.toAscii().constData()] = pSceneManager;
		return pSceneManager;
	}

	void _Self::destroyScene(const QString& name)
	{
		if (name.isEmpty())
			return;
		auto it = _scenemanagers.find(name.toAscii().constData());
		if (it != _scenemanagers.end())
		{
			Ogre::SceneManager* pScene = (*it).second;
			_ogre_root->destroySceneManager(pScene);
		}
	}

	Ogre::SceneManager* _Self::getSceneManager(const QString& name) const
	{
		if (name.isEmpty())
			return nullptr;
		auto it = _scenemanagers.find(name.toAscii().constData());
		if (it != _scenemanagers.end())
		{
			Ogre::SceneManager* pScene = (*it).second;
			return pScene;
		}
		return nullptr;
	}

	Ogre::SceneManager* _Self::getFirstSceneManager(void) const
	{
		if (_scenemanagers.size() == 0)
			return nullptr;
		return _scenemanagers.begin()->second;
	}
}
