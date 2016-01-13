#pragma once 

class Identifiable {
public:
	Identifiable();

	unsigned int get_id() const;
private:
	static unsigned int s_current_id;

	unsigned int m_id;
};
