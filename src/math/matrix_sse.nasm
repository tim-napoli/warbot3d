global matrix_copy_sse

global matrix_add_sse
global matrix_add_ex_sse

global matrix_transpose_sse
global matrix_transpose_ex_sse

global matrix_apply_sse2

global matrix_mul_sse2
global matrix_mul_ex_sse2


section .text

matrix_copy_sse:    mov     eax, [esp + 4]
                    mov     edx, [esp + 8]
                    
                    movups  xmm0, [edx]
                    movups  xmm1, [edx + 16]
                    movups  xmm2, [edx + 32]
                    movups  xmm3, [edx + 48]
                    
                    movups  [eax],      xmm0
                    movups  [eax + 16], xmm1
                    movups  [eax + 32], xmm2
                    movups  [eax + 48], xmm3

                    ret
                    
matrix_transpose_sse:   mov     eax, [esp + 4]
                        
                        movups      xmm0, [eax]         ; xmm0 = a0 a1 a2 a3
                        movups      xmm1, [eax + 16]    ; xmm1 = b0 b1 b2 b3
                        movups      xmm2, [eax + 32]    ; xmm2 = c0 c1 c2 c3
                        movups      xmm3, [eax + 48]    ; xmm3 = d0 d1 d2 d3
                        
                        movaps      xmm4, xmm0
                        unpcklps    xmm0, xmm2  ; xmm0 = a0 c0 a1 c1
                        unpckhps    xmm4, xmm2  ; xmm4 = a2 c2 a3 c3
                        
                        movaps      xmm2, xmm1
                        unpcklps    xmm1, xmm3  ; xmm1 = b0 d0 b1 d1
                        unpckhps    xmm2, xmm3  ; xmm2 = b2 d2 b3 d3
                        
                        movaps      xmm3, xmm0 
                        unpcklps    xmm0, xmm1  ; xmm0 = a0 b0 c0 d0
                        unpckhps    xmm3, xmm1  ; xmm3 = a1 b1 c1 d1
                        
                        movaps      xmm1, xmm4  
                        unpcklps    xmm1, xmm2  ; xmm1 = a2 b2 c2 d2
                        unpckhps    xmm4, xmm2  ; xmm4 = a3 b3 c3 d3
                        
                        movups      [eax],      xmm0
                        movups      [eax + 16], xmm3
                        movups      [eax + 32], xmm1
                        movups      [eax + 48], xmm4

                        ret
                      
                        
matrix_transpose_ex_sse:    mov     eax, [esp + 8]
                        
                            movups      xmm0, [eax]         ; xmm0 = a0 a1 a2 a3
                            movups      xmm1, [eax + 16]    ; xmm1 = b0 b1 b2 b3
                            movups      xmm2, [eax + 32]    ; xmm2 = c0 c1 c2 c3
                            movups      xmm3, [eax + 48]    ; xmm3 = d0 d1 d2 d3
                            
                            movaps      xmm4, xmm0
                            unpcklps    xmm0, xmm2  ; xmm0 = a0 c0 a1 c1
                            unpckhps    xmm4, xmm2  ; xmm4 = a2 c2 a3 c3
                            
                            movaps      xmm2, xmm1
                            unpcklps    xmm1, xmm3  ; xmm1 = b0 d0 b1 d1
                            unpckhps    xmm2, xmm3  ; xmm2 = b2 d2 b3 d3
                            
                            movaps      xmm3, xmm0 
                            unpcklps    xmm0, xmm1  ; xmm0 = a0 b0 c0 d0
                            unpckhps    xmm3, xmm1  ; xmm3 = a1 b1 c1 d1
                            
                            movaps      xmm1, xmm4  
                            unpcklps    xmm1, xmm2  ; xmm1 = a2 b2 c2 d2
                            unpckhps    xmm4, xmm2  ; xmm4 = a3 b3 c3 d3
                            
                            mov         eax, [esp + 4]
                            movups      [eax],      xmm0
                            movups      [eax + 16], xmm3
                            movups      [eax + 32], xmm1
                            movups      [eax + 48], xmm4

                            ret


matrix_add_sse: mov     eax, [esp + 4]
                mov     edx, [esp + 8]
                
                movups  xmm0, [eax]
                movups  xmm1, [eax + 16]
                movups  xmm2, [eax + 32]
                movups  xmm3, [eax + 48]
                
                movups  xmm4, [edx]
                movups  xmm5, [edx + 16]
                movups  xmm6, [edx + 32]
                movups  xmm7, [edx + 48]
                
                addps   xmm0, xmm4
                addps   xmm1, xmm5
                addps   xmm2, xmm6
                addps   xmm3, xmm7
                
                movups  [eax],      xmm0
                movups  [eax + 16], xmm1
                movups  [eax + 32], xmm2
                movups  [eax + 48], xmm3
                  
                ret
     
                
matrix_add_ex_sse:  mov     eax, [esp + 8]
                    mov     edx, [esp + 12]
                
                    movups  xmm0, [eax]
                    movups  xmm1, [eax + 16]
                    movups  xmm2, [eax + 32]
                    movups  xmm3, [eax + 48]
                    
                    movups  xmm4, [edx]
                    movups  xmm5, [edx + 16]
                    movups  xmm6, [edx + 32]
                    movups  xmm7, [edx + 48]
                    
                    addps   xmm0, xmm4
                    addps   xmm1, xmm5
                    addps   xmm2, xmm6
                    addps   xmm3, xmm7
                    
                    mov     eax, [esp + 4]
                    movups  [eax],      xmm0
                    movups  [eax + 16], xmm1
                    movups  [eax + 32], xmm2
                    movups  [eax + 48], xmm3
                      
                    ret

matrix_mul_sse2:    mov     edx, [esp + 8]      ; edx is B

                    sub     esp, 64         ; Allocating memory for transposed B matrix.
                    mov     ecx, esp
                    push    dword edx
                    push    dword ecx
                    call    matrix_transpose_ex_sse
                    add     esp, 8
                
                    movups  xmm0, [esp]
                    movups  xmm1, [esp + 16]
                    movups  xmm2, [esp + 32]
                    movups  xmm3, [esp + 48]
                    add esp, 64
                    
                    mov     eax, [esp + 4]
                    mov     edx, [esp + 8]      ; edx is B
                    
                    movups  xmm4, [eax]
                    movaps  xmm5, xmm4
                    movaps  xmm6, xmm4
                    movaps  xmm7, xmm4
                    mulps   xmm4, xmm0
                    mulps   xmm5, xmm1
                    mulps   xmm6, xmm2
                    mulps   xmm7, xmm3
                    haddps  xmm4, xmm4
                    haddps  xmm4, xmm4
                    haddps  xmm5, xmm5
                    haddps  xmm5, xmm5
                    haddps  xmm6, xmm6
                    haddps  xmm6, xmm6
                    haddps  xmm7, xmm7
                    haddps  xmm7, xmm7
                    pslldq  xmm7, 4
                    movss   xmm7, xmm6
                    pslldq  xmm7, 4
                    movss   xmm7, xmm5
                    pslldq  xmm7, 4
                    movss   xmm7, xmm4
                    movups   [eax], xmm7
                    
                    movups  xmm4, [eax + 16]
                    movaps  xmm5, xmm4
                    movaps  xmm6, xmm4
                    movaps  xmm7, xmm4
                    mulps   xmm4, xmm0
                    mulps   xmm5, xmm1
                    mulps   xmm6, xmm2
                    mulps   xmm7, xmm3
                    haddps  xmm4, xmm4
                    haddps  xmm5, xmm5
                    haddps  xmm6, xmm6
                    haddps  xmm7, xmm7
                    haddps  xmm4, xmm4
                    haddps  xmm5, xmm5
                    haddps  xmm6, xmm6
                    haddps  xmm7, xmm7
                    pslldq  xmm7, 4
                    movss   xmm7, xmm6
                    pslldq  xmm7, 4
                    movss   xmm7, xmm5
                    pslldq  xmm7, 4
                    movss   xmm7, xmm4
                    movups   [eax + 16], xmm7
                      
                    movups  xmm4, [eax + 32]
                    movaps  xmm5, xmm4
                    movaps  xmm6, xmm4
                    movaps  xmm7, xmm4
                    mulps   xmm4, xmm0
                    mulps   xmm5, xmm1
                    mulps   xmm6, xmm2
                    mulps   xmm7, xmm3
                    haddps  xmm4, xmm4
                    haddps  xmm5, xmm5
                    haddps  xmm6, xmm6
                    haddps  xmm7, xmm7
                    haddps  xmm4, xmm4
                    haddps  xmm5, xmm5
                    haddps  xmm6, xmm6
                    haddps  xmm7, xmm7
                    pslldq  xmm7, 4
                    movss   xmm7, xmm6
                    pslldq  xmm7, 4
                    movss   xmm7, xmm5
                    pslldq  xmm7, 4
                    movss   xmm7, xmm4
                    movups   [eax + 32], xmm7
                    
                    movups  xmm4, [eax + 48]
                    movaps  xmm5, xmm4
                    movaps  xmm6, xmm4
                    movaps  xmm7, xmm4
                    mulps   xmm4, xmm0
                    mulps   xmm5, xmm1
                    mulps   xmm6, xmm2
                    mulps   xmm7, xmm3
                    haddps  xmm4, xmm4
                    haddps  xmm5, xmm5
                    haddps  xmm6, xmm6
                    haddps  xmm7, xmm7
                    haddps  xmm4, xmm4
                    haddps  xmm5, xmm5
                    haddps  xmm6, xmm6
                    haddps  xmm7, xmm7
                    pslldq  xmm7, 4
                    movss   xmm7, xmm6
                    pslldq  xmm7, 4
                    movss   xmm7, xmm5
                    pslldq  xmm7, 4
                    movss   xmm7, xmm4
                    movups   [eax + 48], xmm7
                      
                    ret
                    
                    
matrix_mul_ex_sse2: mov     edx, [esp + 12]      ; edx is B

                    sub     esp, 64         ; Allocating memory for transposed B matrix.
                    mov     ecx, esp
                    push    dword edx
                    push    dword ecx
                    call    matrix_transpose_ex_sse
                    add     esp, 8
                
                    movups  xmm0, [esp]
                    movups  xmm1, [esp + 16]
                    movups  xmm2, [esp + 32]
                    movups  xmm3, [esp + 48]
                    add esp, 64
                    
                    mov     eax, [esp + 8]
                    mov     edx, [esp + 4]      ; edx is C
                    
                    movups  xmm4, [eax]
                    movaps  xmm5, xmm4
                    movaps  xmm6, xmm4
                    movaps  xmm7, xmm4
                    mulps   xmm4, xmm0
                    mulps   xmm5, xmm1
                    mulps   xmm6, xmm2
                    mulps   xmm7, xmm3
                    haddps  xmm4, xmm4
                    haddps  xmm4, xmm4
                    haddps  xmm5, xmm5
                    haddps  xmm5, xmm5
                    haddps  xmm6, xmm6
                    haddps  xmm6, xmm6
                    haddps  xmm7, xmm7
                    haddps  xmm7, xmm7
                    movss   [eax],      xmm4
                    movss   [eax + 4],  xmm5
                    movss   [eax + 8],  xmm6
                    movss   [eax + 12], xmm7
                    
                    movups  xmm4, [eax + 16]
                    movaps  xmm5, xmm4
                    movaps  xmm6, xmm4
                    movaps  xmm7, xmm4
                    mulps   xmm4, xmm0
                    mulps   xmm5, xmm1
                    mulps   xmm6, xmm2
                    mulps   xmm7, xmm3
                    haddps  xmm4, xmm4
                    haddps  xmm5, xmm5
                    haddps  xmm6, xmm6
                    haddps  xmm7, xmm7
                    haddps  xmm4, xmm4
                    haddps  xmm5, xmm5
                    haddps  xmm6, xmm6
                    haddps  xmm7, xmm7                    
                    movss   [eax + 16], xmm4
                    movss   [eax + 20], xmm5
                    movss   [eax + 24], xmm6
                    movss   [eax + 28], xmm7
                      
                    movups  xmm4, [eax + 32]
                    movaps  xmm5, xmm4
                    movaps  xmm6, xmm4
                    movaps  xmm7, xmm4
                    mulps   xmm4, xmm0
                    mulps   xmm5, xmm1
                    mulps   xmm6, xmm2
                    mulps   xmm7, xmm3
                    haddps  xmm4, xmm4
                    haddps  xmm5, xmm5
                    haddps  xmm6, xmm6
                    haddps  xmm7, xmm7
                    haddps  xmm4, xmm4
                    haddps  xmm5, xmm5
                    haddps  xmm6, xmm6
                    haddps  xmm7, xmm7
                    movss   [eax + 32], xmm4
                    movss   [eax + 36], xmm5
                    movss   [eax + 40], xmm6
                    movss   [eax + 44], xmm7
                    
                    movups  xmm4, [eax + 48]
                    movaps  xmm5, xmm4
                    movaps  xmm6, xmm4
                    movaps  xmm7, xmm4
                    mulps   xmm4, xmm0
                    mulps   xmm5, xmm1
                    mulps   xmm6, xmm2
                    mulps   xmm7, xmm3
                    haddps  xmm4, xmm4
                    haddps  xmm5, xmm5
                    haddps  xmm6, xmm6
                    haddps  xmm7, xmm7
                    haddps  xmm4, xmm4
                    haddps  xmm5, xmm5
                    haddps  xmm6, xmm6
                    haddps  xmm7, xmm7
                    movss   [eax + 48], xmm4
                    movss   [eax + 52], xmm5
                    movss   [eax + 56], xmm6
                    movss   [eax + 60], xmm7
                      
                    ret
                    

matrix_apply_sse2:  mov     eax, [esp + 4]  ; eax is the matrix
                    mov     ecx, [esp + 8]  ; ecx is the "in" vector
                    mov     edx, [esp + 12] ; edx is the "out" vector
                    
                    movups  xmm0, [eax]
                    movups  xmm1, [eax + 16]
                    movups  xmm2, [eax + 32]
                    movups  xmm3, [eax + 48]
                    
                    movups  xmm4, [ecx]
                    movaps  xmm5, xmm4
                    movaps  xmm6, xmm4
                    movaps  xmm7, xmm4
                    
                    mulps   xmm4, xmm0
                    mulps   xmm5, xmm1
                    mulps   xmm6, xmm2
                    mulps   xmm7, xmm3
                    
                    haddps  xmm4, xmm4
                    haddps  xmm4, xmm4
                    haddps  xmm5, xmm5
                    haddps  xmm5, xmm5
                    haddps  xmm6, xmm6
                    haddps  xmm6, xmm6
                    haddps  xmm7, xmm7
                    haddps  xmm7, xmm7
                    
                    movss   [edx],      xmm4
                    movss   [edx + 4],  xmm5
                    movss   [edx + 8],  xmm6
                    movss   [edx + 12], xmm7
                    
                    ret
                    
