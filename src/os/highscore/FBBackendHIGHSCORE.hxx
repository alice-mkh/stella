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
// Copyright (c) 1995-2026 by Bradford W. Mott, Stephen Anthony
// and the Stella Team
//
// See the file "License.txt" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//============================================================================

#ifndef FB_BACKEND_HIGHSCORE_HXX
#define FB_BACKEND_HIGHSCORE_HXX

class OSystem;

#include "bspf.hxx"
#include "FBBackend.hxx"
#include "FBSurfaceHIGHSCORE.hxx"

/**
  This class implements a standard HIGHSCORE framebuffer backend.  Most of
  the functionality is not used, since highscore has its own rendering system.

  @author  Stephen Anthony
*/
class FBBackendHIGHSCORE : public FBBackend
{
  public:
    explicit FBBackendHIGHSCORE(OSystem&) { }
    ~FBBackendHIGHSCORE() override { }

  protected:
    /**
      This method is called to query and initialize the video hardware
      for desktop and fullscreen resolution information.  Since several
      monitors may be attached, we need the resolution for all of them.

      @param fullscreenRes  Maximum resolution supported in fullscreen mode
      @param windowedRes    Maximum resolution supported in windowed mode
      @param renderers      List of renderer names (internal name -> end-user name)
    */
    void queryHardware(std::unordered_map<uInt32, Common::Size>& fullscreenRes,
                       std::unordered_map<uInt32, Common::Size>& windowedRes,
                       VariantList& renderers) override
    {
      fullscreenRes.try_emplace(0, 1920, 1080);
      windowedRes.try_emplace(0, 1920, 1080);

      VarList::push_back(renderers, "software", "Software");
    }

    /**
      This method is called to create a surface with the given attributes.

      @param w     The requested width of the new surface.
      @param h     The requested height of the new surface.
    */
    unique_ptr<FBSurface>
      createSurface(uInt32 w, uInt32 h, ScalingInterpolation,
                    const uInt32*) const override
    {
      return std::make_unique<FBSurfaceHIGHSCORE>(w, h);
    }

    /**
      This method is called to provide information about the backend.
    */
    string about() const override { return "Video system: highscore"; }


    //////////////////////////////////////////////////////////////////////
    // Most methods here aren't used at all.  See FBBacked class for
    // description, if needed.
    //////////////////////////////////////////////////////////////////////

    int scaleX(int x) const override { return x; }
    int scaleY(int y) const override { return y; }
    void setTitle(string_view) override { }
    void showCursor(bool) override { }
    bool fullScreen() const override { return true; }
    uInt32 rMask() const override { return 0; }
    uInt32 gMask() const override { return 0; }
    uInt32 bMask() const override { return 0; }
    uInt32 aMask() const override { return 0; }
    const FBSurface& compositedSurface() { static FBSurfaceHIGHSCORE tmp(0, 0); return tmp; }
    bool isCurrentWindowPositioned() const override { return true; }
    Common::Point getCurrentWindowPos() const override { return Common::Point{}; }
    uInt32 getCurrentDisplayID() const override { return 0; }
    void clear() override { }
    bool setVideoMode(const VideoModeHandler::Mode&,
                      uInt32, const Common::Point&) override { return true; }
    void grabMouse(bool) override { }
    void enableTextEvents(bool enable) override { }
    void renderToScreen() override { }
    int refreshRate() const override { return 0; }
    bool isLightTheme() const override { return false; }
    bool isDarkTheme() const override { return false; }

  private:
    // Following constructors and assignment operators not supported
    FBBackendHIGHSCORE() = delete;
    FBBackendHIGHSCORE(const FBBackendHIGHSCORE&) = delete;
    FBBackendHIGHSCORE(FBBackendHIGHSCORE&&) = delete;
    FBBackendHIGHSCORE& operator=(const FBBackendHIGHSCORE&) = delete;
    FBBackendHIGHSCORE& operator=(FBBackendHIGHSCORE&&) = delete;
};

#endif
