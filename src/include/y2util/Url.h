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

#include <iosfwd>

#include <string>
#include <map>


/** URL class that handles URLs of the form
 * protocol://[[username[:password]]@hostname[:port]]/path[;options]
 * <p>
 * TODO: (un)escaping special characters like space -> %20
 * TODO: check if url is still valid when using set* functions
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

	/** default constructor creating an (invalid) empty Url
	 * */
	Url ();

	/** construct new url
	 *
	 * @param url string of the form protocol://[[username[:password]]@hostname[:port]]/path[;options]
	 * */
	Url( const std::string & url );

	~Url(){}

        void setProtocol( const std::string &str ) { _protocol = str; }
        void setUsername( const std::string &str ) { _username = str; }
        void setPassword( const std::string &str ) { _password = str; }
	void setHost( const std::string &str ) { _host = str; }
	void setPort( const std::string &str ) { _port = str; }
	void setPath( const std::string &str ) { _path = str; }

	const std::string & getProtocol() const { return _protocol; }
	const std::string & getUsername() const { return _username; }
	const std::string & getPassword() const { return _password; }
	const std::string & getHost() const { return _host; }
	const std::string & getPort() const { return _port; }
	const std::string & getPath() const { return _path; }
	const OptionMapType & getOptions() const { return _options; }

	/** return Option
	 *
	 * @param key name of option
	 *
	 * @return option value, emtpy string if not found
	 * */
	std::string getOption(const std::string& key) const;

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
	 * @param path whether to append the path
	 * @param options whether to append options
	 * @param plainpassword whether to include the password in plaintext
	 * (otherwise PASSWORD is printed)
	 *
	 * @see saveAsString
	 *
	 * @return the url
	 * */
	std::string asString( bool path = true, bool options = true, bool plainpassword = false) const;

	/**
	 * use this function to get full Url for storing it in a file
	 * */
	std::string saveAsString() const { return asString(true,true,true); }


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

	friend std::ostream & operator<<( std::ostream & str, const Url & obj );
};

#endif // _Url_h_
