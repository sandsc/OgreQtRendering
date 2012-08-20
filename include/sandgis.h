#pragma once

// Last Change:  2012-08-08 15:16:36
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4251)   // needs to have dll-interface to be used by clients..
#pragma warning(disable:4819)
#endif

namespace sandgis
{
	/// @brief  global major version of application
	const int MajorVersion = 4;


	/// @brief  global minor version of applicaton
	//full version string will representer as "major.minor"
	const int MinorVersion = 0;
}

#include "OgrePrerequisites.h"

#ifdef _MSC_VER
#pragma warning(pop)
#endif
