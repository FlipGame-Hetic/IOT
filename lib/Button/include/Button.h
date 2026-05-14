#pragma once

#include <Arduino.h>

class Button {
    public:
        using Callback = std::function<void()>;

        /**
         * @param pin           GPIO of the Button
         * @param debounceMs    Debounce Time (default 50ms)
         * @param longPressMs   threshold for the long press (default 800ms)
         * @param doubleClickMs threshold for the doubleClick (default 300ms)
         */
        
        Button(
            uint8_t pin,
            unsigned long debounceMs    = 50,
            unsigned long longPressMs   = 800,
            unsigned long doubleClickMs = 300
        );

        // Call in setup, config the pin in INPUT_PULLUP
        void begin();

        // Call in loop
        void update();

        void onPress(Callback cb);
        void onRelease(Callback cb);
        void onClick(Callback cb);
        void onLongPress(Callback cb);
        void onDoubleClick(Callback cb);

        void enableDoubleClick(bool enable) { _doubleClickEnabled = enable; }

        bool isPressed() const { return _stableState == LOW; }

    private:
        enum State : uint8_t {
            IDLE,
            PRESSED,
            WAIT_DOUBLE,
            LONG_PRESS_FIRED
        };

        uint8_t _pin;
        uint16_t _debounceMs;
        uint16_t _longPressMs;
        uint16_t _doubleClickMs;

        State _state            = IDLE;
        bool _lastReading       = HIGH;
        bool _stableState       = HIGH;
        unsigned long _lastChangeTime   = 0;
        unsigned long _pressStartTime   = 0;
        unsigned long _releaseTime      = 0;
        bool _doubleClickEnabled        = false;

        Callback _onPress           = nullptr;
        Callback _onRelease         = nullptr;
        Callback _onClick           = nullptr;
        Callback _onLongPress       = nullptr;
        Callback _onDoubleClick     = nullptr;

        static inline void fire(const Callback &cb) { if (cb) cb(); }
};