/// @file OgreContext.h
/// @brief  Ogre context holds all ogre objects
/// @author luoshasha
/// @version 1.0
/// @date 2012-08-09
#ifndef _OGREPRESENTER_H_
#define _OGREPRESENTER_H_

#include "sandgis.h"
#include "Noncopyable.h"

#include <QString>

//stl
#include <unordered_map>
#include <string>

QT_BEGIN_NAMESPACE
class QX11Info;
class QRectF;
QT_END_NAMESPACE

namespace sandgis
{
	///@brief ogre context holds all ogre objects
	///@autor luoshasha
	class OgreContext : Noncopyable
	{
	public:

		typedef std::unordered_map<std::string, Ogre::SceneManager*> SceneManagerMap;

		///cont.
		OgreContext(void);
		///@brief cond.
		~OgreContext(void);
		/// @brief  setup ogre
		///
		/// @param ogre_plugin ogre plugin config file
		/// @param ogre_config ogre config file
		/// @param ogre_log ogre log file
		/// @param ogre_res_config ogre resources config file
		void initialiseOgre(const QString& orge_plugin, 
			const QString& ogre_config, 
			const QString& orge_log);

		///@brief load ogre resources config file
		///@param res_cfg_file resource config file name
		void loadResourcesConfig(const QString& res_cfg_file);

		///@brief create renderwindow from external window info
		///@param hwnd w32 window handle
		///@param x11info x11 window info when running on non-win32 platform
		Ogre::RenderWindow* initRenderWindowFromExternal(const QString& name,
			unsigned hwnd, 
			int width, 
			int height,
			const QX11Info* x11info = 0,
			bool borderless = false);

		///@brief gets if ogre has been initialised
		///@remark call initialiseOgre method to initialise ogre
		bool isOgreInitialised() const
		{
			return (_ogre_root != nullptr);
		}

		///@brief load a scene
		///@param rect:scene initial extents
		///@param bkcolor scene background color
		///@param eye_pos initial eye position
		///@param target_pos initial target position
		///@param renderwindow render window which scene rendering into
		void createScene(const QString& name,
			const QRectF& rect,
			const Ogre::ColourValue& bkcolor, 
			const Ogre::Vector3& eye_pos,
			const Ogre::Vector3& target_pos,
			Ogre::RenderWindow* pRenderWindow);

		///@brief distroy a scene
		///@param scene name
		void destroyScene(const QString& name);

		///@brief gets scene manager by name
		///@param name scene manager name
		Ogre::SceneManager* getSceneManager(const QString& name) const;

		///@brief gets first created scene manager
		Ogre::SceneManager* getFirstSceneManager() const;

		///@brief gets if there is an invalid scene
		bool isSceneLoaded(void) const
		{
			return (_scenemanagers.size() != 0);
		}

		///@brief gets ogre root instance
		Ogre::Root* root() const
		{
			return _ogre_root;
		}
	private:
		SceneManagerMap _scenemanagers;
		Ogre::Root* _ogre_root;
	};
}


#endif
