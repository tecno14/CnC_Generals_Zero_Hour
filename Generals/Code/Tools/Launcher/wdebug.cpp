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

#include <stdlib.h>
#include "wdebug.h"
#include "streamer.h"
#include "odevice.h"


static MsgManager         *msg_manager=NULL;

static int                debug_enabled=0;
#ifdef OG
static std::ostream           *debug_ostream=NULL;
#endif // OG
#ifdef ZH
static ostream           *debug_ostream=NULL;
#endif // ZH
static Streamer           debug_streamer;

static int                info_enabled=0;
#ifdef OG
static std::ostream           *info_ostream=NULL;
#endif // OG
#ifdef ZH
static ostream           *info_ostream=NULL;
#endif // ZH
static Streamer           info_streamer;

static int                warn_enabled=0;
#ifdef OG
static std::ostream           *warn_ostream=NULL;
#endif // OG
#ifdef ZH
static ostream           *warn_ostream=NULL;
#endif // ZH
static Streamer           warn_streamer;

static int                error_enabled=0;
#ifdef OG
static std::ostream           *error_ostream=NULL;  
#endif // OG
#ifdef ZH
static ostream           *error_ostream=NULL;  
#endif // ZH
static Streamer           error_streamer;



int MsgManager::setAllStreams(OutputDevice *device)
{
  if (device==NULL)
    return(1);


  debug_streamer.setOutputDevice(device);
  delete(debug_ostream);
#ifdef OG
  debug_ostream=new std::ostream(&debug_streamer);
#endif // OG
#ifdef ZH
  debug_ostream=new ostream(&debug_streamer);
#endif // ZH

  info_streamer.setOutputDevice(device);
  delete(info_ostream);
#ifdef OG
  info_ostream=new std::ostream(&info_streamer);
#endif // OG
#ifdef ZH
  info_ostream=new ostream(&info_streamer);
#endif // ZH

  warn_streamer.setOutputDevice(device);
  delete(warn_ostream);
#ifdef OG
  warn_ostream=new std::ostream(&warn_streamer);
#endif // OG
#ifdef ZH
  warn_ostream=new ostream(&warn_streamer);
#endif // ZH

  error_streamer.setOutputDevice(device);
  delete(error_ostream);
#ifdef OG
  error_ostream=new std::ostream(&error_streamer);
#endif // OG
#ifdef ZH
  error_ostream=new ostream(&error_streamer);
#endif // ZH

  return(0);
}


int MsgManager::setDebugStream(OutputDevice *device)
{
  if (device==NULL)
    return(1);

 
  debug_streamer.setOutputDevice(device);
  delete(debug_ostream);
#ifdef OG
  debug_ostream=new std::ostream(&debug_streamer);
#endif // OG
#ifdef ZH
  debug_ostream=new ostream(&debug_streamer);
#endif // ZH

  return(0);
}

int MsgManager::setInfoStream(OutputDevice *device)
{
  if (device==NULL)
    return(1);


  info_streamer.setOutputDevice(device);
  delete(info_ostream);
#ifdef OG
  info_ostream=new std::ostream(&info_streamer);
#endif // OG
#ifdef ZH
  info_ostream=new ostream(&info_streamer);
#endif // ZH

  return(0);
}

int MsgManager::setWarnStream(OutputDevice *device)
{
  if (device==NULL)
    return(1);


  warn_streamer.setOutputDevice(device);
  delete(warn_ostream);
#ifdef OG
  warn_ostream=new std::ostream(&warn_streamer);
#endif // OG
#ifdef ZH
  warn_ostream=new ostream(&warn_streamer);
#endif // ZH

  return(0);
}

int MsgManager::setErrorStream(OutputDevice *device)
{
  if (device==NULL)
    return(1);


  error_streamer.setOutputDevice(device);
  delete(error_ostream);
#ifdef OG
  error_ostream=new std::ostream(&error_streamer);
#endif // OG
#ifdef ZH
  error_ostream=new ostream(&error_streamer);
#endif // ZH

  return(0);
}



#ifdef OG
std::ostream *MsgManager::debugStream(void)
#endif // OG
#ifdef ZH
ostream *MsgManager::debugStream(void)
#endif // ZH
{
  return(debug_ostream);
}   

#ifdef OG
std::ostream *MsgManager::infoStream(void)
#endif // OG
#ifdef ZH
ostream *MsgManager::infoStream(void)
#endif // ZH
{
  return(info_ostream);
}   

#ifdef OG
std::ostream *MsgManager::warnStream(void)
#endif // OG
#ifdef ZH
ostream *MsgManager::warnStream(void)
#endif // ZH
{
  return(warn_ostream);
}

#ifdef OG
std::ostream *MsgManager::errorStream(void)
#endif // OG
#ifdef ZH
ostream *MsgManager::errorStream(void)
#endif // ZH
{
  return(error_ostream);
}   
