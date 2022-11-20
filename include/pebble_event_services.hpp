#pragma once

namespace Pebble {

//! @addtogroup TickTimerService
//! @{

class TickTimer
{
protected:
    //! Event handler type for tick timer events
    //! @param tick_time the time at which the tick event was triggered
    //! @param units_changed which unit change triggered this tick event
    virtual void OnTick(tm &tick_time, TimeUnits units_changed) = 0; 

    //! Tick Timer destructor; ends the timer    
    virtual ~TickTimer() { TickTimerEnd(); }

    //! Starts the tick timer; fires on the OnTick event on every requested unit change.
    //! Calling this method multiple times will override the units and handler (i.e., only 
    //! the last tick_units and handler passed will be used).
    //! @param tick_units a bitmask of all the units that have changed
    inline void TickTimerStart(TimeUnits tick_units) { 
        active() = this;
        tick_timer_service_subscribe(tick_units, handler_tick);
    }    

    //! Stops the tick timer events. 
    inline void TickTimerEnd() { if (active() == this) { tick_timer_service_unsubscribe(); active() = 0; } }
    
private:
    //! Static pointer to the active timer instance
    inline static TickTimer*& active() { static TickTimer* active = 0; return active; }
    
    //! Callback type for tick timer events
    //! @param tick_time the time at which the tick event was triggered
    //! @param units_changed which unit change triggered this tick event    
    static void handler_tick(tm *tick_time, TimeUnits units_changed) { active()->OnTick(*tick_time, units_changed); }
};

//! @}

}