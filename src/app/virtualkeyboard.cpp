#include "virtualkeyboard.hpp"
#include "screen.hpp"
#include "../rmioc/screen.hpp"
#include "../rmioc/virtualkeyboard.hpp"
#include <iostream>

namespace app
{

virtualkeyboard::virtualkeyboard(
    rmioc::virtualkeyboard& device,
    app::screen& screen,
    VirtualKeyboardCallback send_virtual_key_press
)
: device(device)
, screen(screen)
, send_virtual_key_press(std::move(send_virtual_key_press))
, previous_state{}
{}

auto virtualkeyboard::process_events() -> event_loop_status
{
    if (this->device.process_events())
    {
        auto device_state = this->device.get_state();

        if (device_state.last_key != -1) {
            if (device_state.last_key == 0xf001) {
                this->screen.set_repaint_mode(screen::repaint_modes::fast);
            } else if (device_state.last_key == 0xf000) {
                this->screen.set_repaint_mode(screen::repaint_modes::standard);
                this->screen.repaint();
            } else {
                this->send_virtual_key_press(device_state.last_key, device_state.key_pressed);
            }
        }

        this->previous_state = device_state;
    }

    return {/* quit = */ false, /* timeout = */ -1};
}

} // namespace app
