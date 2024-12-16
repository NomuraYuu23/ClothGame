#pragma once

/// <summary>
/// セーフデリート
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="p"></param>
template<class T> inline void SafeDelete(T*& p) {
	delete p;
	p = nullptr;
}
