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
 *                     $Archive:: /Commando/Code/ww3d2/dx8texman.h                            $*
 *                                                                                             *
 *              Original Author:: Hector Yee                                                   *
 *                                                                                             *
#ifdef OG
 *                      $Author:: Jani_p                                                      $*
#endif
#ifdef ZH
 *                       Author : Kenny Mitchell                                               * 
#endif
 *                                                                                             *
#ifdef OG
 *                     $Modtime:: 7/26/01 5:12p                                               $*
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
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef DX8TEXTUREMANAGER_H
#define DX8TEXTUREMANAGER_H

#include "always.h"
#include "texture.h"
#include "dx8wrapper.h"
#include "ww3dformat.h"
#include "dx8list.h"
#include "ww3dformat.h"
#include "multilist.h"

class DX8TextureManagerClass;

#ifdef OG
class DX8TextureTrackerClass : public MultiListObjectClass
#endif
#ifdef ZH
class TextureTrackerClass : public MultiListObjectClass
#endif
{
#ifdef OG
friend DX8TextureManagerClass;
#endif
public:
#ifdef OG
	DX8TextureTrackerClass(unsigned int w, unsigned int h, WW3DFormat format,
		TextureClass::MipCountType count,bool rt,
		TextureClass *tex):
	Width(w),

#endif
#ifdef ZH
	TextureTrackerClass
	(
		unsigned int w, 
		unsigned int h, 
		MipCountType count,
		TextureBaseClass *tex
	)
	: Width(w),
#endif
	Height(h),
#ifdef OG
	Format(format),
#endif
	Mip_level_count(count),
#ifdef OG
	RenderTarget(rt),
#endif
	Texture(tex)	
#ifdef ZH
	{
	}

	virtual void Recreate() const =0;

	void Release()
#endif
	{
#ifdef ZH
		Texture->Set_D3D_Base_Texture(NULL);
#endif
	}
#ifdef OG
private:

#endif
#ifdef ZH

	TextureBaseClass* Get_Texture() const { return Texture; }

protected:

#endif
	unsigned int Width;
	unsigned int Height;
#ifdef ZH
	MipCountType Mip_level_count;
	TextureBaseClass *Texture;
};

class DX8TextureTrackerClass : public TextureTrackerClass
{
public:
	DX8TextureTrackerClass
	(
		unsigned int w, 
		unsigned int h, 
		WW3DFormat format,
		MipCountType count,
		TextureBaseClass *tex,
		bool rt
	) 
	: TextureTrackerClass(w,h,count,tex), Format(format), RenderTarget(rt)
	{
	}

	virtual void Recreate() const
	{
		WWASSERT(Texture->Peek_D3D_Base_Texture()==NULL);
		Texture->Poke_Texture
		(
			DX8Wrapper::_Create_DX8_Texture
			(
				Width,
				Height,
				Format,
				Mip_level_count,
				D3DPOOL_DEFAULT,
				RenderTarget
			)
		);
	}

private:
#endif
	WW3DFormat Format;
#ifdef OG
	TextureClass::MipCountType Mip_level_count;
#endif
	bool RenderTarget;
#ifdef OG
	TextureClass *Texture;

#endif
#ifdef ZH
};

class DX8ZTextureTrackerClass : public TextureTrackerClass
{
public:
	DX8ZTextureTrackerClass
	(
		unsigned int w,
		unsigned int h,
		WW3DZFormat zformat,
		MipCountType count,
		TextureBaseClass* tex
	)
	: TextureTrackerClass(w,h,count,tex), ZFormat(zformat)
	{
	}

	virtual void Recreate() const
	{
		WWASSERT(Texture->Peek_D3D_Base_Texture()==NULL);
		Texture->Poke_Texture
		(
			DX8Wrapper::_Create_DX8_ZTexture
			(
				Width,
				Height,
				ZFormat,
				Mip_level_count,
				D3DPOOL_DEFAULT
			)
		);
	}

private:
	WW3DZFormat ZFormat;
#endif
};

class DX8TextureManagerClass
{
public:
	static void Shutdown();
#ifdef OG
	static void Add(DX8TextureTrackerClass *track);
	static void Remove(TextureClass *tex);
#endif
#ifdef ZH
	static void Add(TextureTrackerClass *track);
	static void Remove(TextureBaseClass *tex);
#endif
	static void Release_Textures();
	static void Recreate_Textures();
private:
#ifdef OG
	static DX8TextureTrackerList Managed_Textures;
#endif
#ifdef ZH
	static TextureTrackerList Managed_Textures;
#endif
};

#endif // ifdef TEXTUREMANAGER