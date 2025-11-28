#ifndef APP_VIRTUALKEYBOARD_HPP
#define APP_VIRTUALKEYBOARD_HPP

#include "event_loop.hpp"
#include "../rmioc/virtualkeyboard.hpp"

namespace rmioc
{
    class screen;
    class virtualkeyboard;
}

namespace app
{

class screen;
class virtualkeyboard
{
public:
    virtualkeyboard(
        rmioc::virtualkeyboard& device,
        app::screen& screen,
        VirtualKeyboardCallback send_virtual_key_press
    );

    /**
     * Process input from the software keyboard.
     */
    event_loop_status process_events();

private:
    /** reMarkable keyboard device. */
    rmioc::virtualkeyboard& device;

    app::screen& screen;

    /** Previous keyboard state. */
    rmioc::virtualkeyboard::virtualkeyboard_state previous_state;

    VirtualKeyboardCallback send_virtual_key_press;
};

} // namespace app

#endif // APP_KEYBOARD_HPP
