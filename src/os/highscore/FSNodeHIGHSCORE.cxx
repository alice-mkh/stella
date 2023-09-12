//============================================================================
//
//   SSSS    tt          lll  lll
//  SS  SS   tt           ll   ll
//  SS     tttttt  eeee   ll   ll   aaaa
//   SSSS    tt   ee  ee  ll   ll      aa
//      SS   tt   eeeeee  ll   ll   aaaaa  --  "An Atari 2600 VCS Emulator"
//  SS  SS   tt   ee      ll   ll  aa  aa
//   SSSS     ttt  eeeee llll llll  aaaaa
//
// Copyright (c) 1995-2023 by Bradford W. Mott, Stephen Anthony
// and the Stella Team
//
// See the file "License.txt" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//============================================================================

#include "bspf.hxx"
#include "Cart.hxx"
#include "FSNodeHIGHSCORE.hxx"

#ifdef _WIN32
  const string SLASH = "\\";
#else
  const string SLASH = "/";
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
FSNodeHIGHSCORE::FSNodeHIGHSCORE()
  : _name{"rom"},
    _path{"." + SLASH}
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
FSNodeHIGHSCORE::FSNodeHIGHSCORE(string_view p)
  : _name{p},
    _path{p}
{
  if(p == "." + SLASH + "nvram") {
    extern const char* hs_get_save_path(void);
    _path = hs_get_save_path();
  }
}

static bool
string_ends_with(string str, string suffix)
{
  if(str.length() < suffix.length())
    return false;

  string str_suffix = str.substr(str.length() - suffix.length());

  return str_suffix == suffix;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool FSNodeHIGHSCORE::exists() const
{
  if(string_ends_with(getPath(), ".pro"))
    return false;

  return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool FSNodeHIGHSCORE::isReadable() const
{
  return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool FSNodeHIGHSCORE::isWritable() const
{
  return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
string FSNodeHIGHSCORE::getShortPath() const
{
  return ".";
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool FSNodeHIGHSCORE::
    getChildren(AbstractFSList& myList, ListMode mode) const
{
  return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool FSNodeHIGHSCORE::makeDir()
{
  return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool FSNodeHIGHSCORE::rename(string_view newfile)
{
  return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
AbstractFSNodePtr FSNodeHIGHSCORE::getParent() const
{
  return nullptr;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
size_t FSNodeHIGHSCORE::read(ByteBuffer& image, size_t) const
{
  image = make_unique<uInt8[]>(Cartridge::maxSize());

  extern uInt32 hs_read_rom(void* data);
  return hs_read_rom(image.get());
}
