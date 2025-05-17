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
 *                 Project Name : Command & Conquer                                            * 
 *                                                                                             * 
 *                     $Archive:: /Commando/Code/wwlib/rndstrng.h                             $* 
 *                                                                                             * 
#ifdef OG
 *                      $Author:: Byon_g                                                      $*
#endif
#ifdef ZH
 *                      $Author:: Jani_p                                                      $*
#endif
 *                                                                                             * 
#ifdef OG
 *                     $Modtime:: 11/03/99 2:26p                                              $*
#endif
#ifdef ZH
 *                     $Modtime:: 9/01/01 11:03a                                              $*
#endif
 *                                                                                             * 
#ifdef OG
 *                    $Revision:: 2                                                           $*
#endif
#ifdef ZH
 *                    $Revision:: 3                                                           $*
#endif
 *                                                                                             *
 *---------------------------------------------------------------------------------------------* 
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef RNDSTRNG_H
#define RNDSTRNG_H

#include	"vector.h"
#include	"random.h"

class	StringClass;

/*
**
*/
class	RandomStringClass {

public:
#ifdef OG
	RandomStringClass( void );
	~RandomStringClass( void );

#endif
	// Add a string to the class.  
	// (future version may have a weight parameter)
	void Add_String( const char * str );

	// Get a random string from the class
	const char * Get_String( void );

private:
#ifdef OG
	DynamicVectorClass<StringClass*>	Strings;
#endif
#ifdef ZH
	DynamicVectorClass<StringClass>	Strings;
#endif
	Random2Class							Randomizer;
};

#endif // RNDSTRNG_H

