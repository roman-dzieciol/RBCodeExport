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


void rbBytecode::PrintOutput( const wxString& format, ... )
{
	va_list args;
	va_start(args, format);
	ScriptText += wxString::FormatV(format,args);
}

void rbBytecode::TrimOptionalOutput()
{
	ScriptText.Trim(true);
	while( ScriptText.Last() == wxT(',') )
	{
		ScriptText.RemoveLast();
		ScriptText.Trim(true);
	}
}

void rbBytecode::ParseAndOutputFunction( wxString& output )
{
	output += wxT("(");
	PrintOutput(output);
	for( dword x=0; x!=0xFF; ++x )
	{
		PrintOutput( x == 0 ? wxT(" ") : wxT(", ") );
		if( LoadToken() == 0x16 )
		{
			TrimOptionalOutput();
			break;
		}
	}
	PrintOutput(wxT(" )"));
}


bool rbBytecode::NeedsOwnLine( rbToken* t )
{
	return false;
}

bool rbBytecode::NeedsSemicolon( rbToken* t )
{
	return false;
}

bool rbBytecode::NeedsNewLine( rbToken* t )
{
	return false;
}

const wxChar* rbBytecode::GetOutputIndent()
{
	OutputIndent = wxString::Format( wxT("/*%.4x*/"), CurrentByte );
	OutputIndent.Append(wxT('\t'),IndentLevel);
	return OutputIndent.c_str();
}


void rbBytecode::PushIndent()
{
	++IndentLevel;
}

void rbBytecode::PopIndent()
{
	--IndentLevel;
}


void rbBytecode::PushBracket(dword offset)
{
	EndBracket.push_back(offset);
}

void rbBytecode::PopBracket()
{
	EndBracket.pop_back();
}

void rbBytecode::OutputLeftBracket()
{
	PrintOutput( wxT("%s{\n"), GetOutputIndent() );
}

void rbBytecode::OutputRightBracket()
{
	PrintOutput( wxT("%s}\n"), GetOutputIndent() );
}

void rbBytecode::CheckBracket(dword offset)
{
	if( !EndBracket.empty() )
	{
		dword b = EndBracket.back();
		if( b == offset )
		{
			PopIndent();
			OutputRightBracket();
			PopBracket();
			
			rbToken* last = GetLastToken();
			if( last->Id == 0x06 && last->ExtraData > offset )
			{
				PrintOutput( wxT("%selse\n"), GetOutputIndent() );
				OutputLeftBracket();
				PushIndent();
				PushBracket( last->ExtraData );
			}
			else
			{
				PrintOutput( wxT("%s\n"), GetOutputIndent() );
			}

			CheckBracket(offset);
		}
		else if( b < offset )
		{
			PopIndent();
			PopBracket();
			//throw upexception(wxString::Format( wxT("missed scheduled output %d < %d"), b, offset ));
		}
		else
		{
		}
	}
}


void rbBytecode::ApplyScheduled( dword offset )
{
	/*if( !Scheduled.empty() )
	{
		rbScheduledOutput& s = Scheduled.back();
		if( s.Offset == offset )
		{
			if( s.bPopIndent )
			{
				PopIndent();
				//ScriptText.RemoveLast(); // remove one tab char
			}

			PrintOutput(s.Text);
			Scheduled.pop_back();
			ApplyScheduled(offset);
		}
		else if( s.Offset < offset )
		{
			throw upexception(wxString::Format( wxT("missed scheduled output %d < %d"), s.Offset, offset ));
		}
		else
		{
		}
	}*/
}

void rbBytecode::AddScheduled( dword offset, bool bRelative, bool bPopIndent, const wxChar* format, ... )
{
/*	va_list args;
	va_start(args, format);
	if( bRelative )
		offset += GetCodeOffset();
	Scheduled.push_back( rbScheduledOutput(offset,bPopIndent,wxString::FormatV(format,args)) );*/
}

void rbBytecode::PrintPreOutput( rbToken* t )
{

	switch(t->Id )
	{
		case 0x0B:	// EX_Nothing 
			return;

		case 0x53:	// EX_End 
			return;

		default:
		{
	
		}
	}

	if( GetStack() == Root )
	{
		PrintOutput( wxT("%s"), GetOutputIndent() );
	}
}

void rbBytecode::PrintPostOutput( rbToken* t )
{
	if( rbHasFlag(t->Flags,TF_SkipPostOutput) )
	{
		return;
	}

	/*switch(t->Id )
	{
		case 0x0B:	// EX_Nothing
		{
			break;
		}

		case 0x07:	// EX_JumpIfNot 
		{
			break;
		}

		case 0x53:	// EX_End 
		{
			break;
		}

		default:
		{
	
		}
	}*/

	if( GetStack() == Root )
	{
		PrintOutput( wxT(";\n") );
	}

	/*for( int i=0; i!=TokenStack.size(); ++i )
	{
	}*/
}



wxString GetCastTokenString( byte t )
{
	switch(t)
	{
		case 0x39: return wxT("Vector");
		case 0x3A: return wxT("Int");
		case 0x3B: return wxT("Bool");
		case 0x3C: return wxT("Float");
		case 0x3D: return wxT("Byte");
		case 0x3E: return wxT("Bool");
		case 0x3F: return wxT("Float");
		case 0x40: return wxT("Byte");
		case 0x41: return wxT("Int");
		case 0x42: return wxT("Float");
		case 0x43: return wxT("Byte");
		case 0x44: return wxT("Int");
		case 0x45: return wxT("Bool");
		case 0x47: return wxT("Bool");
		case 0x48: return wxT("Bool");
		case 0x49: return wxT("Byte");
		case 0x4A: return wxT("Int");
		case 0x4B: return wxT("Bool");
		case 0x4C: return wxT("Float");
		case 0x4D: return wxT("Vector");
		case 0x4E: return wxT("Rotator");
		case 0x4F: return wxT("Bool");
		case 0x50: return wxT("Rotator");
		case 0x51: return wxT("Bool");
		case 0x52: return wxT("String");
		case 0x53: return wxT("String");
		case 0x54: return wxT("String");
		case 0x55: return wxT("String");
		case 0x56: return wxT("String");
		case 0x57: return wxT("String");
		case 0x58: return wxT("String");
		case 0x59: return wxT("String");
		default: return wxT("Unknown");
	}
}

// ============================================================================
//  EOF
// ============================================================================