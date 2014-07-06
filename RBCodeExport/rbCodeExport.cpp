// ============================================================================
//  rbCodeExport.cpp
// ============================================================================

#include "stdafx.h"
#include "rbCodeExport.h"


// ============================================================================
// rbCodeExport
// ============================================================================
class rbCodeExport : public wxAppConsole
{
public:
    virtual bool OnInit() { return true; }
    virtual int OnRun();

private:
    void ParseParams(const wxCmdLineParser& cmdline);
	void LoadPackage( const wxString& filename );
};

IMPLEMENT_APP_CONSOLE(rbCodeExport);


int rbCodeExport::OnRun()
{
	try 
	{
		static const wxCmdLineEntryDesc cmdLineDesc[] =
		{
			{ wxCMD_LINE_PARAM,  NULL, NULL, "input file", wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
			{ wxCMD_LINE_NONE, NULL, NULL, NULL, (wxCmdLineParamType)0, 0 }
		};

		wxCmdLineParser parser(cmdLineDesc, argc, argv);
		
		switch (parser.Parse())
		{
			case -1:
				return 0;

			case 0:
				ParseParams(parser);
				return 0;
		}
		return 0;

	} 
	catch( upexception& e ) { wxLogError(wxT("%s"), e.wwhat().c_str()); return 1; }		  
	catch( exception& e ) { wxLogError(wxT("%hs"), e.what()); return 1; }
}


void rbCodeExport::ParseParams(const wxCmdLineParser& cmdline)
{
	if( cmdline.GetParamCount() == 0 )
	{
		wxArrayString files;
		wxDir::GetAllFiles( wxT("."), &files, wxT("*.u"), wxDIR_FILES | wxDIR_HIDDEN | wxDIR_DIRS );

		for( size_t i=0; i!=files.Count(); ++i )
		{
			LoadPackage(files[i]);
		}
	}
	else
	{
		LoadPackage(cmdline.GetParam(0));
	}
}


void rbCodeExport::LoadPackage( const wxString& filename )
{
	guard;
	wxLogMessage( wxT("") );
	wxLogMessage( wxT("LoadPackage() %s"), PF_STR(filename) );


	upFileReader f(filename);
	rbPackage pkg(filename);

	f << pkg;

	pkg.DumpContext();
	
	pkg.DumpByteCode(f);

	/*wxFile xmlfile( wxString(filename+wxT(".xml")), wxFile::write );
	if( xmlfile.IsOpened() )
	{
		pkg.DumpToXml(xmlfile);
		xmlfile.Close();
	}*/

	//f.DumpByteInfo();

	wxLogMessage( wxT("success") );
	unguard;
}


// ============================================================================
//  EOF
// ============================================================================