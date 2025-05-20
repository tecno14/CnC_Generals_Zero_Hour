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
 *                 Project Name : WW3D                                                         *
 *                                                                                             *
 *                     $Archive:: /Commando/Code/ww3d2/texture.h                              $*
 *                                                                                             *
#ifdef OG
 *                      $Author:: Jani_p                                                      $*
#endif
#ifdef ZH
 *                  $Org Author:: Jani_p                                                      $*
#endif
 *                                                                                             *
#ifdef OG
 *                     $Modtime:: 8/17/01 9:41a                                               $*

#endif
#ifdef ZH
 *                       Author : Kenny Mitchell                                               * 
 *                                                                                             * 
 *                     $Modtime:: 08/05/02 1:27p                                              $*
#endif
 *                                                                                             *
#ifdef OG
 *                    $Revision:: 35                                                          $*
#endif
#ifdef ZH
 *                    $Revision:: 46                                                          $*
#endif
 *                                                                                             *
#ifdef ZH
 * 05/16/02 KM Base texture class to abstract major texture types, e.g. 3d, z, cube, etc.
 * 06/27/02 KM Texture class abstraction																			*
 * 08/05/02 KM Texture class redesign (revisited)
#endif
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#if defined(_MSC_VER)
#pragma once
#endif

#ifndef TEXTURE_H
#define TEXTURE_H

#include "always.h"
#include "refcount.h"
#include "chunkio.h"
#include "surfaceclass.h"
#include "ww3dformat.h"
#include "wwstring.h"
#ifdef ZH
#include "vector3.h"
#include "texturefilter.h"
#endif

#ifdef ZH
struct IDirect3DBaseTexture8;
struct IDirect3DTexture8;
struct IDirect3DCubeTexture8;
struct IDirect3DVolumeTexture8;

#endif
class DX8Wrapper;
#ifdef OG
struct IDirect3DTexture8;
#endif
class TextureLoader;
class LoaderThreadClass;
#ifdef OG
class DX8TextureManagerClass;
#endif
class TextureLoadTaskClass;
#ifdef ZH
class CubeTextureClass;
class VolumeTextureClass;
#endif

#ifdef OG
/*************************************************************************
**                             TextureClass
**
** This is our texture class. For legacy reasons it contains some
** information beyond the D3D texture itself, such as texture addressing
** modes.
**
*************************************************************************/
class TextureClass : public W3DMPO, public RefCountClass
#endif
#ifdef ZH
class TextureBaseClass : public RefCountClass

#endif
{
#ifdef OG
	W3DMPO_GLUE(TextureClass)

	friend DX8Wrapper;
	friend TextureLoader;
	friend LoaderThreadClass;
	friend DX8TextureManagerClass;
#endif
#ifdef ZH
	friend class TextureLoader;
	friend class LoaderThreadClass;
	friend class DX8TextureTrackerClass;  //(gth) so it can call Poke_Texture, 
	friend class DX8ZTextureTrackerClass;

#endif

	public:

#ifdef OG
		enum PoolType {

#endif
#ifdef ZH
	enum PoolType 
	{
#endif
			POOL_DEFAULT=0,
			POOL_MANAGED,
			POOL_SYSTEMMEM
		};

#ifdef OG
		enum FilterType {
			FILTER_TYPE_NONE,
			FILTER_TYPE_FAST,
			FILTER_TYPE_BEST,
			FILTER_TYPE_DEFAULT,
			FILTER_TYPE_COUNT
#endif
#ifdef ZH
	enum TexAssetType
	{
		TEX_REGULAR,
		TEX_CUBEMAP,
		TEX_VOLUME

#endif
		};

#ifdef OG
		enum TxtAddrMode {
			TEXTURE_ADDRESS_REPEAT=0,
			TEXTURE_ADDRESS_CLAMP
		};

		enum MipCountType {
			MIP_LEVELS_ALL=0,		// generate all mipmap levels down to 1x1 size
			MIP_LEVELS_1,			// no mipmapping at all (just one mip level)
			MIP_LEVELS_2,
			MIP_LEVELS_3,
			MIP_LEVELS_4,
			MIP_LEVELS_5,
			MIP_LEVELS_6,
			MIP_LEVELS_7,
			MIP_LEVELS_8,
			MIP_LEVELS_10,
			MIP_LEVELS_11,
			MIP_LEVELS_12,
			MIP_LEVELS_MAX			// This isn't to be used (use MIP_LEVELS_ALL instead), it is just an enum for creating static tables etc.
		};

		// Create texture with desired height, width and format.
		TextureClass(
#endif
#ifdef ZH
	// base constructor for derived classes
	TextureBaseClass
	(

#endif
			unsigned width, 
			unsigned height, 
#ifdef OG
			WW3DFormat format,
#endif
			MipCountType mip_level_count=MIP_LEVELS_ALL,
			PoolType pool=POOL_MANAGED,
#ifdef OG
			bool rendertarget=false);

#endif
#ifdef ZH
		bool rendertarget=false,
		bool reducible=true
	);
#endif

#ifdef OG
		// Create texture from a file. If format is specified the texture is converted to that format.
		// Note that the format must be supported by the current device and that a texture can't exist
		// in the system with the same name in multiple formats.
		TextureClass(
			const char *name,
			const char *full_path=NULL,
			MipCountType mip_level_count=MIP_LEVELS_ALL,
			WW3DFormat texture_format=WW3D_FORMAT_UNKNOWN,
			bool allow_compression=true);

		// Create texture from a surface.
		TextureClass(
			SurfaceClass *surface, 
			MipCountType mip_level_count=MIP_LEVELS_ALL);		
#endif
#ifdef ZH
	virtual ~TextureBaseClass();

#endif

#ifdef OG
		TextureClass(IDirect3DTexture8* d3d_texture);
#endif
#ifdef ZH
	virtual TexAssetType Get_Asset_Type() const=0;
#endif

#ifdef OG
		virtual ~TextureClass(void);

#endif
		// Names
		void	Set_Texture_Name(const char * name);
		void	Set_Full_Path(const char * path)			{ FullPath = path; }
#ifdef OG
		const char * Get_Texture_Name(void) const		{ return Name; }
		const char * Get_Full_Path(void) const			{ if (FullPath.Is_Empty ()) return Name; return FullPath; }
#endif
#ifdef ZH
	const StringClass& Get_Texture_Name(void) const		{ return Name; }
	const StringClass& Get_Full_Path(void) const			{ if (FullPath.Is_Empty ()) return Name; return FullPath; }
#endif

		unsigned Get_ID() const { return texture_id; }	// Each textrure has a unique id

		// The number of Mip levels in the texture
#ifdef OG
		unsigned int Get_Mip_Level_Count(void);

#endif
#ifdef ZH
	unsigned int Get_Mip_Level_Count(void) const
	{
		return MipLevelCount;
	}
#endif

#ifdef OG
		// Get surface description of a Mip level (defaults to the highest-resolution one)
		void Get_Level_Description(SurfaceClass::SurfaceDescription &surface_desc, unsigned int level = 0);

#endif
#ifdef ZH
	// Note! Width and Height may be zero and may change if texture uses mipmaps
	int Get_Width() const
	{
		return Width;
	}
	int Get_Height() const
	{
		return Height; 
	}
#endif

#ifdef OG
		// Get the surface of one of the mipmap levels (defaults to highest-resolution one)
		SurfaceClass *Get_Surface_Level(unsigned int level = 0);

#endif
#ifdef ZH
	// Time, after which the texture is invalidated if not used. Set to zero to indicate infinite.
	// Time is in milliseconds.
	void Set_Inactivation_Time(unsigned time) { InactivationTime=time; }
	int Get_Inactivation_Time() const { return InactivationTime; }
#endif

		// Texture priority affects texture management and caching.
		unsigned int Get_Priority(void);
		unsigned int Set_Priority(unsigned int priority);	// Returns previous priority

#ifdef OG
		// Filter and MIPmap settings:
		FilterType Get_Min_Filter(void) const { return TextureMinFilter; }
		FilterType Get_Mag_Filter(void) const { return TextureMagFilter; }
		FilterType Get_Mip_Mapping(void) const { return MipMapFilter; }
		void Set_Min_Filter(FilterType filter) { TextureMinFilter=filter; }
		void Set_Mag_Filter(FilterType filter) { TextureMagFilter=filter; }
		void Set_Mip_Mapping(FilterType mipmap);

		// Texture address mode
		TxtAddrMode Get_U_Addr_Mode(void) const { return UAddressMode; }
		TxtAddrMode Get_V_Addr_Mode(void) const { return VAddressMode; }
		void Set_U_Addr_Mode(TxtAddrMode mode) { UAddressMode=mode; }
		void Set_V_Addr_Mode(TxtAddrMode mode) { VAddressMode=mode; }
#endif
#ifdef ZH
	// Debug utility functions for returning the texture memory usage
	virtual unsigned Get_Texture_Memory_Usage() const=0;

#endif

#ifdef OG
		// Debug utility functions for returning the texture memory usage
		unsigned Get_Texture_Memory_Usage() const;
#endif
		bool Is_Initialized() const { return Initialized; }
		bool Is_Lightmap() const { return IsLightmap; }
		bool Is_Procedural() const { return IsProcedural; }
#ifdef ZH
	bool Is_Reducible() const { return IsReducible; } //can texture be reduced in resolution for LOD purposes?
#endif

		static int _Get_Total_Locked_Surface_Size();
		static int _Get_Total_Texture_Size();
		static int _Get_Total_Lightmap_Texture_Size();
		static int _Get_Total_Procedural_Texture_Size();
		static int _Get_Total_Locked_Surface_Count();
		static int _Get_Total_Texture_Count();
		static int _Get_Total_Lightmap_Texture_Count();
		static int _Get_Total_Procedural_Texture_Count();

#ifdef OG
		// This needs to be called after device has been created
		static void _Init_Filters();

		static void _Set_Default_Min_Filter(FilterType filter);
		static void _Set_Default_Mag_Filter(FilterType filter);
		static void _Set_Default_Mip_Filter(FilterType filter);
#endif
#ifdef ZH
	virtual void Init()=0;

#endif

		// This utility function processes the texture reduction (used during rendering)
		void Invalidate();
#ifdef ZH

	// texture accessors (dx8)
	IDirect3DBaseTexture8 *Peek_D3D_Base_Texture() const;
	void Set_D3D_Base_Texture(IDirect3DBaseTexture8* tex);
#endif

#ifdef OG
		IDirect3DTexture8 *Peek_DX8_Texture()
		{
			return D3DTexture;
		}
#endif
#ifdef ZH
	PoolType Get_Pool() const { return Pool; }

#endif

		bool Is_Missing_Texture();

		// Support for self managed textures
		bool Is_Dirty() { WWASSERT(Pool==POOL_DEFAULT); return Dirty; };
#ifdef ZH
	void Set_Dirty() { WWASSERT(Pool==POOL_DEFAULT); Dirty=true; }
#endif
		void Clean() { Dirty=false; };
#ifdef ZH

	void Set_HSV_Shift(const Vector3 &hsv_shift);
	const Vector3& Get_HSV_Shift() { return HSVShift; }

	bool Is_Compression_Allowed() const { return IsCompressionAllowed; }
#endif

		unsigned Get_Reduction() const;
#ifdef OG
		WW3DFormat Get_Texture_Format() const { return TextureFormat; }
		bool Is_Compression_Allowed() const { return IsCompressionAllowed; }

#endif
#ifdef ZH

	// Background texture loader will call this when texture has been loaded
	virtual void Apply_New_Surface(IDirect3DBaseTexture8* tex, bool initialized, bool disable_auto_invalidation = false)=0;	// If the parameter is true, the texture will be flagged as initialised

	MipCountType MipLevelCount;

	// Inactivate textures that haven't been used in a while. Pass zero to use textures'
	// own inactive times (default). In urgent need to free up texture memory, try
	// calling with relatively small (just few seconds) time override to free up everything
	// but the currently used textures.
	static void Invalidate_Old_Unused_Textures(unsigned inactive_time_override);

	// Apply this texture's settings into D3D
	virtual void Apply(unsigned int stage)=0;

	// Apply a Null texture's settings into D3D
	static void Apply_Null(unsigned int stage);

	virtual TextureClass* As_TextureClass() { return NULL; }
	virtual CubeTextureClass* As_CubeTextureClass() { return NULL; }
	virtual VolumeTextureClass* As_VolumeTextureClass() { return NULL; }

	IDirect3DTexture8* Peek_D3D_Texture() const { return (IDirect3DTexture8*)Peek_D3D_Base_Texture(); }
	IDirect3DVolumeTexture8* Peek_D3D_VolumeTexture() const { return (IDirect3DVolumeTexture8*)Peek_D3D_Base_Texture(); }
	IDirect3DCubeTexture8* Peek_D3D_CubeTexture() const { return (IDirect3DCubeTexture8*)Peek_D3D_Base_Texture(); }
#endif

	protected:
#ifdef OG
		// Apply this texture's settings into D3D
		virtual void Apply(unsigned int stage);
#endif
		void Load_Locked_Surface();
#ifdef ZH
	void Poke_Texture(IDirect3DBaseTexture8* tex) { D3DTexture = tex; }

	bool Initialized;

	// For debug purposes the texture sets this true if it is a lightmap texture
	bool IsLightmap;
	bool IsCompressionAllowed;
	bool IsProcedural;
	bool IsReducible;

	unsigned InactivationTime;	// In milliseconds
	unsigned ExtendedInactivationTime;	// This is set by the engine, if needed
	unsigned LastInactivationSyncTime;
	mutable unsigned LastAccessed;
#endif

#ifdef OG
		void Init();

#endif
#ifdef ZH
	// If this is non-zero, the texture will have a hue shift done at the next init (this
	// value should only be changed by Set_HSV_Shift() function, which also invalidates the
	// texture).
	Vector3 HSVShift;	
#endif

#ifdef OG
		// Apply a Null texture's settings into D3D
		static void Apply_Null(unsigned int stage);
#endif
#ifdef ZH
	int Width;
	int Height;
#endif

#ifdef OG
		// State not contained in the Direct3D texture object:
		FilterType TextureMinFilter;
		FilterType TextureMagFilter;
		FilterType MipMapFilter;
		TxtAddrMode UAddressMode;
		TxtAddrMode VAddressMode;
#endif
#ifdef ZH
private:

#endif

		// Direct3D texture object
#ifdef OG
		IDirect3DTexture8 *D3DTexture;
		bool Initialized;
#endif
#ifdef ZH
	IDirect3DBaseTexture8 *D3DTexture;

#endif

		// Name
		StringClass Name;
		StringClass	FullPath;

		// Unique id
		unsigned texture_id;
#ifdef ZH

	// Support for self-managed textures

	PoolType Pool;
	bool Dirty;

	friend class TextureLoadTaskClass;
	friend class CubeTextureLoadTaskClass;
	friend class VolumeTextureLoadTaskClass;
	TextureLoadTaskClass* TextureLoadTask;
	TextureLoadTaskClass* ThumbnailLoadTask;

};

/*************************************************************************
**                             TextureClass
**
** This is our regular texture class. For legacy reasons it contains some
** information beyond the D3D texture itself, such as texture addressing
** modes.
**
*************************************************************************/
class TextureClass : public TextureBaseClass
{
	W3DMPO_GLUE(TextureClass)
//	friend DX8Wrapper;

public:

	// Create texture with desired height, width and format.
	TextureClass
	(
		unsigned width, 
		unsigned height, 
		WW3DFormat format,
		MipCountType mip_level_count=MIP_LEVELS_ALL,
		PoolType pool=POOL_MANAGED,
		bool rendertarget=false,
		bool allow_reduction=true
	);

	// Create texture from a file. If format is specified the texture is converted to that format.
	// Note that the format must be supported by the current device and that a texture can't exist
	// in the system with the same name in multiple formats.
	TextureClass
	(
		const char *name,
		const char *full_path=NULL,
		MipCountType mip_level_count=MIP_LEVELS_ALL,
		WW3DFormat texture_format=WW3D_FORMAT_UNKNOWN,
		bool allow_compression=true,
		bool allow_reduction=true
	);

	// Create texture from a surface.
	TextureClass
	(
		SurfaceClass *surface, 
		MipCountType mip_level_count=MIP_LEVELS_ALL
	);		

	TextureClass(IDirect3DBaseTexture8* d3d_texture);
#endif

#ifdef OG
		// NOTE: Since "texture wrapping" (NOT TEXTURE WRAP MODE - THIS IS
		// SOMETHING ELSE) is a global state that affects all texture stages,
		// and this class only affects its own stage, we will not worry about
		// it for now. Later (probably when we implement world-oriented
		// environment maps) we will consider where to put it.

#endif
#ifdef ZH
	// defualt constructors for derived classes (cube & vol)
	TextureClass
	(
		unsigned width,
		unsigned height,
		MipCountType mip_level_count=MIP_LEVELS_ALL,
		PoolType pool=POOL_MANAGED,
		bool rendertarget=false,
		WW3DFormat format=WW3D_FORMAT_UNKNOWN,
		bool allow_reduction=true
	)
	: TextureBaseClass(width,height,mip_level_count,pool,rendertarget,allow_reduction), TextureFormat(format), Filter(mip_level_count) { }

	virtual TexAssetType Get_Asset_Type() const { return TEX_REGULAR; }

	virtual void Init();

	// Background texture loader will call this when texture has been loaded
	virtual void Apply_New_Surface(IDirect3DBaseTexture8* tex, bool initialized, bool disable_auto_invalidation = false);	// If the parameter is true, the texture will be flagged as initialised

	// Get the surface of one of the mipmap levels (defaults to highest-resolution one)
	SurfaceClass *Get_Surface_Level(unsigned int level = 0);
	IDirect3DSurface8 *Get_D3D_Surface_Level(unsigned int level = 0);
	void Get_Level_Description( SurfaceClass::SurfaceDescription & desc, unsigned int level = 0 );
	
	TextureFilterClass& Get_Filter() { return Filter; }

	WW3DFormat Get_Texture_Format() const { return TextureFormat; }

	virtual void Apply(unsigned int stage);

	virtual unsigned Get_Texture_Memory_Usage() const;

	virtual TextureClass* As_TextureClass() { return this; }
#endif

#ifdef OG
		// For debug purposes the texture sets this true if it is a lightmap texture
		bool IsLightmap;
		bool IsProcedural;
		bool IsCompressionAllowed;
#endif
#ifdef ZH
protected:

#endif

#ifdef OG
		unsigned LastAccessed;
		WW3DFormat TextureFormat;
#endif
#ifdef ZH
	WW3DFormat				TextureFormat;

#endif

#ifdef OG
		// Support for self-managed textures

#endif
#ifdef ZH
	// legacy
	TextureFilterClass	Filter;
};
#endif

#ifdef OG
		PoolType Pool;
		bool Dirty;
#endif
#ifdef ZH
class ZTextureClass : public TextureBaseClass
{
#endif
public:
#ifdef OG
		MipCountType MipLevelCount;
		TextureLoadTaskClass* TextureLoadTask;

#endif
#ifdef ZH
	// Create a z texture with desired height, width and format
	ZTextureClass
	(
		unsigned width,
		unsigned height,
		WW3DZFormat zformat,
		MipCountType mip_level_count=MIP_LEVELS_ALL,
		PoolType pool=POOL_MANAGED
	);

	WW3DZFormat Get_Texture_Format() const { return DepthStencilTextureFormat; }

	virtual TexAssetType Get_Asset_Type() const { return TEX_REGULAR; }

	virtual void Init() {}

#endif
		// Background texture loader will call this when texture has been loaded
#ifdef OG
		void Apply_New_Surface(bool initialized);	// If the parameter is true, the texture will be flagged as initialised

#endif
#ifdef ZH
	virtual void Apply_New_Surface(IDirect3DBaseTexture8* tex, bool initialized, bool disable_auto_invalidation = false);	// If the parameter is true, the texture will be flagged as initialised

	virtual void Apply(unsigned int stage);

	IDirect3DSurface8 *Get_D3D_Surface_Level(unsigned int level = 0);
	virtual unsigned Get_Texture_Memory_Usage() const;

private:

	WW3DZFormat DepthStencilTextureFormat;
};

class CubeTextureClass : public TextureClass
{
public:
	// Create texture with desired height, width and format.
	CubeTextureClass
	(
		unsigned width, 
		unsigned height, 
		WW3DFormat format,
		MipCountType mip_level_count=MIP_LEVELS_ALL,
		PoolType pool=POOL_MANAGED,
		bool rendertarget=false,
		bool allow_reduction=true
	);

	// Create texture from a file. If format is specified the texture is converted to that format.
	// Note that the format must be supported by the current device and that a texture can't exist
	// in the system with the same name in multiple formats.
	CubeTextureClass
	(
		const char *name,
		const char *full_path=NULL,
		MipCountType mip_level_count=MIP_LEVELS_ALL,
		WW3DFormat texture_format=WW3D_FORMAT_UNKNOWN,
		bool allow_compression=true,
		bool allow_reduction=true
	);

	// Create texture from a surface.
	CubeTextureClass
	(
		SurfaceClass *surface, 
		MipCountType mip_level_count=MIP_LEVELS_ALL
	);		

	CubeTextureClass(IDirect3DBaseTexture8* d3d_texture);

	virtual void Apply_New_Surface(IDirect3DBaseTexture8* tex, bool initialized, bool disable_auto_invalidation = false);	// If the parameter is true, the texture will be flagged as initialised

	virtual TexAssetType Get_Asset_Type() const { return TEX_CUBEMAP; }

	virtual CubeTextureClass* As_CubeTextureClass() { return this; }
#endif

};

#ifdef OG
class BumpmapTextureClass : public TextureClass
#endif
#ifdef ZH
class VolumeTextureClass : public TextureClass
#endif
{
public:
#ifdef OG
	// Generate bumpmap texture procedurally from the source texture
	BumpmapTextureClass(TextureClass* texture);
	virtual ~BumpmapTextureClass();

#endif
#ifdef ZH
	// Create texture with desired height, width and format.
	VolumeTextureClass
	(
		unsigned width, 
		unsigned height, 
		unsigned depth,
		WW3DFormat format,
		MipCountType mip_level_count=MIP_LEVELS_ALL,
		PoolType pool=POOL_MANAGED,
		bool rendertarget=false,
		bool allow_reduction=true
	);

	// Create texture from a file. If format is specified the texture is converted to that format.
	// Note that the format must be supported by the current device and that a texture can't exist
	// in the system with the same name in multiple formats.
	VolumeTextureClass
	(
		const char *name,
		const char *full_path=NULL,
		MipCountType mip_level_count=MIP_LEVELS_ALL,
		WW3DFormat texture_format=WW3D_FORMAT_UNKNOWN,
		bool allow_compression=true,
		bool allow_reduction=true
	);

	// Create texture from a surface.
	VolumeTextureClass
	(
		SurfaceClass *surface, 
		MipCountType mip_level_count=MIP_LEVELS_ALL
	);		

	VolumeTextureClass(IDirect3DBaseTexture8* d3d_texture);

	virtual void Apply_New_Surface(IDirect3DBaseTexture8* tex, bool initialized, bool disable_auto_invalidation = false);	// If the parameter is true, the texture will be flagged as initialised

	virtual TexAssetType Get_Asset_Type() const { return TEX_VOLUME; }

	virtual VolumeTextureClass* As_VolumeTextureClass() { return this; }

protected:

	int Depth;
#endif
};

// Utility functions for loading and saving texture descriptions from/to W3D files
TextureClass *Load_Texture(ChunkLoadClass & cload);
void Save_Texture(TextureClass * texture, ChunkSaveClass & csave);

#endif //TEXTURE_H
