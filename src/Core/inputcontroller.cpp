#include "inputcontroller.hpp"

InputController::InputController() {
}

InputController::~InputController() {
}

void InputController::ClearWhileKeyPressed() {
    m_keyLambdas.clear();
}

void InputController::DeregisterWhileKeyPressed(InputKey key) {
    m_keyLambdas.erase(key);
}

void InputController::RegisterWhileKeyPressed(InputKey key, const std::function<void(void)>& lambda) {
    m_keyLambdas[key] = lambda;
}

void InputController::CallKeyLambdas() {
    for(InputKey key : m_pressedKeys) {
        auto lambda = m_keyLambdas.find(key);
        if(lambda != m_keyLambdas.end()) {
            lambda->second();
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
		m_pressedKeys.insert(ikey);
	} else if(m_pressedKeys.find(ikey) != m_pressedKeys.end()) {
		m_pressedKeys.erase(ikey);
	}
}
