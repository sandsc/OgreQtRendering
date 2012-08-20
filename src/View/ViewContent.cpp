/// @file ViewContent.cpp
/// @brief  view content source
/// @author luoshasha
/// @version 1.0
/// @date 2012-08-15
#include "view/ViewContent.h"

namespace sandgis
{

	ViewContent::ViewContent(void)
	{

	}

	ViewContent::~ViewContent(void)
	{

	}

	void ViewContent::setTitle(const std::string& title)
	{
		if (title_ == title)
		{
			title_ = title;
			this->onTitleChanged();
		}
	}


	//----------------------------------------------------------------------
	//  dock content impl
	//----------------------------------------------------------------------
	DockContent::DockContent(void)
		: dock_state_(dockstate::None)
	{

	}

	DockContent::~DockContent(void)
	{

	}

	void DockContent::setDockState(dockstate::DockState state)
	{
		if (dock_state_ != state)
		{
			dock_state_ = state;
			onDockStateChanged();
		}
	}
}
