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
      
      @result True, if the signature is valid, false if not.
    */
    bool check_file ( const std::string& filename, bool strip = false );

  private:
    std::string keyring;
};

#endif
