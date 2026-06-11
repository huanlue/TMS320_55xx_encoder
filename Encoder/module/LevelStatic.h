#if !defined(AFX_LEVELSTATIC1_H__71269B3D_F5F3_4445_A534_9C5CA2B3F446__INCLUDED_)
#define AFX_LEVELSTATIC1_H__71269B3D_F5F3_4445_A534_9C5CA2B3F446__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LevelStatic1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLevelStatic window

class AFX_EXT_CLASS CLevelStatic : public CStatic
{
// Construction
public:
	CLevelStatic();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLevelStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLevelStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLevelStatic)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEVELSTATIC1_H__71269B3D_F5F3_4445_A534_9C5CA2B3F446__INCLUDED_)
