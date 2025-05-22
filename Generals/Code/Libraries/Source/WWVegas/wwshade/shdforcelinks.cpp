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
 *                 Project Name : wwshade                                                        *
 *                                                                                             *
 *                     $Archive:: \code\wwshade\shdforcelinks.cpp											 $*
 *                                                                                             *
 *                   Org Author:: Patrick Smith                                                *
 *                                                                                             *
 *                       Author:: Kenny Mitchell                                                *
 *                                                                                             *
 *                     $Modtime:: 6/27/2002 4:28p                                            $*
 *                                                                                             *
 *                    $Revision:: 3                                                           $*
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "shdforcelinks.h"
#include "wwhack.h"


/////////////////////////////////////////////////////////////////////////////
//
//	SHD_Force_Links
//
/////////////////////////////////////////////////////////////////////////////
void SHD_Force_Links ()
{
	//
	//	Force-link those modules that would be "linked-out" of the static lib
	//	because they are not directly referenced.
	//
	FORCE_LINK (SimpleShader);
	FORCE_LINK (GlossMaskShader);
	FORCE_LINK (BumpSpecShader);
	FORCE_LINK (BumpDiffShader);
	FORCE_LINK (CubeMapShader);
	FORCE_LINK (LegacyW3DShader);

	return ;
}
