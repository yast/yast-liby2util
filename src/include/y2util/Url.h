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
#include <y2util/Y2SLog.h>

/** URL class that handles URLs of the form
 * protocol://[[username[:password]]@hostname[:port]]/path[;options]
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

	void clearifinvalid(bool valid)
	{
	    if(valid) return;

	    _protocol = _username = _password = _host = _port = _path.erase();
	}

    public:

	Url () {}

	/** construct new url
	 *
	 * @param url string of the form protocol://[[username[:password]]@hostname[:port]]/path[;options]
	 * */
	Url( const std::string & url )
	{
	    _valid = split( url, _protocol, _username, _password, _host, _port, _path, _options );
	    clearifinvalid(_valid);
	}

	~Url(){}

	void  setHost( const std::string & hostStr )
	{ _host = hostStr; }
	void setPath( const std::string &pathStr )
	{ _path = pathStr; }

	const std::string & getHost() const { return _host; }
	const std::string & getPath() const { return _path; }

	bool isLocal()   const { return _host.empty(); }

	bool isValid() const { return _valid; }
	
	bool set( const std::string url )
	{
	    _valid = split( url, _protocol, _username, _password, _host, _port, _path , _options);
	    clearifinvalid(_valid);

	    return _valid;
	}

	/** join tokens to a valid url
	 *
	 * @return the url
	 * */
	std::string asString()   const
	{
	    std::string url(_protocol+"://");
	    if(!_username.empty())
	    {
		url+=_username;
		if(!_password.empty())
		{
		    url+=':';
		    url+=_password;
		}
		url+='@';
	    }
	    url+=_host;
	    if(!_port.empty())
	    {
		url+=':';
		url+=_port;
	    }
	    url+=_path;
	    // TODO add options
	    
	    return url;
	}

	
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
		      OptionMapType& options )
	{
	    std::string::size_type pos;
	    std::string::size_type lastpos = 0;

	    protocol = username = password
		= hostname = port = path
		= std::string();

	    // protocol
	    pos = url.find(':');
	    if ( pos != std::string::npos && pos != 0 )
		protocol = url.substr(0,pos);
	    else
		return false;
	    
	    D__ << "protocol " << protocol << std::endl;

	    lastpos = pos+1;

	    // check for hierarchical url
	    if( url.substr(lastpos,2) != "//" )
		return false;

	    lastpos = pos = lastpos + 2;
	    
	    // check if non local url
	    if( url[lastpos] != '/' )
	    {
		D__ << "nonlocal url " << url.substr(lastpos) << std::endl;
		// optional username&password
		pos = url.find('@',lastpos);
		if ( pos != std::string::npos )
		{
		    std::string userandpass = url.substr(lastpos,pos-lastpos);
		    // set marker behind @
		    lastpos=pos+1;
		    // optional password
		    pos = userandpass.find(':');
		    if ( pos != std::string::npos )
		    {
			// no username?
			if(pos==0) return false;

			password = userandpass.substr(pos+1);
			D__ << "password " << password << std::endl;
		    }
		    username = userandpass.substr(0,pos);
		    D__ << "username " << username << std::endl;
		}

		// hostname&port
		pos = url.find('/',lastpos);
		if ( pos != std::string::npos && pos != lastpos )
		{
		    std::string hostandport = url.substr(lastpos,pos-lastpos);
		    // set marker on /
		    lastpos=pos;
		    // optional port
		    pos = hostandport.find(':');
		    if ( pos != std::string::npos )
		    {
			// no hostname?
			if(pos==0) return false;

			port = hostandport.substr(pos+1);
			D__ << "port " << port << std::endl;
		    }
		    hostname = hostandport.substr(0,pos);
		    D__ << "hostname " << hostname << std::endl;
		}
		// url must have path behind hostname
		else
		    return false;
	    }

	    // locate options
	    pos = url.find(';',lastpos);

	    path = url.substr(lastpos,pos-lastpos);
	    D__ << "path " << path << std::endl;

	    if(pos != std::string::npos )
	    {
		// TODO parse options
	    }
	    else
		options = OptionMapType();

	    return true;
	}
};

#endif // _Url_h_
