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
// 08/07/02 KM Texture class redesign (revisited)
#endif
#ifndef TEXTURETHUMBNAIL_H
#define TEXTURETHUMBNAIL_H

#if defined(_MSC_VER)
#pragma once
#endif

#include "always.h"
#include "wwstring.h"
#ifdef ZH
#include "hashtemplate.h"
#include "dllist.h"
#include "ww3dformat.h"
#endif

#ifdef ZH
#define GLOBAL_THUMBNAIL_MANAGER_FILENAME "global.th6"

class ThumbnailManagerClass;

#endif
// ----------------------------------------------------------------------------

class ThumbnailClass
{
#ifdef ZH
	friend ThumbnailManagerClass;

#endif
	StringClass Name;
	unsigned char* Bitmap;
	unsigned Width;
	unsigned Height;
#ifdef ZH
	unsigned OriginalTextureWidth;
	unsigned OriginalTextureHeight;
	unsigned OriginalTextureMipLevelCount;
	WW3DFormat OriginalTextureFormat;
	unsigned long DateTime;
#endif
	bool Allocated;	// if true, destructor will free the memory
#ifdef ZH
	ThumbnailManagerClass* Manager;

	ThumbnailClass(
		ThumbnailManagerClass* manager,
		const char* name, 
		unsigned char* bitmap, 
		unsigned w, 
		unsigned h, 
		unsigned original_w, 
		unsigned original_h, 
		unsigned original_mip_level_count,
		WW3DFormat original_format,
		bool allocated,
		unsigned long date_time);
	ThumbnailClass(
		ThumbnailManagerClass* manager,
		const StringClass& filename);
	~ThumbnailClass();
#endif
public:
#ifdef OG
	ThumbnailClass(const char* name, unsigned char* bitmap, unsigned w, unsigned h, bool allocated);
	ThumbnailClass(const StringClass& filename);
	~ThumbnailClass();
#endif

	unsigned char* Peek_Bitmap() { return Bitmap; }
#ifdef ZH
	WW3DFormat Get_Format() { return WW3D_FORMAT_A4R4G4B4; }
#endif
	unsigned Get_Width() const { return Width; }
	unsigned Get_Height() const { return Height; }
#ifdef ZH
	unsigned Get_Original_Texture_Width() const { return OriginalTextureWidth; }
	unsigned Get_Original_Texture_Height() const { return OriginalTextureHeight; }
	unsigned Get_Original_Texture_Mip_Level_Count() const { return OriginalTextureMipLevelCount; }
	WW3DFormat Get_Original_Texture_Format() const { return OriginalTextureFormat; }
	unsigned long Get_Date_Time() const { return DateTime; }
#endif
	const StringClass& Get_Name() const { return Name; }
#ifdef ZH

};

// ----------------------------------------------------------------------------
#endif

#ifdef OG
	static ThumbnailClass* Peek_Instance(const StringClass& name);

#endif
#ifdef ZH
class ThumbnailManagerClass : public DLNodeClass<ThumbnailManagerClass>
{
	W3DMPO_GLUE(ThumbnailManagerClass);

	friend ThumbnailClass;

	static bool CreateThumbnailIfNotFound;
	bool PerTextureTimeStampUsed;
	StringClass ThumbnailFileName;
	StringClass MixFileName;
	HashTemplateClass<StringClass,ThumbnailClass*> ThumbnailHash;
	unsigned char* ThumbnailMemory;
	bool Changed;
	unsigned long DateTime;

	ThumbnailManagerClass(const char* thumbnail_filename, const char* mix_file_name);
	~ThumbnailManagerClass();

	void ThumbnailManagerClass::Remove_From_Hash(ThumbnailClass* thumb);
	void ThumbnailManagerClass::Insert_To_Hash(ThumbnailClass* thumb);
	ThumbnailClass* ThumbnailManagerClass::Get_From_Hash(const StringClass& name);

	void Create_Thumbnails();
	static void Update_Thumbnail_File(const char* mix_file_name, bool display_message_box);

	void Load();
	void Save(bool force=false);
public:
	ThumbnailClass* Peek_Thumbnail_Instance(const StringClass& name);

	static void Add_Thumbnail_Manager(const char* thumbnail_filename, const char* mix_file_name);
	static void Remove_Thumbnail_Manager(const char* thumbnail_filename);
	static ThumbnailManagerClass* Peek_Thumbnail_Manager(const char* thumbnail_filename);

	static ThumbnailClass* Peek_Thumbnail_Instance_From_Any_Manager(const StringClass& name);

	static bool Is_Thumbnail_Created_If_Not_Found() { return CreateThumbnailIfNotFound; }
	static void Create_Thumbnail_If_Not_Found(bool create) { CreateThumbnailIfNotFound=create; }

	bool Is_Per_Texture_Time_Stamp_Used() const { return PerTextureTimeStampUsed; }
	void Enable_Per_Texture_Time_Stamp(bool enable) { PerTextureTimeStampUsed=enable; }

	static void Pre_Init(bool display_message_box);
#endif
	static void Init();
	static void Deinit();
};

// ----------------------------------------------------------------------------

#endif
