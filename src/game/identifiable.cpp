#include "game/identifiable.h"

Identifiable::Identifiable()
	: m_id(s_current_id++) {
}

unsigned int Identifiable::get_id() const {
	return m_id;
}

unsigned int Identifiable::s_current_id = 0;
