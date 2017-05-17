#ifndef _math_vec2_hpp_
#define _math_vec2_hpp_

namespace math {

class vec2 {
    private:
        float _v [2];
    
    public:
        vec2 (float x = 0, float y = 0);
        vec2 (const vec2& v);
        
        ~vec2 ();
        
        float x () const {return _v [0];}
        float y () const {return _v [1];}
        
        float& x () {return _v [0];}
        float& y () {return _v [1];}
        
        
        vec2& add (const vec2& v);
        
        vec2& sub (const vec2& v);
        
        vec2& mul (float f);
        
        vec2& div (float f);
        
        float dot (const vec2& v);
        
        float cross (const vec2& v);
        
        float powLength ();
        
        float length ();
        
        float angle (const vec2& v = vec2 (0, 0));
};

}

#endif

