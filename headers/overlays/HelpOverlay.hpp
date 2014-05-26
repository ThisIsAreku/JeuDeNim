#ifndef _HELP_OVERLAY_H_
#define _HELP_OVERLAY_H_

#include "display/Overlay.hpp"

class HelpOverlay : public Overlay
{
    typedef Overlay super;
public:
    HelpOverlay(WindowManager *);
    virtual ~HelpOverlay();

    void initOverlay();
    UpdateState updateOverlay(chtype);
    void renderOverlay();
};

#endif
