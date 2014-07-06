// ============================================================================
//  rbCodeExport.cpp
// ============================================================================

#include "stdafx.h"
#include "rbBasicTypes.h"
#include "rbDebug.h"
#include "rbArchives.h"
#include "rbComplexTypes.h"
#include "rbNativeObjects.h"

// ============================================================================
//  rbsFunction
// ============================================================================
void operator << ( upArchive& A, rbsFunction& D )
{
	UP_BYTE_MARKER(rbsFunction,A,A.Tell(),BI_Object);

	A.Serialize(&D.unk1[0],sizeof(D.unk1));

	A << D.scriptsize;
	if( D.script )
	{
		delete []D.script;
		D.script = 0;
	}
	D.script = new byte[D.scriptsize];
	A.Serialize(D.script,D.scriptsize);
	
	A << D.fidx;
	A << D.prec;
	A << D.flags;

	if( rbHasFlag(D.flags,0x40) )
		A << D.netstuff;
	else
		D.netstuff = 0;

	A << D.fname;
	A << D.unk2;
}


// ============================================================================
//  EOF
// ============================================================================