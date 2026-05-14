#include "Button.h"

Button::Button(
    uint8_t pin, 
    unsigned long debounceMs, 
    unsigned long longPressMs, 
    unsigned long doubleClickMs
) : _pin(pin),
    _debounceMs(debounceMs),
    _longPressMs(longPressMs),
    _doubleClickMs(doubleClickMs) {}

void Button::begin() {
    pinMode(_pin, INPUT_PULLUP);
    _lastReading    = digitalRead(_pin);
    _stableState    = _lastReading;
    _lastChangeTime = millis();
}

void Button::update() {
    const unsigned long now = millis();
    const bool reading = digitalRead(_pin);

    // Debounce listening
    if (reading != _lastReading) {
        _lastChangeTime = now;
        _lastReading    = reading;
    }

    // If the reading is in the debounce time and the state has changed
    if ((now - _lastChangeTime) >= _debounceMs && reading != _stableState) {
        _stableState = reading;

        if (_stableState == LOW) {
            fire(_onPress);

            if (_state == WAIT_DOUBLE) {
                fire(_onDoubleClick);
                _state          = PRESSED;
                _pressStartTime = now;
            } else {
                _state          = PRESSED;
                _pressStartTime = now;
            }
        } else {
            fire(_onRelease);
            _releaseTime = now;
            
            if (_state == LONG_PRESS_FIRED) {
                // Long press already sent
                _state = IDLE;
            } else if (_state == PRESSED) {
                if (_pressStartTime == 0) {
                    // Release of 2nd press of double click
                    _state = IDLE;
                } else if (_doubleClickEnabled) {
                    // Waiting for another click before sending
                    _state = WAIT_DOUBLE;
                } else {
                    fire(_onClick);
                    _state = IDLE;
                }
            }
        }
    }

    // Detect if its a long press
    if (_state == PRESSED && _stableState == LOW && _pressStartTime != 0) {
        if ((now - _pressStartTime) >= _longPressMs) {
            fire(_onLongPress);
            _state = LONG_PRESS_FIRED;
        }
    }

    if (_state == WAIT_DOUBLE && (now - _releaseTime) >= _doubleClickMs) {
        fire(_onClick);
        _state = IDLE;
    }
}

void Button::onPress(Callback cb)       { _onPress      = cb; }
void Button::onRelease(Callback cb)     { _onRelease    = cb; }
void Button::onClick(Callback cb)       { _onClick      = cb; }
void Button::onLongPress(Callback cb)   { _onLongPress  = cb; }
void Button::onDoubleClick(Callback cb) {
    _onDoubleClick      = cb;
    _doubleClickEnabled = true;
}