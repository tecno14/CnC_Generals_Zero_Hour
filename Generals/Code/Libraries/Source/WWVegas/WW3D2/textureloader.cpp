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

#ifdef ZH
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : DX8 Texture Manager                                          *
 *                                                                                             *
 *                     $Archive:: /Commando/Code/ww3d2/textureloader.h                            $*
 *                                                                                             *
 *              Original Author:: vss_sync                                                   *
 *                                                                                             *
 *                       Author : Kenny Mitchell                                               * 
 *                                                                                             * 
 *								$Modtime:: 08/05/02 10:03a                                             $*
 *                                                                                             *
 *                    $Revision:: 3                                                           $*
 *                                                                                             *
 * 06/27/02 KM Texture class abstraction																			*
 * 08/05/02 KM Texture class redesign (revisited)
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#endif
#include "textureloader.h"
#include "mutex.h"
#include "thread.h"
#include "wwdebug.h"
#include "texture.h"
#include "ffactory.h"
#include "wwstring.h"
#include	"bufffile.h"
#include "ww3d.h"
#include "texfcach.h"
#include "assetmgr.h"
#include "dx8wrapper.h"
#include "dx8caps.h"
#include "missingtexture.h"
#include "targa.h"
#include <D3dx8tex.h>
#include <cstdio>
#include "wwmemlog.h"
#include "texture.h"
#include "formconv.h"
#include "texturethumbnail.h"
#include "ddsfile.h"
#include "bitmaphandler.h"
#ifdef ZH
#include "wwprofile.h"

//#pragma optimize("", off)
//#pragma MESSAGE("************************************** WARNING, optimization disabled for debugging purposes")

bool TextureLoader::TextureLoadSuspended;
int TextureLoader::TextureInactiveOverrideTime = 0;

#define USE_MANAGED_TEXTURES

////////////////////////////////////////////////////////////////////////////////
// 
// TextureLoadTaskListClass implementation
// 
////////////////////////////////////////////////////////////////////////////////

TextureLoadTaskListClass::TextureLoadTaskListClass(void)
: Root()
{
	Root.Next = Root.Prev = &Root;
}

void TextureLoadTaskListClass::Push_Front	(TextureLoadTaskClass *task)
{
	// task should non-null and not on any list
	WWASSERT(task != NULL && task->Next == NULL && task->Prev == NULL);

	// update inserted task to point to list
	task->Next			= Root.Next;
	task->Prev			= &Root;
	task->List			= this;

	// update list to point to inserted task
	Root.Next->Prev	= task;
	Root.Next			= task;
}

void TextureLoadTaskListClass::Push_Back(TextureLoadTaskClass *task)
{
	// task should be non-null and not on any list
	WWASSERT(task != NULL && task->Next == NULL && task->Prev == NULL);

	// update inserted task to point to list
	task->Next			= &Root;
	task->Prev			= Root.Prev;
	task->List			= this;

	// update list to point to inserted task
	Root.Prev->Next	= task;
	Root.Prev			= task;
}

TextureLoadTaskClass *TextureLoadTaskListClass::Pop_Front(void)
{
	// exit early if list is empty
	if (Is_Empty()) {
		return 0;
	}

	// otherwise, grab first task and remove it.
	TextureLoadTaskClass *task = (TextureLoadTaskClass *)Root.Next;
	Remove(task);
	return task;

}

TextureLoadTaskClass *TextureLoadTaskListClass::Pop_Back(void)
{
	// exit early if list is empty
	if (Is_Empty()) { 
		return 0;
	}

	// otherwise, grab last task and remove it.
	TextureLoadTaskClass *task = (TextureLoadTaskClass *)Root.Prev;
	Remove(task);
	return task;
}

void TextureLoadTaskListClass::Remove(TextureLoadTaskClass *task)
{
	// exit early if task is not on this list.
	if (task->List != this) {
		return;
	}

	// update list to skip task
	task->Prev->Next = task->Next;
	task->Next->Prev = task->Prev;

	// update task to no longer point at list
	task->Prev	= 0;
	task->Next	= 0;
	task->List	= 0;
}

////////////////////////////////////////////////////////////////////////////////
// 
// SynchronizedTextureLoadTaskListClass implementation
// 
////////////////////////////////////////////////////////////////////////////////

SynchronizedTextureLoadTaskListClass::SynchronizedTextureLoadTaskListClass(void)
:	TextureLoadTaskListClass(),
	CriticalSection()
{
}

void SynchronizedTextureLoadTaskListClass::Push_Front(TextureLoadTaskClass *task)
{
	FastCriticalSectionClass::LockClass lock(CriticalSection);
	TextureLoadTaskListClass::Push_Front(task);
}

void SynchronizedTextureLoadTaskListClass::Push_Back(TextureLoadTaskClass *task)
{
	FastCriticalSectionClass::LockClass lock(CriticalSection);
	TextureLoadTaskListClass::Push_Back(task);
}

TextureLoadTaskClass *SynchronizedTextureLoadTaskListClass::Pop_Front(void)
{
	// this duplicates code inside base class, but saves us an unnecessary lock.
	if (Is_Empty()) {
		return 0;
	}

	FastCriticalSectionClass::LockClass lock(CriticalSection);
	return TextureLoadTaskListClass::Pop_Front();

}

TextureLoadTaskClass *SynchronizedTextureLoadTaskListClass::Pop_Back(void)
{
	// this duplicates code inside base class, but saves us an unnecessary lock.
	if (Is_Empty()) {
		return 0;
	}

	FastCriticalSectionClass::LockClass lock(CriticalSection);
	return TextureLoadTaskListClass::Pop_Back();
}

void SynchronizedTextureLoadTaskListClass::Remove(TextureLoadTaskClass *task)
{
	FastCriticalSectionClass::LockClass lock(CriticalSection);
	TextureLoadTaskListClass::Remove(task);
}

// Locks

// To prevent deadlock, threads should acquire locks in the order in which
// they are defined below. No ordering is necessary for the task list locks,
// since one thread can never hold two at once.

static FastCriticalSectionClass					_ForegroundCriticalSection;
static FastCriticalSectionClass					_BackgroundCriticalSection;

// Lists

static SynchronizedTextureLoadTaskListClass	_ForegroundQueue;
static SynchronizedTextureLoadTaskListClass	_BackgroundQueue;
#endif

#ifdef OG
static TextureLoadTaskClass* LoadListHead;
static TextureLoadTaskClass* DeferredListHead;
static TextureLoadTaskClass* FinishedListHead;
static TextureLoadTaskClass* ThumbnailListHead;
static TextureLoadTaskClass* DeleteTaskListHead;

#endif
#ifdef ZH
static TextureLoadTaskListClass					_TexLoadFreeList;
static TextureLoadTaskListClass					_CubeTexLoadFreeList;
static TextureLoadTaskListClass					_VolTexLoadFreeList;

// The background texture loading thread.
static class LoaderThreadClass : public ThreadClass
{
public:
#ifdef Exception_Handler
	LoaderThreadClass(const char *thread_name = "Texture loader thread") : ThreadClass(thread_name, &Exception_Handler) {}
#else
	LoaderThreadClass(const char *thread_name = "Texture loader thread") : ThreadClass(thread_name) {}
#endif

	void Thread_Function();
} _TextureLoadThread;

// TODO: Legacy - remove this call!
IDirect3DTexture8* Load_Compressed_Texture(
	const StringClass& filename,
	unsigned reduction_factor,
	MipCountType mip_level_count,
	WW3DFormat dest_format)
{
	// If DDS file isn't available, use TGA file to convert to DDS.

	DDSFileClass dds_file(filename,reduction_factor);
	if (!dds_file.Is_Available()) return NULL;
	if (!dds_file.Load()) return NULL;

	unsigned width=dds_file.Get_Width(0);
	unsigned height=dds_file.Get_Height(0);
	unsigned mips=dds_file.Get_Mip_Level_Count();

	// If format isn't defined get the nearest valid texture format to the compressed file format
	// Note that the nearest valid format could be anything, even uncompressed.
	if (dest_format==WW3D_FORMAT_UNKNOWN) dest_format=Get_Valid_Texture_Format(dds_file.Get_Format(),true);

	IDirect3DTexture8* d3d_texture = DX8Wrapper::_Create_DX8_Texture
	(
		width,
		height,
		dest_format,
		(MipCountType)mips
	);
#endif

#ifdef OG
TextureLoadTaskClass* TextureLoadTaskClass::FreeTaskListHead;

#endif
#ifdef ZH
	for (unsigned level=0;level<mips;++level) {
		IDirect3DSurface8* d3d_surface=NULL;
		WWASSERT(d3d_texture);
		DX8_ErrorCode(d3d_texture->GetSurfaceLevel(level/*-reduction_factor*/,&d3d_surface));
		dds_file.Copy_Level_To_Surface(level,d3d_surface);
		d3d_surface->Release();
	}
	return d3d_texture;
}
#endif

static bool Is_Format_Compressed(WW3DFormat texture_format,bool allow_compression)
{
	// Verify that the user isn't requesting compressed texture without hardware support

	bool compressed=false;
	if (texture_format!=WW3D_FORMAT_UNKNOWN) {
#ifdef OG
		if (!DX8Caps::Support_DXTC() || !allow_compression) {
#endif
#ifdef ZH
		if (!DX8Wrapper::Get_Current_Caps()->Support_DXTC() || !allow_compression) {
#endif
			WWASSERT(texture_format!=WW3D_FORMAT_DXT1);
			WWASSERT(texture_format!=WW3D_FORMAT_DXT2);
			WWASSERT(texture_format!=WW3D_FORMAT_DXT3);
			WWASSERT(texture_format!=WW3D_FORMAT_DXT4);
			WWASSERT(texture_format!=WW3D_FORMAT_DXT5);
		}
		if (texture_format==WW3D_FORMAT_DXT1 ||
			texture_format==WW3D_FORMAT_DXT2 ||
			texture_format==WW3D_FORMAT_DXT3 ||
			texture_format==WW3D_FORMAT_DXT4 ||
			texture_format==WW3D_FORMAT_DXT5) {
			compressed=true;
		}
	}

	// If hardware supports DXTC compression, load a compressed texture. Proceed only if the texture format hasn't been
	// defined as non-compressed.
	compressed|=(
		texture_format==WW3D_FORMAT_UNKNOWN && 
#ifdef OG
		DX8Caps::Support_DXTC() && 
		WW3D::Get_Texture_Compression_Mode()==WW3D::TEXTURE_COMPRESSION_ENABLE &&
#endif
#ifdef ZH
		DX8Wrapper::Get_Current_Caps()->Support_DXTC() &&

#endif
		allow_compression);

	return compressed;
}

#ifdef OG
// ----------------------------------------------------------------------------

#endif
#ifdef ZH
////////////////////////////////////////////////////////////////////////////////
// 
// TextureLoader implementation
// 
////////////////////////////////////////////////////////////////////////////////
#endif

#ifdef OG
static CriticalSectionClass mutex;
static class LoaderThreadClass : public ThreadClass
#endif
#ifdef ZH
void TextureLoader::Init()

#endif
{
#ifdef OG
	TextureLoadTaskClass* Get_Task_From_Load_List();
#endif
#ifdef ZH
	WWASSERT(!_TextureLoadThread.Is_Running());
#endif

#ifdef OG
	static void Add_Task_To_Finished_List(TextureLoadTaskClass* task);
#endif
#ifdef ZH
	ThumbnailManagerClass::Init();
#endif

#ifdef OG
public:
	LoaderThreadClass::LoaderThreadClass() : ThreadClass() {}

#endif
#ifdef ZH
	_TextureLoadThread.Execute();
	_TextureLoadThread.Set_Priority(-4);
	TextureInactiveOverrideTime = 0;
}
#endif

#ifdef OG
	void Thread_Function();
#endif

#ifdef OG
	static void Add_Task_To_Load_List(TextureLoadTaskClass* task);

} thread;

// ----------------------------------------------------------------------------

void TextureLoader::Init()
#endif
#ifdef ZH
void TextureLoader::Deinit()

#endif
{
#ifdef OG
	WWASSERT(!thread.Is_Running());

#endif
#ifdef ZH
	FastCriticalSectionClass::LockClass lock(_BackgroundCriticalSection);
	_TextureLoadThread.Stop();
#endif

#ifdef OG
	ThumbnailClass::Init();

	thread.Execute();
	thread.Set_Priority(-3);
#endif
#ifdef ZH
	ThumbnailManagerClass::Deinit();
	TextureLoadTaskClass::Delete_Free_Pool();

#endif
}

#ifdef OG
// ----------------------------------------------------------------------------
#endif

#ifdef OG
void TextureLoader::Deinit()
#endif
#ifdef ZH
bool TextureLoader::Is_DX8_Thread(void)
#endif
{
#ifdef OG
	CriticalSectionClass::LockClass m(mutex);
	thread.Stop();
#endif
#ifdef ZH
	return (ThreadClass::_Get_Current_Thread_ID() == DX8Wrapper::_Get_Main_Thread_ID());
}
#endif

#ifdef OG
	ThumbnailClass::Deinit();
}
#endif

// ----------------------------------------------------------------------------
//
// Modify given texture size to nearest valid size on current hardware.
//
// ----------------------------------------------------------------------------

#ifdef OG
void TextureLoader::Validate_Texture_Size(unsigned& width, unsigned& height)

#endif
#ifdef ZH
void TextureLoader::Validate_Texture_Size
(
	unsigned& width, 
	unsigned& height,
	unsigned& depth
)
#endif
{
#ifdef OG
	const D3DCAPS8& dx8caps=DX8Caps::Get_Default_Caps();
#endif
#ifdef ZH
	const D3DCAPS8& dx8caps=DX8Wrapper::Get_Current_Caps()->Get_DX8_Caps();
#endif

	unsigned poweroftwowidth = 1;
#ifdef OG
	while (poweroftwowidth < width) {

#endif
#ifdef ZH
	while (poweroftwowidth < width) 
	{
#endif
		poweroftwowidth <<= 1;
	}

	unsigned poweroftwoheight = 1;
#ifdef OG
	while (poweroftwoheight < height) {

#endif
#ifdef ZH
	while (poweroftwoheight < height) 
	{
#endif
		poweroftwoheight <<= 1;
	}

#ifdef OG
//	unsigned size = MAX (width, height);
//	unsigned poweroftwosize = 1;
//	while (poweroftwosize < size) {
//		poweroftwosize <<= 1;
//	}
#endif
#ifdef ZH
	unsigned poweroftwodepth = 1;
	while (poweroftwodepth< depth)
	{
		poweroftwodepth <<= 1;
	}
#endif

#ifdef OG
	if (poweroftwowidth>dx8caps.MaxTextureWidth) {

#endif
#ifdef ZH
	if (poweroftwowidth>dx8caps.MaxTextureWidth) 
	{
#endif
		poweroftwowidth=dx8caps.MaxTextureWidth;
	}
#ifdef OG
	if (poweroftwoheight>dx8caps.MaxTextureHeight) {

#endif
#ifdef ZH
	if (poweroftwoheight>dx8caps.MaxTextureHeight) 
	{
#endif
		poweroftwoheight=dx8caps.MaxTextureHeight;
	}
#ifdef ZH
	if (poweroftwodepth>dx8caps.MaxVolumeExtent)
	{
		poweroftwodepth=dx8caps.MaxVolumeExtent;
	}
#endif

#ifdef OG
	if (poweroftwowidth>poweroftwoheight) {
		while (poweroftwowidth/poweroftwoheight>8) {

#endif
#ifdef ZH
	if (poweroftwowidth>poweroftwoheight) 
	{
		while (poweroftwowidth/poweroftwoheight>8) 
		{
#endif
			poweroftwoheight*=2;
		}
	}
#ifdef OG
	else {
		while (poweroftwoheight/poweroftwowidth>8) {

#endif
#ifdef ZH
	else 
	{
		while (poweroftwoheight/poweroftwowidth>8) 
		{
#endif
			poweroftwowidth*=2;
		}
	}

#ifdef OG
//	width = height = poweroftwosize;
#endif
	width=poweroftwowidth;
	height=poweroftwoheight;
#ifdef ZH
	depth=poweroftwodepth;
#endif
}

#ifdef ZH
IDirect3DTexture8* TextureLoader::Load_Thumbnail(const StringClass& filename, const Vector3& hsv_shift)//,WW3DFormat texture_format)
{
	WWASSERT(Is_DX8_Thread());
#endif

#ifdef ZH
	ThumbnailClass* thumb=NULL;
	thumb=ThumbnailManagerClass::Peek_Thumbnail_Instance_From_Any_Manager(filename);
#endif

#ifdef OG
IDirect3DTexture8* TextureLoader::Load_Thumbnail(const StringClass& filename,WW3DFormat texture_format)
{
	ThumbnailClass* thumb=ThumbnailClass::Peek_Instance(filename);
#endif
#ifdef ZH
	// If no thumb is found return a missing texture

#endif
	if (!thumb) {
#ifdef OG
		thumb=W3DNEW ThumbnailClass(filename);
		// If load failed, return missing texture
		if (!thumb->Peek_Bitmap()) {
			delete thumb;
#endif
			return MissingTexture::_Get_Missing_Texture();
#ifdef OG
		}
#endif
	}

#ifdef OG
	unsigned src_pitch=thumb->Get_Width()*4;	// Thumbs are always 32 bits

#endif
#ifdef ZH
	WWASSERT(thumb->Get_Format()==WW3D_FORMAT_A4R4G4B4);
	unsigned src_pitch=thumb->Get_Width()*2;	// Thumbs are always 16 bits
#endif
	WW3DFormat dest_format;
#ifdef ZH
	WW3DFormat texture_format=WW3D_FORMAT_UNKNOWN;
#endif
	if (texture_format==WW3D_FORMAT_UNKNOWN) {
#ifdef OG
		dest_format=Get_Valid_Texture_Format(WW3D_FORMAT_A8R8G8B8,false); // no compressed formats please
#endif
#ifdef ZH
		dest_format=Get_Valid_Texture_Format(WW3D_FORMAT_A4R4G4B4,false); // no compressed formats please
#endif
	}
	else {
		dest_format=Get_Valid_Texture_Format(texture_format,false);	// no compressed formats please
		WWASSERT(dest_format==texture_format);
	}

#ifdef OG
	IDirect3DTexture8* d3d_texture = DX8Wrapper::_Create_DX8_Texture(
#endif
#ifdef ZH
	IDirect3DTexture8* sysmem_texture = DX8Wrapper::_Create_DX8_Texture(
#endif
		thumb->Get_Width(),
		thumb->Get_Height(),
		dest_format,
#ifdef OG
		TextureClass::MIP_LEVELS_ALL);

#endif
#ifdef ZH
		MIP_LEVELS_ALL,
#ifdef USE_MANAGED_TEXTURES
		D3DPOOL_MANAGED);
#else
		D3DPOOL_SYSTEMMEM);
#endif
#endif

	unsigned level=0;
	D3DLOCKED_RECT locked_rects[12];
#ifdef OG
	WWASSERT(d3d_texture->GetLevelCount()<=12);
#endif
#ifdef ZH
	WWASSERT(sysmem_texture->GetLevelCount()<=12);
#endif

	// Lock all surfaces
#ifdef OG
	for (level=0;level<d3d_texture->GetLevelCount();++level) {
#endif
#ifdef ZH
	for (level=0;level<sysmem_texture->GetLevelCount();++level) {
#endif
		DX8_ErrorCode(
#ifdef OG
			d3d_texture->LockRect(
#endif
#ifdef ZH
			sysmem_texture->LockRect(
#endif
				level,
				&locked_rects[level],
				NULL,
				0));
	}

	unsigned char* src_surface=thumb->Peek_Bitmap();
#ifdef OG
	WW3DFormat src_format=WW3D_FORMAT_A8R8G8B8;
#endif
#ifdef ZH
	WW3DFormat src_format=thumb->Get_Format();
#endif
	unsigned width=thumb->Get_Width();
	unsigned height=thumb->Get_Height();

#ifdef OG
	for (level=0;level<d3d_texture->GetLevelCount()-1;++level) {

#endif
#ifdef ZH
	Vector3 hsv=hsv_shift;
	for (level=0;level<sysmem_texture->GetLevelCount()-1;++level) {
#endif
		BitmapHandlerClass::Copy_Image_Generate_Mipmap(
			width,
			height,
			(unsigned char*)locked_rects[level].pBits, 
			locked_rects[level].Pitch,
			dest_format,
			src_surface,
			src_pitch,
			src_format,
			(unsigned char*)locked_rects[level+1].pBits,	// mipmap
#ifdef OG
			locked_rects[level+1].Pitch);// mipmap

#endif
#ifdef ZH
			locked_rects[level+1].Pitch,
			hsv);
		hsv=Vector3(0.0f,0.0f,0.0f);	// Only do the shift for the first level, as the mipmaps are based on it.
#endif

		src_format=dest_format;
		src_surface=(unsigned char*)locked_rects[level].pBits;
		src_pitch=locked_rects[level].Pitch;
		width>>=1;
		height>>=1;
	}

	// Unlock all surfaces
#ifdef OG
	for (level=0;level<d3d_texture->GetLevelCount();++level) {
		DX8_ErrorCode(d3d_texture->UnlockRect(level));
	}

	return d3d_texture;
#endif
#ifdef ZH
	for (level=0;level<sysmem_texture->GetLevelCount();++level) {
		DX8_ErrorCode(sysmem_texture->UnlockRect(level));

#endif
}
#ifdef OG

static bool Is_Power_Of_Two(unsigned i)
{
	if (!i) return false;
	unsigned n=i;
	unsigned shift=0;
	while (n) {
		shift++;
		n>>=1;
	}
	return ((i>>(shift-1))<<(shift-1))==i;
}

// ----------------------------------------------------------------------------

// TODO: Legacy - remove this call!
IDirect3DTexture8* Load_Compressed_Texture(
	const StringClass& filename, 
	unsigned reduction_factor,
	TextureClass::MipCountType mip_level_count,
	WW3DFormat dest_format)
{
	// If DDS file isn't available, use TGA file to convert to DDS.

	DDSFileClass dds_file(filename,reduction_factor);
	if (!dds_file.Is_Available()) return NULL;
	if (!dds_file.Load()) return NULL;

	unsigned width=dds_file.Get_Width(0);
	unsigned height=dds_file.Get_Height(0);
	unsigned mips=dds_file.Get_Mip_Level_Count();

	// If format isn't defined get the nearest valid texture format to the compressed file format
	// Note that the nearest valid format could be anything, even uncompressed.
	if (dest_format==WW3D_FORMAT_UNKNOWN) dest_format=Get_Valid_Texture_Format(dds_file.Get_Format(),true);

#endif
#ifdef ZH
#ifdef USE_MANAGED_TEXTURES
	return sysmem_texture;
#else

#endif
	IDirect3DTexture8* d3d_texture = DX8Wrapper::_Create_DX8_Texture(
#ifdef OG
		width,
		height,
#endif
#ifdef ZH
		thumb->Get_Width(),
		thumb->Get_Height(),
#endif
		dest_format,
#ifdef OG
		(TextureClass::MipCountType)mips);

#endif
#ifdef ZH
		TextureBaseClass::MIP_LEVELS_ALL,
		D3DPOOL_DEFAULT);
	DX8CALL(UpdateTexture(sysmem_texture,d3d_texture));
	sysmem_texture->Release();
#endif

#ifdef OG
	for (unsigned level=0;level<mips;++level) {
		IDirect3DSurface8* d3d_surface=NULL;
		WWASSERT(d3d_texture);
		DX8_ErrorCode(d3d_texture->GetSurfaceLevel(level/*-reduction_factor*/,&d3d_surface));
		dds_file.Copy_Level_To_Surface(level,d3d_surface);
		d3d_surface->Release();
	}
#endif
#ifdef ZH
	WWDEBUG_SAY(("Created non-managed texture (%s)\n",filename));

#endif
	return d3d_texture;
#ifdef ZH
#endif
#endif
}

// ----------------------------------------------------------------------------
//
// Load image to a surface. The function tries to create texture that matches
// targa format. If suitable format is not available, it selects closest matching
// format and performs color space conversion.
//
// ----------------------------------------------------------------------------
IDirect3DSurface8* TextureLoader::Load_Surface_Immediate(
	const StringClass& filename,
	WW3DFormat texture_format,
	bool allow_compression)
{
#ifdef ZH
	WWASSERT(Is_DX8_Thread());

#endif
	bool compressed=Is_Format_Compressed(texture_format,allow_compression);

	if (compressed) {
#ifdef OG
		IDirect3DTexture8* comp_tex=Load_Compressed_Texture(filename,0,TextureClass::MIP_LEVELS_1,WW3D_FORMAT_UNKNOWN);
#endif
#ifdef ZH
		IDirect3DTexture8* comp_tex=Load_Compressed_Texture(filename,0,MIP_LEVELS_1,WW3D_FORMAT_UNKNOWN);
#endif
		if (comp_tex) {
			IDirect3DSurface8* d3d_surface=NULL;
			DX8_ErrorCode(comp_tex->GetSurfaceLevel(0,&d3d_surface));
			comp_tex->Release();
			return d3d_surface;
		}
	}

	// Make sure the file can be opened. If not, return missing texture.
	Targa targa;
	if (TARGA_ERROR_HANDLER(targa.Open(filename, TGA_READMODE),filename)) return MissingTexture::_Create_Missing_Surface();

	// DX8 uses image upside down compared to TGA
	targa.Header.ImageDescriptor ^= TGAIDF_YORIGIN;

	WW3DFormat src_format,dest_format;
	unsigned src_bpp=0;
	Get_WW3D_Format(dest_format,src_format,src_bpp,targa);

	if (texture_format!=WW3D_FORMAT_UNKNOWN) {
		dest_format=texture_format;
	}

	// Destination size will be the next power of two square from the larger width and height...
	unsigned width, height;
	width=targa.Header.Width;
	height=targa.Header.Height;
	unsigned src_width=targa.Header.Width;
	unsigned src_height=targa.Header.Height;

	// NOTE: We load the palette but we do not yet support paletted textures!
	char palette[256*4];
	targa.SetPalette(palette);
	if (TARGA_ERROR_HANDLER(targa.Load(filename, TGAF_IMAGE, false),filename)) return MissingTexture::_Create_Missing_Surface();

	unsigned char* src_surface=(unsigned char*)targa.GetImage();

	// No paletted destination format allowed
	unsigned char* converted_surface=NULL;
	if (src_format==WW3D_FORMAT_A1R5G5B5 || src_format==WW3D_FORMAT_R5G6B5 || src_format==WW3D_FORMAT_A4R4G4B4 ||
		src_format==WW3D_FORMAT_P8 || src_format==WW3D_FORMAT_L8 || src_width!=width || src_height!=height) {
		converted_surface=W3DNEWARRAY unsigned char[width*height*4];
		dest_format=Get_Valid_Texture_Format(WW3D_FORMAT_A8R8G8B8,false);
		BitmapHandlerClass::Copy_Image(
			converted_surface, 
			width,
			height,
			width*4,
			WW3D_FORMAT_A8R8G8B8,//dest_format,
			src_surface,
			src_width,
			src_height,
			src_width*src_bpp,
			src_format,
			(unsigned char*)targa.GetPalette(),
			targa.Header.CMapDepth>>3,
			false);
		src_surface=converted_surface;
		src_format=WW3D_FORMAT_A8R8G8B8;//dest_format;
		src_width=width;
		src_height=height;
		src_bpp=Get_Bytes_Per_Pixel(src_format);
	}

	unsigned src_pitch=src_width*src_bpp;

	IDirect3DSurface8* d3d_surface = DX8Wrapper::_Create_DX8_Surface(width,height,dest_format);
	WWASSERT(d3d_surface);
	D3DLOCKED_RECT locked_rect;
	DX8_ErrorCode(
		d3d_surface->LockRect(
			&locked_rect,
			NULL,
			0));

	BitmapHandlerClass::Copy_Image(
		(unsigned char*)locked_rect.pBits, 
		width,
		height,
		locked_rect.Pitch,
		dest_format,
		src_surface,
		src_width,
		src_height,
		src_pitch,
		src_format,
		(unsigned char*)targa.GetPalette(),
		targa.Header.CMapDepth>>3,
		false);	// No mipmap

	DX8_ErrorCode(d3d_surface->UnlockRect());

	if (converted_surface) delete[] converted_surface;

	return d3d_surface;
#ifdef ZH
}

void TextureLoader::Request_Thumbnail(TextureBaseClass *tc)
{
	// Grab the foreground lock. This prevents the foreground thread
	// from retiring any tasks related to this texture. It also
	// serializes calls to Request_Thumbnail from multiple threads.
	FastCriticalSectionClass::LockClass lock(_ForegroundCriticalSection);

	// Has a Direct3D texture already been loaded?
	if (tc->Peek_D3D_Base_Texture()) {
		return;
#endif
}
#ifdef ZH

	TextureLoadTaskClass *task = tc->ThumbnailLoadTask;

	if (Is_DX8_Thread()) {
		// load the thumbnail immediately
		TextureLoader::Load_Thumbnail(tc);

		// clear any pending thumbnail load
		if (task) {
			_ForegroundQueue.Remove(task);
			task->Destroy();
		}

	} else {
		TextureLoadTaskClass *load_task = tc->TextureLoadTask;

		// if texture is not already loading a thumbnail and there is no
		// background load near completion. (a background load waiting
		// to be applied will be ready at the same time as a queued thumbnail.
		// Why do the extra work?)
		if (!task && (!load_task || load_task->Get_State() < TextureLoadTaskClass::STATE_LOAD_MIPMAP)) {
#endif

#ifdef ZH
			// create a thumbnail load task and add to foreground queue.
			task = TextureLoadTaskClass::Create(tc, TextureLoadTaskClass::TASK_THUMBNAIL, TextureLoadTaskClass::PRIORITY_LOW);
			_ForegroundQueue.Push_Back(task);
		}
	}
}
#endif

#ifdef OG
// ----------------------------------------------------------------------------
//
// Load mipmap levels to a pre-generated and locked texture object based on
// information in load task object. Try loading from a DDS file first and if
// that fails try a TGA.
//
// ----------------------------------------------------------------------------
#endif

#ifdef OG
void TextureLoader::Load_Mipmap_Levels(TextureLoadTaskClass* task)
#endif
#ifdef ZH
void TextureLoader::Request_Background_Loading(TextureBaseClass *tc)
#endif
{
#ifdef OG
	WWASSERT(task->Peek_D3D_Texture());
	WWMEMLOG(MEM_TEXTURE);

#endif
#ifdef ZH
	WWPROFILE(("TextureLoader::Request_Background_Loading()"));
	// Grab the foreground lock. This prevents the foreground thread
	// from retiring any tasks related to this texture. It also 
	// serializes calls to Request_Background_Loading from other
	// threads.
	FastCriticalSectionClass::LockClass foreground_lock(_ForegroundCriticalSection);
#endif

#ifdef OG
	if (task->Peek_Texture()->Is_Compression_Allowed()) {
		DDSFileClass dds_file(task->Peek_Texture()->Get_Full_Path(),task->Get_Reduction());
		if (dds_file.Is_Available() && dds_file.Load()) {
			unsigned width=task->Get_Width();
			unsigned height=task->Get_Height();
			for (unsigned level=0;level<task->Get_Mip_Level_Count();++level) {
				WWASSERT(width && height);
				dds_file.Copy_Level_To_Surface(
					level,
					task->Get_Format(),
					width, 
					height, 
					task->Get_Locked_Surface_Ptr(level), 
					task->Get_Locked_Surface_Pitch(level));
				width>>=1;
				height>>=1;
#endif
#ifdef ZH
	// Has the texture already been loaded?
	if (tc->Is_Initialized()) {
		return;

#endif
			}
#ifdef ZH

	TextureLoadTaskClass *task = tc->TextureLoadTask;

	// if texture already has a load task, we don't need to create another one.
	if (task) {
#endif
			return;
		}
#ifdef ZH

	task = TextureLoadTaskClass::Create(tc, TextureLoadTaskClass::TASK_LOAD, TextureLoadTaskClass::PRIORITY_LOW);
	
	if (Is_DX8_Thread()) {
		Begin_Load_And_Queue(task);
	} else {
		_ForegroundQueue.Push_Back(task);
#endif
	}
#ifdef OG
	if (Load_Uncompressed_Mipmap_Levels_From_TGA(task)) return;
#endif
}

#ifdef OG
// ----------------------------------------------------------------------------
//
// Use load task to load texture surface from a targa file. Calculate mipmaps
// if needed.
//
// ----------------------------------------------------------------------------
#endif

#ifdef OG
bool TextureLoader::Load_Uncompressed_Mipmap_Levels_From_TGA(TextureLoadTaskClass* task)
#endif
#ifdef ZH
void TextureLoader::Request_Foreground_Loading(TextureBaseClass *tc)
#endif
{
#ifdef OG
	if (!task->Get_Mip_Level_Count()) return false;
	TextureClass* texture=task->Peek_Texture();

#endif
#ifdef ZH
	WWPROFILE(("TextureLoader::Request_Foreground_Loading()"));
	// Grab the foreground lock. This prevents the foreground thread
	// from retiring the load tasks for this texture. It also 
	// serializes calls to Request_Foreground_Loading from other
	// threads.
	FastCriticalSectionClass::LockClass foreground_lock(_ForegroundCriticalSection);
#endif

#ifdef OG
	Targa targa;
	if (TARGA_ERROR_HANDLER(targa.Open(texture->Get_Full_Path(), TGA_READMODE),texture->Get_Full_Path())) {
		task->Set_Fail(true);
		return false;
#endif
#ifdef ZH
	// Has the texture already been loaded?
	if (tc->Is_Initialized()) {
		return;

#endif
	}
#ifdef OG
	// DX8 uses image upside down compared to TGA
	targa.Header.ImageDescriptor ^= TGAIDF_YORIGIN;
#endif

#ifdef OG
	WW3DFormat src_format,dest_format;
	unsigned src_bpp=0;
	Get_WW3D_Format(dest_format,src_format,src_bpp,targa);
//	WWASSERT(task->Get_Format()==dest_format);
	dest_format=task->Get_Format();	// Texture can be requested in different format than the most obvious from the TGA
#endif
#ifdef ZH
	TextureLoadTaskClass *task			= tc->TextureLoadTask;
	TextureLoadTaskClass *task_thumb = tc->ThumbnailLoadTask;

#endif

#ifdef OG
	char palette[256*4];
	targa.SetPalette(palette);
#endif
#ifdef ZH
	if (Is_DX8_Thread()) {

#endif

#ifdef OG
	unsigned src_width=targa.Header.Width;
	unsigned src_height=targa.Header.Height;
	unsigned width=task->Get_Width();
	unsigned height=task->Get_Height();
#endif
#ifdef ZH
		// since we're in the DX8 thread, we can load the entire
		// texture right now.

#endif

#ifdef OG
	// NOTE: We load the palette but we do not yet support paletted textures!
	if (TARGA_ERROR_HANDLER(targa.Load(texture->Get_Full_Path(), TGAF_IMAGE, false),texture->Get_Full_Path())) {
		task->Set_Fail(true);
		return false;
#endif
#ifdef ZH
		// if we have a thumbnail task waiting, kill it.
		if (task_thumb) {
			_ForegroundQueue.Remove(task_thumb);
			task_thumb->Destroy();
#endif
	}
#ifdef OG
	unsigned char* src_surface=(unsigned char*)targa.GetImage();

#endif
#ifdef ZH

		if (task) {
			// we need to remove the task from any queue, since we're going
			// to finish it up right now.
#endif

#ifdef OG
	// No paletted format allowed when generating mipmaps
	unsigned char* converted_surface=NULL;
	if (src_format==WW3D_FORMAT_A1R5G5B5 || src_format==WW3D_FORMAT_R5G6B5 || src_format==WW3D_FORMAT_A4R4G4B4 ||
		src_format==WW3D_FORMAT_P8 || src_format==WW3D_FORMAT_L8 || src_width!=width || src_height!=height) {
		converted_surface=W3DNEWARRAY unsigned char[width*height*4];
		dest_format=Get_Valid_Texture_Format(WW3D_FORMAT_A8R8G8B8,false);
		BitmapHandlerClass::Copy_Image(
			converted_surface, 
			width,
			height,
			width*4,
			WW3D_FORMAT_A8R8G8B8,	//dest_format,
			src_surface,
			src_width,
			src_height,
			src_width*src_bpp,
			src_format,
			(unsigned char*)targa.GetPalette(),
			targa.Header.CMapDepth>>3,
			false);
		src_surface=converted_surface;
		src_format=WW3D_FORMAT_A8R8G8B8;	//dest_format;
		src_width=width;
		src_height=height;
		src_bpp=Get_Bytes_Per_Pixel(src_format);
#endif
#ifdef ZH
			// halt background thread. After we're holding this lock,
			// we know the background thread cannot begin loading
			// mipmap levels for this texture.
			FastCriticalSectionClass::LockClass background_lock(_BackgroundCriticalSection);
			_ForegroundQueue.Remove(task);
			_BackgroundQueue.Remove(task);
		} else {
			// Since the task manages all the state associated with loading
			// a texture, we temporarily create one.
			task = TextureLoadTaskClass::Create(tc, TextureLoadTaskClass::TASK_LOAD, TextureLoadTaskClass::PRIORITY_HIGH);

#endif
	}
#ifdef ZH

		// finish loading the task and destroy it.
		task->Finish_Load();
		task->Destroy();
#endif

#ifdef OG
	unsigned src_pitch=src_width*src_bpp;

#endif
#ifdef ZH
	} else {
		// we are not in the DX8 thread. We need to add a high-priority loading
		// task to the foreground queue.
#endif

#ifdef OG
	for (unsigned level=0;level<task->Get_Mip_Level_Count();++level) {
		WWASSERT(task->Get_Locked_Surface_Ptr(level));
		BitmapHandlerClass::Copy_Image(
			task->Get_Locked_Surface_Ptr(level),
			width,
			height,
			task->Get_Locked_Surface_Pitch(level),
			task->Get_Format(),
			src_surface,
			src_width,
			src_height,
			src_pitch,
			src_format,
			NULL,
			0,
			true);
#endif
#ifdef ZH
		// Grab the background lock. After we're holding this lock, we
		// know the background thread cannot begin loading mipmap levels 
		// for this texture.
		FastCriticalSectionClass::LockClass background_lock(_BackgroundCriticalSection);

#endif

#ifdef OG
		width>>=1;
		height>>=1;
		src_width>>=1;
		src_height>>=1;
		if (!width || !height || !src_width || !src_height) break;
#endif
#ifdef ZH
		// if we have a thumbnail task, we should cancel it. Since we are not
		// the foreground thread, we are not allowed to call Destroy(). Instead,
		// leave it queued in the completed state so it will be destroyed by Update().
		if (task_thumb) {
			task_thumb->Set_State(TextureLoadTaskClass::STATE_COMPLETE);
#endif
	}
#ifdef ZH

		if (task) {
			// if a load task is waiting on the background queue, we need to 
			// move it to the foreground queue.
			if (task->Get_List() == &_BackgroundQueue) {
#endif

#ifdef OG
	if (converted_surface) delete[] converted_surface;

#endif
#ifdef ZH
				// remove task from list
				_BackgroundQueue.Remove(task);
#endif

#ifdef OG
	return true;

#endif
#ifdef ZH
				// add to foreground queue.
				_ForegroundQueue.Push_Back(task);
#endif
}

#ifdef OG
// ----------------------------------------------------------------------------
//
// Return a task from the load list head. The loading thread uses this function
// to retrieve tasks from the load list.
//
// ----------------------------------------------------------------------------
#endif
#ifdef ZH
			// upgrade the task priority
			task->Set_Priority(TextureLoadTaskClass::PRIORITY_HIGH);

#endif

#ifdef OG
TextureLoadTaskClass* LoaderThreadClass::Get_Task_From_Load_List()
{
	CriticalSectionClass::LockClass m(mutex);
#endif
#ifdef ZH
		} else {
			// allocate high priority load task
			task = TextureLoadTaskClass::Create(tc, TextureLoadTaskClass::TASK_LOAD, TextureLoadTaskClass::PRIORITY_HIGH);
#endif

#ifdef OG
	TextureLoadTaskClass* task=LoadListHead;
	if (task) {
		LoadListHead=task->Peek_Succ();
		task->Set_Succ(NULL);
#endif
#ifdef ZH
			// add to back of foreground queue.
			_ForegroundQueue.Push_Back(task);
		}

#endif
	}
#ifdef OG
	return task;
#endif
}
#ifdef ZH


void TextureLoader::Flush_Pending_Load_Tasks(void)
{
	// This function can only be called from the main thread.
	// (Only the main thread can make the DX8 calls necessary
	// to complete texture loading. If we wanted to flush
	// the pending tasks from another thread, we'd probably
	// want to set a bool that is checked by Update().
	WWASSERT(Is_DX8_Thread());
#endif

#ifdef OG
// ----------------------------------------------------------------------------
//
// This function adds a load task to the head of the loading thread task list.
// The latest added task will be the next processed (There are good reasons
// for such ordering). The loading thread will process tasks from this list
// as soons as it can and then move the tasks to finished list.
//
// ----------------------------------------------------------------------------
#endif
#ifdef ZH
	for (;;) {
		bool done = false;

#endif

#ifdef OG
void LoaderThreadClass::Add_Task_To_Load_List(TextureLoadTaskClass* task)
#endif
{
#ifdef OG
	CriticalSectionClass::LockClass m(mutex);

#endif
#ifdef ZH
			// we have no pending load tasks when both queues are empty
			// and the background thread is not processing a texture.
			
			// Grab the background lock. Once we're holding it, we
			// know that the background thread is not processing any
			// textures.
#endif

#ifdef OG
	WWASSERT(task->Peek_Succ()==NULL);

#endif
#ifdef ZH
			// NOTE: It's important that we do only hold on to the background
			// lock while we check for completion. Otherwise, we will either
			// violate the lock order when we call Update() (which grabs
			// the foreground lock) or never give the background thread
			// a chance to empty its queue.
			FastCriticalSectionClass::LockClass background_lock(_BackgroundCriticalSection);
			done = _BackgroundQueue.Is_Empty() && _ForegroundQueue.Is_Empty();
		}

		// exit loop if no entries in list
		if (done) {
			break;
		}
#endif

#ifdef OG
	task->Set_Succ(LoadListHead);
	LoadListHead=task;

#endif
#ifdef ZH
		Update();
		ThreadClass::Switch_Thread();
	}
#endif
}
#ifdef ZH


// Nework update macro for texture loader.
#pragma warning(disable:4201) // warning C4201: nonstandard extension used : nameless struct/union
#include <mmsystem.h>
#define UPDATE_NETWORK 											\
	if (network_callback) {                            \
		unsigned long time2 = timeGetTime();            \
		if (time2 - time > 20) {                        \
			network_callback();                          \
			time = time2;                                \
		}                                               \
	}                                                  \
#endif

#ifdef OG
// ----------------------------------------------------------------------------
//
// After the loading thread is done with the texture, it is moved to the list
// of finished tasks so that the main thread can then finish up by unlocking
// the surfaces and applying the changes to the texture class object.
//
// ----------------------------------------------------------------------------
#endif

#ifdef OG
void LoaderThreadClass::Add_Task_To_Finished_List(TextureLoadTaskClass* task)
#endif
#ifdef ZH
void TextureLoader::Update(void (*network_callback)(void))
#endif
{
#ifdef OG
	CriticalSectionClass::LockClass m(mutex);

#endif
#ifdef ZH
	WWASSERT_PRINT(Is_DX8_Thread(), "TextureLoader::Update must be called from the main thread!");

	if (TextureLoadSuspended) {
		return;
	}

	// grab foreground lock to prevent any other thread from
	// modifying texture tasks.
	FastCriticalSectionClass::LockClass lock(_ForegroundCriticalSection);

	unsigned long time = timeGetTime();
#endif

#ifdef OG
	WWASSERT(task->Peek_Succ()==NULL);

#endif
#ifdef ZH
	// while we have tasks on the foreground queue
	while (TextureLoadTaskClass *task = _ForegroundQueue.Pop_Front()) {
		UPDATE_NETWORK;
		// dispatch to proper task handler
		switch (task->Get_Type()) {
			case TextureLoadTaskClass::TASK_THUMBNAIL:
				Process_Foreground_Thumbnail(task);
				break;
#endif

#ifdef OG
	task->Set_Succ(FinishedListHead);
	FinishedListHead=task;

#endif
#ifdef ZH
			case TextureLoadTaskClass::TASK_LOAD:
				Process_Foreground_Load(task);
				break;
#endif
}
#ifdef ZH
	}
#endif

#ifdef OG
// ----------------------------------------------------------------------------
//
// If we need to find out if the load task list is empty this is the function
// to use. We can't use Get_Task_From_Load_List() as if the list isn't empty
// it also removes the head node from the list.
//
// ----------------------------------------------------------------------------
#endif
#ifdef ZH
	TextureBaseClass::Invalidate_Old_Unused_Textures(TextureInactiveOverrideTime);
}

#endif

#ifdef OG
bool Is_Load_List_Empty()
#endif
#ifdef ZH
void TextureLoader::Suspend_Texture_Load()
#endif
{
#ifdef OG
	return !LoadListHead;

#endif
#ifdef ZH
	WWASSERT_PRINT(Is_DX8_Thread(),"TextureLoader::Suspend_Texture_Load must be called from the main thread!");
	TextureLoadSuspended=true;
#endif
}
#ifdef OG
// ----------------------------------------------------------------------------
//
// Texture loading thread loads textures that appear in loading_task_list.
// If the list is empty the thread sleeps.
//
// ----------------------------------------------------------------------------
#endif

#ifdef OG
void LoaderThreadClass::Thread_Function()
#endif
#ifdef ZH
void TextureLoader::Continue_Texture_Load()
#endif
{
#ifdef OG
	while (running) {
		TextureLoadTaskClass* task=Get_Task_From_Load_List();
		if (task) {
			TextureLoader::Load_Mipmap_Levels(task);
			Add_Task_To_Finished_List(task);
#endif
#ifdef ZH
	WWASSERT_PRINT(Is_DX8_Thread(),"TextureLoader::Continue_Texture_Load must be called from the main thread!");
	TextureLoadSuspended=false;

#endif
		}
#ifdef ZH

void TextureLoader::Process_Foreground_Thumbnail(TextureLoadTaskClass *task)
{
	switch (task->Get_State()) {
		case TextureLoadTaskClass::STATE_NONE:
			Load_Thumbnail(task->Peek_Texture());
			// NOTE: fall-through is intentional
#endif

#ifdef OG
		Switch_Thread();

#endif
#ifdef ZH
		case TextureLoadTaskClass::STATE_COMPLETE:
			task->Destroy();
			break;
#endif
	}
}

#ifdef OG
// ----------------------------------------------------------------------------
//
// Update refreshes all completed texture loading tasks
//
// ----------------------------------------------------------------------------
#endif

#ifdef OG
TextureLoadTaskClass* Get_Finished_Task()
#endif
#ifdef ZH
void TextureLoader::Process_Foreground_Load(TextureLoadTaskClass *task)
#endif
{
#ifdef OG
	CriticalSectionClass::LockClass m(mutex);

#endif
#ifdef ZH
	// Is high-priority task?
	if (task->Get_Priority() == TextureLoadTaskClass::PRIORITY_HIGH) {
		task->Finish_Load();
		task->Destroy();
		return;
	}

	// otherwise, must be a low-priority task.

	switch (task->Get_State()) {
		case TextureLoadTaskClass::STATE_NONE:
			Begin_Load_And_Queue(task);
			break;
#endif

#ifdef OG
	TextureLoadTaskClass* task=FinishedListHead;
	if (task) {
		FinishedListHead=task->Peek_Succ();
		task->Set_Succ(NULL);
#endif
#ifdef ZH
		case TextureLoadTaskClass::STATE_LOAD_MIPMAP:
			task->End_Load();
			task->Destroy();
			break;
#endif
	}
#ifdef OG
	return task;
#endif
}

#ifdef OG
TextureLoadTaskClass* Get_Thumbnail_Task()
#endif
#ifdef ZH
void TextureLoader::Begin_Load_And_Queue(TextureLoadTaskClass *task)
#endif
{
#ifdef OG
	CriticalSectionClass::LockClass m(mutex);

#endif
#ifdef ZH
	// should only be called from the DX8 thread.
	WWASSERT(Is_DX8_Thread());

	if (task->Begin_Load()) {
		// add to front of background queue. This means the
		// background load thread will service tasks in LIFO
		// (last in, first out) order.
#endif

#ifdef OG
	TextureLoadTaskClass* task=ThumbnailListHead;
	if (task) {
		ThumbnailListHead=task->Peek_Succ();
		task->Set_Succ(NULL);

#endif
#ifdef ZH
		// NOTE: this was how the old code did it, with a 
		// comment that mentioned good reasons for doing so,
		// without actually listing the reasons. I suspect 
		// it has something to do with visually important textures,
		// like those in the foreground, starting their load last.
		_BackgroundQueue.Push_Front(task);
	} else {
		// unable to load.
		task->Apply_Missing_Texture();
		task->Destroy();
#endif
	}
#ifdef OG
	return task;
#endif
}

#ifdef OG
void Add_Thumbnail_Task(TextureLoadTaskClass* task)

#endif
#ifdef ZH

void TextureLoader::Load_Thumbnail(TextureBaseClass *tc)
#endif
{
#ifdef OG
	CriticalSectionClass::LockClass m(mutex);

#endif
#ifdef ZH
	// All D3D operations must run from main thread
	WWASSERT(Is_DX8_Thread());
#endif

#ifdef OG
	WWASSERT(task->Peek_Succ()==NULL);

#endif
#ifdef ZH
	// load thumbnail texture
	IDirect3DTexture8 *d3d_texture = Load_Thumbnail(tc->Get_Full_Path(),tc->Get_HSV_Shift());
#endif

#ifdef OG
	task->Set_Succ(ThumbnailListHead);
	ThumbnailListHead=task;

#endif
#ifdef ZH
	// apply thumbnail to texture
	if (tc->Get_Asset_Type()==TextureBaseClass::TEX_REGULAR)
	{
		tc->Apply_New_Surface(d3d_texture, false);
	}
#endif

#ifdef ZH
	// release our reference to thumbnail texture
	d3d_texture->Release();
	d3d_texture = 0;
#endif
}

#ifdef OG
// ----------------------------------------------------------------------------
//
// The main thread's update function deletes tasks from the load task list
// once a frame.
//
// ----------------------------------------------------------------------------
#endif

#ifdef OG
TextureLoadTaskClass* Get_Task_From_Delete_List()
#endif
#ifdef ZH
void LoaderThreadClass::Thread_Function(void)
#endif
{
#ifdef OG
	WWASSERT(ThreadClass::_Get_Current_Thread_ID()==DX8Wrapper::_Get_Main_Thread_ID());

#endif
#ifdef ZH
	while (running) {
		// if there are no tasks on the background queue, no need to grab background lock.
		if (!_BackgroundQueue.Is_Empty()) {
			// Grab background load so other threads know we could be 
			// loading a texture.
			FastCriticalSectionClass::LockClass lock(_BackgroundCriticalSection);
#endif

#ifdef OG
	TextureLoadTaskClass* task=DeleteTaskListHead;

#endif
#ifdef ZH
			// try to remove a task from the background queue. This could fail
			// if another thread modified the queue between our test above and
			// grabbing the lock.
			TextureLoadTaskClass* task = _BackgroundQueue.Pop_Front();
#endif
	if (task) {
#ifdef OG
		DeleteTaskListHead=task->Peek_Succ();
		task->Set_Succ(NULL);

#endif
#ifdef ZH
				// verify task is in proper state for background processing.
				WWASSERT(task->Get_Type() == TextureLoadTaskClass::TASK_LOAD);
				WWASSERT(task->Get_State() == TextureLoadTaskClass::STATE_LOAD_BEGUN);

				// load mip map levels and return to foreground queue for final step.
				task->Load();
				_ForegroundQueue.Push_Back(task);
			}
		}

		Switch_Thread();
#endif
	}
#ifdef OG
	return task;
#endif
}

#ifdef OG
// ----------------------------------------------------------------------------

#endif
#ifdef ZH

////////////////////////////////////////////////////////////////////////////////
#endif
//
#ifdef OG
// When task wants to delete itself it adds itself to a delete list. This list
// can only be accessed from the main thread.
#endif
#ifdef ZH
// TextureLoaderTaskClass implementation

#endif
//
#ifdef OG
// ----------------------------------------------------------------------------
#endif
#ifdef ZH
////////////////////////////////////////////////////////////////////////////////
#endif

#ifdef OG
void Add_Task_To_Delete_List(TextureLoadTaskClass* task)

#endif
#ifdef ZH
TextureLoadTaskClass::TextureLoadTaskClass()
:	Texture			(0),
	D3DTexture		(0),
	Format			(WW3D_FORMAT_UNKNOWN),
	Width				(0),
	Height			(0),
	MipLevelCount	(0),
	Reduction		(0),
	Type				(TASK_NONE),
	Priority			(PRIORITY_LOW),
	State				(STATE_NONE),
	HSVShift			(0.0f,0.0f,0.0f)	
#endif
{
#ifdef OG
	WWASSERT(ThreadClass::_Get_Current_Thread_ID()==DX8Wrapper::_Get_Main_Thread_ID());

#endif
#ifdef ZH
	// because texture load tasks are pooled, the constructor and destructor
	// don't need to do much. The work of attaching a task to a texture is
	// is done by Init() and Deinit().

	for (int i = 0; i < MIP_LEVELS_MAX; ++i) {
		LockedSurfacePtr[i]		= NULL;
		LockedSurfacePitch[i]	= 0;
	}
}
#endif

#ifdef OG
	WWASSERT(task->Peek_Succ()==NULL);
#endif

#ifdef OG
	task->Set_Succ(DeleteTaskListHead);
	DeleteTaskListHead=task;

#endif
#ifdef ZH
TextureLoadTaskClass::~TextureLoadTaskClass(void)
{
	Deinit();
#endif
}

#ifdef OG
TextureLoadTaskClass* Get_Deferred_Task()

#endif
#ifdef ZH

TextureLoadTaskClass *TextureLoadTaskClass::Create(TextureBaseClass *tc, TaskType type, PriorityType priority)
{
	// recycle or create a new texture load task with the given type 
	// and priority, then associate the texture with the task.

	// pull a load task from front of free list
	TextureLoadTaskClass *task = NULL;
	switch (tc->Get_Asset_Type()) 
#endif
{
#ifdef OG
	CriticalSectionClass::LockClass m(mutex);

#endif
#ifdef ZH
		case TextureBaseClass::TEX_REGULAR : task=_TexLoadFreeList.Pop_Front(); break;
		case TextureBaseClass::TEX_CUBEMAP : task=_CubeTexLoadFreeList.Pop_Front(); break;
		case TextureBaseClass::TEX_VOLUME : task=_VolTexLoadFreeList.Pop_Front(); break;
		default : WWASSERT(0);
	};
#endif

#ifdef OG
	TextureLoadTaskClass* task=DeferredListHead;
	if (task) {
		DeferredListHead=task->Peek_Succ();
		task->Set_Succ(NULL);

#endif
#ifdef ZH
	// if no tasks on free list, allocate a new task
	if (!task) 
	{
		switch (tc->Get_Asset_Type())
		{
		case TextureBaseClass::TEX_REGULAR : task=new TextureLoadTaskClass; break;
		case TextureBaseClass::TEX_CUBEMAP : task=new CubeTextureLoadTaskClass; break;
		case TextureBaseClass::TEX_VOLUME : task=new VolumeTextureLoadTaskClass; break;
		default : WWASSERT(0);
		}
#endif
	}
#ifdef ZH
	task->Init(tc, type, priority);
#endif
	return task;
}

#ifdef OG
void Add_Deferred_Task(TextureLoadTaskClass* task)

#endif
#ifdef ZH

void TextureLoadTaskClass::Destroy(void)
#endif
{
#ifdef OG
	CriticalSectionClass::LockClass m(mutex);

#endif
#ifdef ZH
	// detach the task from its texture, and return to free pool.
	Deinit();
	_TexLoadFreeList.Push_Front(this);
}

#endif

#ifdef OG
	WWASSERT(task->Peek_Succ()==NULL);
	task->Set_Succ(DeferredListHead);
	DeferredListHead=task;

#endif
#ifdef ZH
void TextureLoadTaskClass::Delete_Free_Pool(void)
{
	// (gth) We should probably just MEMPool these task objects...
	while (TextureLoadTaskClass *task = _TexLoadFreeList.Pop_Front()) {
		delete task;
	}
	while (TextureLoadTaskClass *task = _CubeTexLoadFreeList.Pop_Front()) {
		delete task;
	}
	while (TextureLoadTaskClass *task = _VolTexLoadFreeList.Pop_Front()) {
		delete task;
	}
#endif
}
#ifdef ZH


void TextureLoadTaskClass::Init(TextureBaseClass* tc, TaskType type, PriorityType priority)
{
	WWASSERT(tc);
#endif

#ifdef OG
void TextureLoader::Flush_Pending_Load_Tasks()

#endif
#ifdef ZH
	// NOTE: we must be in the main thread to avoid corrupting the texture's refcount.
	WWASSERT(TextureLoader::Is_DX8_Thread());
	REF_PTR_SET(Texture, tc);

	// Make sure texture has a filename.
	WWASSERT(Texture->Get_Full_Path() != "");

	Type				= type;
	Priority			= priority;
	State				= STATE_NONE;

	D3DTexture		= 0;

	TextureClass* tex=Texture->As_TextureClass();

	if (tex)
	{
		Format			= tex->Get_Texture_Format(); // don't assume format yet KM
	}
	else
#endif
{
#ifdef OG
	while (!Is_Load_List_Empty()) {
		Update();
		ThreadClass::Switch_Thread();
#endif
#ifdef ZH
		Format			= WW3D_FORMAT_UNKNOWN;

#endif
	}
#ifdef ZH

	Width				= 0;
	Height			= 0;
	MipLevelCount	= Texture->MipLevelCount;
	Reduction		= Texture->Get_Reduction();
	HSVShift			= Texture->Get_HSV_Shift();

	for (int i = 0; i < MIP_LEVELS_MAX; ++i) 
	{
		LockedSurfacePtr[i]		= NULL;
		LockedSurfacePitch[i]	= 0;
#endif
}

#ifdef OG
void TextureLoader::Update()
#endif
#ifdef ZH
	switch (Type) 
#endif
{
#ifdef OG
	WWASSERT_PRINT(DX8Wrapper::_Get_Main_Thread_ID()==ThreadClass::_Get_Current_Thread_ID(),"TextureLoader::Update must be called from the main thread!");

#endif
#ifdef ZH
		case TASK_THUMBNAIL:
			WWASSERT(Texture->ThumbnailLoadTask == NULL);
			Texture->ThumbnailLoadTask = this;
			break;
#endif

#ifdef OG
	while (TextureLoadTaskClass* task=Get_Deferred_Task()) {
		task->Begin_Texture_Load();	// This will add the task to load list

#endif
#ifdef ZH
		case TASK_LOAD:
			WWASSERT(Texture->TextureLoadTask == NULL);
			Texture->TextureLoadTask = this;
			break;
	}
#endif
	}
#ifdef ZH


void TextureLoadTaskClass::Deinit()
{
	// task should not be on any list when it is being detached from texture.
	WWASSERT(Next == NULL);
	WWASSERT(Prev == NULL);
#endif

#ifdef OG
	while (TextureLoadTaskClass* task=Get_Finished_Task()) {
		task->End_Load();
		task->Apply(true);
		TextureLoadTaskClass::Release_Instance(task);
#endif
#ifdef ZH
	WWASSERT(D3DTexture == NULL);

	for (int i = 0; i < MIP_LEVELS_MAX; ++i) {
		WWASSERT(LockedSurfacePtr[i] == NULL);
#endif
	}

#ifdef OG
	while (TextureLoadTaskClass* task=Get_Thumbnail_Task()) {
		task->Begin_Thumbnail_Load();

#endif
#ifdef ZH
	if (Texture) {
		switch (Type) {
			case TASK_THUMBNAIL:
				WWASSERT(Texture->ThumbnailLoadTask == this);
				Texture->ThumbnailLoadTask = NULL;
				break;

			case TASK_LOAD:
				WWASSERT(Texture->TextureLoadTask == this);
				Texture->TextureLoadTask = NULL;
				break;
#endif
	}

#ifdef OG
	while (TextureLoadTaskClass* task=Get_Task_From_Delete_List()) {
//		delete task;
		TextureLoadTaskClass::Release_Instance(task);
#endif
#ifdef ZH
		// NOTE: we must be in main thread to avoid corrupting Texture's refcount.
		WWASSERT(TextureLoader::Is_DX8_Thread());
		REF_PTR_RELEASE(Texture);
#endif
	}
}

#ifdef OG
// ----------------------------------------------------------------------------
#endif

#ifdef OG
static DWORD VectortoRGBA( D3DXVECTOR3* v, FLOAT fHeight )
#endif
#ifdef ZH
bool TextureLoadTaskClass::Begin_Load(void)
#endif
{
#ifdef OG
    DWORD r = (DWORD)( 127.0f * v->x + 128.0f );
    DWORD g = (DWORD)( 127.0f * v->y + 128.0f );
    DWORD b = (DWORD)( 127.0f * v->z + 128.0f );
    DWORD a = (DWORD)( 255.0f * fHeight );
#endif
#ifdef ZH
	WWASSERT(TextureLoader::Is_DX8_Thread());

	bool loaded = false;

#endif
    
#ifdef OG
    return( (a<<24L) + (r<<16L) + (g<<8L) + (b<<0L) );

#endif
#ifdef ZH
	// if allowed, begin a compressed load
	if (Texture->Is_Compression_Allowed()) {
		loaded = Begin_Compressed_Load();
#endif
}

#ifdef OG
IDirect3DTexture8* TextureLoader::Generate_Bumpmap(TextureClass* texture)
{
	WW3DFormat bump_format=WW3D_FORMAT_U8V8;
	if (!DX8Caps::Support_Texture_Format(bump_format)) {
		return MissingTexture::_Get_Missing_Texture();
#endif
#ifdef ZH
	// otherwise, begin an uncompressed load
	if (!loaded) {
		loaded = Begin_Uncompressed_Load();

#endif
	}

#ifdef OG
	D3DSURFACE_DESC desc;
	IDirect3DTexture8* src_d3d_tex=texture->Peek_DX8_Texture();
	WWASSERT(src_d3d_tex);
	DX8_ErrorCode(src_d3d_tex->GetLevelDesc(0,&desc));
	unsigned width=desc.Width;
	unsigned height=desc.Height;
#endif
#ifdef ZH
	// if not loaded, abort.
	if (!loaded) {
		return false;
	}

#endif

#ifdef OG
	IDirect3DTexture8* d3d_texture = DX8Wrapper::_Create_DX8_Texture(
		width,
		height,
		bump_format,
		TextureClass::MIP_LEVELS_1);
#endif
#ifdef ZH
	// lock surfaces in preparation for copy
	Lock_Surfaces();

#endif

#ifdef OG
	D3DLOCKED_RECT src_locked_rect;
	DX8_ErrorCode(
		texture->Peek_DX8_Texture()->LockRect(
			0,
			&src_locked_rect,
			NULL,
			D3DLOCK_READONLY));
#endif
#ifdef ZH
	State = STATE_LOAD_BEGUN;

#endif

#ifdef OG
	D3DLOCKED_RECT dest_locked_rect;
	DX8_ErrorCode(
		d3d_texture->LockRect(
			0,
			&dest_locked_rect,
			NULL,
			0));
#endif
#ifdef ZH
	return true;
}

#endif

#ifdef OG
	WW3DFormat format=D3DFormat_To_WW3DFormat(desc.Format);
	unsigned bpp=Get_Bytes_Per_Pixel(format);
#endif

#ifdef OG
	for( unsigned y=0; y<desc.Height; y++ )

#endif
#ifdef ZH
// ----------------------------------------------------------------------------
//
// Load mipmap levels to a pre-generated and locked texture object based on
// information in load task object. Try loading from a DDS file first and if
// that fails try a TGA.
//
// ----------------------------------------------------------------------------
bool TextureLoadTaskClass::Load(void)
#endif
	{
#ifdef OG
		unsigned char* dest_ptr  = (unsigned char*)dest_locked_rect.pBits;
		dest_ptr+=y*dest_locked_rect.Pitch;
		unsigned char* src_ptr_mid = (unsigned char*)src_locked_rect.pBits;
		src_ptr_mid+=y*src_locked_rect.Pitch;
		unsigned char* src_ptr_next_line = ( src_ptr_mid + src_locked_rect.Pitch );
		unsigned char* src_ptr_prev_line = ( src_ptr_mid - src_locked_rect.Pitch );
#endif
#ifdef ZH
	WWMEMLOG(MEM_TEXTURE);
	WWASSERT(Peek_D3D_Texture());

	bool loaded = false;

#endif

#ifdef OG
		if( y == desc.Height-1 )  // Don't go past the last line
			src_ptr_next_line = src_ptr_mid;
		if( y == 0 )               // Don't go before first line
			src_ptr_prev_line = src_ptr_mid;
#endif
#ifdef ZH
	// if allowed, try to load compressed mipmaps
	if (Texture->Is_Compression_Allowed()) {
		loaded = Load_Compressed_Mipmap();
	}
#endif

#ifdef OG
		for( unsigned x=0; x<desc.Width; x++ )
		{
			unsigned pixel00;
			unsigned pixel01;
			unsigned pixelM1;
			unsigned pixel10;
			unsigned pixel1M;
#endif
#ifdef ZH
	// otherwise, load uncompressed mipmaps
	if (!loaded) {
		loaded = Load_Uncompressed_Mipmap();
	}

#endif

#ifdef OG
			BitmapHandlerClass::Read_B8G8R8A8(pixel00,src_ptr_mid,format,NULL,0);
			BitmapHandlerClass::Read_B8G8R8A8(pixel01,src_ptr_mid+bpp,format,NULL,0);
			BitmapHandlerClass::Read_B8G8R8A8(pixelM1,src_ptr_mid-bpp,format,NULL,0);
			BitmapHandlerClass::Read_B8G8R8A8(pixel10,src_ptr_prev_line,format,NULL,0);
			BitmapHandlerClass::Read_B8G8R8A8(pixel1M,src_ptr_next_line,format,NULL,0);
#endif
#ifdef ZH
	State = STATE_LOAD_MIPMAP;

#endif

#ifdef OG
			// Convert to luminance
			unsigned char bv00;
			unsigned char bv01;
			unsigned char bvM1;
			unsigned char bv10;
			unsigned char bv1M;
			BitmapHandlerClass::Write_B8G8R8A8(&bv00,WW3D_FORMAT_L8,pixel00);
			BitmapHandlerClass::Write_B8G8R8A8(&bv01,WW3D_FORMAT_L8,pixel01);
			BitmapHandlerClass::Write_B8G8R8A8(&bvM1,WW3D_FORMAT_L8,pixelM1);
			BitmapHandlerClass::Write_B8G8R8A8(&bv10,WW3D_FORMAT_L8,pixel10);
			BitmapHandlerClass::Write_B8G8R8A8(&bv1M,WW3D_FORMAT_L8,pixel1M);
			int v00=bv00,v01=bv01,vM1=bvM1,v10=bv10,v1M=bv1M;
#endif
#ifdef ZH
	return loaded;
}

#endif

#ifdef OG
			int iDu = (vM1-v01); // The delta-u bump value
			int iDv = (v1M-v10); // The delta-v bump value
#endif

#ifdef OG
			if( (v00 < vM1) && (v00 < v01) )  // If we are at valley
#endif
#ifdef ZH
void TextureLoadTaskClass::End_Load(void)
#endif
			{
#ifdef OG
				 iDu = vM1-v00;                 // Choose greater of 1st order diffs
				 if( iDu < v00-v01 )
					  iDu = v00-v01;

#endif
#ifdef ZH
	WWASSERT(TextureLoader::Is_DX8_Thread());

	Unlock_Surfaces();
	Apply(true);

	State = STATE_LOAD_COMPLETE;
#endif
			}

#ifdef OG
			// The luminance bump value (land masses are less shiny)
			unsigned short uL = ( v00>1 ) ? 63 : 127;
#endif

#ifdef OG
			switch( bump_format )
#endif
#ifdef ZH
void TextureLoadTaskClass::Finish_Load(void)
#endif
			{
#ifdef OG
			case WW3D_FORMAT_U8V8:
				*dest_ptr++ = (unsigned char)iDu;
				*dest_ptr++ = (unsigned char)iDv;

#endif
#ifdef ZH
	switch (State) {
		// NOTE: fall-through below is intentional.

		case STATE_NONE:
			if (!Begin_Load()) {
				Apply_Missing_Texture();
#endif
				break;
#ifdef ZH
			}
#endif

#ifdef OG
			case WW3D_FORMAT_L6V5U5:
				*(unsigned short*)dest_ptr  = (unsigned short)( ( (iDu>>3) & 0x1f ) <<  0 );
				*(unsigned short*)dest_ptr |= (unsigned short)( ( (iDv>>3) & 0x1f ) <<  5 );
				*(unsigned short*)dest_ptr |= (unsigned short)( ( ( uL>>2) & 0x3f ) << 10 );
				dest_ptr += 2;
				break;
#endif
#ifdef ZH
		case STATE_LOAD_BEGUN:
			Load();

		case STATE_LOAD_MIPMAP:
			End_Load();

#endif

#ifdef OG
			case WW3D_FORMAT_X8L8V8U8:
				*dest_ptr++ = (unsigned char)iDu;
				*dest_ptr++ = (unsigned char)iDv;
				*dest_ptr++ = (unsigned char)uL;
				*dest_ptr++ = (unsigned char)0L;
#endif
#ifdef ZH
		default:

#endif
				break;
			}
#ifdef ZH
}

void TextureLoadTaskClass::Apply_Missing_Texture(void)
{
	WWASSERT(TextureLoader::Is_DX8_Thread());
	WWASSERT(!D3DTexture);
#endif

#ifdef OG
			// Move one pixel to the left (src is 32-bpp)
			src_ptr_mid+=bpp;
			src_ptr_prev_line+=bpp;
			src_ptr_next_line+=bpp;
#endif
#ifdef ZH
	D3DTexture = MissingTexture::_Get_Missing_Texture();
	Apply(true);

#endif
		}
#ifdef ZH


void TextureLoadTaskClass::Apply(bool initialize)
{
	WWASSERT(D3DTexture);

	// Verify that none of the mip levels are locked
	for (unsigned i=0;i<MipLevelCount;++i) {
		WWASSERT(LockedSurfacePtr[i]==NULL);
#endif
	}

#ifdef OG
	DX8_ErrorCode(d3d_texture->UnlockRect(0));
	DX8_ErrorCode(texture->Peek_DX8_Texture()->UnlockRect(0));
	return d3d_texture;

#endif
#ifdef ZH
	Texture->Apply_New_Surface(D3DTexture, initialize);

	D3DTexture->Release();
	D3DTexture = NULL;
#endif
}
#ifdef ZH

static bool	Get_Texture_Information
(
	const char* filename,
	unsigned& reduction,
	unsigned& w,
	unsigned& h,
	unsigned& d,
	WW3DFormat& format,
	unsigned& mip_count,
	bool compressed
)
{
	ThumbnailClass* thumb=ThumbnailManagerClass::Peek_Thumbnail_Instance_From_Any_Manager(filename);

	if (!thumb) 
	{
		if (compressed) 
		{
			DDSFileClass dds_file(filename, 0);
			if (!dds_file.Is_Available()) return false;
#endif

#ifdef OG
// ----------------------------------------------------------------------------
//
// Public texture request functions. These functions can be used to request
// texture loading.
//
// ----------------------------------------------------------------------------

#endif
#ifdef ZH
			// Destination size will be the next power of two square from the larger width and height...
			w = dds_file.Get_Width(0);
			h = dds_file.Get_Height(0);
			d = dds_file.Get_Depth(0);
			format = dds_file.Get_Format();
			mip_count = dds_file.Get_Mip_Level_Count();
			//Figure out correct reduction
			int reqReduction=WW3D::Get_Texture_Reduction();	//requested reduction

			if (reqReduction >= mip_count)
				reqReduction=mip_count-1;	//leave only the lowest level

			//Clamp reduction
			int curReduction=0;
			int curWidth=w;
			int curHeight=h;
			int minDim=WW3D::Get_Texture_Min_Dimension();

			while (curReduction < reqReduction && curWidth > minDim && curHeight > minDim)
			{	curWidth >>=1;	//keep dividing
				curHeight >>=1;
				curReduction++;
			}
			reduction=curReduction;
			return true;
		}
#endif

#ifdef OG
void TextureLoader::Add_Load_Task(TextureClass* tc)

#endif
#ifdef ZH
		Targa targa;
		if (TARGA_ERROR_HANDLER(targa.Open(filename, TGA_READMODE), filename)) 
#endif
{
#ifdef OG
	// If the texture is already being loaded we just exit here.
	if (tc->TextureLoadTask) return;

#endif
#ifdef ZH
			return false;
		}

		unsigned int bpp;
		WW3DFormat dest_format;
		Get_WW3D_Format(dest_format,format,bpp,targa);

		mip_count = 0;

		//Figure out how many mip levels this texture will occupy
		for (int i=targa.Header.Width, j=targa.Header.Height; i > 0 && j > 0; i>>=1, j>>=1)
				mip_count++;

		//Figure out correct reduction
		int reqReduction=WW3D::Get_Texture_Reduction();	//requested reduction

		if (reqReduction >= mip_count)
			reqReduction=mip_count-1;	//leave only the lowest level

		//Clamp reduction
		int curReduction=0;
		int curWidth=targa.Header.Width;
		int curHeight=targa.Header.Height;
		int minDim=WW3D::Get_Texture_Min_Dimension();

		while (curReduction < reqReduction && curWidth > minDim && curHeight > minDim)
		{	curWidth >>=1;	//keep dividing
			curHeight >>=1;
			curReduction++;
		}
		reduction=curReduction;

		// Destination size will be the next power of two square from the larger width and height...
		w = targa.Header.Width;
		h = targa.Header.Height;
		d = 1;
		return true; 
	}

	if (compressed &&
		thumb->Get_Original_Texture_Format()!=WW3D_FORMAT_DXT1 &&
		thumb->Get_Original_Texture_Format()!=WW3D_FORMAT_DXT2 &&
		thumb->Get_Original_Texture_Format()!=WW3D_FORMAT_DXT3 &&
		thumb->Get_Original_Texture_Format()!=WW3D_FORMAT_DXT4 &&
		thumb->Get_Original_Texture_Format()!=WW3D_FORMAT_DXT5) {
		return false;
	}
#endif

#ifdef OG
	TextureLoadTaskClass* task=TextureLoadTaskClass::Get_Instance(tc,false);
	task->Begin_Texture_Load();

#endif
#ifdef ZH
	w=thumb->Get_Original_Texture_Width() >> reduction;
	h=thumb->Get_Original_Texture_Height() >> reduction;
	//d=thumb->Get_Original_Texture_Depth() >> reduction; // need to a volume texture support to thumbnails...maybe
	mip_count=thumb->Get_Original_Texture_Mip_Level_Count();
	format=thumb->Get_Original_Texture_Format();
	return true;
#endif
}

#ifdef OG
// ----------------------------------------------------------------------------
#endif

#ifdef OG
void TextureLoader::Request_High_Priority_Loading(
	TextureClass* tc,
	TextureClass::MipCountType mip_level_count)

#endif
#ifdef ZH
bool TextureLoadTaskClass::Begin_Compressed_Load(void)
{
	unsigned orig_w,orig_h,orig_d,orig_mip_count,reduction;
	WW3DFormat orig_format;
	if (!Get_Texture_Information
		  (
				Texture->Get_Full_Path(),
				reduction,
				orig_w,
				orig_h,
				orig_d,
				orig_format,
				orig_mip_count,
				true
			)
		)
#endif
{
#ifdef OG
	TextureLoadTaskClass* task=TextureLoadTaskClass::Get_Instance(tc,true);
	task->Begin_Texture_Load();
#endif
#ifdef ZH
		return false;

#endif
}

#ifdef OG
// ----------------------------------------------------------------------------

#endif
#ifdef ZH
	// Destination size will be the next power of two square from the larger width and height...
	unsigned int width	= orig_w;
	unsigned int height	= orig_h;
	TextureLoader::Validate_Texture_Size(width, height,orig_d);
#endif

#ifdef OG
void TextureLoader::Request_Thumbnail(TextureClass* tc)

#endif
#ifdef ZH
	// If the size doesn't match, try and see if texture reduction would help... (mainly for
	// cases where loaded texture is larger than hardware limit)
	if (width != orig_w || height != orig_h) 
	{
		for (unsigned int i = 1; i < orig_mip_count; ++i) 
#endif
{
#ifdef OG
	// If the texture is already being loaded we just exit here.
	if (tc->TextureLoadTask) return;

#endif
#ifdef ZH
			unsigned w=orig_w>>i;
			if (w<4) w=4;
			unsigned h=orig_h>>i;
			if (h<4) h=4;
			unsigned tmp_w=w;
			unsigned tmp_h=h;

			TextureLoader::Validate_Texture_Size(w,h,orig_d);
#endif

#ifdef OG
	TextureLoadTaskClass* task=TextureLoadTaskClass::Get_Instance(tc,false);
	task->Begin_Thumbnail_Load();

#endif
#ifdef ZH
			if (w == tmp_w && h == tmp_h) 
			{
				Reduction	+= i;
				width			=	w;
				height		=	h;
				break;
			}
		}
#endif
}
#ifdef ZH

	Width		= width;
	Height	= height;
	Format	= Get_Valid_Texture_Format(orig_format, Texture->Is_Compression_Allowed());
	Reduction = reduction;

	if (!Texture->Is_Reducible() || Texture->MipLevelCount == MIP_LEVELS_1)
		Reduction = 0;	//app doesn't want this texture to ever be reduced.
	else
	//Make sure we don't reduce below the level requested by the app
	if (Texture->MipLevelCount != MIP_LEVELS_ALL && (Texture->MipLevelCount - Reduction) < 1)
		Reduction = Texture->MipLevelCount - 1;
#endif

#ifdef OG
// ----------------------------------------------------------------------------
//
// Texture loader task handler
//
// ----------------------------------------------------------------------------

#endif
#ifdef ZH
	//Another sanity check
	if (Reduction >= orig_mip_count)
		Reduction = 0;	//should not be possible to get here, but check just in case.

	unsigned int mip_level_count = Get_Mip_Level_Count();
	int reducedWidth=Width;
	int reducedHeight=Height;
#endif

#ifdef OG
TextureLoadTaskClass::TextureLoadTaskClass()
	:
	Texture(0),
	Succ(0), 
	D3DTexture(0),
	Width(0),
	Height(0),
	Format(WW3D_FORMAT_UNKNOWN),
	IsLoading(false),
	HasFailed(false),
	MipLevelCount(0),
	HighPriorityRequested(false),
	Reduction(0)
#endif
#ifdef ZH
	// If texture wants all mip levels, take as many as the file contains (not necessarily all)
	// Otherwise take as many mip levels as the texture wants, not to exceed the count in file...
	if (!mip_level_count) 

#endif
{
#ifdef ZH
		reducedWidth >>= Reduction;
		reducedHeight >>= Reduction;
		mip_level_count = orig_mip_count-Reduction;//dds_file.Get_Mip_Level_Count();
		if (mip_level_count < 1)
			mip_level_count = 1;	//sanity check to make sure something gets loaded.
#endif
}
#ifdef ZH
	else
	{
		if (mip_level_count > orig_mip_count) 
		{	//dds_file.Get_Mip_Level_Count()) {
			mip_level_count = orig_mip_count;//dds_file.Get_Mip_Level_Count();
		}
#endif

#ifdef OG
// ----------------------------------------------------------------------------
//
// Destruct texture load task. The load task deinitialization will fail from
// any other than the main thread so the task must be either deleted from the
// main thread or deinitialized in the main thread prior to deleting it in
// another thread.
//
// ----------------------------------------------------------------------------

#endif
#ifdef ZH
		if (Reduction)
		{	reducedWidth >>= Reduction;
			reducedHeight >>= Reduction;
			mip_level_count -= Reduction;	//reduced requested number by those removed.
		}
	}

	// Once more, verify that the mip level count is correct (in case it was changed here it might not
	// match the size...well actually it doesn't have to match but it can't be bigger than the size)
	unsigned int max_mip_level_count = 1;
	unsigned int w = 4;
	unsigned int h = 4;

	while (w < Width && h < Height) 
	{
		w += w;
		h += h;
		max_mip_level_count++;
	}
#endif

#ifdef OG
TextureLoadTaskClass::~TextureLoadTaskClass()
#endif
#ifdef ZH
	if (mip_level_count > max_mip_level_count) 
#endif
{
#ifdef OG
	Deinit();

#endif
#ifdef ZH
		mip_level_count = max_mip_level_count;
	}

	D3DTexture	= DX8Wrapper::_Create_DX8_Texture
	(
		reducedWidth, 
		reducedHeight, 
		Format, 
		(MipCountType)mip_level_count,
#ifdef USE_MANAGED_TEXTURES
		D3DPOOL_MANAGED
#else
		D3DPOOL_SYSTEMMEM
#endif
	);

	MipLevelCount = mip_level_count;

	return true;
#endif
}

#ifdef OG
void TextureLoadTaskClass::Init(TextureClass* tc,bool high_priority)

#endif
#ifdef ZH
bool TextureLoadTaskClass::Begin_Uncompressed_Load(void)
{
	unsigned width,height,depth,orig_mip_count,reduction;
	WW3DFormat orig_format;
	if (!Get_Texture_Information
		  (
				Texture->Get_Full_Path(),
				reduction,
				width,
				height,
				depth,
				orig_format,
				orig_mip_count,
				false
			)
		)
#endif
{
#ifdef OG
	// Make sure texture has a filename.
	REF_PTR_SET(Texture,tc);
	WWASSERT(Texture->Get_Full_Path() != NULL);
#endif
#ifdef ZH
		return false;
	}

#endif

#ifdef OG
	Reduction=Texture->Get_Reduction();
	HighPriorityRequested=high_priority;
	IsLoading=false;
	HasFailed=false;
	MipLevelCount=tc->MipLevelCount;
	D3DTexture=0;
	Width=0;
	Height=0;
	Format=Texture->Get_Texture_Format();
#endif
#ifdef ZH
	WW3DFormat src_format=orig_format;
	WW3DFormat dest_format=src_format;
	dest_format=Get_Valid_Texture_Format(dest_format,false);	// No compressed destination format if reading from targa...

#endif

#ifdef OG
	Texture->TextureLoadTask=this;
	for (int i=0;i<TextureClass::MIP_LEVELS_MAX;++i) {
		LockedSurfacePtr[i]=NULL;
		LockedSurfacePitch[i]=0;

#endif
#ifdef ZH
   if (	src_format != WW3D_FORMAT_A8R8G8B8 
   	&&	src_format != WW3D_FORMAT_R8G8B8 
  		&&	src_format != WW3D_FORMAT_X8R8G8B8 ) 
	{
		WWDEBUG_SAY(("Invalid TGA format used in %s - only 24 and 32 bit formats should be used!\n", Texture->Get_Full_Path()));
#endif
	}
#ifdef ZH

	// Destination size will be the next power of two square from the larger width and height...
	unsigned ow = width;
	unsigned oh = height;
	TextureLoader::Validate_Texture_Size(width, height,depth);
	if (width != ow || height != oh) 
	{
		WWDEBUG_SAY(("Invalid texture size, scaling required. Texture: %s, size: %d x %d -> %d x %d\n", Texture->Get_Full_Path(), ow, oh, width, height));
#endif
}
#ifdef ZH

	Width		= width;
	Height	= height;
	Reduction = reduction;

	if (!Texture->Is_Reducible() || Texture->MipLevelCount == MIP_LEVELS_1)
		Reduction = 0;	//app doesn't want this texture to ever be reduced.
	else
	//Make sure we don't reduce below the level requested by the app
	if (Texture->MipLevelCount != MIP_LEVELS_ALL && (Texture->MipLevelCount - Reduction) < 1)
		Reduction = Texture->MipLevelCount - 1;
#endif

#ifdef OG
void TextureLoadTaskClass::Deinit()

#endif
#ifdef ZH
	//Another sanity check
	if (Reduction >= orig_mip_count)
		Reduction = 0;	//should not be possible to get here, but check just in case.

	if (Format == WW3D_FORMAT_UNKNOWN) 
#endif
{
#ifdef OG
	WWASSERT(Succ==NULL);
	WWASSERT(D3DTexture==NULL);
	WWASSERT(IsLoading==false);
	for (int i=0;i<TextureClass::MIP_LEVELS_MAX;++i) {
		WWASSERT(LockedSurfacePtr[i]==NULL);
#endif
#ifdef ZH
		Format=dest_format;
	//	Format = Get_Valid_Texture_Format(dest_format, false); validated above

#endif
	}
#ifdef OG
	if (Texture) {
		WWASSERT(Texture->TextureLoadTask==this);
		Texture->TextureLoadTask=NULL;
#endif
#ifdef ZH
	else 
	{
		Format = Get_Valid_Texture_Format(Format, false);
#endif
	}
#ifdef ZH

	int reducedWidth=Width;
	int reducedHeight=Height;
	int reducedMipCount=Texture->MipLevelCount;
#endif

#ifdef OG
	REF_PTR_RELEASE(Texture);
	WWASSERT(!D3DTexture);

#endif
#ifdef ZH
	if (Reduction)
	{	//we don't care about specific levels so reduce them if needed.
		reducedWidth >>= Reduction;
		reducedHeight >>= Reduction;
		if (reducedMipCount != MIP_LEVELS_ALL)
			reducedMipCount -= Reduction;
#endif
}

#ifdef OG
// ----------------------------------------------------------------------------
//
//
//
// ----------------------------------------------------------------------------

#endif
#ifdef ZH
	D3DTexture = DX8Wrapper::_Create_DX8_Texture
	(
		reducedWidth, 
		reducedHeight, 
		Format, 
		(MipCountType)reducedMipCount,
#ifdef USE_MANAGED_TEXTURES
		D3DPOOL_MANAGED
#else
		D3DPOOL_SYSTEMMEM
#endif
	);

	return true;
}
#endif

#ifdef OG
void TextureLoadTaskClass::Begin_Texture_Load()

#endif
#ifdef ZH
/*
bool TextureLoadTaskClass::Begin_Compressed_Load(void)
#endif
{
#ifdef OG
	// If we're in main thread, init for loading and add to the load list
	if (ThreadClass::_Get_Current_Thread_ID()==DX8Wrapper::_Get_Main_Thread_ID()) {

#endif
#ifdef ZH
	DDSFileClass dds_file(Texture->Get_Full_Path(), Get_Reduction());
	if (!dds_file.Is_Available()) {
		return false; 
	}
#endif

#ifdef OG
		bool loaded=false;
		if (Texture->Is_Compression_Allowed()) {
			DDSFileClass dds_file(Texture->Get_Full_Path(),Get_Reduction());
			if (dds_file.Is_Available()) {
#endif
				// Destination size will be the next power of two square from the larger width and height...
#ifdef OG
				unsigned width, height;
				width=dds_file.Get_Width(0);
				height=dds_file.Get_Height(0);
				TextureLoader::Validate_Texture_Size(width,height);
#endif
#ifdef ZH
	unsigned int width	= dds_file.Get_Width(0);
	unsigned int height	= dds_file.Get_Height(0);
	TextureLoader::Validate_Texture_Size(width, height);

#endif

				// If the size doesn't match, try and see if texture reduction would help... (mainly for
				// cases where loaded texture is larger than hardware limit)
#ifdef OG
				if (width!=dds_file.Get_Width(0) || height!=dds_file.Get_Height(0)) {
					for (unsigned i=1;i<dds_file.Get_Mip_Level_Count();++i) {
						unsigned w=dds_file.Get_Width(i);
						unsigned h=dds_file.Get_Height(i);
						TextureLoader::Validate_Texture_Size(width,height);
						if (w==dds_file.Get_Width(i) || h==dds_file.Get_Height(i)) {
							Reduction+=i;
							width=w;
							height=h;

#endif
#ifdef ZH
	if (width != dds_file.Get_Width(0) || height != dds_file.Get_Height(0)) {
		for (unsigned int i = 1; i < dds_file.Get_Mip_Level_Count(); ++i) {
			unsigned int w = dds_file.Get_Width(i);
			unsigned int h = dds_file.Get_Height(i);
			TextureLoader::Validate_Texture_Size(w,h);

			if (w == dds_file.Get_Width(i) && h == dds_file.Get_Height(i)) {
				Reduction	+= i;
				width			=	w;
				height		=	h;
#endif
							break;
						}
					}
				}

#ifdef OG
				IsLoading=true;
				Width=width;
				Height=height;
				Format=Get_Valid_Texture_Format(dds_file.Get_Format(),Texture->Is_Compression_Allowed());

#endif
#ifdef ZH
	Width		= width;
	Height	= height;
	Format	= Get_Valid_Texture_Format(dds_file.Get_Format(), Texture->Is_Compression_Allowed());

	unsigned int mip_level_count = Get_Mip_Level_Count();
#endif

#ifdef OG
				unsigned mip_level_count=Get_Mip_Level_Count();
#endif
				// If texture wants all mip levels, take as many as the file contains (not necessarily all)
				// Otherwise take as many mip levels as the texture wants, not to exceed the count in file...
#ifdef OG
				if (!mip_level_count) mip_level_count=dds_file.Get_Mip_Level_Count();
				else if (mip_level_count>dds_file.Get_Mip_Level_Count()) mip_level_count=dds_file.Get_Mip_Level_Count();

#endif
#ifdef ZH
	if (!mip_level_count) {
		mip_level_count = dds_file.Get_Mip_Level_Count();
	} else if (mip_level_count > dds_file.Get_Mip_Level_Count()) {
		mip_level_count = dds_file.Get_Mip_Level_Count();
	}
#endif

				// Once more, verify that the mip level count is correct (in case it was changed here it might not
				// match the size...well actually it doesn't have to match but it can't be bigger than the size)
#ifdef OG
				unsigned max_mip_level_count=1;
				unsigned w=4;
				unsigned h=4;
				while (w<Width && h<Height) {
					w+=w;
					h+=h;

#endif
#ifdef ZH
	unsigned int max_mip_level_count = 1;
	unsigned int w = 4;
	unsigned int h = 4;

	while (w < Width && h < Height) {
		w += w;
		h += h;
#endif
					max_mip_level_count++;
				}
#ifdef OG
				if (mip_level_count>max_mip_level_count) mip_level_count=max_mip_level_count;
#endif

#ifdef OG
				D3DTexture=DX8Wrapper::_Create_DX8_Texture(Width,Height,Format,(TextureClass::MipCountType)mip_level_count);
				MipLevelCount=mip_level_count;
				//Texture->MipLevelCount);
				loaded=true;
#endif
#ifdef ZH
	if (mip_level_count > max_mip_level_count) {
		mip_level_count = max_mip_level_count;

#endif
			}
#ifdef ZH

	D3DTexture	= DX8Wrapper::_Create_DX8_Texture(
		Width, 
		Height, 
		Format, 
		(TextureBaseClass::MipCountType)mip_level_count,
#ifdef USE_MANAGED_TEXTURES
		D3DPOOL_MANAGED);
#else
		D3DPOOL_SYSTEMMEM);
#endif
	MipLevelCount = mip_level_count;
	return true;
#endif
		}

#ifdef OG
		if (!loaded) {

#endif
#ifdef ZH

bool TextureLoadTaskClass::Begin_Uncompressed_Load(void)
{
#endif
			Targa targa;
			if (TARGA_ERROR_HANDLER(targa.Open(Texture->Get_Full_Path(), TGA_READMODE),Texture->Get_Full_Path())) {
#ifdef OG
				D3DTexture=MissingTexture::_Get_Missing_Texture();
				HasFailed=true;
				IsLoading=false;
				End_Load();
				Apply(true);
				Add_Task_To_Delete_List(this);
				return;
#endif
#ifdef ZH
		return false;

#endif
			}

#ifdef ZH
	unsigned int bpp;
	WW3DFormat src_format, dest_format;
	Get_WW3D_Format(dest_format,src_format,bpp,targa);
#endif

#ifdef OG
			unsigned bpp;
			WW3DFormat src_format,dest_format;
			Get_WW3D_Format(dest_format,src_format,bpp,targa);
			if (src_format!=WW3D_FORMAT_A8R8G8B8 &&
				src_format!=WW3D_FORMAT_R8G8B8 &&
				src_format!=WW3D_FORMAT_X8R8G8B8) {
				WWDEBUG_SAY(("Invalid TGA format used in %s - only 24 and 32 bit formats should be used!\n",Texture->Get_Full_Path()));
#endif
#ifdef ZH
	if (	src_format != WW3D_FORMAT_A8R8G8B8 
		&&	src_format != WW3D_FORMAT_R8G8B8 
		&&	src_format != WW3D_FORMAT_X8R8G8B8) {
		WWDEBUG_SAY(("Invalid TGA format used in %s - only 24 and 32 bit formats should be used!\n", Texture->Get_Full_Path()));

#endif
			}

			// Destination size will be the next power of two square from the larger width and height...
			unsigned width=targa.Header.Width, height=targa.Header.Height;
			int ReductionFactor=Get_Reduction();
			int MipLevels=0;

			//Figure out how many mip levels this texture will occupy
			for (int i=width, j=height; i > 0 && j > 0; i>>=1, j>>=1)
					MipLevels++;

			//Adjust the reduction factor to keep textures above some minimum dimensions
			if (MipLevels <= WW3D::Get_Texture_Min_Mip_Levels())
				ReductionFactor=0;
			else
			{	int mipToDrop=MipLevels-WW3D::Get_Texture_Min_Mip_Levels();
				if (ReductionFactor >= mipToDrop)
					ReductionFactor=mipToDrop;
			}

			width=targa.Header.Width>>ReductionFactor;
			height=targa.Header.Height>>ReductionFactor;
#ifdef OG
			unsigned ow=width;
			unsigned oh=height;
#endif
#ifdef ZH
	unsigned ow = width;
	unsigned oh = height;
#endif
			TextureLoader::Validate_Texture_Size(width,height);
#ifdef OG
			if (width!=ow || height!=oh) {
#endif
#ifdef ZH
	if (width != ow || height != oh) {
#endif
				WWDEBUG_SAY(("Invalid texture size, scaling required. Texture: %s, size: %d x %d -> %d x %d\n",Texture->Get_Full_Path(),ow,oh,width,height));
			}
#ifdef ZH

	Width		= width;
	Height	= height;
#endif

#ifdef OG
			IsLoading=true;
			Width=width;
			Height=height;

#endif
#ifdef ZH
	// changed because format was being read from previous loading task?! KJM
	Format=dest_format;
	//if (Format == WW3D_FORMAT_UNKNOWN) {
	//	Format = Get_Valid_Texture_Format(dest_format, false);
	//} else {
	//	Format = Get_Valid_Texture_Format(Format, false);
	//}
#endif

#ifdef OG
			if (Format==WW3D_FORMAT_UNKNOWN) {
				Format=Get_Valid_Texture_Format(dest_format,false);

#endif
#ifdef ZH
	D3DTexture = DX8Wrapper::_Create_DX8_Texture
	(
		Width, 
		Height, 
		Format, 
		Texture->MipLevelCount,
#ifdef USE_MANAGED_TEXTURES
		D3DPOOL_MANAGED);
#else
		D3DPOOL_SYSTEMMEM);
#endif
	return true;
#endif
			}
#ifdef OG
			else {
				Format=Get_Valid_Texture_Format(Format,false);
			}
#endif
#ifdef ZH
*/

#endif

#ifdef OG
			D3DTexture=DX8Wrapper::_Create_DX8_Texture(Width,Height,Format,Texture->MipLevelCount);
		}

#endif
#ifdef ZH
void TextureLoadTaskClass::Lock_Surfaces(void)
{
	MipLevelCount = D3DTexture->GetLevelCount();
#endif

#ifdef OG
		MipLevelCount=D3DTexture->GetLevelCount();
		for (unsigned i=0;i<MipLevelCount;++i) {
#endif
#ifdef ZH
	for (unsigned int i = 0; i < MipLevelCount; ++i) 
	{
#endif
			D3DLOCKED_RECT locked_rect;
#ifdef OG
			DX8_ErrorCode(
				D3DTexture->LockRect(

#endif
#ifdef ZH
		DX8_ErrorCode
		(
			Peek_D3D_Texture()->LockRect
			(
#endif
					i,
					&locked_rect,
					NULL,
#ifdef OG
					0));
			LockedSurfacePtr[i]=(unsigned char*)locked_rect.pBits;
			LockedSurfacePitch[i]=locked_rect.Pitch;

#endif
#ifdef ZH
				0
			)
		);
		LockedSurfacePtr[i]		= (unsigned char *)locked_rect.pBits;
		LockedSurfacePitch[i]	= locked_rect.Pitch;
	}
#endif
		}

#ifdef OG
		if (HighPriorityRequested) {
			TextureLoader::Load_Mipmap_Levels(this);
			End_Load();
			Apply(true);
			Add_Task_To_Delete_List(this);

#endif
#ifdef ZH

void TextureLoadTaskClass::Unlock_Surfaces(void)
{
	for (unsigned int i = 0; i < MipLevelCount; ++i) 
	{
		if (LockedSurfacePtr[i]) 
		{
			WWASSERT(ThreadClass::_Get_Current_Thread_ID() == DX8Wrapper::_Get_Main_Thread_ID());
			DX8_ErrorCode(Peek_D3D_Texture()->UnlockRect(i));
#endif
		}
#ifdef OG
		else {
			LoaderThreadClass::Add_Task_To_Load_List(this);
#endif
#ifdef ZH
		LockedSurfacePtr[i] = NULL;

#endif
		}
#ifdef ZH

#ifndef USE_MANAGED_TEXTURES
	IDirect3DTexture8* tex = DX8Wrapper::_Create_DX8_Texture(Width, Height, Format, Texture->MipLevelCount,D3DPOOL_DEFAULT);
	DX8CALL(UpdateTexture(Peek_D3D_Texture(),tex));
	Peek_D3D_Texture()->Release();
	D3DTexture=tex;
	WWDEBUG_SAY(("Created non-managed texture (%s)\n",Texture->Get_Full_Path()));
#endif

#endif
	}
#ifdef OG
	// Otherwise add to deferred list which will be handled by main thread
	else {
		Add_Deferred_Task(this);
	}
}
#endif

#ifdef OG
/*	file_auto_ptr my_tga_file(_TheFileFactory,Texture->Get_Full_Path());	
	if (my_tga_file->Is_Available()) {
		my_tga_file->Open();
		unsigned size=my_tga_file->Size();
		char* tga_memory=W3DNEWARRAY char[size];
		my_tga_file->Read(tga_memory,size);
		my_tga_file->Close();
#endif

#ifdef OG
		StringClass pth("data\\");
		pth+=Texture->Get_Texture_Name();
		RawFileClass tmp_tga_file(pth);
		tmp_tga_file.Create();
		tmp_tga_file.Write(tga_memory,size);
		tmp_tga_file.Close();
		delete[] tga_memory;
#endif
#ifdef ZH
bool TextureLoadTaskClass::Load_Compressed_Mipmap(void)
{
	DDSFileClass dds_file(Texture->Get_Full_Path(), Get_Reduction());

#endif

#ifdef ZH
	// if we can't load from file, indicate rror.
	if (!dds_file.Is_Available() || !dds_file.Load()) 
	{
		return false;
#endif
	}
#ifdef OG
*/
#endif

#ifdef OG
// ----------------------------------------------------------------------------
//
//
//
// ----------------------------------------------------------------------------

#endif
#ifdef ZH
	// regular 2d texture
	unsigned int width	= Get_Width();
	unsigned int height	= Get_Height();

	if (Reduction)
	{	for (unsigned int level = 0; level < Reduction; ++level) {
			width		>>= 1;
			height		>>= 1;
		}
	}
#endif

#ifdef OG
void TextureLoadTaskClass::Begin_Thumbnail_Load()
#endif
#ifdef ZH
	for (unsigned int level = 0; level < Get_Mip_Level_Count(); ++level) 
#endif
{
#ifdef OG
//	CriticalSectionClass::LockClass m(mutex);

#endif
#ifdef ZH
		WWASSERT(width && height);
		dds_file.Copy_Level_To_Surface
		(
			level,
			Get_Format(),
			width,
			height,
			Get_Locked_Surface_Ptr(level),
			Get_Locked_Surface_Pitch(level),
			HSVShift
		);
#endif

#ifdef OG
	unsigned thread_id=ThreadClass::_Get_Current_Thread_ID();
	if (thread_id==DX8Wrapper::_Get_Main_Thread_ID()) {
		WW3DFormat format=Texture->Get_Texture_Format();
		// No compressed thumbnails
		switch (format) {
		case WW3D_FORMAT_DXT1:
		case WW3D_FORMAT_DXT2:
		case WW3D_FORMAT_DXT3:
		case WW3D_FORMAT_DXT4:
		case WW3D_FORMAT_DXT5:
			format=WW3D_FORMAT_A8R8G8B8; break;
		default:
			break;
#endif
#ifdef ZH
		width		>>= 1;
		height	>>= 1;

#endif
		}
#ifdef OG
		D3DTexture=TextureLoader::Load_Thumbnail(Texture->Get_Full_Path(),format);
#endif

#ifdef OG
		// Thumbnail loads are always high priority, so apply immediatelly
		End_Load();
		Apply(false);
		Add_Task_To_Delete_List(this);
		return;
#endif
#ifdef ZH
	return true;

#endif
	}
#ifdef OG
	else {
		Add_Thumbnail_Task(this);
	}
}
#endif

#ifdef OG
// ----------------------------------------------------------------------------
//
// Deinit can be called multiple times. If any surfaces are locked this call
// can only be called from the main thread.
//
// ----------------------------------------------------------------------------
#endif

#ifdef OG
void TextureLoadTaskClass::End_Load()

#endif
#ifdef ZH
bool TextureLoadTaskClass::Load_Uncompressed_Mipmap(void)
{
	if (!Get_Mip_Level_Count()) 
#endif
{
#ifdef OG
	for (unsigned i=0;i<MipLevelCount;++i) {
		if (LockedSurfacePtr[i]) {
			WWASSERT(ThreadClass::_Get_Current_Thread_ID()==DX8Wrapper::_Get_Main_Thread_ID());
			DX8_ErrorCode(D3DTexture->UnlockRect(i));
#endif
#ifdef ZH
		return false;

#endif
		}
#ifdef OG
		LockedSurfacePtr[i]=NULL;

#endif
#ifdef ZH

	Targa targa;
	if (TARGA_ERROR_HANDLER(targa.Open(Texture->Get_Full_Path(), TGA_READMODE), Texture->Get_Full_Path())) {
		return false;
#endif
	}
#ifdef OG
	IsLoading=false;

#endif
#ifdef ZH

	// DX8 uses image upside down compared to TGA
	targa.Header.ImageDescriptor ^= TGAIDF_YORIGIN;

	WW3DFormat src_format;
	WW3DFormat dest_format;
	unsigned int src_bpp = 0;
	Get_WW3D_Format(dest_format,src_format,src_bpp,targa);
	if (src_format==WW3D_FORMAT_UNKNOWN) return false;

	dest_format = Get_Format();	// Texture can be requested in different format than the most obvious from the TGA

	char palette[256*4];
	targa.SetPalette(palette);

	unsigned int src_width	= targa.Header.Width;
	unsigned int src_height	= targa.Header.Height;
	unsigned int width		= Get_Width();
	unsigned int height		= Get_Height();
#endif

#ifdef ZH
	// NOTE: We load the palette but we do not yet support paletted textures!
	if (TARGA_ERROR_HANDLER(targa.Load(Texture->Get_Full_Path(), TGAF_IMAGE, false), Texture->Get_Full_Path())) {
		return false;
#endif
}
#ifdef ZH

	unsigned char * src_surface			= (unsigned char*)targa.GetImage();
	unsigned char * converted_surface	= NULL;

	// No paletted format allowed when generating mipmaps
	Vector3 hsv_shift=HSVShift;
	if (	src_format	== WW3D_FORMAT_A1R5G5B5 
		|| src_format	== WW3D_FORMAT_R5G6B5 
		|| src_format	== WW3D_FORMAT_A4R4G4B4 
		||	src_format	== WW3D_FORMAT_P8 
		|| src_format	== WW3D_FORMAT_L8 
		|| src_width	!= width 
		|| src_height	!= height) {
#endif

#ifdef OG
// ----------------------------------------------------------------------------
//
// Link the node to another task node. This can only be done if the node isn't
// linked to something else already. If the node is linked to some other node,
// the only acceptable parameter to this function is NULL, which will unlink
// the connection.
//
// ----------------------------------------------------------------------------

#endif
#ifdef ZH
		converted_surface = new unsigned char[width*height*4];
		dest_format = Get_Valid_Texture_Format(WW3D_FORMAT_A8R8G8B8, false);

		BitmapHandlerClass::Copy_Image(
			converted_surface,
			width,
			height,
			width*4,
			WW3D_FORMAT_A8R8G8B8,	//dest_format,
			src_surface,
			src_width,
			src_height,
			src_width*src_bpp,
			src_format,
			(unsigned char*)targa.GetPalette(),
			targa.Header.CMapDepth>>3,
			false,
			hsv_shift);
		hsv_shift=Vector3(0.0f,0.0f,0.0f);
#endif

#ifdef OG
void TextureLoadTaskClass::Set_Succ(TextureLoadTaskClass* succ)
{
	WWASSERT((succ && !Succ) || (!succ));	// Can't set successor pointer if it has been set already
	Succ=succ; 

#endif
#ifdef ZH
		src_surface	= converted_surface;
		src_format	= WW3D_FORMAT_A8R8G8B8;	//dest_format;
		src_width	= width;
		src_height	= height;
		src_bpp		= Get_Bytes_Per_Pixel(src_format);
#endif
}
#ifdef ZH

	unsigned src_pitch = src_width * src_bpp;
#endif

#ifdef OG
// ----------------------------------------------------------------------------
//
//
//
// ----------------------------------------------------------------------------

#endif
#ifdef ZH
	if (Reduction)
	{	//texture needs to be reduced so allocate storage for full-sized version.
		unsigned char * destination_surface	= new unsigned char[width*height*4];
		//generate upper mip-levels that will be dropped in final texture
		for (unsigned int level = 0; level < Reduction; ++level) {
		BitmapHandlerClass::Copy_Image(
			(unsigned char *)destination_surface,
			width,
			height,
			src_pitch,
			Get_Format(),
			src_surface,
			src_width,
			src_height,
			src_pitch,
			src_format,
			NULL,
			0,
			true,
			hsv_shift);
#endif

#ifdef OG
void TextureLoadTaskClass::Set_D3D_Texture(IDirect3DTexture8* texture)
{
	WWASSERT(D3DTexture==0);
	D3DTexture=texture;

#endif
#ifdef ZH
			width			>>= 1;
			height		>>= 1;
			src_width	>>= 1;
			src_height	>>= 1;
		}
		delete [] destination_surface;
#endif
}
#ifdef ZH

	for (unsigned int level = 0; level < Get_Mip_Level_Count(); ++level) {
		WWASSERT(Get_Locked_Surface_Ptr(level));
		BitmapHandlerClass::Copy_Image(
			Get_Locked_Surface_Ptr(level),
			width,
			height,
			Get_Locked_Surface_Pitch(level),
			Get_Format(),
			src_surface,
			src_width,
			src_height,
			src_pitch,
			src_format,
			NULL,
			0,
			true,
			hsv_shift);
		hsv_shift=Vector3(0.0f,0.0f,0.0f);
#endif

#ifdef OG
// ----------------------------------------------------------------------------
//
// Apply the D3D texture surface to the client texture. The parameter 'initialize'
// determines whether the client texture will be set to initialized state or
// not. Generally thumbnail tasks will not set texture to initialised state but
// all other loads do.
//
// ----------------------------------------------------------------------------
#endif
#ifdef ZH
		width			>>= 1;
		height		>>= 1;
		src_width	>>= 1;
		src_height	>>= 1;

#endif

#ifdef OG
void TextureLoadTaskClass::Apply(bool initialize)
{
	WWASSERT(D3DTexture);
	// Verify that none of the mip levels are locked
	for (unsigned i=0;i<MipLevelCount;++i) {
		WWASSERT(LockedSurfacePtr[i]==NULL);
#endif
#ifdef ZH
		if (!width || !height || !src_width || !src_height) {
			break;
		}

#endif
	}

#ifdef OG
	Texture->Apply_New_Surface(initialize);

#endif
#ifdef ZH
	if (converted_surface) {
		delete[] converted_surface;
	}
#endif

#ifdef OG
	D3DTexture->Release();
	D3DTexture=NULL;
#endif
#ifdef ZH
	return true;

#endif
}

#ifdef OG
// ----------------------------------------------------------------------------
//
// Return locked surface pointer at a specific level. The call will
// assert if level is greater or equal to the number of mip levels or if the
// requested level has not been locked.
//
// ----------------------------------------------------------------------------
#endif

#ifdef OG
unsigned char* TextureLoadTaskClass::Get_Locked_Surface_Ptr(unsigned level)
#endif
#ifdef ZH
unsigned char * TextureLoadTaskClass::Get_Locked_Surface_Ptr(unsigned int level)
#endif
{
	WWASSERT(level<MipLevelCount);
	WWASSERT(LockedSurfacePtr[level]);
	return LockedSurfacePtr[level];
}

// ----------------------------------------------------------------------------
//
// Return locked surface pitch (in bytes) at a specific level. The call will
// assert if level is greater or equal to the number of mip levels or if the
// requested level has not been locked.
//
// ----------------------------------------------------------------------------

#ifdef OG
unsigned TextureLoadTaskClass::Get_Locked_Surface_Pitch(unsigned level) const
#endif
#ifdef ZH
unsigned int TextureLoadTaskClass::Get_Locked_Surface_Pitch(unsigned int level) const
#endif
{
	WWASSERT(level<MipLevelCount);
	WWASSERT(LockedSurfacePtr[level]);
	return LockedSurfacePitch[level];
#ifdef ZH
}

// CubeTextureLoadTaskClass
CubeTextureLoadTaskClass::CubeTextureLoadTaskClass()
:	TextureLoadTaskClass()
{
	// because texture load tasks are pooled, the constructor and destructor
	// don't need to do much. The work of attaching a task to a texture is
	// is done by Init() and Deinit().

	for (int f=0;f<6;f++)
	{
		for (int i = 0; i < MIP_LEVELS_MAX; ++i) 
		{
			LockedCubeSurfacePtr[f][i]		= NULL;
			LockedCubeSurfacePitch[f][i]	= 0;
		}
	}
}

void CubeTextureLoadTaskClass::Destroy(void)
{
	// detach the task from its texture, and return to free pool.
	Deinit();
	_CubeTexLoadFreeList.Push_Front(this);
}

void CubeTextureLoadTaskClass::Init(TextureBaseClass* tc, TaskType type, PriorityType priority)
{
	WWASSERT(tc);

	// NOTE: we must be in the main thread to avoid corrupting the texture's refcount.
	WWASSERT(TextureLoader::Is_DX8_Thread());
	REF_PTR_SET(Texture, tc);

	// Make sure texture has a filename.
	WWASSERT(Texture->Get_Full_Path() != "");

	Type				= type;
	Priority			= priority;
	State				= STATE_NONE;

	D3DTexture		= 0;

	CubeTextureClass* tex=Texture->As_CubeTextureClass();

	if (tex)
	{
		Format			= tex->Get_Texture_Format(); // don't assume format yet KM
	}
	else
	{
		Format			= WW3D_FORMAT_UNKNOWN;
	}

	Width				= 0;
	Height			= 0;
	MipLevelCount	= Texture->MipLevelCount;
	Reduction		= Texture->Get_Reduction();
	HSVShift			= Texture->Get_HSV_Shift();

	for (int f=0; f<6; f++)
	{
		for (int i = 0; i < MIP_LEVELS_MAX; ++i) 
		{
			LockedCubeSurfacePtr[f][i]		= NULL;
			LockedCubeSurfacePitch[f][i]	= 0;
		}
	}

	switch (Type) 
	{
	case TASK_THUMBNAIL:
		WWASSERT(Texture->ThumbnailLoadTask == NULL);
		Texture->ThumbnailLoadTask = this;
		break;

	case TASK_LOAD:
		WWASSERT(Texture->TextureLoadTask == NULL);
		Texture->TextureLoadTask = this;
		break;
	}
}

void CubeTextureLoadTaskClass::Deinit()
{
	// task should not be on any list when it is being detached from texture.
	WWASSERT(Next == NULL);
	WWASSERT(Prev == NULL);

	WWASSERT(D3DTexture == NULL);

	for (int f=0; f<6; f++)
	{
		for (int i = 0; i < MIP_LEVELS_MAX; ++i) 
		{
			WWASSERT(LockedCubeSurfacePtr[f][i] == NULL);
		}
	}

	if (Texture) 
	{
		switch (Type) 
		{
			case TASK_THUMBNAIL:
				WWASSERT(Texture->ThumbnailLoadTask == this);
				Texture->ThumbnailLoadTask = NULL;
				break;

			case TASK_LOAD:
				WWASSERT(Texture->TextureLoadTask == this);
				Texture->TextureLoadTask = NULL;
				break;
		}

		// NOTE: we must be in main thread to avoid corrupting Texture's refcount.
		WWASSERT(TextureLoader::Is_DX8_Thread());
		REF_PTR_RELEASE(Texture);
	}
}

void CubeTextureLoadTaskClass::Lock_Surfaces(void)
{
	for (unsigned int f=0; f<6; f++)
	{
		for (unsigned int i=0; i<MipLevelCount; i++)
		{
			D3DLOCKED_RECT locked_rect;
			DX8_ErrorCode
			(
				Peek_D3D_Cube_Texture()->LockRect
				(
					(D3DCUBEMAP_FACES)f,
					i,
					&locked_rect,
					NULL,
					0
				)
			);
			LockedCubeSurfacePtr[f][i]	 = (unsigned char *)locked_rect.pBits;
			LockedCubeSurfacePitch[f][i]= locked_rect.Pitch;
		}
	}
}

void CubeTextureLoadTaskClass::Unlock_Surfaces(void)
{
	for (unsigned int f=0; f<6; f++)
	{
		for (unsigned int i = 0; i < MipLevelCount; ++i) 
		{
			if (LockedCubeSurfacePtr[f][i]) 
			{
				WWASSERT(ThreadClass::_Get_Current_Thread_ID() == DX8Wrapper::_Get_Main_Thread_ID());
				DX8_ErrorCode
				(
					Peek_D3D_Cube_Texture()->UnlockRect((D3DCUBEMAP_FACES)f,i)
				);
			}
			LockedCubeSurfacePtr[f][i] = NULL;
		}
	}

#ifndef USE_MANAGED_TEXTURES
	IDirect3DCubeTexture8* tex = DX8Wrapper::_Create_DX8_Cube_Texture
	(
		Width, 
		Height, 
		Format, 
		Texture->MipLevelCount,
		D3DPOOL_DEFAULT
	);
	DX8CALL(UpdateTexture(Peek_D3D_Volume_Texture(),tex));
	Peek_D3D_Volume_Texture()->Release();
	D3DTexture=tex;
	WWDEBUG_SAY(("Created non-managed texture (%s)\n",Texture->Get_Full_Path()));
#endif

}

bool CubeTextureLoadTaskClass::Begin_Compressed_Load()
{
	unsigned orig_w,orig_h,orig_d,orig_mip_count,reduction;
	WW3DFormat orig_format;
	if (!Get_Texture_Information
		  (
				Texture->Get_Full_Path(),
				reduction,
				orig_w,
				orig_h,
				orig_d,
				orig_format,
				orig_mip_count,
				true
		  )
		)
	{
		return false;
	}

	// Destination size will be the next power of two square from the larger width and height...
	unsigned int width	= orig_w;
	unsigned int height	= orig_h;
	TextureLoader::Validate_Texture_Size(width, height,orig_d);

	// If the size doesn't match, try and see if texture reduction would help... (mainly for
	// cases where loaded texture is larger than hardware limit)
	if (width != orig_w || height != orig_h) 
	{
		for (unsigned int i = 1; i < orig_mip_count; ++i) 
		{
			unsigned w=orig_w>>i;
			if (w<4) w=4;
			unsigned h=orig_h>>i;
			if (h<4) h=4;
			unsigned tmp_w=w;
			unsigned tmp_h=h;

			TextureLoader::Validate_Texture_Size(w,h,orig_d);

			if (w == tmp_w && h == tmp_h) 
			{
				Reduction	+= i;
				width			=	w;
				height		=	h;
				break;
			}
		}
	}

	Width		= width;
	Height	= height;
	Format	= Get_Valid_Texture_Format(orig_format, Texture->Is_Compression_Allowed());

	unsigned int mip_level_count = Get_Mip_Level_Count();

	// If texture wants all mip levels, take as many as the file contains (not necessarily all)
	// Otherwise take as many mip levels as the texture wants, not to exceed the count in file...
	if (!mip_level_count) 
	{
		mip_level_count = orig_mip_count;//dds_file.Get_Mip_Level_Count();
	} 
	else if (mip_level_count > orig_mip_count) 
	{//dds_file.Get_Mip_Level_Count()) {
		mip_level_count = orig_mip_count;//dds_file.Get_Mip_Level_Count();
	}

	// Once more, verify that the mip level count is correct (in case it was changed here it might not
	// match the size...well actually it doesn't have to match but it can't be bigger than the size)
	unsigned int max_mip_level_count = 1;
	unsigned int w = 4;
	unsigned int h = 4;

	while (w < Width && h < Height) 
	{
		w += w;
		h += h;
		max_mip_level_count++;
	}

	if (mip_level_count > max_mip_level_count) 
	{
		mip_level_count = max_mip_level_count;
	}

	D3DTexture	= DX8Wrapper::_Create_DX8_Cube_Texture
	(
		Width, 
		Height, 
		Format, 
		(MipCountType)mip_level_count,
#ifdef USE_MANAGED_TEXTURES
		D3DPOOL_MANAGED
#else
		D3DPOOL_SYSTEMMEM
#endif
	);

	MipLevelCount = mip_level_count;
	return true;
}

bool CubeTextureLoadTaskClass::Begin_Uncompressed_Load(void)
{
	unsigned width,height,depth,orig_mip_count,reduction;
	WW3DFormat orig_format;
	if (!Get_Texture_Information
		  (
				Texture->Get_Full_Path(),
				reduction,
				width,
				height,
				depth,
				orig_format,
				orig_mip_count,
				false
			)
		)
	{
		return false;
	}

	WW3DFormat src_format=orig_format;
	WW3DFormat dest_format=src_format;
	dest_format=Get_Valid_Texture_Format(dest_format,false);	// No compressed destination format if reading from targa...

   if (		src_format != WW3D_FORMAT_A8R8G8B8 
   		&&	src_format != WW3D_FORMAT_R8G8B8 
  			&&	src_format != WW3D_FORMAT_X8R8G8B8 ) 
	{
		WWDEBUG_SAY(("Invalid TGA format used in %s - only 24 and 32 bit formats should be used!\n", Texture->Get_Full_Path()));
	}

	// Destination size will be the next power of two square from the larger width and height...
	unsigned ow = width;
	unsigned oh = height;
	TextureLoader::Validate_Texture_Size(width, height,depth);
	if (width != ow || height != oh) 
	{
		WWDEBUG_SAY(("Invalid texture size, scaling required. Texture: %s, size: %d x %d -> %d x %d\n", Texture->Get_Full_Path(), ow, oh, width, height));
	}

	Width		= width;
	Height	= height;

	if (Format == WW3D_FORMAT_UNKNOWN) 
	{
		Format=dest_format;
	}
	else 
	{
		Format = Get_Valid_Texture_Format(Format, false);
	}

	D3DTexture = DX8Wrapper::_Create_DX8_Cube_Texture
	(
		Width, 
		Height, 
		Format, 
		Texture->MipLevelCount,
#ifdef USE_MANAGED_TEXTURES
		D3DPOOL_MANAGED
#else
		D3DPOOL_SYSTEMMEM
#endif
	);

	return true;
}

bool CubeTextureLoadTaskClass::Load_Compressed_Mipmap(void)
{
	DDSFileClass dds_file(Texture->Get_Full_Path(), Get_Reduction());

	// if we can't load from file, indicate rror.
	if (!dds_file.Is_Available() || !dds_file.Load()) 
	{
		return false;
	}

	// load cube map faces
	for (unsigned int face=0; face<6; face++)
	{
		unsigned int width = Get_Width();
		unsigned int height = Get_Height();

		for (unsigned int level=0; level<Get_Mip_Level_Count(); level++)
		{
			WWASSERT(width && height);

			// get cube map surface
			dds_file.Copy_CubeMap_Level_To_Surface
			(
				face,
				level,
				Get_Format(),
				width,
				height,
				Get_Locked_CubeMap_Surface_Pointer(face,level),
				Get_Locked_CubeMap_Surface_Pitch(face,level),
				HSVShift
			);

			width>>=1;
			height>>=1;
		}
	}

	return true;
}

unsigned char*	CubeTextureLoadTaskClass::Get_Locked_CubeMap_Surface_Pointer(unsigned int face, unsigned int level)
{
	WWASSERT(face<6 && level<MipLevelCount);
	WWASSERT(LockedCubeSurfacePtr[face][level]);
	return LockedCubeSurfacePtr[face][level];
}

unsigned int CubeTextureLoadTaskClass::Get_Locked_CubeMap_Surface_Pitch(unsigned int face, unsigned int level) const
{
	WWASSERT(face<6 && level<MipLevelCount);
	WWASSERT(LockedCubeSurfacePitch[face][level]);
	return LockedCubeSurfacePitch[face][level];
}

// VolumeTextureLoadTaskClass
VolumeTextureLoadTaskClass::VolumeTextureLoadTaskClass()
:	TextureLoadTaskClass()
{
	// because texture load tasks are pooled, the constructor and destructor
	// don't need to do much. The work of attaching a task to a texture is
	// is done by Init() and Deinit().

	for (int i = 0; i < MIP_LEVELS_MAX; ++i) 
	{
		LockedSurfacePtr[i]			= NULL;
		LockedSurfacePitch[i]		= 0;
		LockedSurfaceSlicePitch[i]	= 0;
#endif
}
#ifdef ZH
}

void VolumeTextureLoadTaskClass::Destroy(void)
{
	// detach the task from its texture, and return to free pool.
	Deinit();
	_VolTexLoadFreeList.Push_Front(this);
}

void VolumeTextureLoadTaskClass::Init(TextureBaseClass* tc, TaskType type, PriorityType priority)
{
	WWASSERT(tc);

	// NOTE: we must be in the main thread to avoid corrupting the texture's refcount.
	WWASSERT(TextureLoader::Is_DX8_Thread());
	REF_PTR_SET(Texture, tc);

	// Make sure texture has a filename.
	WWASSERT(Texture->Get_Full_Path() != "");

	Type				= type;
	Priority			= priority;
	State				= STATE_NONE;

	D3DTexture		= 0;

	VolumeTextureClass* tex=Texture->As_VolumeTextureClass();

	if (tex)
	{
		Format			= tex->Get_Texture_Format(); // don't assume format yet KM
	}
	else
	{
		Format			= WW3D_FORMAT_UNKNOWN;
	}

	Width				= 0;
	Height			= 0;
	Depth				= 0;
	MipLevelCount	= Texture->MipLevelCount;
	Reduction		= Texture->Get_Reduction();
	HSVShift			= Texture->Get_HSV_Shift();

	for (int i = 0; i < MIP_LEVELS_MAX; ++i) 
	{
		LockedSurfacePtr[i]			= NULL;
		LockedSurfacePitch[i]		= 0;
		LockedSurfaceSlicePitch[i]	= 0;
	}

	switch (Type) 
	{
	case TASK_THUMBNAIL:
		WWASSERT(Texture->ThumbnailLoadTask == NULL);
		Texture->ThumbnailLoadTask = this;
		break;

	case TASK_LOAD:
		WWASSERT(Texture->TextureLoadTask == NULL);
		Texture->TextureLoadTask = this;
		break;
	}
}

void VolumeTextureLoadTaskClass::Lock_Surfaces()
{
	for (unsigned int i=0; i<MipLevelCount; i++)
	{
		D3DLOCKED_BOX locked_box;
		DX8_ErrorCode
		(
			Peek_D3D_Volume_Texture()->LockBox
			(
				i,
				&locked_box,
				NULL,
				0
			)
		);
		LockedSurfacePtr[i]			= (unsigned char *)locked_box.pBits;
		LockedSurfacePitch[i]		= locked_box.RowPitch;
		LockedSurfaceSlicePitch[i]	= locked_box.SlicePitch;
	}
}

void VolumeTextureLoadTaskClass::Unlock_Surfaces()
{
	for (unsigned int i = 0; i < MipLevelCount; ++i) 
	{
		if (LockedSurfacePtr[i]) 
		{
			WWASSERT(ThreadClass::_Get_Current_Thread_ID() == DX8Wrapper::_Get_Main_Thread_ID());
			DX8_ErrorCode
			(
				Peek_D3D_Volume_Texture()->UnlockBox(i)
			);
		}
		LockedSurfacePtr[i] = NULL;
	}

#ifndef USE_MANAGED_TEXTURES
	IDirect3DTexture8* tex = DX8Wrapper::_Create_DX8_Volume_Texture(Width, Height, Depth, Format, Texture->MipLevelCount,D3DPOOL_DEFAULT);
	DX8CALL(UpdateTexture(Peek_D3D_Volume_Texture(),tex));
	Peek_D3D_Volume_Texture()->Release();
	D3DTexture=tex;
	WWDEBUG_SAY(("Created non-managed texture (%s)\n",Texture->Get_Full_Path()));
#endif

}

bool VolumeTextureLoadTaskClass::Begin_Compressed_Load()
{
	unsigned orig_w,orig_h,orig_d,orig_mip_count,reduction;
	WW3DFormat orig_format;
	if (!Get_Texture_Information
		  (
				Texture->Get_Full_Path(),
				reduction,
				orig_w,
				orig_h,
				orig_d,
				orig_format,
				orig_mip_count,
				true
		  )
		)
	{
		return false;
	}

	// Destination size will be the next power of two square from the larger width and height...
	unsigned int width	= orig_w;
	unsigned int height	= orig_h;
	unsigned int depth	= orig_d;
	TextureLoader::Validate_Texture_Size(width, height, depth);

	// If the size doesn't match, try and see if texture reduction would help... (mainly for
	// cases where loaded texture is larger than hardware limit)
	if (width != orig_w || height != orig_h || depth != orig_d) 
	{
		for (unsigned int i = 1; i < orig_mip_count; ++i) 
		{
			unsigned w=orig_w>>i;
			if (w<4) w=4;
			unsigned h=orig_h>>i;
			if (h<4) h=4;
			unsigned d=orig_d>>i;
			if (d<1) d=1;
			unsigned tmp_w=w;
			unsigned tmp_h=h;
			unsigned tmp_d=d;

			TextureLoader::Validate_Texture_Size(w,h,d);

			if (w == tmp_w && h == tmp_h && d== tmp_d) 
			{
				Reduction	+= i;
				width			=	w;
				height		=	h;
				depth			=  d;
				break;
			}
		}
	}

	Width		= width;
	Height	= height;
	Depth		= depth;
	Format	= Get_Valid_Texture_Format(orig_format, Texture->Is_Compression_Allowed());

	unsigned int mip_level_count = Get_Mip_Level_Count();

	// If texture wants all mip levels, take as many as the file contains (not necessarily all)
	// Otherwise take as many mip levels as the texture wants, not to exceed the count in file...
	if (!mip_level_count) 
	{
		mip_level_count = orig_mip_count;//dds_file.Get_Mip_Level_Count();
	} 
	else if (mip_level_count > orig_mip_count) 
	{//dds_file.Get_Mip_Level_Count()) {
		mip_level_count = orig_mip_count;//dds_file.Get_Mip_Level_Count();
	}
#endif

#ifdef OG
// ----------------------------------------------------------------------------
//
// Load tasks are stored in a pool when they are not used. If the pool is empty
// a new task is created.
//
// ----------------------------------------------------------------------------

#endif
#ifdef ZH
	// Once more, verify that the mip level count is correct (in case it was changed here it might not
	// match the size...well actually it doesn't have to match but it can't be bigger than the size)
	unsigned int max_mip_level_count = 1;
	unsigned int w = 4;
	unsigned int h = 4;

	while (w < Width && h < Height) 
	{
		w += w;
		h += h;
		max_mip_level_count++;
	}
	if (mip_level_count > max_mip_level_count) 
	{
		mip_level_count = max_mip_level_count;
	}

	D3DTexture	= DX8Wrapper::_Create_DX8_Volume_Texture
	(
		Width, 
		Height, 
		Depth,
		Format, 
		(MipCountType)mip_level_count,
#ifdef USE_MANAGED_TEXTURES
		D3DPOOL_MANAGED
#else
		D3DPOOL_SYSTEMMEM
#endif
	);

	MipLevelCount = mip_level_count;
	return true;
}

bool VolumeTextureLoadTaskClass::Begin_Uncompressed_Load(void)
{
	unsigned width,height,depth,orig_mip_count,reduction;
	WW3DFormat orig_format;
	if (!Get_Texture_Information
		  (
				Texture->Get_Full_Path(),
				reduction,
				width,
				height,
				depth,
				orig_format,
				orig_mip_count,
				false
			)
		)
	{
		return false;
	}

	WW3DFormat src_format=orig_format;
	WW3DFormat dest_format=src_format;
	dest_format=Get_Valid_Texture_Format(dest_format,false);	// No compressed destination format if reading from targa...

   if (		src_format != WW3D_FORMAT_A8R8G8B8 
   		&&	src_format != WW3D_FORMAT_R8G8B8 
  			&&	src_format != WW3D_FORMAT_X8R8G8B8 ) 
	{
		WWDEBUG_SAY(("Invalid TGA format used in %s - only 24 and 32 bit formats should be used!\n", Texture->Get_Full_Path()));
	}
#endif

#ifdef OG
TextureLoadTaskClass* TextureLoadTaskClass::Get_Instance(TextureClass* tc, bool high_priority)

#endif
#ifdef ZH
	// Destination size will be the next power of two square from the larger width and height...
	unsigned ow = width;
	unsigned oh = height;
	unsigned od = depth;
	TextureLoader::Validate_Texture_Size(width, height, depth);
	if (width != ow || height != oh || depth != od) 
#endif
{
#ifdef OG
	CriticalSectionClass::LockClass m(mutex);

#endif
#ifdef ZH
		WWDEBUG_SAY(("Invalid texture size, scaling required. Texture: %s, size: %d x %d -> %d x %d\n", Texture->Get_Full_Path(), ow, oh, width, height));
	}

	Width		= width;
	Height	= height;
	Depth		= depth;
#endif

#ifdef OG
	TextureLoadTaskClass* task=FreeTaskListHead;
	if (task) {
		FreeTaskListHead=task->Peek_Succ();
		task->Set_Succ(NULL);

#endif
#ifdef ZH
	if (Format == WW3D_FORMAT_UNKNOWN) 
	{
		Format=dest_format;
	}
	else 
	{
		Format = Get_Valid_Texture_Format(Format, false);
#endif
	}
#ifdef OG
	else {
		task=W3DNEW TextureLoadTaskClass();

#endif
#ifdef ZH

	D3DTexture = DX8Wrapper::_Create_DX8_Volume_Texture
	(
		Width, 
		Height, 
		Depth,
		Format, 
		Texture->MipLevelCount,
#ifdef USE_MANAGED_TEXTURES
		D3DPOOL_MANAGED
#else
		D3DPOOL_SYSTEMMEM
#endif
	);

	return true;
#endif
	}
#ifdef OG
	task->Init(tc,high_priority);
	return task;

#endif
#ifdef ZH

bool VolumeTextureLoadTaskClass::Load_Compressed_Mipmap(void)
{
	DDSFileClass dds_file(Texture->Get_Full_Path(), Get_Reduction());

	// if we can't load from file, indicate rror.
	if (!dds_file.Is_Available() || !dds_file.Load()) 
	{
		return false;
#endif
}
#ifdef ZH

	// load volume
	unsigned int depth=dds_file.Get_Depth(0);
	unsigned int width=Get_Width();
	unsigned int height=Get_Height();
#endif

#ifdef OG
// ----------------------------------------------------------------------------
//
// When task is no longer needed it is returned to the pool.
//
// ----------------------------------------------------------------------------
#endif
#ifdef ZH
	WWASSERT(width && height && depth);

#endif

#ifdef OG
void TextureLoadTaskClass::Release_Instance(TextureLoadTaskClass* task)
#endif
#ifdef ZH
	for (unsigned int level=0; level<Get_Mip_Level_Count(); level++)
#endif
{
#ifdef OG
	if (!task) return;

#endif
#ifdef ZH
		if (width<1) width=1;
		if (height<1) height=1;
		if (depth<1) depth=1;

		// get volume
		dds_file.Copy_Volume_Level_To_Surface
		(
			level,
			depth,
			Get_Format(),
			width,
			height,
			Get_Locked_Volume_Pointer(level),
			Get_Locked_Volume_Row_Pitch(level),
			Get_Locked_Volume_Slice_Pitch(level),
			HSVShift	
		);
#endif

#ifdef OG
	CriticalSectionClass::LockClass m(mutex);

#endif
#ifdef ZH
		width>>=1;
		height>>=1;
		depth>>=1;
	}
#endif

#ifdef OG
	task->Deinit();

#endif
#ifdef ZH
	return true;
}
#endif

#ifdef OG
	// Task must not be in any list when it is being freed
	WWASSERT(task->Peek_Succ()==NULL);

#endif
#ifdef ZH
unsigned char* VolumeTextureLoadTaskClass::Get_Locked_Volume_Pointer(unsigned int level)
{
	WWASSERT(level<MipLevelCount);
	WWASSERT(LockedSurfacePtr[level]);
	return LockedSurfacePtr[level];
}
#endif

#ifdef OG
	task->Set_Succ(FreeTaskListHead);
	FreeTaskListHead=task;

#endif
#ifdef ZH
unsigned int VolumeTextureLoadTaskClass::Get_Locked_Volume_Row_Pitch(unsigned int level)
{
	WWASSERT(level<MipLevelCount);
	WWASSERT(LockedSurfacePtr[level]);
	return LockedSurfacePitch[level];
#endif
}

#ifdef ZH
unsigned int VolumeTextureLoadTaskClass::Get_Locked_Volume_Slice_Pitch(unsigned int level)
{
	WWASSERT(level<MipLevelCount);
	WWASSERT(LockedSurfacePtr[level]);
	return LockedSurfaceSlicePitch[level];
}
#endif

