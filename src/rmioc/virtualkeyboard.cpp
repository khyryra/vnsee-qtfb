#include "virtualkeyboard.hpp"
#include <vector>
#include <fcntl.h>
#include <linux/input-event-codes.h>
#include <linux/input.h>

namespace rmioc
{

virtualkeyboard::virtualkeyboard(const char* device_path)
: input(device_path)
{}

auto virtualkeyboard::process_events() -> bool
{
    auto events = this->fetch_events();

    if (!events.empty())
    {
        for (const input_event& event : events)
        {
            if (event.type == EV_KEY)
            {
                this->state.last_key = event.code;
                this->state.key_pressed = (event.value != 0);
            }
        }

        return true;
    }

    return false;
}

auto virtualkeyboard::get_state() const -> const virtualkeyboard::virtualkeyboard_state&
{
    return this->state;
}

} // namespace rmioc
