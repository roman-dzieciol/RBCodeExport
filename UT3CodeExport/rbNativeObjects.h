// ============================================================================
//  rbNativeObjects.h
// ============================================================================
#pragma once


// ============================================================================
//  rbsFunction
// ============================================================================
class rbsFunction
{
public:
	rbsFunction::rbsFunction() : script(0) 
	{
	}

    rbsFunction( rbsFunction& c )
    :   scriptsize(c.scriptsize)
    ,   script(c.script)
    ,   fidx(c.fidx)
    ,   prec(c.prec)
    ,   flags(c.flags)
    ,   fname(c.fname)
    //,   unk2(c.unk2)
    {
		for( int i=0; i!=sizeof(unk1)/sizeof(dword); ++i )
		{
			unk1[i] = c.unk1[i];
		}
		c.script = NULL;
    }

	rbsFunction::~rbsFunction() 
	{ 
		if( script )
		{
			delete []script;
			script = 0;
		}
	}

public:
	friend void operator << ( upArchive& A, rbsFunction& D );

public:
	dword unk1[8];
	dword scriptsize;
	byte* script;
	word fidx;
	byte prec;
	dword flags;
	//word netstuff;
	dword fname;
	//dword unk2;
};

// ============================================================================
//  EOF
// ============================================================================