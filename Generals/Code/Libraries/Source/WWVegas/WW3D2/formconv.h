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
 *                     $Archive:: /Commando/Code/ww3d2/formconv.h                             $*
 *                                                                                             *
 *              Original Author:: Nathaniel Hoffman                                            *
 *                                                                                             *
#ifdef OG
 *                      $Author:: Jani_p                                                      $*
#endif
#ifdef ZH
 *                       Author : Kenny Mitchell                                               * 
#endif
 *                                                                                             *
#ifdef OG
 *                     $Modtime:: 7/16/01 1:33p                                               $*
#endif
#ifdef ZH
 *                     $Modtime:: 06/27/02 1:27p                                              $*
#endif
 *                                                                                             *
#ifdef OG
 *                    $Revision:: 2                                                           $*
#endif
#ifdef ZH
 *                    $Revision:: 3                                                           $*
#endif
 *                                                                                             *
#ifdef ZH
 * 06/27/02 KM Z Format support																						*
#endif
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef FORMCONV_H
#define FORMCONV_H

#include "ww3dformat.h"
#include <d3d8.h>

/*
** This file is used for conversions between D3DFORMAT and WW3DFormat.
*/

D3DFORMAT WW3DFormat_To_D3DFormat(WW3DFormat ww3d_format);
WW3DFormat D3DFormat_To_WW3DFormat(D3DFORMAT d3d_format);
#ifdef ZH

D3DFORMAT WW3DZFormat_To_D3DFormat(WW3DZFormat ww3d_zformat);
WW3DZFormat D3DFormat_To_WW3DZFormat(D3DFORMAT d3d_format);
#endif

void Init_D3D_To_WW3_Conversion();

#endif