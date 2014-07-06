// ============================================================================
//  Debug.h
// ============================================================================
#pragma once



// vector accessor macro
//#ifdef _DEBUG
//	#define  UP_AT(num)	.at(num)
//#else
	#define  UP_AT(num)	[num]
//#endif


// adds file name and line number to memory leak reports
#ifdef _DEBUG
    #ifdef __WXMSW__
	    #undef WXDEBUG_NEW
	    #define WXDEBUG_NEW new(__FILE__,__LINE__)
	    #include <crtdbg.h>

	    // this define works around a bug with inline declarations of new, see
	    // http://support.microsoft.com/support/kb/articles/Q140/8/58.asp
        #define new  new( _NORMAL_BLOCK, __FILE__, __LINE__)
    #endif
#endif

//
#define STATIC_CREATOR(NAME,BASE) static BASE* Create() { return new NAME(); }
#define UP_CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))

#define UP_BREAK _asm int 3;


// ============================================================================
// Global
// ============================================================================
inline void upLogMessage( const wxString& S )
{
	wxLogMessage(S);
}

inline void upLogError( const wxString& S )
{
	wxLogError(S);
}


// ============================================================================
// upexception
// ============================================================================
class upexception : public std::exception
{
public:
	upexception()
    : exception()
	{
	}

	upexception( const wxString& text )
    : exception( text.mb_str() )
    , Text(text)
	{
	}

	upexception( const upexception& rhs )
    : exception( rhs.Text.mb_str() )
    , Text(rhs.Text)
	{
	}

    upexception& operator=( const upexception& rhs )
	{
		exception::operator=( rhs ); 
		Text = rhs.Text; 
		return *this; 
	}

	virtual ~upexception()
	{
	}

    const wxString& wwhat() const { return Text; }

private:
    wxString Text; 
} ;


// ============================================================================
// guards
// ============================================================================
#define guard	try {
#define unguard } catch( upexception& e ) { upLogError( wxString::Format(wxT("%s"), e.wwhat().c_str() )); }\
				  catch( exception& e ) { upLogError( wxString::Format(wxT("%hs"), e.what())); }

/*
// ============================================================================
// StdPrintf
// ============================================================================
inline stdstr StdPrintf( cstr format, ... )
{
	char buff[2048];
	memset(buff, 0, sizeof(buff));
	va_list args;
	va_start(args, format);
	vsnprintf_s( buff, sizeof(buff), _TRUNCATE, format, args);
	return stdstr(buff);
}

// ============================================================================
// rbThrowf
// ============================================================================
inline void rbThrowf( cstr format, ... )
{
	char buff[2048];
	memset(buff, 0, sizeof(buff));
	va_list args;
	va_start(args, format);
	vsnprintf_s( buff, sizeof(buff), _TRUNCATE, format, args);
	throw exception( buff );
}
*/

inline wxString MakePrintfSafe( const wxString& s )
{ 
	wxString safe = s;
	safe.Replace(wxT("%"),wxT("%%"),true); 
	return safe; 
}

// ============================================================================
//  Any string that's a Printf/Format parameter should use this macro
// ============================================================================
//  TODO: find a better way around "%" strings
// ============================================================================
#define PF_STR(text) MakePrintfSafe(text).c_str()
#define PF_SAFESTR(text) MakePrintfSafe(text).c_str()


// ============================================================================
//  EOF
// ============================================================================