// ============================================================================
//  rbNativeObjects.h
// ============================================================================
#pragma once



// ============================================================================
//  rbTokenStream
// ============================================================================
/*class rbTokenStream : public vector<rbToken*>
{
public:
    rbTokenStream(){}

public:
    rbToken* Add( rbToken* token ) { push_back(token); return token; }
};*/

class rbToken;

typedef vector<rbToken*> rbTokenStream;

enum ETokenFlags
{
	TF_SkipPostOutput	= 0x0001
};


// [\s]*([aA-zZ]+_)([aA-zZ]+)(To[aA-zZ]+)[\s]*= (0x[0-9A-F]+),([\s\S]*)
// case \4: return wxT("\2");


enum ECastToken
{
	CST_RotatorToVector		= 0x39,
	CST_ByteToInt			= 0x3A,
	CST_ByteToBool			= 0x3B,
	CST_ByteToFloat			= 0x3C,
	CST_IntToByte			= 0x3D,
	CST_IntToBool			= 0x3E,
	CST_IntToFloat			= 0x3F,
	CST_BoolToByte			= 0x40,
	CST_BoolToInt			= 0x41,
	CST_BoolToFloat			= 0x42,
	CST_FloatToByte			= 0x43,
	CST_FloatToInt			= 0x44,
	CST_FloatToBool			= 0x45,
	//
	CST_ObjectToBool		= 0x47,
	CST_NameToBool			= 0x48,
	CST_StringToByte		= 0x49,
	CST_StringToInt			= 0x4A,
	CST_StringToBool		= 0x4B,
	CST_StringToFloat		= 0x4C,
	CST_StringToVector		= 0x4D,
	CST_StringToRotator		= 0x4E,
	CST_VectorToBool		= 0x4F,
	CST_VectorToRotator		= 0x50,
	CST_RotatorToBool		= 0x51,
	CST_ByteToString		= 0x52,
	CST_IntToString			= 0x53,
	CST_BoolToString		= 0x54,
	CST_FloatToString		= 0x55,
	CST_ObjectToString		= 0x56,
	CST_NameToString		= 0x57,
	CST_VectorToString		= 0x58,
	CST_RotatorToString		= 0x59,
	CST_Max					= 0xFF,
};

wxString GetCastTokenString( byte t );


// ============================================================================
//  rbToken
// ============================================================================
class rbToken : public rbTokenStream
{
public:
	rbToken( rbToken* parent=NULL, word id=-1, const wxString& name=wxEmptyString, const wxString& output=wxEmptyString ) 
		: Parent(parent), Id(id), Name(name), Output(output)
	{
		Flags = 0;
		ExtraData = 0;
	}

	wxString ToString()
	{
		return wxString::Format(wxT("[%.4hx][%s] [%s]"),Id, PF_SAFESTR(Name), PF_SAFESTR(Output) );
	}

	rbToken* AddChild( rbToken* t )
	{
		t->Parent = this;
		push_back(t);
		return t;
	}

	rbToken* AddChild( word id, const wxString& name=wxEmptyString, const wxString& output=wxEmptyString )
	{
		return AddChild( new rbToken(this,id,name,output) );
	}

	rbToken* GetChild( dword idx )
	{
		return at(idx);
	}

	wxString GetChildOutput( dword idx )
	{
		return GetChild(idx)->Output;
	}

	void SetFlag( dword f )
	{
		Flags |= f;
	}


public:
	rbToken* Parent;
	word Id;
	wxString Name;
	wxString Output;
	dword Flags;
	dword ExtraData;
};



// ============================================================================
//  rbOutputModifier
// ============================================================================
class rbOutputModifier
{
public:
	rbOutputModifier::rbOutputModifier(dword offset) : Offset(offset), Next(NULL)
	{
	}

	virtual rbOutputModifier::~rbOutputModifier()
	{
	}

public:
	dword Offset;
	rbOutputModifier* Next;
};


// ============================================================================
//  rbScheduledOutput
// ============================================================================
struct rbScheduledOutput
{
	rbScheduledOutput(dword offset, bool popindent, const wxString& text) : Offset(offset), bPopIndent(popindent), Text(text)
	{
	}
	dword Offset;
	bool bPopIndent;
	wxString Text;
};


// ============================================================================
//  rbBytecode
// ============================================================================
class rbBytecode
{
public:
	rbBytecode::rbBytecode( rbPackage& package, upArchive& archive, wxFileOffset limit, int fidx ) 
		: Pkg(package), Arc(archive), Limit(limit), FuncIdx(fidx)
	{
		Start = Arc.Tell();
		IdExtended = 0x60;
		IdFirst = 0x70;
		Indent=0;
		bQuiet=false;
		Root = NULL;
		ScriptText.Alloc(2048);
		IndentLevel = 0;
		CurrentByte = 0;
	}

    void Parse();
	
	dword LoadToken();

    void ParseCode(dword token);
    void ParseFunc(dword token);

	rbToken* GetStack();
	rbToken* PushStack( rbToken* t );
	rbToken* PopStack();

	//rbToken* AddToken( word token, const wxString& name=wxEmptyString, const wxString& output=wxEmptyString );
	rbToken* AddToken( rbToken* t );

	wxString GetPrefix();
	wxString GetIndent();

	void PrintOutput( const wxString& format, ... );
	void TrimOptionalOutput();
	void ParseAndOutputFunction( wxString& output );

	bool NeedsOwnLine( rbToken* t );
	bool NeedsSemicolon( rbToken* t );
	bool NeedsNewLine( rbToken* t );

	void PrintPreOutput( rbToken* t );
	void PrintPostOutput( rbToken* t );


	dword GetCodeOffset();

	void ApplyScheduled( dword offset );
	void AddScheduled( dword offset, bool bRelative, bool bPopIndent, const wxChar* format, ... );

	const wxChar* GetOutputIndent();
	void PushIndent();
	void PopIndent();

	void CheckBracket(dword offset);
	void PushBracket(dword offset);
	void PopBracket();
	void OutputLeftBracket();
	void OutputRightBracket();

	rbToken* GetLastToken();


public:
	rbPackage& Pkg;
	upArchive& Arc;
	wxFileOffset Start;
	wxFileOffset Limit;

public:
    dword IdExtended;
    dword IdFirst;

public:
	int CurrentByte;
	bool bQuiet;
	int Indent;
	int FuncIdx;
	int IndentLevel;
	wxString PrefixStr;
	wxString IndentStr;
	wxString OutputIndent;
	rbTokenStream TokenStack;
	wxString ScriptText;
	rbToken* Root;
	rbOutputModifier* Modifiers;
	vector<rbScheduledOutput> Scheduled;
	vector<dword> EndBracket;
};


// ============================================================================

class rbPushStack
{
public:
	rbPushStack::rbPushStack( rbBytecode& bc ) : Bytecode(bc)
	{
		Bytecode.PushStack(Bytecode.GetStack()->back());
	}

	rbPushStack::~rbPushStack()
	{
		Bytecode.PopStack();
	}

private:
	rbBytecode& Bytecode;
};



// ============================================================================

struct rbIndenter
{
	dword& Var;

	rbIndenter::rbIndenter(dword& var) : Var(var)
	{
		++Var;
	}

	rbIndenter::~rbIndenter()
	{
		--Var;
	}
};

// ============================================================================
//  EOF
// ============================================================================