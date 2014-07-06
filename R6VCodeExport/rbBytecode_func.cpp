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
			RB_TOKEN_CODE("29A301000029A40100000B");
			RB_TOKEN_NAME("$");
			flags=0x00023401;
			break;
		}

		case 0x0071:	// GotoState 
		{
			RB_TOKEN_CODE("298F0000002990000000299100000029920000000B");
			RB_TOKEN_NAME("GotoState");
			flags=0x00020401;
			break;
		}

		case 0x0072:	// == 
		{
			RB_TOKEN_CODE("293E010000293F0100000B");
			RB_TOKEN_NAME("==");
			flags=0x00023401;
			break;
		}

		case 0x0073:	// < 
		{
			RB_TOKEN_CODE("299B010000299C0100000B");
			RB_TOKEN_NAME("<");
			flags=0x00023401;
			break;
		}

		case 0x0074:	// > 
		{
			RB_TOKEN_CODE("299701000029980100000B");
			RB_TOKEN_NAME(">");
			flags=0x00023401;
			break;
		}

		case 0x0075:	// Enable 
		{
			RB_TOKEN_CODE("29750000000B");
			RB_TOKEN_NAME("Enable");
			flags=0x00020401;
			break;
		}

		case 0x0076:	// Disable 
		{
			RB_TOKEN_CODE("29730000000B");
			RB_TOKEN_NAME("Disable");
			flags=0x00020401;
			break;
		}

		case 0x0077:	// != 
		{
			RB_TOKEN_CODE("293A010000293B0100000B");
			RB_TOKEN_NAME("!=");
			flags=0x00023401;
			break;
		}

		case 0x0078:	// <= 
		{
			RB_TOKEN_CODE("299301000029940100000B");
			RB_TOKEN_NAME("<=");
			flags=0x00023401;
			break;
		}

		case 0x0079:	// >= 
		{
			RB_TOKEN_CODE("298F01000029900100000B");
			RB_TOKEN_NAME(">=");
			flags=0x00023401;
			break;
		}

		case 0x007A:	// == 
		{
			RB_TOKEN_CODE("298B010000298C0100000B");
			RB_TOKEN_NAME("==");
			flags=0x00023401;
			break;
		}

		case 0x007B:	// != 
		{
			RB_TOKEN_CODE("298701000029880100000B");
			RB_TOKEN_NAME("!=");
			flags=0x00023401;
			break;
		}

		case 0x007C:	// ~= 
		{
			RB_TOKEN_CODE("298301000029840100000B");
			RB_TOKEN_NAME("~=");
			flags=0x00023401;
			break;
		}

		case 0x007D:	// Len 
		{
			RB_TOKEN_CODE("29760100000B");
			RB_TOKEN_NAME("Len");
			flags=0x00022401;
			break;
		}

		case 0x007E:	// InStr 
		{
			RB_TOKEN_CODE("297201000029730100000B");
			RB_TOKEN_NAME("InStr");
			flags=0x00022401;
			break;
		}

		case 0x007F:	// Mid 
		{
			RB_TOKEN_CODE("296D010000296E010000296F0100000B");
			RB_TOKEN_NAME("Mid");
			flags=0x00022401;
			break;
		}

		case 0x0080:	// Left 
		{
			RB_TOKEN_CODE("2969010000296A0100000B");
			RB_TOKEN_NAME("Left");
			flags=0x00022401;
			break;
		}

		case 0x0081:	// ! 
		{
			RB_TOKEN_CODE("29560000000B");
			RB_TOKEN_NAME("!");
			flags=0x00023411;
			break;
		}

		case 0x0082:	// && 
		{
			RB_TOKEN_CODE("298900000029850000000B");
			RB_TOKEN_NAME("&&");
			flags=0x00023401;
			break;
		}

		case 0x0083:	// ^^ 
		{
			RB_TOKEN_CODE("29B300000029940000000B");
			RB_TOKEN_NAME("^^");
			flags=0x00023401;
			break;
		}

		case 0x0084:	// || 
		{
			RB_TOKEN_CODE("29BF00000029BB0000000B");
			RB_TOKEN_NAME("||");
			flags=0x00023401;
			break;
		}

		case 0x0085:	// *= 
		{
			RB_TOKEN_CODE("29C200000029C10000000B");
			RB_TOKEN_NAME("*=");
			flags=0x00423401;
			break;
		}

		case 0x0086:	// /= 
		{
			RB_TOKEN_CODE("29C500000029C40000000B");
			RB_TOKEN_NAME("/=");
			flags=0x00423401;
			break;
		}

		case 0x0087:	// += 
		{
			RB_TOKEN_CODE("29C800000029C70000000B");
			RB_TOKEN_NAME("+=");
			flags=0x00423401;
			break;
		}

		case 0x0088:	// -= 
		{
			RB_TOKEN_CODE("29E200000029E10000000B");
			RB_TOKEN_NAME("-=");
			flags=0x00423401;
			break;
		}

		case 0x0089:	// ++ 
		{
			RB_TOKEN_CODE("29EB0000000B");
			RB_TOKEN_NAME("++");
			flags=0x00423411;
			break;
		}

		case 0x008A:	// -- 
		{
			RB_TOKEN_CODE("29F30000000B");
			RB_TOKEN_NAME("--");
			flags=0x00423411;
			break;
		}

		case 0x008B:	// ++ 
		{
			RB_TOKEN_CODE("29F80000000B");
			RB_TOKEN_NAME("++");
			flags=0x00423401;
			break;
		}

		case 0x008C:	// -- 
		{
			RB_TOKEN_CODE("29FE0000000B");
			RB_TOKEN_NAME("--");
			flags=0x00423401;
			break;
		}

		case 0x008D:	// ~ 
		{
			RB_TOKEN_CODE("29000100000B");
			RB_TOKEN_NAME("~");
			flags=0x00023411;
			break;
		}

		case 0x008E:	// == 
		{
			RB_TOKEN_CODE("291802000029190200000B");
			RB_TOKEN_NAME("==");
			flags=0x00023401;
			break;
		}

		case 0x008F:	// - 
		{
			RB_TOKEN_CODE("29020100000B");
			RB_TOKEN_NAME("-");
			flags=0x00023411;
			break;
		}

		case 0x0090:	// * 
		{
			RB_TOKEN_CODE("290F010000290C0100000B");
			RB_TOKEN_NAME("*");
			flags=0x00023401;
			break;
		}

		case 0x0091:	// / 
		{
			RB_TOKEN_CODE("291A01000029160100000B");
			RB_TOKEN_NAME("/");
			flags=0x00023401;
			break;
		}

		case 0x0092:	// + 
		{
			RB_TOKEN_CODE("292501000029210100000B");
			RB_TOKEN_NAME("+");
			flags=0x00023401;
			break;
		}

		case 0x0093:	// - 
		{
			RB_TOKEN_CODE("2931010000292D0100000B");
			RB_TOKEN_NAME("-");
			flags=0x00023401;
			break;
		}

		case 0x0094:	// << 
		{
			RB_TOKEN_CODE("293C01000029380100000B");
			RB_TOKEN_NAME("<<");
			flags=0x00023401;
			break;
		}

		case 0x0095:	// >> 
		{
			RB_TOKEN_CODE("294C01000029430100000B");
			RB_TOKEN_NAME(">>");
			flags=0x00023401;
			break;
		}

		case 0x0096:	// < 
		{
			RB_TOKEN_CODE("296301000029600100000B");
			RB_TOKEN_NAME("<");
			flags=0x00023401;
			break;
		}

		case 0x0097:	// > 
		{
			RB_TOKEN_CODE("2970010000296B0100000B");
			RB_TOKEN_NAME(">");
			flags=0x00023401;
			break;
		}

		case 0x0098:	// <= 
		{
			RB_TOKEN_CODE("297B01000029770100000B");
			RB_TOKEN_NAME("<=");
			flags=0x00023401;
			break;
		}

		case 0x0099:	// >= 
		{
			RB_TOKEN_CODE("298901000029850100000B");
			RB_TOKEN_NAME(">=");
			flags=0x00023401;
			break;
		}

		case 0x009A:	// == 
		{
			RB_TOKEN_CODE("299501000029910100000B");
			RB_TOKEN_NAME("==");
			flags=0x00023401;
			break;
		}

		case 0x009B:	// != 
		{
			RB_TOKEN_CODE("29A1010000299D0100000B");
			RB_TOKEN_NAME("!=");
			flags=0x00023401;
			break;
		}

		case 0x009C:	// & 
		{
			RB_TOKEN_CODE("29C301000029C00100000B");
			RB_TOKEN_NAME("&");
			flags=0x00023401;
			break;
		}

		case 0x009D:	// ^ 
		{
			RB_TOKEN_CODE("29D501000029CF0100000B");
			RB_TOKEN_NAME("^");
			flags=0x00023401;
			break;
		}

		case 0x009E:	// | 
		{
			RB_TOKEN_CODE("29E001000029DD0100000B");
			RB_TOKEN_NAME("|");
			flags=0x00023401;
			break;
		}

		case 0x009F:	// *= 
		{
			RB_TOKEN_CODE("29F601000029F20100000B");
			RB_TOKEN_NAME("*=");
			flags=0x00423401;
			break;
		}

		case 0x00A0:	// /= 
		{
			RB_TOKEN_CODE("290202000029FE0100000B");
			RB_TOKEN_NAME("/=");
			flags=0x00423401;
			break;
		}

		case 0x00A1:	// += 
		{
			RB_TOKEN_CODE("290E020000290A0200000B");
			RB_TOKEN_NAME("+=");
			flags=0x00423401;
			break;
		}

		case 0x00A2:	// -= 
		{
			RB_TOKEN_CODE("291A02000029160200000B");
			RB_TOKEN_NAME("-=");
			flags=0x00423401;
			break;
		}

		case 0x00A3:	// ++ 
		{
			RB_TOKEN_CODE("29230200000B");
			RB_TOKEN_NAME("++");
			flags=0x00423411;
			break;
		}

		case 0x00A4:	// -- 
		{
			RB_TOKEN_CODE("292B0200000B");
			RB_TOKEN_NAME("--");
			flags=0x00423411;
			break;
		}

		case 0x00A5:	// ++ 
		{
			RB_TOKEN_CODE("29320200000B");
			RB_TOKEN_NAME("++");
			flags=0x00423401;
			break;
		}

		case 0x00A6:	// -- 
		{
			RB_TOKEN_CODE("293A0200000B");
			RB_TOKEN_NAME("--");
			flags=0x00423401;
			break;
		}

		case 0x00A7:	// Rand 
		{
			RB_TOKEN_CODE("29400200000B");
			RB_TOKEN_NAME("Rand");
			flags=0x00022401;
			break;
		}

		case 0x00A8:	// @ 
		{
			RB_TOKEN_CODE("299F01000029A00100000B");
			RB_TOKEN_NAME("@");
			flags=0x00023401;
			break;
		}

		case 0x00A9:	// - 
		{
			RB_TOKEN_CODE("296E0200000B");
			RB_TOKEN_NAME("-");
			flags=0x00023411;
			break;
		}

		case 0x00AA:	// ** 
		{
			RB_TOKEN_CODE("297A02000029760200000B");
			RB_TOKEN_NAME("**");
			flags=0x00023401;
			break;
		}

		case 0x00AB:	// * 
		{
			RB_TOKEN_CODE("298602000029820200000B");
			RB_TOKEN_NAME("*");
			flags=0x00023401;
			break;
		}

		case 0x00AC:	// / 
		{
			RB_TOKEN_CODE("2992020000298E0200000B");
			RB_TOKEN_NAME("/");
			flags=0x00023401;
			break;
		}

		case 0x00AD:	// % 
		{
			RB_TOKEN_CODE("299E020000299A0200000B");
			RB_TOKEN_NAME("%");
			flags=0x00023401;
			break;
		}

		case 0x00AE:	// + 
		{
			RB_TOKEN_CODE("29A902000029A60200000B");
			RB_TOKEN_NAME("+");
			flags=0x00023401;
			break;
		}

		case 0x00AF:	// - 
		{
			RB_TOKEN_CODE("29B802000029B40200000B");
			RB_TOKEN_NAME("-");
			flags=0x00023401;
			break;
		}

		case 0x00B0:	// < 
		{
			RB_TOKEN_CODE("29CA02000029C50200000B");
			RB_TOKEN_NAME("<");
			flags=0x00023401;
			break;
		}

		case 0x00B1:	// > 
		{
			RB_TOKEN_CODE("29CF02000029CE0200000B");
			RB_TOKEN_NAME(">");
			flags=0x00023401;
			break;
		}

		case 0x00B2:	// <= 
		{
			RB_TOKEN_CODE("29DB02000029D50200000B");
			RB_TOKEN_NAME("<=");
			flags=0x00023401;
			break;
		}

		case 0x00B3:	// >= 
		{
			RB_TOKEN_CODE("29EC02000029E70200000B");
			RB_TOKEN_NAME(">=");
			flags=0x00023401;
			break;
		}

		case 0x00B4:	// == 
		{
			RB_TOKEN_CODE("29F902000029F50200000B");
			RB_TOKEN_NAME("==");
			flags=0x00023401;
			break;
		}

		case 0x00B5:	// != 
		{
			RB_TOKEN_CODE("290B03000029070300000B");
			RB_TOKEN_NAME("!=");
			flags=0x00023401;
			break;
		}

		case 0x00B6:	// *= 
		{
			RB_TOKEN_CODE("291303000029100300000B");
			RB_TOKEN_NAME("*=");
			flags=0x00423401;
			break;
		}

		case 0x00B7:	// /= 
		{
			RB_TOKEN_CODE("291C03000029190300000B");
			RB_TOKEN_NAME("/=");
			flags=0x00423401;
			break;
		}

		case 0x00B8:	// += 
		{
			RB_TOKEN_CODE("292203000029230300000B");
			RB_TOKEN_NAME("+=");
			flags=0x00423401;
			break;
		}

		case 0x00B9:	// -= 
		{
			RB_TOKEN_CODE("291E030000291F0300000B");
			RB_TOKEN_NAME("-=");
			flags=0x00423401;
			break;
		}

		case 0x00BA:	// Abs 
		{
			RB_TOKEN_CODE("291B0300000B");
			RB_TOKEN_NAME("Abs");
			flags=0x00022401;
			break;
		}

		case 0x00BB:	// Sin 
		{
			RB_TOKEN_CODE("29180300000B");
			RB_TOKEN_NAME("Sin");
			flags=0x00022401;
			break;
		}

		case 0x00BC:	// Cos 
		{
			RB_TOKEN_CODE("29120300000B");
			RB_TOKEN_NAME("Cos");
			flags=0x00022401;
			break;
		}

		case 0x00BD:	// Tan 
		{
			RB_TOKEN_CODE("290C0300000B");
			RB_TOKEN_NAME("Tan");
			flags=0x00022401;
			break;
		}

		case 0x00BE:	// Atan 
		{
			RB_TOKEN_CODE("2909030000290A0300000B");
			RB_TOKEN_NAME("Atan");
			flags=0x00022401;
			break;
		}

		case 0x00BF:	// Exp 
		{
			RB_TOKEN_CODE("29060300000B");
			RB_TOKEN_NAME("Exp");
			flags=0x00022401;
			break;
		}

		case 0x00C0:	// Loge 
		{
			RB_TOKEN_CODE("29030300000B");
			RB_TOKEN_NAME("Loge");
			flags=0x00022401;
			break;
		}

		case 0x00C1:	// Sqrt 
		{
			RB_TOKEN_CODE("29000300000B");
			RB_TOKEN_NAME("Sqrt");
			flags=0x00022401;
			break;
		}

		case 0x00C2:	// Square 
		{
			RB_TOKEN_CODE("29FD0200000B");
			RB_TOKEN_NAME("Square");
			flags=0x00022401;
			break;
		}

		case 0x00C3:	// FRand 
		{
			RB_TOKEN_CODE("0B");
			RB_TOKEN_NAME("FRand");
			flags=0x00022401;
			break;
		}

		case 0x00C4:	// >>> 
		{
			RB_TOKEN_CODE("295A01000029570100000B");
			RB_TOKEN_NAME(">>>");
			flags=0x00023401;
			break;
		}

		case 0x00C5:	// IsA 
		{
			RB_TOKEN_CODE("29330100000B");
			RB_TOKEN_NAME("IsA");
			flags=0x00020401;
			break;
		}

		case 0x00C9:	// Repl 
		{
			RB_TOKEN_CODE("29530100002954010000295501000029560100000B");
			RB_TOKEN_NAME("Repl");
			flags=0x00022401;
			break;
		}

		case 0x00CB:	// != 
		{
			RB_TOKEN_CODE("291402000029150200000B");
			RB_TOKEN_NAME("!=");
			flags=0x00023401;
			break;
		}

		case 0x00D2:	// ~= 
		{
			RB_TOKEN_CODE("290103000029FE0200000B");
			RB_TOKEN_NAME("~=");
			flags=0x00023401;
			break;
		}

		case 0x00D3:	// - 
		{
			RB_TOKEN_CODE("29A80200000B");
			RB_TOKEN_NAME("-");
			flags=0x00023411;
			break;
		}

		case 0x00D4:	// * 
		{
			RB_TOKEN_CODE("29A402000029A50200000B");
			RB_TOKEN_NAME("*");
			flags=0x00023401;
			break;
		}

		case 0x00D5:	// * 
		{
			RB_TOKEN_CODE("29A002000029A10200000B");
			RB_TOKEN_NAME("*");
			flags=0x00023401;
			break;
		}

		case 0x00D6:	// / 
		{
			RB_TOKEN_CODE("299802000029990200000B");
			RB_TOKEN_NAME("/");
			flags=0x00023401;
			break;
		}

		case 0x00D7:	// + 
		{
			RB_TOKEN_CODE("299402000029950200000B");
			RB_TOKEN_NAME("+");
			flags=0x00023401;
			break;
		}

		case 0x00D8:	// - 
		{
			RB_TOKEN_CODE("299002000029910200000B");
			RB_TOKEN_NAME("-");
			flags=0x00023401;
			break;
		}

		case 0x00D9:	// == 
		{
			RB_TOKEN_CODE("298402000029850200000B");
			RB_TOKEN_NAME("==");
			flags=0x00023401;
			break;
		}

		case 0x00DA:	// != 
		{
			RB_TOKEN_CODE("298002000029810200000B");
			RB_TOKEN_NAME("!=");
			flags=0x00023401;
			break;
		}

		case 0x00DB:	// Dot 
		{
			RB_TOKEN_CODE("297C020000297D0200000B");
			RB_TOKEN_NAME("Dot");
			flags=0x00023401;
			break;
		}

		case 0x00DC:	// Cross 
		{
			RB_TOKEN_CODE("297802000029790200000B");
			RB_TOKEN_NAME("Cross");
			flags=0x00023401;
			break;
		}

		case 0x00DD:	// *= 
		{
			RB_TOKEN_CODE("297402000029750200000B");
			RB_TOKEN_NAME("*=");
			flags=0x00423401;
			break;
		}

		case 0x00DE:	// /= 
		{
			RB_TOKEN_CODE("296C020000296D0200000B");
			RB_TOKEN_NAME("/=");
			flags=0x00423401;
			break;
		}

		case 0x00DF:	// += 
		{
			RB_TOKEN_CODE("296802000029690200000B");
			RB_TOKEN_NAME("+=");
			flags=0x00423401;
			break;
		}

		case 0x00E0:	// -= 
		{
			RB_TOKEN_CODE("296402000029650200000B");
			RB_TOKEN_NAME("-=");
			flags=0x00423401;
			break;
		}

		case 0x00E1:	// VSize 
		{
			RB_TOKEN_CODE("29420200000B");
			RB_TOKEN_NAME("VSize");
			flags=0x00022401;
			break;
		}

		case 0x00E2:	// Normal 
		{
			RB_TOKEN_CODE("29390200000B");
			RB_TOKEN_NAME("Normal");
			flags=0x00022401;
			break;
		}

		case 0x00E5:	// GetAxes 
		{
			RB_TOKEN_CODE("29E701000029E801000029E901000029EA0100000B");
			RB_TOKEN_NAME("GetAxes");
			flags=0x00422401;
			break;
		}

		case 0x00E6:	// GetUnAxes 
		{
			RB_TOKEN_CODE("29E201000029E301000029E401000029E50100000B");
			RB_TOKEN_NAME("GetUnAxes");
			flags=0x00422401;
			break;
		}

		case 0x00E7:	// Log 
		{
			RB_TOKEN_CODE("29BC00000029BD00000029BE0000000B");
			RB_TOKEN_NAME("Log");
			flags=0x00022401;
			break;
		}

		case 0x00E8:	// Warn 
		{
			RB_TOKEN_CODE("29BA0000000B");
			RB_TOKEN_NAME("Warn");
			flags=0x00022401;
			break;
		}

		case 0x00EA:	// Right 
		{
			RB_TOKEN_CODE("296501000029660100000B");
			RB_TOKEN_NAME("Right");
			flags=0x00022401;
			break;
		}

		case 0x00EB:	// Caps 
		{
			RB_TOKEN_CODE("29620100000B");
			RB_TOKEN_NAME("Caps");
			flags=0x00022401;
			break;
		}

		case 0x00EC:	// Chr 
		{
			RB_TOKEN_CODE("29B60200000B");
			RB_TOKEN_NAME("Chr");
			flags=0x00022401;
			break;
		}

		case 0x00ED:	// Asc 
		{
			RB_TOKEN_CODE("29590100000B");
			RB_TOKEN_NAME("Asc");
			flags=0x00022401;
			break;
		}

		case 0x00EE:	// Locs 
		{
			RB_TOKEN_CODE("295F0100000B");
			RB_TOKEN_NAME("Locs");
			flags=0x00022401;
			break;
		}

		case 0x00F2:	// == 
		{
			RB_TOKEN_CODE("296700000029620000000B");
			RB_TOKEN_NAME("==");
			flags=0x00023401;
			break;
		}

		case 0x00F3:	// != 
		{
			RB_TOKEN_CODE("297B00000029710000000B");
			RB_TOKEN_NAME("!=");
			flags=0x00023401;
			break;
		}

		case 0x00F4:	// FMin 
		{
			RB_TOKEN_CODE("29F702000029F80200000B");
			RB_TOKEN_NAME("FMin");
			flags=0x00022401;
			break;
		}

		case 0x00F5:	// FMax 
		{
			RB_TOKEN_CODE("29F302000029F40200000B");
			RB_TOKEN_NAME("FMax");
			flags=0x00022401;
			break;
		}

		case 0x00F6:	// FClamp 
		{
			RB_TOKEN_CODE("29EE02000029EF02000029F00200000B");
			RB_TOKEN_NAME("FClamp");
			flags=0x00022401;
			break;
		}

		case 0x00F7:	// Lerp 
		{
			RB_TOKEN_CODE("29E902000029EA02000029EB0200000B");
			RB_TOKEN_NAME("Lerp");
			flags=0x00022401;
			break;
		}

		case 0x00F9:	// Min 
		{
			RB_TOKEN_CODE("294B02000029470200000B");
			RB_TOKEN_NAME("Min");
			flags=0x00022401;
			break;
		}

		case 0x00FA:	// Max 
		{
			RB_TOKEN_CODE("295702000029530200000B");
			RB_TOKEN_NAME("Max");
			flags=0x00022401;
			break;
		}

		case 0x00FB:	// Clamp 
		{
			RB_TOKEN_CODE("29660200002962020000295F0200000B");
			RB_TOKEN_NAME("Clamp");
			flags=0x00022401;
			break;
		}

		case 0x00FC:	// VRand 
		{
			RB_TOKEN_CODE("0B");
			RB_TOKEN_NAME("VRand");
			flags=0x00022401;
			break;
		}

		case 0x00FE:	// == 
		{
			RB_TOKEN_CODE("292F01000029300100000B");
			RB_TOKEN_NAME("==");
			flags=0x00023401;
			break;
		}

		case 0x00FF:	// != 
		{
			RB_TOKEN_CODE("292B010000292C0100000B");
			RB_TOKEN_NAME("!=");
			flags=0x00023401;
			break;
		}

		case 0x0100:	// Sleep 
		{
			RB_TOKEN_CODE("29000B00000B");
			RB_TOKEN_NAME("Sleep");
			flags=0x00020409;
			break;
		}

		case 0x0102:	// ClassIsChildOf 
		{
			RB_TOKEN_CODE("293601000029370100000B");
			RB_TOKEN_NAME("ClassIsChildOf");
			flags=0x00022401;
			break;
		}

		case 0x0105:	// FinishAnim 
		{
			RB_TOKEN_CODE("29FF0A00000B");
			RB_TOKEN_NAME("FinishAnim");
			flags=0x00020409;
			break;
		}

		case 0x0106:	// SetCollision 
		{
			RB_TOKEN_CODE("29EA0A000029EB0A00000B");
			RB_TOKEN_NAME("SetCollision");
			flags=0x00020401;
			break;
		}

		case 0x010A:	// Move 
		{
			RB_TOKEN_CODE("29DC0A00000B");
			RB_TOKEN_NAME("Move");
			flags=0x00020401;
			break;
		}

		case 0x010B:	// SetLocation 
		{
			RB_TOKEN_CODE("29D80A000029D90A00000B");
			RB_TOKEN_NAME("SetLocation");
			flags=0x00020401;
			break;
		}

		case 0x010E:	// + 
		{
			RB_TOKEN_CODE("290401000029050100000B");
			RB_TOKEN_NAME("+");
			flags=0x00023401;
			break;
		}

		case 0x010F:	// - 
		{
			RB_TOKEN_CODE("29FB00000029FC0000000B");
			RB_TOKEN_NAME("-");
			flags=0x00023401;
			break;
		}

		case 0x0110:	// SetOwner 
		{
			RB_TOKEN_CODE("29600B00000B");
			RB_TOKEN_NAME("SetOwner");
			flags=0x00020401;
			break;
		}

		case 0x0113:	// << 
		{
			RB_TOKEN_CODE("298C020000298D0200000B");
			RB_TOKEN_NAME("<<");
			flags=0x00023401;
			break;
		}

		case 0x0114:	// >> 
		{
			RB_TOKEN_CODE("298802000029890200000B");
			RB_TOKEN_NAME(">>");
			flags=0x00023401;
			break;
		}

		case 0x0115:	// Trace 
		{
			RB_TOKEN_CODE("292B0D000029290D000029280D000029270D000029260D000029240D000029230D000029180D00000B");
			RB_TOKEN_NAME("Trace");
			flags=0x00420401;
			break;
		}

		case 0x0116:	// Spawn 
		{
			RB_TOKEN_CODE("29740D000029730D000029720D000029710D0000296F0D0000296E0D0000296D0D00000B");
			RB_TOKEN_NAME("Spawn");
			flags=0x00020401;
			break;
		}

		case 0x0117:	// Destroy 
		{
			RB_TOKEN_CODE("0B");
			RB_TOKEN_NAME("Destroy");
			flags=0x00020401;
			break;
		}

		case 0x0118:	// SetTimer 
		{
			RB_TOKEN_CODE("297E0D0000297C0D0000297B0D00000B");
			RB_TOKEN_NAME("SetTimer");
			flags=0x00020401;
			break;
		}

		case 0x0119:	// IsInState 
		{
			RB_TOKEN_CODE("298B000000298C0000000B");
			RB_TOKEN_NAME("IsInState");
			flags=0x00020401;
			break;
		}

		case 0x011B:	// SetCollisionSize 
		{
			RB_TOKEN_CODE("29E70A000029E80A00000B");
			RB_TOKEN_NAME("SetCollisionSize");
			flags=0x00020401;
			break;
		}

		case 0x011C:	// GetStateName 
		{
			RB_TOKEN_CODE("0B");
			RB_TOKEN_NAME("GetStateName");
			flags=0x00020401;
			break;
		}

		case 0x011F:	// * 
		{
			RB_TOKEN_CODE("291002000029110200000B");
			RB_TOKEN_NAME("*");
			flags=0x00023401;
			break;
		}

		case 0x0120:	// * 
		{
			RB_TOKEN_CODE("290C020000290D0200000B");
			RB_TOKEN_NAME("*");
			flags=0x00023401;
			break;
		}

		case 0x0121:	// / 
		{
			RB_TOKEN_CODE("290802000029090200000B");
			RB_TOKEN_NAME("/");
			flags=0x00023401;
			break;
		}

		case 0x0122:	// *= 
		{
			RB_TOKEN_CODE("290402000029050200000B");
			RB_TOKEN_NAME("*=");
			flags=0x00423401;
			break;
		}

		case 0x0123:	// /= 
		{
			RB_TOKEN_CODE("290002000029010200000B");
			RB_TOKEN_NAME("/=");
			flags=0x00423401;
			break;
		}

		case 0x0128:	// * 
		{
			RB_TOKEN_CODE("299C020000299D0200000B");
			RB_TOKEN_NAME("*");
			flags=0x00023401;
			break;
		}

		case 0x0129:	// *= 
		{
			RB_TOKEN_CODE("297002000029710200000B");
			RB_TOKEN_NAME("*=");
			flags=0x00423401;
			break;
		}

		case 0x012A:	// SetBase 
		{
			RB_TOKEN_CODE("29490B000029480B000029470B000029420B00000B");
			RB_TOKEN_NAME("SetBase");
			flags=0x00020401;
			break;
		}

		case 0x012B:	// SetRotation 
		{
			RB_TOKEN_CODE("29D40A000029D50A00000B");
			RB_TOKEN_NAME("SetRotation");
			flags=0x00020401;
			break;
		}

		case 0x012C:	// MirrorVectorByNormal 
		{
			RB_TOKEN_CODE("2929020000292A0200000B");
			RB_TOKEN_NAME("MirrorVectorByNormal");
			flags=0x00022401;
			break;
		}

		case 0x0130:	// AllActors 
		{
			RB_TOKEN_CODE("294B0E000029490E00000B");
			RB_TOKEN_NAME("AllActors");
			flags=0x00420405;
			break;
		}

		case 0x0131:	// ChildActors 
		{
			RB_TOKEN_CODE("29570E000029520E00000B");
			RB_TOKEN_NAME("ChildActors");
			flags=0x00420405;
			break;
		}

		case 0x0132:	// BasedActors 
		{
			RB_TOKEN_CODE("295B0E0000295A0E00000B");
			RB_TOKEN_NAME("BasedActors");
			flags=0x00420405;
			break;
		}

		case 0x0133:	// TouchingActors 
		{
			RB_TOKEN_CODE("295F0E0000295D0E00000B");
			RB_TOKEN_NAME("TouchingActors");
			flags=0x00420405;
			break;
		}

		case 0x0134:	// ZoneActors 
		{
			RB_TOKEN_CODE("294418000029451800000B");
			RB_TOKEN_NAME("ZoneActors");
			flags=0x00420405;
			break;
		}

		case 0x0135:	// TraceActors 
		{
			RB_TOKEN_CODE("296A0E000029680E000029670E000029650E000029640E000029620E000029610E00000B");
			RB_TOKEN_NAME("TraceActors");
			flags=0x00420405;
			break;
		}

		case 0x0137:	// VisibleActors 
		{
			RB_TOKEN_CODE("296F0E0000296E0E0000296D0E0000296C0E00000B");
			RB_TOKEN_NAME("VisibleActors");
			flags=0x00420405;
			break;
		}

		case 0x0138:	// VisibleCollidingActors 
		{
			RB_TOKEN_CODE("29770E000029760E000029750E000029730E000029720E00000B");
			RB_TOKEN_NAME("VisibleCollidingActors");
			flags=0x00420405;
			break;
		}

		case 0x0139:	// DynamicActors 
		{
			RB_TOKEN_CODE("294E0E0000294D0E00000B");
			RB_TOKEN_NAME("DynamicActors");
			flags=0x00420405;
			break;
		}

		case 0x013C:	// + 
		{
			RB_TOKEN_CODE("29FC01000029FD0100000B");
			RB_TOKEN_NAME("+");
			flags=0x00023401;
			break;
		}

		case 0x013D:	// - 
		{
			RB_TOKEN_CODE("29F801000029F90100000B");
			RB_TOKEN_NAME("-");
			flags=0x00023401;
			break;
		}

		case 0x013E:	// += 
		{
			RB_TOKEN_CODE("29F401000029F50100000B");
			RB_TOKEN_NAME("+=");
			flags=0x00423401;
			break;
		}

		case 0x013F:	// -= 
		{
			RB_TOKEN_CODE("29F001000029F10100000B");
			RB_TOKEN_NAME("-=");
			flags=0x00423401;
			break;
		}

		case 0x0140:	// RotRand 
		{
			RB_TOKEN_CODE("29DF0100000B");
			RB_TOKEN_NAME("RotRand");
			flags=0x00022401;
			break;
		}

		case 0x0141:	// CollidingActors 
		{
			RB_TOKEN_CODE("297F0E0000297D0E0000297B0E0000297A0E00000B");
			RB_TOKEN_NAME("CollidingActors");
			flags=0x00420405;
			break;
		}

		case 0x0142:	// $= 
		{
			RB_TOKEN_CODE("298001000029810100000B");
			RB_TOKEN_NAME("$=");
			flags=0x00423401;
			break;
		}

		case 0x0143:	// @= 
		{
			RB_TOKEN_CODE("297C010000297D0100000B");
			RB_TOKEN_NAME("@=");
			flags=0x00423401;
			break;
		}

		case 0x0144:	// -= 
		{
			RB_TOKEN_CODE("2979010000297A0100000B");
			RB_TOKEN_NAME("-=");
			flags=0x00423401;
			break;
		}

		case 0x01D0:	// StrLen 
		{
			RB_TOKEN_CODE("29081100002909110000290A1100000B");
			RB_TOKEN_NAME("StrLen");
			flags=0x00420401;
			break;
		}

		case 0x01D1:	// DrawText 
		{
			RB_TOKEN_CODE("291611000029171100000B");
			RB_TOKEN_NAME("DrawText");
			flags=0x00020401;
			break;
		}

		case 0x01D2:	// DrawTile 
		{
			RB_TOKEN_CODE("290010000029011000002902100000290310000029041000002905100000290A1000000B");
			RB_TOKEN_NAME("DrawTile");
			flags=0x00020401;
			break;
		}

		case 0x01D5:	// DrawTextClipped 
		{
			RB_TOKEN_CODE("291A1100000B");
			RB_TOKEN_NAME("DrawTextClipped");
			flags=0x00020401;
			break;
		}

		case 0x01D6:	// TextSize 
		{
			RB_TOKEN_CODE("290C110000290D110000290E1100000B");
			RB_TOKEN_NAME("TextSize");
			flags=0x00420401;
			break;
		}

		case 0x01F4:	// MoveTo 
		{
			RB_TOKEN_CODE("296F1D0000296E1D0000296D1D00000B");
			RB_TOKEN_NAME("MoveTo");
			flags=0x00020409;
			break;
		}

		case 0x01F6:	// MoveToward 
		{
			RB_TOKEN_CODE("29761D000029751D000029741D000029731D000029721D00000B");
			RB_TOKEN_NAME("MoveToward");
			flags=0x00020409;
			break;
		}

		case 0x01FC:	// FinishRotation 
		{
			RB_TOKEN_CODE("0B");
			RB_TOKEN_NAME("FinishRotation");
			flags=0x00020409;
			break;
		}

		case 0x0200:	// MakeNoise 
		{
			RB_TOKEN_CODE("29FD0D000029EE0D000029ED0D000029EC0D00000B");
			RB_TOKEN_NAME("MakeNoise");
			flags=0x00020401;
			break;
		}

		case 0x0202:	// LineOfSightTo 
		{
			RB_TOKEN_CODE("295C1D0000295B1D00000B");
			RB_TOKEN_NAME("LineOfSightTo");
			flags=0x00020401;
			break;
		}

		case 0x0205:	// FindPathToward 
		{
			RB_TOKEN_CODE("297E1D0000297C1D00000B");
			RB_TOKEN_NAME("FindPathToward");
			flags=0x00020401;
			break;
		}

		case 0x0206:	// FindPathTo 
		{
			RB_TOKEN_CODE("297A1D00000B");
			RB_TOKEN_NAME("FindPathTo");
			flags=0x00020401;
			break;
		}

		case 0x0208:	// ActorReachable 
		{
			RB_TOKEN_CODE("29A51D00000B");
			RB_TOKEN_NAME("ActorReachable");
			flags=0x00020401;
			break;
		}

		case 0x0209:	// PointReachable 
		{
			RB_TOKEN_CODE("29881D00000B");
			RB_TOKEN_NAME("PointReachable");
			flags=0x00020401;
			break;
		}

		case 0x020C:	// FindStairRotation 
		{
			RB_TOKEN_CODE("29411500000B");
			RB_TOKEN_NAME("FindStairRotation");
			flags=0x00020401;
			break;
		}

		case 0x020D:	// FindRandomDest 
		{
			RB_TOKEN_CODE("0B");
			RB_TOKEN_NAME("FindRandomDest");
			flags=0x00020401;
			break;
		}

		case 0x020E:	// PickWallAdjust 
		{
			RB_TOKEN_CODE("29A81D00000B");
			RB_TOKEN_NAME("PickWallAdjust");
			flags=0x00020401;
			break;
		}

		case 0x020F:	// WaitForLanding 
		{
			RB_TOKEN_CODE("29AA1D00000B");
			RB_TOKEN_NAME("WaitForLanding");
			flags=0x00020409;
			break;
		}

		case 0x0213:	// PickTarget 
		{
			RB_TOKEN_CODE("29661D000029651D000029641D000029631D000029611D000029601D00000B");
			RB_TOKEN_NAME("PickTarget");
			flags=0x00420401;
			break;
		}

		case 0x0214:	// PlayerCanSeeMe 
		{
			RB_TOKEN_CODE("0B");
			RB_TOKEN_NAME("PlayerCanSeeMe");
			flags=0x00020401;
			break;
		}

		case 0x0215:	// CanSee 
		{
			RB_TOKEN_CODE("295E1D00000B");
			RB_TOKEN_NAME("CanSee");
			flags=0x00020401;
			break;
		}

		case 0x0218:	// SaveConfig 
		{
			RB_TOKEN_CODE("0B");
			RB_TOKEN_NAME("SaveConfig");
			flags=0x00020401;
			break;
		}

		case 0x0222:	// UpdateURL 
		{
			RB_TOKEN_CODE("2933150000293215000029311500000B");
			RB_TOKEN_NAME("UpdateURL");
			flags=0x00020401;
			break;
		}

		case 0x0223:	// GetURLMap 
		{
			RB_TOKEN_CODE("0B");
			RB_TOKEN_NAME("GetURLMap");
			flags=0x00020401;
			break;
		}

		case 0x0224:	// FastTrace 
		{
			RB_TOKEN_CODE("293F0D0000293E0D00000B");
			RB_TOKEN_NAME("FastTrace");
			flags=0x00020401;
			break;
		}

		case 0x0225:	// - 
		{
			RB_TOKEN_CODE("29990E000029980E00000B");
			RB_TOKEN_NAME("-");
			flags=0x00023401;
			break;
		}

		case 0x0226:	// * 
		{
			RB_TOKEN_CODE("29A40E0000299D0E00000B");
			RB_TOKEN_NAME("*");
			flags=0x00023401;
			break;
		}

		case 0x0227:	// + 
		{
			RB_TOKEN_CODE("29A90E000029A70E00000B");
			RB_TOKEN_NAME("+");
			flags=0x00023401;
			break;
		}

		case 0x0228:	// * 
		{
			RB_TOKEN_CODE("29B20E000029B00E00000B");
			RB_TOKEN_NAME("*");
			flags=0x00023401;
			break;
		}

		case 0x0258:	// - 
		{
			RB_TOKEN_CODE("29610200000B");
			RB_TOKEN_NAME("-");
			flags=0x00023411;
			break;
		}

		case 0x0259:	// * 
		{
			RB_TOKEN_CODE("295D020000295E0200000B");
			RB_TOKEN_NAME("*");
			flags=0x00023401;
			break;
		}

		case 0x025A:	// * 
		{
			RB_TOKEN_CODE("2959020000295A0200000B");
			RB_TOKEN_NAME("*");
			flags=0x00023401;
			break;
		}

		case 0x025B:	// + 
		{
			RB_TOKEN_CODE("295502000029560200000B");
			RB_TOKEN_NAME("+");
			flags=0x00023401;
			break;
		}

		case 0x025C:	// - 
		{
			RB_TOKEN_CODE("295102000029520200000B");
			RB_TOKEN_NAME("-");
			flags=0x00023401;
			break;
		}

		case 0x025D:	// *= 
		{
			RB_TOKEN_CODE("294D020000294E0200000B");
			RB_TOKEN_NAME("*=");
			flags=0x00423401;
			break;
		}

		case 0x025E:	// += 
		{
			RB_TOKEN_CODE("2949020000294A0200000B");
			RB_TOKEN_NAME("+=");
			flags=0x00423401;
			break;
		}

		case 0x025F:	// -= 
		{
			RB_TOKEN_CODE("294502000029460200000B");
			RB_TOKEN_NAME("-=");
			flags=0x00423401;
			break;
		}

		case 0x0384:	// LogEx 
		{
			RB_TOKEN_CODE("29AA00000029AB00000029AC0000000B");
			RB_TOKEN_NAME("LogEx");
			flags=0x00022401;
			break;
		}

		case 0x0385:	// LogFatal 
		{
			RB_TOKEN_CODE("29A700000029A80000000B");
			RB_TOKEN_NAME("LogFatal");
			flags=0x00022401;
			break;
		}

		case 0x0386:	// LogError 
		{
			RB_TOKEN_CODE("29A400000029A50000000B");
			RB_TOKEN_NAME("LogError");
			flags=0x00022401;
			break;
		}

		case 0x0387:	// LogWarn 
		{
			RB_TOKEN_CODE("29A100000029A20000000B");
			RB_TOKEN_NAME("LogWarn");
			flags=0x00022401;
			break;
		}

		case 0x0388:	// LogInfo 
		{
			RB_TOKEN_CODE("299E000000299F0000000B");
			RB_TOKEN_NAME("LogInfo");
			flags=0x00022401;
			break;
		}

		case 0x0389:	// LogDebug 
		{
			RB_TOKEN_CODE("299B000000299C0000000B");
			RB_TOKEN_NAME("LogDebug");
			flags=0x00022401;
			break;
		}

		case 0x038A:	// LogTrace 
		{
			RB_TOKEN_CODE("299800000029990000000B");
			RB_TOKEN_NAME("LogTrace");
			flags=0x00022401;
			break;
		}

		case 0x05DC:	// ProjectOnTo 
		{
			RB_TOKEN_CODE("292502000029260200000B");
			RB_TOKEN_NAME("ProjectOnTo");
			flags=0x00022401;
			break;
		}

		case 0x05DD:	// IsZero 
		{
			RB_TOKEN_CODE("29220200000B");
			RB_TOKEN_NAME("IsZero");
			flags=0x00022401;
			break;
		}

		case 0x0E10:	// SBBulletHit 
		{
			RB_TOKEN_CODE("29D11600000B");
			RB_TOKEN_NAME("SBBulletHit");
			flags=0x00020401;
			break;
		}

		case 0x0E11:	// SBExplosionHit 
		{
			RB_TOKEN_CODE("29CE16000029CF1600000B");
			RB_TOKEN_NAME("SBExplosionHit");
			flags=0x00020401;
			break;
		}

		case 0x0E12:	// SBSlash 
		{
			RB_TOKEN_CODE("29CC1600000B");
			RB_TOKEN_NAME("SBSlash");
			flags=0x00020401;
			break;
		}

		case 0x0E13:	// RemoveAttach 
		{
			RB_TOKEN_CODE("29CA1600000B");
			RB_TOKEN_NAME("RemoveAttach");
			flags=0x00020401;
			break;
		}

		case 0x0E14:	// AttachPawn 
		{
			RB_TOKEN_CODE("29C616000029C716000029C81600000B");
			RB_TOKEN_NAME("AttachPawn");
			flags=0x00020401;
			break;
		}

		case 0x0E15:	// StepClimb 
		{
			RB_TOKEN_CODE("29C31600000B");
			RB_TOKEN_NAME("StepClimb");
			flags=0x00020401;
			break;
		}

		case 0x0F81:	// MoveSmooth 
		{
			RB_TOKEN_CODE("29FD0A00000B");
			RB_TOKEN_NAME("MoveSmooth");
			flags=0x00020401;
			break;
		}

		case 0x0F82:	// SetPhysics 
		{
			RB_TOKEN_CODE("29320C00000B");
			RB_TOKEN_NAME("SetPhysics");
			flags=0x00020401;
			break;
		}

		case 0x0F83:	// AutonomousPhysics 
		{
			RB_TOKEN_CODE("29110B00000B");
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