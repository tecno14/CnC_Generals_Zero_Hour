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
 *                 Project Name : DX8 Texture Manager                                          *
 *                                                                                             *
 *                     $Archive:: /Commando/Code/ww3d2/dx8texman.cpp                          $*
 *                                                                                             *
 *              Original Author:: Hector Yee                                                   *
 *                                                                                             *
#ifdef OG
 *                      $Author:: Hector_y                                                    $*
#endif
#ifdef ZH
 *                       Author : Kenny Mitchell                                               * 
#endif
 *                                                                                             *
#ifdef OG
 *                     $Modtime:: 4/26/01 1:41p                                               $*
#endif
#ifdef ZH
 *                     $Modtime:: 06/27/02 1:27p                                              $*
#endif
 *                                                                                             *
#ifdef OG
 *                    $Revision:: 3                                                           $*
#endif
#ifdef ZH
 *                    $Revision:: 4                                                           $*
#endif
 *                                                                                             *
#ifdef ZH
 * 06/27/02 KM Texture class abstraction																			*
#endif
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   DX8TextureManagerClass::Shutdown -- Shuts down the texture manager                        *
 *   DX8TextureManagerClass::Add -- Adds a texture to be managed                               *
 *   DX8TextureManagerClass::Remove -- Removes a texture from being managed                    *
 *   DX8TextureManagerClass::Release_Textures -- Releases the internal d3d texture             *
 *   DX8TextureManagerClass::Recreate_Textures -- Reallocates lost textures                    *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


// This class manages textures that are in the default pool
// ensuring that they are released on device loss
// and created on device reset

// Note: It does NOT addref to textures because it is called in the texture
// destructor

#include "dx8texman.h"

#ifdef OG
DX8TextureTrackerList DX8TextureManagerClass::Managed_Textures;
#endif
#ifdef ZH
TextureTrackerList DX8TextureManagerClass::Managed_Textures;
#endif


/***********************************************************************************************
 * DX8TextureManagerClass::Shutdown -- Shuts down the texture manager                          *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:                                                                                      *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   4/25/2001  hy : Created.                                                                  *
#ifdef ZH
 *   5/16/2002  km : Added depth stencil texture tracking and abstraction                      *
#endif
 *=============================================================================================*/
void DX8TextureManagerClass::Shutdown()
{
	while (!Managed_Textures.Is_Empty())
	{
#ifdef OG
		DX8TextureTrackerClass *track=Managed_Textures.Remove_Head();
#endif
#ifdef ZH
		TextureTrackerClass *track=Managed_Textures.Remove_Head();
#endif
		delete track;
		track=NULL;
	}
}

/***********************************************************************************************
 * DX8TextureManagerClass::Add -- Adds a texture to be managed                                 *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:                                                                                      *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   4/25/2001  hy : Created.                                                                  *
#ifdef ZH
 *   5/16/2002  km : Added depth stencil texture tracking and abstraction                      *
#endif
 *=============================================================================================*/
#ifdef OG
void DX8TextureManagerClass::Add(DX8TextureTrackerClass *track)
#endif
#ifdef ZH
void DX8TextureManagerClass::Add(TextureTrackerClass *track)
#endif
{
	// this function should only be called by the texture constructor
	Managed_Textures.Add(track);
}


/***********************************************************************************************
 * DX8TextureManagerClass::Remove -- Removes a texture from being managed                      *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:                                                                                      *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   4/25/2001  hy : Created.                                                                  *
#ifdef ZH
 *   5/16/2002  km : Added depth stencil texture tracking and abstraction                      *
#endif
 *=============================================================================================*/
#ifdef OG
void DX8TextureManagerClass::Remove(TextureClass *tex)
#endif
#ifdef ZH
void DX8TextureManagerClass::Remove(TextureBaseClass *tex)
#endif
{
	// this function should only be called by the texture destructor
#ifdef OG
	DX8TextureTrackerListIterator it(&Managed_Textures);
#endif
#ifdef ZH
	TextureTrackerListIterator it(&Managed_Textures);
#endif

	while (!it.Is_Done())
	{
#ifdef OG
		DX8TextureTrackerClass *track=it.Peek_Obj();		
		if (track->Texture==tex)
#endif
#ifdef ZH
		TextureTrackerClass *track=it.Peek_Obj();		
		if (track->Get_Texture()==tex)
#endif
		{			
			it.Remove_Current_Object();
			delete track;
			break;
		}
		it.Next();
	}
}


/***********************************************************************************************
 * DX8TextureManagerClass::Release_Textures -- Releases the internal d3d texture               *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:                                                                                      *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   4/25/2001  hy : Created.                                                                  *
#ifdef ZH
 *   5/16/2002  km : Added depth stencil texture tracking and abstraction                      *
#endif
 *=============================================================================================*/
void DX8TextureManagerClass::Release_Textures()
{
#ifdef OG
	DX8TextureTrackerListIterator it(&Managed_Textures);
#endif
#ifdef ZH
	TextureTrackerListIterator it(&Managed_Textures);
#endif

	while (!it.Is_Done())
	{
#ifdef OG
		DX8TextureTrackerClass *track=it.Peek_Obj();		
		WWASSERT(track->Texture->D3DTexture);
		track->Texture->D3DTexture->Release();
		track->Texture->D3DTexture=NULL;
#endif
#ifdef ZH
		TextureTrackerClass *track=it.Peek_Obj();		
		track->Release();

#endif
		it.Next();
	}
}


/***********************************************************************************************
 * DX8TextureManagerClass::Recreate_Textures -- Reallocates lost textures                      *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:                                                                                      *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   4/25/2001  hy : Created.                                                                  *
#ifdef ZH
 *   5/16/2002  km : Added depth stencil texture tracking and abstraction                      *
#endif
 *=============================================================================================*/
void DX8TextureManagerClass::Recreate_Textures()
{
#ifdef OG
	DX8TextureTrackerListIterator it(&Managed_Textures);
#endif
#ifdef ZH
	TextureTrackerListIterator it(&Managed_Textures);
#endif

	while (!it.Is_Done())
	{
#ifdef OG
		DX8TextureTrackerClass *track=it.Peek_Obj();
		WWASSERT(track->Texture->D3DTexture==NULL);
		track->Texture->D3DTexture=DX8Wrapper::_Create_DX8_Texture(track->Width,track->Height,
			track->Format,track->Mip_level_count,D3DPOOL_DEFAULT,track->RenderTarget);
		track->Texture->Dirty=true;
#endif
#ifdef ZH
		TextureTrackerClass *track=it.Peek_Obj();
		track->Recreate();
		track->Get_Texture()->Set_Dirty();

#endif
		it.Next();
	}
}
