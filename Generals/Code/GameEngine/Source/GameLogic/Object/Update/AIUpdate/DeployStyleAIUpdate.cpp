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

// DeployStyleAIUpdate.cpp ////////////
// Author: Kris Morness, August 2002
// Desc:   A standard ai update that also handles units that must deploy to attack and pack before moving.

#include "PreRTS.h"	// This must go first in EVERY cpp file int the GameEngine

#include "Common/Player.h"
#include "Common/ThingFactory.h"
#include "Common/ThingTemplate.h"

#include "GameClient/Drawable.h"
#include "GameClient/InGameUI.h"

#include "GameLogic/ExperienceTracker.h"
#include "GameLogic/Locomotor.h"
#include "GameLogic/Object.h"
#include "GameLogic/PartitionManager.h"
#include "GameLogic/Weapon.h"

#include "GameLogic/Module/BodyModule.h"
#include "GameLogic/Module/ContainModule.h"
#include "GameLogic/Module/DeployStyleAIUpdate.h"
#include "GameLogic/Module/PhysicsUpdate.h"

#ifdef _INTERNAL
// for occasional debugging...
//#pragma optimize("", off)
//#pragma MESSAGE("************************************** WARNING, optimization disabled for debugging purposes")
#endif

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
DeployStyleAIUpdate::DeployStyleAIUpdate( Thing *thing, const ModuleData* moduleData ) : AIUpdateInterface( thing, moduleData )
{
#ifdef OG
	m_hasOutsideCommand = false;
#endif
	m_state = READY_TO_MOVE;
#ifdef OG
	m_frameToWakeForDeploy = 0;
	reset();
#endif
#ifdef ZH
	m_frameToWaitForDeploy = 0;

#endif
} 

//-------------------------------------------------------------------------------------------------
DeployStyleAIUpdate::~DeployStyleAIUpdate( void )
{
} 

//-------------------------------------------------------------------------------------------------
Bool DeployStyleAIUpdate::isIdle() const
{
#ifdef OG
	// we need to do this because we enter an idle state during deploy states in these cases,
	// but scripting relies on us never claiming to be "idle"...
	if (m_hasOutsideCommand)
		return false;

#endif
	return AIUpdateInterface::isIdle();
#ifdef OG
}

//-------------------------------------------------------------------------------------------------
void DeployStyleAIUpdate::reset()
{
	m_designatedTargetID	= INVALID_ID;
	m_isAttackMultiple		= FALSE;
	m_overriddenAttack		= FALSE;
	m_isGuardingPosition	= FALSE;
	m_isAttackObject			= FALSE;
	m_attackObjectID			= INVALID_ID;
	m_isAttackPosition		= FALSE;
	m_position.zero();
#endif
}

//-------------------------------------------------------------------------------------------------
void DeployStyleAIUpdate::aiDoCommand( const AICommandParms* parms )
{
	if (!isAllowedToRespondToAiCommands(parms))
		return;

#ifdef OG
	if( parms->m_cmd != AICMD_IDLE && parms->m_cmd != AICMD_FOLLOW_PATH_APPEND )
	{
		aiIdle( CMD_FROM_AI );
	}
	if( parms->m_cmdSource != CMD_FROM_AI )
	{
		reset();
		if( parms->m_cmd != AICMD_IDLE )
		{
			m_lastOutsideCommand.store( *parms );
			m_hasOutsideCommand = TRUE;
		}
		
		if( m_state != DEPLOY && m_state != UNDEPLOY )
		{
			//Only issue the command if we're not in the process of deploying/undeploying.
			AIUpdateInterface::aiDoCommand( parms );
		}
#endif
#ifdef ZH
	/*
	//Hack code to allow follow waypoint scripts to be converted to attack follow waypoint scripts
	//for simple-test-map-purposes.

#endif
		switch( parms->m_cmd ) 
		{
#ifdef OG
			case AICMD_GUARD_POSITION:
				m_position.set( &parms->m_pos );
				m_isGuardingPosition = TRUE;
				//fall through (no break)
			case AICMD_GUARD_OBJECT:
			case AICMD_GUARD_AREA:
			case AICMD_ATTACKMOVE_TO_POSITION:
			case AICMD_HUNT:
			case AICMD_ATTACKFOLLOW_WAYPOINT_PATH:
			case AICMD_ATTACKFOLLOW_WAYPOINT_PATH_AS_TEAM:
			case AICMD_ATTACK_AREA:
				m_isAttackMultiple = TRUE;
#endif
#ifdef ZH
		case AICMD_FOLLOW_WAYPOINT_PATH:
			((AICommandParms*)parms)->m_cmd = AICMD_ATTACKFOLLOW_WAYPOINT_PATH;

#endif
				break;
#ifdef OG
			case AICMD_ATTACK_OBJECT:
			case AICMD_FORCE_ATTACK_OBJECT:
				m_isAttackObject = TRUE;
				m_attackObjectID = parms->m_obj ? parms->m_obj->getID() : INVALID_ID;
				break;
			case AICMD_ATTACK_POSITION:
				m_isAttackPosition = TRUE;
				m_position.set( &parms->m_pos );
#endif
#ifdef ZH
		case AICMD_FOLLOW_WAYPOINT_PATH_AS_TEAM:
			((AICommandParms*)parms)->m_cmd = AICMD_ATTACKFOLLOW_WAYPOINT_PATH_AS_TEAM;

#endif
				break;
		}
#ifdef OG
	}
	else
	{
		//Always process AI issued commands.
#endif
#ifdef ZH
	*/

#endif
		AIUpdateInterface::aiDoCommand( parms );
#ifdef OG
	}
#endif
}

//-------------------------------------------------------------------------------------------------
UpdateSleepTime DeployStyleAIUpdate::update( void )
{
	// have to call our parent's isIdle, because we override it to never return true
	// when we have a pending command...
	Object *self = getObject();
	Weapon *weapon = self->getCurrentWeapon();
#ifdef OG
	Bool inRange = FALSE;
	Object *designatedTarget = NULL;
	Bool isAttacking = FALSE;

#endif
#ifdef ZH
	UnsignedInt now = TheGameLogic->getFrame();
	const DeployStyleAIUpdateModuleData *data = getDeployStyleAIUpdateModuleData();

	//Are we attempting to move? If so we can't do it unless we are undeployed.
	Bool isTryingToMove = isWaitingForPath() || getPath();
	
	//Are we trying to attack something. If so, we need to be in range before we can do so.
	Bool isTryingToAttack = getStateMachine()->isInAttackState();
	Bool isInRange = FALSE;

	//Are we in guard mode? If so, are we idle... idle guarders deploy for fastest response against attackers.
	Bool isInGuardIdleState = getStateMachine()->isInGuardIdleState();

	AIUpdateInterface *ai = self->getAI();
#endif

#ifdef OG
	if( weapon )
#endif
#ifdef ZH
	if( isTryingToAttack && weapon )
#endif
	{
#ifdef OG
		if( m_isAttackPosition )

#endif
#ifdef ZH
		Object *victim = ai->getCurrentVictim();
		if( victim )
#endif
		{
#ifdef OG
			//Handle force attacking attacking specific position
			inRange = weapon->isWithinAttackRange( self, &m_position );
			isAttacking = TRUE;
#endif
#ifdef ZH
			isInRange = weapon->isWithinAttackRange( self, victim );

#endif
		}
#ifdef OG
		else if( m_isAttackObject )
#endif
#ifdef ZH
		else 
#endif
		{
#ifdef OG
			//Handle attacking a specific object.
			designatedTarget = TheGameLogic->findObjectByID( m_attackObjectID );
			if( designatedTarget && designatedTarget->isEffectivelyDead() )
#endif
#ifdef ZH
			const Coord3D *pos = ai->getCurrentVictimPos();
			if( pos )

#endif
			{
#ifdef OG
				designatedTarget = NULL;
#endif
#ifdef ZH
				isInRange = weapon->isWithinAttackRange( self, pos );
#endif
			}
#ifdef OG
			if( designatedTarget )
			{
				inRange = weapon->isWithinAttackRange( self, designatedTarget );
				isAttacking = TRUE;
#endif
			}
		}
#ifdef OG
		else if( m_isAttackMultiple )

#endif
#ifdef ZH

	if( m_frameToWaitForDeploy != 0 && now >= m_frameToWaitForDeploy)
#endif
		{
#ifdef OG
			Bool newTarget = FALSE;
			//Handle attackmove and guard.
			//We are attacking in a different way... so attempt to figure out how.
			WhichTurretType tur = getWhichTurretForCurWeapon();
			if( tur != TURRET_INVALID )
#endif
#ifdef ZH
		switch( m_state )

#endif
			{
#ifdef OG
				//Get the turret's current target.
				designatedTarget = getTurretTargetObject( tur );

#endif
#ifdef ZH
			case DEPLOY:
				setMyState( READY_TO_ATTACK );
				break;
			case UNDEPLOY:
				setMyState( READY_TO_MOVE );
				break;
#endif
			}
#ifdef OG
			else
			{
				//Get the current goal object (NULL if we have a turret).
				designatedTarget = getGoalObject();
#endif
			}
#ifdef OG
			if( !designatedTarget )

#endif
#ifdef ZH
	
	if( isInRange || isInGuardIdleState )
#endif
			{
#ifdef OG
				//If we still don't have a target, get the last known target.
				designatedTarget = TheGameLogic->findObjectByID( m_designatedTargetID );
			}
			if( designatedTarget && designatedTarget->isEffectivelyDead() )
#endif
#ifdef ZH
		switch( m_state )

#endif
			{
#ifdef OG
				//See if we can acquire one!
				designatedTarget = getNextMoodTarget( TRUE, FALSE );
				newTarget = TRUE;
			}
			if( !designatedTarget && m_isGuardingPosition )
			{
				//While guarding, acquire a target periodically.
				designatedTarget = getNextMoodTarget( FALSE, FALSE );
				if( designatedTarget )

#endif
#ifdef ZH
			case READY_TO_MOVE:
				//We're need to deploy before we can attack.
				setMyState( DEPLOY );
				break;
			case READY_TO_ATTACK:
				//Let the AI handle attacking.
				break;
			case DEPLOY:
				//We can't start attacking until we are fully deployed.
				break;
			case UNDEPLOY:
				if( m_frameToWaitForDeploy != 0 )
#endif
				{
#ifdef OG
					inRange = weapon->isWithinAttackRange( self, designatedTarget );
					isAttacking = TRUE;
					if( inRange )
					{
						//Continue overriding it as long as there are enemies in range!
						aiAttackObject( designatedTarget, NO_MAX_SHOTS_LIMIT, CMD_FROM_AI );
						m_overriddenAttack = TRUE;
						m_designatedTargetID = designatedTarget->getID();
#endif
#ifdef ZH
					//Reverse the undeploy at it's current frame!
					setMyState( DEPLOY, TRUE );

#endif
					}
#ifdef OG
					else 
					{
						designatedTarget = NULL;

#endif
#ifdef ZH
				break;
			case ALIGNING_TURRETS:
				//If turrets are aligning, we are able to attack right now.
				setMyState( READY_TO_ATTACK );
				break;
#endif
					}
				}
#ifdef OG
				else 
#endif
#ifdef ZH
	else if( isTryingToMove )
#endif
				{
#ifdef OG
					designatedTarget = NULL;
				}
			}
			else if( designatedTarget )
			{
				//Finally... we have a target -- so are we in range?
				inRange = weapon->isWithinAttackRange( self, designatedTarget );
				isAttacking = TRUE;
				m_designatedTargetID = designatedTarget->getID();
				if( m_overriddenAttack && newTarget && inRange )
				{
					//Continue overriding it as long as there are enemies in range!
					aiAttackObject( designatedTarget, NO_MAX_SHOTS_LIMIT, CMD_FROM_AI );
				}
			}
			else
			{
				//No target, we must be done or moving.
				m_designatedTargetID = INVALID_ID;
			}
		}
	}

	Bool remainDeployed = m_isGuardingPosition && !designatedTarget && !isMoving() && !isWaitingForPath();

	UnsignedInt now = TheGameLogic->getFrame();
#endif
	switch( m_state )
	{
		case READY_TO_MOVE:
#ifdef OG
			if( remainDeployed || (inRange && isAttacking) )
			{
				setMyState( DEPLOY );
			}
#endif
#ifdef ZH
				//We're ready... ai will handle moving.

#endif
			break;
		case READY_TO_ATTACK:
#ifdef OG
			if( !remainDeployed && (!inRange && isAttacking || !isAttacking && (isWaitingForPath() || getPath())) )
#endif
			{
				WhichTurretType tur = getWhichTurretForCurWeapon();
				if( tur != TURRET_INVALID )
				{
					if( doTurretsHaveToCenterBeforePacking() )
					{
#ifdef ZH
						//Turrets need to center before we can undeploy.
#endif
						setMyState( ALIGNING_TURRETS );
						break;
					}
				}
#ifdef ZH
				//Start undeploying.
#endif
				setMyState( UNDEPLOY );
#ifdef OG
			}
			else if( !designatedTarget && m_overriddenAttack && m_hasOutsideCommand )
			{
				//For multiple attacks we specifically order the unit to attack one object. After
				//object is dead, we revert to our original command.
				AICommandParms parms( AICMD_MOVE_TO_POSITION, CMD_FROM_AI );	// values don't matter, will be wiped by next line
				m_lastOutsideCommand.reconstitute( parms );
 				aiDoCommand(&parms);
#endif
#ifdef ZH
				break;

#endif
			}
#ifdef OG
			break;
#endif
		case DEPLOY:
#ifdef OG
			if( m_frameToWakeForDeploy != 0 && now >= m_frameToWakeForDeploy)
#endif
#ifdef ZH
				if( m_frameToWaitForDeploy != 0 )
#endif
			{
#ifdef OG
				setMyState( READY_TO_ATTACK );
				if( m_isAttackMultiple && inRange && isAttacking && designatedTarget )
				{
					aiAttackObject( designatedTarget, NO_MAX_SHOTS_LIMIT, CMD_FROM_AI );
					m_overriddenAttack = TRUE;
				}
#endif
#ifdef ZH
					//Reverse the deploy at it's current frame!
					setMyState( UNDEPLOY, TRUE );

#endif
			}
			break;
		case UNDEPLOY:
#ifdef OG
			if( m_frameToWakeForDeploy != 0 && now >= m_frameToWakeForDeploy)
			{
				setMyState( READY_TO_MOVE );
			}
#endif
			break;
		case ALIGNING_TURRETS:
		{
			WhichTurretType tur = getWhichTurretForCurWeapon();
			if( tur != TURRET_INVALID )
			{
				if( isTurretInNaturalPosition( tur ) )
				{
#ifdef ZH
						//Turrets are centers, so now we can undeploy.
#endif
					setMyState( UNDEPLOY );
				}
			}
			break;
		}
	}
#ifdef ZH
	}
#endif
	
#ifdef OG
	UpdateSleepTime mine = UPDATE_SLEEP_FOREVER;
#endif
	switch( m_state )
#ifdef ZH
	{
		case READY_TO_MOVE:
			if( isTryingToMove )
#endif
	{
#ifdef ZH
				self->setModelConditionState( MODELCONDITION_MOVING );
			}
			break;

#endif
		case READY_TO_ATTACK:
#ifdef OG
		case READY_TO_MOVE:
			mine = UPDATE_SLEEP_FOREVER;	// we can sleep for, well, a while
#endif
			break;

		case DEPLOY:
#ifdef ZH
			if( data->m_manualDeployAnimations )
			{
				UnsignedInt totalFrames = getPackTime();
				UnsignedInt framesLeft = m_frameToWaitForDeploy - now;
				Drawable *draw = self->getDrawable();
				if( draw )
				{
					draw->setAnimationFrame( totalFrames - framesLeft );
				}
			}
			getStateMachine()->setTemporaryState( AI_BUSY, UPDATE_SLEEP_NONE );
			setLocomotorGoalNone();
			break;
#endif
		case UNDEPLOY:
#ifdef OG
			mine = m_frameToWakeForDeploy > now ? UPDATE_SLEEP(m_frameToWakeForDeploy - now) : UPDATE_SLEEP_NONE;
			aiIdle( CMD_FROM_AI );

#endif
#ifdef ZH
			if( data->m_manualDeployAnimations )
			{
				UnsignedInt framesLeft = m_frameToWaitForDeploy - now;
				Drawable *draw = self->getDrawable();
				if( draw )
				{
					draw->setAnimationFrame( framesLeft );
				}
			}
			getStateMachine()->setTemporaryState( AI_BUSY, UPDATE_SLEEP_NONE );
			setLocomotorGoalNone();
#endif
			break;

		case ALIGNING_TURRETS:
#ifdef OG
			mine = UPDATE_SLEEP_NONE;	// no sleep for us right now.
			aiIdle( CMD_FROM_AI );
#endif
#ifdef ZH
			getStateMachine()->setTemporaryState( AI_BUSY, UPDATE_SLEEP_NONE );
			setLocomotorGoalNone();
#endif
			break;
	}
#ifdef OG
	UpdateSleepTime ret = AIUpdateInterface::update();
	return (mine < ret) ? mine : ret;

#endif
#ifdef ZH

	AIUpdateInterface::update();
	//We can't sleep the deploy AI because any new commands need to be caught and sent
	//into busy state during the update.
	return UPDATE_SLEEP_NONE;

#endif
}

//-------------------------------------------------------------------------------------------------
#ifdef OG
void DeployStyleAIUpdate::setMyState( DeployStateTypes stateID )
#endif
#ifdef ZH
void DeployStyleAIUpdate::setMyState( DeployStateTypes stateID, Bool reverseDeploy )
#endif
{
	m_state = stateID;
	Object *self = getObject();
#ifdef ZH
	UnsignedInt now = TheGameLogic->getFrame();
	const DeployStyleAIUpdateModuleData *data = getDeployStyleAIUpdateModuleData();
	
#endif
	switch( stateID )
	{
		case DEPLOY:
		{
			//Tell our object to deploy (so it can continue the same attack later).
#ifdef OG
			aiIdle( CMD_FROM_AI );
#endif
			self->clearAndSetModelConditionFlags( MAKE_MODELCONDITION_MASK( MODELCONDITION_PACKING ), 
																						 MAKE_MODELCONDITION_MASK( MODELCONDITION_UNPACKING ) );
#ifdef OG
			m_frameToWakeForDeploy = getUnpackTime(); //In frames
			//Make sure the animation matches the length of unpacking
			self->getDrawable()->setAnimationLoopDuration( m_frameToWakeForDeploy );
			m_frameToWakeForDeploy += TheGameLogic->getFrame();	// convert to absolute frame

#endif
#ifdef ZH

			if( reverseDeploy )
			{
				//This is a zero to max animation.
				UnsignedInt totalFrames = getUnpackTime();
				UnsignedInt framesLeft = m_frameToWaitForDeploy - now;
				m_frameToWaitForDeploy = now + totalFrames - framesLeft;
				if( data->m_manualDeployAnimations )
				{
					Drawable *draw = self->getDrawable();
					if( draw )
					{
						draw->setAnimationFrame( totalFrames - framesLeft );
					}
				}
			}
			else
			{
				m_frameToWaitForDeploy = getUnpackTime() + now; 
			}
#endif

			//Play deploy sound
			const ThingTemplate *thing = self->getTemplate();
			const AudioEventRTS* soundToPlayPtr = thing->getPerUnitSound( "Deploy" );
			if( soundToPlayPtr )
			{
				AudioEventRTS soundToPlay = *soundToPlayPtr;
				soundToPlay.setObjectID( self->getID() );
				TheAudio->addAudioEvent( &soundToPlay );
			}
			
			break;
		}
		case UNDEPLOY:
		{
#ifdef ZH
			//This status will tell the approach AI state to succeed automatically. This prevents
			//twitching on deploy. Make sure we clear it now so when it goes into that state, it'll
			//actually move.
			self->clearStatus( MAKE_OBJECT_STATUS_MASK( OBJECT_STATUS_DEPLOYED ) );

#endif
			//Tell our object to pack up (so it can continue the same move later).
#ifdef OG
			aiIdle( CMD_FROM_AI );
#endif
			self->clearAndSetModelConditionFlags( MAKE_MODELCONDITION_MASK2( MODELCONDITION_UNPACKING, MODELCONDITION_DEPLOYED ),
																						 MAKE_MODELCONDITION_MASK( MODELCONDITION_PACKING ) );
#ifdef OG
			m_frameToWakeForDeploy = getPackTime(); //In frames
			//Make sure the animation matches the length of packing
			self->getDrawable()->setAnimationLoopDuration( m_frameToWakeForDeploy );
			m_frameToWakeForDeploy += TheGameLogic->getFrame();	// convert to absolute frame

#endif
#ifdef ZH

			if( reverseDeploy )
			{
				//This is a max to zero animation.
				UnsignedInt totalFrames = getUnpackTime();
				UnsignedInt framesLeft = m_frameToWaitForDeploy - now;
				m_frameToWaitForDeploy = now + totalFrames - framesLeft;
				if( data->m_manualDeployAnimations )
				{
					Drawable *draw = self->getDrawable();
					if( draw )
					{
						draw->setAnimationFrame( framesLeft );
					}
				}
			}
			else
			{
				m_frameToWaitForDeploy = getPackTime() + now;
			}
#endif

			if( doTurretsFunctionOnlyWhenDeployed() )
			{
				WhichTurretType tur = getWhichTurretForCurWeapon();
				if( tur != TURRET_INVALID )
				{
					setTurretEnabled( tur, false );
				}
			}
			
			//Play undeploy sound
			const ThingTemplate *thing = self->getTemplate();
			const AudioEventRTS* soundToPlayPtr = thing->getPerUnitSound( "Undeploy" );
			if( soundToPlayPtr )
			{
				AudioEventRTS soundToPlay = *soundToPlayPtr;
				soundToPlay.setObjectID( self->getID() );
				TheAudio->addAudioEvent( &soundToPlay );
			}

			break;
		}
		case READY_TO_MOVE:
		{
#ifdef OG
			m_frameToWakeForDeploy = 0;
#endif
#ifdef ZH
			m_frameToWaitForDeploy = 0;
#endif

#ifdef OG
			//We're ready to move, so restore our command!
			if( m_hasOutsideCommand )
			{
				AICommandParms parms( AICMD_MOVE_TO_POSITION, CMD_FROM_AI );	// values don't matter, will be wiped by next line
				m_lastOutsideCommand.reconstitute( parms );
	 			aiDoCommand(&parms);
			}

#endif
			self->clearModelConditionFlags( MAKE_MODELCONDITION_MASK( MODELCONDITION_PACKING ) );
			break;
		}
		case READY_TO_ATTACK:
		{
#ifdef OG
			m_frameToWakeForDeploy = 0;

#endif
#ifdef ZH
			//This status will tell the approach AI state to succeed automatically. This prevents
			//twitching on deploy.
			self->setStatus( MAKE_OBJECT_STATUS_MASK( OBJECT_STATUS_DEPLOYED ) );
#endif

#ifdef OG
			//We're ready to attack, so restore our command!
			if( !m_isAttackMultiple && m_hasOutsideCommand )
			{
				AICommandParms parms( AICMD_MOVE_TO_POSITION, CMD_FROM_AI );	// values don't matter, will be wiped by next line
				m_lastOutsideCommand.reconstitute( parms );
 				aiDoCommand(&parms);
			}
#endif
#ifdef ZH
			m_frameToWaitForDeploy = 0;

#endif

			self->clearAndSetModelConditionFlags( MAKE_MODELCONDITION_MASK( MODELCONDITION_UNPACKING ), 
																						 MAKE_MODELCONDITION_MASK( MODELCONDITION_DEPLOYED) );

			if( doTurretsFunctionOnlyWhenDeployed() )
			{
				WhichTurretType tur = getWhichTurretForCurWeapon();
				if( tur != TURRET_INVALID )
				{
					setTurretEnabled( tur, true );
				}
			}

			break;
		}
		case ALIGNING_TURRETS:
		{
#ifdef OG
			m_frameToWakeForDeploy = 0;

#endif
#ifdef ZH
			m_frameToWaitForDeploy = 0;

#endif
			WhichTurretType tur = getWhichTurretForCurWeapon();
			if( tur != TURRET_INVALID )
			{
				recenterTurret( tur );
			}
			break;
		}
	}

}

// ------------------------------------------------------------------------------------------------
/** CRC */
// ------------------------------------------------------------------------------------------------
void DeployStyleAIUpdate::crc( Xfer *xfer )
{
	// extend base class
	AIUpdateInterface::crc(xfer);
}  // end crc

// ------------------------------------------------------------------------------------------------
/** Xfer method
	* Version Info:
	* 1: Initial version 
	* 2: Added support for attack move
	* 3: Added improved support for guard, and support for hunt AI
 **/
// ------------------------------------------------------------------------------------------------
void DeployStyleAIUpdate::xfer( Xfer *xfer )
{
  // version
#ifdef OG
  XferVersion currentVersion = 3;
#endif
#ifdef ZH
  XferVersion currentVersion = 4;
#endif
  XferVersion version = currentVersion;
  xfer->xferVersion( &version, currentVersion );
 
 // extend base class
	AIUpdateInterface::xfer(xfer);

#ifdef OG
	xfer->xferBool(&m_hasOutsideCommand);
	xfer->xferUnsignedInt(&m_frameToWakeForDeploy);
#endif
#ifdef ZH
	if( version >= 4 )
	{
#endif
	xfer->xferUser(&m_state, sizeof(m_state));
#ifdef ZH
		xfer->xferUnsignedInt(&m_frameToWaitForDeploy);
	}
	else if( xfer->getXferMode() == XFER_LOAD )
	{
		//Good riddance!!!
		Bool obsoleteBool;
		UnsignedInt obsoleteUnsignedInt;
		ObjectID obsoleteObjectID;
		Coord3D obsoleteCoord3D;
   	AICommandParmsStorage	obsoleteAICommandParmsStorage;
#endif

#ifdef ZH
		xfer->xferBool( &obsoleteBool );
		xfer->xferUnsignedInt( &obsoleteUnsignedInt );
		xfer->xferUser(&m_state, sizeof(m_state));

#endif
	if( version >= 2 )
	{
#ifdef OG
		xfer->xferObjectID( &m_designatedTargetID );
		xfer->xferObjectID( &m_attackObjectID );
		xfer->xferCoord3D( &m_position );
		xfer->xferBool( &m_isAttackMultiple );
		xfer->xferBool( &m_isAttackObject );
		xfer->xferBool( &m_isAttackPosition );
#endif
#ifdef ZH
			xfer->xferObjectID( &obsoleteObjectID );
			xfer->xferObjectID( &obsoleteObjectID );
			xfer->xferCoord3D( &obsoleteCoord3D );
			xfer->xferBool( &obsoleteBool );
			xfer->xferBool( &obsoleteBool );
			xfer->xferBool( &obsoleteBool );
#endif
	}
	if( version >= 3 )
	{
#ifdef OG
		xfer->xferBool( &m_isGuardingPosition );
		xfer->xferBool( &m_overriddenAttack );
#endif
#ifdef ZH
			xfer->xferBool( &obsoleteBool );
			xfer->xferBool( &obsoleteBool );
#endif
	}

#ifdef OG
	m_lastOutsideCommand.doXfer(xfer);
#endif
#ifdef ZH
		obsoleteAICommandParmsStorage.doXfer( xfer );
#endif

	if( version < 2 )
	{
#ifdef OG
		AICommandParmsStorage obsolete;
		obsolete.doXfer(xfer);

#endif
#ifdef ZH
			obsoleteAICommandParmsStorage.doXfer(xfer);
		}

		//Initialize unit to able to move.
		m_state = READY_TO_MOVE;
#endif
	}

}  // end xfer

// ------------------------------------------------------------------------------------------------
/** Load post process */
// ------------------------------------------------------------------------------------------------
void DeployStyleAIUpdate::loadPostProcess( void )
{
 // extend base class
	AIUpdateInterface::loadPostProcess();
}  // end loadPostProcess

