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
 *                     $Archive:: /VSS_Sync/wwlib/trim.cpp                                    $* 
#endif
#ifdef ZH
 *                     $Archive:: /Commando/Code/wwlib/trim.cpp                               $* 
#endif
 *                                                                                             * 
#ifdef OG
 *                      $Author:: Vss_sync                                                    $*
#endif
#ifdef ZH
 *                      $Author:: Denzil_l                                                    $*
#endif
 *                                                                                             * 
#ifdef OG
 *                     $Modtime:: 8/29/01 10:24p                                              $*
#endif
#ifdef ZH
 *                     $Modtime:: 11/08/01 11:35a                                             $*
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
 * Functions:                                                                                  * 
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"always.h"
#include	"trim.h"
#ifdef OG
#include	<ctype.h>
#include	<stddef.h>
#endif
#include	<string.h>

#ifdef _UNIX
#include	<wctype.h>
#endif // _UNIX

/*********************************************************************************************** 
 * strtrim -- Trim leading and trailing white space off of string.                             * 
 *                                                                                             * 
 *    This routine will remove the leading and trailing whitespace from the string specifed.   * 
 *    The string is modified in place.                                                         * 
 *                                                                                             * 
 * INPUT:   buffer   -- Pointer to the string to be trimmed.                                   * 
 *                                                                                             * 
 * OUTPUT:  none                                                                               * 
 *                                                                                             * 
 * WARNINGS:   none                                                                            * 
 *                                                                                             * 
 * HISTORY:                                                                                    * 
 *   02/06/1997 JLB : Created.                                                                 * 
 *=============================================================================================*/
#ifdef OG
char * strtrim(char * buffer)
#endif
#ifdef ZH
char* strtrim(char* buffer)
#endif
{
#ifdef OG
	if (buffer != NULL) {

#endif
#ifdef ZH
	if (buffer) {
		/* Strip leading white space from the string. */
		char* source = buffer;
#endif

#ifdef OG
		/*
		**	Strip leading white space from the string.
		*/
		char * source = buffer;
		while (isspace(*source)) {
			source++;
#endif
#ifdef ZH
		while ((*source != 0) && ((unsigned char)*source <= 32)) {
			++source;

#endif
		}
		if (source != buffer) {
			strcpy(buffer, source);
		}

#ifdef OG
		/*
		**	Clip trailing white space from the string.
		*/
		for (int index = strlen(buffer)-1; index >= 0; index--) {
			if (isspace(buffer[index])) {
#endif
#ifdef ZH
		/* Clip trailing white space from the string. */
		for (int index = strlen(buffer) - 1; index >= 0; --index) {
			if ((*source != 0) && ((unsigned char)buffer[index] <= 32)) {

#endif
				buffer[index] = '\0';
			} else {
				break;
			}
		}
	}
#ifdef OG
	return(buffer);

#endif
#ifdef ZH

	return buffer;
#endif
}

#ifdef OG
wchar_t * wcstrim(wchar_t * buffer)
#endif
#ifdef ZH
wchar_t* wcstrim(wchar_t* buffer)
#endif
{
#ifdef OG
	if (buffer != NULL) {

#endif
#ifdef ZH
	if (buffer) {
		/* Strip leading white space from the string. */
		wchar_t* source = buffer;
#endif

#ifdef OG
		/*
		**	Strip leading white space from the string.
		*/
		wchar_t * source = buffer;
		while (iswspace(*source)) {
			source++;
#endif
#ifdef ZH
		while ((*source != 0) && ((unsigned int)*source <= 32)) {
			++source;

#endif
		}
		if (source != buffer) {
			wcscpy(buffer, source);
		}

#ifdef OG
		/*
		**	Clip trailing white space from the string.
		*/
		for (int index = wcslen(buffer)-1; index >= 0; index--) {
			if (iswspace(buffer[index])) {
#endif
#ifdef ZH
		/* Clip trailing white space from the string. */
		for (int index = wcslen(buffer) - 1; index >= 0; --index) {
			if ((*source != 0) && ((unsigned int)buffer[index] <= 32)) {

#endif
				buffer[index] = L'\0';
			} else {
				break;
			}
		}
	}
#ifdef OG
	return(buffer);

#endif
#ifdef ZH

	return buffer;
#endif
}



