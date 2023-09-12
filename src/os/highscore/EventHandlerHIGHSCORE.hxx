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

#ifndef EVENTHANDLER_HIGHSCORE_HXX
#define EVENTHANDLER_HIGHSCORE_HXX

#include "EventHandler.hxx"

/**
  This class handles event collection from the point of view of the specific
  backend toolkit (HIGHSCORE).  It converts from HIGHSCORE-specific events into events
  that the Stella core can understand.

  @author  Stephen Anthony
*/
class EventHandlerHIGHSCORE : public EventHandler
{
  public:
    /**
      Create a new HIGHSCORE event handler object
    */
    explicit EventHandlerHIGHSCORE(OSystem& osystem) : EventHandler(osystem) { }
    ~EventHandlerHIGHSCORE() override = default;

  private:
    /**
      Enable/disable text events (distinct from single-key events).
    */
    void enableTextEvents(bool enable) override { }

    /**
      Collects and dispatches any pending SDL2 events.
    */
    void pollEvent() override { }

  private:
    // Following constructors and assignment operators not supported
    EventHandlerHIGHSCORE() = delete;
    EventHandlerHIGHSCORE(const EventHandlerHIGHSCORE&) = delete;
    EventHandlerHIGHSCORE(EventHandlerHIGHSCORE&&) = delete;
    EventHandlerHIGHSCORE& operator=(const EventHandlerHIGHSCORE&) = delete;
    EventHandlerHIGHSCORE& operator=(EventHandlerHIGHSCORE&&) = delete;
};

#endif
