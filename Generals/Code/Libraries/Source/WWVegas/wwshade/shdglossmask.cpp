/*
**	Command & Conquer Generals Zero Hour(tm)
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
 *                 Project Name : WWSHADE                                                         *
 *                                                                                             *
 *                     $Archive:: wwshade/shdglossmask.cpp                           $*
 *                                                                                             *
 *                  $Org Author:: Kenny_m
 *																																	
 *                      $Author:: Kenny_m
 *																																	
 *							  $Modtime:: 8/01/02 11:39a                                               $*
 *                                                                                             *
 *                    $Revision:: 1                                                          $*
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include <d3dx8math.h>
#include "dx8fvf.h"
#include "dx8wrapper.h"
#include "assetmgr.h"

#include "shdglossmask.h"

#include "editable.h"
#include "shdclassids.h"
#include "shddeffactory.h"
#include "shdinterface.h"
#include "shdhwshader.h"

#include "persistfactory.h"
#include "wwhack.h"

DECLARE_FORCE_LINK(GlossMaskShader);
REGISTER_SHDDEF(ShdGlossMaskDefClass,SHDDEF_CLASSID_GLOSSMASK,"Gloss Mask");


// Save-Load methods for ShdDefClass
enum 
{
	CHUNKID_VARIABLES =			0x16490460,

	VARID_TEXTURE_NAME =					0x00,

	VARID_AMBIENT_COLOR,
	VARID_DIFFUSE_COLOR,
	VARID_SPECULAR_COLOR
};

ShdGlossMaskDefClass::ShdGlossMaskDefClass() 
:	ShdDefClass(SHDDEF_CLASSID_GLOSSMASK),
	Ambient(1,1,1),
	Diffuse(1,1,1),
	Specular(1,1,1)
{
	NAMED_TEXTURE_FILENAME_PARAM(ShdGlossMaskDefClass, TextureName, "Texture Name", "Targa Files", ".tga");

	NAMED_EDITABLE_PARAM(ShdGlossMaskDefClass, ParameterClass::TYPE_COLOR, Ambient, "Ambient");
	NAMED_EDITABLE_PARAM(ShdGlossMaskDefClass, ParameterClass::TYPE_COLOR, Diffuse, "Diffuse");
	NAMED_EDITABLE_PARAM(ShdGlossMaskDefClass, ParameterClass::TYPE_COLOR, Specular, "Specular");
}

ShdGlossMaskDefClass::ShdGlossMaskDefClass(const ShdGlossMaskDefClass& that)
:	ShdDefClass(that),
	Ambient(that.Ambient),
	Diffuse(that.Diffuse),
	Specular(that.Specular)
{
	TextureName=that.TextureName;
}

ShdGlossMaskDefClass::~ShdGlossMaskDefClass()
{
}

bool ShdGlossMaskDefClass::Is_Valid_Config(StringClass &message)
{
	return true;
}

bool ShdGlossMaskDefClass::Save(ChunkSaveClass &csave)
{
	ShdDefClass::Save(csave);

	csave.Begin_Chunk(CHUNKID_VARIABLES);	

		bool retval = true;
	
		// only save the file name
		char fname[_MAX_PATH];

		_splitpath(TextureName,NULL,NULL,fname,NULL);
		strcat(fname,".tga");
		TextureName=fname;

		WRITE_MICRO_CHUNK_WWSTRING(csave, VARID_TEXTURE_NAME, TextureName);

		WRITE_MICRO_CHUNK(csave, VARID_AMBIENT_COLOR, Ambient);
		WRITE_MICRO_CHUNK(csave, VARID_DIFFUSE_COLOR, Diffuse);
		WRITE_MICRO_CHUNK(csave, VARID_SPECULAR_COLOR, Specular);

	csave.End_Chunk();

	return retval;
}

bool ShdGlossMaskDefClass::Load(ChunkLoadClass &cload)
{
	ShdDefClass::Load(cload);

	//	Loop through all the microchunks that define the variables
	while (cload.Open_Chunk()) {
		switch (cload.Cur_Chunk_ID())
		{
		case CHUNKID_VARIABLES:
			while (cload.Open_Micro_Chunk()) 
			{
				switch (cload.Cur_Micro_Chunk_ID()) 
				{
				READ_MICRO_CHUNK_WWSTRING(cload, VARID_TEXTURE_NAME, TextureName);

				READ_MICRO_CHUNK(cload, VARID_AMBIENT_COLOR, Ambient);
				READ_MICRO_CHUNK(cload, VARID_DIFFUSE_COLOR, Diffuse);
				READ_MICRO_CHUNK(cload, VARID_SPECULAR_COLOR, Specular);
				}

				cload.Close_Micro_Chunk();
			}
			break;
		
		default:
			break;
		}

		cload.Close_Chunk();
	}

	return true;
}


void ShdGlossMaskDefClass::Init()
{
	Shd6GlossMaskClass::Init();
}

void ShdGlossMaskDefClass::Shutdown()
{
	Shd6GlossMaskClass::Shutdown();
}

ShdInterfaceClass* ShdGlossMaskDefClass::Create() const
{
	return new Shd6GlossMaskClass(this);
}



Matrix4x4				Shd6GlossMaskClass::View_Projection_Matrix;

Shd6GlossMaskClass::Shd6GlossMaskClass(const ShdDefClass* def)
:	ShdInterfaceClass(def,SHDDEF_CLASSID_GLOSSMASK),
	Texture(NULL)
{
	ShdGlossMaskDefClass* Definition=(ShdGlossMaskDefClass*)def;

	Texture=WW3DAssetManager::Get_Instance()->Get_Texture
	(
		Definition->Get_Texture_Name()
	);

	const Vector3& a=Definition->Get_Ambient();
	Ambient.Set(a.X,a.Y,a.Z,1.0f);

	const Vector3& d=Definition->Get_Diffuse();
	Diffuse.Set(d.X,d.Y,d.Z,1.0f);

	const Vector3& s=Definition->Get_Specular();
	Specular.Set(s.X,s.Y,s.Z,1.0f);

	Material=new D3DMATERIAL8;
	memset(Material,0,sizeof(D3DMATERIAL8));
	Material->Ambient.r=a.X; Material->Ambient.g=a.Y; Material->Ambient.b=a.Z; 
	Material->Diffuse.r=d.X; Material->Diffuse.g=d.Y; Material->Diffuse.b=d.Z; 
	Material->Specular.r=s.X; Material->Specular.g=s.Y; Material->Specular.b=s.Z;
	Material->Power=20;
}

Shd6GlossMaskClass::~Shd6GlossMaskClass()
{
	REF_PTR_RELEASE(Texture);
}

void Shd6GlossMaskClass::Init()
{
}

void Shd6GlossMaskClass::Shutdown()
{
}


/**********************************************************************************************
//! Apply shared states for 1 pass DX6
/*! 7/12/02 3:39p KJM Created
*/
void Shd6GlossMaskClass::Apply_Shared(int pass, RenderInfoClass& rinfo)
{
	// fixed function uses pass through by default
	DX8Wrapper::Set_DX8_Texture_Stage_State(0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_PASSTHRU);

	// set vertex shader
	DX8Wrapper::Set_Vertex_Shader(DX8_FVF_XYZNDUV1);

	// set texture stage settings
	if (DX8Wrapper::Get_Current_Caps()->Support_ModAlphaAddClr())
	{
		DX8Wrapper::Set_DX8_Texture_Stage_State(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		DX8Wrapper::Set_DX8_Texture_Stage_State(0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
		DX8Wrapper::Set_DX8_Texture_Stage_State(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

		DX8Wrapper::Set_DX8_Texture_Stage_State(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		DX8Wrapper::Set_DX8_Texture_Stage_State(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );

		DX8Wrapper::Set_DX8_Texture_Stage_State(1, D3DTSS_COLORARG1, D3DTA_CURRENT );
		DX8Wrapper::Set_DX8_Texture_Stage_State(1, D3DTSS_COLOROP,   D3DTOP_MODULATEALPHA_ADDCOLOR);
		DX8Wrapper::Set_DX8_Texture_Stage_State(1, D3DTSS_COLORARG2, D3DTA_SPECULAR );

		DX8Wrapper::Set_DX8_Texture_Stage_State(1, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
		DX8Wrapper::Set_DX8_Texture_Stage_State(1, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );

		DX8Wrapper::Set_DX8_Texture_Stage_State(2, D3DTSS_COLOROP,   D3DTOP_DISABLE );
		DX8Wrapper::Set_DX8_Texture_Stage_State(2, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
	}
	else
	{
		DX8Wrapper::Set_DX8_Texture_Stage_State(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		DX8Wrapper::Set_DX8_Texture_Stage_State(0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
		DX8Wrapper::Set_DX8_Texture_Stage_State(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

		DX8Wrapper::Set_DX8_Texture_Stage_State(1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
		DX8Wrapper::Set_DX8_Texture_Stage_State(1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
	}


	DX8Wrapper::Set_DX8_Render_State(D3DRS_LIGHTING, TRUE);

	DX8Wrapper::Set_DX8_Render_State(D3DRS_AMBIENTMATERIALSOURCE,D3DMCS_MATERIAL);
	DX8Wrapper::Set_DX8_Render_State(D3DRS_DIFFUSEMATERIALSOURCE,D3DMCS_MATERIAL);
	DX8Wrapper::Set_DX8_Render_State(D3DRS_SPECULARMATERIALSOURCE,D3DMCS_MATERIAL);
	DX8Wrapper::Set_DX8_Render_State(D3DRS_EMISSIVEMATERIALSOURCE,D3DMCS_MATERIAL);

	DX8Wrapper::Set_DX8_Material(Material);

}

//**********************************************************************************************
//! Apply per instance states for 1 pass DX6 
/*! 7/10/02 5:39p KJM Created
*/
void Shd6GlossMaskClass::Apply_Instance(int cur_pass, RenderInfoClass& rinfo)
{
	DX8Wrapper::Set_Texture(0, Texture);

	if (DX8Wrapper::Get_Current_Caps()->Support_ModAlphaAddClr())
	{
		DX8Wrapper::Set_Texture(1, Texture);
	}

	DX8Wrapper::Set_DX8_Material(Material);
}

unsigned Shd6GlossMaskClass::Get_Vertex_Stream_Count() const
{
	return 1;
}

unsigned Shd6GlossMaskClass::Get_Vertex_Size(unsigned stream) const
{
	return sizeof(VertexFormatXYZNDUV1);
}

void Shd6GlossMaskClass::Copy_Vertex_Stream
(
	unsigned stream, 
	void* dest_buffer, 
	const VertexStreamStruct& vss, 
	unsigned vertex_count
)
{
   VertexFormatXYZNDUV1* verts=(VertexFormatXYZNDUV1*)dest_buffer;

	for (unsigned i=0; i<vertex_count; ++i) 
	{
		if (vss.Locations) 
		{
			verts[i].x=vss.Locations[i][0];
			verts[i].y=vss.Locations[i][1];
			verts[i].z=vss.Locations[i][2];
		}
		else 
		{
			verts[i].x=0.0f;
			verts[i].y=0.0f;
			verts[i].z=0.0f;
		}

		if (vss.DiffuseInt) 
		{
			verts[i].diffuse=vss.DiffuseInt[i];
		}
		else 
		{
			verts[i].diffuse=0xffffffff;
		}
	
		if (vss.Normals) 
		{
			verts[i].nx=vss.Normals[i][0];
			verts[i].ny=vss.Normals[i][1];
			verts[i].nz=vss.Normals[i][2];
		}
		else 
		{
			verts[i].nx=0.0f;
			verts[i].ny=0.0f;
			verts[i].nz=0.0f;
		}

		if (vss.UV[0])
		{
			verts[i].u1=vss.UV[0][i].U;
			verts[i].v1=vss.UV[0][i].V;
		}
		else 
		{
			verts[i].u1=0.0f;
			verts[i].v1=0.0f;
		}
	}
}

