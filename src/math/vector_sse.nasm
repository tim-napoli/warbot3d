global vector_add_sse
global vector_add_ex_sse

global vector_sub_sse
global vector_sub_ex_sse

global vector_scalar_mul_sse
global vector_scalar_mul_ex_sse

global vector_length_sse
global vector_length_sse2
global vector_length_sse41

global vector_dot_product_sse2
global vector_dot_product_sse41

global vector_cross_product_sse

global vector_normalize_sse
global vector_normalize_sse2

global vector_normalize_ex_sse
global vector_normalize_ex_sse2


section .text

vector_add_sse: mov     eax, [esp + 8]
                movups  xmm0, [eax]
            
                mov     eax, [esp + 4]
                movups  xmm1, [eax]
            
                addps   xmm0, xmm1
                movups  [eax], xmm0
                
                ret
            
            
vector_add_ex_sse:  mov     eax, [esp + 12]
                    movups  xmm0, [eax]
                
                    mov     eax, [esp + 8]
                    movups  xmm1, [eax]
                    
                    addps   xmm0, xmm1
                    
                    mov     eax, [esp + 4]
                    movups  [eax], xmm0
                    
                    ret
     
     
     
vector_sub_sse: mov     eax, [esp + 8]
                movups  xmm1, [eax]
                
                mov     eax, [esp + 4]
                movups  xmm0, [eax]
                
                subps   xmm0, xmm1
                movups  [eax], xmm0
                
                ret
            
            
vector_sub_ex_sse:  mov     eax, [esp + 8]
                    movups  xmm0, [eax]
                
                    mov     eax, [esp + 12]
                    movups  xmm1, [eax]
                    
                    subps   xmm0, xmm1
                    
                    mov     eax, [esp + 4]
                    movups  [eax], xmm0
                    
                    ret
                
              
                
vector_scalar_mul_sse:  movss   xmm1, [esp + 8]
                        shufps  xmm1, xmm1, 0
                        
                        mov     eax, [esp + 4]
                        movups  xmm0, [eax]
                        
                        mulps   xmm0, xmm1
                        
                        movups  [eax], xmm0
                        
                        ret
           
                    
vector_scalar_mul_ex_sse:   movss   xmm1, [esp + 12]
                            shufps  xmm1, xmm1, 0
                        
                            mov     eax, [esp + 8]
                            movups  xmm0, [eax]
                            
                            mulps   xmm0, xmm1
                            
                            mov     eax, [esp + 4]
                            movups  [eax], xmm0
                            
                            ret
            
                        
vector_length_sse:  mov     eax, [esp + 4]
                    movups  xmm0, [eax]
                    movups  xmm1, xmm0
                    
                    mulps   xmm0, xmm1
                    
                    movups  xmm1, xmm0
                    shufps  xmm1, xmm1, 01b
                    addss   xmm0, xmm1
                    
                    movups  xmm1, xmm0
                    shufps  xmm1, xmm1, 10b
                    addss   xmm0, xmm1
                    
                    ; Pour w...
                    ;movups  xmm1, xmm0
                    ;shufps  xmm1, xmm1, 11b
                    ;addss   xmm0, xmm1
                    
                    sqrtss  xmm0, xmm0
                    
                    sub     esp, 4
                    movss   [esp], xmm0
                    
                    fld     dword [esp]
                    
                    add     esp, 4
                    
                    ret
              
                    
vector_length_sse2: mov     eax, [esp + 4]
                    movups  xmm0, [eax]
                    movups  xmm1, xmm0
                    
                    ; Evaluates length.
                    mulps   xmm0, xmm1
                    haddps  xmm0, xmm0
                    haddps  xmm0, xmm0
                    sqrtss  xmm0, xmm0
                    
                    ; Putting the result on the floats stack
                    sub     esp, 4
                    movss   [esp], xmm0
                    fld     dword [esp]
                    add     esp, 4
                    
                    ret


vector_length_sse41:    mov     eax, [esp + 4]
                        movups  xmm0, [eax]
                        movups  xmm1, xmm0
                        
                        ; Evaluates length.
                        dpps    xmm0, xmm1, 11110001b
                        sqrtss  xmm0, xmm0
                        
                        ; Putting the result on the floats stack
                        sub     esp, 4
                        movss   [esp], xmm0
                        fld     dword [esp]
                        add     esp, 4
                            
                        ret
              
                         
vector_dot_product_sse2: ; Getting u and v
                         mov     eax, [esp + 4]
                         movups  xmm0, [eax]
                         mov     eax, [esp + 8]
                         movups  xmm1, [eax]
                        
                         ; Evaluating u.v
                         mulps   xmm0, xmm1
                         haddps  xmm0, xmm0
                         haddps  xmm0, xmm0
                        
                         ; Putting the result on the floats stack
                         sub     esp, 4
                         movss   [esp], xmm0
                         fld     dword [esp]
                         add     esp, 4

                         ret   
                
                         
vector_dot_product_sse41:   ; Getting u and v
                            mov     eax, [esp + 4]
                            movups  xmm0, [eax]
                            mov     eax, [esp + 8]
                            movups  xmm1, [eax]
                            
                            ; Evaluating u.v
                            dpps xmm0, xmm1, 11110001b
                            
                            ; Putting the result on the floats stack
                            sub     esp, 4
                            movss   [esp], xmm0
                            fld     dword [esp]
                            add     esp, 4
                            
                            ret 
                
                            
vector_cross_product_sse:   mov     eax,    [esp + 8]
                            movups  xmm0,   [eax]
                            movups  xmm2,   xmm0
                            mov     eax,    [esp + 12]
                            movups  xmm1,   [eax]
                            movups  xmm3,   xmm1
                            
                            ; x = 00, y = 01, z = 10, w = 11
                            shufps  xmm0, xmm0, 11001001b   ; u.w | u.x | u.z | u.y
                            shufps  xmm1, xmm1, 11010010b   ; v.w | v.y | v.x | u.z
                            mulps   xmm0, xmm1
                            
                            shufps  xmm2, xmm2, 11010010b   ; u.w | u.y | u.x | u.z
                            shufps  xmm3, xmm3, 11001001b   ; v.w | v.x | v.z | u.y
                            mulps   xmm2, xmm3
                            
                            subps   xmm0, xmm2
                            
                            mov     eax,    [esp + 4]
                            movups  [eax],  xmm0
                            
                            ret
   
                            
vector_normalize_sse:       mov     eax,    [esp + 4]
                            
                            ; Evaluates the vector length.
                            movups  xmm0,   [eax]
                            movups  xmm1,   xmm0
                            mulps   xmm0, xmm1
                            
                            ; Sum elements
                            movups  xmm1, xmm0
                            shufps  xmm1, xmm1, 01b
                            addss   xmm0, xmm1
                            movups  xmm1, xmm0
                            shufps  xmm1, xmm1, 10b
                            addss   xmm0, xmm1
                            
                            sqrtss  xmm0, xmm0
                            
                            rcpss   xmm0, xmm0     ; xmm0 = 1.0 / ||u||
                            
                            shufps  xmm0, xmm0, 00000000b
                            movups  xmm1, [eax]
                            mulps   xmm0, xmm1
                            
                            movups  [eax], xmm0
                            
                            ret
                            

vector_normalize_sse2:      mov     eax,    [esp + 4]
                            
                            ; Evaluates the vector length.
                            movups  xmm0,   [eax]
                            movups  xmm1,   xmm0
                            mulps   xmm0, xmm1
                            
                            ; Sum elements
                            haddps  xmm0, xmm0
                            haddps  xmm0, xmm0
                            
                            sqrtss  xmm0, xmm0
                            
                            rcpss   xmm0, xmm0     ; xmm0 = 1.0 / ||u||
                            
                            shufps  xmm0, xmm0, 00000000b
                            movups  xmm1, [eax]
                            mulps   xmm0, xmm1
                            
                            movups  [eax], xmm0
                            
                            ret
                            
vector_normalize_ex_sse:    mov     eax,    [esp + 8]
                            
                            ; Evaluates the vector length.
                            movups  xmm0,   [eax]
                            movups  xmm1,   xmm0
                            mulps   xmm0, xmm1
                            
                            ; Sum elements
                            movups  xmm1, xmm0
                            shufps  xmm1, xmm1, 01b
                            addss   xmm0, xmm1
                            movups  xmm1, xmm0
                            shufps  xmm1, xmm1, 10b
                            addss   xmm0, xmm1
                            
                            sqrtss  xmm0, xmm0
                            
                            rcpss   xmm0, xmm0     ; xmm0 = 1.0 / ||u||
                            
                            shufps  xmm0, xmm0, 00000000b
                            movups  xmm1, [eax]
                            mulps   xmm0, xmm1
                            
                            mov     eax,   [esp + 4]
                            movups  [eax], xmm0
                            
                            ret
                            

vector_normalize_ex_sse2:   mov     eax,    [esp + 8]
                            
                            ; Evaluates the vector length.
                            movups  xmm0,   [eax]
                            movups  xmm1,   xmm0
                            mulps   xmm0, xmm1
                            
                            ; Sum elements
                            haddps  xmm0, xmm0
                            haddps  xmm0, xmm0
                            
                            sqrtss  xmm0, xmm0
                            rcpss   xmm0, xmm0     ; xmm0 = 1.0 / ||u||
                            shufps  xmm0, xmm0, 00000000b
                            
                            movups  xmm1, [eax]
                            mulps   xmm0, xmm1
                            
                            mov     eax, [esp + 4]
                            movups  [eax], xmm0
                            
                            ret
                            
