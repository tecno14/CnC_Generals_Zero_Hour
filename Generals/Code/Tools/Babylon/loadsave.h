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

//
// loadsave.h
//

#ifndef __LOADSAVE_H
#define __LOADSAVE_H

#ifdef OG
int WriteMainDB(TransDB *db, const char *filename, CNoxstringDlg *dlg );
#endif
#ifdef ZH
int WriteMainDB(TransDB *db, const char *filename, CBabylonDlg *dlg );
#endif
int LoadMainDB(TransDB *db, const char *filename, void (*cb) (void ) = NULL );
int	GetLabelCountDB ( char *filename );


#endif