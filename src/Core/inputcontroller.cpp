#include "inputcontroller.hpp"

#include <iostream>

InputController::InputController() {
}

InputController::~InputController() {
}

void InputController::ClearWhileKeyPressed() {
    m_whileKeyPressed.clear();
}

void InputController::DeregisterWhileKeyPressed(InputKey key) {
    m_whileKeyPressed.erase(key);
}

void InputController::RegisterWhileKeyPressed(InputKey key, const std::function<void(void)>& lambda) {
    m_whileKeyPressed[key] = lambda;
}

void InputController::ClearOnKeyPressed() {
    m_onKeyPressed.clear();
}

void InputController::DeregisterOnKeyPressed(InputKey key) {
    m_onKeyPressed.erase(key);
}

void InputController::RegisterOnKeyPressed(InputKey key, const std::function<void(void)>& lambda) {
    m_onKeyPressed[key] = lambda;
}

void InputController::CallKeyLambdas() {
    for(auto& key : m_pressedKeys) {
        auto lambda = m_whileKeyPressed.find(key.first);
        if(lambda != m_whileKeyPressed.end()) {
            lambda->second();
        }

        if(key.second) {
            key.second = false;
            auto lambda = m_onKeyPressed.find(key.first);
            if(lambda != m_onKeyPressed.end()) {
                lambda->second();
            }
        }
    }
}

bool InputController::IsKeyPressed(InputKey key) const {
	return m_pressedKeys.find(key) != m_pressedKeys.end();
}

glm::vec2 InputController::GetMouseLocation() const {
	return glm::vec2(0.0f, 0.0f);
}

void InputController::UpdateKey(int key, bool pressed) {
	InputKey ikey = static_cast<InputKey>(key);

	if(pressed && m_pressedKeys.find(ikey) == m_pressedKeys.end()) {
		m_pressedKeys[ikey] = true;
	} else if(m_pressedKeys.find(ikey) != m_pressedKeys.end()) {
		m_pressedKeys.erase(ikey);
	}
}
