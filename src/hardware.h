#ifdef HARD_XIAO
    #warning Using Hardware XIAO RP2040
#elif defined(HARD_PICO)
    #warning Using Hardware PI PICO
#else

    #error Keine Plattform ausgewählt

#endif