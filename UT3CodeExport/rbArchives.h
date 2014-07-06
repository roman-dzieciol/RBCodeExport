// ============================================================================
//  rbArchives.h
// ============================================================================
#pragma once


// ============================================================================
// EByteInfo
// ============================================================================
enum EByteInfo
{
	BI_None					= 0x00,
	BI_Header				= 0x01,
	BI_NameTable			= 0x02,
	BI_ImportTable			= 0x04,
	BI_ExportTable			= 0x08,
	BI_GenerationTable		= 0x10,
	BI_Object				= 0x20,
};

inline wxString upGetByteInfoName( EByteInfo info )
{
	switch(info)
	{
	case BI_None:				return wxT("BI_None");
	case BI_Header:				return wxT("BI_Header");
	case BI_NameTable:			return wxT("BI_NameTable");
	case BI_ImportTable:		return wxT("BI_ImportTable");
	case BI_ExportTable:		return wxT("BI_ExportTable");
	case BI_GenerationTable:	return wxT("BI_GenerationTable");
	case BI_Object:				return wxT("BI_Object");
	default:					return wxT("BI_UNKNOWN");
	}
}


// ============================================================================
// upFileGuard
// ============================================================================
struct upFileGuard
{
	dword	From;
	dword	To;

    upFileGuard( dword from, dword to ) : From(from), To(to)
	{
	}
};


// ============================================================================
// upArchive
// ============================================================================
class upArchive
{
public:
	virtual ~upArchive() = 0;

public:
	virtual void			Serialize	( void* buffer, dword count ) = 0;

public:
	virtual void			Seek		( wxFileOffset pos, wxSeekMode from = wxFromStart ) = 0;
	virtual void			SeekEnd		( wxFileOffset pos = 0 ) = 0;
	virtual wxFileOffset	Tell		() = 0;
	virtual wxFileOffset	Length		() = 0;

public:
	virtual bool			IsOpened	() = 0;
	virtual bool			IsLoading	() = 0;
	virtual bool			IsSaving	() = 0;

public:
	virtual void			PushGuard	( wxFileOffset from, wxFileOffset to ) = 0;
	virtual void			PopGuard	() = 0;
	virtual upFileGuard		GetGuard	() = 0;

public:
	virtual void			SetByteInfo	( EByteInfo info ) = 0;
	virtual EByteInfo		GetByteInfo	() = 0;
	
public:
	virtual wxString		DebugInfo		() = 0;
	virtual void			DumpByteInfo	() = 0;
	
public:
	friend void operator << ( upArchive& A, qword& D )		{ A.Serialize( &D, sizeof(qword) ); /*wxUINT64_SWAP_ON_?E(D);*/  }
	friend void operator << ( upArchive& A, dword& D )		{ A.Serialize( &D, sizeof(dword) ); /*wxUINT32_SWAP_ON_?E(D);*/  }
	friend void operator << ( upArchive& A, word& D )		{ A.Serialize( &D, sizeof(word) ); /*wxUINT16_SWAP_ON_?E(D);*/  }
	friend void operator << ( upArchive& A, byte& D )		{ A.Serialize( &D, sizeof(byte) );  }
	friend void operator << ( upArchive& A, int64& D )		{ A.Serialize( &D, sizeof(int64) ); /*wxINT64_SWAP_ON_?E(D);*/  }
	friend void operator << ( upArchive& A, int32& D )		{ A.Serialize( &D, sizeof(int32) ); /*wxINT32_SWAP_ON_?E(D);*/  }
	friend void operator << ( upArchive& A, int16& D )		{ A.Serialize( &D, sizeof(int16) ); /*wxINT16_SWAP_ON_?E(D);*/ }
	friend void operator << ( upArchive& A, int8& D )		{ A.Serialize( &D, sizeof(int8) );  }
	friend void operator << ( upArchive& A, char& D )		{ A.Serialize( &D, sizeof(char) );  }
	friend void operator << ( upArchive& A, wchar_t& D )	{ A.Serialize( &D, sizeof(wchar_t) );  }
	friend void operator << ( upArchive& A, float& D )		{ A.Serialize( &D, sizeof(float) ); /*wxINT32_SWAP_ON_?E(D);*/ }

public:
	friend upArchive& operator << ( upArchive& A, EByteInfo D )		{ A.SetByteInfo(D); return A; }
};


// ============================================================================
// upOffsetGuard
// ============================================================================
class upOffsetGuard
{
private:
	upArchive& A; 

public:
	upOffsetGuard( upArchive& a, wxFileOffset from, wxFileOffset to ) : A(a)
	{
		A.PushGuard(from,to);
	}

	~upOffsetGuard()
	{
		A.PopGuard();
	}
};

#define UP_OFFSET_GUARD(name,arc,from,to) upOffsetGuard offsetguard_##name(arc,from,to);



// ============================================================================
// upByteMarker
// ============================================================================
class upByteMarker
{
private:
	upArchive& A; 

public:
	upByteMarker( upArchive& a, wxFileOffset offset, EByteInfo byteinfo ) : A(a)
	{
		A.Seek(offset);
		A << byteinfo;
	}

	~upByteMarker()
	{
		A << BI_None;
	}
};

#define UP_BYTE_MARKER(name,arc,offset,byteinfo) upByteMarker bytemarker_##name(arc,offset,byteinfo);


// ============================================================================
// upFile
// ============================================================================
class upFileBuffer
{
protected:
	dword Offset;
	dword Size;
	dword MaxSize;
	dword GuardMin;
	dword GuardMax;
	EByteInfo ByteInfo;
	vector<byte> Data;
	vector<byte> Markers;
	vector<upFileGuard> Guards;
	bool bOpened;

public:
	upFileBuffer( dword maxsize = 0x40000000 ) 
	:	Offset(0)
	,	Size(0)
	,	MaxSize(maxsize)
	,	GuardMin(0)
	,	GuardMax(0)
	,	ByteInfo(BI_None)
	,	Guards( 1, upFileGuard(0,0) )
	,	bOpened(false)
	{
		// Guard 0 is 0,0
		// Guard 1 is 0,MaxSize
	}

	~upFileBuffer()
	{
	}


public:
	void Seek( wxFileOffset offset, wxSeekMode mode = wxFromStart )
	{
		if( mode == wxFromStart )		Offset = offset;
		else if( mode > wxFromCurrent )	Offset += offset;
		else							Offset = static_cast<dword>(Size)-offset;
		
		if( Offset < 0 || Offset > Size )
		{
			throw upexception( wxString::Format( wxT("Invalid seek <0x%.8x> outside valid range <0x%.8x 0x%.8x>")
			, static_cast<dword>(Offset) 
			, static_cast<dword>(0) 
			, static_cast<dword>(Size) 
			) );
		}
	}

	wxFileOffset Tell() const { return Offset; }
	wxFileOffset Length() const { return Size; }
	
	byte* GetDataPtr() { return Data.empty() ? NULL : &Data UP_AT(0);  }

public:
	bool IsOpened() const { return bOpened; }

public:
    void PushGuard( dword from, dword to )
    {
		if( from >= 0 && from < MaxSize && to >= 0 && to <= MaxSize )
		{
			Guards.push_back( upFileGuard(from,to) );
			GuardMin = from;
			GuardMax = to;
		}
		else
		{
			throw upexception( wxString::Format( wxT("File guard <0x%.8x 0x%.8x> outside valid range <0x%.8x 0x%.8x>")
			, static_cast<dword>(from) 
			, static_cast<dword>(to) 
			, static_cast<dword>(0) 
			, static_cast<dword>(MaxSize) 
			) );
		}
    }

	void PopGuard()
	{
		Guards.pop_back();
		GuardMin = Guards.back().From;
		GuardMax = Guards.back().To;
	}

	upFileGuard GetGuard() const
	{
		return Guards.back();
	}

public:
	EByteInfo	GetByteInfo	() const		{ return ByteInfo; }
	void		SetByteInfo	( EByteInfo d )	{ ByteInfo = d; }

public:
	void DumpByteInfo();
};


// ============================================================================
// upFileReadBuffer
// ============================================================================
class upFileReadBuffer : public upFileBuffer
{
public:
	upFileReadBuffer( dword maxsize = 0x40000000 ) 
	:	upFileBuffer(maxsize)
	{
	}

	~upFileReadBuffer()
	{
	}

	void Init( wxFFile& file )
	{
		//UP_TIMER(t);

		if( !bOpened )
			bOpened = true;
		else
			throw upexception( wxString::Format( wxT("upFileBuffer already opened") ) );
		
		Size = file.Length();
		if( Size > MaxSize )
			throw upexception( wxString::Format( wxT("Excessive file size: %d"), Size ) );

		MaxSize = Size;
		
		Data.reserve(Size);
		Data.resize(Size);

		Markers.reserve(Size);
		Markers.resize(Size);
		
		PushGuard(0,MaxSize);

		file.Seek(0);
		file.Read(&Data UP_AT(0),Size);

		memset(&Markers UP_AT(0),0,Size);

		//UP_TIMER_LOGF(t,0);
	}

public:
    void Read( void* buffer, dword count ) 
	{  
		if( Offset >= GuardMin && Offset + count <= GuardMax )
		{
			// Mark byte as read
			memset( &Markers UP_AT(Offset), static_cast<byte>(ByteInfo), count );

			// Read
			memcpy( buffer, &Data UP_AT(Offset), count );
			Offset += count;
		}
		else
		{
			throw upexception( wxString::Format( wxT("File read <0x%.8x 0x%.8x> outside guarded range <0x%.8x 0x%.8x>")
			, static_cast<dword>(Offset) 
			, static_cast<dword>(Offset + count) 
			, static_cast<dword>(GuardMin) 
			, static_cast<dword>(GuardMax) 
			) );
		}
	}
};


// ============================================================================
// upFile
// ============================================================================
class upFile : public upArchive
{
protected:
	wxFFile			File;
	wxFileName		FileName;

public:
	explicit upFile( const wxFileName& filename );
	virtual ~upFile();

public:
	virtual wxString		DebugInfo	();
	virtual byte*			GetDataPtr	() = 0;

public:
	wxFFile&	GetFile		()	{ return File; }
    wxFileName	GetFileName	()	{ return FileName; }
    wxString	GetName		()	{ return FileName.GetName(); }
};


// ============================================================================
// upFileReader
// ============================================================================
class upFileReader : public upFile
{
protected:
	upFileReadBuffer	Buf;

public:
	explicit upFileReader( const wxFileName& filename );

public:
	virtual void			Serialize	( void* buffer, dword count ) { Buf.Read(buffer,count); }
	virtual byte*			GetDataPtr	() { return Buf.GetDataPtr(); }
	virtual void			DumpByteInfo();

public:
	virtual void			Seek		( wxFileOffset pos, wxSeekMode from = wxFromStart ) { Buf.Seek(pos,from); }
	virtual void			SeekEnd		( wxFileOffset pos = 0 ) { Buf.Seek(pos,wxFromEnd); }
	virtual wxFileOffset	Tell		() { return Buf.Tell(); }
	virtual wxFileOffset	Length		() { return Buf.Length(); }

public:
	virtual bool			IsOpened	() { return Buf.IsOpened(); }
	virtual bool			IsLoading	() { return true; }
	virtual bool			IsSaving	() { return false; }

public:
	virtual void			PushGuard	( wxFileOffset from, wxFileOffset to ) { Buf.PushGuard(from,to); }
	virtual void			PopGuard	() { Buf.PopGuard(); }
	virtual upFileGuard		GetGuard	() { return Buf.GetGuard(); }

public:
	virtual void			SetByteInfo	( EByteInfo info ) { Buf.SetByteInfo(info); }
	virtual EByteInfo		GetByteInfo	() { return Buf.GetByteInfo(); }
};

// ============================================================================
//  EOF
// ============================================================================