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

// DlgProxy.cpp : implementation file
//

#include "stdafx.h"
#ifdef OG
#include "noxstring.h"
#endif
#ifdef ZH
#include "Babylon.h"
#endif
#include "DlgProxy.h"
#ifdef OG
#include "noxstringDlg.h"
#endif
#ifdef ZH
#include "BabylonDlg.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
#ifdef OG
// CNoxstringDlgAutoProxy
#endif
#ifdef ZH
// CBabylonDlgAutoProxy
#endif

#ifdef OG
IMPLEMENT_DYNCREATE(CNoxstringDlgAutoProxy, CCmdTarget)
#endif
#ifdef ZH
IMPLEMENT_DYNCREATE(CBabylonDlgAutoProxy, CCmdTarget)
#endif

#ifdef OG
CNoxstringDlgAutoProxy::CNoxstringDlgAutoProxy()
#endif
#ifdef ZH
CBabylonDlgAutoProxy::CBabylonDlgAutoProxy()
#endif
{
	EnableAutomation();
	
	// To keep the application running as long as an automation 
	//	object is active, the constructor calls AfxOleLockApp.
	AfxOleLockApp();

	// Get access to the dialog through the application's
	//  main window pointer.  Set the proxy's internal pointer
	//  to point to the dialog, and set the dialog's back pointer to
	//  this proxy.
	ASSERT (AfxGetApp()->m_pMainWnd != NULL);
	ASSERT_VALID (AfxGetApp()->m_pMainWnd);
#ifdef OG
	ASSERT_KINDOF(CNoxstringDlg, AfxGetApp()->m_pMainWnd);
	m_pDialog = (CNoxstringDlg*) AfxGetApp()->m_pMainWnd;
#endif
#ifdef ZH
	ASSERT_KINDOF(CBabylonDlg, AfxGetApp()->m_pMainWnd);
	m_pDialog = (CBabylonDlg*) AfxGetApp()->m_pMainWnd;
#endif
	m_pDialog->m_pAutoProxy = this;
}

#ifdef OG
CNoxstringDlgAutoProxy::~CNoxstringDlgAutoProxy()
#endif
#ifdef ZH
CBabylonDlgAutoProxy::~CBabylonDlgAutoProxy()
#endif
{
	// To terminate the application when all objects created with
	// 	with automation, the destructor calls AfxOleUnlockApp.
	//  Among other things, this will destroy the main dialog
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

#ifdef OG
void CNoxstringDlgAutoProxy::OnFinalRelease()
#endif
#ifdef ZH
void CBabylonDlgAutoProxy::OnFinalRelease()
#endif
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}

#ifdef OG
BEGIN_MESSAGE_MAP(CNoxstringDlgAutoProxy, CCmdTarget)
	//{{AFX_MSG_MAP(CNoxstringDlgAutoProxy)
#endif
#ifdef ZH
BEGIN_MESSAGE_MAP(CBabylonDlgAutoProxy, CCmdTarget)
	//{{AFX_MSG_MAP(CBabylonDlgAutoProxy)
#endif
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#ifdef OG
BEGIN_DISPATCH_MAP(CNoxstringDlgAutoProxy, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CNoxstringDlgAutoProxy)
#endif
#ifdef ZH
BEGIN_DISPATCH_MAP(CBabylonDlgAutoProxy, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CBabylonDlgAutoProxy)
#endif
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

#ifdef OG
// Note: we add support for IID_INoxstring to support typesafe binding
#endif
#ifdef ZH
// Note: we add support for IID_IBabylon to support typesafe binding
#endif
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {2BF31248-3BA1-11D3-B9DA-006097B90D93}
#ifdef OG
static const IID IID_INoxstring =
#endif
#ifdef ZH
static const IID IID_IBabylon =
#endif
{ 0x2bf31248, 0x3ba1, 0x11d3, { 0xb9, 0xda, 0x0, 0x60, 0x97, 0xb9, 0xd, 0x93 } };

#ifdef OG
BEGIN_INTERFACE_MAP(CNoxstringDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CNoxstringDlgAutoProxy, IID_INoxstring, Dispatch)
#endif
#ifdef ZH
BEGIN_INTERFACE_MAP(CBabylonDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CBabylonDlgAutoProxy, IID_IBabylon, Dispatch)
#endif
END_INTERFACE_MAP()

// The IMPLEMENT_OLECREATE2 macro is defined in StdAfx.h of this project
// {2BF31246-3BA1-11D3-B9DA-006097B90D93}
#ifdef OG
IMPLEMENT_OLECREATE2(CNoxstringDlgAutoProxy, "Noxstring.Application", 0x2bf31246, 0x3ba1, 0x11d3, 0xb9, 0xda, 0x0, 0x60, 0x97, 0xb9, 0xd, 0x93)
#endif
#ifdef ZH
IMPLEMENT_OLECREATE2(CBabylonDlgAutoProxy, "Babylon.Application", 0x2bf31246, 0x3ba1, 0x11d3, 0xb9, 0xda, 0x0, 0x60, 0x97, 0xb9, 0xd, 0x93)
#endif

/////////////////////////////////////////////////////////////////////////////
#ifdef OG
// CNoxstringDlgAutoProxy message handlers
#endif
#ifdef ZH
// CBabylonDlgAutoProxy message handlers
#endif
