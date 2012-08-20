/// @file WorkspaceRoot.cpp
/// @brief  workspace root implementation
/// @author luoshasha
/// @version 1.0
/// @date 2012-08-09
#include "presenter\workspaceroot.h"
#include "presenter\OgreContext.h"
#include "view\workspace.h"
#include "view\Qtmainwindow.h"

//_Self define just for convenience
#define _Self WorkspaceRoot

namespace sandgis
{
	// -------------------------------------------------------------------------------------
	/// @brief  define static instance
	// =------------------------------------------------------------------------------------
	_Self* WorkspaceRoot::_instance = nullptr;
	_Self::WorkspaceRoot(void)
	{
		_instance = this;
	}

	_Self::~WorkspaceRoot(void)
	{
	}

	// -------------------------------------------------------------------------------------
	/// @brief  initialise intialise ogre rendering system and workspace
	// =------------------------------------------------------------------------------------
	void _Self::initialise(void)
	{
		//setup ogre
		_ogre_context.reset(new OgreContext());
#if defined(_DEBUG)
		_ogre_context->initialiseOgre("plugins_d.cfg", "ogre.cfg", "ogre.log");
		_ogre_context->loadResourcesConfig("resources_d.cfg");
#else
		_ogre_context->initialiseOgre("plugins.cfg", "ogre.cfg", "ogre.log");
		_ogre_context->loadResourcesConfig("resources_d.cfg");
#endif
		//create workspace
		QtMainWindow* mainwindow = new QtMainWindow();
		mainwindow->setWindowState(Qt::WindowMaximized);
		_workspace.reset(new Workspace(mainwindow));

		//notify workspace created
		ListenerList::iterator it = _listeners.begin();
		for (; it != _listeners.end(); ++it)
			(*it)->WorkspaceCreated();
	}

	// -------------------------------------------------------------------------------------
	/// @brief  _workspaceUnloaded <++>internal using
	///  called when app closed
	///  don't call this method directly
	// =------------------------------------------------------------------------------------
	void _Self::_workspaceUnloaded(void)
	{
		ListenerList::iterator it = _listeners.begin();
		for (; it != _listeners.end(); ++it)
			(*it)->WorkspaceUnloaded();
	}

	void _Self::addListener(WorkspaceListener* listener)
	{
		_listeners.push_front(listener);
	}

	void _Self::removeListener(WorkspaceListener* listener)
	{
		_listeners.remove(listener);
	}

	void _Self::safeInvork(std::function<void(void)> func)
	{
		//not implement!
	}
}
