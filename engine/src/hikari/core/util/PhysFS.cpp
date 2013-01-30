#include "hikari/core/util/PhysFS.hpp"

namespace PhysFS
{
  void init(const char* argv0, bool symLinks)
  {
    if (!PHYSFS_init(argv0))
      throw Exception(PHYSFS_getLastError());

    PHYSFS_permitSymbolicLinks(symLinks);
  }

  void deinit()
  {
    if (!PHYSFS_deinit())
      throw Exception(PHYSFS_getLastError());
  }

  bool isInit()
  {
    return PHYSFS_isInit() != 0;
  }


  StringVector supportedArchiveTypes()
  {
    const PHYSFS_ArchiveInfo** lst = PHYSFS_supportedArchiveTypes();
    StringVector list;
    for (; *lst != 0; ++lst)
      list.push_back((*lst)->extension);
    return list;
    // lst is static data, don't free
  }


  StringVector getCdRomDirs()
  {
    StringVector list;
    char** lst = PHYSFS_getCdRomDirs();
    for (char** l = lst; *l != 0; ++l)
      list.push_back(*l);
    PHYSFS_freeList(lst);
    return list;
  }


  void addToSearchPath(const std::string& location, bool append)
  {
    if (!PHYSFS_addToSearchPath(location.c_str(), append))
      throw Exception(PHYSFS_getLastError());
  }

  void removeFromSearchPath(const std::string& location)
  {
    if (!PHYSFS_removeFromSearchPath(location.c_str()))
      throw Exception(PHYSFS_getLastError());
  }

  void mount(const std::string& location, const std::string& mountPoint, bool append)
  {
    if (!PHYSFS_mount(location.c_str(), mountPoint.c_str(), append))
      throw Exception(PHYSFS_getLastError());
  }

  std::string getMountPoint(const std::string& location)
  {
    std::string mount = PHYSFS_getMountPoint(location.c_str());
    if (mount.empty())
      throw Exception(PHYSFS_getLastError());
    return mount;
  }


  StringVector getSearchPath()
  {
    char** locList = PHYSFS_getSearchPath();
    if (locList == 0)
      throw Exception(PHYSFS_getLastError());

    StringVector list;
    for (char** l = locList; *l != 0; ++l)
      list.push_back(*l);
    PHYSFS_freeList(locList);
    return list;
  }

  const std::string& getUserDir()
  {
    // since this won't likely change during one execution, we cache the result
    static const std::string userDir = PHYSFS_getUserDir();
    return userDir;
  }

  const std::string& getBaseDir()
  {
    // cache the result
    static const std::string baseDir = PHYSFS_getBaseDir();
    return baseDir;
  }



  std::string getWriteDir()
  {
    return PHYSFS_getWriteDir();
  }

  void setWriteDir(const std::string& dir)
  {
    if (!PHYSFS_setWriteDir(dir.c_str()))
      throw Exception(PHYSFS_getLastError());
  }



  void setSaneConfig(const std::string& organisation, const std::string& appName, 
    const std::string& archiveExt, bool includeCdDrives, bool archivesFirst)
  {
    if (!PHYSFS_setSaneConfig(organisation.c_str(), appName.c_str(), archiveExt.c_str(),
      includeCdDrives, archivesFirst))
    {
      throw Exception(PHYSFS_getLastError());
    }
  }



  bool exists(const std::string& path)
  {
    return PHYSFS_exists(path.c_str()) != 0;
  }

  bool isDirectory(const std::string& path)
  {
    return PHYSFS_isDirectory(path.c_str()) != 0;
  }

  bool isSymbolicLink(const std::string& path)
  {
    return PHYSFS_isSymbolicLink(path.c_str()) != 0;
  }


  int64 getLastModTime(const std::string& file)
  {
    int64 timestamp = PHYSFS_getLastModTime(file.c_str());
    if (timestamp == -1)
      throw Exception("PhysFS::getLastModTime: Can't determine time of last modification");
    return timestamp;
  }


  void mkdir(const std::string& dir)
  {
    if (!PHYSFS_mkdir(dir.c_str()))
      throw Exception(PHYSFS_getLastError());
  }


  void remove(const std::string& path)
  {
    if (!PHYSFS_delete(path.c_str()))
      throw Exception(PHYSFS_getLastError());
  }


  std::string getRealDir(const std::string& file)
  {
    std::string dir = PHYSFS_getRealDir(file.c_str());
    if (dir.empty())
      throw Exception("PhysFS::getRealDir: File not found");
    return dir;
  }


  StringVector enumerateFiles(const std::string& dir)
  {
    StringVector list;
    char** lst = PHYSFS_enumerateFiles(dir.c_str());
    for (char** l = lst; *l != 0; ++l)
      list.push_back(*l);
    PHYSFS_freeList(lst);
    return list;
  }

  StringVector getFileListing(const std::string& dir)
  {
    StringVector tmpList = enumerateFiles(dir);
    // now filter the list to only keep files
    StringVector list;
    for (StringVector::iterator it = tmpList.begin(); it != tmpList.end(); ++it)
    {
      if (!isDirectory(*it))
        list.push_back(*it);
    }
    return list;
  }

  StringVector getDirListing(const std::string& dir)
  {
    StringVector tmpList = enumerateFiles(dir);
    // now filter the list to only keep dirs
    StringVector list;
    for (StringVector::iterator it = tmpList.begin(); it != tmpList.end(); ++it)
    {
      if (isDirectory(*it))
        list.push_back(*it);
    }
    return list;
  }
}
