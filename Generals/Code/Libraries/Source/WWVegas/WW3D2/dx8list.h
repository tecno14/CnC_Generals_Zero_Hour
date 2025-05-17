/*
**	Command & Conquer Generals(tm)
**	Copyright 2025 Electronic Arts Inc.
**
**	This program is free software: you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**	the Free Software Foundation, either version 3 of the License, or
**	(at your option) any later version.
**
**	This program is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
**	You should have received a copy of the GNU General Public License
**	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : ww3d                                                         *
 *                                                                                             *
 *                     $Archive:: /Commando/Code/ww3d2/dx8list.h                              $*
 *                                                                                             *
 *              Original Author:: Greg Hjelstrom                                               *
 *                                                                                             *
#ifdef OG
 *                      $Author:: Hector_y                                                    $*
#endif
#ifdef ZH
 *                       Author : Kenny Mitchell                                               * 
#endif
 *                                                                                             *
#ifdef OG
 *                     $Modtime:: 4/25/01 1:37p                                               $*
#endif
#ifdef ZH
 *                     $Modtime:: 06/27/02 1:27p                                              $*
#endif
 *                                                                                             *
#ifdef OG
 *                    $Revision:: 4                                                           $*
#endif
#ifdef ZH
 *                    $Revision:: 5                                                           $*
#endif
 *                                                                                             *
#ifdef ZH
 * 06/27/02 KM Texture class abstraction																			*
#endif
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#if defined(_MSC_VER)
#pragma once
#endif

#ifndef DX8LIST_H
#define DX8LIST_H

#include "always.h"
#include "multilist.h"


/*
** Here we're just typedefing some multi-lists so we don't have to write the
** long template names.
*/
class DX8TextureCategoryClass;
typedef MultiListClass<DX8TextureCategoryClass>			TextureCategoryList;
typedef MultiListIterator<DX8TextureCategoryClass>		TextureCategoryListIterator;

class DX8FVFCategoryContainer;
typedef MultiListClass<DX8FVFCategoryContainer>			FVFCategoryList;
typedef MultiListIterator<DX8FVFCategoryContainer>		FVFCategoryListIterator;

class DX8PolygonRendererClass;
typedef MultiListClass<DX8PolygonRendererClass>			DX8PolygonRendererList;
typedef MultiListIterator<DX8PolygonRendererClass>		DX8PolygonRendererListIterator; 

#ifdef OG
class DX8TextureTrackerClass;
typedef MultiListClass<DX8TextureTrackerClass>			DX8TextureTrackerList;
typedef MultiListIterator<DX8TextureTrackerClass>		DX8TextureTrackerListIterator;
#endif
#ifdef ZH
class TextureTrackerClass;
typedef MultiListClass<TextureTrackerClass>				TextureTrackerList;
typedef MultiListIterator<TextureTrackerClass>			TextureTrackerListIterator;
#endif


#endif //DX8LIST_H

