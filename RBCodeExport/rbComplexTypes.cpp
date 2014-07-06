// ============================================================================
//  rbCodeExport.cpp
// ============================================================================

#include "stdafx.h"
#include "rbBasicTypes.h"
#include "rbDebug.h"
#include "rbArchives.h"
#include "rbComplexTypes.h"
#include "rbNativeObjects.h"
#include "rbXmlDocument.h"
#include "rbByteCode.h"



// ============================================================================
//  rbString
// ============================================================================
void operator << ( upArchive& A, rbString& D )
{
	dword length;
	A << length;
	
	if( length > 0 )
	{
		char* ascii_str = new char[length];
		A.Serialize( ascii_str, length );
		D.Text = wxString(ascii_str, wxConvUTF8);
		delete []ascii_str;

		/*wxCharBuffer buffer(length);
		A.Serialize( buffer.data(), length );
		wxWCharBuffer wbuffer = wxConvLocal.cMB2WC(buffer);
		D.Text = wxString(wbuffer);*/
	}
}


// ============================================================================
//  rbGUID
// ============================================================================
void operator << ( upArchive& A, rbGUID& D )
{
	A << D.Data[0];
	A << D.Data[1];
	A << D.Data[2];
	A << D.Data[3];
}

// ============================================================================
//  rbUNICODEZ
// ============================================================================
void operator << ( upArchive& A, rbUNICODEZ& D )
{
    D.Text.Clear();
    wxChar c;
    do		{ A << c; D.Text.Append(c); }
    while	( c != 0 );
}


// ============================================================================
//  rbASCIIZ
// ============================================================================
void operator << ( upArchive& A, rbASCIIZ& D )
{
	D.Text.Clear();
	vector<char> chars;
	char c;
	do		{ A << c; chars.push_back(c); }
    while	( c != 0 );
	D.Text = wxString::FromAscii(&chars[0]);
}



// ============================================================================
//  rbNameTable
// ============================================================================
void rbNameTable::Init( rbPkgHeader& header )
{
	Offset = header.NameOffset;
	Items.resize(header.NameCount);
}

void operator << ( upArchive& A, rbNameTable& D )
{
	UP_BYTE_MARKER(names,A,D.Offset,BI_NameTable);
	for(int i=0; i!=D.Items.size(); ++i )
	{
		int offset = A.Tell();
		A << D.Items[i].Name;
		A << D.Items[i].Unknown;
		A << D.Items[i].Flags;
		//wxLogMessage( wxString::Format(wxT("Name[0x%.8x][%i]=(%s)"), offset, i, PF_STR(D.Items[i].Print())  ) );
	}
}

void rbNameTable::DumpContext( rbPackage& p )
{
	wxLogMessage( wxString::Format(wxT("Dumping %d Names..."), Items.size() ) );
	for(int i=0; i!=Items.size(); ++i )
	{
		wxLogMessage( wxString::Format(wxT("Name[%i]=(%s)"), i, PF_SAFESTR(Items[i].PrintContext(p))  ) );
	}
}

void rbNameTable::DumpToXml( rbPackage& p, wxFile& file )
{
}



wxString rbNameTable::Item::Print()
{
	return wxString::Format(wxT("\"%s\", %x, %x"), PF_STR(Name) , Unknown, Flags);
}

wxString rbNameTable::Item::PrintContext( rbPackage& p )
{
	return Print();
}


// ============================================================================
//  rbExportTable
// ============================================================================
void rbExportTable::Init( rbPkgHeader& header )
{
	Offset = header.ExportOffset;
	Items.resize(header.ExportCount);
}


void operator << ( upArchive& A, rbExportTable& D )
{
	UP_BYTE_MARKER(exports,A,D.Offset,BI_ExportTable);
	for(int i=0; i!=D.Items.size(); ++i )
	{
		int offset = A.Tell();
		/*A << D.Items[i].Unk1;
		A << D.Items[i].Class;
		A << D.Items[i].Super;
		A << D.Items[i].Package;
		A << D.Items[i].ObjectName;
		A << D.Items[i].ObjectFlags;
		A << D.Items[i].ObjectSize;*/

		A.Serialize(&D.Items[i],sizeof(rbExportTable::Item)-sizeof(dword*));

		int count = D.Items[i].unk[10];
		if( count > 0 )
		{
			dword* data = new dword[count*3];
			A.Serialize(data,sizeof(dword)*count*3);
			D.Items[i].extras = data;
		}


		//wxLogMessage( wxString::Format(wxT("Export[0x%.8x][%i]=(%s)"), offset, i, PF_STR(D.Items[i].Print())  ) );
	}
}

void rbExportTable::DumpContext( rbPackage& p )
{
	wxLogMessage( wxString::Format(wxT("Dumping %d Exports..."), Items.size() ) );
	for(int i=0; i!=Items.size(); ++i )
	{
		wxLogMessage( wxString::Format(wxT("Export[%i]=(%s)"), i, PF_SAFESTR(Items[i].PrintContext(p))  ) );
		//wxLogMessage( wxString::Format(wxT("%s"), Items[i].PrintContext(p) ) );
	}
}

void rbExportTable::DumpToXml( rbPackage& p, wxFile&  file )
{
}

wxString rbExportTable::Item::Print()
{
	wxString s;

	s += wxString::Format(wxT("%d, "), unk[0]);
	s += wxString::Format(wxT("%d, "), unk[1]);
	s += wxString::Format(wxT("%d, "), unk[2]);
	s += wxString::Format(wxT("%d, "), unk[3]);
	s += wxString::Format(wxT("%d, "), unk[4]);
	s += wxString::Format(wxT("%d, "), unk[5]);
	s += wxString::Format(wxT("%d, "), unk[6]);
	s += wxString::Format(wxT("0x%X, "), unk[7]);
	s += wxString::Format(wxT("%d, "), unk[8]);
	s += wxString::Format(wxT("0x%X, "), unk[9]);
	s += wxString::Format(wxT("%d, "), unk[10]);
	s += wxString::Format(wxT("%d, "), unk[11]);
	s += wxString::Format(wxT("%d, "), unk[12]);
	s += wxString::Format(wxT("%d, "), unk[13]);
	s += wxString::Format(wxT("%d, "), unk[14]);
	s += wxString::Format(wxT("%d, "), unk[15]);
	s += wxString::Format(wxT("%d, "), unk[16]);
	
	dword count = unk[10];
	if( count > 0 )
	{
		s += wxString::Format(wxT("EX(%d): "), count);
		for(int x=0; x!=count*3; ++x )
		{
			s += wxString::Format(wxT("%d, "), extras[x]);
		}
	}

	return s;
}

wxString rbExportTable::Item::PrintContext( rbPackage& p )
{
	wxString s;
	guard;

	s += wxString::Format(wxT("%d=%s, "), unk[0], PF_STR(p.GetObjectName(unk[0])) );
	s += wxString::Format(wxT("%d=%s, "), unk[1], PF_STR(p.GetObjectName(unk[1])) );
	s += wxString::Format(wxT("%d=%s, "), unk[2], PF_STR(p.GetObjectName(unk[2])) );
	s += wxString::Format(wxT("%d=%s, "), unk[3], PF_STR(p.GetNameString(unk[3])) );
	s += wxString::Format(wxT("%d, "), unk[4]);
	s += wxString::Format(wxT("%d=%s, "), unk[5], PF_STR(p.GetObjectName(unk[5])) );
	s += wxString::Format(wxT("%d, "), unk[6]);
	s += wxString::Format(wxT("0x%X, "), unk[7]);
	s += wxString::Format(wxT("%d, "), unk[8]);
	s += wxString::Format(wxT("0x%X, "), unk[9]);
	s += wxString::Format(wxT("%d, "), unk[10]);
	s += wxString::Format(wxT("%d, "), unk[11]);
	s += wxString::Format(wxT("%d, "), unk[12]);
	s += wxString::Format(wxT("%d, "), unk[13]);
	s += wxString::Format(wxT("%d, "), unk[14]);
	s += wxString::Format(wxT("%d, "), unk[15]);
	s += wxString::Format(wxT("%d, "), unk[16]);
	
	dword count = unk[10];
	if( count > 0 )
	{
		s += wxString::Format(wxT("EX(%d): "), count);
		for(int x=0; x!=count*3; ++x )
		{
			s += wxString::Format(wxT("%d, "), extras[x]);
		}
	}

	unguard;
	return s;
}


// ============================================================================
//  rbImportTable
// ============================================================================
void rbImportTable::Init( rbPkgHeader& header )
{
	Offset = header.ImportOffset;
	Items.resize(header.ImportCount);
}

void operator << ( upArchive& A, rbImportTable& D )
{
	UP_BYTE_MARKER(imports,A,D.Offset,BI_ImportTable);
	for(int i=0; i!=D.Items.size(); ++i )
	{
		A.Serialize(&D.Items[i],sizeof(rbImportTable::Item));
		//wxLogMessage( wxString::Format(wxT("Import[%i]=(%s)"), i, D.Items[i].Print() ) );
	}
}

void rbImportTable::DumpContext( rbPackage& p )
{
	wxLogMessage( wxString::Format(wxT("Dumping %d Imports..."), Items.size() ) );
	for(int i=0; i!=Items.size(); ++i )
	{
		wxLogMessage( wxString::Format(wxT("Import[%i]=(%s)"), i, PF_SAFESTR(Items[i].PrintContext(p)) ) );
	}
}

void rbImportTable::DumpToXml( rbPackage& p, wxFile&  file )
{
}

wxString rbImportTable::Item::Print()
{
	return wxString::Format(wxT("%d [%d], %d [%d], %d, %d [%d]"), ClassPackage, Unk1, ClassName, Unk2, Package, ObjectName, Unk3);
}

wxString rbImportTable::Item::PrintContext( rbPackage& p )
{
	return wxString::Format(wxT("%s [%d], %s [%d], %s, %s [%d]")
		, PF_STR(p.GetNameString(ClassPackage))
		, Unk1
		, PF_STR(p.GetNameString(ClassName))
		, Unk2
		, PF_STR(p.GetObjectName(Package))
		, PF_STR(p.GetNameString(ObjectName))
		, Unk3
		);
}


// ============================================================================
//  rbPackage
// ============================================================================
void operator << ( upArchive& A, rbPackage& D )
{
	A << D.Header;

	D.NameTable.Init(D.Header);
	D.ExportTable.Init(D.Header);
	D.ImportTable.Init(D.Header);

	A << D.NameTable;
	A << D.ExportTable;
	A << D.ImportTable;
}
	
void rbPackage::DumpContext()
{
	NameTable.DumpContext(*this);
	ImportTable.DumpContext(*this);
	ExportTable.DumpContext(*this);
}
void rbPackage::DumpToXml( wxFile& file )
{
	file.Write( wxT("<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n") ); 

	NameTable.DumpToXml(*this,file);
	ImportTable.DumpToXml(*this,file);
	ExportTable.DumpToXml(*this,file);
}


wxString rbPackage::GetNameString( int i )
{
	return NameTable.GetItem(i).Name;
}


wxString rbPackage::GetObjectName( int r )
{
	//if( r > 0 )			return wxT("EXPORT");
	if( r > 0 )			return NameTable.GetItem(ExportTable.GetItem(r-1).unk[3]).Name;
	else if( r < 0 )	return NameTable.GetItem(ImportTable.GetItem(-r-1).ObjectName).Name;
	else				return wxT("");
}

wxString rbPackage::GetObjectPath( int r )
{
	wxString S;

	if( r == 0 )
		return wxT("");

	// object name
	if( r > 0 )
	{
		S = NameTable.GetItem(ExportTable.GetItem(r-1).unk[3]).Name;
		r = ExportTable.GetItem(r-1).unk[2];
	}
	else
	{
		S = NameTable.GetItem(ImportTable.GetItem(-r-1).ObjectName).Name;
		r = ImportTable.GetItem(-r-1).Package;
	}

	if( r == 0 )
		return S;

	// package path
	for( int i=0; i!=255; ++i )
	{
		if( r > 0 )
		{
			S.Prepend( NameTable.GetItem(ExportTable.GetItem(r-1).unk[3]).Name + wxT(".") );
			r = ExportTable.GetItem(r-1).unk[2];
		}
		else
		{
			S.Prepend( NameTable.GetItem(ImportTable.GetItem(-r-1).ObjectName).Name + wxT(".") );
			r = ImportTable.GetItem(-r-1).Package;
		}

		if( r == 0 )
			return S;
	}

	throw upexception( wxString::Format( wxT("Object path depth > 255") ) );
}

int rbPackage::GetSuperClass( int r )
{
	if( r == 0 )
		return 0;

	if( r > 0 )
	{
		return ExportTable.GetItem(r-1).unk[2];
	}
	else
	{
		return ImportTable.GetItem(-r-1).Package;
	}
}


void rbPackage::DumpFunctions( upArchive& A )
{

	//wxLogMessage( wxString::Format(wxT("Dumping functions...") ) );

	wxTextFile f;
	if( !f.Open(wxT("func.txt")) )
		f.Create(wxT("func.txt"));

/*
(0x[0-9A-F]+) (0x[0-9A-F]+) (0x[0-9A-F]+) ([0-9A-F]+) ([\s\S]*)

case \1:\t// \5 \n{\n\t\n\tbreak;\n}\n\n

*/

	/*unXmlDocument doc;
	doc.Load(wxT("bytecode.xml"), wxT("UTF-8"));
	unXmlNode* root = doc.GetRoot();
	unXmlNode* group = rbxml::GetElement(root,wxT("group"));
	unXmlNode* gname = rbxml::GetElement(group,wxT("name"),wxT("functions"));*/



	wxString functag = wxT("Function");
	for(int e=0; e!=ExportTable.Items.size(); ++e )
	{
		rbExportTable::Item ei = ExportTable.Items[e];
		if( !functag.IsSameAs(GetObjectName(ei.unk[0]),false) )
			continue;
		
		A.Seek( ei.unk[9] );
		UP_OFFSET_GUARD(rbsFunction,A,A.Tell(),A.Tell()+ei.unk[8]);
			
		rbsFunction func;
		A << func;

		//if( !rbHasFlag(func.flags,20000000) )
		//	continue;

		if( func.fidx == 0 )
			continue;


		/*wxLogMessage( wxString::Format(wxT("Func[%i]=[%d] (%s) (%s)")
			, e
			, func.fidx
			, PF_SAFESTR(GetNameString(func.fname))
			, PF_SAFESTR(GetNameString(ei.unk[3]))
			) );*/

		wxString tokens;
		for( dword t=0; t<func.scriptsize; ++t )
		{
			tokens += wxString::Format(wxT("%.2X"),func.script[t]);
		}


		f.AddLine( wxString::Format(wxT("0x%.4X 0x%.2X 0x%.8X %s %s"), dword(func.fidx), dword(func.prec), func.flags, tokens, GetNameString(func.fname)  ) );
	}

	f.Write();
	f.Close();

	// myfile2.xml will be indentic to myfile.xml saving it this way:
	//doc.Save(wxT("bytecode.xml"));


}



void rbPackage::DumpByteCode( upArchive& A )
{
	guard;
	wxLogMessage(wxT(""));
	wxLogMessage( wxString::Format(wxT("DumpByteCode...") ) );

	wxString codefile = wxString::Format( wxT("%s.script.txt"), Name.c_str() );

	wxFile f;
	if( f.Open(codefile,wxFile::write) )
	{
	}
		//f.Create(wxT("bytecode.txt"));

	wxString functag = wxT("Function");
	for(int e=0; e!=ExportTable.Items.size(); ++e )
	{
		guard;
		rbExportTable::Item ei = ExportTable.Items[e];
		if( !functag.IsSameAs(GetObjectName(ei.unk[0]),false) )
			continue;
		
		A.Seek( ei.unk[9] );
		UP_OFFSET_GUARD(rbsFunction,A,A.Tell(),A.Tell()+ei.unk[8]);
			
		rbsFunction func;
		A << func;

		//if( func.fidx == 0 ) continue;

		A.Seek(ei.unk[9] + 44);
		UP_OFFSET_GUARD(rbsFunction2,A,A.Tell(),A.Tell()+func.scriptsize);
		wxString sdebug = A.DebugInfo();
		wxLogMessage(wxT(""));
		wxLogMessage( wxString::Format(wxT("DumpByteCode: [%u] [%s] [%s][%s]")
			, ei.unk[8], PF_SAFESTR(sdebug), PF_SAFESTR(GetNameString(ei.unk[3])), PF_SAFESTR(GetObjectName(ei.unk[2]))  ) );

		wxString tokens;
		for( dword t=0; t<func.scriptsize; ++t )
		{
			tokens += wxString::Format(wxT("%.2X"),func.script[t]);
		}

		wxLogMessage( wxString::Format(wxT("Code: [%u] %s"), func.scriptsize, PF_SAFESTR(tokens)  ) );

		rbBytecode bc(*this,A,A.Tell()+func.scriptsize,e+1);
		bc.Parse();
		
		f.Write(wxString::Format( wxT("\n//\n// BEGIN: %s\n//\n"), PF_SAFESTR(GetObjectPath(e+1)) ));
		f.Write(bc.ScriptText);
		f.Write(wxString::Format( wxT("\n//\n// END: %s\n//\n"), PF_SAFESTR(GetObjectPath(e+1)) ));

		/*
		f.Write(wxString::Format( wxT("\n//\n// BEGIN: %s.%s\n//\n"), PF_SAFESTR(GetObjectName(ei.unk[2])), PF_SAFESTR(GetObjectName(ei.unk[3])) ));
		f.Write(bc.ScriptText);
		f.Write(wxString::Format( wxT("\n//\n// END: %s.%s\n//\n"), PF_SAFESTR(GetObjectName(ei.unk[2])), PF_SAFESTR(GetObjectName(ei.unk[3])) ));
		
		*/
		//dword chunk = 1024;
		//f
		/*for( dword i=0; i<bc.ScriptText.Length(); ++i )
		{
			wxLogMessage(bc.ScriptText.Mid(i,chunk));
			i += chunk;
		}*/

		//f.AddLine( wxString::Format(wxT("0x%.4X 0x%.2X 0x%.8X %s %s"), dword(func.fidx), dword(func.prec), func.flags, tokens, GetNameString(func.fname)  ) );
	
		unguard;
	}

	//f.Write();
	f.Close();

	unguard;
}

// ============================================================================
//  EOF
// ============================================================================