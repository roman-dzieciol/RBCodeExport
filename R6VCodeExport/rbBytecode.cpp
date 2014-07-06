// ============================================================================
//  rbCodeExport.cpp
// ============================================================================

#include "stdafx.h"
#include "rbBasicTypes.h"
#include "rbDebug.h"
#include "rbArchives.h"
#include "rbComplexTypes.h"
#include "rbNativeObjects.h"
#include "rbBytecode.h"



// ============================================================================

dword rbBytecode::GetCodeOffset()
{
	return Arc.Tell() - Start;
}

wxString rbBytecode::GetPrefix()
{
	PrefixStr = wxString::Format(wxT("[%.4x][%.4x]"),CurrentByte,GetStack()->Id);
	PrefixStr.Append('\t',Indent);
	return PrefixStr;
}

wxString rbBytecode::GetIndent()
{
	IndentStr.Clear();
	IndentStr.Append('\t',Indent);
	return IndentStr;
}



// ============================================================================
//  Token stack
// ============================================================================
rbToken* rbBytecode::GetStack()
{
	return TokenStack.back();
}

rbToken* rbBytecode::PushStack( rbToken* t )
{
	++Indent;
	TokenStack.push_back(t);
	return t;
}

rbToken* rbBytecode::PopStack()
{
	--Indent;
	rbToken* t = TokenStack.back();
	TokenStack.pop_back();
	return t;
}

// ============================================================================
//  Delim stack
// ============================================================================
#if 0
dword rbBytecode::GetDelim()
{
	return DelimStack.back();
}

bool rbBytecode::HasDelim()
{
	return !DelimStack.empty();
}

dword rbBytecode::PushDelim( dword token )
{
	DelimStack.push_back(token);
	return token;
}

dword rbBytecode::PopDelim()
{
	dword token = DelimStack.back();
	DelimStack.pop_back();
	return token;
}
#endif


// ============================================================================
//  Parsing
// ============================================================================



/*rbToken* rbBytecode::AddToken( word token, const wxString& name, const wxString& output )
{
	rbToken* t = GetStack()->AddChild(token,name,output);
	wxLogMessage( wxString::Format(wxT("%sADD %s"),GetPrefix(),PF_SAFESTR(t->ToString())));
	return t;
}*/

rbToken* rbBytecode::AddToken( rbToken* t )
{
	GetStack()->AddChild(t);
	wxLogMessage( wxString::Format(wxT("%sADD %s"),GetPrefix(),PF_SAFESTR(t->ToString())));
	return t;
}

void rbBytecode::Parse()
{
	wxLogMessage(wxT(""));

	// Initialize stack
	// parse until end-script marker
	Root = new rbToken(NULL,-1,wxT("ROOT"),wxT("ROOT"));
	PushIndent();
	PrintOutput(wxT("{\n"));
	PushStack(Root);	
	while( true )
	{
		dword token = LoadToken();
		if( GetCodeOffset() == Limit )
			break;
	}
	PopStack();
	PopIndent();
	//ScriptText.RemoveLast(); // remove tab
	PrintOutput( wxT("}\n") );


}

dword rbBytecode::LoadToken()
{
	CurrentByte = GetCodeOffset();
	ApplyScheduled( CurrentByte );
	CheckBracket( CurrentByte );

	byte b;
	Arc << b;
	dword token = b;

	if( token >= IdExtended )
	{
		if( (token & 0xF0) == IdExtended )
		{
			byte token2;
			Arc << token2;
			token = ((token-IdExtended)<<8) + token2;
		}

		ParseFunc(token);
	}
	else
	{
		ParseCode(token);
	}
	return token;
}


rbToken* rbBytecode::GetLastToken()
{
	rbToken* t = GetStack();
	if( t && !t->empty() )
	{
		return t->back();
	}
	return t;
}





// ============================================================================
//  EOF
// ============================================================================