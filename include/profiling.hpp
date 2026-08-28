#pragma once

#if defined(TRACY_ENABLE)
    #include <tracy/Tracy.hpp>
#else
    #define ZoneScoped
    #define ZoneScopedN(X)
    #define FrameMark
    #define FrameMarkStart(X)
    #define FrameMarkEnd(X)
#endif
