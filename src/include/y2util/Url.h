/*---------------------------------------------------------------------\
|                                                                      |
|                      __   __    ____ _____ ____                      |
|                      \ \ / /_ _/ ___|_   _|___ \                     |
|                       \ V / _` \___ \ | |   __) |                    |
|                        | | (_| |___) || |  / __/                     |
|                        |_|\__,_|____/ |_| |_____|                    |
|                                                                      |
|                               core system                            |
|                                                     (C) 2002 SuSE AG |
\----------------------------------------------------------------------/

   File:       Url.h
   Purpose:    An URL class
   Author:     Ludwig Nussel <lnussel@suse.de>
   Maintainer: Ludwig Nussel <lnussel@suse.de>

/-*/

#ifndef _Url_h_
#define _Url_h_

#include <string>
#include <map>


/** URL class that handles URLs of the form
 * protocol://[[username[:password]]@hostname[:port]]/path[;options]
 * <p>
 * TODO: (un)escaping special characters like space -> %20
 * */
class Url {
    public:

	typedef std::map<std::string,std::string> OptionMapType;

    private:

	std::string _protocol;
	std::string _username;
	std::string _password;
	std::string _host;
	std::string _port;
	std::string _path;
	OptionMapType _options;

	bool _valid;

	/** clear all variables
	 *
	 * @param valid clear if this parameter is false, do nothing if true
	 * */
	void clearifinvalid(bool valid);

    public:

	Url () {}

	/** construct new url
	 *
	 * @param url string of the form protocol://[[username[:password]]@hostname[:port]]/path[;options]
	 * */
	Url( const std::string & url );

	~Url(){}

	void  setHost( const std::string & hostStr )
	{ _host = hostStr; }
	void setPath( const std::string &pathStr )
	{ _path = pathStr; }

	const std::string & getHost() const { return _host; }
	const std::string & getPath() const { return _path; }

	bool isLocal()   const { return _host.empty(); }

	bool isValid() const { return _valid; }
	
	/** set url
	 *
	 * @param url string of the form protocol://[[username[:password]]@hostname[:port]]/path[;options]
	 *
	 * @return true if url is valid, false otherwise
	 * */
	bool set( const std::string url );

	/** join tokens to a valid url
	 *
	 * @return the url
	 * */
	std::string asString() const;

	
	/** split url into tokens
	 *
	 * @param url string of the form protocol://[[username[:password]]@hostname[:port]]/path[;options]
	 *
	 * @return true if valid url, false otherwise
	 * */
	static bool split( const std::string& url,
		      std::string& protocol,
		      std::string& username,
		      std::string& password,
		      std::string& hostname,
		      std::string& port,
		      std::string& path,
		      OptionMapType& options );
};

#endif // _Url_h_
