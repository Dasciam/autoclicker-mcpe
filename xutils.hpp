#ifndef XUTILS_HPP
#define XUTILS_HPP

#include <string>
#include <X11/Xlib.h>

namespace xutils {
Display *GetDefaultDisplay();

void CloseDisplaySession(Display *);

std::string GetWindowTitle(Display *, Window);

bool IsLeftMouseButtonPressed(Display *);

void SimulateLeftMouseButtonClick(Display *, Window);

Window GetActiveWindow(Display *);
}

#endif //XUTILS_HPP
