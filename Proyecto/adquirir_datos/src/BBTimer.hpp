//
// Created by B Bryant on 9/15/2021
//
// Access the five nRF52840 32-bit hardware timers.
// User must ensure selected hardware timers do not conflict with application
// utilized MBED OS features or other library timer usage.
//
// For each timer used:
//  - Call setupTimer() to set initial period and callback function.
//  - Call startTimer() and stopTimer() as needed.
//  - Use updatePeriod() to change period of a running timer.
//    updatePeriod() can be called from within the user callback.
//
// Note that the callback provided to setupTimer() is called by the timer
// interrupt service routine (ISR), so do not call resources that depend on
// interrupts. Also, be sure callback execution time will not exceed the timer
// period, or the system will lock up.
//
// Although array bounds are managed using BBTimerIdType indexes,
// there is no explicit parameter, bounds, or nullptr error checking.

#pragma once
#include <nrf_timer.h>

typedef enum {
	BB_TIMER0 = 0,
	BB_TIMER1,
	BB_TIMER2,
	BB_TIMER3,
	BB_TIMER4,
	NUM_TIMERS
} BBTimerIdType;

typedef void (*TimerCallback)();

class BBTimer
{
public: // members

private: // Static, per class, members
	static NRF_TIMER_Type* const NRF_TIMER_LIST[NUM_TIMERS];
	static const IRQn_Type       IRQ_LIST[NUM_TIMERS];
	static const void*           ISR_LIST[NUM_TIMERS];
	static BBTimer*              timer_object[NUM_TIMERS];

private: // per object members
	BBTimerIdType   timer_id;
	NRF_TIMER_Type* nrf_timer;
	IRQn_Type       timer_irq;
	TimerCallback   isr_callback;

public: // methods
	/// BBTimer - Select hardware timer.  May construct at global scope.
	/// No system calls in constructor. OS may overwrite vectors after global
	/// constructors, so wait for setup() to call NVIC_SetVector.
	explicit BBTimer(BBTimerIdType id){
		timer_id  = id;
		nrf_timer = NRF_TIMER_LIST[timer_id];
		timer_irq = IRQ_LIST[timer_id];
		isr_callback = nullptr;

		timer_object[timer_id] = this;  // static object enables ISR to find this instance
  }

	/// setupTimer - Set timer period.  The provided callback function will be
	/// called periodically at the timer period after start() is called.
	/// Call setupTimer() in or after setup().
	void setupTimer(unsigned int period_us, TimerCallback callback)
	{
		// save pointer to callback function to be called by timer ISR
		isr_callback = callback;

		// Initialize hardware timer's registers
		nrf_timer->TASKS_STOP  = 1;  // Stop counter, just in case already running
		nrf_timer->TASKS_CLEAR = 1;  // counter to zero

		nrf_timer->BITMODE   = 3UL;       // 32 bit
		nrf_timer->MODE      = 0UL;       // timer, not counter
		nrf_timer->PRESCALER = 4UL;       // freq = 16Mhz / 2^prescaler = 1Mhz
		nrf_timer->CC[0]     = period_us; // Counter is compared to this
		nrf_timer->INTENSET  = 1UL << TIMER_INTENSET_COMPARE0_Pos;     // interrupt on compare event.
		nrf_timer->SHORTS    = 1UL << TIMER_SHORTS_COMPARE0_CLEAR_Pos; // clear counter on compare event.

		// It's also possible to add a SHORT to STOP counter upon compare.
		// I leave counter running to keep period constant. The downside is that
		// the ISR must complete before period expires, or it will lock up.

		// Point to one of the five static ISR handlers, which will call this
		// object's myObjectISR() via the static timer_object list.
		NVIC_SetVector(timer_irq, (uint32_t)ISR_LIST[timer_id]);
	}

	/// updatePeriod - Change the period of a setup, started, or stopped timer.
	/// May be called from the callback function.
	/// Must call setupTimer(), once, before using updatePeriod().
	inline void updatePeriod(unsigned int new_period_us) const
	{
		nrf_timer->CC[0] = new_period_us;
	}

	inline void timerStart() const
	{
		NVIC_EnableIRQ(timer_irq);
		nrf_timer->TASKS_START = 1;
	}

	inline void timerStop() const
	{
		NVIC_DisableIRQ(timer_irq);
		nrf_timer->TASKS_STOP = 1;
	}

private: // methods
	static void timer0Isr()
	{
		NVIC_DisableIRQ(IRQ_LIST[BB_TIMER0]);
		BBTimer::timer_object[BB_TIMER0]->instanceIsr();
		NVIC_EnableIRQ(IRQ_LIST[BB_TIMER0]);
	}
	static void timer1Isr()
	{
		NVIC_DisableIRQ(IRQ_LIST[BB_TIMER1]);
		BBTimer::timer_object[BB_TIMER1]->instanceIsr();
		NVIC_EnableIRQ(IRQ_LIST[BB_TIMER1]);
	}
	static void timer2Isr()
	{
		NVIC_DisableIRQ(IRQ_LIST[BB_TIMER2]);
		BBTimer::timer_object[BB_TIMER2]->instanceIsr();
		NVIC_EnableIRQ(IRQ_LIST[BB_TIMER2]);
	}

	static void timer3Isr()
	{
		NVIC_DisableIRQ(IRQ_LIST[BB_TIMER3]);
		BBTimer::timer_object[BB_TIMER3]->instanceIsr();
		NVIC_EnableIRQ(IRQ_LIST[BB_TIMER3]);
	}

	static void timer4Isr()
	{
		NVIC_DisableIRQ(IRQ_LIST[BB_TIMER4]);
		BBTimer::timer_object[BB_TIMER4]->instanceIsr();
		NVIC_EnableIRQ(IRQ_LIST[BB_TIMER4]);
	}

	/// instanceIsr - called by static ISR after IRQ disabled.
	/// Provides ISR access to BBTimer instance member variables.
	void instanceIsr()
	{
		if (nrf_timer->EVENTS_COMPARE[0] == 1) // make sure this is a compare event
		{
			nrf_timer->EVENTS_COMPARE[0] = 0; // clear event flag
			// Counter is cleared automatically via SHORTS

			isr_callback(); // caller's callback function
		}
	}
};

/// Declare and initialize Static BBTimer class members
BBTimer* BBTimer::timer_object[NUM_TIMERS] = {
		nullptr, nullptr, nullptr, nullptr, nullptr
};

// const pointers to non const register structures
NRF_TIMER_Type* const BBTimer::NRF_TIMER_LIST[NUM_TIMERS] = {
		NRF_TIMER0, NRF_TIMER1, NRF_TIMER2, NRF_TIMER3, NRF_TIMER4
};

const IRQn_Type BBTimer::IRQ_LIST[NUM_TIMERS] = {
		TIMER0_IRQn, TIMER1_IRQn, TIMER2_IRQn, TIMER3_IRQn, TIMER4_IRQn
};

const void* BBTimer::ISR_LIST[NUM_TIMERS] = {
		(void*)BBTimer::timer0Isr, (void*)BBTimer::timer1Isr,
		(void*)BBTimer::timer2Isr, (void*)BBTimer::timer3Isr,
		(void*)BBTimer::timer4Isr
};