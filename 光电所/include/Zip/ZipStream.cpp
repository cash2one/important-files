#include "stdafx.h"

#include "ZipStream.hpp"


namespace zip 
{

	// ----------------------------------------------------------------------------
	// Internal classes to implement gzstream. See header file for user classes.
	// ----------------------------------------------------------------------------

	// --------------------------------------
	// class gzstreambuf:
	// --------------------------------------

	ZipStreamBuf* ZipStreamBuf::open( const char* name, int open_mode) 
	{
		if ( is_open())
			return (ZipStreamBuf*)0;
		mode = open_mode;
		// no append nor read/write mode
		if ((mode & std::ios::ate) || (mode & std::ios::app)
			|| ((mode & std::ios::in) && (mode & std::ios::out)))
			return (ZipStreamBuf*)0;
		char  fmode[10] = {0};
		char* fmodeptr = fmode;
		if ( mode & std::ios::in)
			*fmodeptr++ = 'r';
		else if ( mode & std::ios::out)
			*fmodeptr++ = 'w';
		*fmodeptr++ = 'b';
		*fmodeptr = '\0';
		file = ::gzopen( name, fmode);
		if (file == 0)
			return (ZipStreamBuf*)0;
		opened = 1;
		return this;
	}

	ZipStreamBuf * ZipStreamBuf::close() 
	{
		if ( is_open()) 
		{
			sync();
			opened = 0;
			if ( ::gzclose( file) == Z_OK )
				return this;
		}
		return (ZipStreamBuf*)0;
	}

	int ZipStreamBuf::underflow() 
	{ // used for input buffer only
		if ( gptr() && ( gptr() < egptr()))
			return * reinterpret_cast<unsigned char *>( gptr());

		if ( ! (mode & std::ios::in) || ! opened )
			return EOF;
		// Josuttis' implementation of inbuf
		int n_putback = gptr() - eback();
		if ( n_putback > 4)
			n_putback = 4;
		memcpy( buffer + (4 - n_putback), gptr() - n_putback, n_putback);

		int num = ::gzread( file, buffer+4, bufferSize-4);
		if (num <= 0) // ERROR or EOF
			return EOF;

		// reset buffer pointers
		setg( buffer + (4 - n_putback),   // beginning of putback area
			buffer + 4,                 // read position
			buffer + 4 + num);          // end of buffer

		// return next character
		return * reinterpret_cast<unsigned char *>( gptr());    
	}

	int ZipStreamBuf::flush_buffer() 
	{
		// Separate the writing of the buffer from overflow() and
		// sync() operation.
		int w = pptr() - pbase();
		if ( ::gzwrite( file, pbase(), w) != w)
			return EOF;
		pbump( -w);
		return w;
	}

	int ZipStreamBuf::overflow( int c) 
	{ // used for output buffer only
		if ( ! ( mode & std::ios::out) || ! opened)
			return EOF;
		if (c != EOF) 
		{
			*pptr() = c;
			pbump(1);
		}
		if ( flush_buffer() == EOF)
			return EOF;
		return c;
	}

	int ZipStreamBuf::sync() 
	{
		// Changed to use flush_buffer() instead of overflow( EOF)
		// which caused improper behavior with std::endl and flush(),
		// bug reported by Vincent Ricard.
		if ( pptr() && pptr() > pbase()) 
		{
			if ( flush_buffer() == EOF)
				return -1;
		}
		return 0;
	}

	// --------------------------------------
	// class gzstreambase:
	// --------------------------------------

	ZipStreamBase::ZipStreamBase( const char* name, int mode) 
	{
		init( &buf);
		open( name, mode);
	}

	ZipStreamBase::~ZipStreamBase() 
	{
		buf.close();
	}

	void ZipStreamBase::open( const char* name, int open_mode) 
	{
		if ( ! buf.open( name, open_mode))
			clear( rdstate() | std::ios::badbit);
	}

	void ZipStreamBase::close()
	{
		if ( buf.is_open())
			if ( ! buf.close())
				clear( rdstate() | std::ios::badbit);
	}

}

