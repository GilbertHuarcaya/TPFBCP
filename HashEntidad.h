#pragma once

template <typename T>
class HashEntidad {
private:
	int key;
	T value;
public:
	HashEntidad(int key, T value) {
		this->key = key;
		this->value = value;
	}
	int getKey() { return key; }
	T getValue() { return value; }
};
