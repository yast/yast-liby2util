/*---------------------------------------------------------------------\
|                                                                      |
|                      __   __    ____ _____ ____                      |
|                      \ \ / /_ _/ ___|_   _|___ \                     |
|                       \ V / _` \___ \ | |   __) |                    |
|                        | | (_| |___) || |  / __/                     |
|                        |_|\__,_|____/ |_| |_____|                    |
|                                                                      |
|                               core system                            |
|                                                        (C) SuSE GmbH |
\----------------------------------------------------------------------/

   File:       PathInfo.h

   Author:     Michael Andres <ma@suse.de>
   Maintainer: Michael Andres <ma@suse.de>

/-*/
#ifndef PathInfo_h
#define PathInfo_h

extern "C"
{
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
}

#include <cerrno>
#include <iosfwd>
#include <list>

#include <y2util/Pathname.h>

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : PathInfo
//
//	DESCRIPTION :
//
class PathInfo {

  public:

    enum Mode { STAT, LSTAT };

  private:

    Pathname    path_t;

    struct stat statbuf_C;
    Mode        mode_e;
    int         error_i;

  public:

    PathInfo( const Pathname & path = "", Mode initial = STAT );
    PathInfo( const std::string & path, Mode initial = STAT );
    PathInfo( const char * path, Mode initial = STAT );
    virtual ~PathInfo();

    const Pathname &    path()     const { return path_t; }
    const std::string & asString() const { return path_t.asString(); }
    Mode                mode()     const { return mode_e; }
    int                 error()    const { return error_i; }

    void setPath( const Pathname & path ) { if ( path != path_t ) error_i = -1; path_t = path; }
    void setMode( Mode mode )             { if ( mode != mode_e ) error_i = -1; mode_e = mode; }

    bool stat      ( const Pathname & path ) { setPath( path ); setMode( STAT );  return operator()(); }
    bool lstat     ( const Pathname & path ) { setPath( path ); setMode( LSTAT ); return operator()(); }
    bool operator()( const Pathname & path ) { setPath( path ); return operator()(); }

    bool stat()   { setMode( STAT );  return operator()(); }
    bool lstat()  { setMode( LSTAT ); return operator()(); }
    bool operator()();

  public:

    bool   isExist() const { return !error_i; }

    // file type
    bool   isFile()  const { return isExist() && S_ISREG( statbuf_C.st_mode ); }
    bool   isDir ()  const { return isExist() && S_ISDIR( statbuf_C.st_mode ); }
    bool   isLink()  const { return isExist() && S_ISLNK( statbuf_C.st_mode ); }
    bool   isChr()   const { return isExist() && S_ISCHR( statbuf_C.st_mode ); }
    bool   isBlk()   const { return isExist() && S_ISBLK( statbuf_C.st_mode ); }
    bool   isFifo()  const { return isExist() && S_ISFIFO( statbuf_C.st_mode ); }
    bool   isSock()  const { return isExist() && S_ISSOCK( statbuf_C.st_mode ); }

    nlink_t nlink()  const { return isExist() ? statbuf_C.st_nlink : 0; }

    // owner
    uid_t  owner()   const { return isExist() ? statbuf_C.st_uid : 0; }
    gid_t  group()   const { return isExist() ? statbuf_C.st_gid : 0; }

    // permission
    bool   isRUsr()  const { return isExist() && (statbuf_C.st_mode & S_IRUSR); }
    bool   isWUsr()  const { return isExist() && (statbuf_C.st_mode & S_IWUSR); }
    bool   isXUsr()  const { return isExist() && (statbuf_C.st_mode & S_IXUSR); }

    bool   isR()     const { return isRUsr(); }
    bool   isW()     const { return isWUsr(); }
    bool   isX()     const { return isXUsr(); }

    bool   isRGrp()  const { return isExist() && (statbuf_C.st_mode & S_IRGRP); }
    bool   isWGrp()  const { return isExist() && (statbuf_C.st_mode & S_IWGRP); }
    bool   isXGrp()  const { return isExist() && (statbuf_C.st_mode & S_IXGRP); }

    bool   isROth()  const { return isExist() && (statbuf_C.st_mode & S_IROTH); }
    bool   isWOth()  const { return isExist() && (statbuf_C.st_mode & S_IWOTH); }
    bool   isXOth()  const { return isExist() && (statbuf_C.st_mode & S_IXOTH); }

    bool   isUid()   const { return isExist() && (statbuf_C.st_mode & S_ISUID); }
    bool   isGid()   const { return isExist() && (statbuf_C.st_mode & S_ISGID); }
    bool   isVtx()   const { return isExist() && (statbuf_C.st_mode & S_ISVTX); }

    mode_t uperm()   const { return isExist() ? (statbuf_C.st_mode & S_IRWXU) : 0; }
    mode_t gperm()   const { return isExist() ? (statbuf_C.st_mode & S_IRWXG) : 0; }
    mode_t operm()   const { return isExist() ? (statbuf_C.st_mode & S_IRWXO) : 0; }
    mode_t perm()    const { return isExist() ? (statbuf_C.st_mode & (S_IRWXU|S_IRWXG|S_IRWXO|S_ISUID|S_ISGID|S_ISVTX)) : 0; }

    bool   isPerm ( mode_t m ) const { return (m == perm()); }
    bool   hasPerm( mode_t m ) const { return (m == (m & perm())); }

    // device
    dev_t  dev()     const { return isExist() ? statbuf_C.st_dev  : 0; }
    dev_t  rdev()    const { return isExist() ? statbuf_C.st_rdev : 0; }
    ino_t  ino()     const { return isExist() ? statbuf_C.st_ino  : 0; }

    // size
    off_t         size()    const { return isExist() ? statbuf_C.st_size : 0; }
    unsigned long blksize() const { return isExist() ? statbuf_C.st_blksize : 0; }
    unsigned long blocks()  const { return isExist() ? statbuf_C.st_blocks  : 0; }

    // time
    time_t atime()   const { return isExist() ? statbuf_C.st_atime : 0; } /* time of last access */
    time_t mtime()   const { return isExist() ? statbuf_C.st_mtime : 0; } /* time of last modification */
    time_t ctime()   const { return isExist() ? statbuf_C.st_ctime : 0; }

  public:

    ///////////////////////////////////////////////////////////////////
    // convenience stuff
    ///////////////////////////////////////////////////////////////////
    // static functions as they may or may not invalidate any stat info
    // stored by a PathiInfo.
    ///////////////////////////////////////////////////////////////////

    /**
     * Like '::mkdir'. Attempt to create a new directory named path. mode
     * specifies the permissions to use. It is modified by the process's
     * umask in the usual way.
     *
     * @return 0 on success, errno on failure
     **/
    static int mkdir( const Pathname & path, unsigned mode = 0755 );

    /**
     * Like 'mkdir -p'. No error if directory exists. Make parent directories
     * as needed. mode specifies the permissions to use, if directories have to
     * be created. It is modified by the process's umask in the usual way.
     *
     * @return 0 on success, errno on failure
     **/
    static int assert_dir( const Pathname & path, unsigned mode = 0755 );

    /**
     * Like '::rmdir'. Delete a directory, which must be empty.
     *
     * @return 0 on success, errno on failure
     **/
    static int rmdir( const Pathname & path );

    /**
     * Like '::unlink'. Delete a file (symbolic link, socket, fifo or device).
     *
     * @return 0 on success, errno on failure
     **/
    static int unlink( const Pathname & path );

    /**
     * Like 'rm -r DIR'. Delete a directory, recursively removing its contents.
     *
     * @return 0 on success, ENOTDIR if path is not a directory, otherwise the
     * commands return value.
     **/
    static int recursive_rmdir( const Pathname & path );

    /**
     * Like 'rm -r DIR/ *'. Delete directory contents, but keep the directory itself.
     *
     * @return 0 on success, ENOTDIR if path is not a directory, otherwise the
     * commands return value.
     **/
    static int clean_dir( const Pathname & path );

    /**
     * Like 'cp -a srcpath destpath'. Copy directory tree. srcpath/destpath must be
     * directories. 'basename srcpath' must not exist in destpath.
     *
     * @return 0 on success, ENOTDIR if srcpath/destpath is not a directory, EEXIST if
     * 'basename srcpath' exists in destpath, otherwise the commands return value.
     **/
    static int copy_dir( const Pathname & srcpath, const Pathname & destpath );

    /**
     * Return content of directory via retlist. If dots is false
     * entries starting with '.' are not reported. "." and ".."
     * are never reported.
     *
     * @return 0 on success, errno on failure.
     **/
    static int readdir( std::list<std::string> & retlist,
			const Pathname & path, bool dots = true );
};

///////////////////////////////////////////////////////////////////

extern std::ostream & operator<<( std::ostream & str, const PathInfo & obj );

///////////////////////////////////////////////////////////////////

#endif // PathInfo_h
