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
 *                     $Archive:: /Commando/Code/ww3d2/texture.cpp                            $*
 *                                                                                             *
#ifdef OG
 *                      $Author:: Jani_p                                                      $*
#endif
#ifdef ZH
 *                  $Org Author:: Steve_t                                                     $*
#endif
 *                                                                                             *
#ifdef OG
 *                     $Modtime:: 8/29/01 7:06p                                               $*
#endif
#ifdef ZH
 *                       Author : Kenny Mitchell                                               * 
#endif
 *                                                                                             *
#ifdef OG
 *                    $Revision:: 65                                                          $*
#endif
#ifdef ZH
 *                     $Modtime:: 08/05/02 1:27p                                              $*
#endif
 *                                                                                             *
#ifdef ZH
 *                    $Revision:: 85                                                          $*
 *                                                                                             *
 * 06/27/02 KM Texture class abstraction																			*
 * 08/05/02 KM Texture class redesign (revisited)
#endif
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   FileListTextureClass::Load_Frame_Surface -- Load source texture                           * 
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "texture.h"

#include <d3d8.h>
#include <stdio.h>
#include <D3dx8core.h>
#include "dx8wrapper.h"
#include "targa.h"
#include <nstrdup.h>
#include "w3d_file.h"
#include "assetmgr.h"
#include "formconv.h"
#include "textureloader.h"
#include "missingtexture.h"
#include "ffactory.h"
#include "dx8caps.h"
#include "dx8texman.h"
#include "meshmatdesc.h"
#ifdef ZH
#include "texturethumbnail.h"
#include "wwprofile.h"

//#pragma optimize("", off)
//#pragma MESSAGE("************************************** WARNING, optimization disabled for debugging purposes")
#endif

#ifdef ZH
const unsigned DEFAULT_INACTIVATION_TIME=20000;

#endif
/*
** Definitions of static members:
*/

static unsigned unused_texture_id;

#ifdef OG
unsigned _MinTextureFilters[TextureClass::FILTER_TYPE_COUNT];
unsigned _MagTextureFilters[TextureClass::FILTER_TYPE_COUNT];
unsigned _MipMapFilters[TextureClass::FILTER_TYPE_COUNT];
#endif
#ifdef ZH
// This throttles submissions to the background texture loading queue.
static unsigned TexturesAppliedPerFrame;
const unsigned MAX_TEXTURES_APPLIED_PER_FRAME=2;
#endif

#ifdef OG
// ----------------------------------------------------------------------------
#endif

#ifdef OG
static int Calculate_Texture_Memory_Usage(const TextureClass* texture,int red_factor=0)
{
	// Set performance statistics

	int size=0;
	IDirect3DTexture8* d3d_texture=const_cast<TextureClass*>(texture)->Peek_DX8_Texture();
	if (!d3d_texture) return 0;
	for (unsigned i=red_factor;i<d3d_texture->GetLevelCount();++i) {
		D3DSURFACE_DESC desc;
		DX8_ErrorCode(d3d_texture->GetLevelDesc(i,&desc));
		size+=desc.Size;
	}
	return size;
}

/*************************************************************************
**                             TextureClass
*************************************************************************/

TextureClass::TextureClass(unsigned width, unsigned height, WW3DFormat format, MipCountType mip_level_count, PoolType pool,bool rendertarget)
	:
#endif
#ifdef ZH
/*!
 * KM General base constructor for texture classes
 */
TextureBaseClass::TextureBaseClass
(
	unsigned int width,
	unsigned int height,
	enum MipCountType mip_level_count,
	enum PoolType pool,
	bool rendertarget,
	bool reducible
)
:	MipLevelCount(mip_level_count),

#endif
	D3DTexture(NULL),
#ifdef ZH
	Initialized(false),
   Name(""),
	FullPath(""),
#endif
	texture_id(unused_texture_id++),
#ifdef OG
	Initialized(true),
	TextureMinFilter(FILTER_TYPE_DEFAULT),
	TextureMagFilter(FILTER_TYPE_DEFAULT),
	MipMapFilter((mip_level_count!=MIP_LEVELS_1) ? FILTER_TYPE_DEFAULT : FILTER_TYPE_NONE),
	UAddressMode(TEXTURE_ADDRESS_REPEAT),
	VAddressMode(TEXTURE_ADDRESS_REPEAT),
	MipLevelCount(mip_level_count),

#endif
#ifdef ZH
	IsLightmap(false),
	IsProcedural(false),
	IsReducible(reducible),
	IsCompressionAllowed(false),
	InactivationTime(0),
	ExtendedInactivationTime(0),
	LastInactivationSyncTime(0),
	LastAccessed(0),
	Width(width),
	Height(height),
#endif
	Pool(pool),
	Dirty(false),
#ifdef OG
	IsLightmap(false),
	IsProcedural(true),
	Name(""),
	TextureFormat(format),
	IsCompressionAllowed(false),
	TextureLoadTask(NULL)
#endif
#ifdef ZH
	TextureLoadTask(NULL),
	ThumbnailLoadTask(NULL),
	HSVShift(0.0f,0.0f,0.0f)

#endif
{
#ifdef OG
	switch (format) {
	case WW3D_FORMAT_DXT1:
	case WW3D_FORMAT_DXT2:
	case WW3D_FORMAT_DXT3:
	case WW3D_FORMAT_DXT4:
	case WW3D_FORMAT_DXT5:
		IsCompressionAllowed=true;
		break;
	default:
		break;
#endif
	}
#ifdef OG
	D3DPOOL d3dpool=(D3DPOOL) 0;
	switch(pool)

#endif
#ifdef ZH


//**********************************************************************************************
//! Base texture class destructor
/*! KJM
*/
TextureBaseClass::~TextureBaseClass(void)
#endif
	{
#ifdef OG
	case POOL_DEFAULT:
		d3dpool=D3DPOOL_DEFAULT;		
		break;
	case POOL_MANAGED:
		d3dpool=D3DPOOL_MANAGED;
		break;
	case POOL_SYSTEMMEM:
		d3dpool=D3DPOOL_SYSTEMMEM;
		break;
	default:
		WWASSERT(0);		
	}
	D3DTexture = DX8Wrapper::_Create_DX8_Texture(width, height, format, mip_level_count,d3dpool,rendertarget);
	if (pool==POOL_DEFAULT)
#endif
#ifdef ZH
	delete TextureLoadTask;
	TextureLoadTask=NULL;
	delete ThumbnailLoadTask;
	ThumbnailLoadTask=NULL;

	if (D3DTexture) 

#endif
	{
#ifdef OG
		Dirty=true;
		DX8TextureTrackerClass *track=W3DNEW
		DX8TextureTrackerClass(width, height, format, mip_level_count,rendertarget,
		this);
		DX8TextureManagerClass::Add(track);
#endif
#ifdef ZH
		D3DTexture->Release();
		D3DTexture = NULL;

#endif
	}
#ifdef OG
	LastAccessed=WW3D::Get_Sync_Time();

#endif
#ifdef ZH

	DX8TextureManagerClass::Remove(this);
#endif
}

#ifdef OG
// ----------------------------------------------------------------------------
#endif

#ifdef OG
TextureClass::TextureClass(
	const char *name, 
	const char *full_path, 
	MipCountType mip_level_count,
	WW3DFormat texture_format,
	bool allow_compression)
	:
	D3DTexture(NULL),
	texture_id(unused_texture_id++),
	Initialized(false),
	TextureMinFilter(FILTER_TYPE_DEFAULT),
	TextureMagFilter(FILTER_TYPE_DEFAULT),
	MipMapFilter((mip_level_count!=MIP_LEVELS_1) ? FILTER_TYPE_DEFAULT : FILTER_TYPE_NONE),
	UAddressMode(TEXTURE_ADDRESS_REPEAT),
	VAddressMode(TEXTURE_ADDRESS_REPEAT),
	MipLevelCount(mip_level_count),
	Pool(POOL_MANAGED),
	Dirty(false),
	IsLightmap(false),
	IsProcedural(false),
	TextureFormat(texture_format),
	IsCompressionAllowed(allow_compression),
	TextureLoadTask(NULL)
#endif
#ifdef ZH
//**********************************************************************************************
//! Invalidate old unused textures
/*! 
*/
void TextureBaseClass::Invalidate_Old_Unused_Textures(unsigned invalidation_time_override)

#endif
{
#ifdef OG
	switch (TextureFormat) {
	case WW3D_FORMAT_DXT1:
	case WW3D_FORMAT_DXT2:
	case WW3D_FORMAT_DXT3:
	case WW3D_FORMAT_DXT4:
	case WW3D_FORMAT_DXT5:
		IsCompressionAllowed=true;
		break;
	case WW3D_FORMAT_U8V8:		// Bumpmap
	case WW3D_FORMAT_L6V5U5:	// Bumpmap
	case WW3D_FORMAT_X8L8V8U8:	// Bumpmap
		// If requesting bumpmap format that isn't available we'll just return the surface in whatever color
		// format the texture file is in. (This is illegal case, the format support should always be queried
		// before creating a bump texture!)
		if (!DX8Caps::Support_Texture_Format(TextureFormat)) {
			TextureFormat=WW3D_FORMAT_UNKNOWN;
#endif
#ifdef ZH
	// (gth) If thumbnails are not enabled, then we don't run this code.
	if (WW3D::Get_Thumbnail_Enabled() == false) {
		return;

#endif
		}
#ifdef OG
		// If bump format is valid, make sure compression is not allowed so that we don't even attempt to load
		// from a compressed file (quality isn't good enough for bump map). Also disable mipmapping.
		else {
			IsCompressionAllowed=false;
			MipLevelCount=MIP_LEVELS_1;
			MipMapFilter=FILTER_TYPE_NONE;
		}
		break;
#endif
#ifdef ZH

	// Zero the texture apply count in this function because this is called every frame...(this wasn't in E&B main branch KJM)
	TexturesAppliedPerFrame=0;

	unsigned synctime=WW3D::Get_Sync_Time();
	HashTemplateIterator<StringClass,TextureClass*> ite(WW3DAssetManager::Get_Instance()->Texture_Hash());
	// Loop through all the textures in the manager

#endif

#ifdef OG
	default:
		break;
	}
#endif
#ifdef ZH
	for (ite.First ();!ite.Is_Done();ite.Next ()) 
	{
		TextureClass* tex=ite.Peek_Value();
#endif

#ifdef OG
	WWASSERT_PRINT(name && name[0], "TextureClass CTor: NULL or empty texture name\n");
	int len=strlen(name);
	for (int i=0;i<len;++i) {
		if (name[i]=='+') {
			IsLightmap=true;
#endif
#ifdef ZH
		// Consider invalidating if texture has been initialized and defines inactivation time
		if (tex->Initialized && tex->InactivationTime) 
		{
			unsigned age=synctime-tex->LastAccessed;

#endif

#ifdef OG
			// Set bilinear filtering for lightmaps (they are very stretched and
			// low detail so we don't care for anisotropic or trilinear filtering...)
			TextureMinFilter=FILTER_TYPE_FAST;
			TextureMagFilter=FILTER_TYPE_FAST;
			if (mip_level_count!=MIP_LEVELS_1) MipMapFilter=FILTER_TYPE_FAST;
			break;

#endif
#ifdef ZH
			if (invalidation_time_override) 
			{
				if (age>invalidation_time_override) 
				{
					tex->Invalidate();
					tex->LastInactivationSyncTime=synctime;
				}
#endif
		}
#ifdef ZH
			else 
			{
				// Not used in the last n milliseconds?
				if (age>(tex->InactivationTime+tex->ExtendedInactivationTime)) 
				{
					tex->Invalidate();
					tex->LastInactivationSyncTime=synctime;
#endif
	}
#ifdef OG
	Set_Texture_Name(name);
	Set_Full_Path(full_path);
	WWASSERT(name[0]!='\0');
	if (!WW3D::Is_Texturing_Enabled()) {
		Initialized=true;
		D3DTexture=0;
#endif
	}
#ifdef OG
	else if (WW3D::Get_Texture_Thumbnail_Mode()==WW3D::TEXTURE_THUMBNAIL_MODE_OFF || mip_level_count==MIP_LEVELS_1)
	{
		Initialized=true;
		D3DTexture=0;
		TextureLoader::Request_High_Priority_Loading(this,mip_level_count);
#endif
	}
#ifdef OG
	else {
		Load_Locked_Surface();
		TextureFormat=texture_format;	// Locked surface may be in a wrong format
#endif
	}
#ifdef OG
	LastAccessed=WW3D::Get_Sync_Time();
#endif
}

#ifdef OG
// ----------------------------------------------------------------------------

#endif

#ifdef OG
TextureClass::TextureClass(SurfaceClass *surface, MipCountType mip_level_count)
	:
	D3DTexture(NULL),
	texture_id(unused_texture_id++),
	Initialized(true),
	TextureMinFilter(FILTER_TYPE_DEFAULT),
	TextureMagFilter(FILTER_TYPE_DEFAULT),
	MipMapFilter((mip_level_count!=MIP_LEVELS_1) ? FILTER_TYPE_DEFAULT : FILTER_TYPE_NONE),
	UAddressMode(TEXTURE_ADDRESS_REPEAT),
	VAddressMode(TEXTURE_ADDRESS_REPEAT),
	MipLevelCount(mip_level_count),
	Pool(POOL_MANAGED),
	Dirty(false),
	IsLightmap(false),
	Name(""),
	IsProcedural(true),
	TextureFormat(surface->Get_Surface_Format()),
	IsCompressionAllowed(false),
	TextureLoadTask(NULL)
#endif
#ifdef ZH

//**********************************************************************************************
//! Invalidate this texture
/*! 
*/
void TextureBaseClass::Invalidate()

#endif
{
#ifdef OG
	SurfaceClass::SurfaceDescription sd;
	surface->Get_Description(sd);
	switch (sd.Format) {
	case WW3D_FORMAT_DXT1:
	case WW3D_FORMAT_DXT2:
	case WW3D_FORMAT_DXT3:
	case WW3D_FORMAT_DXT4:
	case WW3D_FORMAT_DXT5:
		IsCompressionAllowed=true;
		break;
	default:
		break;
#endif
#ifdef ZH
	if (TextureLoadTask) {
		return;
	}
	if (ThumbnailLoadTask) {
		return;

#endif
	}
	
#ifdef OG
	D3DTexture = DX8Wrapper::_Create_DX8_Texture(surface->Peek_D3D_Surface(), mip_level_count);
	LastAccessed=WW3D::Get_Sync_Time();

#endif
#ifdef ZH
	// Don't invalidate procedural textures
	if (IsProcedural) {
		return;
#endif
}

#ifdef OG
// ----------------------------------------------------------------------------

TextureClass::TextureClass(IDirect3DTexture8* d3d_texture)
	:
	D3DTexture(d3d_texture),
	texture_id(unused_texture_id++),
	Initialized(true),
	TextureMinFilter(FILTER_TYPE_DEFAULT),
	TextureMagFilter(FILTER_TYPE_DEFAULT),
	MipMapFilter((d3d_texture->GetLevelCount()!=1) ? FILTER_TYPE_DEFAULT : FILTER_TYPE_NONE),
	UAddressMode(TEXTURE_ADDRESS_REPEAT),
	VAddressMode(TEXTURE_ADDRESS_REPEAT),
	MipLevelCount((MipCountType)d3d_texture->GetLevelCount()),
	Pool(POOL_MANAGED),
	Dirty(false),
	IsLightmap(false),
	Name(""),
	IsProcedural(true),
	IsCompressionAllowed(false),
	TextureLoadTask(NULL)
#endif
#ifdef ZH
	if (D3DTexture) 

#endif
{
#ifdef OG
	D3DTexture->AddRef();
	IDirect3DSurface8* surface;
	DX8_ErrorCode(D3DTexture->GetSurfaceLevel(0,&surface));
	D3DSURFACE_DESC d3d_desc;
	::ZeroMemory(&d3d_desc, sizeof(D3DSURFACE_DESC));
	DX8_ErrorCode(surface->GetDesc(&d3d_desc));
	TextureFormat=D3DFormat_To_WW3DFormat(d3d_desc.Format);
	switch (TextureFormat) {
	case WW3D_FORMAT_DXT1:
	case WW3D_FORMAT_DXT2:
	case WW3D_FORMAT_DXT3:
	case WW3D_FORMAT_DXT4:
	case WW3D_FORMAT_DXT5:
		IsCompressionAllowed=true;
		break;
	default:
		break;
#endif
#ifdef ZH
		D3DTexture->Release();
		D3DTexture = NULL;

#endif
	}
#ifdef ZH

	Initialized=false;
#endif
	
	LastAccessed=WW3D::Get_Sync_Time();
#ifdef ZH
/*	was battlefield version// If the texture has already been initialised we should exit now
	if (Initialized) return;

	WWPROFILE(("TextureClass::Init()"));

	// If the texture has recently been inactivated, increase the inactivation time (this texture obviously
	// should not have been inactivated yet).

	if (InactivationTime && LastInactivationSyncTime) {
		if ((WW3D::Get_Sync_Time()-LastInactivationSyncTime)<InactivationTime) {
			ExtendedInactivationTime=3*InactivationTime;
		}
		LastInactivationSyncTime=0;
#endif
}

#ifdef OG
// ----------------------------------------------------------------------------

#endif
#ifdef ZH
	if (ThumbnailLoadTask) 
	{
		return;
	}
#endif

#ifdef OG
TextureClass::~TextureClass(void)

#endif
#ifdef ZH
	// Don't invalidate procedural textures
	if (IsProcedural) 
#endif
{
#ifdef OG
	TextureLoadTaskClass::Release_Instance(TextureLoadTask);
	TextureLoadTask=NULL;

	if (!Initialized) {
		WWDEBUG_SAY(("Warning: Texture %s was loaded but never used\n",Get_Texture_Name()));
#endif
#ifdef ZH
		return;

#endif
	}

#ifdef OG
	if (D3DTexture) {

#endif
#ifdef ZH
	if (D3DTexture) 
	{
#endif
		D3DTexture->Release();
		D3DTexture = NULL;
	}
#ifdef OG
	DX8TextureManagerClass::Remove(this);

#endif
#ifdef ZH

	Initialized=false;

	LastAccessed=WW3D::Get_Sync_Time();*/
#endif
}

#ifdef OG
// ----------------------------------------------------------------------------

void TextureClass::Init()

#endif
#ifdef ZH
//**********************************************************************************************
//! Returns a pointer to the d3d texture
/*! 
*/
IDirect3DBaseTexture8 * TextureBaseClass::Peek_D3D_Base_Texture() const 
#endif
{
#ifdef OG
	// If the texture has already been initialised we should exit now
	if (Initialized) return;

	TextureLoader::Add_Load_Task(this);
#endif
	LastAccessed=WW3D::Get_Sync_Time();
#ifdef ZH
	return D3DTexture; 
#endif
}

#ifdef OG
void TextureClass::Invalidate()

#endif
#ifdef ZH
//**********************************************************************************************
//! Set the d3d texture pointer.  Handles ref counts properly.
/*! 
*/
void TextureBaseClass::Set_D3D_Base_Texture(IDirect3DBaseTexture8* tex) 
#endif
{
#ifdef OG
	// Don't invalidate procedural textures
	if (IsProcedural) return;

#endif
#ifdef ZH
	// (gth) Generals does stuff directly with the D3DTexture pointer so lets
	// reset the access timer whenever someon messes with this pointer.
	LastAccessed=WW3D::Get_Sync_Time();
#endif

#ifdef OG
	// Don't invalidate missing texture
	if (Is_Missing_Texture()) return;

	if (D3DTexture) {
#endif
#ifdef ZH
	if (D3DTexture != NULL) {

#endif
		D3DTexture->Release();
#ifdef OG
		D3DTexture = NULL;
#endif
	}
#ifdef OG

	if (!WW3D::Is_Texturing_Enabled()) {
		Initialized=true;
		D3DTexture=0;
	}
	else if (WW3D::Get_Texture_Thumbnail_Mode()==WW3D::TEXTURE_THUMBNAIL_MODE_OFF || MipLevelCount==MIP_LEVELS_1)
	{
		Initialized=true;
		D3DTexture=0;
		TextureLoader::Request_High_Priority_Loading(this,MipLevelCount);
	}
	else {
		Initialized=false;
		Load_Locked_Surface();
#endif
#ifdef ZH
	D3DTexture = tex;
	if (D3DTexture != NULL) {
		D3DTexture->AddRef();

#endif
	}
}

#ifdef OG
// ----------------------------------------------------------------------------
#endif

#ifdef OG
void TextureClass::Load_Locked_Surface()

#endif
#ifdef ZH
//**********************************************************************************************
//! Load locked surface
/*! 
*/
void TextureBaseClass::Load_Locked_Surface()
#endif
{
#ifdef ZH
	WWPROFILE(("TextureClass::Load_Locked_Surface()"));
#endif
	if (D3DTexture) D3DTexture->Release();
	D3DTexture=0;
	TextureLoader::Request_Thumbnail(this);
	Initialized=false;
}

#ifdef OG
// ----------------------------------------------------------------------------
#endif

#ifdef OG
bool TextureClass::Is_Missing_Texture()

#endif
#ifdef ZH
//**********************************************************************************************
//! Is missing texture
/*! 
*/
bool TextureBaseClass::Is_Missing_Texture()
#endif
{
	bool flag = false;
#ifdef OG
	IDirect3DTexture8 *missing_texture = MissingTexture::_Get_Missing_Texture();
#endif
#ifdef ZH
	IDirect3DBaseTexture8 *missing_texture = MissingTexture::_Get_Missing_Texture();
#endif
	
	if(D3DTexture == missing_texture)
		flag = true;

	if(missing_texture)
#ifdef ZH
	{
#endif
		missing_texture->Release();
#ifdef ZH
	}
#endif

	return flag;
}

#ifdef OG
// ----------------------------------------------------------------------------
#endif

#ifdef OG
void TextureClass::Set_Texture_Name(const char * name)

#endif
#ifdef ZH
//**********************************************************************************************
//! Set texture name
/*! 
*/
void TextureBaseClass::Set_Texture_Name(const char * name)
#endif
{
	Name=name;
}

#ifdef OG
// ----------------------------------------------------------------------------
#endif

#ifdef OG
unsigned int TextureClass::Get_Mip_Level_Count(void)
{
	if (!D3DTexture) {
		WWASSERT_PRINT(0, "Get_Mip_Level_Count: D3DTexture is NULL!\n");
		return 0;
	}
#endif

#ifdef OG
	return D3DTexture->GetLevelCount();
}
#endif

#ifdef OG
// ----------------------------------------------------------------------------

void TextureClass::Get_Level_Description(SurfaceClass::SurfaceDescription &surface_desc, unsigned int level)

#endif
#ifdef ZH
//**********************************************************************************************
//! Get priority
/*! 
*/
unsigned int TextureBaseClass::Get_Priority(void)
#endif
{
#ifdef OG
	if (!D3DTexture) {
		WWASSERT_PRINT(0, "Get_Surface_Description: D3DTexture is NULL!\n");
	}

	D3DSURFACE_DESC d3d_surf_desc;
	DX8_ErrorCode(D3DTexture->GetLevelDesc(level, &d3d_surf_desc));
	surface_desc.Format = D3DFormat_To_WW3DFormat(d3d_surf_desc.Format);
	surface_desc.Height = d3d_surf_desc.Height; 
	surface_desc.Width = d3d_surf_desc.Width;
}

// ----------------------------------------------------------------------------

SurfaceClass *TextureClass::Get_Surface_Level(unsigned int level)
{
	IDirect3DSurface8 *d3d_surface = NULL;
	DX8_ErrorCode(D3DTexture->GetSurfaceLevel(level, &d3d_surface));
	SurfaceClass *surface = W3DNEW SurfaceClass(d3d_surface);
	d3d_surface->Release();
	return surface;
}

// ----------------------------------------------------------------------------

unsigned int TextureClass::Get_Priority(void)
#endif
#ifdef ZH
	if (!D3DTexture) 

#endif
{
#ifdef OG
	if (!D3DTexture) {
#endif
		WWASSERT_PRINT(0, "Get_Priority: D3DTexture is NULL!\n");
		return 0;
	}

#ifdef ZH
#ifndef _XBOX
#endif
	return D3DTexture->GetPriority();
#ifdef ZH
#else
	return 0;
#endif
#endif
}

#ifdef OG
// ----------------------------------------------------------------------------
#endif

#ifdef OG
unsigned int TextureClass::Set_Priority(unsigned int priority)

#endif
#ifdef ZH
//**********************************************************************************************
//! Set priority
/*! 
*/
unsigned int TextureBaseClass::Set_Priority(unsigned int priority)
#endif
{
#ifdef OG
	if (!D3DTexture) {

#endif
#ifdef ZH
	if (!D3DTexture) 
	{
#endif
		WWASSERT_PRINT(0, "Set_Priority: D3DTexture is NULL!\n");
		return 0;
	}

#ifdef ZH
#ifndef _XBOX
#endif
	return D3DTexture->SetPriority(priority);
#ifdef ZH
#else
	return 0;
#endif
#endif
}

#ifdef OG
// ----------------------------------------------------------------------------
#endif

#ifdef OG
void TextureClass::Set_Mip_Mapping(FilterType mipmap)

#endif
#ifdef ZH
//**********************************************************************************************
//! Get reduction mip levels
/*! 
*/
unsigned TextureBaseClass::Get_Reduction() const
#endif
{
#ifdef OG
	if (mipmap != FILTER_TYPE_NONE && Get_Mip_Level_Count() <= 1) {
		WWASSERT_PRINT(0, "Trying to enable MipMapping on texture w/o Mip levels!\n");
		return;
	}
	MipMapFilter=mipmap;
}

unsigned TextureClass::Get_Reduction() const
{
#endif
#ifdef ZH
	// don't reduce if the texture is too small already or
	// has no mip map levels

#endif
	if (MipLevelCount==MIP_LEVELS_1) return 0;
#ifdef ZH
	if (Width <= 32 || Height <= 32) return 0;
#endif

	int reduction=WW3D::Get_Texture_Reduction();
#ifdef ZH

	// 'large texture extra reduction' causes textures above 256x256 to be reduced one more step.
	if (WW3D::Is_Large_Texture_Extra_Reduction_Enabled() && (Width > 256 || Height > 256)) {
		reduction++;
	}
#endif
	if (MipLevelCount && reduction>MipLevelCount) {
		reduction=MipLevelCount;
	}
	return reduction;
#ifdef OG
}

// ----------------------------------------------------------------------------

void TextureClass::Apply(unsigned int stage)
{
	if (!Initialized) {
		Init();
#endif
	}
#ifdef OG
	LastAccessed=WW3D::Get_Sync_Time();
#endif

#ifdef OG
	DX8_RECORD_TEXTURE(this);
#endif

#ifdef OG
	// Set texture itself
	if (WW3D::Is_Texturing_Enabled()) {
		DX8Wrapper::Set_DX8_Texture(stage, D3DTexture);
	} else {
		DX8Wrapper::Set_DX8_Texture(stage, NULL);
	}
#endif

#ifdef OG
	DX8Wrapper::Set_DX8_Texture_Stage_State(stage,D3DTSS_MINFILTER,_MinTextureFilters[TextureMinFilter]);
	DX8Wrapper::Set_DX8_Texture_Stage_State(stage,D3DTSS_MAGFILTER,_MagTextureFilters[TextureMagFilter]);
	DX8Wrapper::Set_DX8_Texture_Stage_State(stage,D3DTSS_MIPFILTER,_MipMapFilters[MipMapFilter]);

	switch (Get_U_Addr_Mode()) {

		case TEXTURE_ADDRESS_REPEAT:
			DX8Wrapper::Set_DX8_Texture_Stage_State(stage, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
			break;

		case TEXTURE_ADDRESS_CLAMP:
			DX8Wrapper::Set_DX8_Texture_Stage_State(stage, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
			break;

	}

	switch (Get_V_Addr_Mode()) {

		case TEXTURE_ADDRESS_REPEAT:
			DX8Wrapper::Set_DX8_Texture_Stage_State(stage, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
			break;

		case TEXTURE_ADDRESS_CLAMP:
			DX8Wrapper::Set_DX8_Texture_Stage_State(stage, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);
			break;

	}
}

// ----------------------------------------------------------------------------

void TextureClass::Apply_Null(unsigned int stage)
#endif
#ifdef ZH
//**********************************************************************************************
//! Apply NULL texture state
/*! 
*/
void TextureBaseClass::Apply_Null(unsigned int stage)

#endif
{
	// This function sets the render states for a "NULL" texture
	DX8Wrapper::Set_DX8_Texture(stage, NULL);
}

// ----------------------------------------------------------------------------
#ifdef OG

void TextureClass::Apply_New_Surface(bool initialized)
{
	if (D3DTexture) D3DTexture->Release();
	D3DTexture=TextureLoadTask->Peek_D3D_Texture();
	D3DTexture->AddRef();
	if (initialized) Initialized=true;

	WWASSERT(D3DTexture);
	IDirect3DSurface8* surface;
	DX8_ErrorCode(D3DTexture->GetSurfaceLevel(0,&surface));
	D3DSURFACE_DESC d3d_desc;
	::ZeroMemory(&d3d_desc, sizeof(D3DSURFACE_DESC));
	DX8_ErrorCode(surface->GetDesc(&d3d_desc));
//	if (TextureFormat==WW3D_FORMAT_UNKNOWN) {
		TextureFormat=D3DFormat_To_WW3DFormat(d3d_desc.Format);
//	}
//	else {
//		WWASSERT(D3DFormat_To_WW3DFormat(d3d_desc.Format)==TextureFormat);
//	}
	surface->Release();
}

#endif
#ifdef ZH
// Setting HSV_Shift value is always relative to the original texture. This function invalidates the
// texture surface and causes the texture to be reloaded. For thumbnailable textures, the hue shifting
// is done in the background loading thread.

#endif
// ----------------------------------------------------------------------------
#ifdef OG

unsigned TextureClass::Get_Texture_Memory_Usage() const
#endif
#ifdef ZH
void TextureBaseClass::Set_HSV_Shift(const Vector3 &hsv_shift)

#endif
{
#ifdef OG
	if (/*!ReductionEnabled || */!Initialized) return Calculate_Texture_Memory_Usage(this,0);
//	unsigned reduction=WW3D::Get_Texture_Reduction();
//	if (CurrentReductionFactor>reduction) reduction=CurrentReductionFactor;
	return Calculate_Texture_Memory_Usage(this,0);//reduction);
#endif
#ifdef ZH
	Invalidate();
	HSVShift=hsv_shift;

#endif
}

#ifdef OG
// ----------------------------------------------------------------------------

int TextureClass::_Get_Total_Locked_Surface_Size()

#endif
#ifdef ZH
//**********************************************************************************************
//! Get total locked surface size
/*! KM
*/
int TextureBaseClass::_Get_Total_Locked_Surface_Size()
#endif
{
	int total_locked_surface_size=0;

	HashTemplateIterator<StringClass,TextureClass*> ite(WW3DAssetManager::Get_Instance()->Texture_Hash());
	// Loop through all the textures in the manager
#ifdef OG
	for (ite.First ();!ite.Is_Done();ite.Next ()) {

#endif
#ifdef ZH
	for (ite.First ();!ite.Is_Done();ite.Next ()) 
	{
#endif
		// Get the current texture
#ifdef OG
		TextureClass* tex=ite.Peek_Value();
		if (!tex->Initialized) {
//			total_locked_surface_size+=tex->Get_Non_Reduced_Texture_Memory_Usage();
#endif
#ifdef ZH
		TextureBaseClass* tex=ite.Peek_Value();
		if (!tex->Initialized) 
		{
#endif
			total_locked_surface_size+=tex->Get_Texture_Memory_Usage();
		}
	}
	return total_locked_surface_size;
}

#ifdef OG
// ----------------------------------------------------------------------------

int TextureClass::_Get_Total_Texture_Size()

#endif
#ifdef ZH
//**********************************************************************************************
//! Get total texture size
/*! KM
*/
int TextureBaseClass::_Get_Total_Texture_Size()
#endif
{
	int total_texture_size=0;

	HashTemplateIterator<StringClass,TextureClass*> ite(WW3DAssetManager::Get_Instance()->Texture_Hash());
	// Loop through all the textures in the manager
#ifdef OG
	for (ite.First ();!ite.Is_Done();ite.Next ()) {

#endif
#ifdef ZH
	for (ite.First ();!ite.Is_Done();ite.Next ()) 
	{
#endif
		// Get the current texture
#ifdef OG
		TextureClass* tex=ite.Peek_Value();
#endif
#ifdef ZH
		TextureBaseClass* tex=ite.Peek_Value();
#endif
		total_texture_size+=tex->Get_Texture_Memory_Usage();
	}
	return total_texture_size;
}

// ----------------------------------------------------------------------------

#ifdef OG
int TextureClass::_Get_Total_Lightmap_Texture_Size()

#endif
#ifdef ZH
//**********************************************************************************************
//! Get total lightmap texture size
/*!
*/
int TextureBaseClass::_Get_Total_Lightmap_Texture_Size()
#endif
{
	int total_texture_size=0;

	HashTemplateIterator<StringClass,TextureClass*> ite(WW3DAssetManager::Get_Instance()->Texture_Hash());
	// Loop through all the textures in the manager
#ifdef OG
	for (ite.First ();!ite.Is_Done();ite.Next ()) {

#endif
#ifdef ZH
	for (ite.First ();!ite.Is_Done();ite.Next ()) 
	{
#endif
		// Get the current texture
#ifdef OG
		TextureClass* tex=ite.Peek_Value();
		if (tex->Is_Lightmap()) {

#endif
#ifdef ZH
		TextureBaseClass* tex=ite.Peek_Value();
		if (tex->Is_Lightmap()) 
		{
#endif
			total_texture_size+=tex->Get_Texture_Memory_Usage();
		}
	}
	return total_texture_size;
}

#ifdef OG
// ----------------------------------------------------------------------------
#endif

#ifdef OG
int TextureClass::_Get_Total_Procedural_Texture_Size()

#endif
#ifdef ZH
//**********************************************************************************************
//! Get total procedural texture size
/*!
*/
int TextureBaseClass::_Get_Total_Procedural_Texture_Size()
#endif
{
	int total_texture_size=0;

	HashTemplateIterator<StringClass,TextureClass*> ite(WW3DAssetManager::Get_Instance()->Texture_Hash());
	// Loop through all the textures in the manager
#ifdef OG
	for (ite.First ();!ite.Is_Done();ite.Next ()) {

#endif
#ifdef ZH
	for (ite.First ();!ite.Is_Done();ite.Next ()) 
	{
#endif
		// Get the current texture
#ifdef OG
		TextureClass* tex=ite.Peek_Value();
		if (tex->Is_Procedural()) {

#endif
#ifdef ZH
		TextureBaseClass* tex=ite.Peek_Value();
		if (tex->Is_Procedural()) 
		{
#endif
			total_texture_size+=tex->Get_Texture_Memory_Usage();
		}
	}
	return total_texture_size;
}

#ifdef OG
// ----------------------------------------------------------------------------

int TextureClass::_Get_Total_Texture_Count()

#endif
#ifdef ZH
//**********************************************************************************************
//! Get total texture count
/*!
*/
int TextureBaseClass::_Get_Total_Texture_Count()
#endif
{
	int texture_count=0;

	HashTemplateIterator<StringClass,TextureClass*> ite(WW3DAssetManager::Get_Instance()->Texture_Hash());
	// Loop through all the textures in the manager
#ifdef OG
	for (ite.First ();!ite.Is_Done();ite.Next ()) {

#endif
#ifdef ZH
	for (ite.First ();!ite.Is_Done();ite.Next ()) 
	{
#endif
		texture_count++;
	}

	return texture_count;
}

// ----------------------------------------------------------------------------

#ifdef OG
int TextureClass::_Get_Total_Lightmap_Texture_Count()

#endif
#ifdef ZH

//**********************************************************************************************
//! Get total light map texture count
/*!
*/
int TextureBaseClass::_Get_Total_Lightmap_Texture_Count()
#endif
{
	int texture_count=0;

	HashTemplateIterator<StringClass,TextureClass*> ite(WW3DAssetManager::Get_Instance()->Texture_Hash());
	// Loop through all the textures in the manager
#ifdef OG
	for (ite.First ();!ite.Is_Done();ite.Next ()) {
		if (ite.Peek_Value()->Is_Lightmap()) {

#endif
#ifdef ZH
	for (ite.First ();!ite.Is_Done();ite.Next ()) 
	{
		if (ite.Peek_Value()->Is_Lightmap()) 
		{
#endif
			texture_count++;
		}
	}

	return texture_count;
}

#ifdef OG
// ----------------------------------------------------------------------------

int TextureClass::_Get_Total_Procedural_Texture_Count()

#endif
#ifdef ZH
//**********************************************************************************************
//! Get total procedural texture count
/*!
*/
int TextureBaseClass::_Get_Total_Procedural_Texture_Count()
#endif
{
	int texture_count=0;

	HashTemplateIterator<StringClass,TextureClass*> ite(WW3DAssetManager::Get_Instance()->Texture_Hash());
	// Loop through all the textures in the manager
#ifdef OG
	for (ite.First ();!ite.Is_Done();ite.Next ()) {
		if (ite.Peek_Value()->Is_Procedural()) {

#endif
#ifdef ZH
	for (ite.First ();!ite.Is_Done();ite.Next ()) 
	{
		if (ite.Peek_Value()->Is_Procedural()) 
		{
#endif
			texture_count++;
		}
	}

	return texture_count;
}

#ifdef OG
// ----------------------------------------------------------------------------
#endif

#ifdef OG
int TextureClass::_Get_Total_Locked_Surface_Count()

#endif
#ifdef ZH
//**********************************************************************************************
//! Get total locked surface count
/*!
*/
int TextureBaseClass::_Get_Total_Locked_Surface_Count()
#endif
{
	int texture_count=0;

	HashTemplateIterator<StringClass,TextureClass*> ite(WW3DAssetManager::Get_Instance()->Texture_Hash());
	// Loop through all the textures in the manager
#ifdef OG
	for (ite.First ();!ite.Is_Done();ite.Next ()) {

#endif
#ifdef ZH
	for (ite.First ();!ite.Is_Done();ite.Next ()) 
	{
#endif
		// Get the current texture
#ifdef OG
		TextureClass* tex=ite.Peek_Value();
		if (!tex->Initialized) {

#endif
#ifdef ZH
		TextureBaseClass* tex=ite.Peek_Value();
		if (!tex->Initialized) 
		{
#endif
			texture_count++;
		}
	}

	return texture_count;
}
#ifdef OG
/*
bool Validate_Filters(unsigned type)

#endif
#ifdef ZH

/*************************************************************************
**                             TextureClass
*************************************************************************/
TextureClass::TextureClass
(
	unsigned width, 
	unsigned height, 
	WW3DFormat format, 
	MipCountType mip_level_count, 
	PoolType pool,
	bool rendertarget,
	bool allow_reduction
)
:	TextureBaseClass(width, height, mip_level_count, pool, rendertarget,allow_reduction),
	Filter(mip_level_count),
	TextureFormat(format)
{
	Initialized=true;
	IsProcedural=true;
	IsReducible=false;

	switch (format) 
	{
	case WW3D_FORMAT_DXT1:
	case WW3D_FORMAT_DXT2:
	case WW3D_FORMAT_DXT3:
	case WW3D_FORMAT_DXT4:
	case WW3D_FORMAT_DXT5:
		IsCompressionAllowed=true;
		break;
	default : break;
	}
		
	D3DPOOL d3dpool=(D3DPOOL)0;
	switch(pool)
	{
	case POOL_DEFAULT		: d3dpool=D3DPOOL_DEFAULT; break;
	case POOL_MANAGED		: d3dpool=D3DPOOL_MANAGED; break;
	case POOL_SYSTEMMEM	: d3dpool=D3DPOOL_SYSTEMMEM; break;
	default: WWASSERT(0);
	}

	Poke_Texture
	(
		DX8Wrapper::_Create_DX8_Texture
		(
			width, 
			height, 
			format, 
			mip_level_count,
			d3dpool,
			rendertarget
		)
	);

	if (pool==POOL_DEFAULT)
	{
		Set_Dirty();
		DX8TextureTrackerClass *track=new DX8TextureTrackerClass
		(
			width, 
			height, 
			format, 
			mip_level_count,
			this,
			rendertarget
		);
		DX8TextureManagerClass::Add(track);
	}
	LastAccessed=WW3D::Get_Sync_Time();
}

// ----------------------------------------------------------------------------
TextureClass::TextureClass
(
	const char *name,
	const char *full_path,
	MipCountType mip_level_count,
	WW3DFormat texture_format,
	bool allow_compression,
	bool allow_reduction
)
:	TextureBaseClass(0, 0, mip_level_count),
	Filter(mip_level_count),
	TextureFormat(texture_format)
{
	IsCompressionAllowed=allow_compression;
	InactivationTime=DEFAULT_INACTIVATION_TIME;		// Default inactivation time 30 seconds
	IsReducible=allow_reduction;

	switch (TextureFormat) 
	{
	case WW3D_FORMAT_DXT1:
	case WW3D_FORMAT_DXT2:
	case WW3D_FORMAT_DXT3:
	case WW3D_FORMAT_DXT4:
	case WW3D_FORMAT_DXT5:
		IsCompressionAllowed=true;
		break;
	case WW3D_FORMAT_U8V8:		// Bumpmap
	case WW3D_FORMAT_L6V5U5:	// Bumpmap
	case WW3D_FORMAT_X8L8V8U8:	// Bumpmap
		// If requesting bumpmap format that isn't available we'll just return the surface in whatever color
		// format the texture file is in. (This is illegal case, the format support should always be queried
		// before creating a bump texture!)
		if (!DX8Wrapper::Is_Initted() || !DX8Wrapper::Get_Current_Caps()->Support_Texture_Format(TextureFormat)) 
		{
			TextureFormat=WW3D_FORMAT_UNKNOWN;
		}
		// If bump format is valid, make sure compression is not allowed so that we don't even attempt to load
		// from a compressed file (quality isn't good enough for bump map). Also disable mipmapping.
		else 
		{
			IsCompressionAllowed=false;
			MipLevelCount=MIP_LEVELS_1;
			Filter.Set_Mip_Mapping(TextureFilterClass::FILTER_TYPE_NONE);
		}
		break;
	default:	break;
	}

	WWASSERT_PRINT(name && name[0], "TextureClass CTor: NULL or empty texture name\n");
	int len=strlen(name);
	for (int i=0;i<len;++i) 
	{
		if (name[i]=='+') 
		{
			IsLightmap=true;

			// Set bilinear filtering for lightmaps (they are very stretched and
			// low detail so we don't care for anisotropic or trilinear filtering...)
			Filter.Set_Min_Filter(TextureFilterClass::FILTER_TYPE_FAST);
			Filter.Set_Mag_Filter(TextureFilterClass::FILTER_TYPE_FAST);
			if (mip_level_count!=MIP_LEVELS_1) Filter.Set_Mip_Mapping(TextureFilterClass::FILTER_TYPE_FAST);
			break;
		}
	}
	Set_Texture_Name(name);
	Set_Full_Path(full_path);
	WWASSERT(name[0]!='\0');
	if (!WW3D::Is_Texturing_Enabled()) 
	{
		Initialized=true;
		Poke_Texture(NULL);
	}

	// Find original size from the thumbnail (but don't create thumbnail texture yet!)
	ThumbnailClass* thumb=ThumbnailManagerClass::Peek_Thumbnail_Instance_From_Any_Manager(Get_Full_Path());
	if (thumb) 
	{
		Width=thumb->Get_Original_Texture_Width();
		Height=thumb->Get_Original_Texture_Height();
 		if (MipLevelCount!=MIP_LEVELS_1) {
 			MipLevelCount=(MipCountType)thumb->Get_Original_Texture_Mip_Level_Count();
 		}
	}

	LastAccessed=WW3D::Get_Sync_Time();

	// If the thumbnails are not enabled, init the texture at this point to avoid stalling when the
	// mesh is rendered.
	if (!WW3D::Get_Thumbnail_Enabled()) 
	{
		if (TextureLoader::Is_DX8_Thread()) 
		{
			Init();
		}
	}
}

// ----------------------------------------------------------------------------
TextureClass::TextureClass
(
	SurfaceClass *surface, 
	MipCountType mip_level_count
)
:  TextureBaseClass(0,0,mip_level_count),
	Filter(mip_level_count),
	TextureFormat(surface->Get_Surface_Format())
{
	IsProcedural=true;
	Initialized=true;
	IsReducible=false;

	SurfaceClass::SurfaceDescription sd;
	surface->Get_Description(sd);
	Width=sd.Width;
	Height=sd.Height;
	switch (sd.Format) 
	{
	case WW3D_FORMAT_DXT1:
	case WW3D_FORMAT_DXT2:
	case WW3D_FORMAT_DXT3:
	case WW3D_FORMAT_DXT4:
	case WW3D_FORMAT_DXT5:
		IsCompressionAllowed=true;
		break;
	default: break;
	}

	Poke_Texture
	(
		DX8Wrapper::_Create_DX8_Texture
		(
			surface->Peek_D3D_Surface(), 
			mip_level_count
		)
	);
	LastAccessed=WW3D::Get_Sync_Time();
}

// ----------------------------------------------------------------------------
TextureClass::TextureClass(IDirect3DBaseTexture8* d3d_texture)
:	TextureBaseClass
	(
		0,
		0,
		((MipCountType)d3d_texture->GetLevelCount())
	),
	Filter((MipCountType)d3d_texture->GetLevelCount())
{
	Initialized=true;
	IsProcedural=true;
	IsReducible=false;
	
	Set_D3D_Base_Texture(d3d_texture);
	IDirect3DSurface8* surface;
	DX8_ErrorCode(Peek_D3D_Texture()->GetSurfaceLevel(0,&surface));
	D3DSURFACE_DESC d3d_desc;
	::ZeroMemory(&d3d_desc, sizeof(D3DSURFACE_DESC));
	DX8_ErrorCode(surface->GetDesc(&d3d_desc));
	Width=d3d_desc.Width;
	Height=d3d_desc.Height;
	TextureFormat=D3DFormat_To_WW3DFormat(d3d_desc.Format);
	switch (TextureFormat) 
	{
	case WW3D_FORMAT_DXT1:
	case WW3D_FORMAT_DXT2:
	case WW3D_FORMAT_DXT3:
	case WW3D_FORMAT_DXT4:
	case WW3D_FORMAT_DXT5:
		IsCompressionAllowed=true;
		break;
	default: break;
	}

	LastAccessed=WW3D::Get_Sync_Time();
}

//**********************************************************************************************
//! Initialise the texture
/*! 
*/
void TextureClass::Init()
{
	// If the texture has already been initialised we should exit now
	if (Initialized) return;

	WWPROFILE("TextureClass::Init");

	// If the texture has recently been inactivated, increase the inactivation time (this texture obviously
	// should not have been inactivated yet).
	if (InactivationTime && LastInactivationSyncTime) 
	{
		if ((WW3D::Get_Sync_Time()-LastInactivationSyncTime)<InactivationTime) 
		{
			ExtendedInactivationTime=3*InactivationTime;
		}
		LastInactivationSyncTime=0;
	}

	if (!Peek_D3D_Base_Texture()) 
	{
		if (!WW3D::Get_Thumbnail_Enabled() || MipLevelCount==MIP_LEVELS_1) 
		{
//		if (MipLevelCount==MIP_LEVELS_1) {
			TextureLoader::Request_Foreground_Loading(this);
		}
		else 
		{
			WW3DFormat format=TextureFormat;
			Load_Locked_Surface();
			TextureFormat=format;
		}
	}

	if (!Initialized) 
#endif
{
#ifdef OG
	ShaderClass shader=ShaderClass::_PresetOpaqueShader;
	shader.Apply();
	DX8Wrapper::Set_DX8_Texture(0, MissingTexture::_Get_Missing_Texture());
	DX8Wrapper::Set_DX8_Texture_Stage_State(0,D3DTSS_MINFILTER,_MinTextureFilters[type]);
	DX8Wrapper::Set_DX8_Texture_Stage_State(0,D3DTSS_MAGFILTER,_MagTextureFilters[type]);
	DX8Wrapper::Set_DX8_Texture_Stage_State(0,D3DTSS_MIPFILTER,_MipMapFilters[type]);
	unsigned long passes;
	HRESULT hres=DX8Wrapper::_Get_D3D_Device8()->ValidateDevice(&passes);
	return !FAILED(hres);
#endif
#ifdef ZH
		TextureLoader::Request_Background_Loading(this);
	}

	LastAccessed=WW3D::Get_Sync_Time();

#endif
}
#ifdef ZH

//**********************************************************************************************
//! Apply new surface to texture
/*! 
#endif
*/
#ifdef OG
void TextureClass::_Init_Filters()

#endif
#ifdef ZH
void TextureClass::Apply_New_Surface
(
	IDirect3DBaseTexture8* d3d_texture,
	bool initialized,
	bool disable_auto_invalidation
)
#endif
{
#ifdef OG
	const D3DCAPS8& dx8caps=DX8Caps::Get_Default_Caps();
#endif
#ifdef ZH
	IDirect3DBaseTexture8* d3d_tex=Peek_D3D_Base_Texture();
#endif

#ifdef OG
	_MinTextureFilters[FILTER_TYPE_NONE]=D3DTEXF_POINT;
	_MagTextureFilters[FILTER_TYPE_NONE]=D3DTEXF_POINT;
	_MipMapFilters[FILTER_TYPE_NONE]=D3DTEXF_NONE;

#endif
#ifdef ZH
	if (d3d_tex) d3d_tex->Release();

	Poke_Texture(d3d_texture);//TextureLoadTask->Peek_D3D_Texture();
	d3d_texture->AddRef();
#endif

#ifdef OG
	_MinTextureFilters[FILTER_TYPE_FAST]=D3DTEXF_LINEAR;
	_MagTextureFilters[FILTER_TYPE_FAST]=D3DTEXF_LINEAR;
	_MipMapFilters[FILTER_TYPE_FAST]=D3DTEXF_POINT;
#endif
#ifdef ZH
	if (initialized) Initialized=true;
	if (disable_auto_invalidation) InactivationTime = 0;

#endif

#ifdef OG
	// Jani: Disabling anisotropic filtering as it doesn't seem to work with the latest nVidia drivers.
	if (dx8caps.TextureFilterCaps&D3DPTFILTERCAPS_MAGFAFLATCUBIC) _MagTextureFilters[FILTER_TYPE_BEST]=D3DTEXF_FLATCUBIC;
	else if (dx8caps.TextureFilterCaps&D3DPTFILTERCAPS_MAGFANISOTROPIC) _MagTextureFilters[FILTER_TYPE_BEST]=D3DTEXF_ANISOTROPIC;
	else if (dx8caps.TextureFilterCaps&D3DPTFILTERCAPS_MAGFGAUSSIANCUBIC) _MagTextureFilters[FILTER_TYPE_BEST]=D3DTEXF_GAUSSIANCUBIC;
	else if (dx8caps.TextureFilterCaps&D3DPTFILTERCAPS_MAGFLINEAR) _MagTextureFilters[FILTER_TYPE_BEST]=D3DTEXF_LINEAR;
	else if (dx8caps.TextureFilterCaps&D3DPTFILTERCAPS_MAGFPOINT) _MagTextureFilters[FILTER_TYPE_BEST]=D3DTEXF_POINT;
	else {
		WWASSERT_PRINT(0,("No magnification filter found!"));

#endif
#ifdef ZH
	WWASSERT(d3d_texture);
	IDirect3DSurface8* surface;
	DX8_ErrorCode(Peek_D3D_Texture()->GetSurfaceLevel(0,&surface));
	D3DSURFACE_DESC d3d_desc;
	::ZeroMemory(&d3d_desc, sizeof(D3DSURFACE_DESC));
	DX8_ErrorCode(surface->GetDesc(&d3d_desc));
	if (initialized) 
	{
		TextureFormat=D3DFormat_To_WW3DFormat(d3d_desc.Format);
		Width=d3d_desc.Width;
		Height=d3d_desc.Height;
#endif
	}
#ifdef ZH
	surface->Release();
#endif

#ifdef OG
	if (dx8caps.TextureFilterCaps&D3DPTFILTERCAPS_MINFANISOTROPIC) _MinTextureFilters[FILTER_TYPE_BEST]=D3DTEXF_ANISOTROPIC;
	else if (dx8caps.TextureFilterCaps&D3DPTFILTERCAPS_MINFLINEAR) _MinTextureFilters[FILTER_TYPE_BEST]=D3DTEXF_LINEAR;
	else if (dx8caps.TextureFilterCaps&D3DPTFILTERCAPS_MINFPOINT) _MinTextureFilters[FILTER_TYPE_BEST]=D3DTEXF_POINT;
	else {
		WWASSERT_PRINT(0,("No minification filter found!"));
#endif
	}
#ifdef ZH


//**********************************************************************************************
//! Apply texture states
/*! 
*/
void TextureClass::Apply(unsigned int stage)
{
	// Initialization needs to be done when texture is used if it hasn't been done before.
	// XBOX always initializes textures at creation time.
	if (!Initialized) 
	{
		Init();
#endif

#ifdef OG
	if (dx8caps.TextureFilterCaps&D3DPTFILTERCAPS_MIPFLINEAR) _MipMapFilters[FILTER_TYPE_BEST]=D3DTEXF_LINEAR;
	else if (dx8caps.TextureFilterCaps&D3DPTFILTERCAPS_MIPFPOINT) _MipMapFilters[FILTER_TYPE_BEST]=D3DTEXF_POINT;
	else {
		WWASSERT_PRINT(0,("No mip filter found!"));

#endif
#ifdef ZH
		/* was in battlefield// Non-thumbnailed textures are always initialized when used
		if (MipLevelCount==MIP_LEVELS_1) 
		{
		}
		// Thumbnailed textures have delayed initialization and a background loading system
		else 
		{
			// Limit the number of texture initializations per frame to reduce stuttering
			if (TexturesAppliedPerFrame<MAX_TEXTURES_APPLIED_PER_FRAME) 
			{
				TexturesAppliedPerFrame++;
				Init();
			}
			else 
			{
				// If texture can't be initialized in this frame, at least make sure we have the thumbnail.
				if (!Peek_Texture()) 
				{
					WW3DFormat format=TextureFormat;
					Load_Locked_Surface();
					TextureFormat=format;
				}
			}
		}*/
#endif
	}
#ifdef ZH
	LastAccessed=WW3D::Get_Sync_Time();
#endif

#ifdef OG
//_MagTextureFilters[FILTER_TYPE_BEST]=D3DTEXF_FLATCUBIC;
//	WWASSERT(Validate_Filters(FILTER_TYPE_BEST));

#endif
#ifdef ZH
	DX8_RECORD_TEXTURE(this);

	// Set texture itself
	if (WW3D::Is_Texturing_Enabled()) 
	{
		DX8Wrapper::Set_DX8_Texture(stage, Peek_D3D_Base_Texture());
	}
	else 
	{
		DX8Wrapper::Set_DX8_Texture(stage, NULL);
	}
#endif

#ifdef OG
	_MinTextureFilters[FILTER_TYPE_DEFAULT]=_MinTextureFilters[FILTER_TYPE_BEST];
	_MagTextureFilters[FILTER_TYPE_DEFAULT]=_MagTextureFilters[FILTER_TYPE_BEST];
	_MipMapFilters[FILTER_TYPE_DEFAULT]=_MipMapFilters[FILTER_TYPE_BEST];
#endif
#ifdef ZH
	Filter.Apply(stage);
}

#endif

#ifdef OG
	for (int stage=0;stage<MeshMatDescClass::MAX_TEX_STAGES;++stage) {
		DX8Wrapper::Set_DX8_Texture_Stage_State(stage,D3DTSS_MAXANISOTROPY,2);

#endif
#ifdef ZH
//**********************************************************************************************
//! Get surface from mip level
/*! 
*/
SurfaceClass *TextureClass::Get_Surface_Level(unsigned int level)
{
	if (!Peek_D3D_Texture()) 
	{
		WWASSERT_PRINT(0, "Get_Surface_Level: D3DTexture is NULL!\n");
		return 0;
#endif
	}

#ifdef ZH
	IDirect3DSurface8 *d3d_surface = NULL;
	DX8_ErrorCode(Peek_D3D_Texture()->GetSurfaceLevel(level, &d3d_surface));
	SurfaceClass *surface = new SurfaceClass(d3d_surface);
	d3d_surface->Release();
#endif

#ifdef ZH
	return surface;
#endif
}

#ifdef OG
void TextureClass::_Set_Default_Min_Filter(FilterType filter)

#endif
#ifdef ZH
//**********************************************************************************************
//! Get surface description for a mip level
/*!
*/
void TextureClass::Get_Level_Description( SurfaceClass::SurfaceDescription & desc, unsigned int level )
#endif
{
#ifdef OG
	_MinTextureFilters[FILTER_TYPE_DEFAULT]=_MinTextureFilters[filter];

#endif
#ifdef ZH
	SurfaceClass * surf = Get_Surface_Level(level);
	if (surf != NULL) {
		surf->Get_Description(desc);
	}
	REF_PTR_RELEASE(surf);
#endif
}

#ifdef OG
void TextureClass::_Set_Default_Mag_Filter(FilterType filter)

#endif
#ifdef ZH
//**********************************************************************************************
//! Get D3D surface from mip level
/*! 
*/
IDirect3DSurface8 *TextureClass::Get_D3D_Surface_Level(unsigned int level)
{
	if (!Peek_D3D_Texture()) 
#endif
{
#ifdef OG
	_MagTextureFilters[FILTER_TYPE_DEFAULT]=_MagTextureFilters[filter];

#endif
#ifdef ZH
		WWASSERT_PRINT(0, "Get_D3D_Surface_Level: D3DTexture is NULL!\n");
		return 0;
	}

	IDirect3DSurface8 *d3d_surface = NULL;
	DX8_ErrorCode(Peek_D3D_Texture()->GetSurfaceLevel(level, &d3d_surface));
	return d3d_surface;
#endif
}

#ifdef OG
void TextureClass::_Set_Default_Mip_Filter(FilterType filter)

#endif
#ifdef ZH
//**********************************************************************************************
//! Get texture memory usage
/*! 
*/
unsigned TextureClass::Get_Texture_Memory_Usage() const
{
	int size=0;
	if (!Peek_D3D_Texture()) return 0;
	for (unsigned i=0;i<Peek_D3D_Texture()->GetLevelCount();++i) 
#endif
{
#ifdef OG
	_MipMapFilters[FILTER_TYPE_DEFAULT]=_MipMapFilters[filter];

#endif
#ifdef ZH
		D3DSURFACE_DESC desc;
		DX8_ErrorCode(Peek_D3D_Texture()->GetLevelDesc(i,&desc));
		size+=desc.Size;
	}
	return size;
#endif
}

// Utility functions
#ifdef OG
TextureClass *Load_Texture(ChunkLoadClass & cload)
#endif
#ifdef ZH
TextureClass* Load_Texture(ChunkLoadClass & cload)
#endif
{
	// Assume failure
	TextureClass *newtex = NULL;

	char name[256];
#ifdef OG
	if (cload.Open_Chunk () && (cload.Cur_Chunk_ID () == W3D_CHUNK_TEXTURE)) {

#endif
#ifdef ZH
	if (cload.Open_Chunk () && (cload.Cur_Chunk_ID () == W3D_CHUNK_TEXTURE)) 
	{
#endif

		W3dTextureInfoStruct texinfo;
		bool hastexinfo = false;

		/*
		** Read in the texture filename, and a possible texture info structure.
		*/
		while (cload.Open_Chunk()) {
			switch (cload.Cur_Chunk_ID()) {
				case W3D_CHUNK_TEXTURE_NAME:
					cload.Read(&name,cload.Cur_Chunk_Length());
					break;

				case W3D_CHUNK_TEXTURE_INFO:
					cload.Read(&texinfo,sizeof(W3dTextureInfoStruct));
					hastexinfo = true;
					break;
			};
			cload.Close_Chunk();
		}
		cload.Close_Chunk();

		/*
		** Get the texture from the asset manager
		*/
#ifdef OG
		if (hastexinfo) {

#endif
#ifdef ZH
		if (hastexinfo) 
		{
#endif
			
#ifdef OG
			TextureClass::MipCountType mipcount;
#endif
#ifdef ZH
			MipCountType mipcount;
#endif

			bool no_lod = ((texinfo.Attributes & W3DTEXTURE_NO_LOD) == W3DTEXTURE_NO_LOD);
			
#ifdef OG
			if (no_lod) {
				mipcount = TextureClass::MIP_LEVELS_1;
			} else {

#endif
#ifdef ZH
			if (no_lod) 
			{
				mipcount = MIP_LEVELS_1;
			} 
			else 
			{
#endif
				switch (texinfo.Attributes & W3DTEXTURE_MIP_LEVELS_MASK) {

					case W3DTEXTURE_MIP_LEVELS_ALL:
#ifdef OG
						mipcount = TextureClass::MIP_LEVELS_ALL;
#endif
#ifdef ZH
						mipcount = MIP_LEVELS_ALL;
#endif
						break;

					case W3DTEXTURE_MIP_LEVELS_2:
#ifdef OG
						mipcount = TextureClass::MIP_LEVELS_2;
#endif
#ifdef ZH
						mipcount = MIP_LEVELS_2;
#endif
						break;

					case W3DTEXTURE_MIP_LEVELS_3:
#ifdef OG
						mipcount = TextureClass::MIP_LEVELS_3;
#endif
#ifdef ZH
						mipcount = MIP_LEVELS_3;
#endif
						break;

					case W3DTEXTURE_MIP_LEVELS_4:
#ifdef OG
						mipcount = TextureClass::MIP_LEVELS_4;
#endif
#ifdef ZH
						mipcount = MIP_LEVELS_4;
#endif
						break;

					default:
						WWASSERT (false);
#ifdef OG
						mipcount = TextureClass::MIP_LEVELS_ALL;
#endif
#ifdef ZH
						mipcount = MIP_LEVELS_ALL;
#endif
						break;
				}
			}
#ifdef OG
			newtex = WW3DAssetManager::Get_Instance()->Get_Texture (name, mipcount);
#endif

#ifdef OG
			if (no_lod) {
				newtex->Set_Mip_Mapping(TextureClass::FILTER_TYPE_NONE);
			}
			bool u_clamp = ((texinfo.Attributes & W3DTEXTURE_CLAMP_U) != 0);
			newtex->Set_U_Addr_Mode(u_clamp ? TextureClass::TEXTURE_ADDRESS_CLAMP : TextureClass::TEXTURE_ADDRESS_REPEAT);
			bool v_clamp = ((texinfo.Attributes & W3DTEXTURE_CLAMP_V) != 0);
			newtex->Set_V_Addr_Mode(v_clamp ? TextureClass::TEXTURE_ADDRESS_CLAMP : TextureClass::TEXTURE_ADDRESS_REPEAT);
#endif
#ifdef ZH
			WW3DFormat format=WW3D_FORMAT_UNKNOWN;

#endif

#ifdef OG
			switch (texinfo.Attributes & W3DTEXTURE_TYPE_MASK) {

#endif
#ifdef ZH
			switch (texinfo.Attributes & W3DTEXTURE_TYPE_MASK) 
			{
#endif

				case W3DTEXTURE_TYPE_COLORMAP: 
					// Do nothing.
					break;

				case W3DTEXTURE_TYPE_BUMPMAP:
				{
#ifdef OG
					TextureClass *releasetex = newtex;

#endif
#ifdef ZH
					if (DX8Wrapper::Is_Initted() && DX8Wrapper::Get_Current_Caps()->Support_Bump_Envmap()) 
					{
						// No mipmaps to bumpmap for now
						mipcount=MIP_LEVELS_1;
#endif

#ifdef OG
					// Format is assumed to be a grayscale heightmap. Convert it to a bump map.
					newtex = WW3DAssetManager::Get_Instance()->Get_Bumpmap_Based_On_Texture (newtex);
					WW3DAssetManager::Get_Instance()->Release_Texture (releasetex);

#endif
#ifdef ZH
						if (DX8Wrapper::Get_Current_Caps()->Support_Texture_Format(WW3D_FORMAT_U8V8)) format=WW3D_FORMAT_U8V8;
						else if (DX8Wrapper::Get_Current_Caps()->Support_Texture_Format(WW3D_FORMAT_X8L8V8U8)) format=WW3D_FORMAT_X8L8V8U8;
						else if (DX8Wrapper::Get_Current_Caps()->Support_Texture_Format(WW3D_FORMAT_L6V5U5)) format=WW3D_FORMAT_L6V5U5;
					}
#endif
					break;
				}

				default:
					WWASSERT (false);
					break;
			}
#ifdef ZH

			newtex = WW3DAssetManager::Get_Instance()->Get_Texture (name, mipcount, format);

			if (no_lod) 
			{
				newtex->Get_Filter().Set_Mip_Mapping(TextureFilterClass::FILTER_TYPE_NONE);
			}
			bool u_clamp = ((texinfo.Attributes & W3DTEXTURE_CLAMP_U) != 0);
			newtex->Get_Filter().Set_U_Addr_Mode(u_clamp ? TextureFilterClass::TEXTURE_ADDRESS_CLAMP : TextureFilterClass::TEXTURE_ADDRESS_REPEAT);
			bool v_clamp = ((texinfo.Attributes & W3DTEXTURE_CLAMP_V) != 0);
			newtex->Get_Filter().Set_V_Addr_Mode(v_clamp ? TextureFilterClass::TEXTURE_ADDRESS_CLAMP : TextureFilterClass::TEXTURE_ADDRESS_REPEAT);
#endif

#ifdef OG
		} else {

#endif
#ifdef ZH
		} else 
		{
#endif
			newtex = WW3DAssetManager::Get_Instance()->Get_Texture(name);
		}

		WWASSERT(newtex);
	}

	// Return a pointer to the new texture	
	return newtex;
}

// Utility function used by Save_Texture
void setup_texture_attributes(TextureClass * tex, W3dTextureInfoStruct * texinfo)
{
	texinfo->Attributes = 0;

#ifdef OG
	if (tex->Get_Mip_Mapping() == TextureClass::FILTER_TYPE_NONE) texinfo->Attributes |= W3DTEXTURE_NO_LOD;
	if (tex->Get_U_Addr_Mode() == TextureClass::TEXTURE_ADDRESS_CLAMP) texinfo->Attributes |= W3DTEXTURE_CLAMP_U;
	if (tex->Get_V_Addr_Mode() == TextureClass::TEXTURE_ADDRESS_CLAMP) texinfo->Attributes |= W3DTEXTURE_CLAMP_V;
#endif
#ifdef ZH
	if (tex->Get_Filter().Get_Mip_Mapping() == TextureFilterClass::FILTER_TYPE_NONE) texinfo->Attributes |= W3DTEXTURE_NO_LOD;
	if (tex->Get_Filter().Get_U_Addr_Mode() == TextureFilterClass::TEXTURE_ADDRESS_CLAMP) texinfo->Attributes |= W3DTEXTURE_CLAMP_U;
	if (tex->Get_Filter().Get_V_Addr_Mode() == TextureFilterClass::TEXTURE_ADDRESS_CLAMP) texinfo->Attributes |= W3DTEXTURE_CLAMP_V;
#endif
}


void Save_Texture(TextureClass * texture,ChunkSaveClass & csave)
{
	const char * filename;
	W3dTextureInfoStruct texinfo;
	memset(&texinfo,0,sizeof(texinfo));

	filename = texture->Get_Full_Path();

	setup_texture_attributes(texture, &texinfo);

	csave.Begin_Chunk(W3D_CHUNK_TEXTURE_NAME);
	csave.Write(filename,strlen(filename)+1);
	csave.End_Chunk();

	if ((texinfo.Attributes != 0) || (texinfo.AnimType != 0) || (texinfo.FrameCount != 0)) {
		csave.Begin_Chunk(W3D_CHUNK_TEXTURE_INFO);
		csave.Write(&texinfo, sizeof(texinfo));
		csave.End_Chunk();
#ifdef ZH
	}
}

/*!
 *	KJM depth stencil texture constructor
 */
ZTextureClass::ZTextureClass
(
	unsigned width, 
	unsigned height, 
	WW3DZFormat zformat, 
	MipCountType mip_level_count, 
	PoolType pool
)
:	TextureBaseClass(width,height, mip_level_count, pool),
	DepthStencilTextureFormat(zformat)
{
	D3DPOOL d3dpool=(D3DPOOL)0;
	switch (pool)
	{
	case POOL_DEFAULT: d3dpool=D3DPOOL_DEFAULT; break;
	case POOL_MANAGED: d3dpool=D3DPOOL_MANAGED; break;
	case POOL_SYSTEMMEM: d3dpool=D3DPOOL_SYSTEMMEM;	break;
	default:	WWASSERT(0);
	}

	Poke_Texture
	(
		DX8Wrapper::_Create_DX8_ZTexture
		(
			width,
			height, 
			zformat, 
			mip_level_count,
			d3dpool
		)
	);

	if (pool==POOL_DEFAULT)
	{
		Set_Dirty();
		DX8ZTextureTrackerClass *track=new DX8ZTextureTrackerClass
		(
			width, 
			height, 
			zformat, 
			mip_level_count,
			this
		);
		DX8TextureManagerClass::Add(track);
	}
	Initialized=true;
	IsProcedural=true;
	IsReducible=false;

	LastAccessed=WW3D::Get_Sync_Time();
}

//**********************************************************************************************
//! Apply depth stencil texture
/*! KM
*/
void ZTextureClass::Apply(unsigned int stage)
{
	DX8Wrapper::Set_DX8_Texture(stage, Peek_D3D_Base_Texture());
}

//**********************************************************************************************
//! Apply new surface to texture
/*! KM
*/
void ZTextureClass::Apply_New_Surface
(
	IDirect3DBaseTexture8* d3d_texture,
	bool initialized,
	bool disable_auto_invalidation
)
{
	IDirect3DBaseTexture8* d3d_tex=Peek_D3D_Base_Texture();

	if (d3d_tex) d3d_tex->Release();

	Poke_Texture(d3d_texture);//TextureLoadTask->Peek_D3D_Texture();
	d3d_texture->AddRef();

	if (initialized) Initialized=true;
	if (disable_auto_invalidation) InactivationTime = 0;

	WWASSERT(Peek_D3D_Texture());
	IDirect3DSurface8* surface;
	DX8_ErrorCode(Peek_D3D_Texture()->GetSurfaceLevel(0,&surface));
	D3DSURFACE_DESC d3d_desc;
	::ZeroMemory(&d3d_desc, sizeof(D3DSURFACE_DESC));
	DX8_ErrorCode(surface->GetDesc(&d3d_desc));
	if (initialized) 
	{
		DepthStencilTextureFormat=D3DFormat_To_WW3DZFormat(d3d_desc.Format);
		Width=d3d_desc.Width;
		Height=d3d_desc.Height;
	}
	surface->Release();
}

//**********************************************************************************************
//! Get D3D surface from mip level
/*! 
*/
IDirect3DSurface8* ZTextureClass::Get_D3D_Surface_Level(unsigned int level)
{
	if (!Peek_D3D_Texture()) 
	{
		WWASSERT_PRINT(0, "Get_D3D_Surface_Level: D3DTexture is NULL!\n");
		return 0;
	}

	IDirect3DSurface8 *d3d_surface = NULL;
	DX8_ErrorCode(Peek_D3D_Texture()->GetSurfaceLevel(level, &d3d_surface));
	return d3d_surface;
}

//**********************************************************************************************
//! Get texture memory usage
/*! 
*/
unsigned ZTextureClass::Get_Texture_Memory_Usage() const
{
	int size=0;
	if (!Peek_D3D_Texture()) return 0;
	for (unsigned i=0;i<Peek_D3D_Texture()->GetLevelCount();++i) 
	{
		D3DSURFACE_DESC desc;
		DX8_ErrorCode(Peek_D3D_Texture()->GetLevelDesc(i,&desc));
		size+=desc.Size;
	}
	return size;
}

/*************************************************************************
**                             CubeTextureClass
*************************************************************************/
CubeTextureClass::CubeTextureClass
(
	unsigned width, 
	unsigned height, 
	WW3DFormat format, 
	MipCountType mip_level_count, 
	PoolType pool,
	bool rendertarget,
	bool allow_reduction
)
: TextureClass(width, height, format, mip_level_count, pool, rendertarget)
{
	Initialized=true;
	IsProcedural=true;
	IsReducible=false;

	switch (format) 
	{
	case WW3D_FORMAT_DXT1:
	case WW3D_FORMAT_DXT2:
	case WW3D_FORMAT_DXT3:
	case WW3D_FORMAT_DXT4:
	case WW3D_FORMAT_DXT5:
		IsCompressionAllowed=true;
		break;
	default : break;
	}
		
	D3DPOOL d3dpool=(D3DPOOL)0;
	switch(pool)
	{
	case POOL_DEFAULT		: d3dpool=D3DPOOL_DEFAULT; break;
	case POOL_MANAGED		: d3dpool=D3DPOOL_MANAGED; break;
	case POOL_SYSTEMMEM	: d3dpool=D3DPOOL_SYSTEMMEM; break;
	default: WWASSERT(0);
	}

	Poke_Texture
	(
		DX8Wrapper::_Create_DX8_Cube_Texture
		(
			width, 
			height, 
			format, 
			mip_level_count,
			d3dpool,
			rendertarget
		)
	);

	if (pool==POOL_DEFAULT)
	{
		Set_Dirty();
		DX8TextureTrackerClass *track=new DX8TextureTrackerClass
		(
			width, 
			height, 
			format, 
			mip_level_count,
			this,
			rendertarget
		);
		DX8TextureManagerClass::Add(track);
	}
	LastAccessed=WW3D::Get_Sync_Time();
}

// ----------------------------------------------------------------------------
CubeTextureClass::CubeTextureClass
(
	const char *name,
	const char *full_path,
	MipCountType mip_level_count,
	WW3DFormat texture_format,
	bool allow_compression,
	bool allow_reduction
)
:	TextureClass(0,0,mip_level_count, POOL_MANAGED, false, texture_format)
{
	IsCompressionAllowed=allow_compression;
	InactivationTime=DEFAULT_INACTIVATION_TIME;		// Default inactivation time 30 seconds

	switch (TextureFormat) 
	{
	case WW3D_FORMAT_DXT1:
	case WW3D_FORMAT_DXT2:
	case WW3D_FORMAT_DXT3:
	case WW3D_FORMAT_DXT4:
	case WW3D_FORMAT_DXT5:
		IsCompressionAllowed=true;
		break;
	case WW3D_FORMAT_U8V8:		// Bumpmap
	case WW3D_FORMAT_L6V5U5:	// Bumpmap
	case WW3D_FORMAT_X8L8V8U8:	// Bumpmap
		// If requesting bumpmap format that isn't available we'll just return the surface in whatever color
		// format the texture file is in. (This is illegal case, the format support should always be queried
		// before creating a bump texture!)
		if (!DX8Wrapper::Is_Initted() || !DX8Wrapper::Get_Current_Caps()->Support_Texture_Format(TextureFormat)) 
		{
			TextureFormat=WW3D_FORMAT_UNKNOWN;
		}
		// If bump format is valid, make sure compression is not allowed so that we don't even attempt to load
		// from a compressed file (quality isn't good enough for bump map). Also disable mipmapping.
		else 
		{
			IsCompressionAllowed=false;
			MipLevelCount=MIP_LEVELS_1;
			Filter.Set_Mip_Mapping(TextureFilterClass::FILTER_TYPE_NONE);
		}
		break;
	default:	break;
	}

	WWASSERT_PRINT(name && name[0], "TextureClass CTor: NULL or empty texture name\n");
	int len=strlen(name);
	for (int i=0;i<len;++i) 
	{
		if (name[i]=='+') 
		{
			IsLightmap=true;

			// Set bilinear filtering for lightmaps (they are very stretched and
			// low detail so we don't care for anisotropic or trilinear filtering...)
			Filter.Set_Min_Filter(TextureFilterClass::FILTER_TYPE_FAST);
			Filter.Set_Mag_Filter(TextureFilterClass::FILTER_TYPE_FAST);
			if (mip_level_count!=MIP_LEVELS_1) Filter.Set_Mip_Mapping(TextureFilterClass::FILTER_TYPE_FAST);
			break;
		}
	}
	Set_Texture_Name(name);
	Set_Full_Path(full_path);
	WWASSERT(name[0]!='\0');
	if (!WW3D::Is_Texturing_Enabled()) 
	{
		Initialized=true;
		Poke_Texture(NULL);
	}

	// Find original size from the thumbnail (but don't create thumbnail texture yet!)
	ThumbnailClass* thumb=ThumbnailManagerClass::Peek_Thumbnail_Instance_From_Any_Manager(Get_Full_Path());
	if (thumb) 
	{
		Width=thumb->Get_Original_Texture_Width();
		Height=thumb->Get_Original_Texture_Height();
 		if (MipLevelCount!=MIP_LEVELS_1) {
 			MipLevelCount=(MipCountType)thumb->Get_Original_Texture_Mip_Level_Count();
#endif
	}
}
#ifdef ZH

	LastAccessed=WW3D::Get_Sync_Time();
#endif

#ifdef ZH
	// If the thumbnails are not enabled, init the texture at this point to avoid stalling when the
	// mesh is rendered.
	if (!WW3D::Get_Thumbnail_Enabled()) 
	{
		if (TextureLoader::Is_DX8_Thread()) 
		{
			Init();
		}
	}
}

// don't know if these are needed
#if 0
#endif
// ----------------------------------------------------------------------------
#ifdef ZH
CubeTextureClass::CubeTextureClass
(
	SurfaceClass *surface, 
	MipCountType mip_level_count
)
:	TextureClass(0,0,mip_level_count, POOL_MANAGED, false, surface->Get_Surface_Format())
{
	IsProcedural=true;
	Initialized=true;
	IsReducible=false;
#endif

#ifdef OG
BumpmapTextureClass::BumpmapTextureClass(TextureClass* texture)
	:
//	TextureClass(texture->Get_Width(),texture->Get_Height(),texture->Get_Textur4e_Format(),MIP_LEVELS_1)
	TextureClass(TextureLoader::Generate_Bumpmap(texture))

#endif
#ifdef ZH
	SurfaceClass::SurfaceDescription sd;
	surface->Get_Description(sd);
	Width=sd.Width;
	Height=sd.Height;
	switch (sd.Format) 
	{
	case WW3D_FORMAT_DXT1:
	case WW3D_FORMAT_DXT2:
	case WW3D_FORMAT_DXT3:
	case WW3D_FORMAT_DXT4:
	case WW3D_FORMAT_DXT5:
		IsCompressionAllowed=true;
		break;
	default: break;
	}

	Poke_Texture
	(
		DX8Wrapper::_Create_DX8_Cube_Texture
		(
			surface->Peek_D3D_Surface(), 
			mip_level_count
		)
	);
	LastAccessed=WW3D::Get_Sync_Time();
}

// ----------------------------------------------------------------------------
CubeTextureClass::CubeTextureClass(IDirect3DBaseTexture8* d3d_texture)
:	TextureBaseClass
	(
		0,
		0,
		((MipCountType)d3d_texture->GetLevelCount())
	),
	Filter((MipCountType)d3d_texture->GetLevelCount())
{
	Initialized=true;
	IsProcedural=true;
	IsReducible=false;

	Peek_Texture()->AddRef();
	IDirect3DSurface8* surface;
	DX8_ErrorCode(Peek_D3D_Texture()->GetSurfaceLevel(0,&surface));
	D3DSURFACE_DESC d3d_desc;
	::ZeroMemory(&d3d_desc, sizeof(D3DSURFACE_DESC));
	DX8_ErrorCode(surface->GetDesc(&d3d_desc));
	Width=d3d_desc.Width;
	Height=d3d_desc.Height;
	TextureFormat=D3DFormat_To_WW3DFormat(d3d_desc.Format);
	switch (TextureFormat) 
	{
	case WW3D_FORMAT_DXT1:
	case WW3D_FORMAT_DXT2:
	case WW3D_FORMAT_DXT3:
	case WW3D_FORMAT_DXT4:
	case WW3D_FORMAT_DXT5:
		IsCompressionAllowed=true;
		break;
	default: break;
	}

	LastAccessed=WW3D::Get_Sync_Time();
}
#endif

//**********************************************************************************************
//! Apply new surface to texture
/*! 
*/
void CubeTextureClass::Apply_New_Surface
(
	IDirect3DBaseTexture8* d3d_texture,
	bool initialized,
	bool disable_auto_invalidation
)
{
	IDirect3DBaseTexture8* d3d_tex=Peek_D3D_Base_Texture();

	if (d3d_tex) d3d_tex->Release();

	Poke_Texture(d3d_texture);//TextureLoadTask->Peek_D3D_Texture();
	d3d_texture->AddRef();

	if (initialized) Initialized=true;
	if (disable_auto_invalidation) InactivationTime = 0;

	WWASSERT(d3d_texture);
	D3DSURFACE_DESC d3d_desc;
	::ZeroMemory(&d3d_desc, sizeof(D3DSURFACE_DESC));
	DX8_ErrorCode(Peek_D3D_CubeTexture()->GetLevelDesc(0,&d3d_desc));

	if (initialized) 
	{
		TextureFormat=D3DFormat_To_WW3DFormat(d3d_desc.Format);
		Width=d3d_desc.Width;
		Height=d3d_desc.Height;
	}
}

/*************************************************************************
**                             VolumeTextureClass
*************************************************************************/
VolumeTextureClass::VolumeTextureClass
(
	unsigned width, 
	unsigned height, 
	unsigned depth,
	WW3DFormat format, 
	MipCountType mip_level_count, 
	PoolType pool,
	bool rendertarget,
	bool allow_reduction
)
: TextureClass(width, height, format, mip_level_count, pool, rendertarget),
  Depth(depth)
{
	Initialized=true;
	IsProcedural=true;
	IsReducible=false;

	switch (format) 
	{
	case WW3D_FORMAT_DXT1:
	case WW3D_FORMAT_DXT2:
	case WW3D_FORMAT_DXT3:
	case WW3D_FORMAT_DXT4:
	case WW3D_FORMAT_DXT5:
		IsCompressionAllowed=true;
		break;
	default : break;
	}
		
	D3DPOOL d3dpool=(D3DPOOL)0;
	switch(pool)
	{
	case POOL_DEFAULT		: d3dpool=D3DPOOL_DEFAULT; break;
	case POOL_MANAGED		: d3dpool=D3DPOOL_MANAGED; break;
	case POOL_SYSTEMMEM	: d3dpool=D3DPOOL_SYSTEMMEM; break;
	default: WWASSERT(0);
	}

	Poke_Texture
	(
		DX8Wrapper::_Create_DX8_Volume_Texture
		(
			width, 
			height, 
			depth,
			format, 
			mip_level_count,
			d3dpool
		)
	);

	if (pool==POOL_DEFAULT)
	{
		Set_Dirty();
		DX8TextureTrackerClass *track=new DX8TextureTrackerClass
		(
			width, 
			height, 
			format, 
			mip_level_count,
			this,
			rendertarget
		);
		DX8TextureManagerClass::Add(track);
	}
	LastAccessed=WW3D::Get_Sync_Time();
}

// ----------------------------------------------------------------------------
VolumeTextureClass::VolumeTextureClass
(
	const char *name,
	const char *full_path,
	MipCountType mip_level_count,
	WW3DFormat texture_format,
	bool allow_compression,
	bool allow_reduction
)
:	TextureClass(0,0,mip_level_count, POOL_MANAGED, false, texture_format),
	Depth(0)
{
	IsCompressionAllowed=allow_compression;
	InactivationTime=DEFAULT_INACTIVATION_TIME;		// Default inactivation time 30 seconds

	switch (TextureFormat) 
	{
	case WW3D_FORMAT_DXT1:
	case WW3D_FORMAT_DXT2:
	case WW3D_FORMAT_DXT3:
	case WW3D_FORMAT_DXT4:
	case WW3D_FORMAT_DXT5:
		IsCompressionAllowed=true;
		break;
	case WW3D_FORMAT_U8V8:		// Bumpmap
	case WW3D_FORMAT_L6V5U5:	// Bumpmap
	case WW3D_FORMAT_X8L8V8U8:	// Bumpmap
		// If requesting bumpmap format that isn't available we'll just return the surface in whatever color
		// format the texture file is in. (This is illegal case, the format support should always be queried
		// before creating a bump texture!)
		if (!DX8Wrapper::Is_Initted() || !DX8Wrapper::Get_Current_Caps()->Support_Texture_Format(TextureFormat)) 
		{
			TextureFormat=WW3D_FORMAT_UNKNOWN;
		}
		// If bump format is valid, make sure compression is not allowed so that we don't even attempt to load
		// from a compressed file (quality isn't good enough for bump map). Also disable mipmapping.
		else 
		{
			IsCompressionAllowed=false;
			MipLevelCount=MIP_LEVELS_1;
			Filter.Set_Mip_Mapping(TextureFilterClass::FILTER_TYPE_NONE);
		}
		break;
	default:	break;
	}

	WWASSERT_PRINT(name && name[0], "TextureClass CTor: NULL or empty texture name\n");
	int len=strlen(name);
	for (int i=0;i<len;++i) 
	{
		if (name[i]=='+') 
		{
			IsLightmap=true;

			// Set bilinear filtering for lightmaps (they are very stretched and
			// low detail so we don't care for anisotropic or trilinear filtering...)
			Filter.Set_Min_Filter(TextureFilterClass::FILTER_TYPE_FAST);
			Filter.Set_Mag_Filter(TextureFilterClass::FILTER_TYPE_FAST);
			if (mip_level_count!=MIP_LEVELS_1) Filter.Set_Mip_Mapping(TextureFilterClass::FILTER_TYPE_FAST);
			break;
		}
	}
	Set_Texture_Name(name);
	Set_Full_Path(full_path);
	WWASSERT(name[0]!='\0');
	if (!WW3D::Is_Texturing_Enabled()) 
	{
		Initialized=true;
		Poke_Texture(NULL);
	}

	// Find original size from the thumbnail (but don't create thumbnail texture yet!)
	ThumbnailClass* thumb=ThumbnailManagerClass::Peek_Thumbnail_Instance_From_Any_Manager(Get_Full_Path());
	if (thumb) 
	{
		Width=thumb->Get_Original_Texture_Width();
		Height=thumb->Get_Original_Texture_Height();
 		if (MipLevelCount!=MIP_LEVELS_1) {
 			MipLevelCount=(MipCountType)thumb->Get_Original_Texture_Mip_Level_Count();
 		}
	}

	LastAccessed=WW3D::Get_Sync_Time();

	// If the thumbnails are not enabled, init the texture at this point to avoid stalling when the
	// mesh is rendered.
	if (!WW3D::Get_Thumbnail_Enabled()) 
	{
		if (TextureLoader::Is_DX8_Thread()) 
		{
			Init();
		}
	}
}

// don't know if these are needed
#if 0
// ----------------------------------------------------------------------------
CubeTextureClass::CubeTextureClass
(
	SurfaceClass *surface, 
	MipCountType mip_level_count
)
:	TextureClass(0,0,mip_level_count, POOL_MANAGED, false, surface->Get_Surface_Format())
{
	IsProcedural=true;
	Initialized=true;
	IsReducible=false;

	SurfaceClass::SurfaceDescription sd;
	surface->Get_Description(sd);
	Width=sd.Width;
	Height=sd.Height;
	switch (sd.Format) 
#endif
{
#ifdef OG
//	D3DTexture=TextureLoader::Generate_Bumpmap(texture);
//		TextureLoader:::Generage_Bumpmap

#endif
#ifdef ZH
	case WW3D_FORMAT_DXT1:
	case WW3D_FORMAT_DXT2:
	case WW3D_FORMAT_DXT3:
	case WW3D_FORMAT_DXT4:
	case WW3D_FORMAT_DXT5:
		IsCompressionAllowed=true;
		break;
	default: break;
	}

	Poke_Texture
	(
		DX8Wrapper::_Create_DX8_Cube_Texture
		(
			surface->Peek_D3D_Surface(), 
			mip_level_count
		)
	);
	LastAccessed=WW3D::Get_Sync_Time();
#endif
}
#ifdef ZH

// ----------------------------------------------------------------------------
CubeTextureClass::CubeTextureClass(IDirect3DBaseTexture8* d3d_texture)
:	TextureBaseClass
	(
		0,
		0,
		((MipCountType)d3d_texture->GetLevelCount())
	),
	Filter((MipCountType)d3d_texture->GetLevelCount())
{
	Initialized=true;
	IsProcedural=true;
	IsReducible=false;
#endif

#ifdef OG
BumpmapTextureClass::~BumpmapTextureClass()

#endif
#ifdef ZH
	Peek_Texture()->AddRef();
	IDirect3DSurface8* surface;
	DX8_ErrorCode(Peek_D3D_Texture()->GetSurfaceLevel(0,&surface));
	D3DSURFACE_DESC d3d_desc;
	::ZeroMemory(&d3d_desc, sizeof(D3DSURFACE_DESC));
	DX8_ErrorCode(surface->GetDesc(&d3d_desc));
	Width=d3d_desc.Width;
	Height=d3d_desc.Height;
	TextureFormat=D3DFormat_To_WW3DFormat(d3d_desc.Format);
	switch (TextureFormat) 
#endif
{
#ifdef ZH
	case WW3D_FORMAT_DXT1:
	case WW3D_FORMAT_DXT2:
	case WW3D_FORMAT_DXT3:
	case WW3D_FORMAT_DXT4:
	case WW3D_FORMAT_DXT5:
		IsCompressionAllowed=true;
		break;
	default: break;
#endif
}
#ifdef ZH

	LastAccessed=WW3D::Get_Sync_Time();
}
#endif

#endif

#ifdef ZH


//**********************************************************************************************
//! Apply new surface to texture
/*! 
*/
void VolumeTextureClass::Apply_New_Surface
(
	IDirect3DBaseTexture8* d3d_texture,
	bool initialized,
	bool disable_auto_invalidation
)
{
	IDirect3DBaseTexture8* d3d_tex=Peek_D3D_Base_Texture();

	if (d3d_tex) d3d_tex->Release();

	Poke_Texture(d3d_texture);//TextureLoadTask->Peek_D3D_Texture();
	d3d_texture->AddRef();

	if (initialized) Initialized=true;
	if (disable_auto_invalidation) InactivationTime = 0;

	WWASSERT(d3d_texture);
	D3DVOLUME_DESC d3d_desc;
	::ZeroMemory(&d3d_desc, sizeof(D3DVOLUME_DESC));

	DX8_ErrorCode(Peek_D3D_VolumeTexture()->GetLevelDesc(0,&d3d_desc));

	if (initialized)
	{
		TextureFormat=D3DFormat_To_WW3DFormat(d3d_desc.Format);
		Width=d3d_desc.Width;
		Height=d3d_desc.Height;
		Depth=d3d_desc.Depth;
	}
}
#endif

