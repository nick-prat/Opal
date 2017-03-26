#include "inputcontroller.hpp"

#include <iostream>

void InputController::clearWhileKeyPressed() {
    m_whileKeyPressed.clear();
}

void InputController::deregisterWhileKeyPressed(const InputKey key) {
    m_whileKeyPressed.erase(key);
}

void InputController::registerWhileKeyPressed(const InputKey key, const std::function<void(InputKey)>& lambda) {
    m_whileKeyPressed[key] = lambda;
}

void InputController::clearOnKeyPressed() {
    m_onKeyPressed.clear();
}

void InputController::deregisterOnKeyPressed(const InputKey key) {
    m_onKeyPressed.erase(key);
}

void InputController::registerOnKeyPressed(const InputKey key, const std::function<void(InputKey)>& lambda) {
    m_onKeyPressed[key] = lambda;
}

void InputController::callKeyLambdas() {
    for(auto& key : m_pressedKeys) {
        auto lambda = m_whileKeyPressed.find(key.first);
        if(lambda != m_whileKeyPressed.end()) {
            lambda->second(key.first);
        }

        if(key.second) {
            key.second = false;
            auto lambda = m_onKeyPressed.find(key.first);
            if(lambda != m_onKeyPressed.end()) {
                lambda->second(key.first);
            }
        }
    }
}

void InputController::updateMousePosition(const double xpos, const double ypos) {
    m_mousePos.first = xpos;
    m_mousePos.second = ypos;
}

const std::pair<double, double>& InputController::getMousePosition() const {
    return m_mousePos;
}

bool InputController::isKeyPressed(const InputKey key) const {
	return m_pressedKeys.find(key) != m_pressedKeys.end();
}

glm::vec2 InputController::getMouseLocation() const {
	return glm::vec2(0.0f, 0.0f);
}

void InputController::updateKey(const int key, const bool pressed) {
	InputKey ikey = static_cast<InputKey>(key);

	if(pressed && m_pressedKeys.find(ikey) == m_pressedKeys.end()) {
		m_pressedKeys[ikey] = true;
	} else if(m_pressedKeys.find(ikey) != m_pressedKeys.end()) {
		m_pressedKeys.erase(ikey);
	}
}
