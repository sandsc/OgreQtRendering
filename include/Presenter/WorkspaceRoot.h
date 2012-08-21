/// @file WorkspaceRoot.h
/// @brief  workspace root class holds all app global info
/// @author luoshasha
/// @version 1.0
/// @date 2012-08-09
#pragma once

#include "Noncopyable.h"

//stl
#include <functional>
#include <forward_list>
#include <memory>

namespace sandgis{
	class Workspace;
	class OgreContext;
}

namespace sandgis
{
	///@brief  workspace listener
	///@autor luoshasha
	struct WorkspaceListenevoidr
	{	
		/// @brief  virtul dest.
		virtual ~WorkspaceListener() {}
		/// @brief  event occurs when workspace closed
		virtual void WorkspaceUnloaded() = 0;

		/// @brief  events occurs when workspace created
		virtual void WorkspaceCreated() = 0;
	};

	/// @brief  workspace root instance hold all global workspace info
	/// @author luoshasha
	class WorkspaceRoot : Noncopyable
	{
	public:
		///listener list
		typedef std::forward_list<WorkspaceListener*> ListenerList;
		/// @brief  cont.
		WorkspaceRoot(void);
		/// @brief  dest.
		~WorkspaceRoot(void);

		/// @brief  gets workspace singleton
		/// @returns  selfptr
		static WorkspaceRoot* instance(void)
		{
			return _instance;
		}

		/**
		 * @brief gets workspace instance
		 *
		 * @returns   
		 */
		Workspace* workspace(void) const
		{
			return _workspace.get();
		}

		/*/// @brief  gets main window
		///
		/// @returns  mainwindow interface 
		IMainWindow* mainwindow(void) const
		{
			return _mainwindow.get();
		}*/
		
		/// @brief  gets ogre context 
		///
		/// @returns  ogre context contains all ogre rendering info 
		//e.g. ogre root instance, renderwindow and scenemanager
		OgreContext* ogreContext(void) const
		{
			return _ogre_context.get();
		}

		/// @brief  add workspace listener
		///
		/// @param listener
		void addListener(WorkspaceListener* listener);

		/// @brief  remove workspace listener
		///
		/// @param listener
		void removeListener(WorkspaceListener* listener);

		/// @brief  invork function on ui thread
		///
		/// @param func func to invoke
		void safeInvork(std::function<void(void)> func);

		/// @brief  initialise workspace and ogre context
		void initialise(void);

		///@brief internal use
		///notify workspace has been unloaded
		void _workspaceUnloaded(void);

	private:
		///sinleton instance
		static WorkspaceRoot* _instance;
		///ogre context
		std::unique_ptr<OgreContext> _ogre_context;
		///workspace is same instance with IMainwindow;
		std::unique_ptr<Workspace> _workspace;
		///worksapce listners
		ListenerList _listeners;
	};
}
