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


static const dword FUNC_PreOperator	= 0x00000010;	// Unary operator is a prefix operator.
static const dword FUNC_Operator	= 0x00001000;   // Operator function.
	
#define RB_TOKEN_NAME(text)			name = wxT(text)
#define RB_TOKEN_CODE(text)			code = wxT(text)

void rbBytecode::ParseFunc( dword token )
{
	rbToken* t = new rbToken(NULL,token);
	wxString& name = t->Name;
	wxString& output = t->Output;
	wxString code;
	dword flags = 0;

	

	try{
	//wxLogMessage( wxT("%sFUNC %x"), GetPrefix(), dword(token) );

	PrintPreOutput(t);

	switch( token )
	{

	case 0x0070:	// $ 
	{
		RB_TOKEN_CODE("29A001000029A10100000B53");
		RB_TOKEN_NAME("$");
		flags=0x00023401;
		break;
	}

	case 0x0071:	// GotoState 
	{
		RB_TOKEN_CODE("299D000000299E000000299F00000029A00000000B53");
		RB_TOKEN_NAME("GotoState");
		flags=0x00020401;
		break;
	}

	case 0x0072:	// == 
	{
		RB_TOKEN_CODE("293B010000293C0100000B53");
		RB_TOKEN_NAME("==");
		flags=0x00023401;
		break;
	}

	case 0x0073:	// < 
	{
		RB_TOKEN_CODE("299801000029990100000B53");
		RB_TOKEN_NAME("<");
		flags=0x00023401;
		break;
	}

	case 0x0074:	// > 
	{
		RB_TOKEN_CODE("299401000029950100000B53");
		RB_TOKEN_NAME(">");
		flags=0x00023401;
		break;
	}

	case 0x0075:	// Enable 
	{
		RB_TOKEN_CODE("29830000000B53");
		RB_TOKEN_NAME("Enable");
		flags=0x00020401;
		break;
	}

	case 0x0076:	// Disable 
	{
		RB_TOKEN_CODE("29810000000B53");
		RB_TOKEN_NAME("Disable");
		flags=0x00020401;
		break;
	}

	case 0x0077:	// != 
	{
		RB_TOKEN_CODE("293701000029380100000B53");
		RB_TOKEN_NAME("!=");
		flags=0x00023401;
		break;
	}

	case 0x0078:	// <= 
	{
		RB_TOKEN_CODE("299001000029910100000B53");
		RB_TOKEN_NAME("<=");
		flags=0x00023401;
		break;
	}

	case 0x0079:	// >= 
	{
		RB_TOKEN_CODE("298C010000298D0100000B53");
		RB_TOKEN_NAME(">=");
		flags=0x00023401;
		break;
	}

	case 0x007A:	// == 
	{
		RB_TOKEN_CODE("298801000029890100000B53");
		RB_TOKEN_NAME("==");
		flags=0x00023401;
		break;
	}

	case 0x007B:	// != 
	{
		RB_TOKEN_CODE("298401000029850100000B53");
		RB_TOKEN_NAME("!=");
		flags=0x00023401;
		break;
	}

	case 0x007C:	// ~= 
	{
		RB_TOKEN_CODE("298001000029810100000B53");
		RB_TOKEN_NAME("~=");
		flags=0x00023401;
		break;
	}

	case 0x007D:	// Len 
	{
		RB_TOKEN_CODE("29730100000B53");
		RB_TOKEN_NAME("Len");
		flags=0x00022401;
		break;
	}

	case 0x007E:	// InStr 
	{
		RB_TOKEN_CODE("296F01000029700100000B53");
		RB_TOKEN_NAME("InStr");
		flags=0x00022401;
		break;
	}

	case 0x007F:	// Mid 
	{
		RB_TOKEN_CODE("296A010000296B010000296C0100000B53");
		RB_TOKEN_NAME("Mid");
		flags=0x00022401;
		break;
	}

	case 0x0080:	// Left 
	{
		RB_TOKEN_CODE("296601000029670100000B53");
		RB_TOKEN_NAME("Left");
		flags=0x00022401;
		break;
	}

	case 0x0081:	// ! 
	{
		RB_TOKEN_CODE("29B70300000B53");
		RB_TOKEN_NAME("!");
		flags=0x00023411;
		break;
	}

	case 0x0082:	// && 
	{
		RB_TOKEN_CODE("29AB03000029AC0300000B53");
		RB_TOKEN_NAME("&&");
		flags=0x00023401;
		break;
	}

	case 0x0083:	// ^^ 
	{
		RB_TOKEN_CODE("29A703000029A80300000B53");
		RB_TOKEN_NAME("^^");
		flags=0x00023401;
		break;
	}

	case 0x0084:	// || 
	{
		RB_TOKEN_CODE("29A303000029A40300000B53");
		RB_TOKEN_NAME("||");
		flags=0x00023401;
		break;
	}

	case 0x0085:	// *= 
	{
		RB_TOKEN_CODE("299F03000029A00300000B53");
		RB_TOKEN_NAME("*=");
		flags=0x00423401;
		break;
	}

	case 0x0086:	// /= 
	{
		RB_TOKEN_CODE("299B030000299C0300000B53");
		RB_TOKEN_NAME("/=");
		flags=0x00423401;
		break;
	}

	case 0x0087:	// += 
	{
		RB_TOKEN_CODE("299703000029980300000B53");
		RB_TOKEN_NAME("+=");
		flags=0x00423401;
		break;
	}

	case 0x0088:	// -= 
	{
		RB_TOKEN_CODE("299303000029940300000B53");
		RB_TOKEN_NAME("-=");
		flags=0x00423401;
		break;
	}

	case 0x0089:	// ++ 
	{
		RB_TOKEN_CODE("29900300000B53");
		RB_TOKEN_NAME("++");
		flags=0x00423411;
		break;
	}

	case 0x008A:	// -- 
	{
		RB_TOKEN_CODE("298D0300000B53");
		RB_TOKEN_NAME("--");
		flags=0x00423411;
		break;
	}

	case 0x008B:	// ++ 
	{
		RB_TOKEN_CODE("298A0300000B53");
		RB_TOKEN_NAME("++");
		flags=0x00423401;
		break;
	}

	case 0x008C:	// -- 
	{
		RB_TOKEN_CODE("29870300000B53");
		RB_TOKEN_NAME("--");
		flags=0x00423401;
		break;
	}

	case 0x008D:	// ~ 
	{
		RB_TOKEN_CODE("29840300000B53");
		RB_TOKEN_NAME("~");
		flags=0x00023411;
		break;
	}

	case 0x008E:	// == 
	{
		RB_TOKEN_CODE("290D020000290E0200000B53");
		RB_TOKEN_NAME("==");
		flags=0x00023401;
		break;
	}

	case 0x008F:	// - 
	{
		RB_TOKEN_CODE("29810300000B53");
		RB_TOKEN_NAME("-");
		flags=0x00023411;
		break;
	}

	case 0x0090:	// * 
	{
		RB_TOKEN_CODE("297D030000297E0300000B53");
		RB_TOKEN_NAME("*");
		flags=0x00023401;
		break;
	}

	case 0x0091:	// / 
	{
		RB_TOKEN_CODE("2979030000297A0300000B53");
		RB_TOKEN_NAME("/");
		flags=0x00023401;
		break;
	}

	case 0x0092:	// + 
	{
		RB_TOKEN_CODE("297503000029760300000B53");
		RB_TOKEN_NAME("+");
		flags=0x00023401;
		break;
	}

	case 0x0093:	// - 
	{
		RB_TOKEN_CODE("297103000029720300000B53");
		RB_TOKEN_NAME("-");
		flags=0x00023401;
		break;
	}

	case 0x0094:	// << 
	{
		RB_TOKEN_CODE("296D030000296E0300000B53");
		RB_TOKEN_NAME("<<");
		flags=0x00023401;
		break;
	}

	case 0x0095:	// >> 
	{
		RB_TOKEN_CODE("2969030000296A0300000B53");
		RB_TOKEN_NAME(">>");
		flags=0x00023401;
		break;
	}

	case 0x0096:	// < 
	{
		RB_TOKEN_CODE("296103000029620300000B53");
		RB_TOKEN_NAME("<");
		flags=0x00023401;
		break;
	}

	case 0x0097:	// > 
	{
		RB_TOKEN_CODE("295D030000295E0300000B53");
		RB_TOKEN_NAME(">");
		flags=0x00023401;
		break;
	}

	case 0x0098:	// <= 
	{
		RB_TOKEN_CODE("2959030000295A0300000B53");
		RB_TOKEN_NAME("<=");
		flags=0x00023401;
		break;
	}

	case 0x0099:	// >= 
	{
		RB_TOKEN_CODE("295503000029560300000B53");
		RB_TOKEN_NAME(">=");
		flags=0x00023401;
		break;
	}

	case 0x009A:	// == 
	{
		RB_TOKEN_CODE("295103000029520300000B53");
		RB_TOKEN_NAME("==");
		flags=0x00023401;
		break;
	}

	case 0x009B:	// != 
	{
		RB_TOKEN_CODE("294D030000294E0300000B53");
		RB_TOKEN_NAME("!=");
		flags=0x00023401;
		break;
	}

	case 0x009C:	// & 
	{
		RB_TOKEN_CODE("2949030000294A0300000B53");
		RB_TOKEN_NAME("&");
		flags=0x00023401;
		break;
	}

	case 0x009D:	// ^ 
	{
		RB_TOKEN_CODE("294503000029460300000B53");
		RB_TOKEN_NAME("^");
		flags=0x00023401;
		break;
	}

	case 0x009E:	// | 
	{
		RB_TOKEN_CODE("294103000029420300000B53");
		RB_TOKEN_NAME("|");
		flags=0x00023401;
		break;
	}

	case 0x009F:	// *= 
	{
		RB_TOKEN_CODE("293D030000293E0300000B53");
		RB_TOKEN_NAME("*=");
		flags=0x00423401;
		break;
	}

	case 0x00A0:	// /= 
	{
		RB_TOKEN_CODE("2939030000293A0300000B53");
		RB_TOKEN_NAME("/=");
		flags=0x00423401;
		break;
	}

	case 0x00A1:	// += 
	{
		RB_TOKEN_CODE("293503000029360300000B53");
		RB_TOKEN_NAME("+=");
		flags=0x00423401;
		break;
	}

	case 0x00A2:	// -= 
	{
		RB_TOKEN_CODE("293103000029320300000B53");
		RB_TOKEN_NAME("-=");
		flags=0x00423401;
		break;
	}

	case 0x00A3:	// ++ 
	{
		RB_TOKEN_CODE("292E0300000B53");
		RB_TOKEN_NAME("++");
		flags=0x00423411;
		break;
	}

	case 0x00A4:	// -- 
	{
		RB_TOKEN_CODE("292B0300000B53");
		RB_TOKEN_NAME("--");
		flags=0x00423411;
		break;
	}

	case 0x00A5:	// ++ 
	{
		RB_TOKEN_CODE("29280300000B53");
		RB_TOKEN_NAME("++");
		flags=0x00423401;
		break;
	}

	case 0x00A6:	// -- 
	{
		RB_TOKEN_CODE("29250300000B53");
		RB_TOKEN_NAME("--");
		flags=0x00423401;
		break;
	}

	case 0x00A7:	// Rand 
	{
		RB_TOKEN_CODE("29220300000B53");
		RB_TOKEN_NAME("Rand");
		flags=0x00022401;
		break;
	}

	case 0x00A8:	// @ 
	{
		RB_TOKEN_CODE("299C010000299D0100000B53");
		RB_TOKEN_NAME("@");
		flags=0x00023401;
		break;
	}

	case 0x00A9:	// - 
	{
		RB_TOKEN_CODE("29120300000B53");
		RB_TOKEN_NAME("-");
		flags=0x00023411;
		break;
	}

	case 0x00AA:	// ** 
	{
		RB_TOKEN_CODE("290E030000290F0300000B53");
		RB_TOKEN_NAME("**");
		flags=0x00023401;
		break;
	}

	case 0x00AB:	// * 
	{
		RB_TOKEN_CODE("290A030000290B0300000B53");
		RB_TOKEN_NAME("*");
		flags=0x00023401;
		break;
	}

	case 0x00AC:	// / 
	{
		RB_TOKEN_CODE("290603000029070300000B53");
		RB_TOKEN_NAME("/");
		flags=0x00023401;
		break;
	}

	case 0x00AD:	// % 
	{
		RB_TOKEN_CODE("290203000029030300000B53");
		RB_TOKEN_NAME("%");
		flags=0x00023401;
		break;
	}

	case 0x00AE:	// + 
	{
		RB_TOKEN_CODE("29FE02000029FF0200000B53");
		RB_TOKEN_NAME("+");
		flags=0x00023401;
		break;
	}

	case 0x00AF:	// - 
	{
		RB_TOKEN_CODE("29FA02000029FB0200000B53");
		RB_TOKEN_NAME("-");
		flags=0x00023401;
		break;
	}

	case 0x00B0:	// < 
	{
		RB_TOKEN_CODE("29F602000029F70200000B53");
		RB_TOKEN_NAME("<");
		flags=0x00023401;
		break;
	}

	case 0x00B1:	// > 
	{
		RB_TOKEN_CODE("29F202000029F30200000B53");
		RB_TOKEN_NAME(">");
		flags=0x00023401;
		break;
	}

	case 0x00B2:	// <= 
	{
		RB_TOKEN_CODE("29EE02000029EF0200000B53");
		RB_TOKEN_NAME("<=");
		flags=0x00023401;
		break;
	}

	case 0x00B3:	// >= 
	{
		RB_TOKEN_CODE("29EA02000029EB0200000B53");
		RB_TOKEN_NAME(">=");
		flags=0x00023401;
		break;
	}

	case 0x00B4:	// == 
	{
		RB_TOKEN_CODE("29E602000029E70200000B53");
		RB_TOKEN_NAME("==");
		flags=0x00023401;
		break;
	}

	case 0x00B5:	// != 
	{
		RB_TOKEN_CODE("29DE02000029DF0200000B53");
		RB_TOKEN_NAME("!=");
		flags=0x00023401;
		break;
	}

	case 0x00B6:	// *= 
	{
		RB_TOKEN_CODE("29DA02000029DB0200000B53");
		RB_TOKEN_NAME("*=");
		flags=0x00423401;
		break;
	}

	case 0x00B7:	// /= 
	{
		RB_TOKEN_CODE("29D602000029D70200000B53");
		RB_TOKEN_NAME("/=");
		flags=0x00423401;
		break;
	}

	case 0x00B8:	// += 
	{
		RB_TOKEN_CODE("29D202000029D30200000B53");
		RB_TOKEN_NAME("+=");
		flags=0x00423401;
		break;
	}

	case 0x00B9:	// -= 
	{
		RB_TOKEN_CODE("29CE02000029CF0200000B53");
		RB_TOKEN_NAME("-=");
		flags=0x00423401;
		break;
	}

	case 0x00BA:	// Abs 
	{
		RB_TOKEN_CODE("29CB0200000B53");
		RB_TOKEN_NAME("Abs");
		flags=0x00022401;
		break;
	}

	case 0x00BB:	// Sin 
	{
		RB_TOKEN_CODE("29C80200000B53");
		RB_TOKEN_NAME("Sin");
		flags=0x00022401;
		break;
	}

	case 0x00BC:	// Cos 
	{
		RB_TOKEN_CODE("29C20200000B53");
		RB_TOKEN_NAME("Cos");
		flags=0x00022401;
		break;
	}

	case 0x00BD:	// Tan 
	{
		RB_TOKEN_CODE("29BC0200000B53");
		RB_TOKEN_NAME("Tan");
		flags=0x00022401;
		break;
	}

	case 0x00BE:	// Atan 
	{
		RB_TOKEN_CODE("29B902000029BA0200000B53");
		RB_TOKEN_NAME("Atan");
		flags=0x00022401;
		break;
	}

	case 0x00BF:	// Exp 
	{
		RB_TOKEN_CODE("29B60200000B53");
		RB_TOKEN_NAME("Exp");
		flags=0x00022401;
		break;
	}

	case 0x00C0:	// Loge 
	{
		RB_TOKEN_CODE("29B30200000B53");
		RB_TOKEN_NAME("Loge");
		flags=0x00022401;
		break;
	}

	case 0x00C1:	// Sqrt 
	{
		RB_TOKEN_CODE("29B00200000B53");
		RB_TOKEN_NAME("Sqrt");
		flags=0x00022401;
		break;
	}

	case 0x00C2:	// Square 
	{
		RB_TOKEN_CODE("29AD0200000B53");
		RB_TOKEN_NAME("Square");
		flags=0x00022401;
		break;
	}

	case 0x00C3:	// FRand 
	{
		RB_TOKEN_CODE("0B53");
		RB_TOKEN_NAME("FRand");
		flags=0x00022401;
		break;
	}

	case 0x00C4:	// >>> 
	{
		RB_TOKEN_CODE("296503000029660300000B53");
		RB_TOKEN_NAME(">>>");
		flags=0x00023401;
		break;
	}

	case 0x00C5:	// IsA 
	{
		RB_TOKEN_CODE("29280100000B53");
		RB_TOKEN_NAME("IsA");
		flags=0x00020401;
		break;
	}

	case 0x00C9:	// Repl 
	{
		RB_TOKEN_CODE("29500100002951010000295201000029530100000B53");
		RB_TOKEN_NAME("Repl");
		flags=0x00022401;
		break;
	}

	case 0x00CB:	// != 
	{
		RB_TOKEN_CODE("2909020000290A0200000B53");
		RB_TOKEN_NAME("!=");
		flags=0x00023401;
		break;
	}

	case 0x00D2:	// ~= 
	{
		RB_TOKEN_CODE("29E202000029E30200000B53");
		RB_TOKEN_NAME("~=");
		flags=0x00023401;
		break;
	}

	case 0x00D3:	// - 
	{
		RB_TOKEN_CODE("29800200000B53");
		RB_TOKEN_NAME("-");
		flags=0x00023411;
		break;
	}

	case 0x00D4:	// * 
	{
		RB_TOKEN_CODE("297D020000297E0200000B53");
		RB_TOKEN_NAME("*");
		flags=0x00023401;
		break;
	}

	case 0x00D5:	// * 
	{
		RB_TOKEN_CODE("2979020000297A0200000B53");
		RB_TOKEN_NAME("*");
		flags=0x00023401;
		break;
	}

	case 0x00D6:	// / 
	{
		RB_TOKEN_CODE("297102000029720200000B53");
		RB_TOKEN_NAME("/");
		flags=0x00023401;
		break;
	}

	case 0x00D7:	// + 
	{
		RB_TOKEN_CODE("296D020000296E0200000B53");
		RB_TOKEN_NAME("+");
		flags=0x00023401;
		break;
	}

	case 0x00D8:	// - 
	{
		RB_TOKEN_CODE("2969020000296A0200000B53");
		RB_TOKEN_NAME("-");
		flags=0x00023401;
		break;
	}

	case 0x00D9:	// == 
	{
		RB_TOKEN_CODE("295D020000295E0200000B53");
		RB_TOKEN_NAME("==");
		flags=0x00023401;
		break;
	}

	case 0x00DA:	// != 
	{
		RB_TOKEN_CODE("2959020000295A0200000B53");
		RB_TOKEN_NAME("!=");
		flags=0x00023401;
		break;
	}

	case 0x00DB:	// Dot 
	{
		RB_TOKEN_CODE("295502000029560200000B53");
		RB_TOKEN_NAME("Dot");
		flags=0x00023401;
		break;
	}

	case 0x00DC:	// Cross 
	{
		RB_TOKEN_CODE("295102000029520200000B53");
		RB_TOKEN_NAME("Cross");
		flags=0x00023401;
		break;
	}

	case 0x00DD:	// *= 
	{
		RB_TOKEN_CODE("294D020000294E0200000B53");
		RB_TOKEN_NAME("*=");
		flags=0x00423401;
		break;
	}

	case 0x00DE:	// /= 
	{
		RB_TOKEN_CODE("294502000029460200000B53");
		RB_TOKEN_NAME("/=");
		flags=0x00423401;
		break;
	}

	case 0x00DF:	// += 
	{
		RB_TOKEN_CODE("294102000029420200000B53");
		RB_TOKEN_NAME("+=");
		flags=0x00423401;
		break;
	}

	case 0x00E0:	// -= 
	{
		RB_TOKEN_CODE("293D020000293E0200000B53");
		RB_TOKEN_NAME("-=");
		flags=0x00423401;
		break;
	}

	case 0x00E1:	// VSize 
	{
		RB_TOKEN_CODE("293A0200000B53");
		RB_TOKEN_NAME("VSize");
		flags=0x00022401;
		break;
	}

	case 0x00E2:	// Normal 
	{
		RB_TOKEN_CODE("292E0200000B53");
		RB_TOKEN_NAME("Normal");
		flags=0x00022401;
		break;
	}

	case 0x00E5:	// GetAxes 
	{
		RB_TOKEN_CODE("29DC01000029DD01000029DE01000029DF0100000B53");
		RB_TOKEN_NAME("GetAxes");
		flags=0x00422401;
		break;
	}

	case 0x00E6:	// GetUnAxes 
	{
		RB_TOKEN_CODE("29D701000029D801000029D901000029DA0100000B53");
		RB_TOKEN_NAME("GetUnAxes");
		flags=0x00422401;
		break;
	}

	case 0x00E7:	// LogInternal 
	{
		RB_TOKEN_CODE("29AC00000029AD0000000B53");
		RB_TOKEN_NAME("LogInternal");
		flags=0x00022401;
		break;
	}

	case 0x00E8:	// WarnInternal 
	{
		RB_TOKEN_CODE("29AA0000000B53");
		RB_TOKEN_NAME("WarnInternal");
		flags=0x00022401;
		break;
	}

	case 0x00EA:	// Right 
	{
		RB_TOKEN_CODE("296201000029630100000B53");
		RB_TOKEN_NAME("Right");
		flags=0x00022401;
		break;
	}

	case 0x00EB:	// Caps 
	{
		RB_TOKEN_CODE("295F0100000B53");
		RB_TOKEN_NAME("Caps");
		flags=0x00022401;
		break;
	}

	case 0x00EC:	// Chr 
	{
		RB_TOKEN_CODE("29590100000B53");
		RB_TOKEN_NAME("Chr");
		flags=0x00022401;
		break;
	}

	case 0x00ED:	// Asc 
	{
		RB_TOKEN_CODE("29560100000B53");
		RB_TOKEN_NAME("Asc");
		flags=0x00022401;
		break;
	}

	case 0x00EE:	// Locs 
	{
		RB_TOKEN_CODE("295C0100000B53");
		RB_TOKEN_NAME("Locs");
		flags=0x00022401;
		break;
	}

	case 0x00F2:	// == 
	{
		RB_TOKEN_CODE("29B303000029B40300000B53");
		RB_TOKEN_NAME("==");
		flags=0x00023401;
		break;
	}

	case 0x00F3:	// != 
	{
		RB_TOKEN_CODE("29AF03000029B00300000B53");
		RB_TOKEN_NAME("!=");
		flags=0x00023401;
		break;
	}

	case 0x00F4:	// FMin 
	{
		RB_TOKEN_CODE("29A702000029A80200000B53");
		RB_TOKEN_NAME("FMin");
		flags=0x00022401;
		break;
	}

	case 0x00F5:	// FMax 
	{
		RB_TOKEN_CODE("29A302000029A40200000B53");
		RB_TOKEN_NAME("FMax");
		flags=0x00022401;
		break;
	}

	case 0x00F6:	// FClamp 
	{
		RB_TOKEN_CODE("299E020000299F02000029A00200000B53");
		RB_TOKEN_NAME("FClamp");
		flags=0x00022401;
		break;
	}

	case 0x00F7:	// Lerp 
	{
		RB_TOKEN_CODE("2999020000299A020000299B0200000B53");
		RB_TOKEN_NAME("Lerp");
		flags=0x00022401;
		break;
	}

	case 0x00F9:	// Min 
	{
		RB_TOKEN_CODE("291E030000291F0300000B53");
		RB_TOKEN_NAME("Min");
		flags=0x00022401;
		break;
	}

	case 0x00FA:	// Max 
	{
		RB_TOKEN_CODE("291A030000291B0300000B53");
		RB_TOKEN_NAME("Max");
		flags=0x00022401;
		break;
	}

	case 0x00FB:	// Clamp 
	{
		RB_TOKEN_CODE("2915030000291603000029170300000B53");
		RB_TOKEN_NAME("Clamp");
		flags=0x00022401;
		break;
	}

	case 0x00FC:	// VRand 
	{
		RB_TOKEN_CODE("0B53");
		RB_TOKEN_NAME("VRand");
		flags=0x00022401;
		break;
	}

	case 0x00FE:	// == 
	{
		RB_TOKEN_CODE("292401000029250100000B53");
		RB_TOKEN_NAME("==");
		flags=0x00023401;
		break;
	}

	case 0x00FF:	// != 
	{
		RB_TOKEN_CODE("292001000029210100000B53");
		RB_TOKEN_NAME("!=");
		flags=0x00023401;
		break;
	}

	case 0x0100:	// Sleep 
	{
		RB_TOKEN_CODE("294C0D00000B53");
		RB_TOKEN_NAME("Sleep");
		flags=0x00020409;
		break;
	}

	case 0x0102:	// ClassIsChildOf 
	{
		RB_TOKEN_CODE("292B010000292C0100000B53");
		RB_TOKEN_NAME("ClassIsChildOf");
		flags=0x00022401;
		break;
	}

	case 0x0105:	// FinishAnim 
	{
		RB_TOKEN_CODE("294B0D00000B53");
		RB_TOKEN_NAME("FinishAnim");
		flags=0x00020409;
		break;
	}

	case 0x0106:	// SetCollision 
	{
		RB_TOKEN_CODE("29470D000029480D000029490D00000B53");
		RB_TOKEN_NAME("SetCollision");
		flags=0x00020401;
		break;
	}

	case 0x010A:	// Move 
	{
		RB_TOKEN_CODE("293D0D00000B53");
		RB_TOKEN_NAME("Move");
		flags=0x00020401;
		break;
	}

	case 0x010B:	// SetLocation 
	{
		RB_TOKEN_CODE("293A0D00000B53");
		RB_TOKEN_NAME("SetLocation");
		flags=0x00020401;
		break;
	}

	case 0x010E:	// + 
	{
		RB_TOKEN_CODE("29F900000029FA0000000B53");
		RB_TOKEN_NAME("+");
		flags=0x00023401;
		break;
	}

	case 0x010F:	// - 
	{
		RB_TOKEN_CODE("29F000000029F10000000B53");
		RB_TOKEN_NAME("-");
		flags=0x00023401;
		break;
	}

	case 0x0110:	// SetOwner 
	{
		RB_TOKEN_CODE("29230D00000B53");
		RB_TOKEN_NAME("SetOwner");
		flags=0x00020401;
		break;
	}

	case 0x0113:	// << 
	{
		RB_TOKEN_CODE("296502000029660200000B53");
		RB_TOKEN_NAME("<<");
		flags=0x00023401;
		break;
	}

	case 0x0114:	// >> 
	{
		RB_TOKEN_CODE("296102000029620200000B53");
		RB_TOKEN_NAME(">>");
		flags=0x00023401;
		break;
	}

	case 0x0115:	// Trace 
	{
		RB_TOKEN_CODE("29940C000029950C000029960C000029970C000029980C000029990C0000299A0C0000299B0C00000B53");
		RB_TOKEN_NAME("Trace");
		flags=0x00420401;
		break;
	}

	case 0x0116:	// Spawn 
	{
		RB_TOKEN_CODE("29730C000029740C000029750C000029760C000029770C000029780C000029790C00000B53");
		RB_TOKEN_NAME("Spawn");
		flags=0x00020401;
		break;
	}

	case 0x0117:	// Destroy 
	{
		RB_TOKEN_CODE("0B53");
		RB_TOKEN_NAME("Destroy");
		flags=0x00020401;
		break;
	}

	case 0x0118:	// SetTimer 
	{
		RB_TOKEN_CODE("296B0C0000296C0C0000296D0C0000296E0C00000B53");
		RB_TOKEN_NAME("SetTimer");
		flags=0x00020401;
		break;
	}

	case 0x0119:	// IsInState 
	{
		RB_TOKEN_CODE("2999000000299A0000000B53");
		RB_TOKEN_NAME("IsInState");
		flags=0x00020401;
		break;
	}

	case 0x011B:	// SetCollisionSize 
	{
		RB_TOKEN_CODE("29440D000029450D00000B53");
		RB_TOKEN_NAME("SetCollisionSize");
		flags=0x00020401;
		break;
	}

	case 0x011C:	// GetStateName 
	{
		RB_TOKEN_CODE("0B53");
		RB_TOKEN_NAME("GetStateName");
		flags=0x00020401;
		break;
	}

	case 0x011F:	// * 
	{
		RB_TOKEN_CODE("290502000029060200000B53");
		RB_TOKEN_NAME("*");
		flags=0x00023401;
		break;
	}

	case 0x0120:	// * 
	{
		RB_TOKEN_CODE("290102000029020200000B53");
		RB_TOKEN_NAME("*");
		flags=0x00023401;
		break;
	}

	case 0x0121:	// / 
	{
		RB_TOKEN_CODE("29FD01000029FE0100000B53");
		RB_TOKEN_NAME("/");
		flags=0x00023401;
		break;
	}

	case 0x0122:	// *= 
	{
		RB_TOKEN_CODE("29F901000029FA0100000B53");
		RB_TOKEN_NAME("*=");
		flags=0x00423401;
		break;
	}

	case 0x0123:	// /= 
	{
		RB_TOKEN_CODE("29F501000029F60100000B53");
		RB_TOKEN_NAME("/=");
		flags=0x00423401;
		break;
	}

	case 0x0128:	// * 
	{
		RB_TOKEN_CODE("297502000029760200000B53");
		RB_TOKEN_NAME("*");
		flags=0x00023401;
		break;
	}

	case 0x0129:	// *= 
	{
		RB_TOKEN_CODE("2949020000294A0200000B53");
		RB_TOKEN_NAME("*=");
		flags=0x00423401;
		break;
	}

	case 0x012A:	// SetBase 
	{
		RB_TOKEN_CODE("29250D000029260D000029270D000029280D00000B53");
		RB_TOKEN_NAME("SetBase");
		flags=0x00020401;
		break;
	}

	case 0x012B:	// SetRotation 
	{
		RB_TOKEN_CODE("29370D00000B53");
		RB_TOKEN_NAME("SetRotation");
		flags=0x00020401;
		break;
	}

	case 0x012C:	// MirrorVectorByNormal 
	{
		RB_TOKEN_CODE("291E020000291F0200000B53");
		RB_TOKEN_NAME("MirrorVectorByNormal");
		flags=0x00022401;
		break;
	}

	case 0x0130:	// AllActors 
	{
		RB_TOKEN_CODE("29360C000029370C00000B53");
		RB_TOKEN_NAME("AllActors");
		flags=0x00420405;
		break;
	}

	case 0x0131:	// ChildActors 
	{
		RB_TOKEN_CODE("29300C000029310C00000B53");
		RB_TOKEN_NAME("ChildActors");
		flags=0x00420405;
		break;
	}

	case 0x0132:	// BasedActors 
	{
		RB_TOKEN_CODE("292D0C0000292E0C00000B53");
		RB_TOKEN_NAME("BasedActors");
		flags=0x00420405;
		break;
	}

	case 0x0133:	// TouchingActors 
	{
		RB_TOKEN_CODE("292A0C0000292B0C00000B53");
		RB_TOKEN_NAME("TouchingActors");
		flags=0x00420405;
		break;
	}

	case 0x0135:	// TraceActors 
	{
		RB_TOKEN_CODE("29210C000029220C000029230C000029240C000029250C000029260C000029270C000029280C00000B53");
		RB_TOKEN_NAME("TraceActors");
		flags=0x00420405;
		break;
	}

	case 0x0137:	// VisibleActors 
	{
		RB_TOKEN_CODE("291C0C0000291D0C0000291E0C0000291F0C00000B53");
		RB_TOKEN_NAME("VisibleActors");
		flags=0x00420405;
		break;
	}

	case 0x0138:	// VisibleCollidingActors 
	{
		RB_TOKEN_CODE("29160C000029170C000029180C000029190C0000291A0C00000B53");
		RB_TOKEN_NAME("VisibleCollidingActors");
		flags=0x00420405;
		break;
	}

	case 0x0139:	// DynamicActors 
	{
		RB_TOKEN_CODE("29330C000029340C00000B53");
		RB_TOKEN_NAME("DynamicActors");
		flags=0x00420405;
		break;
	}

	case 0x013C:	// + 
	{
		RB_TOKEN_CODE("29F101000029F20100000B53");
		RB_TOKEN_NAME("+");
		flags=0x00023401;
		break;
	}

	case 0x013D:	// - 
	{
		RB_TOKEN_CODE("29ED01000029EE0100000B53");
		RB_TOKEN_NAME("-");
		flags=0x00023401;
		break;
	}

	case 0x013E:	// += 
	{
		RB_TOKEN_CODE("29E901000029EA0100000B53");
		RB_TOKEN_NAME("+=");
		flags=0x00423401;
		break;
	}

	case 0x013F:	// -= 
	{
		RB_TOKEN_CODE("29E501000029E60100000B53");
		RB_TOKEN_NAME("-=");
		flags=0x00423401;
		break;
	}

	case 0x0140:	// RotRand 
	{
		RB_TOKEN_CODE("29D40100000B53");
		RB_TOKEN_NAME("RotRand");
		flags=0x00022401;
		break;
	}

	case 0x0141:	// CollidingActors 
	{
		RB_TOKEN_CODE("29100C000029110C000029120C000029130C000029140C00000B53");
		RB_TOKEN_NAME("CollidingActors");
		flags=0x00420405;
		break;
	}

	case 0x0142:	// $= 
	{
		RB_TOKEN_CODE("297D010000297E0100000B53");
		RB_TOKEN_NAME("$=");
		flags=0x00423401;
		break;
	}

	case 0x0143:	// @= 
	{
		RB_TOKEN_CODE("2979010000297A0100000B53");
		RB_TOKEN_NAME("@=");
		flags=0x00423401;
		break;
	}

	case 0x0144:	// -= 
	{
		RB_TOKEN_CODE("297601000029770100000B53");
		RB_TOKEN_NAME("-=");
		flags=0x00423401;
		break;
	}

	case 0x01D0:	// StrLen 
	{
		RB_TOKEN_CODE("29262C000029272C000029282C00000B53");
		RB_TOKEN_NAME("StrLen");
		flags=0x00420401;
		break;
	}

	case 0x01D1:	// DrawText 
	{
		RB_TOKEN_CODE("291F2C000029202C00000B53");
		RB_TOKEN_NAME("DrawText");
		flags=0x00020401;
		break;
	}

	case 0x01D2:	// DrawTile 
	{
		RB_TOKEN_CODE("29312C000029322C000029332C000029342C000029352C000029362C000029372C00000B53");
		RB_TOKEN_NAME("DrawTile");
		flags=0x00020401;
		break;
	}

	case 0x01D5:	// DrawTextClipped 
	{
		RB_TOKEN_CODE("291C2C0000291D2C00000B53");
		RB_TOKEN_NAME("DrawTextClipped");
		flags=0x00020401;
		break;
	}

	case 0x01D6:	// TextSize 
	{
		RB_TOKEN_CODE("29222C000029232C000029242C00000B53");
		RB_TOKEN_NAME("TextSize");
		flags=0x00420401;
		break;
	}

	case 0x01F4:	// MoveTo 
	{
		RB_TOKEN_CODE("2916260000291726000029182600000B53");
		RB_TOKEN_NAME("MoveTo");
		flags=0x00020409;
		break;
	}

	case 0x01F6:	// MoveToward 
	{
		RB_TOKEN_CODE("291026000029112600002912260000291326000029142600000B53");
		RB_TOKEN_NAME("MoveToward");
		flags=0x00020409;
		break;
	}

	case 0x01FC:	// FinishRotation 
	{
		RB_TOKEN_CODE("0B53");
		RB_TOKEN_NAME("FinishRotation");
		flags=0x00020409;
		break;
	}

	case 0x0200:	// MakeNoise 
	{
		RB_TOKEN_CODE("294F0C000029500C00000B53");
		RB_TOKEN_NAME("MakeNoise");
		flags=0x00020401;
		break;
	}

	case 0x0202:	// LineOfSightTo 
	{
		RB_TOKEN_CODE("293326000029342600000B53");
		RB_TOKEN_NAME("LineOfSightTo");
		flags=0x00020401;
		break;
	}

	case 0x0205:	// FindPathToward 
	{
		RB_TOKEN_CODE("29032600002904260000290526000029062600000B53");
		RB_TOKEN_NAME("FindPathToward");
		flags=0x00020401;
		break;
	}

	case 0x0206:	// FindPathTo 
	{
		RB_TOKEN_CODE("2909260000290A260000290B2600000B53");
		RB_TOKEN_NAME("FindPathTo");
		flags=0x00020401;
		break;
	}

	case 0x0208:	// ActorReachable 
	{
		RB_TOKEN_CODE("29EE2500000B53");
		RB_TOKEN_NAME("ActorReachable");
		flags=0x00020401;
		break;
	}

	case 0x0209:	// PointReachable 
	{
		RB_TOKEN_CODE("29F12500000B53");
		RB_TOKEN_NAME("PointReachable");
		flags=0x00020401;
		break;
	}

	case 0x020C:	// FindStairRotation 
	{
		RB_TOKEN_CODE("294F2B00000B53");
		RB_TOKEN_NAME("FindStairRotation");
		flags=0x00020401;
		break;
	}

	case 0x020D:	// FindRandomDest 
	{
		RB_TOKEN_CODE("0B53");
		RB_TOKEN_NAME("FindRandomDest");
		flags=0x00020401;
		break;
	}

	case 0x020E:	// PickWallAdjust 
	{
		RB_TOKEN_CODE("29E82500000B53");
		RB_TOKEN_NAME("PickWallAdjust");
		flags=0x00020401;
		break;
	}

	case 0x020F:	// WaitForLanding 
	{
		RB_TOKEN_CODE("29E62500000B53");
		RB_TOKEN_NAME("WaitForLanding");
		flags=0x00020409;
		break;
	}

	case 0x0213:	// PickTarget 
	{
		RB_TOKEN_CODE("2923260000292426000029252600002926260000292726000029282600000B53");
		RB_TOKEN_NAME("PickTarget");
		flags=0x00420401;
		break;
	}

	case 0x0214:	// PlayerCanSeeMe 
	{
		RB_TOKEN_CODE("0B53");
		RB_TOKEN_NAME("PlayerCanSeeMe");
		flags=0x00020401;
		break;
	}

	case 0x0215:	// CanSee 
	{
		RB_TOKEN_CODE("29302600000B53");
		RB_TOKEN_NAME("CanSee");
		flags=0x00020401;
		break;
	}

	case 0x0218:	// SaveConfig 
	{
		RB_TOKEN_CODE("0B53");
		RB_TOKEN_NAME("SaveConfig");
		flags=0x00020401;
		break;
	}

	case 0x0219:	// CanSeeByPoints 
	{
		RB_TOKEN_CODE("292B260000292C260000292D2600000B53");
		RB_TOKEN_NAME("CanSeeByPoints");
		flags=0x00020401;
		break;
	}

	case 0x0222:	// UpdateURL 
	{
		RB_TOKEN_CODE("29642B000029652B000029662B00000B53");
		RB_TOKEN_NAME("UpdateURL");
		flags=0x00020401;
		break;
	}

	case 0x0223:	// GetURLMap 
	{
		RB_TOKEN_CODE("0B53");
		RB_TOKEN_NAME("GetURLMap");
		flags=0x00020401;
		break;
	}

	case 0x0224:	// FastTrace 
	{
		RB_TOKEN_CODE("29860C000029870C000029880C00000B53");
		RB_TOKEN_NAME("FastTrace");
		flags=0x00020401;
		break;
	}

	case 0x05DC:	// ProjectOnTo 
	{
		RB_TOKEN_CODE("291A020000291B0200000B53");
		RB_TOKEN_NAME("ProjectOnTo");
		flags=0x00022401;
		break;
	}

	case 0x05DD:	// IsZero 
	{
		RB_TOKEN_CODE("29170200000B53");
		RB_TOKEN_NAME("IsZero");
		flags=0x00022401;
		break;
	}

	case 0x0F81:	// MoveSmooth 
	{
		RB_TOKEN_CODE("292D0D00000B53");
		RB_TOKEN_NAME("MoveSmooth");
		flags=0x00020401;
		break;
	}

	case 0x0F82:	// SetPhysics 
	{
		RB_TOKEN_CODE("29E20C00000B53");
		RB_TOKEN_NAME("SetPhysics");
		flags=0x00020401;
		break;
	}

	case 0x0F83:	// AutonomousPhysics 
	{
		RB_TOKEN_CODE("292C0D00000B53");
		RB_TOKEN_NAME("AutonomousPhysics");
		flags=0x00020401;
		break;
	}




	default:
	{
		throw upexception(wxString::Format(wxT("Unknown Function Token %x at %s"),dword(token),Arc.DebugInfo().c_str()));
	}


	}

	output = name;
	AddToken(t);
	PushStack(t);


	if( rbHasFlag(flags,0x1000) )
	{
		if( rbHasFlag(flags,0x10) )
		{
			// preoperator
			//wxLogMessage( wxT("%x %x preoperator"),token,flags);
			output = name;
			PrintOutput(output);	
			for( dword x=0; x!=0xFF; ++x )
				if( LoadToken() == 0x16 )
					break;
		}
		else
		{
			// post/operator
			//wxLogMessage( wxT("%x %x post/operator"),token,flags);
			output = wxString::Format( wxT(" %s "), PF_SAFESTR(name) );

			for( dword x=0; x!=0xFF; ++x )
			{
				if( LoadToken() == 0x16 )
					break;

				if( x == 0 )
				{
					PrintOutput(output);	
				}
			}
		}
	}
	else
	{
		output = wxString::Format( wxT("%s"), PF_SAFESTR(name) );
		ParseAndOutputFunction( output );
	}

	PopStack();

	PrintPostOutput(t);


/*
static final preoperator byte #(byte a){return a;}          
// 00000000000000100011000000010010

static final postoperator byte #(byte a){return a;}         
// 00000000000000100011000000000011

static final operator(1) byte #(byte a, byte b){return a;}  
// 00000000000000100011000000000011

// 0072 ==
// 00000000000000100011010000000001

// 00000000000000000001000000000000


	FUNC_Operator       = 0x00001000,   // Operator function.
	FUNC_PreOperator	= 0x00000010,	// Unary operator is a prefix operator.


	FUNC_Final			= 0x00000001,	// Function is final (prebindable, non-overridable function).
	FUNC_Defined		= 0x00000002,	// Function has been defined (not just declared).
	FUNC_Iterator		= 0x00000004,	// Function is an iterator.
	FUNC_Latent		    = 0x00000008,	// Function is a latent state function.
	FUNC_Singular       = 0x00000020,   // Function cannot be reentered.
	FUNC_Net            = 0x00000040,   // Function is network-replicated.
	FUNC_NetReliable    = 0x00000080,   // Function should be sent reliably on the network.
	FUNC_Simulated		= 0x00000100,	// Function executed on the client side.
	FUNC_Exec		    = 0x00000200,	// Executable from command line.
	FUNC_Native			= 0x00000400,	// Native function.
	FUNC_Event          = 0x00000800,   // Event function.
	FUNC_Static         = 0x00002000,   // Static function.
	FUNC_NoExport       = 0x00004000,   // Don't export intrinsic function to C++.
	FUNC_Const          = 0x00008000,   // Function doesn't modify this object.
	FUNC_Invariant      = 0x00010000,   // Return value is purely dependent on parameters; no state dependencies or internal state changes.
	FUNC_Public			= 0x00020000,	// Function is accessible in all classes (if overridden, parameters much remain unchanged).
	FUNC_Private		= 0x00040000,	// Function is accessible only in the class it is defined in (cannot be overriden, but function name may be reused in subclasses.  IOW: if overridden, parameters don't need to match, and Super.Func() cannot be accessed since it's private.)
	FUNC_Protected		= 0x00080000,	// Function is accessible only in the class it is defined in and subclasses (if overridden, parameters much remain unchanged).
	FUNC_Delegate		= 0x00100000,	// Function is actually a delegate.
*/
	
	/*if( !t )
	{ 
		t = new rbToken(token,name,output); 
		AddToken(t); 
	}

	dword idx = 0;
	dword param = 0;
	while( idx < code.Length() )
	{
		unsigned long val;
		if( !code.Mid(idx,2).ToULong(&val,16) )
			throw upexception(wxString::Format(wxT("Unknown Function Token code %x at %s"),dword(token),Arc.DebugInfo().c_str()));

		idx += 2;
		if( val == 0x29 )
		{
			++param;
			if( !code.Mid(idx,4).ToULong(&val,16) )
				throw upexception(wxString::Format(wxT("Unknown Function Token code 2 %x at %s"),dword(token),Arc.DebugInfo().c_str()));

			//val = wxUINT32_SWAP_ALWAYS(val);
			idx += 4;
			wxLogMessage( wxString::Format(wxT("LOAD param %d for %s"),param,PF_SAFESTR(name)));
			AddToken(LoadToken(true));
		}
		else if( val == 0x0b )
		{
			// should be 0x16 = endparams
			AddToken(LoadToken(true));
			wxLogMessage( wxString::Format(wxT("END param for %s"),PF_SAFESTR(name)));
		}
	}

*/
	} 
	catch( upexception& e ) { throw upexception( wxString::Format(wxT(" [%.4x]%s -- %s"), token, PF_SAFESTR(name), e.wwhat().c_str() )); }
	catch( exception& e ) { throw exception( wxString::Format(wxT(" [%.4x]%s -- %hs"), token, PF_SAFESTR(name), e.what()).ToAscii()); }

}

// ============================================================================
//  EOF
// ============================================================================