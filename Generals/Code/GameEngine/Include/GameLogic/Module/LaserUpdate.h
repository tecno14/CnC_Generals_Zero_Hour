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

////////////////////////////////////////////////////////////////////////////////
//																																						//
//  (c) 2001-2003 Electronic Arts Inc.																				//
//																																						//
////////////////////////////////////////////////////////////////////////////////

// FILE: LaserUpdate.h //////////////////////////////////////////////////////////////////////////
// Author: Kris Morness, July 2002
// Desc:   Handles laser update processing for render purposes and game control.
// Modifications: Kris Morness - Oct 2002 -- moved to Client update (will rename later)
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __LASER_UPDATE_H
#define __LASER_UPDATE_H

// INCLUDES ///////////////////////////////////////////////////////////////////////////////////////
#include "Common/ClientUpdateModule.h"

// FORWARD REFERENCES /////////////////////////////////////////////////////////////////////////////
class Thing;
class Vector3;
enum ParticleSystemID;

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
class LaserUpdateModuleData : public ClientUpdateModuleData
{
public:
	AsciiString m_particleSystemName;  ///< Used for the muzzle flare while laser active.
#ifdef OG
	AsciiString m_parentFireBoneName;  ///< Used to fire laser at specified parent bone position.
	Bool m_parentFireBoneOnTurret;			///< And used to specifiy where to look for the bone.
#endif

	AsciiString m_targetParticleSystemName;  ///< Used for the target effect while laser active.

#ifdef ZH
	Real m_punchThroughScalar;	///< If non-zero, length modifier when we used to have a target object and now don't

#endif
	LaserUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse& p);

private: 

};


//-------------------------------------------------------------------------------------------------
/** The default	update module */
//-------------------------------------------------------------------------------------------------
class LaserUpdate : public ClientUpdateModule
{

	MEMORY_POOL_GLUE_WITH_USERLOOKUP_CREATE( LaserUpdate, "LaserUpdate" )
	MAKE_STANDARD_MODULE_MACRO_WITH_MODULE_DATA( LaserUpdate, LaserUpdateModuleData );

public:

	LaserUpdate( Thing *thing, const ModuleData* moduleData );
	// virtual destructor prototype provided by memory pool declaration

	//Actually puts the laser in the world.
#ifdef OG
	void initLaser( const Object *parent, const Coord3D *startPos, const Coord3D *endPos, Int sizeDeltaFrames = 0 );
#endif
#ifdef ZH
	void initLaser( const Object *parent, const Object *target, const Coord3D *startPos, const Coord3D *endPos, AsciiString parentBoneName, Int sizeDeltaFrames = 0 );
#endif
	void setDecayFrames( UnsignedInt decayFrames );

	const Coord3D* getStartPos() { return &m_startPos; }
	const Coord3D* getEndPos() { return &m_endPos; }

	Real getCurrentLaserRadius() const;

	void setDirty( Bool dirty ) { m_dirty = dirty; }
	Bool isDirty() { return m_dirty; }

	Real getWidthScale() const { return m_currentWidthScalar; }

	virtual void clientUpdate();

protected:
#ifdef ZH

	void updateStartPos(); ///< figures out and sets startPos
	void updateEndPos(); ///< figures out and sets endPos
#endif

	//If the master dies, so will this laser (although if it has a fade delay, it'll just skip to the fade)
	Coord3D m_startPos;
	Coord3D m_endPos;
#ifdef ZH

	DrawableID m_parentID;
	DrawableID m_targetID;

#endif
	Bool m_dirty;
	ParticleSystemID m_particleSystemID;
	ParticleSystemID m_targetParticleSystemID;
	Bool m_widening;
	Bool m_decaying;
	UnsignedInt m_widenStartFrame;
	UnsignedInt m_widenFinishFrame;
	Real m_currentWidthScalar;
	UnsignedInt m_decayStartFrame;
	UnsignedInt m_decayFinishFrame;
#ifdef ZH
	AsciiString m_parentBoneName;
#endif
};


#endif

