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
#ifdef OG
 *                     $Archive:: /Commando/Code/wwlib/STIMER.CPP                             $* 
#endif // OG
#ifdef ZH
 *                     $Archive:: /Commando/Code/wwlib/stimer.cpp                             $*
#endif // ZH
 *                                                                                             * 
#ifdef OG
 *                      $Author:: Greg_h                                                      $*
#endif // OG
#ifdef ZH
 *                      $Author:: Steve_t                                                     $*
#endif // ZH
 *                                                                                             * 
#ifdef OG
 *                     $Modtime:: 7/09/99 1:46p                                               $*
#endif // OG
#ifdef ZH
 *                     $Modtime:: 12/09/01 6:42p                                              $*
#endif // ZH
 *                                                                                             * 
#ifdef OG
 *                    $Revision:: 3                                                           $*
#endif // OG
#ifdef ZH
 *                    $Revision:: 4                                                           $*
#endif // ZH
 *                                                                                             *
 *---------------------------------------------------------------------------------------------* 
 * Functions:                                                                                  * 
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"always.h"
#include	"stimer.h"
#include	"win.h"

#ifdef _MSC_VER
#pragma warning (push,3)
#endif

#ifdef OG
#include <mmsystem.h>
#endif // OG
#ifdef ZH
#include "systimer.h"
#endif // ZH

#ifdef _MSC_VER
#pragma warning (pop)
#endif


long SystemTimerClass::operator () (void) const
{
#ifdef OG
	return timeGetTime()/16;
#endif // OG
#ifdef ZH
	return TIMEGETTIME()/16;
#endif // ZH
}


SystemTimerClass::operator long (void) const
{
#ifdef OG
	return timeGetTime()/16;
#endif // OG
#ifdef ZH
	return TIMEGETTIME()/16;
#endif // ZH
}
