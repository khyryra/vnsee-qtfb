#include "touch.hpp"
#include "screen.hpp"
#include "../rmioc/screen.hpp"
#include "../rmioc/touch.hpp"
#include <cstdlib>
#include <functional>
#include <map>
#include <utility>
// IWYU pragma: no_include <ratio>
// IWYU pragma: no_include <type_traits>

namespace app
{

touch::touch(
    rmioc::touch& device,
    app::screen& screen,
    const rmioc::screen& screen_device,
    MouseCallback send_button_press
)
: device(device)
, screen(screen)
, screen_device(screen_device)
, send_button_press(std::move(send_button_press))
{}

auto touch::process_events(bool inhibit) -> event_loop_status
{
    if (this->device.process_events())
    {
        if (inhibit)
        {
            this->state = TouchState::Inactive; // @todo Check if this is the cause of touch lifts not being registered
        }
        else
        {
            auto device_state = this->device.get_state();

            if (!device_state.empty())
            {
                // Compute the mean touch position
                int summed_x = 0;
                int summed_y = 0;
                int total_points = static_cast<int>(device_state.size());

                for (const auto& [id, slot] : device_state)
                {
                    summed_x += slot.x;
                    summed_y += slot.y;
                }

                summed_x /= total_points;
                summed_y /= total_points;

                // Convert to screen coordinates
                int screen_xres = this->screen_device.get_xres();
                int screen_yres = this->screen_device.get_yres();

                int touch_xres = this->device.get_xres();
                int touch_yres = this->device.get_yres();

                int screen_x = summed_x * screen_xres / touch_xres;
                int screen_y = summed_y * screen_yres / touch_yres;

                if (this->state == TouchState::Inactive)
                {
                    this->screen.set_repaint_mode(screen::repaint_modes::fast);
                }

                this->state = TouchState::Tap;

                this->x_last = screen_x;
                this->y_last = screen_y;

                this->send_button_press(
                    screen_x, screen_y,
                    MouseButton::Left
                );
            }
            else
            {
                if (this->state == TouchState::Tap) {
                    this->send_button_press(
                        this->x_last, this->y_last,
                        MouseButton::None
                    );

                    this->screen.set_repaint_mode(screen::repaint_modes::standard);
                    this->screen.repaint();
                }

                this->state = TouchState::Inactive;
            }
        }
    }

    return {/* quit = */ false, /* timeout = */ -1};
}

} // namespace app
