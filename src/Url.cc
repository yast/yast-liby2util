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

   File:       Url.cc
   Purpose:    An URL class
   Author:     Ludwig Nussel <lnussel@suse.de>
   Maintainer: Ludwig Nussel <lnussel@suse.de>

/-*/

#include <iostream>

#include <y2util/Y2SLog.h>
#include <y2util/Url.h>

using namespace std;

Url::Url()
    : _valid( false )
{
}

Url::Url( const string & url )
{
    _valid = split( url, _protocol, _username, _password, _host, _port, _path, _options );
    clearifinvalid(_valid);
}

void Url::clearifinvalid(bool valid)
{
    if(valid) return;

    _protocol = _username = _password = _host = _port = _path.erase();
    _options.erase(_options.begin(),_options.end());
}

bool Url::set( const string url )
{
    _valid = split( url, _protocol, _username, _password, _host, _port, _path , _options);
    clearifinvalid(_valid);

    return _valid;
}

string Url::asString( bool path, bool options, bool plainpassword )   const
{
    string url(_protocol+"://");
    if(!_username.empty())
    {
	url+=_username;
	if(!_password.empty())
	{
	    url+=':';
	    if(plainpassword)
		url+=_password;
	    else
		url+="PASSWORD";
	}
	url+='@';
    }
    url+=_host;
    if(!_port.empty())
    {
	url+=':';
	url+=_port;
    }

    if(path)
    {
	url+=_path;
	if(options)
	{
	    for(OptionMapType::const_iterator i = _options.begin();
		i != _options.end();
		++i)
	    {
		url+=';';
		url+=i->first;
		url+='=';
		url+=i->second;
	    }
	}
    }

    return url;
}

string Url::getOption(const string& key) const
{
    OptionMapType::const_iterator it;
    string value;

    if((it=_options.find(key)) != _options.end())
	value=it->second;

    return value;
}

bool Url::split( const string& url,
	      string& protocol,
	      string& username,
	      string& password,
	      string& hostname,
	      string& port,
	      string& path,
	      OptionMapType& options )
{
    string::size_type pos;
    string::size_type lastpos = 0;

    protocol = username = password
	= hostname = port = path
	= string();

    // protocol
    pos = url.find(':');
    if ( pos != string::npos && pos != 0 )
	protocol = url.substr(0,pos);
    else
	return false;

    D__ << "protocol " << protocol << endl;

    lastpos = pos+1;

    // check for hierarchical url
    if( url.substr(lastpos,2) != "//" )
	return false;

    lastpos = pos = lastpos + 2;

    // check if non local url
    if( url[lastpos] != '/' )
    {
	D__ << "nonlocal url " << url.substr(lastpos) << endl;
	// optional username&password
	pos = url.find('@',lastpos);
	if ( pos != string::npos )
	{
	    string userandpass = url.substr(lastpos,pos-lastpos);
	    // set marker behind @
	    lastpos=pos+1;
	    // optional password
	    pos = userandpass.find(':');
	    if ( pos != string::npos )
	    {
		// no username?
		if(pos==0) return false;

		password = userandpass.substr(pos+1);
		D__ << "password " << string( password.size(), '*' ) << endl;
	    }
	    username = userandpass.substr(0,pos);
	    D__ << "username " << username << endl;
	}

	// hostname&port
	pos = url.find('/',lastpos);
	if ( pos != string::npos && pos != lastpos )
	{
	    string hostandport = url.substr(lastpos,pos-lastpos);
	    // set marker on /
	    lastpos=pos;
	    // optional port
	    pos = hostandport.find(':');
	    if ( pos != string::npos )
	    {
		// no hostname?
		if(pos==0) return false;

		port = hostandport.substr(pos+1);
		D__ << "port " << port << endl;
	    }
	    hostname = hostandport.substr(0,pos);
	    D__ << "hostname " << hostname << endl;
	}
	// url must have path behind hostname
	else
	    return false;
    }

    // locate options
    pos = url.find(';',lastpos);

    path = url.substr(lastpos,pos-lastpos);
    D__ << "path " << path << endl;

    options = OptionMapType();

    if(pos != string::npos)
    {
	string optionstr = url.substr(pos+1);
	string::size_type pos2;
	while( !optionstr.empty() )
	{
	    pos2 = optionstr.find(';');
	    string option = optionstr.substr(0,pos2);
	    if( pos2 != string::npos )
		optionstr = optionstr.substr(pos2+1);
	    else
		optionstr.erase();

	    // options consist of key=value
	    pos2 = option.find('=');
	    if( pos2 != string::npos )
	    {
		string key = option.substr(0,pos2);
		string value = option.substr(pos2+1);
		options[key]=value;
	    }
	    else
		return false;
	}
    }

    return true;
}

/******************************************************************
**
**
**	FUNCTION NAME : operator<<
**	FUNCTION TYPE : ostream &
**
**	DESCRIPTION :
*/
ostream & operator<<( ostream & str, const Url & obj )
{
  return str << obj.asString();
}




// vim:sw=4
