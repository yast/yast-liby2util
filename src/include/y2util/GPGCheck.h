#ifndef GPGCHECK_H
#define GPGCHECK_H

#include <string>

class GPGCheck
{
  public:
    GPGCheck ();

    /**
      Set keyring.
    */
    void set_keyring ( const std::string& keyring );

    /**
      Check signature of given file.
      
      @param filename Name of file to be checked.
      @param strip    If true, signature is stripped from file
      
      @return True, if the signature is valid, false if not.
    */
    bool check_file ( const std::string& filename, bool strip = false );

    /**
      Check signature of given file. Strip signature from file and write
      result to destination. This is done regardless, if the signature is
      valid or not.
    
      @param sourceFile Name of file to be checked.
      @param destFile   Destination where stripped file will be written.
      
      @return true, if signature is valid, false if not.
    */
    bool check_file ( const std::string &sourceFile,
                      const std::string &destFile );

  protected:
    std::string assembleCommand( const std::string &args );

  private:
    std::string _keyring;
    std::string _gnupghome;
};

#endif
