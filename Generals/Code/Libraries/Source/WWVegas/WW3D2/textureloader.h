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
 *                     $Modtime:: 06/27/02 1:27p                                              $*
 *                                                                                             *
 *                    $Revision:: 2                                                           $*
 *                                                                                             *
 * 06/27/02 KM Texture class abstraction																			*
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#endif
#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#if defined(_MSC_VER)
#pragma once
#endif

#include "always.h"
#include "texture.h"

class StringClass;
struct IDirect3DTexture8;
class TextureLoadTaskClass;

class TextureLoader
{
#ifdef OG
	static void Init_Load_Task(TextureClass* tc);

#endif
#ifdef ZH
public:
	static void Init(void);
	static void Deinit(void);

	// Modify given texture size to nearest valid size on current hardware.
	static void Validate_Texture_Size(unsigned& width, unsigned& height, unsigned& depth);
#endif

#ifdef OG
	static bool Load_Uncompressed_Mipmap_Levels_From_TGA(TextureLoadTaskClass* texture);

#endif
#ifdef ZH
	static IDirect3DTexture8 * Load_Thumbnail(
		const StringClass& filename,const Vector3& hsv_shift);
//		WW3DFormat texture_format);	// Pass WW3D_FORMAT_UNKNOWN if you don't care
#endif

#ifdef OG
public:
	static void Init();
	static void Deinit();

#endif
#ifdef ZH
	static IDirect3DSurface8 *		Load_Surface_Immediate(
		const StringClass& filename,
		WW3DFormat surface_format,		// Pass WW3D_FORMAT_UNKNOWN if you don't care
		bool allow_compression);
#endif

#ifdef OG
	// Modify given texture size to nearest valid size on current hardware.
	static void Validate_Texture_Size(unsigned& width, unsigned& height);
#endif
#ifdef ZH
	static void	Request_Thumbnail(TextureBaseClass* tc);

#endif

	// Adds a loading task to the system. The task if processed in a separate
	// thread as soon as possible. The task will appear in finished tasks list
	// when it's been completed. The texture will be refreshed on the next
	// update call after appearing to the finished tasks list.
#ifdef OG
	static void Add_Load_Task(TextureClass* tc);
	static IDirect3DTexture8* Load_Thumbnail(
		const StringClass& filename,
		WW3DFormat texture_format);	// Pass WW3D_FORMAT_UNKNOWN if you don't care
	static void Load_Mipmap_Levels(TextureLoadTaskClass* texture);

	static IDirect3DSurface8* Load_Surface_Immediate(
		const StringClass& filename,
		WW3DFormat surface_format,		// Pass WW3D_FORMAT_UNKNOWN if you don't care
		bool allow_compression);
#endif
#ifdef ZH
	static void Request_Background_Loading(TextureBaseClass* tc);

#endif

	// Textures can only be created and locked by the main thread so this function sends a request to the texture
	// handling system to load the texture immediatelly next time it enters the main thread. If this function
	// is called from the main thread the texture is loaded immediatelly.
#ifdef OG
	static void Request_High_Priority_Loading(
		TextureClass* texture,
		TextureClass::MipCountType mip_level_count);
	static void	Request_Thumbnail(TextureClass* tc);

#endif
#ifdef ZH
	static void Request_Foreground_Loading(TextureBaseClass* tc);

	static void	Flush_Pending_Load_Tasks(void);
	static void Update(void(*network_callback)(void) = NULL);

	// returns true if current thread of execution is allowed to make DX8 calls.
	static bool Is_DX8_Thread(void);

	static void Suspend_Texture_Load();
	static void Continue_Texture_Load();

	static void Set_Texture_Inactive_Override_Time(int time_ms) {TextureInactiveOverrideTime = time_ms;}

private:
	static void Process_Foreground_Load			(TextureLoadTaskClass *task);
	static void Process_Foreground_Thumbnail	(TextureLoadTaskClass *task);

	static void Begin_Load_And_Queue				(TextureLoadTaskClass *task);
	static void Load_Thumbnail						(TextureBaseClass *tc);

	static bool TextureLoadSuspended;

	// The time in ms before a texture is thrown out.
	// The default is zero.  The scripted movies set this to reduce texture stalls in movies.
	static int	TextureInactiveOverrideTime;
};

class TextureLoadTaskListNodeClass
{
	friend class TextureLoadTaskListClass;

	public:
		TextureLoadTaskListNodeClass(void) : Next(0), Prev(0) { }

		TextureLoadTaskListClass *Get_List(void)		{ return List; }

		TextureLoadTaskListNodeClass *Next;
		TextureLoadTaskListNodeClass *Prev;
		TextureLoadTaskListClass *		List;
};

class TextureLoadTaskListClass
{
	// This class implements an unsynchronized, double-linked list of TextureLoadTaskClass 
	// objects, using an embedded list node.

	public:
		TextureLoadTaskListClass(void);

		// Returns true if list is empty, false otherwise.
		bool									Is_Empty		(void) const		{ return (Root.Next == &Root); }

		// Add a task to beginning of list
		void									Push_Front	(TextureLoadTaskClass *task);

		// Add a task to end of list
		void									Push_Back	(TextureLoadTaskClass *task);

		// Remove and return a task from beginning of list, or NULL if list is empty.
		TextureLoadTaskClass *			Pop_Front	(void);
#endif

#ifdef OG
	static void Update();
	static void Flush_Pending_Load_Tasks();
#endif
#ifdef ZH
		// Remove and return a task from end of list, or NULL if list is empty
		TextureLoadTaskClass *			Pop_Back		(void);
#endif

#ifdef OG
	static IDirect3DTexture8* Generate_Bumpmap(TextureClass* texture);

#endif
#ifdef ZH
		// Remove specified task from list, if present
		void									Remove		(TextureLoadTaskClass *task);
#endif

#ifdef ZH
	private:
		// This list is implemented using a sentinel node.
		TextureLoadTaskListNodeClass	Root;
#endif
};

#ifdef OG
// ----------------------------------------------------------------------------
//
// Texture loader task handler
//
// ----------------------------------------------------------------------------
#endif

#ifdef OG
class TextureLoadTaskClass : public W3DMPO
#endif
#ifdef ZH
class SynchronizedTextureLoadTaskListClass : public TextureLoadTaskListClass
#endif
{
#ifdef OG
	W3DMPO_GLUE(TextureLoadTaskClass)

#endif
#ifdef ZH
	// This class added thread-safety to the basic TextureLoadTaskListClass.

	public:
		SynchronizedTextureLoadTaskListClass(void);

		// See comments above for description of member functions.
		void									Push_Front	(TextureLoadTaskClass *task);
		void									Push_Back	(TextureLoadTaskClass *task);
		TextureLoadTaskClass *			Pop_Front	(void);
		TextureLoadTaskClass *			Pop_Back		(void);
		void									Remove		(TextureLoadTaskClass *task);

	private:
		FastCriticalSectionClass		CriticalSection;
};
#endif

#ifdef OG
	static TextureLoadTaskClass* FreeTaskListHead;

#endif
#ifdef ZH
/*
** (gth) The allocation system we're using for TextureLoadTaskClass has gotten a little
** complicated since Kenny added the new task types for Cube and Volume textures.  The
** ::Destroy member is used to return a task to the pool now and must be over-ridden in
** each derived class to put the task back into the correct free list.  
*/
#endif

#ifdef OG
	TextureClass* Texture;
	IDirect3DTexture8 *D3DTexture;
	unsigned Width;
	unsigned Height;
	WW3DFormat Format;
	unsigned char* LockedSurfacePtr[TextureClass::MIP_LEVELS_MAX];
	unsigned LockedSurfacePitch[TextureClass::MIP_LEVELS_MAX];
	unsigned MipLevelCount;
	unsigned Reduction;
	TextureLoadTaskClass* Succ;
	bool IsLoading;
	bool HasFailed;
	bool HighPriorityRequested;
#endif

#ifdef OG
	~TextureLoadTaskClass();
	TextureLoadTaskClass();
#endif
#ifdef ZH
class TextureLoadTaskClass : public TextureLoadTaskListNodeClass
{
#endif
public:
#ifdef OG
	static TextureLoadTaskClass* Get_Instance(TextureClass* tc, bool high_priority);
	static void Release_Instance(TextureLoadTaskClass* task);
	static void shutdown(void) {TextureLoadTaskClass *pT; while (FreeTaskListHead) {pT = FreeTaskListHead; FreeTaskListHead = pT->Peek_Succ(); pT->Set_Succ(NULL); delete pT;} };

#endif
#ifdef ZH
		enum TaskType {
			TASK_NONE,
			TASK_THUMBNAIL,
			TASK_LOAD,
		};

		enum PriorityType {
			PRIORITY_LOW,
			PRIORITY_HIGH,
		};

		enum StateType {
			STATE_NONE,

			STATE_LOAD_BEGUN,
			STATE_LOAD_MIPMAP,
			STATE_LOAD_COMPLETE,

			STATE_COMPLETE,
		};

		TextureLoadTaskClass(void);
		~TextureLoadTaskClass(void);

		static TextureLoadTaskClass *	Create			(TextureBaseClass *tc, TaskType type, PriorityType priority);
		static void				Delete_Free_Pool			(void);

		virtual void			Destroy						(void);
		virtual void			Init							(TextureBaseClass *tc, TaskType type, PriorityType priority);
		virtual void			Deinit						(void);

		TaskType					Get_Type						(void) const		{ return Type;				}
		PriorityType			Get_Priority				(void) const		{ return Priority;		}
		StateType				Get_State					(void) const		{ return State;			}

		WW3DFormat				Get_Format					(void) const		{ return Format;			}
		unsigned int			Get_Width					(void) const		{ return Width;			}
		unsigned int			Get_Height					(void) const		{ return Height;			}
		unsigned int			Get_Mip_Level_Count		(void) const		{ return MipLevelCount; }
		unsigned int			Get_Reduction				(void) const		{ return Reduction;		}

		unsigned char *		Get_Locked_Surface_Ptr	(unsigned int level);
		unsigned int			Get_Locked_Surface_Pitch(unsigned int level) const;

		TextureBaseClass *	Peek_Texture				(void)				{ return Texture;			}
		IDirect3DTexture8	*	Peek_D3D_Texture			(void)				{ return (IDirect3DTexture8*)D3DTexture;		}

		void						Set_Type						(TaskType t)		{ Type		= t;			}
		void						Set_Priority				(PriorityType p)	{ Priority	= p;			}
		void						Set_State					(StateType s)		{ State		= s;			}

		bool						Begin_Load					(void);
		bool						Load							(void);
		void						End_Load						(void);
		void						Finish_Load					(void);
		void						Apply_Missing_Texture	(void);						

	protected:
		virtual bool			Begin_Compressed_Load	(void);
		virtual bool			Begin_Uncompressed_Load	(void);

		virtual bool			Load_Compressed_Mipmap	(void);
		virtual bool			Load_Uncompressed_Mipmap(void);

		virtual void			Lock_Surfaces				(void);
		virtual void			Unlock_Surfaces			(void);

		void						Apply							(bool initialize);
		
		TextureBaseClass*		Texture;
		IDirect3DBaseTexture8*	D3DTexture;
		WW3DFormat				Format;

		unsigned int			Width;
		unsigned	int			Height;
		unsigned	int			MipLevelCount;
		unsigned	int			Reduction;
		Vector3					HSVShift;
#endif

#ifdef OG
	void Init(TextureClass* tc,bool high_priority);
	void Deinit();
#endif
#ifdef ZH
		unsigned char *		LockedSurfacePtr[MIP_LEVELS_MAX];
		unsigned	int			LockedSurfacePitch[MIP_LEVELS_MAX];
#endif

#ifdef OG
	unsigned Get_Mip_Level_Count() const { return MipLevelCount; }
	unsigned Get_Width() const { return Width; }
	unsigned Get_Height() const { return Height; }
	WW3DFormat Get_Format() const { return Format; }
	unsigned Get_Reduction() const { return Reduction; }
#endif
#ifdef ZH
		TaskType					Type;
		PriorityType			Priority;
		StateType				State;
};

#endif

#ifdef OG
	unsigned char* Get_Locked_Surface_Ptr(unsigned level);
	unsigned Get_Locked_Surface_Pitch(unsigned level) const;

#endif
#ifdef ZH
class CubeTextureLoadTaskClass : public TextureLoadTaskClass
{
public:
	CubeTextureLoadTaskClass();
#endif

#ifdef OG
	bool Has_Failed() const { return HasFailed; }
	void Set_Fail(bool b) { HasFailed=b; }

#endif
#ifdef ZH
	virtual void			Destroy						(void);
	virtual void			Init							(TextureBaseClass *tc, TaskType type, PriorityType priority);
	virtual void			Deinit						(void);
#endif

#ifdef OG
	// Init the task or put it to a deferred init list if called from outside the main thread
	void Begin_Texture_Load();
	void Begin_Thumbnail_Load();
	void End_Load();		// Deinit must be called before Applying()
#endif
#ifdef ZH
protected:
	virtual bool			Begin_Compressed_Load	(void);
	virtual bool			Begin_Uncompressed_Load	(void);

#endif

#ifdef OG
	void Apply(bool initialize);

#endif
#ifdef ZH
	virtual bool			Load_Compressed_Mipmap	(void);
//	virtual bool			Load_Uncompressed_Mipmap(void);
#endif

#ifdef OG
	TextureLoadTaskClass* Peek_Succ() { return Succ; }
	void Set_Succ(TextureLoadTaskClass* succ);

#endif
#ifdef ZH
	virtual void			Lock_Surfaces				(void);
	virtual void			Unlock_Surfaces			(void);

private:
	unsigned char*			Get_Locked_CubeMap_Surface_Pointer(unsigned int face, unsigned int level);
	unsigned int			Get_Locked_CubeMap_Surface_Pitch(unsigned int face, unsigned int level) const;
#endif

#ifdef OG
	TextureClass* Peek_Texture() { return Texture; }
	IDirect3DTexture8* Peek_D3D_Texture() { return D3DTexture; }
#endif
#ifdef ZH
	IDirect3DCubeTexture8*	Peek_D3D_Cube_Texture(void)				{ return (IDirect3DCubeTexture8*)D3DTexture;		}

#endif

#ifdef OG
	void Set_D3D_Texture(IDirect3DTexture8* texture);

#endif
#ifdef ZH
	unsigned char*			LockedCubeSurfacePtr[6][MIP_LEVELS_MAX];
	unsigned int			LockedCubeSurfacePitch[6][MIP_LEVELS_MAX];
#endif
};

#ifdef OG
#endif

#endif
#ifdef ZH
class VolumeTextureLoadTaskClass : public TextureLoadTaskClass
{
public:
	VolumeTextureLoadTaskClass();

	virtual void			Destroy						(void);
	virtual void			Init							(TextureBaseClass *tc, TaskType type, PriorityType priority);

protected:
	virtual bool			Begin_Compressed_Load	(void);
	virtual bool			Begin_Uncompressed_Load	(void);

	virtual bool			Load_Compressed_Mipmap	(void);
//	virtual bool			Load_Uncompressed_Mipmap(void);

	virtual void			Lock_Surfaces				(void);
	virtual void			Unlock_Surfaces			(void);

private:
	unsigned char*			Get_Locked_Volume_Pointer(unsigned int level);
	unsigned int			Get_Locked_Volume_Row_Pitch(unsigned int level);
	unsigned int			Get_Locked_Volume_Slice_Pitch(unsigned int level);

	IDirect3DVolumeTexture8*	Peek_D3D_Volume_Texture(void)				{ return (IDirect3DVolumeTexture8*)D3DTexture;		}

	unsigned	int			LockedSurfaceSlicePitch[MIP_LEVELS_MAX];
#endif

#ifdef ZH
	unsigned int		Depth;
};
#endif

#ifdef ZH
#endif
#endif


