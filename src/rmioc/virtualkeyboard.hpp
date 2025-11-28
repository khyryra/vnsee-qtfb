#ifndef RMIOC_VIRTUALKEYBOARD_HPP
#define RMIOC_VIRTUALKEYBOARD_HPP

#include "input.hpp"

namespace rmioc
{

/**
 * Access to the state of the device’s virtual keyboard.
 */
class virtualkeyboard : public input
{
public:
    /**
     * Open the virtual keyboard device.
     *
     * @param path Path to the device.
     */
    virtualkeyboard(const char* device_path);

    /**
     * Fetch new events from the virtual keyboard and process them.
     *
     * @return True if the keys’ state changed since last call.
     */
    bool process_events();

    /**
     * Information about each key of the device.
     */
    struct virtualkeyboard_state
    {
        bool key_pressed = false;
        int last_key = -1;
    };

    const virtualkeyboard_state& get_state() const;

private:
    virtualkeyboard_state state;
}; // class virtualkeyboard

} // namespace rmioc

#endif // RMIOC_VIRTUALKEYBOARD_HPP
