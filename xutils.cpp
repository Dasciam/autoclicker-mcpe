#include "xutils.hpp"

std::string xutils::GetWindowTitle(Display *display, Window window) {
  char *window_name = nullptr;
  int status = XFetchName(display, window, &window_name);
  if (status == 0) {
	return "";
  }
  std::string title(window_name);
  XFree(window_name);
  return title;
}

bool xutils::IsLeftMouseButtonPressed(Display *display) {
  Window root_window;
  int root_x, root_y;
  unsigned int mask;
  XQueryPointer(
	  display,
	  RootWindow(
		  display,
		  DefaultScreen(display)
	  ),
	  &root_window,
	  &root_window,
	  &root_x,
	  &root_y,
	  &root_x,
	  &root_y,
	  &mask
  );

  return mask & Button1Mask;
}

void xutils::SimulateLeftMouseButtonClick(Display *display, Window window) {
  XEvent event;
  event.type = ButtonPress;
  event.xbutton.button = Button1;
  event.xbutton.same_screen = True;
  XSendEvent(display, window, True, ButtonPressMask, &event);

  event.type = ButtonRelease;
  event.xbutton.button = Button1;
  event.xbutton.same_screen = True;
  XSendEvent(display, window, True, ButtonReleaseMask, &event);
}

Window xutils::GetActiveWindow(Display *display) {
  Window active_window;
  int revert_to;
  XGetInputFocus(display, &active_window, &revert_to);
  return active_window;
}

Display *xutils::GetDefaultDisplay() {
  return XOpenDisplay(nullptr);
}
void xutils::CloseDisplaySession(Display *display) {
  XCloseDisplay(display);
}
