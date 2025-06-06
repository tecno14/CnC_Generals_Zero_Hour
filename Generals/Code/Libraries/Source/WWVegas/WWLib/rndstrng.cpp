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
 *                     $Archive:: /Commando/Code/wwlib/rndstrng.cpp                           $* 
 *                                                                                             * 
#ifdef OG
 *                      $Author:: Byon_g                                                      $*
#endif
#ifdef ZH
 *                      $Author:: Jani_p                                                      $*
#endif
 *                                                                                             * 
#ifdef OG
 *                     $Modtime:: 2/08/00 10:59a                                              $*
#endif
#ifdef ZH
 *                     $Modtime:: 9/01/01 11:05a                                              $*
#endif
 *                                                                                             * 
#ifdef OG
 *                    $Revision:: 3                                                           $*
#endif
#ifdef ZH
 *                    $Revision:: 4                                                           $*
#endif
 *                                                                                             *
 *---------------------------------------------------------------------------------------------* 
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"rndstrng.h"
#include	"wwstring.h"

/*
**
*/
#ifdef OG
RandomStringClass::RandomStringClass( void )
{
}

RandomStringClass::~RandomStringClass( void )
{
	while ( Strings.Count() ) {
		delete Strings[ 0 ];
		Strings.Delete( 0 );
	}
}
#endif

void RandomStringClass::Add_String( const char * str )
{
#ifdef OG
	StringClass	 * string = W3DNEW StringClass( str );
	Strings.Add( string );
#endif
#ifdef ZH
	Strings.Add( str );

#endif
}

const char * RandomStringClass::Get_String( void )
{
	if ( Strings.Count() == 0 ) {
		return NULL;
	}
	unsigned int index = Randomizer();
	index %= Strings.Count();
#ifdef OG
	return *Strings[index];
#endif
#ifdef ZH
	return Strings[index];
#endif
}

