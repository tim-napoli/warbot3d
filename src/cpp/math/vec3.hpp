#ifndef _math_vec3_hpp_
#define _math_vec3_hpp_

namespace math {

class vec2 {
    private:
        float v [3];
    
    public:
        vec3 (float x = 0, float y = 0, float z = 0);
        vec3 (const vec2& xy, float z);
        vec3 (float x, const vec2& yz);
        
        ~vec3 ();
};

}

#endif

