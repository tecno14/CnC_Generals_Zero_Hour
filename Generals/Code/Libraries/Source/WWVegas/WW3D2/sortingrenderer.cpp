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
 *                 Project Name : ww3d                                                         *
 *                                                                                             *
 *                     $Archive:: /Commando/Code/ww3d2/sortingrenderer.cpp                    $*
 *                                                                                             *
 *              Original Author:: Greg Hjelstrom                                               *
 *                                                                                             *
 *                       Author : Kenny Mitchell                                               * 
 *                                                                                             * 
 *                     $Modtime:: 06/27/02 1:27p                                              $*
 *                                                                                             *
 *                    $Revision:: 2                                                           $*
 *                                                                                             *
 * 06/26/02 KM Matrix name change to avoid MAX conflicts                                       *
 * 06/27/02 KM Changes to max texture stage caps																*
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#endif
#include "sortingrenderer.h"
#include "dx8vertexbuffer.h"
#include "dx8indexbuffer.h"
#include "dx8wrapper.h"
#include "vertmaterial.h"
#include "texture.h"
#include "d3d8.h"
#include "D3dx8math.h"
#include "statistics.h"
#ifdef ZH
#include <wwprofile.h>
#include <algorithm>
#endif

#ifdef ZH
#ifdef _INTERNAL
// for occasional debugging...
// #pragma optimize("", off)
// #pragma MESSAGE("************************************** WARNING, optimization disabled for debugging purposes")
#endif

#endif
bool SortingRendererClass::_EnableTriangleDraw=true;
#ifdef OG
unsigned DEFAULT_SORTING_POLY_COUNT = 16384;	// (count * 3) must be less than 65536
unsigned DEFAULT_SORTING_VERTEX_COUNT = 32768;	// count must be less than 65536
#endif
#ifdef ZH
static unsigned DEFAULT_SORTING_POLY_COUNT = 16384;	// (count * 3) must be less than 65536
static unsigned DEFAULT_SORTING_VERTEX_COUNT = 32768;	// count must be less than 65536
#endif

void SortingRendererClass::SetMinVertexBufferSize( unsigned val )
{
	DEFAULT_SORTING_VERTEX_COUNT = val;
	DEFAULT_SORTING_POLY_COUNT = val/2;	//typically have 2:1 vertex:triangle ratio.
}

struct ShortVectorIStruct
{
	unsigned short i;
	unsigned short j;
	unsigned short k;
#ifdef OG

	ShortVectorIStruct(unsigned short i_,unsigned short j_,unsigned short k_) : i(i_),j(j_),k(k_) {}
	ShortVectorIStruct() {}
#endif
};

struct TempIndexStruct
{
	ShortVectorIStruct tri;
	unsigned short idx;
#ifdef OG

	TempIndexStruct() {}
	TempIndexStruct(const ShortVectorIStruct& tri_, unsigned short idx_)
		:
		tri(tri_),
		idx(idx_)
	{
	}
#endif
#ifdef ZH
	float z;

#endif
};

#ifdef ZH
bool operator <(const TempIndexStruct &l, const TempIndexStruct &r) { return l.z < r.z; }
bool operator <=(const TempIndexStruct &l, const TempIndexStruct &r) { return l.z <= r.z; }
bool operator >(const TempIndexStruct &l, const TempIndexStruct &r) { return l.z > r.z; }
bool operator >=(const TempIndexStruct &l, const TempIndexStruct &r) { return l.z >= r.z; }
bool operator ==(const TempIndexStruct &l, const TempIndexStruct &r) { return l.z == r.z; }
#endif
// ----------------------------------------------------------------------------
#ifdef OG
//
// InsertionSort (T* array, K *keys, int l, int r)
// Performs insertion sort on array 'array' elements [l-r]. Uses values from array
// 'keys' as sort keys.
//
// ----------------------------------------------------------------------------

template <class T, class K>
void InsertionSort (
	T* array,		// array to sort
	K* keys,			// sort keys
	int l,			//	first item
	int r)			//	last item
#endif
#ifdef ZH
static
void InsertionSort(TempIndexStruct *begin, TempIndexStruct *end)

#endif
{
#ifdef OG
	for (int i = l+1; i < r; i++) {
		K v=keys[i];
		T tv=array[i];
		int j=i;

		while (keys[j-1] > v) {
			keys[j]=keys[j-1];
			array[j]=array[j-1];
			j--;
			if (j == l) break;
		};
		keys[j]=v;
		array[j]=tv;
#endif
#ifdef ZH
	for (TempIndexStruct *iter = begin + 1; iter < end; ++iter) {
		TempIndexStruct val = iter[0];
		TempIndexStruct *insert = iter;
		while (insert != begin && insert[-1] > val) {
			insert[0] = insert[-1];
			insert -= 1;
		}
		insert[0] = val;

#endif
	}
}

// ----------------------------------------------------------------------------
#ifdef OG
//
//	QuickSort (T* array, K* a, int l, int r)
//
//	Performs quicksort on array 'array'. Uses values from array 'keys' as sort keys.
//
// Once the length of the array to be sorted is less than 8, the routine calls
// InsertionSort() to perform the actual sorting work.
//
// ----------------------------------------------------------------------------

template <class T, class K>
void QuickSort (
	T* array,		//	array to sort
	K* keys,			// sort keys
	int l,			// first element
	int r)			// last element
#endif
#ifdef ZH
static
void Sort(TempIndexStruct *begin, TempIndexStruct *end)

#endif
{
#ifdef OG
	if (r-l <= 8) {
		InsertionSort(array,keys,l,r+1);
		return;

#endif
#ifdef ZH
	const int diff = end - begin;
	if (diff <= 16) {
		// Insertion sort has less overhead for small arrays
		InsertionSort(begin, end);
	} else {
		// Choose the median of begin, mid, and (end - 1) as the partitioning element.
		// Rearrange so that *(begin + 1) <= *begin <= *(end - 1).  These will be guard
		// elements.
		TempIndexStruct *mid = begin + diff/2;
		std::swap(mid[0], begin[1]);
		if (begin[1] > end[-1]) {
			std::swap(begin[1], end[-1]);
#endif
	}
#ifdef ZH
		if (begin[0] > end[-1]) {
			std::swap(begin[0], end[-1]);
		}																// end[-1] has the largest element
		if (begin[1] > begin[0]) {
			std::swap(begin[1], begin[0]);
		}																// begin[0] has the middle element and begin[1] has the smallest element
#endif

#ifdef OG
	K t;
	K v=keys[r];
	T ttemp;
	int i=l-1;
	int j=r;

	do {
		do { i++; } while (i<r && keys[i]<v);
		do { j--; } while (j>0 && keys[j]>v);
		
		WWASSERT(j>=0);
		WWASSERT(i<=r);

		ttemp=array[i]; array[i]=array[j]; array[j]=ttemp;
		t=keys[i]; keys[i]=keys[j]; keys[j]=t;
	} while (j>i);

	array[j]=array[i];
	array[i]=array[r];
	array[r]=ttemp;
	keys[j]=keys[i];
	keys[i]=keys[r];
	keys[r]=t;
	
	if (i-1>l) QuickSort(array,keys,l,i-1);
	if (r>i+1) QuickSort(array,keys,i+1,r);
#endif
#ifdef ZH
		// *begin is now the partitioning element
		TempIndexStruct *begin1 = begin + 1;	// TODO: Temp fix until I find out who is passing me NaN
		TempIndexStruct *end1 = end - 1;			// TODO: Temp fix until I find out who is passing me NaN
		TempIndexStruct *left = begin + 1;
		TempIndexStruct *right = end - 1;
		for (;;) {
#if 0		// TODO: Temp fix until I find out who is passing me NaN.
			do ++left; while (left[0] < begin[0]);		// Scan up to find element >= than partition
			do --right; while (right[0] > begin[0]);	// Scan down to find element <= than partition
#else
			do ++left; while (left < end1 && left[0] < begin[0]);		// Scan up to find element >= than partition
			do --right; while (right > begin1 && right[0] > begin[0]);	// Scan down to find element <= than partition
#endif
			if (right < left) break;									// Pointers crossed.  Partitioning completed.
			std::swap(left[0], right[0]);							// Exchange elements.

#endif
}
#ifdef ZH
		std::swap(begin[0], right[0]);							// Insert partition element
#endif

#ifdef OG
// ----------------------------------------------------------------------------
//
// Sorts and array. Uses values from array 'keys' as sort keys.
//
// ----------------------------------------------------------------------------

template <class T, class K>
void Sort (
	T* array,		// array to sort
	K *keys,	// sort keys
	int count)		// array element count
{
	bool do_insertion = false;

	if (count<=1) return;									// only one element.. return..

	int c=0;														// count number of rise pairs
	int i;
	for (i = 1; i < count; i++)
	if (keys[i] >= keys[i-1]) c++;

	if (c+1 == count) return;								// array already sorted
	if (c<50) do_insertion=true;							// array smaller than 50 should use insertion sort

	if (c<count/3) {											// if array is not rising
		T tmp;
		K tval;

		for (i=0;i<count/2;i++) {
			int neg = count-1-i;

			tmp=array[i]; array[i]=array[neg]; array[neg]=tmp;
			tval=keys[i]; keys[i] = keys[neg]; keys[neg]=tval;
#endif
#ifdef ZH
		// Sort the smaller subarray first then the larger
		if (right - begin > end - (right + 1)) {
			Sort(right + 1, end);
			Sort(begin, right);
		} else {
			Sort(begin, right);
			Sort(right + 1, end);

#endif
		}
#ifdef OG

		if (!c) return;

		do_insertion = true;
#endif
	}
#ifdef OG
	if (do_insertion) InsertionSort(array,keys,0,count);
	else QuickSort(array,keys,0,count-1);			// quick sort
#endif
}

// ----------------------------------------------------------------------------

class SortingNodeStruct : public DLNodeClass<SortingNodeStruct>
{
	W3DMPO_GLUE(SortingNodeStruct)

public:
	RenderStateStruct sorting_state;

	SphereClass bounding_sphere;

	Vector3 transformed_center;
	unsigned short start_index;			// First index used in the ib
	unsigned short polygon_count;			// Polygon count to process (3 indices = one polygon)
	unsigned short min_vertex_index;		// First index used in the vb
	unsigned short vertex_count;			// Number of vertices used in vb
};

static DLListClass<SortingNodeStruct> sorted_list;
static DLListClass<SortingNodeStruct> clean_list;
static unsigned total_sorting_vertices;

static SortingNodeStruct* Get_Sorting_Struct()
{

	SortingNodeStruct* state=clean_list.Head();
	if (state) {
		state->Remove();
		return state;
	}
	state=W3DNEW SortingNodeStruct();
	return state;
}

// ----------------------------------------------------------------------------
//
// Temporary arrays for the sorting system
//
// ----------------------------------------------------------------------------

#ifdef OG
static float* vertex_z_array;
static float* polygon_z_array;
static unsigned * node_id_array;
static unsigned * sorted_node_id_array;
static ShortVectorIStruct* polygon_index_array;
static unsigned vertex_z_array_count;
static unsigned polygon_z_array_count;
static unsigned node_id_array_count;
static unsigned sorted_node_id_array_count;
static unsigned polygon_index_array_count;
TempIndexStruct* temp_index_array;
unsigned temp_index_array_count;
#endif
#ifdef ZH
static TempIndexStruct* temp_index_array;
static unsigned temp_index_array_count;

#endif

static TempIndexStruct* Get_Temp_Index_Array(unsigned count)
{
	if (count < DEFAULT_SORTING_POLY_COUNT)
		count = DEFAULT_SORTING_POLY_COUNT;
	if (count>temp_index_array_count) {
		delete[] temp_index_array;
		temp_index_array=W3DNEWARRAY TempIndexStruct[count];
		temp_index_array_count=count;
	}
	return temp_index_array;
#ifdef OG
}

static float* Get_Vertex_Z_Array(unsigned count)
{
	if (count < DEFAULT_SORTING_VERTEX_COUNT)
		count = DEFAULT_SORTING_VERTEX_COUNT;
	if (count>vertex_z_array_count) {
		delete[] vertex_z_array;
		vertex_z_array=W3DNEWARRAY float[count];
		vertex_z_array_count=count;
	}
	return vertex_z_array;
}

static float* Get_Polygon_Z_Array(unsigned count)
{
	if (count < DEFAULT_SORTING_POLY_COUNT)
		count = DEFAULT_SORTING_POLY_COUNT;
	if (count>polygon_z_array_count) {
		delete[] polygon_z_array;
		polygon_z_array=W3DNEWARRAY float[count];
		polygon_z_array_count=count;
	}
	return polygon_z_array;
}

static unsigned * Get_Node_Id_Array(unsigned count)
{
	if (count < DEFAULT_SORTING_POLY_COUNT)
		count = DEFAULT_SORTING_POLY_COUNT;
	if (count>node_id_array_count) {
		delete[] node_id_array;
		node_id_array=W3DNEWARRAY unsigned[count];
		node_id_array_count=count;
#endif
	}
#ifdef OG
	return node_id_array;
}
#endif

#ifdef OG
static unsigned * Get_Sorted_Node_Id_Array(unsigned count)
{
	if (count < DEFAULT_SORTING_POLY_COUNT)
		count = DEFAULT_SORTING_POLY_COUNT;
	if (count>sorted_node_id_array_count) {
		delete[] sorted_node_id_array;
		sorted_node_id_array=W3DNEWARRAY unsigned[count];
		sorted_node_id_array_count=count;
	}
	return sorted_node_id_array;
}

static ShortVectorIStruct* Get_Polygon_Index_Array(unsigned count)
{
	if (count < DEFAULT_SORTING_POLY_COUNT)
		count = DEFAULT_SORTING_POLY_COUNT;
	if (count>polygon_index_array_count) {
		delete[] polygon_index_array;
		polygon_index_array=W3DNEWARRAY ShortVectorIStruct[count];
		polygon_index_array_count=count;
	}
	return polygon_index_array;
}

#endif
// ----------------------------------------------------------------------------
//
// Insert triangles to the sorting system.
//
// ----------------------------------------------------------------------------

void SortingRendererClass::Insert_Triangles(
	const SphereClass& bounding_sphere,
	unsigned short start_index, 
	unsigned short polygon_count,
	unsigned short min_vertex_index,
	unsigned short vertex_count)
{
	if (!WW3D::Is_Sorting_Enabled()) {
		DX8Wrapper::Draw_Triangles(start_index,polygon_count,min_vertex_index,vertex_count);
		return;
	}

#ifdef ZH
	SNAPSHOT_SAY(("SortingRenderer::Insert(start_i: %d, polygons : %d, min_vi: %d, vertex_count: %d)\n",
		start_index,polygon_count,min_vertex_index,vertex_count));
#endif

	DX8_RECORD_SORTING_RENDER(polygon_count,vertex_count);

	SortingNodeStruct* state=Get_Sorting_Struct();

	DX8Wrapper::Get_Render_State(state->sorting_state);

 	WWASSERT(
		((state->sorting_state.index_buffer_type==BUFFER_TYPE_SORTING || state->sorting_state.index_buffer_type==BUFFER_TYPE_DYNAMIC_SORTING) &&
#ifdef OG
		(state->sorting_state.vertex_buffer_type==BUFFER_TYPE_SORTING || state->sorting_state.vertex_buffer_type==BUFFER_TYPE_DYNAMIC_SORTING)));
#endif
#ifdef ZH
		(state->sorting_state.vertex_buffer_types[0]==BUFFER_TYPE_SORTING || state->sorting_state.vertex_buffer_types[0]==BUFFER_TYPE_DYNAMIC_SORTING)));
#endif


	state->bounding_sphere=bounding_sphere;
	state->start_index=start_index;
	state->polygon_count=polygon_count;
	state->min_vertex_index=min_vertex_index;
	state->vertex_count=vertex_count;

#ifdef OG
	SortingVertexBufferClass* vertex_buffer=static_cast<SortingVertexBufferClass*>(state->sorting_state.vertex_buffer);
#endif
#ifdef ZH
	SortingVertexBufferClass* vertex_buffer=static_cast<SortingVertexBufferClass*>(state->sorting_state.vertex_buffers[0]);
#endif
	WWASSERT(vertex_buffer);
	WWASSERT(state->vertex_count<=vertex_buffer->Get_Vertex_Count());

	D3DXMATRIX mtx=(D3DXMATRIX&)state->sorting_state.world*(D3DXMATRIX&)state->sorting_state.view;
	D3DXVECTOR3 vec=(D3DXVECTOR3&)state->bounding_sphere.Center;
	D3DXVECTOR4 transformed_vec;
	D3DXVec3Transform(
		&transformed_vec,
		&vec,
		&mtx); 
	state->transformed_center=Vector3(transformed_vec[0],transformed_vec[1],transformed_vec[2]);

	
	/// @todo lorenzen sez use a bucket sort here... and stop copying so much data so many times

	SortingNodeStruct* node=sorted_list.Head();
	while (node) {
		if (state->transformed_center.Z>node->transformed_center.Z) {
			if (sorted_list.Head()==sorted_list.Tail())
				sorted_list.Add_Head(state);
			else
				state->Insert_Before(node);
			break;
		}
		node=node->Succ();
	}
	if (!node) sorted_list.Add_Tail(state);

#ifdef WWDEBUG
	unsigned short* indices=NULL;
	SortingIndexBufferClass* index_buffer=static_cast<SortingIndexBufferClass*>(state->sorting_state.index_buffer);
	WWASSERT(index_buffer);
	indices=index_buffer->index_buffer;
	WWASSERT(indices);
	indices+=state->start_index;
	indices+=state->sorting_state.iba_offset;

	for (int i=0;i<state->polygon_count;++i) {
		unsigned short idx1=indices[i*3]-state->min_vertex_index;
		unsigned short idx2=indices[i*3+1]-state->min_vertex_index;
		unsigned short idx3=indices[i*3+2]-state->min_vertex_index;
		WWASSERT(idx1<state->vertex_count);
		WWASSERT(idx2<state->vertex_count);
		WWASSERT(idx3<state->vertex_count);
	}
#ifdef OG
#endif
#endif
#ifdef ZH
#endif // WWDEBUG
#endif
}

// ----------------------------------------------------------------------------
//
// Insert triangles to the sorting system, with no bounding information.
//
// ----------------------------------------------------------------------------

void SortingRendererClass::Insert_Triangles(
	unsigned short start_index, 
	unsigned short polygon_count,
	unsigned short min_vertex_index,
	unsigned short vertex_count)
{
	SphereClass sphere(Vector3(0.0f,0.0f,0.0f),0.0f);
	Insert_Triangles(sphere,start_index,polygon_count,min_vertex_index,vertex_count);
}

// ----------------------------------------------------------------------------
//
// Flush all sorting polygons.
//
// ----------------------------------------------------------------------------

void Release_Refs(SortingNodeStruct* state)
{
#ifdef OG
	REF_PTR_RELEASE(state->sorting_state.vertex_buffer);

#endif
#ifdef ZH
	int i;
	for (i=0;i<MAX_VERTEX_STREAMS;++i) {
		REF_PTR_RELEASE(state->sorting_state.vertex_buffers[i]);
	}
#endif
	REF_PTR_RELEASE(state->sorting_state.index_buffer);
	REF_PTR_RELEASE(state->sorting_state.material);
#ifdef OG
	for (unsigned i=0;i<MAX_TEXTURE_STAGES;++i) {

#endif
#ifdef ZH
	for (i=0;i<DX8Wrapper::Get_Current_Caps()->Get_Max_Textures_Per_Pass();++i) 
	{
#endif
		REF_PTR_RELEASE(state->sorting_state.Textures[i]);
	}
}

static unsigned overlapping_node_count;
static unsigned overlapping_polygon_count;
static unsigned overlapping_vertex_count;
#ifdef OG
const unsigned MAX_OVERLAPPING_NODES=4096;
#endif
#ifdef ZH
static const unsigned MAX_OVERLAPPING_NODES=4096;
#endif
static SortingNodeStruct* overlapping_nodes[MAX_OVERLAPPING_NODES];

// ----------------------------------------------------------------------------

void SortingRendererClass::Insert_To_Sorting_Pool(SortingNodeStruct* state)
{
	if (overlapping_node_count>=MAX_OVERLAPPING_NODES) {
		Release_Refs(state);
		WWASSERT(0);
		return;
	}

	overlapping_nodes[overlapping_node_count]=state;
	overlapping_vertex_count+=state->vertex_count;
	overlapping_polygon_count+=state->polygon_count;
	overlapping_node_count++;
}

// ----------------------------------------------------------------------------
#ifdef ZH
//static unsigned prevLight = 0xffffffff;
#endif

static void Apply_Render_State(RenderStateStruct& render_state)
{
#ifdef OG
/*	state->sorting_state.shader.Apply();
*/

#endif
	DX8Wrapper::Set_Shader(render_state.shader);

#ifdef OG
/*	if (render_state.material) render_state.material->Apply();
*/
#endif
	DX8Wrapper::Set_Material(render_state.material);

#ifdef OG
/*	if (render_state.Textures[2]) render_state.Textures[2]->Apply();
	if (render_state.Textures[3]) render_state.Textures[3]->Apply();
	if (render_state.Textures[4]) render_state.Textures[4]->Apply();
	if (render_state.Textures[5]) render_state.Textures[5]->Apply();
	if (render_state.Textures[6]) render_state.Textures[6]->Apply();
	if (render_state.Textures[7]) render_state.Textures[7]->Apply();
*/
	for (unsigned i=0;i<MAX_TEXTURE_STAGES;++i) {
#endif
#ifdef ZH
	for (int i=0;i<DX8Wrapper::Get_Current_Caps()->Get_Max_Textures_Per_Pass();++i) 
	{

#endif
		DX8Wrapper::Set_Texture(i,render_state.Textures[i]);
	}

	DX8Wrapper::_Set_DX8_Transform(D3DTS_WORLD,render_state.world);
	DX8Wrapper::_Set_DX8_Transform(D3DTS_VIEW,render_state.view);


	if (!render_state.material->Get_Lighting())
#ifdef OG
		return;	//no point changing lights if they are ignored.

#endif
#ifdef ZH
    return;
  //prevLight = render_state.lightsHash;

	if (render_state.LightEnable[0]) 
  {
#endif

#ifdef OG
	if (render_state.LightEnable[0]) {
#endif
		DX8Wrapper::Set_DX8_Light(0,&render_state.Lights[0]);
#ifdef OG
		if (render_state.LightEnable[1]) {

#endif
#ifdef ZH
		if (render_state.LightEnable[1]) 
    {
#endif
			DX8Wrapper::Set_DX8_Light(1,&render_state.Lights[1]);
#ifdef OG
			if (render_state.LightEnable[2]) {

#endif
#ifdef ZH
			if (render_state.LightEnable[2]) 
      {
#endif
				DX8Wrapper::Set_DX8_Light(2,&render_state.Lights[2]);
#ifdef OG
				if (render_state.LightEnable[3]) {
#endif
#ifdef ZH
				if (render_state.LightEnable[3]) 
#endif
					DX8Wrapper::Set_DX8_Light(3,&render_state.Lights[3]);
#ifdef OG
				}
				else {
#endif
#ifdef ZH
				else 

#endif
					DX8Wrapper::Set_DX8_Light(3,NULL);
#ifdef OG
				}
#endif
			}
#ifdef OG
			else {
#endif
#ifdef ZH
			else 
#endif
				DX8Wrapper::Set_DX8_Light(2,NULL);
			}
#ifdef OG
		}
		else {
#endif
#ifdef ZH
		else 

#endif
			DX8Wrapper::Set_DX8_Light(1,NULL);
		}
#ifdef OG
	}
	else {
#endif
#ifdef ZH
	else 

#endif
		DX8Wrapper::Set_DX8_Light(0,NULL);
#ifdef OG
	}
#endif

#ifdef OG
//	Matrix4 mtx;
//	mtx=render_state.world.Transpose();
//	DX8Wrapper::Set_Transform(D3DTS_WORLD,mtx);
//	mtx=render_state.view.Transpose();
//	DX8Wrapper::Set_Transform(D3DTS_VIEW,mtx);
#endif

}

// ----------------------------------------------------------------------------

void SortingRendererClass::Flush_Sorting_Pool()
{
	if (!overlapping_node_count) return;

	SNAPSHOT_SAY(("SortingSystem - Flush \n"));

#ifdef OG
	unsigned node_id;
#endif
	// Fill dynamic index buffer with sorting index buffer vertices
#ifdef OG
	unsigned * node_id_array=Get_Node_Id_Array(overlapping_polygon_count);
	float* polygon_z_array=Get_Polygon_Z_Array(overlapping_polygon_count);
	ShortVectorIStruct* polygon_idx_array=(ShortVectorIStruct*)Get_Polygon_Index_Array(overlapping_polygon_count);
#endif
#ifdef ZH
	TempIndexStruct* tis=Get_Temp_Index_Array(overlapping_polygon_count);

#endif

	unsigned vertexAllocCount = overlapping_vertex_count;
	if (DynamicVBAccessClass::Get_Default_Vertex_Count() < DEFAULT_SORTING_VERTEX_COUNT)
		vertexAllocCount = DEFAULT_SORTING_VERTEX_COUNT;	//make sure that we force the DX8 dynamic vertex buffer to maximum size
	if (overlapping_vertex_count > vertexAllocCount)
		vertexAllocCount = overlapping_vertex_count;
	WWASSERT(DEFAULT_SORTING_VERTEX_COUNT == 1 || vertexAllocCount <= DEFAULT_SORTING_VERTEX_COUNT);
	DynamicVBAccessClass dyn_vb_access(BUFFER_TYPE_DYNAMIC_DX8,dynamic_fvf_type,vertexAllocCount/*overlapping_vertex_count*/);
	{
		DynamicVBAccessClass::WriteLockClass lock(&dyn_vb_access);
#ifdef OG
		VertexFormatXYZNDUV2* dest_verts=lock.Get_Formatted_Vertex_Array();
#endif
#ifdef ZH
		VertexFormatXYZNDUV2* dest_verts=(VertexFormatXYZNDUV2 *)lock.Get_Formatted_Vertex_Array();
#endif

		unsigned polygon_array_offset=0;
		unsigned vertex_array_offset=0;
#ifdef OG
		for (node_id=0;node_id<overlapping_node_count;++node_id) {
#endif
#ifdef ZH
		for (unsigned node_id=0;node_id<overlapping_node_count;++node_id) {
#endif
			SortingNodeStruct* state=overlapping_nodes[node_id];
#ifdef OG
			float* vertex_z_array=Get_Vertex_Z_Array(state->vertex_count);

#endif
			VertexFormatXYZNDUV2* src_verts=NULL;
#ifdef OG
			SortingVertexBufferClass* vertex_buffer=static_cast<SortingVertexBufferClass*>(state->sorting_state.vertex_buffer);
#endif
#ifdef ZH
			SortingVertexBufferClass* vertex_buffer=static_cast<SortingVertexBufferClass*>(state->sorting_state.vertex_buffers[0]);
#endif
			WWASSERT(vertex_buffer);
			src_verts=vertex_buffer->VertexBuffer;
			WWASSERT(src_verts);
			src_verts+=state->sorting_state.vba_offset;
			src_verts+=state->sorting_state.index_base_offset;
			src_verts+=state->min_vertex_index;
#ifdef ZH

			// If you have a crash in here and "dest_verts" points to illegal memory area,
			// it is because D3D is in illegal state, and the only known cure is rebooting.
			// This illegal state is usually caused by Quake3-engine powered games such as MOHAA.
			memcpy(dest_verts, src_verts, sizeof(VertexFormatXYZNDUV2)*state->vertex_count);
			dest_verts += state->vertex_count;
#endif

			D3DXMATRIX d3d_mtx=(D3DXMATRIX&)state->sorting_state.world*(D3DXMATRIX&)state->sorting_state.view;
#ifdef OG
			D3DXMatrixTranspose(&d3d_mtx,&d3d_mtx);
			const Matrix4& mtx=(const Matrix4&)d3d_mtx;
			for (unsigned i=0;i<state->vertex_count;++i,++src_verts) {
				vertex_z_array[i] = (mtx[2][0] * src_verts->x + mtx[2][1] * src_verts->y + mtx[2][2] * src_verts->z + mtx[2][3]);
				*dest_verts++=*src_verts;
			}
#endif
#ifdef ZH
			const Matrix4x4& mtx=(const Matrix4x4&)d3d_mtx;

#endif

			unsigned short* indices=NULL;
			SortingIndexBufferClass* index_buffer=static_cast<SortingIndexBufferClass*>(state->sorting_state.index_buffer);
			WWASSERT(index_buffer);
			indices=index_buffer->index_buffer;
			WWASSERT(indices);
			indices+=state->start_index;
			indices+=state->sorting_state.iba_offset;

#ifdef OG
			for (i=0;i<state->polygon_count;++i) {

#endif
#ifdef ZH
			if (mtx[0][2] == 0.0f && mtx[1][2] == 0.0f && mtx[3][2] == 0.0f && mtx[2][2] == 1.0f) {
				// The common case for particle systems.
				for (int i=0;i<state->polygon_count;++i) {
					unsigned short idx1=indices[i*3]-state->min_vertex_index;
					unsigned short idx2=indices[i*3+1]-state->min_vertex_index;
					unsigned short idx3=indices[i*3+2]-state->min_vertex_index;
					WWASSERT(idx1<state->vertex_count);
					WWASSERT(idx2<state->vertex_count);
					WWASSERT(idx3<state->vertex_count);
					const VertexFormatXYZNDUV2 *v1 = src_verts + idx1;
					const VertexFormatXYZNDUV2 *v2 = src_verts + idx2;
					const VertexFormatXYZNDUV2 *v3 = src_verts + idx3;
					unsigned array_index=i+polygon_array_offset;
					WWASSERT(array_index<overlapping_polygon_count);
					TempIndexStruct *tis_ptr = tis + array_index;
					tis_ptr->tri.i = idx1 + vertex_array_offset;
					tis_ptr->tri.j = idx2 + vertex_array_offset;
					tis_ptr->tri.k = idx3 + vertex_array_offset;
					tis_ptr->idx = node_id;
					tis_ptr->z = (v1->z + v2->z + v3->z)/3.0f;
					DEBUG_ASSERTCRASH((! _isnan(tis_ptr->z) && _finite(tis_ptr->z)), ("Triangle has invalid center"));
				}
			} else {
				for (int i=0;i<state->polygon_count;++i) {
#endif
				unsigned short idx1=indices[i*3]-state->min_vertex_index;
				unsigned short idx2=indices[i*3+1]-state->min_vertex_index;
				unsigned short idx3=indices[i*3+2]-state->min_vertex_index;
				WWASSERT(idx1<state->vertex_count);
				WWASSERT(idx2<state->vertex_count);
				WWASSERT(idx3<state->vertex_count);
#ifdef OG
				float z1=vertex_z_array[idx1];
				float z2=vertex_z_array[idx2];
				float z3=vertex_z_array[idx3];
				float z=(z1+z2+z3)/3.0f;
#endif
#ifdef ZH
					const VertexFormatXYZNDUV2 *v1 = src_verts + idx1;
					const VertexFormatXYZNDUV2 *v2 = src_verts + idx2;
					const VertexFormatXYZNDUV2 *v3 = src_verts + idx3;

#endif
				unsigned array_index=i+polygon_array_offset;
				WWASSERT(array_index<overlapping_polygon_count);
#ifdef OG
				polygon_z_array[array_index]=z;
				node_id_array[array_index]=node_id;
				polygon_idx_array[array_index]=ShortVectorIStruct(
					idx1+vertex_array_offset,
					idx2+vertex_array_offset,
					idx3+vertex_array_offset);

#endif
#ifdef ZH
					TempIndexStruct *tis_ptr = tis + array_index;
					tis_ptr->tri.i = idx1 + vertex_array_offset;
					tis_ptr->tri.j = idx2 + vertex_array_offset;
					tis_ptr->tri.k = idx3 + vertex_array_offset;
					tis_ptr->idx = node_id;
					tis_ptr->z = (mtx[0][2]*(v1->x + v2->x + v3->x) +
												mtx[1][2]*(v1->y + v2->y + v3->y) +
												mtx[2][2]*(v1->z + v2->z + v3->z))/3.0f + mtx[3][2];
					DEBUG_ASSERTCRASH((! _isnan(tis_ptr->z) && _finite(tis_ptr->z)), ("Triangle has invalid center"));
				}
#endif
			}

			state->min_vertex_index=vertex_array_offset;

			polygon_array_offset+=state->polygon_count;
			vertex_array_offset+=state->vertex_count;
		}
	}

#ifdef OG
	TempIndexStruct* tis=Get_Temp_Index_Array(overlapping_polygon_count);
	for (unsigned a=0;a<overlapping_polygon_count;++a) {
		tis[a]=TempIndexStruct(polygon_idx_array[a],node_id_array[a]);
	}
	Sort<TempIndexStruct,float>(tis,polygon_z_array,overlapping_polygon_count);
#endif
#ifdef ZH
	Sort(tis, tis + overlapping_polygon_count);

#endif

/*	///@todo: Add code to break up rendering into multiple index buffer fills to allow more than 65536/3 triangles.  -MW
	int total_overlapping_polygon_count = overlapping_polygon_count;
	while (  > 0)
	{
		if ((total_overlapping_polygon_count*3) > 65535)
		{	//overflowed the index buffer, must break into multiple batches
			overlapping_polygon_count = 65535/3;
		}
		else
			overlapping_polygon_count = total_overlapping_polygon_count;

		//insert rendering code here!!

		total_overlapping_polygon_count -= overlapping_polygon_count;
	}
*/
	unsigned polygonAllocCount = overlapping_polygon_count;
	if ((unsigned)(DynamicIBAccessClass::Get_Default_Index_Count()/3) < DEFAULT_SORTING_POLY_COUNT)
		polygonAllocCount = DEFAULT_SORTING_POLY_COUNT;	//make sure that we force the DX8 index buffer to maximum size
	if (overlapping_polygon_count > polygonAllocCount)
		polygonAllocCount = overlapping_polygon_count;
	WWASSERT(DEFAULT_SORTING_POLY_COUNT <= 1 || polygonAllocCount <= DEFAULT_SORTING_POLY_COUNT);

	DynamicIBAccessClass dyn_ib_access(BUFFER_TYPE_DYNAMIC_DX8,polygonAllocCount*3);
	{
		DynamicIBAccessClass::WriteLockClass lock(&dyn_ib_access);
		ShortVectorIStruct* sorted_polygon_index_array=(ShortVectorIStruct*)lock.Get_Index_Array();

#ifdef OG
		for (a=0;a<overlapping_polygon_count;++a) {

#endif
#ifdef ZH
		try {
		for (unsigned a=0;a<overlapping_polygon_count;++a) {
#endif
			sorted_polygon_index_array[a]=tis[a].tri;
#ifdef ZH
		}
		IndexBufferExceptionFunc();
		} catch(...) {
			IndexBufferExceptionFunc();
#endif
		}
	}

	// Set index buffer and render!

	DX8Wrapper::Set_Index_Buffer(dyn_ib_access,0); // Override with this buffer (do something to prevent need for this!)
	DX8Wrapper::Set_Vertex_Buffer(dyn_vb_access); // Override with this buffer (do something to prevent need for this!)

	DX8Wrapper::Apply_Render_State_Changes();

	unsigned count_to_render=1;
	unsigned start_index=0;
#ifdef OG
	node_id=tis[0].idx;
#endif
#ifdef ZH
	unsigned node_id=tis[0].idx;
#endif
	for (unsigned i=1;i<overlapping_polygon_count;++i) {
		if (node_id!=tis[i].idx) {
			SortingNodeStruct* state=overlapping_nodes[node_id];
			Apply_Render_State(state->sorting_state);

			DX8Wrapper::Draw_Triangles(
				start_index*3,
				count_to_render,
				state->min_vertex_index,
				state->vertex_count);

			count_to_render=0;
			start_index=i;
			node_id=tis[i].idx;
		}
		count_to_render++;	//keep track of number of polygons of same kind
	}

	// Render any remaining polygons...
	if (count_to_render) {
		SortingNodeStruct* state=overlapping_nodes[node_id];
		Apply_Render_State(state->sorting_state);

		DX8Wrapper::Draw_Triangles(
			start_index*3,
			count_to_render,
			state->min_vertex_index,
			state->vertex_count);
	}

	// Release all references and return nodes back to the clean list for the frame...
	for (node_id=0;node_id<overlapping_node_count;++node_id) {
		SortingNodeStruct* state=overlapping_nodes[node_id];
		Release_Refs(state);
		clean_list.Add_Head(state);
	}
	overlapping_node_count=0;
	overlapping_polygon_count=0;
	overlapping_vertex_count=0;

	SNAPSHOT_SAY(("SortingSystem - Done flushing\n"));

}

// ----------------------------------------------------------------------------

void SortingRendererClass::Flush()
{
#ifdef OG
	Matrix4 old_view;
	Matrix4 old_world;

#endif
#ifdef ZH
	WWPROFILE("SortingRenderer::Flush");
	Matrix4x4 old_view;
	Matrix4x4 old_world;
#endif
	DX8Wrapper::Get_Transform(D3DTS_VIEW,old_view);
	DX8Wrapper::Get_Transform(D3DTS_WORLD,old_world);

	while (SortingNodeStruct* state=sorted_list.Head()) {
		state->Remove();
		
		if ((state->sorting_state.index_buffer_type==BUFFER_TYPE_SORTING || state->sorting_state.index_buffer_type==BUFFER_TYPE_DYNAMIC_SORTING) &&
#ifdef OG
			(state->sorting_state.vertex_buffer_type==BUFFER_TYPE_SORTING || state->sorting_state.vertex_buffer_type==BUFFER_TYPE_DYNAMIC_SORTING)) {
#endif
#ifdef ZH
			(state->sorting_state.vertex_buffer_types[0]==BUFFER_TYPE_SORTING || state->sorting_state.vertex_buffer_types[0]==BUFFER_TYPE_DYNAMIC_SORTING)) {
#endif
			Insert_To_Sorting_Pool(state);
		}
		else {
			DX8Wrapper::Set_Render_State(state->sorting_state);
			DX8Wrapper::Draw_Triangles(state->start_index,state->polygon_count,state->min_vertex_index,state->vertex_count);
			DX8Wrapper::Release_Render_State();
			Release_Refs(state);
			clean_list.Add_Head(state);
		}
	}

#ifdef ZH
	bool old_enable=DX8Wrapper::_Is_Triangle_Draw_Enabled();
	DX8Wrapper::_Enable_Triangle_Draw(_EnableTriangleDraw);
#endif
	Flush_Sorting_Pool();
#ifdef ZH
	DX8Wrapper::_Enable_Triangle_Draw(old_enable);
#endif

	DX8Wrapper::Set_Index_Buffer(0,0);
	DX8Wrapper::Set_Vertex_Buffer(0);
	total_sorting_vertices=0;

	DynamicIBAccessClass::_Reset(false);
	DynamicVBAccessClass::_Reset(false);


	DX8Wrapper::Set_Transform(D3DTS_VIEW,old_view);
	DX8Wrapper::Set_Transform(D3DTS_WORLD,old_world);

}

// ----------------------------------------------------------------------------

void SortingRendererClass::Deinit()
{
	SortingNodeStruct *head = NULL;

	//
	//	Flush the sorted list
	//
	while ((head = sorted_list.Head ()) != NULL) {
		sorted_list.Remove_Head ();
		delete head;
	}

	//
	//	Flush the clean list
	//
	while ((head = clean_list.Head ()) != NULL) {
		clean_list.Remove_Head ();
		delete head;
	}

#ifdef OG
	delete[] vertex_z_array;
	vertex_z_array=NULL;
	vertex_z_array_count=0;
	delete[] polygon_z_array;
	polygon_z_array=NULL;
	polygon_z_array_count=0;
	delete[] node_id_array;
	node_id_array=NULL;
	node_id_array_count=0;
	delete[] sorted_node_id_array;
	sorted_node_id_array=NULL;
	sorted_node_id_array_count=0;
	delete[] polygon_index_array;
	polygon_index_array=NULL;
	polygon_index_array_count=0;
#endif
	delete[] temp_index_array;
	temp_index_array=NULL;
	temp_index_array_count=0;
}


// ----------------------------------------------------------------------------
//
// Insert a VolumeParticle triangle into the sorting system.
//
// ----------------------------------------------------------------------------

void SortingRendererClass::Insert_VolumeParticle(
	const SphereClass& bounding_sphere,
	unsigned short start_index, 
	unsigned short polygon_count,
	unsigned short min_vertex_index,
	unsigned short vertex_count,
	unsigned short layerCount)
{
	if (!WW3D::Is_Sorting_Enabled()) {
		DX8Wrapper::Draw_Triangles(start_index,polygon_count,min_vertex_index,vertex_count);
		return;
	}

	//FOR VOLUME_PARTICLE LOGIC:
	// WE MUST MULTIPLY THE VERTCOUNT AND POLYCOUNT BY THE VOLUME_PARTICLE DEPTH
	DX8_RECORD_SORTING_RENDER( polygon_count * layerCount,vertex_count * layerCount);//THIS IS VOLUME_PARTICLE SPECIFIC

	SortingNodeStruct* state=Get_Sorting_Struct();
	DX8Wrapper::Get_Render_State(state->sorting_state);

#ifdef OG
 	WWASSERT(((state->sorting_state.index_buffer_type==BUFFER_TYPE_SORTING || state->sorting_state.index_buffer_type==BUFFER_TYPE_DYNAMIC_SORTING) &&
		(state->sorting_state.vertex_buffer_type==BUFFER_TYPE_SORTING || state->sorting_state.vertex_buffer_type==BUFFER_TYPE_DYNAMIC_SORTING)));

#endif
#ifdef ZH
 	WWASSERT(
		((state->sorting_state.index_buffer_type==BUFFER_TYPE_SORTING || state->sorting_state.index_buffer_type==BUFFER_TYPE_DYNAMIC_SORTING) &&
		(state->sorting_state.vertex_buffer_types[0]==BUFFER_TYPE_SORTING || state->sorting_state.vertex_buffer_types[0]==BUFFER_TYPE_DYNAMIC_SORTING)));
#endif

	state->bounding_sphere=bounding_sphere;
	state->start_index=start_index;
	state->min_vertex_index=min_vertex_index;
	state->polygon_count=polygon_count * layerCount;//THIS IS VOLUME_PARTICLE SPECIFIC
	state->vertex_count=vertex_count * layerCount;//THIS IS VOLUME_PARTICLE SPECIFIC

#ifdef OG
	SortingVertexBufferClass* vertex_buffer=static_cast<SortingVertexBufferClass*>(state->sorting_state.vertex_buffer);
#endif
#ifdef ZH
	SortingVertexBufferClass* vertex_buffer=static_cast<SortingVertexBufferClass*>(state->sorting_state.vertex_buffers[0]);
#endif
	WWASSERT(vertex_buffer);
	WWASSERT(state->vertex_count<=vertex_buffer->Get_Vertex_Count());

	// Transform the center point to view space for sorting

	D3DXMATRIX mtx=(D3DXMATRIX&)state->sorting_state.world*(D3DXMATRIX&)state->sorting_state.view;
	D3DXVECTOR3 vec=(D3DXVECTOR3&)state->bounding_sphere.Center;
	D3DXVECTOR4 transformed_vec;
	D3DXVec3Transform(
		&transformed_vec,
		&vec,
		&mtx); 
	state->transformed_center=Vector3(transformed_vec[0],transformed_vec[1],transformed_vec[2]);


	// BUT WHAT IS THE DEAL WITH THE VERTCOUNT AND POLYCOUNT BEING N BUT TRANSFORMED CENTER COUNT == 1

	//THE TRANSFORMED CENTER[2] IS THE ZBUFFER DEPTH
	
	/// @todo lorenzen sez use a bucket sort here... and stop copying so much data so many times

	SortingNodeStruct* node=sorted_list.Head();
	while (node) {
		if (state->transformed_center.Z>node->transformed_center.Z) {
			if (sorted_list.Head()==sorted_list.Tail())
				sorted_list.Add_Head(state);
			else
				state->Insert_Before(node);
			break;
		}
		node=node->Succ();
	}
	if (!node) sorted_list.Add_Tail(state);
#ifdef OG

//#ifdef WWDEBUG
//	unsigned short* indices=NULL;
//	SortingIndexBufferClass* index_buffer=static_cast<SortingIndexBufferClass*>(state->sorting_state.index_buffer);
//	WWASSERT(index_buffer);
//	indices=index_buffer->index_buffer;
//	WWASSERT(indices);
//	indices+=state->start_index;
//	indices+=state->sorting_state.iba_offset;
//
//	for (int i=0;i<state->polygon_count;++i) {
//		unsigned short idx1=indices[i*3]-state->min_vertex_index;
//		unsigned short idx2=indices[i*3+1]-state->min_vertex_index;
//		unsigned short idx3=indices[i*3+2]-state->min_vertex_index;
//		WWASSERT(idx1<state->vertex_count);
//		WWASSERT(idx2<state->vertex_count);
//		WWASSERT(idx3<state->vertex_count);
//	}
//#endif
#endif
}







