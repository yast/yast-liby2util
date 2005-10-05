/*---------------------------------------------------------------------\
|                                                                      |
|                      __   __    ____ _____ ____                      |
|                      \ \ / /_ _/ ___|_   _|___ \                     |
|                       \ V / _` \___ \ | |   __) |                    |
|                        | | (_| |___) || |  / __/                     |
|                        |_|\__,_|____/ |_| |_____|                    |
|                                                                      |
|                               core system                            |
|                                                    (C) SuSE Linux AG |
\----------------------------------------------------------------------/

   File:       SysConfig.h

   Author:     Cornelius Schumacher <cschum@suse.de>
   Maintainer: Cornelius Schumacher <cschum@suse.de>

/-*/
#ifndef SysConfig_h
#define SysConfig_h

#include <y2util/Pathname.h>

#include <string>
#include <map>

/**
  This class provides access to the configuration files under /etc/sysconfig.
  
  A sysconfig file consists of lines of key/value pairs. It can also contain
  empty lines and comments (lines starting with '#").
  
  Keys are case-sensitive. Values can be quoted like 'value' or "value".
  Whitesspace at the beginning or end of keys and values (outside of quotes) is
  removed.
*/
class SysConfig
{
  public:
    /**
      Construct a sysconfig object. Reads the given file.
      
      @path Path to sysconfig file. This can either be an absolute path or a
            path relative to /etc/sysconfig.
    */
    SysConfig( const char *path );
    /**
      Construct a sysconfig object. Reads the given file.
      
      @path Path to sysconfig file. This can either be an absolute path or a
            path relative to /etc/sysconfig.
    */
    SysConfig( const std::string &path );
    /**
      Construct a sysconfig object. Reads the given file.
      
      @path Path to sysconfig file. This can either be an absolute path or a
            path relative to /etc/sysconfig.
    */
    SysConfig( const Pathname &path );

    ~SysConfig();

    /**
      Load sysconfig file into memory. This function is automatically called
      from the constructor.
    */    
    bool load();

    /**
      Save file to disk.
    
      @return  true on success, false on error.
    */
    bool save();
        
    /**
      Return string entry.

      @param key          Key of entry.
      @param defaultValue Default return value, if key doesn't exit.
    */
    std::string readEntry( const std::string &key,
                           const std::string &defaultValue = std::string() );

    /**
      Return boolean entry. The values "true", "yes" and "1" are considered
      as true, all other values as false.

      @param key          Key of entry.
      @param defaultValue Default return value, if key doesn't exit.
    */
    bool readBoolEntry( const std::string &key, bool defaultValue = false );
    
    /**
      Return integer entry. If the entry can't be converted to an int, 0 is
      returned.

      @param key          Key of entry.
      @param defaultValue Default return value, if key doesn't exit.
    */
    int readIntEntry( const std::string &key, int defaultValue = 0 );

    /**
      Write string entry.
      
      @param key    Key of entry
      @param value  Value of entry.
    */
    void writeEntry( const std::string &key, const char *value );

    /**
      Write string entry.
      
      @param key    Key of entry
      @param value  Value of entry.
    */
    void writeEntry( const std::string &key, const std::string &value );

    /**
      Write string entry.
      
      @param key    Key of entry
      @param value  Value of entry.
    */
    void writeEntry( const std::string &key, bool value );

    /**
      Write string entry.
      
      @param key    Key of entry
      @param value  Value of entry.
    */
    void writeEntry( const std::string &key, int value );

  protected:
    std::string createLine( const std::string &key,
                                       const std::string &value );
 
  private:
    Pathname _path;
    typedef std::map<std::string, std::string> EntryMap; 
    EntryMap _entryMap;
    bool mDirty;
};

#endif
