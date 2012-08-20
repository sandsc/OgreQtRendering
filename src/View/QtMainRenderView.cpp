// =====================================================================================
// 
//       Filename:  QtMainRenderView.cpp
// 
//    Description:  qt render view source
// 
//        Version:  1.0
//        Created:  8/17/2012 11:34:55 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  luoshasha (sand), luoshasha@foxmail.com
//        Company:  
// 
// =====================================================================================
#include "view/QtMainRenderView.h"
#include "view/QtOgreWidget.h"
namespace sandgis
{
	QtMainRenderView::QtMainRenderView(QWidget* parent, bool doloadfile, int f, bool content_owner)
		:ogre_widget_(new QtOgreWidget(parent, doloadfile, static_cast<Qt::WindowFlags>(f))),
		content_owner_(content_owner)
	{
		this->setName("SandGisMainRenderView");
	}

	QtMainRenderView::~QtMainRenderView(void)
	{
		//widget will destroy when it's parent destroyed
		if (content_owner_)
		{
			delete ogre_widget_;
			ogre_widget_ = 0;
		}
	}

	Ogre::RenderWindow* QtMainRenderView::renderWindow(void) const
	{
		return ogre_widget_->renderWindow();
	}

	void QtMainRenderView::sceneLoaded(void)
	{
		ogre_widget_->sceneLoaded();
	}

	void QtMainRenderView::sceneDestroyed(void)
	{
		ogre_widget_->sceneDestroyed();
	}

	const char* QtMainRenderView::currentSceneName(void) const
	{
		return ogre_widget_->currentSceneName();
	}

	void QtMainRenderView::update(void)
	{
		ogre_widget_->update();
	}

	QtMainRenderView::_ContentType* QtMainRenderView::content(void) const
	{
		return ogre_widget_;
	}
}
