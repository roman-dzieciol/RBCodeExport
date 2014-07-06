// ============================================================================
//  rbBasicTypes.h
// ============================================================================
#pragma once

// ============================================================================
//  forward definitions
// ============================================================================

class rbPackage;


// ============================================================================
//  rbString
// ============================================================================
class rbString
{
public:
	operator wxString() const  { return Text; }

public:
	friend void operator << ( upArchive& A, rbString& D );

private: 
	wxString Text;
};

// ============================================================================
//  rbGUID
// ============================================================================
class rbGUID
{
public:
	friend void operator << ( upArchive& A, rbGUID& D );

private: 
	dword Data[4];
};

// ============================================================================
//  rbASCIIZ
// ============================================================================
class rbASCIIZ
{
public:
	operator wxString() const  { return Text; }

public:
	friend void operator << ( upArchive& A, rbASCIIZ& D );

private: 
	wxString Text;
};

// ============================================================================
//  rbUNICODEZ
// ============================================================================
class rbUNICODEZ
{
public:
	operator wxString() const  { return Text; }

public:
	friend void operator << ( upArchive& A, rbUNICODEZ& D );

private: 
	wxString Text;
};


// ============================================================================
// rbPkgHeader
// ============================================================================
class rbPkgHeader
{
public:
	dword		Signature;
	word		PackageVersion;
	word		LicenseMode;
	dword		Unknown;
	dword		Unknown2;
	dword		PackageFlags;
	dword		NameCount;
	dword		NameOffset;
	dword		ExportCount;
	dword		ExportOffset;
	dword		ImportCount;
	dword		ImportOffset;
	rbGUID		GUID;

public:
	friend void operator << ( upArchive& A, rbPkgHeader& D )
	{
		UP_BYTE_MARKER(header,A,0,BI_Header);

		A << D.Signature;	
		if( D.Signature != rbConst::signature )
			throw upexception(wxT("Unknown package signature"));

		A << D.PackageVersion;
		if( D.PackageVersion != rbConst::version )
			throw upexception(wxT("Unknown package version"));

		A << D.LicenseMode;
		A << D.Unknown;
		A << D.Unknown2;
		A << D.PackageFlags;
		A << D.NameCount;
		A << D.NameOffset;
		A << D.ExportCount;
		A << D.ExportOffset;
		A << D.ImportCount;
		A << D.ImportOffset;
		A << D.GUID;
	}
};


// ============================================================================
// rbNameTable
// ============================================================================
class rbNameTable
{
public:
	struct Item
	{
		rbString Name;
		dword Unknown;
		dword Flags;

		wxString Print();
		wxString PrintContext( rbPackage& p );
	};
public:
	rbNameTable(){}
	void Init( rbPkgHeader& p );
	void DumpContext( rbPackage& p );
	void DumpToXml( rbPackage& p, wxFile& file );
	Item& GetItem( dword idx ) { return Items.at(idx); }

public:
	friend void operator << ( upArchive& A, rbNameTable& D );

public:
	dword Offset;
	vector<rbNameTable::Item> Items;
};


// ============================================================================
// rbExportTable
// ============================================================================
class rbExportTable
{
public:
	struct Item
	{
		/*dword Unk1;
		dword Class;
		dword Super;
		dword Package;
		dword ObjectName;
		dword ObjectFlags;
		dword ObjectSize;*/
		dword unk[10];
		dword* extras;

		wxString Print();
		wxString PrintContext( rbPackage& p );

		dword GetFlags() { return unk[6]; }
		dword GetSerialSize() { return unk[7]; }
		dword GetSerialOffset() { return unk[8]; }
	};

public:
	rbExportTable(){}


	void Init( rbPkgHeader& p );
	void DumpContext( rbPackage& p );
	void DumpToXml( rbPackage& p, wxFile& file );
	Item& GetItem( dword idx ) { return Items.at(idx); }

public:
	friend void operator << ( upArchive& A, rbExportTable& D );

public:
	dword Offset;
	vector<rbExportTable::Item> Items;
};


// ============================================================================
// rbImportTable
// ============================================================================
class rbImportTable
{
public:
	struct Item
	{
		dword ClassPackage;
		dword ClassName;
		dword Package;
		dword ObjectName;

		wxString Print();
		wxString PrintContext( rbPackage& p );
	};
public:
	rbImportTable(){}
	void Init( rbPkgHeader& p );
	void DumpContext( rbPackage& p );
	void DumpToXml( rbPackage& p, wxFile& file );
	Item& GetItem( dword idx ) { return Items.at(idx); }

public:
	friend void operator << ( upArchive& A, rbImportTable& D );

public:
	dword Offset;
	vector<rbImportTable::Item> Items;
};


// ============================================================================
// rbPackage
// ============================================================================
class rbPackage
{
public:
	rbPackage(){}
	rbPackage( const wxString& name ) : Name(name){}

	void DumpContext();
	void DumpFunctions( upArchive& A );
	void DumpToXml( wxFile& file );

	void DumpByteCode( upArchive& A );

	wxString GetNameString( int i );
	wxString GetObjectName( int r );
	wxString GetObjectPath( int r );

	int GetSuperClass( int r );

public:
	friend void operator << ( upArchive& A, rbPackage& D );

private :
	wxString Name;
	rbPkgHeader Header;
	rbNameTable NameTable;
	rbExportTable ExportTable;
	rbImportTable ImportTable;
};


// ============================================================================
//  EOF
// ============================================================================