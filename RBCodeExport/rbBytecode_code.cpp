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

#define RB_ADD_TOKEN				AddToken(t); if( !bStack ) { bStack = true; PushStack(GetStack()->back()); }
#define RB_LOAD_TOKEN				LoadToken();
//#define RB_LOAD_TOKEN_UNTIL(delim)	LoadUntil(delim)
#define RB_ADD_OUTPUT(token,text)	token->Output = text; AddLine(token->Output);	
#define RB_TOKEN_NAME(text)			name = wxT(text)
#define RB_TOKEN_DESC(text)			desc = wxT(text)
#define RB_LOAD_DATA(type,name)		type name; Arc << name;

//#define RB_PUSH_STACK	PushStack(GetStack()->back())
//#define RB_POP_STACK	PopStack()
//#define RB_STACK_PUSHER	rbPushStack push(*this);

void rbBytecode::ParseCode( dword token )
{
	rbToken* t = new rbToken(NULL,token);
	wxString desc;
	wxString& name = t->Name;
	wxString& output = t->Output;
	bool bStack = false;

	wxLogMessage( wxT("%sCODE %x"), GetPrefix(), dword(token) );

	try{

	PrintPreOutput(t);

	switch( token )
	{
	case 0x00:	// EX_LocalVariable 
	{
		RB_TOKEN_NAME("EX_LocalVariable");
		RB_TOKEN_DESC("A local variable.");

		RB_LOAD_DATA(dword,idx);
		output = wxString::Format( wxT("%s"), PF_SAFESTR(Pkg.GetObjectName(idx)) );
		RB_ADD_TOKEN;
		PrintOutput( output );
		break;
	}

	case 0x01:	// EX_InstanceVariable 
	{
		RB_TOKEN_NAME("EX_InstanceVariable");
		RB_TOKEN_DESC("An object variable.");

		RB_LOAD_DATA(dword,idx);
		output = wxString::Format( wxT("%s"), PF_SAFESTR(Pkg.GetObjectName(idx)) );
		RB_ADD_TOKEN;
		PrintOutput( output );
		break;
	}

	case 0x02:	// EX_DefaultVariable 
	{
		RB_TOKEN_NAME("EX_DefaultVariable");
		RB_TOKEN_DESC("Default variable for a concrete object.");

		RB_LOAD_DATA(dword,idx);
		output = wxString::Format( wxT("default.%s"), PF_SAFESTR(Pkg.GetObjectName(idx)) );
		RB_ADD_TOKEN;
		PrintOutput( output );
		break;
	}


	/*	bDone = true;
		for( int i=0; i!=t->size(); ++i )
		{
			PrintOutput( wxT("return") );
			rbToken* nt = t->GetChild(i);
			OutputToken(nt);
			PrintOutput( wxT(";\n") );
		}*/


	case 0x04:	// EX_Return 
	{
		RB_TOKEN_NAME("EX_Return");
		RB_TOKEN_DESC("Return from function.");
		RB_ADD_TOKEN;
		PrintOutput(wxT("return "));
		RB_LOAD_TOKEN;
		TrimOptionalOutput();
		break;
	}


	case 0x05:	// EX_Switch 
	{
		RB_TOKEN_NAME("EX_Switch");
		RB_TOKEN_DESC("Switch.");
		t->SetFlag(TF_SkipPostOutput);

		RB_LOAD_DATA(byte,size);

		output = wxString::Format(wxT("switch( "));
		PrintOutput( output );
		RB_ADD_TOKEN;
		RB_LOAD_TOKEN;
		PrintOutput( wxT(" )\n") );
		OutputLeftBracket();
		//PushBracket(0xFFFF);
		//PushIndent();
		break;
	}


	case 0x06:	// EX_Jump 
	{
		RB_TOKEN_NAME("EX_Jump");
		RB_TOKEN_DESC("Goto a local address in code.");

		RB_LOAD_DATA(word,offset);
		t->ExtraData = offset;
		output = wxString::Format(wxT("//goto 0x%.4x"),dword(offset));

		RB_ADD_TOKEN;
		PrintOutput( output );
		break;
	}


	case 0x07:	// EX_JumpIfNot 
	{
		RB_TOKEN_NAME("EX_JumpIfNot");
		RB_TOKEN_DESC("Goto if not expression.");
		t->SetFlag(TF_SkipPostOutput);

		RB_LOAD_DATA(word,offset);
		output = wxString::Format( wxT("if( ") );
		PrintOutput( output );

		RB_ADD_TOKEN;
		RB_LOAD_TOKEN;
		PrintOutput( wxT(" ) //else 0x%.4x\n"), dword(offset) );
		OutputLeftBracket();
		PushBracket(offset);
		PushIndent();
		break;
	}


	case 0x08:	// EX_Stop 
	{
		RB_TOKEN_NAME("EX_Stop");
		RB_TOKEN_DESC("Stop executing state code.");
		output = wxString::Format(wxT("stop"));
		RB_ADD_TOKEN;
		PrintOutput( output );
		break;
	}


	case 0x09:	// EX_Assert 
	{
		RB_TOKEN_NAME("EX_Assert");
		RB_TOKEN_DESC("Assertion.");

		RB_LOAD_DATA(word,value);
		RB_LOAD_DATA(byte,unk);
		output = wxString::Format( wxT("assert(") );
		PrintOutput( output );

		RB_ADD_TOKEN;
		RB_LOAD_TOKEN;
		PrintOutput( wxT(")") );
		break;
	}


	case 0x0A:	// EX_Case 
	{
		RB_TOKEN_NAME("EX_Case");
		RB_TOKEN_DESC("Case.");
		t->SetFlag(TF_SkipPostOutput);

		RB_LOAD_DATA(word,value);
		output = wxString::Format( wxT("case ") );
		PrintOutput( output );

		RB_ADD_TOKEN;
		if( value != 0xFFFF )
		{
			RB_LOAD_TOKEN;
			PrintOutput( wxT(":\n") );
		}
		else
		{
			PrintOutput( wxT("default:\n") );
		}
		break;
	}


	case 0x0B:	// EX_Nothing 
	{
		RB_TOKEN_NAME("EX_Nothing");
		RB_TOKEN_DESC("No operation.");
		t->SetFlag(TF_SkipPostOutput);
		RB_ADD_TOKEN;
		break;
	}


	case 0x0C:	// EX_LabelTable 
	{
		RB_TOKEN_NAME("EX_LabelTable");
		RB_TOKEN_DESC("Table of labels.");

		while( true )
		{
			RB_LOAD_DATA(dword,nidx);

			if( wxString(wxT("None")).IsSameAs(Pkg.GetNameString(nidx)) )
				break;

			RB_LOAD_DATA(dword,offset);
			PrintOutput( wxT("%s: //%u "),Pkg.GetNameString(nidx),offset );
		}
		RB_ADD_TOKEN;
		break;
	}


	case 0x0D:	// EX_GotoLabel 
	{
		RB_TOKEN_NAME("EX_GotoLabel");
		RB_TOKEN_DESC("Goto a label.");

		output = wxString::Format(wxT("Goto"));
		PrintOutput( output );
		RB_ADD_TOKEN;
		RB_LOAD_TOKEN;
		break;
	}


	case 0x0E:	// EX_EatString 
	{
		RB_TOKEN_NAME("EX_EatString");
		RB_TOKEN_DESC("Ignore a dynamic string.");

		RB_LOAD_DATA(dword,idx);

		output = wxString::Format(wxT("EatString %s"),PF_SAFESTR(Pkg.GetObjectName(idx)));
		RB_ADD_TOKEN;
		RB_LOAD_TOKEN;
		break;
	}


	case 0x0F:	// EX_Let 
	{
		RB_TOKEN_NAME("EX_Let");
		RB_TOKEN_DESC("Assign an arbitrary size value to a variable.");
		RB_ADD_TOKEN;
		RB_LOAD_TOKEN;
		PrintOutput(wxT(" = "));
		RB_LOAD_TOKEN;
		break;
	}


	case 0x10:	// EX_DynArrayElement 
	{
		RB_TOKEN_NAME("EX_DynArrayElement");
		RB_TOKEN_DESC("Dynamic array element.!!");
		RB_ADD_TOKEN;
		PrintOutput( wxT("[") );
		RB_LOAD_TOKEN;
		PrintOutput( wxT("]") );
		RB_LOAD_TOKEN;
		break;
	}


	case 0x11:	// EX_New 
	{
		RB_TOKEN_NAME("EX_New");
		RB_TOKEN_DESC("New object allocation.");
		RB_ADD_TOKEN;
		PrintOutput( wxT("new ") );
		RB_LOAD_TOKEN;
		RB_LOAD_TOKEN;
		RB_LOAD_TOKEN;
		RB_LOAD_TOKEN;
		break;
	}


	case 0x12:	// EX_ClassContext 
	{
		RB_TOKEN_NAME("EX_ClassContext");
		RB_TOKEN_DESC("Class default metaobject context.");
		RB_ADD_TOKEN;
		RB_LOAD_TOKEN;
		PrintOutput( wxT(" . ") );
		
		RB_LOAD_DATA(word,value2);
		RB_LOAD_DATA(byte,value3);
		
		RB_LOAD_TOKEN;
		break;;
	}


	case 0x13:	// EX_MetaCast 
	{
		RB_TOKEN_NAME("EX_MetaCast");
		RB_TOKEN_DESC("Metaclass cast.");

		RB_LOAD_DATA(dword,value);
		PrintOutput( wxT("EX_MetaCast ") );

		RB_ADD_TOKEN;
		RB_LOAD_TOKEN;
		break;
	}


	case 0x14:	// EX_LetBool 
	{
		RB_TOKEN_NAME("EX_LetBool");
		RB_TOKEN_DESC("Let boolean variable.");
		RB_ADD_TOKEN;
		RB_LOAD_TOKEN;
		PrintOutput(wxT(" = "));
		RB_LOAD_TOKEN;
		break;
	}

	case 0x15:	// ?
	{
		RB_TOKEN_NAME("EX_0x15");
		RB_TOKEN_DESC("");
		PrintOutput( wxT("0x15 ") );
		RB_ADD_TOKEN;
		break;
	}



/*	case 0x15:	// EX_LineNumber 
	{
		RB_TOKEN_NAME("EX_LineNumber");
		RB_TOKEN_DESC("Set current source code line number in stack frame.");
		break;
	}*/


	case 0x16:	// EX_EndFunctionParms 
	{
		RB_TOKEN_NAME("EX_EndFunctionParms");
		RB_TOKEN_DESC("End of function call parameters.");
		RB_ADD_TOKEN;
		break;
	}


	case 0x17:	// EX_Self 
	{
		RB_TOKEN_NAME("EX_Self");
		RB_TOKEN_DESC("Self object.");
		RB_ADD_TOKEN;
		PrintOutput( wxT("self") );
		break;
	}


	case 0x18:	// EX_Skip 
	{
		RB_TOKEN_NAME("EX_Skip");
		RB_TOKEN_DESC("Skippable expression.");

		RB_LOAD_DATA(word,value);

		RB_ADD_TOKEN;
		RB_LOAD_TOKEN;
		break;
	}


	case 0x19:	// EX_Context 
	{
		RB_TOKEN_NAME("EX_Context");
		RB_TOKEN_DESC("Call a function through an object context.");
		RB_ADD_TOKEN;
		RB_LOAD_TOKEN;
		PrintOutput(wxT("."));
		
		RB_LOAD_DATA(word,value2);
		RB_LOAD_DATA(byte,value3);
		
		RB_LOAD_TOKEN;
		break;
	}

	case 0x1A:	// EX_ArrayElement 
	{
		RB_TOKEN_NAME("EX_ArrayElement");
		RB_TOKEN_DESC("Array element.");
		RB_ADD_TOKEN;
		PrintOutput( wxT("[") );
		RB_LOAD_TOKEN;
		PrintOutput( wxT("]") );
		RB_LOAD_TOKEN;
		break;
	}

	case 0x1B:	// EX_VirtualFunction 
	{
		RB_TOKEN_NAME("EX_VirtualFunction");
		RB_TOKEN_DESC("A function call with parameters.");

		RB_LOAD_DATA(dword,value);
		RB_LOAD_DATA(dword,value2); // TODO :: what's this? super?
		output = wxString::Format( wxT("%s"), PF_SAFESTR(Pkg.GetNameString(value)) );

		RB_ADD_TOKEN;

		/*dword thisfunc = FuncIdx;
		if( value != FuncIdx && t->Parent && t->Parent->Id != 0x19 )
		{
			output = wxString::Format( wxT("Super(%s)."), PF_SAFESTR(Pkg.GetObjectName(Pkg.GetSuperClass(value))) ) + output;
		}*/

		ParseAndOutputFunction( output );
		break;
	}

	case 0x1C:	// EX_FinalFunction 
	{
		RB_TOKEN_NAME("EX_FinalFunction");
		RB_TOKEN_DESC("A prebound function call with parameters.");
		
		RB_LOAD_DATA(dword,value);
		output = wxString::Format( wxT("%s"), PF_SAFESTR(Pkg.GetObjectName(value)) );

		RB_ADD_TOKEN;

		dword thisfunc = FuncIdx;
		if( value != FuncIdx && t->Parent && t->Parent->Id != 0x19 )
		{
			output = wxString::Format( wxT("Super(%s)."), PF_SAFESTR(Pkg.GetObjectName(Pkg.GetSuperClass(value))) ) + output;
		}

		ParseAndOutputFunction( output );
		break;
	}


	case 0x1D:	// EX_IntConst 
	{
		RB_TOKEN_NAME("EX_IntConst");
		RB_TOKEN_DESC("Int constant.");
		
		RB_LOAD_DATA(dword,value);

		RB_ADD_TOKEN;
		output = wxString::Format( wxT("%d"), value );
		PrintOutput( output );
		break;
	}


	case 0x1E:	// EX_FloatConst 
	{
		RB_TOKEN_NAME("EX_FloatConst");
		RB_TOKEN_DESC("Floating point constant.");

		RB_LOAD_DATA(float,value);
		output = wxString::Format( wxT("%f"), value );
		RB_ADD_TOKEN;
		PrintOutput( output );
		break;
	}


	case 0x1F:	// EX_StringConst 
	{
		RB_TOKEN_NAME("EX_StringConst");
		RB_TOKEN_DESC("String constant.");

		RB_LOAD_DATA(rbASCIIZ,value);
		output = wxString::Format( wxT("\"%s\""), PF_SAFESTR(value) );
		RB_ADD_TOKEN;
		PrintOutput( output );
		break;
	}


	case 0x20:	// EX_ObjectConst 
	{
		RB_TOKEN_NAME("EX_ObjectConst");
		RB_TOKEN_DESC("An object constant.");

		RB_LOAD_DATA(dword,value);
		output = wxString::Format( wxT("%s'%s'"), PF_SAFESTR(Pkg.GetObjectName(value)), PF_SAFESTR(Pkg.GetObjectPath(value)) );
		RB_ADD_TOKEN;
		PrintOutput( output );
		break;
	}


	case 0x21:	// EX_NameConst 
	{
		RB_TOKEN_NAME("EX_NameConst");
		RB_TOKEN_DESC("A name constant.");

		RB_LOAD_DATA(dword,value);
		RB_LOAD_DATA(dword,value2);
		output = wxString::Format( wxT("'%s'"), PF_SAFESTR(Pkg.GetNameString(value)) );

		RB_ADD_TOKEN;
		PrintOutput( output );
		break;
	}


	case 0x22:	// EX_RotationConst 
	{
		RB_TOKEN_NAME("EX_RotationConst");
		RB_TOKEN_DESC("A rotation constant.");

		RB_LOAD_DATA(dword,value);
		RB_LOAD_DATA(dword,value2);
		RB_LOAD_DATA(dword,value3);
		output = wxString::Format( wxT("rot(%d,%d,%d)"), value,value2,value3 );

		RB_ADD_TOKEN;
		PrintOutput( output );
		break;
	}


	case 0x23:	// EX_VectorConst 
	{
		RB_TOKEN_NAME("EX_VectorConst");
		RB_TOKEN_DESC("A vector constant.");

		RB_LOAD_DATA(float,value);
		RB_LOAD_DATA(float,value2);
		RB_LOAD_DATA(float,value3);
		output = wxString::Format( wxT("vect(%f,%f,%f)"), value,value2,value3 );

		RB_ADD_TOKEN;
		PrintOutput( output );
		break;
	}


	case 0x24:	// EX_ByteConst 
	{
		RB_TOKEN_NAME("EX_ByteConst");
		RB_TOKEN_DESC("A byte constant.");

		RB_LOAD_DATA(byte,value2);
		output = wxString::Format( wxT("%d"), dword(value2) );

		RB_ADD_TOKEN;
		PrintOutput( output );
		break;
	}


	case 0x25:	// EX_IntZero 
	{
		RB_TOKEN_NAME("EX_IntZero");
		RB_TOKEN_DESC("Zero.");
		output = wxString::Format( wxT("0") );
		RB_ADD_TOKEN;
		PrintOutput( output );
		break;
	}


	case 0x26:	// EX_IntOne 
	{
		RB_TOKEN_NAME("EX_IntOne");
		RB_TOKEN_DESC("One.");
		output = wxString::Format( wxT("1") );
		RB_ADD_TOKEN;
		PrintOutput( output );
		break;
	}


	case 0x27:	// EX_True 
	{
		RB_TOKEN_NAME("EX_True");
		RB_TOKEN_DESC("Bool True.");
		output = wxString::Format( wxT("True") );
		RB_ADD_TOKEN;
		PrintOutput( output );
		break;
	}


	case 0x28:	// EX_False 
	{
		RB_TOKEN_NAME("EX_False");
		RB_TOKEN_DESC("Bool False.");
		output = wxString::Format( wxT("False") );
		RB_ADD_TOKEN;
		PrintOutput( output );
		break;
	}


	case 0x29:	// EX_NativeParm 
	{
		RB_TOKEN_NAME("EX_NativeParm");
		RB_TOKEN_DESC("Native function parameter offset.");

		RB_LOAD_DATA(dword,idx);

		output = Pkg.GetObjectName(idx);
		RB_ADD_TOKEN;
		break;
	}


	case 0x2A:	// EX_NoObject 
	{
		RB_TOKEN_NAME("EX_NoObject");
		RB_TOKEN_DESC("NoObject.");
		output = wxString::Format( wxT("None") );
		RB_ADD_TOKEN;
		PrintOutput( output );
		break;
	}


	case 0x2C:	// EX_IntConstByte 
	{
		RB_TOKEN_NAME("EX_IntConstByte");
		RB_TOKEN_DESC("Int constant that requires 1 byte.");

		RB_LOAD_DATA(byte,value);
		output = wxString::Format( wxT("%d"), dword(value) );

		RB_ADD_TOKEN;
		PrintOutput( output );
		break;
	}


	case 0x2D:	// EX_BoolVariable 
	{
		RB_TOKEN_NAME("EX_BoolVariable");
		RB_TOKEN_DESC("A bool variable which requires a bitmask.");
		RB_ADD_TOKEN;
		RB_LOAD_TOKEN;
		break;
	}


	case 0x2E:	// EX_DynamicCast 
	{
		RB_TOKEN_NAME("EX_DynamicCast");
		RB_TOKEN_DESC("Safe dynamic class casting.");

		RB_LOAD_DATA(dword,idx);
		output = wxString::Format( wxT("class<%s>("), PF_SAFESTR(Pkg.GetObjectName(idx)) );
		PrintOutput( output );

		RB_ADD_TOKEN;
		RB_LOAD_TOKEN;
		PrintOutput( wxT(")") );
		break;
	}


	case 0x2F:	// EX_Iterator 
	{
		RB_TOKEN_NAME("EX_Iterator");
		RB_TOKEN_DESC("Begin an iterator operation.");
		t->SetFlag(TF_SkipPostOutput);

		output = wxT("ForEach ");
		PrintOutput( output );

		RB_ADD_TOKEN;
		RB_LOAD_TOKEN;
		RB_LOAD_DATA(word,offset);

		PopStack();
		bStack = false;

		PrintOutput( wxT("\n") );
		OutputLeftBracket();
		PushBracket(0xFFFF);
		PushIndent();

		for( dword x=0; x!=0xFF; ++x )
		{
			if( LoadToken() == 0x31 )
			{
				break;
			}
		}

		PopIndent();
		PopBracket();
		OutputRightBracket();
		break;
	}


	case 0x30:	// EX_IteratorPop 
	{
		RB_TOKEN_NAME("EX_IteratorPop");
		RB_TOKEN_DESC("Pop an iterator level.");
		RB_ADD_TOKEN;
		break;
	}


	case 0x31:	// EX_IteratorNext 
	{
		RB_TOKEN_NAME("EX_IteratorNext");
		RB_TOKEN_DESC("Go to next iteration.");
		RB_ADD_TOKEN;
		break;
	}


	case 0x32:	// EX_StructCmpEq 
	{
		RB_TOKEN_NAME("EX_StructCmpEq");
		RB_TOKEN_DESC("Struct binary compare-for-equal.");

		RB_LOAD_DATA(dword,idx);

		RB_ADD_TOKEN;
		RB_LOAD_TOKEN;
		PrintOutput( wxT(" == ") );
		RB_LOAD_TOKEN;
		break;
		break;
	}


	case 0x33:	// EX_StructCmpNe 
	{
		RB_TOKEN_NAME("EX_StructCmpNe");
		RB_TOKEN_DESC("Struct binary compare-for-unequal.");

		RB_LOAD_DATA(dword,idx);

		RB_ADD_TOKEN;
		RB_LOAD_TOKEN;
		PrintOutput( wxT(" != ") );
		RB_LOAD_TOKEN;
		break;
	}


	case 0x34:	// EX_UnicodeStringConst 
	{
		RB_TOKEN_NAME("EX_UnicodeStringConst");
		RB_TOKEN_DESC("Unicode string constant.");

		RB_LOAD_DATA(rbUNICODEZ,value);

		RB_ADD_TOKEN;
		break;
	}

	case 0x35:	// EX_StructMember?
	{
		RB_TOKEN_NAME("EX_StructMember");
		RB_TOKEN_DESC("");

		RB_LOAD_DATA(dword,value);
		output = wxString::Format( wxT(".%s"), PF_SAFESTR(Pkg.GetObjectName(value)) );

		RB_ADD_TOKEN;
		RB_LOAD_TOKEN;
		PrintOutput( output );
		break;
	}

	case 0x36:	// EX_DynArrayLength 
	{
		RB_TOKEN_NAME("EX_DynArrayLength");
		RB_TOKEN_DESC("A dynamic array length for setting/getting");
		RB_ADD_TOKEN;
		RB_LOAD_TOKEN;
		PrintOutput( wxT(".Length") );
		break;
	}

	case 0x37:	// EX_GlobalFunction 
	{
		RB_TOKEN_NAME("EX_GlobalFunction");
		RB_TOKEN_DESC("Call non-state version of a function.");

		RB_LOAD_DATA(dword,value);
		RB_LOAD_DATA(dword,value2);
		output = wxString::Format( wxT("%s"), PF_SAFESTR(Pkg.GetNameString(value)) );

		RB_ADD_TOKEN;
		/*dword thisfunc = FuncIdx;
		if( value != FuncIdx && t->Parent && t->Parent->Id != 0x19 )
		{
			output = wxString::Format( wxT("Super(%s)."), PF_SAFESTR(Pkg.GetObjectName(Pkg.GetSuperClass(value))) ) + output;
		}*/

		ParseAndOutputFunction( output );
		break;
	}


	case 0x38:	// EX_PrimitiveCast ?
	{
		RB_TOKEN_NAME("0x38");
		RB_TOKEN_DESC("A casting operator for primitives which reads the type as the subsequent byte");

		RB_LOAD_DATA(byte,value);
		output = wxString::Format( wxT("%s("), PF_SAFESTR(GetCastTokenString(value)) );
		PrintOutput( output );

		RB_ADD_TOKEN;
		RB_LOAD_TOKEN;
		PrintOutput( wxT(")") );
		break;
	}


	case 0x39:	// EX_DynArrayInsert 
	{
		RB_TOKEN_NAME("EX_DynArrayInsert");
		RB_TOKEN_DESC("Inserts into a dynamic array");
		RB_ADD_TOKEN;
		RB_LOAD_TOKEN;
		PrintOutput( wxT(".Insert(") );
		RB_LOAD_TOKEN;
		PrintOutput( wxT(", ") );
		RB_LOAD_TOKEN;
		PrintOutput( wxT(")") );
		break;
	}

	case 0x3a:	// EX_3a
	{
		RB_TOKEN_NAME("EX_3a");
		RB_TOKEN_DESC("");

		RB_LOAD_DATA(dword,value);

		output = wxString::Format(wxT("EX_3a %d %s"),value,PF_SAFESTR(Pkg.GetObjectName(value)));
		PrintOutput( output );
		RB_ADD_TOKEN;
		break;
	}

	case 0x40:	// EX_DynArrayRemove 
	{
		RB_TOKEN_NAME("EX_DynArrayRemove");
		RB_TOKEN_DESC("Removes from a dynamic array");
		RB_ADD_TOKEN;
		RB_LOAD_TOKEN;
		PrintOutput( wxT(".Remove(") );
		RB_LOAD_TOKEN;
		PrintOutput( wxT(", ") );
		RB_LOAD_TOKEN;
		PrintOutput( wxT(")") );
		break;
	}

	case 0x42:	// EX_DelegateFunction 
	{
		RB_TOKEN_NAME("EX_DelegateFunction");
		RB_TOKEN_DESC("Call to a delegate function");
		RB_ADD_TOKEN;
		RB_LOAD_TOKEN;
		RB_LOAD_DATA(dword,fname);
		RB_LOAD_DATA(dword,unk);

		output = wxT("EX_DelegateFunction");
		ParseAndOutputFunction( output );
		break;
	}


	case 0x43:	// EX_DelegateProperty 
	{
		RB_TOKEN_NAME("EX_DelegateProperty");
		RB_TOKEN_DESC("Delegate expression");
		RB_LOAD_DATA(dword,fname);
		RB_LOAD_DATA(dword,unk);
		output = wxString::Format(wxT("delegate= [%d][%d] %s"),fname,unk,PF_SAFESTR(Pkg.GetNameString(fname)));
		PrintOutput( wxT(")") );
		RB_ADD_TOKEN;
		break;
	}


	case 0x44:	// EX_LetDelegate 
	{
		RB_TOKEN_NAME("EX_LetDelegate");
		RB_TOKEN_DESC("Assignment to a delegate");
		RB_ADD_TOKEN;
		RB_LOAD_TOKEN;
		PrintOutput( wxT(" = ") );
		RB_LOAD_TOKEN;
		break;
	}

	case 0x45:	// EX_TernaryIf ?
	{
		RB_TOKEN_NAME("EX_TernaryIf");
		RB_TOKEN_DESC("");
		output = name;
		RB_ADD_TOKEN;

		RB_LOAD_TOKEN;
		RB_LOAD_DATA(byte,end1);
		RB_LOAD_DATA(byte,end11);
		PrintOutput( wxT(" ? ") );
		
		RB_LOAD_TOKEN;
		RB_LOAD_DATA(byte,end2);
		RB_LOAD_DATA(byte,end21);
		PrintOutput( wxT(" : ") );

		RB_LOAD_TOKEN;
		break;
	}


	case 0x46:	// something for enums / numbers ? 
	{
		RB_TOKEN_NAME("EX_0x46");
		RB_TOKEN_DESC("");
		output = name;
		PrintOutput( output );
		RB_ADD_TOKEN;
		RB_LOAD_TOKEN;
		RB_LOAD_TOKEN;
		break;
	}


	case 0x47:	// archetype / replication related? 
	{
		RB_TOKEN_NAME("EX_0x47");
		RB_TOKEN_DESC("");
		output = name;
		PrintOutput( output );
		RB_ADD_TOKEN;
		RB_LOAD_TOKEN; // archetype
		RB_LOAD_TOKEN; // const name
		break;
	}

	case 0x48:	// EX_OutParameter?
	{
		RB_TOKEN_NAME("EX_OutParameter");
		RB_TOKEN_DESC("");
		RB_LOAD_DATA(dword,value);
		output = wxString::Format( wxT("%s"), PF_SAFESTR(Pkg.GetObjectName(value)) );
		RB_ADD_TOKEN;
		PrintOutput( output );
		break;
	}

	case 0x49:	// EX_OptionalParamDefaults?
	{
		RB_TOKEN_NAME("EX_0x49");
		RB_TOKEN_DESC("");
		RB_LOAD_DATA(word,value);
		RB_ADD_TOKEN;
		RB_LOAD_TOKEN;
		break;
	}


	case 0x50:	// 0x50
	{
		RB_TOKEN_NAME("0x50"); // EX_SkipOptionalParam ??
		RB_TOKEN_DESC("0x50");
		t->SetFlag(TF_SkipPostOutput);
		RB_ADD_TOKEN;
		break;
	}

	case 0x51:	// EX_InterfaceToken 
	{
		RB_TOKEN_NAME("EX_InterfaceToken"); // ??
		RB_TOKEN_DESC("0x51");
		RB_ADD_TOKEN;
		RB_LOAD_TOKEN;
		break;
	}

	case 0x52:	// EX_InterfaceStuff  // local onlinegameinterface gi; ... gi != none
	{
		RB_TOKEN_NAME("EX_InterfaceStuff"); // ??
		RB_TOKEN_DESC("0x52");
		RB_LOAD_DATA(dword,value);
		output = wxString::Format(wxT("%s"),PF_SAFESTR(Pkg.GetNameString(value)));
		RB_ADD_TOKEN;
		PrintOutput( output );
		break;
	}


	case 0x53:	// EX_End 
	{
		RB_TOKEN_NAME("EX_End"); // EX_End ??
		RB_TOKEN_DESC("End of script");
		t->SetFlag(TF_SkipPostOutput);
		RB_ADD_TOKEN;
		break;
	}

	default:
	{
		throw upexception(wxString::Format(wxT("Unknown Token %x at %s"),dword(token),Arc.DebugInfo().c_str()));
	}

	}

	if( bStack )
	{
		PopStack();
	}
	else
	{
		//throw upexception( wxString::Format(wxT("misconfigured bytecode %x"),dword(token) ));
	}

	PrintPostOutput(t);
	

	} 
	catch( upexception& e ) { throw upexception( wxString::Format(wxT(" [%.4x]%s -- %s"), token, PF_SAFESTR(name), e.wwhat().c_str() )); }
	catch( exception& e ) {  throw exception( wxString::Format(wxT(" [%.4x]%s -- %hs"), token, PF_SAFESTR(name), e.what()).ToAscii()  ); }

}


// ============================================================================
//  EOF
// ============================================================================