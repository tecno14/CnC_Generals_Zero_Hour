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

// DlgProxy.h : header file
//

#if !defined(AFX_DLGPROXY_H__2BF3124F_3BA1_11D3_B9DA_006097B90D93__INCLUDED_)
#define AFX_DLGPROXY_H__2BF3124F_3BA1_11D3_B9DA_006097B90D93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef OG
class CNoxstringDlg;
#endif
#ifdef ZH
class CBabylonDlg;
#endif

/////////////////////////////////////////////////////////////////////////////
#ifdef OG
// CNoxstringDlgAutoProxy command target
#endif
#ifdef ZH
// CBabylonDlgAutoProxy command target
#endif

#ifdef OG
class CNoxstringDlgAutoProxy : public CCmdTarget
#endif
#ifdef ZH
class CBabylonDlgAutoProxy : public CCmdTarget
#endif
{
#ifdef OG
	DECLARE_DYNCREATE(CNoxstringDlgAutoProxy)
#endif
#ifdef ZH
	DECLARE_DYNCREATE(CBabylonDlgAutoProxy)
#endif

#ifdef OG
	CNoxstringDlgAutoProxy();           // protected constructor used by dynamic creation
#endif
#ifdef ZH
	CBabylonDlgAutoProxy();           // protected constructor used by dynamic creation
#endif

// Attributes
public:
#ifdef OG
	CNoxstringDlg* m_pDialog;
#endif
#ifdef ZH
	CBabylonDlg* m_pDialog;
#endif

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
#ifdef OG
	//{{AFX_VIRTUAL(CNoxstringDlgAutoProxy)
#endif
#ifdef ZH
	//{{AFX_VIRTUAL(CBabylonDlgAutoProxy)
#endif
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
#ifdef OG
	virtual ~CNoxstringDlgAutoProxy();
#endif
#ifdef ZH
	virtual ~CBabylonDlgAutoProxy();
#endif

	// Generated message map functions
#ifdef OG
	//{{AFX_MSG(CNoxstringDlgAutoProxy)
#endif
#ifdef ZH
	//{{AFX_MSG(CBabylonDlgAutoProxy)
#endif
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
#ifdef OG
	DECLARE_OLECREATE(CNoxstringDlgAutoProxy)
#endif
#ifdef ZH
	DECLARE_OLECREATE(CBabylonDlgAutoProxy)
#endif

	// Generated OLE dispatch map functions
#ifdef OG
	//{{AFX_DISPATCH(CNoxstringDlgAutoProxy)
#endif
#ifdef ZH
	//{{AFX_DISPATCH(CBabylonDlgAutoProxy)
#endif
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROXY_H__2BF3124F_3BA1_11D3_B9DA_006097B90D93__INCLUDED_)
