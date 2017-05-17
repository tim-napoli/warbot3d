#include <cmath>

#include "math/vec2.hpp"

namespace math {

vec2::vec2 (float x, float y) : _v {x, y} {

}

vec2::vec2 (const vec2& u) : _v {u.x (), u.y ()} {

}

vec2::~vec2 () {

}

vec2& vec2::add (const vec2& u) {
    _v [0] += u.x ();
    _v [1] += u.y ();
    
    return *this;
}

vec2& vec2::sub (const vec2& u) {
    _v [0] -= u.x ();
    _v [1] -= u.y ();
    
    return *this;
}

vec2& vec2::mul (float f) {
    _v [0] *= f;
    _v [1] *= f;
    
    return *this;
}

vec2& vec2::div (float f) {
    _v [0] /= f;
    _v [1] /= f;
    
    return *this;
}

float vec2::dot (const vec2& v) {
    return _v [0] * v.x () + _v [1] * v.y ();
}

float vec2::cross (const vec2& v) {
    return _v [0] * v.y () - _v [1] * v.x ();
}

float vec2::powLength () {
    return _v [0] * _v [0] + _v [1] * _v [1];
}

float vec2::length () {
    return sqrt (_v [0] * _v [0] + _v [1] * _v [1]);
}

float vec2::angle (const vec2& v) {
    vec2 diff = v.sub (*this);
    return atan2 (diff.y (), diff.x ());
}

}
