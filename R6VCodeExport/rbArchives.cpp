// ============================================================================
//  rbCodeExport.cpp
// ============================================================================

#include "stdafx.h"
#include "rbBasicTypes.h"
#include "rbDebug.h"
#include "rbArchives.h"



// ============================================================================
//	upArchive
// ============================================================================
upArchive::~upArchive()
{
}



// ============================================================================
//	upFileBuffer
// ============================================================================
void upFileBuffer::DumpByteInfo()
{
	//wxLogMessage( wxT("ByteInfo:") );

	//if( !Markers )
	//	return;

	if( Data.size() > 0 )
	{
		int from = 0;
		EByteInfo lastinfo = static_cast<EByteInfo>(Markers[0]);

		for( dword i=0; i!=Data.size(); ++i )
		{
			if( Markers[i] != lastinfo )
			{
				wxLogMessage( wxString::Format( wxT("<0x%.8x 0x%.8x> %s")
				, static_cast<dword>(from) 
				, static_cast<dword>(i-1) 
				, PF_STR(upGetByteInfoName(lastinfo))
				) );
				lastinfo = static_cast<EByteInfo>(Markers[i]);
				from = i;
			}
		}

		wxLogMessage( wxString::Format( wxT("<0x%.8x 0x%.8x> %s")
		, static_cast<dword>(from) 
		, static_cast<dword>(Data.size()-1) 
		, PF_STR(upGetByteInfoName(lastinfo))
		) );
	}

	wxLogMessage( wxString::Format( wxT("<0x%.8x> EOF")
	, static_cast<dword>(Size) 
	) );
}


// ============================================================================
//	upFile
// ============================================================================
upFile::upFile( const wxFileName& filename ) : FileName(filename)
{
}

upFile::~upFile()
{
    if( File.IsOpened() )
        File.Close();
}

wxString upFile::DebugInfo()
{
	wxString S;
	S += wxString::Format( wxT(" N:<%s>"), PF_STR(FileName.GetName())  );
	S += wxString::Format( wxT(" SP:<0x%.8x 0x%.8x>"), static_cast<dword>(Length()), static_cast<dword>(Tell()) );
	S += wxString::Format( wxT( "GR:<0x%.8x-0x%.8x>")
	, static_cast<dword>(GetGuard().From) 
	, static_cast<dword>(GetGuard().To) 
	);
	return S;	
}

// ============================================================================
//	upFileReader
// ============================================================================
upFileReader::upFileReader( const wxFileName& filename ) : upFile(filename)
{
	//UP_TIMER(t);

	File.Open( FileName.GetFullPath(), wxT("rb") );
	if( !File.IsOpened() || File.Error() )
		throw upexception( wxString::Format( wxT("Could not open file %s"), PF_STR(FileName.GetFullPath())  ) );
		
	Buf.Init(File);
	
	//UP_TIMER_LOGF(t,0);
}

void upFileReader::DumpByteInfo()
{
	Buf.DumpByteInfo();
}


// ============================================================================
//  EOF
// ============================================================================