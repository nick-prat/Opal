#include "inputcontroller.hpp"

InputController::InputController() {
	for(int i = 0; i < 256; i++) {
		m_keys[i] = false;
	}
}

InputController::~InputController() {
}

bool InputController::IsKeyPressed(unsigned char key) const {
	return m_keys[key];
}

glm::vec2 InputController::GetMouseLocation() const {
	return glm::vec2(0.0f, 0.0f);
}

void InputController::UpdateKey(unsigned char key, bool pressed) {
	m_keys[key] = pressed;
}
