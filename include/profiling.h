#pragma once

#if defined(TRACY_ENABLE)
    #include <tracy/Tracy.hpp>
#else
    #define ZoneScoped
    #define ZoneScopedN
    #define FrameMark
    #define FrameMarkStart
    #define FrameMarkEnd
#endif
