	.text
	.file	"vector_t.cpp"
	.section	.text.startup,"ax",@progbits
	.p2align	4, 0x90         # -- Begin function __cxx_global_var_init
	.type	__cxx_global_var_init,@function
__cxx_global_var_init:                  # @__cxx_global_var_init
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movabsq	$_ZStL8__ioinit, %rdi
	callq	_ZNSt8ios_base4InitC1Ev
	movabsq	$_ZNSt8ios_base4InitD1Ev, %rax
	movq	%rax, %rdi
	movabsq	$_ZStL8__ioinit, %rsi
	movabsq	$__dso_handle, %rdx
	callq	__cxa_atexit
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end0:
	.size	__cxx_global_var_init, .Lfunc_end0-__cxx_global_var_init
	.cfi_endproc
                                        # -- End function
	.text
	.globl	main                    # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
.Lfunc_begin0:
	.cfi_startproc
	.cfi_personality 3, __gxx_personality_v0
	.cfi_lsda 3, .Lexception0
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$192, %rsp
	movl	$0, -4(%rbp)
	movl	%edi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movl	.Lconstinit+16(%rip), %eax
	movl	%eax, -80(%rbp)
	movups	.Lconstinit(%rip), %xmm0
	movaps	%xmm0, -96(%rbp)
	leaq	-96(%rbp), %rcx
	movq	%rcx, -64(%rbp)
	movq	$5, -56(%rbp)
	leaq	-104(%rbp), %rcx
	movq	%rcx, %rdi
	movq	%rcx, -160(%rbp)        # 8-byte Spill
	callq	_ZNSaIiEC2Ev
	movq	-64(%rbp), %rsi
	movq	-56(%rbp), %rdx
.Ltmp0:
	leaq	-48(%rbp), %rdi
	movq	-160(%rbp), %rcx        # 8-byte Reload
	callq	_ZNSt6vectorIiSaIiEEC2ESt16initializer_listIiERKS0_
.Ltmp1:
	jmp	.LBB1_1
.LBB1_1:
	leaq	-104(%rbp), %rdi
	callq	_ZNSaIiED2Ev
	leaq	-48(%rbp), %rdi
	callq	_ZNKSt6vectorIiSaIiEE4sizeEv
.Ltmp3:
	movl	$_ZSt4cout, %edi
	movq	%rax, %rsi
	callq	_ZNSolsEm
.Ltmp4:
	movq	%rax, -168(%rbp)        # 8-byte Spill
	jmp	.LBB1_2
.LBB1_2:
.Ltmp5:
	movl	$_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_, %esi
	movq	-168(%rbp), %rdi        # 8-byte Reload
	callq	_ZNSolsEPFRSoS_E
.Ltmp6:
	jmp	.LBB1_3
.LBB1_3:
	leaq	-48(%rbp), %rax
	movq	%rax, %rdi
	movq	%rax, -176(%rbp)        # 8-byte Spill
	callq	_ZNKSt6vectorIiSaIiEE4sizeEv
	addq	%rax, %rax
.Ltmp7:
	movq	-176(%rbp), %rdi        # 8-byte Reload
	movq	%rax, %rsi
	callq	_ZNSt6vectorIiSaIiEE6resizeEm
.Ltmp8:
	jmp	.LBB1_4
.LBB1_4:
	leaq	-48(%rbp), %rax
	movq	%rax, -128(%rbp)
	movq	-128(%rbp), %rdi
	callq	_ZNSt6vectorIiSaIiEE5beginEv
	movq	%rax, -136(%rbp)
	movq	-128(%rbp), %rdi
	callq	_ZNSt6vectorIiSaIiEE3endEv
	movq	%rax, -144(%rbp)
.LBB1_5:                                # =>This Inner Loop Header: Depth=1
	leaq	-136(%rbp), %rdi
	leaq	-144(%rbp), %rsi
	callq	_ZN9__gnu_cxxneIPiSt6vectorIiSaIiEEEEbRKNS_17__normal_iteratorIT_T0_EESA_
	testb	$1, %al
	jne	.LBB1_6
	jmp	.LBB1_12
.LBB1_6:                                #   in Loop: Header=BB1_5 Depth=1
	leaq	-136(%rbp), %rdi
	callq	_ZNK9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEEdeEv
	movq	%rax, -152(%rbp)
	movq	-152(%rbp), %rax
	movl	(%rax), %esi
.Ltmp11:
	movl	$_ZSt4cout, %edi
	callq	_ZNSolsEi
.Ltmp12:
	movq	%rax, -184(%rbp)        # 8-byte Spill
	jmp	.LBB1_7
.LBB1_7:                                #   in Loop: Header=BB1_5 Depth=1
.Ltmp13:
	movl	$.L.str, %esi
	movq	-184(%rbp), %rdi        # 8-byte Reload
	callq	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
.Ltmp14:
	jmp	.LBB1_8
.LBB1_8:                                #   in Loop: Header=BB1_5 Depth=1
	jmp	.LBB1_9
.LBB1_9:                                #   in Loop: Header=BB1_5 Depth=1
	leaq	-136(%rbp), %rdi
	callq	_ZN9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEEppEv
	jmp	.LBB1_5
.LBB1_10:
.Ltmp2:
                                        # kill: def $edx killed $edx killed $rdx
	movq	%rax, -112(%rbp)
	movl	%edx, -116(%rbp)
	leaq	-104(%rbp), %rdi
	callq	_ZNSaIiED2Ev
	jmp	.LBB1_14
.LBB1_11:
.Ltmp15:
                                        # kill: def $edx killed $edx killed $rdx
	movq	%rax, -112(%rbp)
	movl	%edx, -116(%rbp)
	leaq	-48(%rbp), %rdi
	callq	_ZNSt6vectorIiSaIiEED2Ev
	jmp	.LBB1_14
.LBB1_12:
.Ltmp9:
	movl	$_ZSt4cout, %edi
	movl	$_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_, %esi
	callq	_ZNSolsEPFRSoS_E
.Ltmp10:
	jmp	.LBB1_13
.LBB1_13:
	movl	$0, -4(%rbp)
	leaq	-48(%rbp), %rdi
	callq	_ZNSt6vectorIiSaIiEED2Ev
	movl	-4(%rbp), %eax
	addq	$192, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.LBB1_14:
	.cfi_def_cfa %rbp, 16
	movq	-112(%rbp), %rdi
	callq	_Unwind_Resume
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table1:
.Lexception0:
	.byte	255                     # @LPStart Encoding = omit
	.byte	255                     # @TType Encoding = omit
	.byte	1                       # Call site Encoding = uleb128
	.uleb128 .Lcst_end0-.Lcst_begin0
.Lcst_begin0:
	.uleb128 .Ltmp0-.Lfunc_begin0   # >> Call Site 1 <<
	.uleb128 .Ltmp1-.Ltmp0          #   Call between .Ltmp0 and .Ltmp1
	.uleb128 .Ltmp2-.Lfunc_begin0   #     jumps to .Ltmp2
	.byte	0                       #   On action: cleanup
	.uleb128 .Ltmp3-.Lfunc_begin0   # >> Call Site 2 <<
	.uleb128 .Ltmp10-.Ltmp3         #   Call between .Ltmp3 and .Ltmp10
	.uleb128 .Ltmp15-.Lfunc_begin0  #     jumps to .Ltmp15
	.byte	0                       #   On action: cleanup
	.uleb128 .Ltmp10-.Lfunc_begin0  # >> Call Site 3 <<
	.uleb128 .Lfunc_end1-.Ltmp10    #   Call between .Ltmp10 and .Lfunc_end1
	.byte	0                       #     has no landing pad
	.byte	0                       #   On action: cleanup
.Lcst_end0:
	.p2align	2
                                        # -- End function
	.section	.text._ZNSaIiEC2Ev,"axG",@progbits,_ZNSaIiEC2Ev,comdat
	.weak	_ZNSaIiEC2Ev            # -- Begin function _ZNSaIiEC2Ev
	.p2align	4, 0x90
	.type	_ZNSaIiEC2Ev,@function
_ZNSaIiEC2Ev:                           # @_ZNSaIiEC2Ev
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	callq	_ZN9__gnu_cxx13new_allocatorIiEC2Ev
	addq	$16, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end2:
	.size	_ZNSaIiEC2Ev, .Lfunc_end2-_ZNSaIiEC2Ev
	.cfi_endproc
                                        # -- End function
	.section	.text._ZNSt6vectorIiSaIiEEC2ESt16initializer_listIiERKS0_,"axG",@progbits,_ZNSt6vectorIiSaIiEEC2ESt16initializer_listIiERKS0_,comdat
	.weak	_ZNSt6vectorIiSaIiEEC2ESt16initializer_listIiERKS0_ # -- Begin function _ZNSt6vectorIiSaIiEEC2ESt16initializer_listIiERKS0_
	.p2align	4, 0x90
	.type	_ZNSt6vectorIiSaIiEEC2ESt16initializer_listIiERKS0_,@function
_ZNSt6vectorIiSaIiEEC2ESt16initializer_listIiERKS0_: # @_ZNSt6vectorIiSaIiEEC2ESt16initializer_listIiERKS0_
.Lfunc_begin1:
	.cfi_startproc
	.cfi_personality 3, __gxx_personality_v0
	.cfi_lsda 3, .Lexception1
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$96, %rsp
	movq	%rsi, -16(%rbp)
	movq	%rdx, -8(%rbp)
	movq	%rdi, -24(%rbp)
	movq	%rcx, -32(%rbp)
	movq	-24(%rbp), %rax
	movq	-32(%rbp), %rsi
	movq	%rax, %rdi
	movq	%rax, -72(%rbp)         # 8-byte Spill
	callq	_ZNSt12_Vector_baseIiSaIiEEC2ERKS0_
	leaq	-16(%rbp), %rax
	movq	%rax, %rdi
	movq	%rax, -80(%rbp)         # 8-byte Spill
	callq	_ZNKSt16initializer_listIiE5beginEv
	movq	-80(%rbp), %rdi         # 8-byte Reload
	movq	%rax, -88(%rbp)         # 8-byte Spill
	callq	_ZNKSt16initializer_listIiE3endEv
.Ltmp16:
	movq	-72(%rbp), %rdi         # 8-byte Reload
	movq	-88(%rbp), %rsi         # 8-byte Reload
	movq	%rax, %rdx
	callq	_ZNSt6vectorIiSaIiEE19_M_range_initializeIPKiEEvT_S5_St20forward_iterator_tag
.Ltmp17:
	jmp	.LBB3_1
.LBB3_1:
	addq	$96, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.LBB3_2:
	.cfi_def_cfa %rbp, 16
.Ltmp18:
                                        # kill: def $edx killed $edx killed $rdx
	movq	%rax, -56(%rbp)
	movl	%edx, -60(%rbp)
	movq	-72(%rbp), %rax         # 8-byte Reload
	movq	%rax, %rdi
	callq	_ZNSt12_Vector_baseIiSaIiEED2Ev
# %bb.3:
	movq	-56(%rbp), %rdi
	callq	_Unwind_Resume
.Lfunc_end3:
	.size	_ZNSt6vectorIiSaIiEEC2ESt16initializer_listIiERKS0_, .Lfunc_end3-_ZNSt6vectorIiSaIiEEC2ESt16initializer_listIiERKS0_
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table3:
.Lexception1:
	.byte	255                     # @LPStart Encoding = omit
	.byte	255                     # @TType Encoding = omit
	.byte	1                       # Call site Encoding = uleb128
	.uleb128 .Lcst_end1-.Lcst_begin1
.Lcst_begin1:
	.uleb128 .Ltmp16-.Lfunc_begin1  # >> Call Site 1 <<
	.uleb128 .Ltmp17-.Ltmp16        #   Call between .Ltmp16 and .Ltmp17
	.uleb128 .Ltmp18-.Lfunc_begin1  #     jumps to .Ltmp18
	.byte	0                       #   On action: cleanup
	.uleb128 .Ltmp17-.Lfunc_begin1  # >> Call Site 2 <<
	.uleb128 .Lfunc_end3-.Ltmp17    #   Call between .Ltmp17 and .Lfunc_end3
	.byte	0                       #     has no landing pad
	.byte	0                       #   On action: cleanup
.Lcst_end1:
	.p2align	2
                                        # -- End function
	.section	.text._ZNSaIiED2Ev,"axG",@progbits,_ZNSaIiED2Ev,comdat
	.weak	_ZNSaIiED2Ev            # -- Begin function _ZNSaIiED2Ev
	.p2align	4, 0x90
	.type	_ZNSaIiED2Ev,@function
_ZNSaIiED2Ev:                           # @_ZNSaIiED2Ev
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	callq	_ZN9__gnu_cxx13new_allocatorIiED2Ev
	addq	$16, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end4:
	.size	_ZNSaIiED2Ev, .Lfunc_end4-_ZNSaIiED2Ev
	.cfi_endproc
                                        # -- End function
	.section	.text._ZNKSt6vectorIiSaIiEE4sizeEv,"axG",@progbits,_ZNKSt6vectorIiSaIiEE4sizeEv,comdat
	.weak	_ZNKSt6vectorIiSaIiEE4sizeEv # -- Begin function _ZNKSt6vectorIiSaIiEE4sizeEv
	.p2align	4, 0x90
	.type	_ZNKSt6vectorIiSaIiEE4sizeEv,@function
_ZNKSt6vectorIiSaIiEE4sizeEv:           # @_ZNKSt6vectorIiSaIiEE4sizeEv
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	8(%rax), %rcx
	movq	(%rax), %rax
	subq	%rax, %rcx
	sarq	$2, %rcx
	movq	%rcx, %rax
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end5:
	.size	_ZNKSt6vectorIiSaIiEE4sizeEv, .Lfunc_end5-_ZNKSt6vectorIiSaIiEE4sizeEv
	.cfi_endproc
                                        # -- End function
	.section	.text._ZNSt6vectorIiSaIiEE6resizeEm,"axG",@progbits,_ZNSt6vectorIiSaIiEE6resizeEm,comdat
	.weak	_ZNSt6vectorIiSaIiEE6resizeEm # -- Begin function _ZNSt6vectorIiSaIiEE6resizeEm
	.p2align	4, 0x90
	.type	_ZNSt6vectorIiSaIiEE6resizeEm,@function
_ZNSt6vectorIiSaIiEE6resizeEm:          # @_ZNSt6vectorIiSaIiEE6resizeEm
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$48, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rax
	movq	-16(%rbp), %rcx
	movq	%rax, %rdi
	movq	%rax, -24(%rbp)         # 8-byte Spill
	movq	%rcx, -32(%rbp)         # 8-byte Spill
	callq	_ZNKSt6vectorIiSaIiEE4sizeEv
	movq	-32(%rbp), %rcx         # 8-byte Reload
	cmpq	%rax, %rcx
	jbe	.LBB6_2
# %bb.1:
	movq	-16(%rbp), %rax
	movq	-24(%rbp), %rdi         # 8-byte Reload
	movq	%rax, -40(%rbp)         # 8-byte Spill
	callq	_ZNKSt6vectorIiSaIiEE4sizeEv
	movq	-40(%rbp), %rcx         # 8-byte Reload
	subq	%rax, %rcx
	movq	-24(%rbp), %rdi         # 8-byte Reload
	movq	%rcx, %rsi
	callq	_ZNSt6vectorIiSaIiEE17_M_default_appendEm
	jmp	.LBB6_5
.LBB6_2:
	movq	-16(%rbp), %rax
	movq	-24(%rbp), %rdi         # 8-byte Reload
	movq	%rax, -48(%rbp)         # 8-byte Spill
	callq	_ZNKSt6vectorIiSaIiEE4sizeEv
	movq	-48(%rbp), %rcx         # 8-byte Reload
	cmpq	%rax, %rcx
	jae	.LBB6_4
# %bb.3:
	movq	-24(%rbp), %rax         # 8-byte Reload
	movq	(%rax), %rcx
	movq	-16(%rbp), %rdx
	shlq	$2, %rdx
	addq	%rdx, %rcx
	movq	%rax, %rdi
	movq	%rcx, %rsi
	callq	_ZNSt6vectorIiSaIiEE15_M_erase_at_endEPi
.LBB6_4:
	jmp	.LBB6_5
.LBB6_5:
	addq	$48, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end6:
	.size	_ZNSt6vectorIiSaIiEE6resizeEm, .Lfunc_end6-_ZNSt6vectorIiSaIiEE6resizeEm
	.cfi_endproc
                                        # -- End function
	.section	.text._ZNSt6vectorIiSaIiEE5beginEv,"axG",@progbits,_ZNSt6vectorIiSaIiEE5beginEv,comdat
	.weak	_ZNSt6vectorIiSaIiEE5beginEv # -- Begin function _ZNSt6vectorIiSaIiEE5beginEv
	.p2align	4, 0x90
	.type	_ZNSt6vectorIiSaIiEE5beginEv,@function
_ZNSt6vectorIiSaIiEE5beginEv:           # @_ZNSt6vectorIiSaIiEE5beginEv
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movq	%rdi, -16(%rbp)
	movq	-16(%rbp), %rax
	leaq	-8(%rbp), %rdi
	movq	%rax, %rsi
	callq	_ZN9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEEC2ERKS1_
	movq	-8(%rbp), %rax
	addq	$16, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end7:
	.size	_ZNSt6vectorIiSaIiEE5beginEv, .Lfunc_end7-_ZNSt6vectorIiSaIiEE5beginEv
	.cfi_endproc
                                        # -- End function
	.section	.text._ZNSt6vectorIiSaIiEE3endEv,"axG",@progbits,_ZNSt6vectorIiSaIiEE3endEv,comdat
	.weak	_ZNSt6vectorIiSaIiEE3endEv # -- Begin function _ZNSt6vectorIiSaIiEE3endEv
	.p2align	4, 0x90
	.type	_ZNSt6vectorIiSaIiEE3endEv,@function
_ZNSt6vectorIiSaIiEE3endEv:             # @_ZNSt6vectorIiSaIiEE3endEv
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movq	%rdi, -16(%rbp)
	movq	-16(%rbp), %rax
	addq	$8, %rax
	leaq	-8(%rbp), %rdi
	movq	%rax, %rsi
	callq	_ZN9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEEC2ERKS1_
	movq	-8(%rbp), %rax
	addq	$16, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end8:
	.size	_ZNSt6vectorIiSaIiEE3endEv, .Lfunc_end8-_ZNSt6vectorIiSaIiEE3endEv
	.cfi_endproc
                                        # -- End function
	.section	.text._ZN9__gnu_cxxneIPiSt6vectorIiSaIiEEEEbRKNS_17__normal_iteratorIT_T0_EESA_,"axG",@progbits,_ZN9__gnu_cxxneIPiSt6vectorIiSaIiEEEEbRKNS_17__normal_iteratorIT_T0_EESA_,comdat
	.weak	_ZN9__gnu_cxxneIPiSt6vectorIiSaIiEEEEbRKNS_17__normal_iteratorIT_T0_EESA_ # -- Begin function _ZN9__gnu_cxxneIPiSt6vectorIiSaIiEEEEbRKNS_17__normal_iteratorIT_T0_EESA_
	.p2align	4, 0x90
	.type	_ZN9__gnu_cxxneIPiSt6vectorIiSaIiEEEEbRKNS_17__normal_iteratorIT_T0_EESA_,@function
_ZN9__gnu_cxxneIPiSt6vectorIiSaIiEEEEbRKNS_17__normal_iteratorIT_T0_EESA_: # @_ZN9__gnu_cxxneIPiSt6vectorIiSaIiEEEEbRKNS_17__normal_iteratorIT_T0_EESA_
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rdi
	callq	_ZNK9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEE4baseEv
	movq	(%rax), %rax
	movq	-16(%rbp), %rdi
	movq	%rax, -24(%rbp)         # 8-byte Spill
	callq	_ZNK9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEE4baseEv
	movq	-24(%rbp), %rcx         # 8-byte Reload
	cmpq	(%rax), %rcx
	setne	%dl
	andb	$1, %dl
	movzbl	%dl, %eax
	addq	$32, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end9:
	.size	_ZN9__gnu_cxxneIPiSt6vectorIiSaIiEEEEbRKNS_17__normal_iteratorIT_T0_EESA_, .Lfunc_end9-_ZN9__gnu_cxxneIPiSt6vectorIiSaIiEEEEbRKNS_17__normal_iteratorIT_T0_EESA_
	.cfi_endproc
                                        # -- End function
	.section	.text._ZNK9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEEdeEv,"axG",@progbits,_ZNK9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEEdeEv,comdat
	.weak	_ZNK9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEEdeEv # -- Begin function _ZNK9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEEdeEv
	.p2align	4, 0x90
	.type	_ZNK9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEEdeEv,@function
_ZNK9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEEdeEv: # @_ZNK9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEEdeEv
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end10:
	.size	_ZNK9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEEdeEv, .Lfunc_end10-_ZNK9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEEdeEv
	.cfi_endproc
                                        # -- End function
	.section	.text._ZN9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEEppEv,"axG",@progbits,_ZN9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEEppEv,comdat
	.weak	_ZN9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEEppEv # -- Begin function _ZN9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEEppEv
	.p2align	4, 0x90
	.type	_ZN9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEEppEv,@function
_ZN9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEEppEv: # @_ZN9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEEppEv
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	(%rax), %rcx
	addq	$4, %rcx
	movq	%rcx, (%rax)
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end11:
	.size	_ZN9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEEppEv, .Lfunc_end11-_ZN9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEEppEv
	.cfi_endproc
                                        # -- End function
	.section	.text._ZNSt6vectorIiSaIiEED2Ev,"axG",@progbits,_ZNSt6vectorIiSaIiEED2Ev,comdat
	.weak	_ZNSt6vectorIiSaIiEED2Ev # -- Begin function _ZNSt6vectorIiSaIiEED2Ev
	.p2align	4, 0x90
	.type	_ZNSt6vectorIiSaIiEED2Ev,@function
_ZNSt6vectorIiSaIiEED2Ev:               # @_ZNSt6vectorIiSaIiEED2Ev
.Lfunc_begin2:
	.cfi_startproc
	.cfi_personality 3, __gxx_personality_v0
	.cfi_lsda 3, .Lexception2
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$48, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	(%rax), %rdi
	movq	8(%rax), %rsi
	movq	%rdi, -32(%rbp)         # 8-byte Spill
	movq	%rax, %rdi
	movq	%rax, -40(%rbp)         # 8-byte Spill
	movq	%rsi, -48(%rbp)         # 8-byte Spill
	callq	_ZNSt12_Vector_baseIiSaIiEE19_M_get_Tp_allocatorEv
.Ltmp19:
	movq	-32(%rbp), %rdi         # 8-byte Reload
	movq	-48(%rbp), %rsi         # 8-byte Reload
	movq	%rax, %rdx
	callq	_ZSt8_DestroyIPiiEvT_S1_RSaIT0_E
.Ltmp20:
	jmp	.LBB12_1
.LBB12_1:
	movq	-40(%rbp), %rax         # 8-byte Reload
	movq	%rax, %rdi
	callq	_ZNSt12_Vector_baseIiSaIiEED2Ev
	addq	$48, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.LBB12_2:
	.cfi_def_cfa %rbp, 16
.Ltmp21:
                                        # kill: def $edx killed $edx killed $rdx
	movq	%rax, -16(%rbp)
	movl	%edx, -20(%rbp)
	movq	-40(%rbp), %rax         # 8-byte Reload
	movq	%rax, %rdi
	callq	_ZNSt12_Vector_baseIiSaIiEED2Ev
# %bb.3:
	movq	-16(%rbp), %rdi
	callq	__clang_call_terminate
.Lfunc_end12:
	.size	_ZNSt6vectorIiSaIiEED2Ev, .Lfunc_end12-_ZNSt6vectorIiSaIiEED2Ev
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table12:
.Lexception2:
	.byte	255                     # @LPStart Encoding = omit
	.byte	3                       # @TType Encoding = udata4
	.uleb128 .Lttbase0-.Lttbaseref0
.Lttbaseref0:
	.byte	1                       # Call site Encoding = uleb128
	.uleb128 .Lcst_end2-.Lcst_begin2
.Lcst_begin2:
	.uleb128 .Ltmp19-.Lfunc_begin2  # >> Call Site 1 <<
	.uleb128 .Ltmp20-.Ltmp19        #   Call between .Ltmp19 and .Ltmp20
	.uleb128 .Ltmp21-.Lfunc_begin2  #     jumps to .Ltmp21
	.byte	1                       #   On action: 1
.Lcst_end2:
	.byte	1                       # >> Action Record 1 <<
                                        #   Catch TypeInfo 1
	.byte	0                       #   No further actions
	.p2align	2
                                        # >> Catch TypeInfos <<
	.long	0                       # TypeInfo 1
.Lttbase0:
	.p2align	2
                                        # -- End function
	.section	.text._ZN9__gnu_cxx13new_allocatorIiEC2Ev,"axG",@progbits,_ZN9__gnu_cxx13new_allocatorIiEC2Ev,comdat
	.weak	_ZN9__gnu_cxx13new_allocatorIiEC2Ev # -- Begin function _ZN9__gnu_cxx13new_allocatorIiEC2Ev
	.p2align	4, 0x90
	.type	_ZN9__gnu_cxx13new_allocatorIiEC2Ev,@function
_ZN9__gnu_cxx13new_allocatorIiEC2Ev:    # @_ZN9__gnu_cxx13new_allocatorIiEC2Ev
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -8(%rbp)
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end13:
	.size	_ZN9__gnu_cxx13new_allocatorIiEC2Ev, .Lfunc_end13-_ZN9__gnu_cxx13new_allocatorIiEC2Ev
	.cfi_endproc
                                        # -- End function
	.section	.text._ZN9__gnu_cxx13new_allocatorIiED2Ev,"axG",@progbits,_ZN9__gnu_cxx13new_allocatorIiED2Ev,comdat
	.weak	_ZN9__gnu_cxx13new_allocatorIiED2Ev # -- Begin function _ZN9__gnu_cxx13new_allocatorIiED2Ev
	.p2align	4, 0x90
	.type	_ZN9__gnu_cxx13new_allocatorIiED2Ev,@function
_ZN9__gnu_cxx13new_allocatorIiED2Ev:    # @_ZN9__gnu_cxx13new_allocatorIiED2Ev
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -8(%rbp)
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end14:
	.size	_ZN9__gnu_cxx13new_allocatorIiED2Ev, .Lfunc_end14-_ZN9__gnu_cxx13new_allocatorIiED2Ev
	.cfi_endproc
                                        # -- End function
	.section	.text._ZNSt12_Vector_baseIiSaIiEEC2ERKS0_,"axG",@progbits,_ZNSt12_Vector_baseIiSaIiEEC2ERKS0_,comdat
	.weak	_ZNSt12_Vector_baseIiSaIiEEC2ERKS0_ # -- Begin function _ZNSt12_Vector_baseIiSaIiEEC2ERKS0_
	.p2align	4, 0x90
	.type	_ZNSt12_Vector_baseIiSaIiEEC2ERKS0_,@function
_ZNSt12_Vector_baseIiSaIiEEC2ERKS0_:    # @_ZNSt12_Vector_baseIiSaIiEEC2ERKS0_
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rdi
	movq	-16(%rbp), %rsi
	callq	_ZNSt12_Vector_baseIiSaIiEE12_Vector_implC2ERKS0_
	addq	$16, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end15:
	.size	_ZNSt12_Vector_baseIiSaIiEEC2ERKS0_, .Lfunc_end15-_ZNSt12_Vector_baseIiSaIiEEC2ERKS0_
	.cfi_endproc
                                        # -- End function
	.section	.text._ZNSt6vectorIiSaIiEE19_M_range_initializeIPKiEEvT_S5_St20forward_iterator_tag,"axG",@progbits,_ZNSt6vectorIiSaIiEE19_M_range_initializeIPKiEEvT_S5_St20forward_iterator_tag,comdat
	.weak	_ZNSt6vectorIiSaIiEE19_M_range_initializeIPKiEEvT_S5_St20forward_iterator_tag # -- Begin function _ZNSt6vectorIiSaIiEE19_M_range_initializeIPKiEEvT_S5_St20forward_iterator_tag
	.p2align	4, 0x90
	.type	_ZNSt6vectorIiSaIiEE19_M_range_initializeIPKiEEvT_S5_St20forward_iterator_tag,@function
_ZNSt6vectorIiSaIiEE19_M_range_initializeIPKiEEvT_S5_St20forward_iterator_tag: # @_ZNSt6vectorIiSaIiEE19_M_range_initializeIPKiEEvT_S5_St20forward_iterator_tag
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$96, %rsp
	movq	%rdi, -16(%rbp)
	movq	%rsi, -24(%rbp)
	movq	%rdx, -32(%rbp)
	movq	-16(%rbp), %rax
	movq	-24(%rbp), %rdi
	movq	-32(%rbp), %rsi
	movq	%rax, -48(%rbp)         # 8-byte Spill
	callq	_ZSt8distanceIPKiENSt15iterator_traitsIT_E15difference_typeES3_S3_
	movq	%rax, -40(%rbp)
	movq	-48(%rbp), %rax         # 8-byte Reload
	movq	-40(%rbp), %rdi
	movq	-48(%rbp), %rcx         # 8-byte Reload
	movq	%rdi, -56(%rbp)         # 8-byte Spill
	movq	%rcx, %rdi
	movq	%rax, -64(%rbp)         # 8-byte Spill
	callq	_ZNSt12_Vector_baseIiSaIiEE19_M_get_Tp_allocatorEv
	movq	-56(%rbp), %rdi         # 8-byte Reload
	movq	%rax, %rsi
	callq	_ZNSt6vectorIiSaIiEE17_S_check_init_lenEmRKS0_
	movq	-64(%rbp), %rdi         # 8-byte Reload
	movq	%rax, %rsi
	callq	_ZNSt12_Vector_baseIiSaIiEE11_M_allocateEm
	movq	-48(%rbp), %rcx         # 8-byte Reload
	movq	%rax, (%rcx)
	movq	(%rcx), %rax
	movq	-40(%rbp), %rdx
	shlq	$2, %rdx
	addq	%rdx, %rax
	movq	%rax, 16(%rcx)
	movq	-24(%rbp), %rdi
	movq	-32(%rbp), %rsi
	movq	(%rcx), %rdx
	movq	%rdi, -72(%rbp)         # 8-byte Spill
	movq	%rcx, %rdi
	movq	%rsi, -80(%rbp)         # 8-byte Spill
	movq	%rdx, -88(%rbp)         # 8-byte Spill
	callq	_ZNSt12_Vector_baseIiSaIiEE19_M_get_Tp_allocatorEv
	movq	-72(%rbp), %rdi         # 8-byte Reload
	movq	-80(%rbp), %rsi         # 8-byte Reload
	movq	-88(%rbp), %rdx         # 8-byte Reload
	movq	%rax, %rcx
	callq	_ZSt22__uninitialized_copy_aIPKiPiiET0_T_S4_S3_RSaIT1_E
	movq	-48(%rbp), %rcx         # 8-byte Reload
	movq	%rax, 8(%rcx)
	addq	$96, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end16:
	.size	_ZNSt6vectorIiSaIiEE19_M_range_initializeIPKiEEvT_S5_St20forward_iterator_tag, .Lfunc_end16-_ZNSt6vectorIiSaIiEE19_M_range_initializeIPKiEEvT_S5_St20forward_iterator_tag
	.cfi_endproc
                                        # -- End function
	.section	.text._ZNKSt16initializer_listIiE5beginEv,"axG",@progbits,_ZNKSt16initializer_listIiE5beginEv,comdat
	.weak	_ZNKSt16initializer_listIiE5beginEv # -- Begin function _ZNKSt16initializer_listIiE5beginEv
	.p2align	4, 0x90
	.type	_ZNKSt16initializer_listIiE5beginEv,@function
_ZNKSt16initializer_listIiE5beginEv:    # @_ZNKSt16initializer_listIiE5beginEv
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end17:
	.size	_ZNKSt16initializer_listIiE5beginEv, .Lfunc_end17-_ZNKSt16initializer_listIiE5beginEv
	.cfi_endproc
                                        # -- End function
	.section	.text._ZNKSt16initializer_listIiE3endEv,"axG",@progbits,_ZNKSt16initializer_listIiE3endEv,comdat
	.weak	_ZNKSt16initializer_listIiE3endEv # -- Begin function _ZNKSt16initializer_listIiE3endEv
	.p2align	4, 0x90
	.type	_ZNKSt16initializer_listIiE3endEv,@function
_ZNKSt16initializer_listIiE3endEv:      # @_ZNKSt16initializer_listIiE3endEv
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	movq	%rax, -16(%rbp)         # 8-byte Spill
	callq	_ZNKSt16initializer_listIiE5beginEv
	movq	-16(%rbp), %rdi         # 8-byte Reload
	movq	%rax, -24(%rbp)         # 8-byte Spill
	callq	_ZNKSt16initializer_listIiE4sizeEv
	shlq	$2, %rax
	movq	-24(%rbp), %rcx         # 8-byte Reload
	addq	%rax, %rcx
	movq	%rcx, %rax
	addq	$32, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end18:
	.size	_ZNKSt16initializer_listIiE3endEv, .Lfunc_end18-_ZNKSt16initializer_listIiE3endEv
	.cfi_endproc
                                        # -- End function
	.section	.text._ZNSt12_Vector_baseIiSaIiEED2Ev,"axG",@progbits,_ZNSt12_Vector_baseIiSaIiEED2Ev,comdat
	.weak	_ZNSt12_Vector_baseIiSaIiEED2Ev # -- Begin function _ZNSt12_Vector_baseIiSaIiEED2Ev
	.p2align	4, 0x90
	.type	_ZNSt12_Vector_baseIiSaIiEED2Ev,@function
_ZNSt12_Vector_baseIiSaIiEED2Ev:        # @_ZNSt12_Vector_baseIiSaIiEED2Ev
.Lfunc_begin3:
	.cfi_startproc
	.cfi_personality 3, __gxx_personality_v0
	.cfi_lsda 3, .Lexception3
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	(%rax), %rcx
	movq	16(%rax), %rdx
	subq	%rcx, %rdx
	sarq	$2, %rdx
.Ltmp22:
	movq	%rax, %rdi
	movq	%rcx, %rsi
	movq	%rax, -32(%rbp)         # 8-byte Spill
	callq	_ZNSt12_Vector_baseIiSaIiEE13_M_deallocateEPim
.Ltmp23:
	jmp	.LBB19_1
.LBB19_1:
	movq	-32(%rbp), %rdi         # 8-byte Reload
	callq	_ZNSt12_Vector_baseIiSaIiEE12_Vector_implD2Ev
	addq	$32, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.LBB19_2:
	.cfi_def_cfa %rbp, 16
.Ltmp24:
                                        # kill: def $edx killed $edx killed $rdx
	movq	%rax, -16(%rbp)
	movl	%edx, -20(%rbp)
	movq	-32(%rbp), %rdi         # 8-byte Reload
	callq	_ZNSt12_Vector_baseIiSaIiEE12_Vector_implD2Ev
# %bb.3:
	movq	-16(%rbp), %rdi
	callq	__clang_call_terminate
.Lfunc_end19:
	.size	_ZNSt12_Vector_baseIiSaIiEED2Ev, .Lfunc_end19-_ZNSt12_Vector_baseIiSaIiEED2Ev
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table19:
.Lexception3:
	.byte	255                     # @LPStart Encoding = omit
	.byte	3                       # @TType Encoding = udata4
	.uleb128 .Lttbase1-.Lttbaseref1
.Lttbaseref1:
	.byte	1                       # Call site Encoding = uleb128
	.uleb128 .Lcst_end3-.Lcst_begin3
.Lcst_begin3:
	.uleb128 .Ltmp22-.Lfunc_begin3  # >> Call Site 1 <<
	.uleb128 .Ltmp23-.Ltmp22        #   Call between .Ltmp22 and .Ltmp23
	.uleb128 .Ltmp24-.Lfunc_begin3  #     jumps to .Ltmp24
	.byte	1                       #   On action: 1
.Lcst_end3:
	.byte	1                       # >> Action Record 1 <<
                                        #   Catch TypeInfo 1
	.byte	0                       #   No further actions
	.p2align	2
                                        # >> Catch TypeInfos <<
	.long	0                       # TypeInfo 1
.Lttbase1:
	.p2align	2
                                        # -- End function
	.section	.text._ZNSt12_Vector_baseIiSaIiEE12_Vector_implC2ERKS0_,"axG",@progbits,_ZNSt12_Vector_baseIiSaIiEE12_Vector_implC2ERKS0_,comdat
	.weak	_ZNSt12_Vector_baseIiSaIiEE12_Vector_implC2ERKS0_ # -- Begin function _ZNSt12_Vector_baseIiSaIiEE12_Vector_implC2ERKS0_
	.p2align	4, 0x90
	.type	_ZNSt12_Vector_baseIiSaIiEE12_Vector_implC2ERKS0_,@function
_ZNSt12_Vector_baseIiSaIiEE12_Vector_implC2ERKS0_: # @_ZNSt12_Vector_baseIiSaIiEE12_Vector_implC2ERKS0_
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, %rcx
	movq	-16(%rbp), %rsi
	movq	%rcx, %rdi
	movq	%rax, -24(%rbp)         # 8-byte Spill
	callq	_ZNSaIiEC2ERKS_
	movq	-24(%rbp), %rax         # 8-byte Reload
	movq	%rax, %rdi
	callq	_ZNSt12_Vector_baseIiSaIiEE17_Vector_impl_dataC2Ev
	addq	$32, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end20:
	.size	_ZNSt12_Vector_baseIiSaIiEE12_Vector_implC2ERKS0_, .Lfunc_end20-_ZNSt12_Vector_baseIiSaIiEE12_Vector_implC2ERKS0_
	.cfi_endproc
                                        # -- End function
	.section	.text._ZNSt12_Vector_baseIiSaIiEE17_Vector_impl_dataC2Ev,"axG",@progbits,_ZNSt12_Vector_baseIiSaIiEE17_Vector_impl_dataC2Ev,comdat
	.weak	_ZNSt12_Vector_baseIiSaIiEE17_Vector_impl_dataC2Ev # -- Begin function _ZNSt12_Vector_baseIiSaIiEE17_Vector_impl_dataC2Ev
	.p2align	4, 0x90
	.type	_ZNSt12_Vector_baseIiSaIiEE17_Vector_impl_dataC2Ev,@function
_ZNSt12_Vector_baseIiSaIiEE17_Vector_impl_dataC2Ev: # @_ZNSt12_Vector_baseIiSaIiEE17_Vector_impl_dataC2Ev
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	$0, (%rax)
	movq	$0, 8(%rax)
	movq	$0, 16(%rax)
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end21:
	.size	_ZNSt12_Vector_baseIiSaIiEE17_Vector_impl_dataC2Ev, .Lfunc_end21-_ZNSt12_Vector_baseIiSaIiEE17_Vector_impl_dataC2Ev
	.cfi_endproc
                                        # -- End function
	.section	.text._ZN9__gnu_cxx13new_allocatorIiEC2ERKS1_,"axG",@progbits,_ZN9__gnu_cxx13new_allocatorIiEC2ERKS1_,comdat
	.weak	_ZN9__gnu_cxx13new_allocatorIiEC2ERKS1_ # -- Begin function _ZN9__gnu_cxx13new_allocatorIiEC2ERKS1_
	.p2align	4, 0x90
	.type	_ZN9__gnu_cxx13new_allocatorIiEC2ERKS1_,@function
_ZN9__gnu_cxx13new_allocatorIiEC2ERKS1_: # @_ZN9__gnu_cxx13new_allocatorIiEC2ERKS1_
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end22:
	.size	_ZN9__gnu_cxx13new_allocatorIiEC2ERKS1_, .Lfunc_end22-_ZN9__gnu_cxx13new_allocatorIiEC2ERKS1_
	.cfi_endproc
                                        # -- End function
	.section	.text._ZSt8distanceIPKiENSt15iterator_traitsIT_E15difference_typeES3_S3_,"axG",@progbits,_ZSt8distanceIPKiENSt15iterator_traitsIT_E15difference_typeES3_S3_,comdat
	.weak	_ZSt8distanceIPKiENSt15iterator_traitsIT_E15difference_typeES3_S3_ # -- Begin function _ZSt8distanceIPKiENSt15iterator_traitsIT_E15difference_typeES3_S3_
	.p2align	4, 0x90
	.type	_ZSt8distanceIPKiENSt15iterator_traitsIT_E15difference_typeES3_S3_,@function
_ZSt8distanceIPKiENSt15iterator_traitsIT_E15difference_typeES3_S3_: # @_ZSt8distanceIPKiENSt15iterator_traitsIT_E15difference_typeES3_S3_
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$48, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rdi
	movq	-16(%rbp), %rsi
	leaq	-8(%rbp), %rax
	movq	%rdi, -40(%rbp)         # 8-byte Spill
	movq	%rax, %rdi
	movq	%rsi, -48(%rbp)         # 8-byte Spill
	callq	_ZSt19__iterator_categoryIPKiENSt15iterator_traitsIT_E17iterator_categoryERKS3_
	movq	-40(%rbp), %rdi         # 8-byte Reload
	movq	-48(%rbp), %rsi         # 8-byte Reload
	callq	_ZSt10__distanceIPKiENSt15iterator_traitsIT_E15difference_typeES3_S3_St26random_access_iterator_tag
	addq	$48, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end23:
	.size	_ZSt8distanceIPKiENSt15iterator_traitsIT_E15difference_typeES3_S3_, .Lfunc_end23-_ZSt8distanceIPKiENSt15iterator_traitsIT_E15difference_typeES3_S3_
	.cfi_endproc
                                        # -- End function
	.section	.text._ZNSt12_Vector_baseIiSaIiEE11_M_allocateEm,"axG",@progbits,_ZNSt12_Vector_baseIiSaIiEE11_M_allocateEm,comdat
	.weak	_ZNSt12_Vector_baseIiSaIiEE11_M_allocateEm # -- Begin function _ZNSt12_Vector_baseIiSaIiEE11_M_allocateEm
	.p2align	4, 0x90
	.type	_ZNSt12_Vector_baseIiSaIiEE11_M_allocateEm,@function
_ZNSt12_Vector_baseIiSaIiEE11_M_allocateEm: # @_ZNSt12_Vector_baseIiSaIiEE11_M_allocateEm
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rax
	cmpq	$0, -16(%rbp)
	movq	%rax, -24(%rbp)         # 8-byte Spill
	je	.LBB24_2
# %bb.1:
	movq	-24(%rbp), %rax         # 8-byte Reload
	movq	-16(%rbp), %rsi
	movq	%rax, %rdi
	callq	_ZNSt16allocator_traitsISaIiEE8allocateERS0_m
	movq	%rax, -32(%rbp)         # 8-byte Spill
	jmp	.LBB24_3
.LBB24_2:
	xorl	%eax, %eax
	movl	%eax, %ecx
	movq	%rcx, -32(%rbp)         # 8-byte Spill
	jmp	.LBB24_3
.LBB24_3:
	movq	-32(%rbp), %rax         # 8-byte Reload
	addq	$32, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end24:
	.size	_ZNSt12_Vector_baseIiSaIiEE11_M_allocateEm, .Lfunc_end24-_ZNSt12_Vector_baseIiSaIiEE11_M_allocateEm
	.cfi_endproc
                                        # -- End function
	.section	.text._ZNSt6vectorIiSaIiEE17_S_check_init_lenEmRKS0_,"axG",@progbits,_ZNSt6vectorIiSaIiEE17_S_check_init_lenEmRKS0_,comdat
	.weak	_ZNSt6vectorIiSaIiEE17_S_check_init_lenEmRKS0_ # -- Begin function _ZNSt6vectorIiSaIiEE17_S_check_init_lenEmRKS0_
	.p2align	4, 0x90
	.type	_ZNSt6vectorIiSaIiEE17_S_check_init_lenEmRKS0_,@function
_ZNSt6vectorIiSaIiEE17_S_check_init_lenEmRKS0_: # @_ZNSt6vectorIiSaIiEE17_S_check_init_lenEmRKS0_
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$48, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rax
	movq	-16(%rbp), %rsi
	leaq	-24(%rbp), %rdi
	movq	%rax, -32(%rbp)         # 8-byte Spill
	callq	_ZNSaIiEC2ERKS_
	leaq	-24(%rbp), %rdi
	callq	_ZNSt6vectorIiSaIiEE11_S_max_sizeERKS0_
	leaq	-24(%rbp), %rdi
	movq	%rax, -40(%rbp)         # 8-byte Spill
	callq	_ZNSaIiED2Ev
	movq	-32(%rbp), %rax         # 8-byte Reload
	movq	-40(%rbp), %rcx         # 8-byte Reload
	cmpq	%rcx, %rax
	jbe	.LBB25_2
# %bb.1:
	movabsq	$.L.str.1, %rdi
	callq	_ZSt20__throw_length_errorPKc
.LBB25_2:
	movq	-8(%rbp), %rax
	addq	$48, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end25:
	.size	_ZNSt6vectorIiSaIiEE17_S_check_init_lenEmRKS0_, .Lfunc_end25-_ZNSt6vectorIiSaIiEE17_S_check_init_lenEmRKS0_
	.cfi_endproc
                                        # -- End function
	.section	.text._ZNSt12_Vector_baseIiSaIiEE19_M_get_Tp_allocatorEv,"axG",@progbits,_ZNSt12_Vector_baseIiSaIiEE19_M_get_Tp_allocatorEv,comdat
	.weak	_ZNSt12_Vector_baseIiSaIiEE19_M_get_Tp_allocatorEv # -- Begin function _ZNSt12_Vector_baseIiSaIiEE19_M_get_Tp_allocatorEv
	.p2align	4, 0x90
	.type	_ZNSt12_Vector_baseIiSaIiEE19_M_get_Tp_allocatorEv,@function
_ZNSt12_Vector_baseIiSaIiEE19_M_get_Tp_allocatorEv: # @_ZNSt12_Vector_baseIiSaIiEE19_M_get_Tp_allocatorEv
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end26:
	.size	_ZNSt12_Vector_baseIiSaIiEE19_M_get_Tp_allocatorEv, .Lfunc_end26-_ZNSt12_Vector_baseIiSaIiEE19_M_get_Tp_allocatorEv
	.cfi_endproc
                                        # -- End function
	.section	.text._ZSt22__uninitialized_copy_aIPKiPiiET0_T_S4_S3_RSaIT1_E,"axG",@progbits,_ZSt22__uninitialized_copy_aIPKiPiiET0_T_S4_S3_RSaIT1_E,comdat
	.weak	_ZSt22__uninitialized_copy_aIPKiPiiET0_T_S4_S3_RSaIT1_E # -- Begin function _ZSt22__uninitialized_copy_aIPKiPiiET0_T_S4_S3_RSaIT1_E
	.p2align	4, 0x90
	.type	_ZSt22__uninitialized_copy_aIPKiPiiET0_T_S4_S3_RSaIT1_E,@function
_ZSt22__uninitialized_copy_aIPKiPiiET0_T_S4_S3_RSaIT1_E: # @_ZSt22__uninitialized_copy_aIPKiPiiET0_T_S4_S3_RSaIT1_E
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	%rcx, -32(%rbp)
	movq	-8(%rbp), %rdi
	movq	-16(%rbp), %rsi
	movq	-24(%rbp), %rdx
	callq	_ZSt18uninitialized_copyIPKiPiET0_T_S4_S3_
	addq	$32, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end27:
	.size	_ZSt22__uninitialized_copy_aIPKiPiiET0_T_S4_S3_RSaIT1_E, .Lfunc_end27-_ZSt22__uninitialized_copy_aIPKiPiiET0_T_S4_S3_RSaIT1_E
	.cfi_endproc
                                        # -- End function
	.section	.text._ZSt10__distanceIPKiENSt15iterator_traitsIT_E15difference_typeES3_S3_St26random_access_iterator_tag,"axG",@progbits,_ZSt10__distanceIPKiENSt15iterator_traitsIT_E15difference_typeES3_S3_St26random_access_iterator_tag,comdat
	.weak	_ZSt10__distanceIPKiENSt15iterator_traitsIT_E15difference_typeES3_S3_St26random_access_iterator_tag # -- Begin function _ZSt10__distanceIPKiENSt15iterator_traitsIT_E15difference_typeES3_S3_St26random_access_iterator_tag
	.p2align	4, 0x90
	.type	_ZSt10__distanceIPKiENSt15iterator_traitsIT_E15difference_typeES3_S3_St26random_access_iterator_tag,@function
_ZSt10__distanceIPKiENSt15iterator_traitsIT_E15difference_typeES3_S3_St26random_access_iterator_tag: # @_ZSt10__distanceIPKiENSt15iterator_traitsIT_E15difference_typeES3_S3_St26random_access_iterator_tag
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -16(%rbp)
	movq	%rsi, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	-16(%rbp), %rcx
	subq	%rcx, %rax
	sarq	$2, %rax
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end28:
	.size	_ZSt10__distanceIPKiENSt15iterator_traitsIT_E15difference_typeES3_S3_St26random_access_iterator_tag, .Lfunc_end28-_ZSt10__distanceIPKiENSt15iterator_traitsIT_E15difference_typeES3_S3_St26random_access_iterator_tag
	.cfi_endproc
                                        # -- End function
	.section	.text._ZSt19__iterator_categoryIPKiENSt15iterator_traitsIT_E17iterator_categoryERKS3_,"axG",@progbits,_ZSt19__iterator_categoryIPKiENSt15iterator_traitsIT_E17iterator_categoryERKS3_,comdat
	.weak	_ZSt19__iterator_categoryIPKiENSt15iterator_traitsIT_E17iterator_categoryERKS3_ # -- Begin function _ZSt19__iterator_categoryIPKiENSt15iterator_traitsIT_E17iterator_categoryERKS3_
	.p2align	4, 0x90
	.type	_ZSt19__iterator_categoryIPKiENSt15iterator_traitsIT_E17iterator_categoryERKS3_,@function
_ZSt19__iterator_categoryIPKiENSt15iterator_traitsIT_E17iterator_categoryERKS3_: # @_ZSt19__iterator_categoryIPKiENSt15iterator_traitsIT_E17iterator_categoryERKS3_
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -8(%rbp)
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end29:
	.size	_ZSt19__iterator_categoryIPKiENSt15iterator_traitsIT_E17iterator_categoryERKS3_, .Lfunc_end29-_ZSt19__iterator_categoryIPKiENSt15iterator_traitsIT_E17iterator_categoryERKS3_
	.cfi_endproc
                                        # -- End function
	.section	.text._ZNSt16allocator_traitsISaIiEE8allocateERS0_m,"axG",@progbits,_ZNSt16allocator_traitsISaIiEE8allocateERS0_m,comdat
	.weak	_ZNSt16allocator_traitsISaIiEE8allocateERS0_m # -- Begin function _ZNSt16allocator_traitsISaIiEE8allocateERS0_m
	.p2align	4, 0x90
	.type	_ZNSt16allocator_traitsISaIiEE8allocateERS0_m,@function
_ZNSt16allocator_traitsISaIiEE8allocateERS0_m: # @_ZNSt16allocator_traitsISaIiEE8allocateERS0_m
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	xorl	%eax, %eax
	movl	%eax, %edx
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rcx
	movq	-16(%rbp), %rsi
	movq	%rcx, %rdi
	callq	_ZN9__gnu_cxx13new_allocatorIiE8allocateEmPKv
	addq	$16, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end30:
	.size	_ZNSt16allocator_traitsISaIiEE8allocateERS0_m, .Lfunc_end30-_ZNSt16allocator_traitsISaIiEE8allocateERS0_m
	.cfi_endproc
                                        # -- End function
	.section	.text._ZN9__gnu_cxx13new_allocatorIiE8allocateEmPKv,"axG",@progbits,_ZN9__gnu_cxx13new_allocatorIiE8allocateEmPKv,comdat
	.weak	_ZN9__gnu_cxx13new_allocatorIiE8allocateEmPKv # -- Begin function _ZN9__gnu_cxx13new_allocatorIiE8allocateEmPKv
	.p2align	4, 0x90
	.type	_ZN9__gnu_cxx13new_allocatorIiE8allocateEmPKv,@function
_ZN9__gnu_cxx13new_allocatorIiE8allocateEmPKv: # @_ZN9__gnu_cxx13new_allocatorIiE8allocateEmPKv
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	-8(%rbp), %rdi
	movq	-16(%rbp), %rax
	movq	%rax, -32(%rbp)         # 8-byte Spill
	callq	_ZNK9__gnu_cxx13new_allocatorIiE8max_sizeEv
	movq	-32(%rbp), %rcx         # 8-byte Reload
	cmpq	%rax, %rcx
	jbe	.LBB31_2
# %bb.1:
	callq	_ZSt17__throw_bad_allocv
.LBB31_2:
	movq	-16(%rbp), %rax
	shlq	$2, %rax
	movq	%rax, %rdi
	callq	_Znwm
	addq	$32, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end31:
	.size	_ZN9__gnu_cxx13new_allocatorIiE8allocateEmPKv, .Lfunc_end31-_ZN9__gnu_cxx13new_allocatorIiE8allocateEmPKv
	.cfi_endproc
                                        # -- End function
	.section	.text._ZNK9__gnu_cxx13new_allocatorIiE8max_sizeEv,"axG",@progbits,_ZNK9__gnu_cxx13new_allocatorIiE8max_sizeEv,comdat
	.weak	_ZNK9__gnu_cxx13new_allocatorIiE8max_sizeEv # -- Begin function _ZNK9__gnu_cxx13new_allocatorIiE8max_sizeEv
	.p2align	4, 0x90
	.type	_ZNK9__gnu_cxx13new_allocatorIiE8max_sizeEv,@function
_ZNK9__gnu_cxx13new_allocatorIiE8max_sizeEv: # @_ZNK9__gnu_cxx13new_allocatorIiE8max_sizeEv
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -8(%rbp)
	movabsq	$2305843009213693951, %rax # imm = 0x1FFFFFFFFFFFFFFF
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end32:
	.size	_ZNK9__gnu_cxx13new_allocatorIiE8max_sizeEv, .Lfunc_end32-_ZNK9__gnu_cxx13new_allocatorIiE8max_sizeEv
	.cfi_endproc
                                        # -- End function
	.section	.text._ZNSt6vectorIiSaIiEE11_S_max_sizeERKS0_,"axG",@progbits,_ZNSt6vectorIiSaIiEE11_S_max_sizeERKS0_,comdat
	.weak	_ZNSt6vectorIiSaIiEE11_S_max_sizeERKS0_ # -- Begin function _ZNSt6vectorIiSaIiEE11_S_max_sizeERKS0_
	.p2align	4, 0x90
	.type	_ZNSt6vectorIiSaIiEE11_S_max_sizeERKS0_,@function
_ZNSt6vectorIiSaIiEE11_S_max_sizeERKS0_: # @_ZNSt6vectorIiSaIiEE11_S_max_sizeERKS0_
.Lfunc_begin4:
	.cfi_startproc
	.cfi_personality 3, __gxx_personality_v0
	.cfi_lsda 3, .Lexception4
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$48, %rsp
	movq	%rdi, -8(%rbp)
	movabsq	$2305843009213693951, %rax # imm = 0x1FFFFFFFFFFFFFFF
	movq	%rax, -16(%rbp)
	movq	-8(%rbp), %rdi
	callq	_ZNSt16allocator_traitsISaIiEE8max_sizeERKS0_
	movq	%rax, -24(%rbp)
.Ltmp25:
	leaq	-16(%rbp), %rdi
	leaq	-24(%rbp), %rsi
	callq	_ZSt3minImERKT_S2_S2_
.Ltmp26:
	movq	%rax, -32(%rbp)         # 8-byte Spill
	jmp	.LBB33_1
.LBB33_1:
	movq	-32(%rbp), %rax         # 8-byte Reload
	movq	(%rax), %rax
	addq	$48, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.LBB33_2:
	.cfi_def_cfa %rbp, 16
.Ltmp27:
                                        # kill: def $edx killed $edx killed $rdx
	movq	%rax, %rdi
	movl	%edx, -36(%rbp)         # 4-byte Spill
	callq	__clang_call_terminate
.Lfunc_end33:
	.size	_ZNSt6vectorIiSaIiEE11_S_max_sizeERKS0_, .Lfunc_end33-_ZNSt6vectorIiSaIiEE11_S_max_sizeERKS0_
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table33:
.Lexception4:
	.byte	255                     # @LPStart Encoding = omit
	.byte	3                       # @TType Encoding = udata4
	.uleb128 .Lttbase2-.Lttbaseref2
.Lttbaseref2:
	.byte	1                       # Call site Encoding = uleb128
	.uleb128 .Lcst_end4-.Lcst_begin4
.Lcst_begin4:
	.uleb128 .Ltmp25-.Lfunc_begin4  # >> Call Site 1 <<
	.uleb128 .Ltmp26-.Ltmp25        #   Call between .Ltmp25 and .Ltmp26
	.uleb128 .Ltmp27-.Lfunc_begin4  #     jumps to .Ltmp27
	.byte	1                       #   On action: 1
.Lcst_end4:
	.byte	1                       # >> Action Record 1 <<
                                        #   Catch TypeInfo 1
	.byte	0                       #   No further actions
	.p2align	2
                                        # >> Catch TypeInfos <<
	.long	0                       # TypeInfo 1
.Lttbase2:
	.p2align	2
                                        # -- End function
	.section	.text._ZNSaIiEC2ERKS_,"axG",@progbits,_ZNSaIiEC2ERKS_,comdat
	.weak	_ZNSaIiEC2ERKS_         # -- Begin function _ZNSaIiEC2ERKS_
	.p2align	4, 0x90
	.type	_ZNSaIiEC2ERKS_,@function
_ZNSaIiEC2ERKS_:                        # @_ZNSaIiEC2ERKS_
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rax
	movq	-16(%rbp), %rcx
	movq	%rax, %rdi
	movq	%rcx, %rsi
	callq	_ZN9__gnu_cxx13new_allocatorIiEC2ERKS1_
	addq	$16, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end34:
	.size	_ZNSaIiEC2ERKS_, .Lfunc_end34-_ZNSaIiEC2ERKS_
	.cfi_endproc
                                        # -- End function
	.section	.text._ZNSt16allocator_traitsISaIiEE8max_sizeERKS0_,"axG",@progbits,_ZNSt16allocator_traitsISaIiEE8max_sizeERKS0_,comdat
	.weak	_ZNSt16allocator_traitsISaIiEE8max_sizeERKS0_ # -- Begin function _ZNSt16allocator_traitsISaIiEE8max_sizeERKS0_
	.p2align	4, 0x90
	.type	_ZNSt16allocator_traitsISaIiEE8max_sizeERKS0_,@function
_ZNSt16allocator_traitsISaIiEE8max_sizeERKS0_: # @_ZNSt16allocator_traitsISaIiEE8max_sizeERKS0_
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	callq	_ZNK9__gnu_cxx13new_allocatorIiE8max_sizeEv
	addq	$16, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end35:
	.size	_ZNSt16allocator_traitsISaIiEE8max_sizeERKS0_, .Lfunc_end35-_ZNSt16allocator_traitsISaIiEE8max_sizeERKS0_
	.cfi_endproc
                                        # -- End function
	.section	.text._ZSt3minImERKT_S2_S2_,"axG",@progbits,_ZSt3minImERKT_S2_S2_,comdat
	.weak	_ZSt3minImERKT_S2_S2_   # -- Begin function _ZSt3minImERKT_S2_S2_
	.p2align	4, 0x90
	.type	_ZSt3minImERKT_S2_S2_,@function
_ZSt3minImERKT_S2_S2_:                  # @_ZSt3minImERKT_S2_S2_
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -16(%rbp)
	movq	%rsi, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movq	-16(%rbp), %rcx
	cmpq	(%rcx), %rax
	jae	.LBB36_2
# %bb.1:
	movq	-24(%rbp), %rax
	movq	%rax, -8(%rbp)
	jmp	.LBB36_3
.LBB36_2:
	movq	-16(%rbp), %rax
	movq	%rax, -8(%rbp)
.LBB36_3:
	movq	-8(%rbp), %rax
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end36:
	.size	_ZSt3minImERKT_S2_S2_, .Lfunc_end36-_ZSt3minImERKT_S2_S2_
	.cfi_endproc
                                        # -- End function
	.section	.text.__clang_call_terminate,"axG",@progbits,__clang_call_terminate,comdat
	.hidden	__clang_call_terminate  # -- Begin function __clang_call_terminate
	.weak	__clang_call_terminate
	.p2align	4, 0x90
	.type	__clang_call_terminate,@function
__clang_call_terminate:                 # @__clang_call_terminate
# %bb.0:
	pushq	%rax
	callq	__cxa_begin_catch
	movq	%rax, (%rsp)            # 8-byte Spill
	callq	_ZSt9terminatev
.Lfunc_end37:
	.size	__clang_call_terminate, .Lfunc_end37-__clang_call_terminate
                                        # -- End function
	.section	.text._ZSt18uninitialized_copyIPKiPiET0_T_S4_S3_,"axG",@progbits,_ZSt18uninitialized_copyIPKiPiET0_T_S4_S3_,comdat
	.weak	_ZSt18uninitialized_copyIPKiPiET0_T_S4_S3_ # -- Begin function _ZSt18uninitialized_copyIPKiPiET0_T_S4_S3_
	.p2align	4, 0x90
	.type	_ZSt18uninitialized_copyIPKiPiET0_T_S4_S3_,@function
_ZSt18uninitialized_copyIPKiPiET0_T_S4_S3_: # @_ZSt18uninitialized_copyIPKiPiET0_T_S4_S3_
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movb	$1, -25(%rbp)
	movq	-8(%rbp), %rdi
	movq	-16(%rbp), %rsi
	movq	-24(%rbp), %rdx
	callq	_ZNSt20__uninitialized_copyILb1EE13__uninit_copyIPKiPiEET0_T_S6_S5_
	addq	$32, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end38:
	.size	_ZSt18uninitialized_copyIPKiPiET0_T_S4_S3_, .Lfunc_end38-_ZSt18uninitialized_copyIPKiPiET0_T_S4_S3_
	.cfi_endproc
                                        # -- End function
	.section	.text._ZNSt20__uninitialized_copyILb1EE13__uninit_copyIPKiPiEET0_T_S6_S5_,"axG",@progbits,_ZNSt20__uninitialized_copyILb1EE13__uninit_copyIPKiPiEET0_T_S6_S5_,comdat
	.weak	_ZNSt20__uninitialized_copyILb1EE13__uninit_copyIPKiPiEET0_T_S6_S5_ # -- Begin function _ZNSt20__uninitialized_copyILb1EE13__uninit_copyIPKiPiEET0_T_S6_S5_
	.p2align	4, 0x90
	.type	_ZNSt20__uninitialized_copyILb1EE13__uninit_copyIPKiPiEET0_T_S6_S5_,@function
_ZNSt20__uninitialized_copyILb1EE13__uninit_copyIPKiPiEET0_T_S6_S5_: # @_ZNSt20__uninitialized_copyILb1EE13__uninit_copyIPKiPiEET0_T_S6_S5_
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	-8(%rbp), %rdi
	movq	-16(%rbp), %rsi
	movq	-24(%rbp), %rdx
	callq	_ZSt4copyIPKiPiET0_T_S4_S3_
	addq	$32, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end39:
	.size	_ZNSt20__uninitialized_copyILb1EE13__uninit_copyIPKiPiEET0_T_S6_S5_, .Lfunc_end39-_ZNSt20__uninitialized_copyILb1EE13__uninit_copyIPKiPiEET0_T_S6_S5_
	.cfi_endproc
                                        # -- End function
	.section	.text._ZSt4copyIPKiPiET0_T_S4_S3_,"axG",@progbits,_ZSt4copyIPKiPiET0_T_S4_S3_,comdat
	.weak	_ZSt4copyIPKiPiET0_T_S4_S3_ # -- Begin function _ZSt4copyIPKiPiET0_T_S4_S3_
	.p2align	4, 0x90
	.type	_ZSt4copyIPKiPiET0_T_S4_S3_,@function
_ZSt4copyIPKiPiET0_T_S4_S3_:            # @_ZSt4copyIPKiPiET0_T_S4_S3_
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	-8(%rbp), %rdi
	callq	_ZSt12__miter_baseIPKiET_S2_
	movq	-16(%rbp), %rdi
	movq	%rax, -32(%rbp)         # 8-byte Spill
	callq	_ZSt12__miter_baseIPKiET_S2_
	movq	-24(%rbp), %rdx
	movq	-32(%rbp), %rdi         # 8-byte Reload
	movq	%rax, %rsi
	callq	_ZSt14__copy_move_a2ILb0EPKiPiET1_T0_S4_S3_
	addq	$32, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end40:
	.size	_ZSt4copyIPKiPiET0_T_S4_S3_, .Lfunc_end40-_ZSt4copyIPKiPiET0_T_S4_S3_
	.cfi_endproc
                                        # -- End function
	.section	.text._ZSt14__copy_move_a2ILb0EPKiPiET1_T0_S4_S3_,"axG",@progbits,_ZSt14__copy_move_a2ILb0EPKiPiET1_T0_S4_S3_,comdat
	.weak	_ZSt14__copy_move_a2ILb0EPKiPiET1_T0_S4_S3_ # -- Begin function _ZSt14__copy_move_a2ILb0EPKiPiET1_T0_S4_S3_
	.p2align	4, 0x90
	.type	_ZSt14__copy_move_a2ILb0EPKiPiET1_T0_S4_S3_,@function
_ZSt14__copy_move_a2ILb0EPKiPiET1_T0_S4_S3_: # @_ZSt14__copy_move_a2ILb0EPKiPiET1_T0_S4_S3_
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$48, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	-8(%rbp), %rdi
	callq	_ZSt12__niter_baseIPKiET_S2_
	movq	-16(%rbp), %rdi
	movq	%rax, -32(%rbp)         # 8-byte Spill
	callq	_ZSt12__niter_baseIPKiET_S2_
	movq	-24(%rbp), %rdi
	movq	%rax, -40(%rbp)         # 8-byte Spill
	callq	_ZSt12__niter_baseIPiET_S1_
	movq	-32(%rbp), %rdi         # 8-byte Reload
	movq	-40(%rbp), %rsi         # 8-byte Reload
	movq	%rax, %rdx
	callq	_ZSt13__copy_move_aILb0EPKiPiET1_T0_S4_S3_
	leaq	-24(%rbp), %rdi
	movq	%rax, %rsi
	callq	_ZSt12__niter_wrapIPiET_RKS1_S1_
	addq	$48, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end41:
	.size	_ZSt14__copy_move_a2ILb0EPKiPiET1_T0_S4_S3_, .Lfunc_end41-_ZSt14__copy_move_a2ILb0EPKiPiET1_T0_S4_S3_
	.cfi_endproc
                                        # -- End function
	.section	.text._ZSt12__miter_baseIPKiET_S2_,"axG",@progbits,_ZSt12__miter_baseIPKiET_S2_,comdat
	.weak	_ZSt12__miter_baseIPKiET_S2_ # -- Begin function _ZSt12__miter_baseIPKiET_S2_
	.p2align	4, 0x90
	.type	_ZSt12__miter_baseIPKiET_S2_,@function
_ZSt12__miter_baseIPKiET_S2_:           # @_ZSt12__miter_baseIPKiET_S2_
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end42:
	.size	_ZSt12__miter_baseIPKiET_S2_, .Lfunc_end42-_ZSt12__miter_baseIPKiET_S2_
	.cfi_endproc
                                        # -- End function
	.section	.text._ZSt12__niter_wrapIPiET_RKS1_S1_,"axG",@progbits,_ZSt12__niter_wrapIPiET_RKS1_S1_,comdat
	.weak	_ZSt12__niter_wrapIPiET_RKS1_S1_ # -- Begin function _ZSt12__niter_wrapIPiET_RKS1_S1_
	.p2align	4, 0x90
	.type	_ZSt12__niter_wrapIPiET_RKS1_S1_,@function
_ZSt12__niter_wrapIPiET_RKS1_S1_:       # @_ZSt12__niter_wrapIPiET_RKS1_S1_
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-16(%rbp), %rax
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end43:
	.size	_ZSt12__niter_wrapIPiET_RKS1_S1_, .Lfunc_end43-_ZSt12__niter_wrapIPiET_RKS1_S1_
	.cfi_endproc
                                        # -- End function
	.section	.text._ZSt13__copy_move_aILb0EPKiPiET1_T0_S4_S3_,"axG",@progbits,_ZSt13__copy_move_aILb0EPKiPiET1_T0_S4_S3_,comdat
	.weak	_ZSt13__copy_move_aILb0EPKiPiET1_T0_S4_S3_ # -- Begin function _ZSt13__copy_move_aILb0EPKiPiET1_T0_S4_S3_
	.p2align	4, 0x90
	.type	_ZSt13__copy_move_aILb0EPKiPiET1_T0_S4_S3_,@function
_ZSt13__copy_move_aILb0EPKiPiET1_T0_S4_S3_: # @_ZSt13__copy_move_aILb0EPKiPiET1_T0_S4_S3_
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movb	$1, -25(%rbp)
	movq	-8(%rbp), %rdi
	movq	-16(%rbp), %rsi
	movq	-24(%rbp), %rdx
	callq	_ZNSt11__copy_moveILb0ELb1ESt26random_access_iterator_tagE8__copy_mIiEEPT_PKS3_S6_S4_
	addq	$32, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end44:
	.size	_ZSt13__copy_move_aILb0EPKiPiET1_T0_S4_S3_, .Lfunc_end44-_ZSt13__copy_move_aILb0EPKiPiET1_T0_S4_S3_
	.cfi_endproc
                                        # -- End function
	.section	.text._ZSt12__niter_baseIPKiET_S2_,"axG",@progbits,_ZSt12__niter_baseIPKiET_S2_,comdat
	.weak	_ZSt12__niter_baseIPKiET_S2_ # -- Begin function _ZSt12__niter_baseIPKiET_S2_
	.p2align	4, 0x90
	.type	_ZSt12__niter_baseIPKiET_S2_,@function
_ZSt12__niter_baseIPKiET_S2_:           # @_ZSt12__niter_baseIPKiET_S2_
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end45:
	.size	_ZSt12__niter_baseIPKiET_S2_, .Lfunc_end45-_ZSt12__niter_baseIPKiET_S2_
	.cfi_endproc
                                        # -- End function
	.section	.text._ZSt12__niter_baseIPiET_S1_,"axG",@progbits,_ZSt12__niter_baseIPiET_S1_,comdat
	.weak	_ZSt12__niter_baseIPiET_S1_ # -- Begin function _ZSt12__niter_baseIPiET_S1_
	.p2align	4, 0x90
	.type	_ZSt12__niter_baseIPiET_S1_,@function
_ZSt12__niter_baseIPiET_S1_:            # @_ZSt12__niter_baseIPiET_S1_
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end46:
	.size	_ZSt12__niter_baseIPiET_S1_, .Lfunc_end46-_ZSt12__niter_baseIPiET_S1_
	.cfi_endproc
                                        # -- End function
	.section	.text._ZNSt11__copy_moveILb0ELb1ESt26random_access_iterator_tagE8__copy_mIiEEPT_PKS3_S6_S4_,"axG",@progbits,_ZNSt11__copy_moveILb0ELb1ESt26random_access_iterator_tagE8__copy_mIiEEPT_PKS3_S6_S4_,comdat
	.weak	_ZNSt11__copy_moveILb0ELb1ESt26random_access_iterator_tagE8__copy_mIiEEPT_PKS3_S6_S4_ # -- Begin function _ZNSt11__copy_moveILb0ELb1ESt26random_access_iterator_tagE8__copy_mIiEEPT_PKS3_S6_S4_
	.p2align	4, 0x90
	.type	_ZNSt11__copy_moveILb0ELb1ESt26random_access_iterator_tagE8__copy_mIiEEPT_PKS3_S6_S4_,@function
_ZNSt11__copy_moveILb0ELb1ESt26random_access_iterator_tagE8__copy_mIiEEPT_PKS3_S6_S4_: # @_ZNSt11__copy_moveILb0ELb1ESt26random_access_iterator_tagE8__copy_mIiEEPT_PKS3_S6_S4_
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	-16(%rbp), %rax
	movq	-8(%rbp), %rcx
	subq	%rcx, %rax
	sarq	$2, %rax
	movq	%rax, -32(%rbp)
	cmpq	$0, -32(%rbp)
	je	.LBB47_2
# %bb.1:
	movq	-24(%rbp), %rax
	movq	-8(%rbp), %rcx
	movq	-32(%rbp), %rdx
	shlq	$2, %rdx
	movq	%rax, %rdi
	movq	%rcx, %rsi
	callq	memmove
.LBB47_2:
	movq	-24(%rbp), %rax
	movq	-32(%rbp), %rcx
	shlq	$2, %rcx
	addq	%rcx, %rax
	addq	$32, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end47:
	.size	_ZNSt11__copy_moveILb0ELb1ESt26random_access_iterator_tagE8__copy_mIiEEPT_PKS3_S6_S4_, .Lfunc_end47-_ZNSt11__copy_moveILb0ELb1ESt26random_access_iterator_tagE8__copy_mIiEEPT_PKS3_S6_S4_
	.cfi_endproc
                                        # -- End function
	.section	.text._ZNKSt16initializer_listIiE4sizeEv,"axG",@progbits,_ZNKSt16initializer_listIiE4sizeEv,comdat
	.weak	_ZNKSt16initializer_listIiE4sizeEv # -- Begin function _ZNKSt16initializer_listIiE4sizeEv
	.p2align	4, 0x90
	.type	_ZNKSt16initializer_listIiE4sizeEv,@function
_ZNKSt16initializer_listIiE4sizeEv:     # @_ZNKSt16initializer_listIiE4sizeEv
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	8(%rax), %rax
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end48:
	.size	_ZNKSt16initializer_listIiE4sizeEv, .Lfunc_end48-_ZNKSt16initializer_listIiE4sizeEv
	.cfi_endproc
                                        # -- End function
	.section	.text._ZNSt12_Vector_baseIiSaIiEE13_M_deallocateEPim,"axG",@progbits,_ZNSt12_Vector_baseIiSaIiEE13_M_deallocateEPim,comdat
	.weak	_ZNSt12_Vector_baseIiSaIiEE13_M_deallocateEPim # -- Begin function _ZNSt12_Vector_baseIiSaIiEE13_M_deallocateEPim
	.p2align	4, 0x90
	.type	_ZNSt12_Vector_baseIiSaIiEE13_M_deallocateEPim,@function
_ZNSt12_Vector_baseIiSaIiEE13_M_deallocateEPim: # @_ZNSt12_Vector_baseIiSaIiEE13_M_deallocateEPim
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	-8(%rbp), %rax
	cmpq	$0, -16(%rbp)
	movq	%rax, -32(%rbp)         # 8-byte Spill
	je	.LBB49_2
# %bb.1:
	movq	-32(%rbp), %rax         # 8-byte Reload
	movq	-16(%rbp), %rsi
	movq	-24(%rbp), %rdx
	movq	%rax, %rdi
	callq	_ZNSt16allocator_traitsISaIiEE10deallocateERS0_Pim
.LBB49_2:
	addq	$32, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end49:
	.size	_ZNSt12_Vector_baseIiSaIiEE13_M_deallocateEPim, .Lfunc_end49-_ZNSt12_Vector_baseIiSaIiEE13_M_deallocateEPim
	.cfi_endproc
                                        # -- End function
	.section	.text._ZNSt12_Vector_baseIiSaIiEE12_Vector_implD2Ev,"axG",@progbits,_ZNSt12_Vector_baseIiSaIiEE12_Vector_implD2Ev,comdat
	.weak	_ZNSt12_Vector_baseIiSaIiEE12_Vector_implD2Ev # -- Begin function _ZNSt12_Vector_baseIiSaIiEE12_Vector_implD2Ev
	.p2align	4, 0x90
	.type	_ZNSt12_Vector_baseIiSaIiEE12_Vector_implD2Ev,@function
_ZNSt12_Vector_baseIiSaIiEE12_Vector_implD2Ev: # @_ZNSt12_Vector_baseIiSaIiEE12_Vector_implD2Ev
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	callq	_ZNSaIiED2Ev
	addq	$16, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end50:
	.size	_ZNSt12_Vector_baseIiSaIiEE12_Vector_implD2Ev, .Lfunc_end50-_ZNSt12_Vector_baseIiSaIiEE12_Vector_implD2Ev
	.cfi_endproc
                                        # -- End function
	.section	.text._ZNSt16allocator_traitsISaIiEE10deallocateERS0_Pim,"axG",@progbits,_ZNSt16allocator_traitsISaIiEE10deallocateERS0_Pim,comdat
	.weak	_ZNSt16allocator_traitsISaIiEE10deallocateERS0_Pim # -- Begin function _ZNSt16allocator_traitsISaIiEE10deallocateERS0_Pim
	.p2align	4, 0x90
	.type	_ZNSt16allocator_traitsISaIiEE10deallocateERS0_Pim,@function
_ZNSt16allocator_traitsISaIiEE10deallocateERS0_Pim: # @_ZNSt16allocator_traitsISaIiEE10deallocateERS0_Pim
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	-8(%rbp), %rax
	movq	-16(%rbp), %rsi
	movq	-24(%rbp), %rdx
	movq	%rax, %rdi
	callq	_ZN9__gnu_cxx13new_allocatorIiE10deallocateEPim
	addq	$32, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end51:
	.size	_ZNSt16allocator_traitsISaIiEE10deallocateERS0_Pim, .Lfunc_end51-_ZNSt16allocator_traitsISaIiEE10deallocateERS0_Pim
	.cfi_endproc
                                        # -- End function
	.section	.text._ZN9__gnu_cxx13new_allocatorIiE10deallocateEPim,"axG",@progbits,_ZN9__gnu_cxx13new_allocatorIiE10deallocateEPim,comdat
	.weak	_ZN9__gnu_cxx13new_allocatorIiE10deallocateEPim # -- Begin function _ZN9__gnu_cxx13new_allocatorIiE10deallocateEPim
	.p2align	4, 0x90
	.type	_ZN9__gnu_cxx13new_allocatorIiE10deallocateEPim,@function
_ZN9__gnu_cxx13new_allocatorIiE10deallocateEPim: # @_ZN9__gnu_cxx13new_allocatorIiE10deallocateEPim
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	-16(%rbp), %rax
	movq	%rax, %rdi
	callq	_ZdlPv
	addq	$32, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end52:
	.size	_ZN9__gnu_cxx13new_allocatorIiE10deallocateEPim, .Lfunc_end52-_ZN9__gnu_cxx13new_allocatorIiE10deallocateEPim
	.cfi_endproc
                                        # -- End function
	.section	.text._ZSt8_DestroyIPiiEvT_S1_RSaIT0_E,"axG",@progbits,_ZSt8_DestroyIPiiEvT_S1_RSaIT0_E,comdat
	.weak	_ZSt8_DestroyIPiiEvT_S1_RSaIT0_E # -- Begin function _ZSt8_DestroyIPiiEvT_S1_RSaIT0_E
	.p2align	4, 0x90
	.type	_ZSt8_DestroyIPiiEvT_S1_RSaIT0_E,@function
_ZSt8_DestroyIPiiEvT_S1_RSaIT0_E:       # @_ZSt8_DestroyIPiiEvT_S1_RSaIT0_E
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	-8(%rbp), %rdi
	movq	-16(%rbp), %rsi
	callq	_ZSt8_DestroyIPiEvT_S1_
	addq	$32, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end53:
	.size	_ZSt8_DestroyIPiiEvT_S1_RSaIT0_E, .Lfunc_end53-_ZSt8_DestroyIPiiEvT_S1_RSaIT0_E
	.cfi_endproc
                                        # -- End function
	.section	.text._ZSt8_DestroyIPiEvT_S1_,"axG",@progbits,_ZSt8_DestroyIPiEvT_S1_,comdat
	.weak	_ZSt8_DestroyIPiEvT_S1_ # -- Begin function _ZSt8_DestroyIPiEvT_S1_
	.p2align	4, 0x90
	.type	_ZSt8_DestroyIPiEvT_S1_,@function
_ZSt8_DestroyIPiEvT_S1_:                # @_ZSt8_DestroyIPiEvT_S1_
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rdi
	movq	-16(%rbp), %rsi
	callq	_ZNSt12_Destroy_auxILb1EE9__destroyIPiEEvT_S3_
	addq	$16, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end54:
	.size	_ZSt8_DestroyIPiEvT_S1_, .Lfunc_end54-_ZSt8_DestroyIPiEvT_S1_
	.cfi_endproc
                                        # -- End function
	.section	.text._ZNSt12_Destroy_auxILb1EE9__destroyIPiEEvT_S3_,"axG",@progbits,_ZNSt12_Destroy_auxILb1EE9__destroyIPiEEvT_S3_,comdat
	.weak	_ZNSt12_Destroy_auxILb1EE9__destroyIPiEEvT_S3_ # -- Begin function _ZNSt12_Destroy_auxILb1EE9__destroyIPiEEvT_S3_
	.p2align	4, 0x90
	.type	_ZNSt12_Destroy_auxILb1EE9__destroyIPiEEvT_S3_,@function
_ZNSt12_Destroy_auxILb1EE9__destroyIPiEEvT_S3_: # @_ZNSt12_Destroy_auxILb1EE9__destroyIPiEEvT_S3_
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end55:
	.size	_ZNSt12_Destroy_auxILb1EE9__destroyIPiEEvT_S3_, .Lfunc_end55-_ZNSt12_Destroy_auxILb1EE9__destroyIPiEEvT_S3_
	.cfi_endproc
                                        # -- End function
	.section	.text._ZNSt6vectorIiSaIiEE17_M_default_appendEm,"axG",@progbits,_ZNSt6vectorIiSaIiEE17_M_default_appendEm,comdat
	.weak	_ZNSt6vectorIiSaIiEE17_M_default_appendEm # -- Begin function _ZNSt6vectorIiSaIiEE17_M_default_appendEm
	.p2align	4, 0x90
	.type	_ZNSt6vectorIiSaIiEE17_M_default_appendEm,@function
_ZNSt6vectorIiSaIiEE17_M_default_appendEm: # @_ZNSt6vectorIiSaIiEE17_M_default_appendEm
.Lfunc_begin5:
	.cfi_startproc
	.cfi_personality 3, __gxx_personality_v0
	.cfi_lsda 3, .Lexception5
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$176, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rax
	cmpq	$0, -16(%rbp)
	movq	%rax, -72(%rbp)         # 8-byte Spill
	je	.LBB56_15
# %bb.1:
	movq	-72(%rbp), %rdi         # 8-byte Reload
	callq	_ZNKSt6vectorIiSaIiEE4sizeEv
	movq	%rax, -24(%rbp)
	movq	-72(%rbp), %rax         # 8-byte Reload
	movq	16(%rax), %rcx
	movq	8(%rax), %rdx
	subq	%rdx, %rcx
	sarq	$2, %rcx
	movq	%rcx, -32(%rbp)
	movq	-24(%rbp), %rcx
	movq	%rax, %rdi
	movq	%rcx, -80(%rbp)         # 8-byte Spill
	callq	_ZNKSt6vectorIiSaIiEE8max_sizeEv
	movq	-80(%rbp), %rcx         # 8-byte Reload
	cmpq	%rax, %rcx
	ja	.LBB56_3
# %bb.2:
	movq	-32(%rbp), %rax
	movq	-72(%rbp), %rdi         # 8-byte Reload
	movq	%rax, -88(%rbp)         # 8-byte Spill
	callq	_ZNKSt6vectorIiSaIiEE8max_sizeEv
	subq	-24(%rbp), %rax
	movq	-88(%rbp), %rcx         # 8-byte Reload
	cmpq	%rax, %rcx
	jbe	.LBB56_4
.LBB56_3:
.LBB56_4:
	movq	-32(%rbp), %rax
	cmpq	-16(%rbp), %rax
	jb	.LBB56_6
# %bb.5:
	movq	-72(%rbp), %rax         # 8-byte Reload
	movq	8(%rax), %rdi
	movq	-16(%rbp), %rsi
	movq	%rdi, -96(%rbp)         # 8-byte Spill
	movq	%rax, %rdi
	movq	%rsi, -104(%rbp)        # 8-byte Spill
	callq	_ZNSt12_Vector_baseIiSaIiEE19_M_get_Tp_allocatorEv
	movq	-96(%rbp), %rdi         # 8-byte Reload
	movq	-104(%rbp), %rsi        # 8-byte Reload
	movq	%rax, %rdx
	callq	_ZSt27__uninitialized_default_n_aIPimiET_S1_T0_RSaIT1_E
	movq	-72(%rbp), %rcx         # 8-byte Reload
	movq	%rax, 8(%rcx)
	jmp	.LBB56_14
.LBB56_6:
	movq	-16(%rbp), %rsi
	movl	$.L.str.2, %edx
	movq	-72(%rbp), %rdi         # 8-byte Reload
	callq	_ZNKSt6vectorIiSaIiEE12_M_check_lenEmPKc
	movq	%rax, -40(%rbp)
	movq	-40(%rbp), %rsi
	movq	-72(%rbp), %rdi         # 8-byte Reload
	callq	_ZNSt12_Vector_baseIiSaIiEE11_M_allocateEm
	movq	%rax, -48(%rbp)
	movq	-48(%rbp), %rax
	movq	-24(%rbp), %rcx
	leaq	(%rax,%rcx,4), %rdi
	movq	-16(%rbp), %rsi
	movq	-72(%rbp), %rax         # 8-byte Reload
	movq	%rdi, -112(%rbp)        # 8-byte Spill
	movq	%rax, %rdi
	movq	%rsi, -120(%rbp)        # 8-byte Spill
	callq	_ZNSt12_Vector_baseIiSaIiEE19_M_get_Tp_allocatorEv
.Ltmp28:
	movq	-112(%rbp), %rdi        # 8-byte Reload
	movq	-120(%rbp), %rsi        # 8-byte Reload
	movq	%rax, %rdx
	callq	_ZSt27__uninitialized_default_n_aIPimiET_S1_T0_RSaIT1_E
.Ltmp29:
	jmp	.LBB56_7
.LBB56_7:
	jmp	.LBB56_13
.LBB56_8:
.Ltmp30:
                                        # kill: def $edx killed $edx killed $rdx
	movq	%rax, -56(%rbp)
	movl	%edx, -60(%rbp)
# %bb.9:
	movq	-56(%rbp), %rdi
	callq	__cxa_begin_catch
	movq	-48(%rbp), %rsi
	movq	-40(%rbp), %rdx
.Ltmp31:
	movq	-72(%rbp), %rdi         # 8-byte Reload
	movq	%rax, -128(%rbp)        # 8-byte Spill
	callq	_ZNSt12_Vector_baseIiSaIiEE13_M_deallocateEPim
.Ltmp32:
	jmp	.LBB56_10
.LBB56_10:
.Ltmp33:
	callq	__cxa_rethrow
.Ltmp34:
	jmp	.LBB56_18
.LBB56_11:
.Ltmp35:
                                        # kill: def $edx killed $edx killed $rdx
	movq	%rax, -56(%rbp)
	movl	%edx, -60(%rbp)
.Ltmp36:
	callq	__cxa_end_catch
.Ltmp37:
	jmp	.LBB56_12
.LBB56_12:
	jmp	.LBB56_16
.LBB56_13:
	movq	-72(%rbp), %rax         # 8-byte Reload
	movq	(%rax), %rdi
	movq	8(%rax), %rsi
	movq	-48(%rbp), %rdx
	movq	%rdi, -136(%rbp)        # 8-byte Spill
	movq	%rax, %rdi
	movq	%rsi, -144(%rbp)        # 8-byte Spill
	movq	%rdx, -152(%rbp)        # 8-byte Spill
	callq	_ZNSt12_Vector_baseIiSaIiEE19_M_get_Tp_allocatorEv
	movq	-136(%rbp), %rdi        # 8-byte Reload
	movq	-144(%rbp), %rsi        # 8-byte Reload
	movq	-152(%rbp), %rdx        # 8-byte Reload
	movq	%rax, %rcx
	callq	_ZNSt6vectorIiSaIiEE11_S_relocateEPiS2_S2_RS0_
	movq	-72(%rbp), %rcx         # 8-byte Reload
	movq	-72(%rbp), %rdx         # 8-byte Reload
	movq	(%rdx), %rsi
	movq	16(%rdx), %rdi
	movq	(%rdx), %r8
	subq	%r8, %rdi
	sarq	$2, %rdi
	movq	%rdi, -160(%rbp)        # 8-byte Spill
	movq	%rcx, %rdi
	movq	-160(%rbp), %rdx        # 8-byte Reload
	movq	%rax, -168(%rbp)        # 8-byte Spill
	callq	_ZNSt12_Vector_baseIiSaIiEE13_M_deallocateEPim
	movq	-48(%rbp), %rax
	movq	-72(%rbp), %rcx         # 8-byte Reload
	movq	%rax, (%rcx)
	movq	-48(%rbp), %rax
	movq	-24(%rbp), %rdx
	shlq	$2, %rdx
	addq	%rdx, %rax
	movq	-16(%rbp), %rdx
	shlq	$2, %rdx
	addq	%rdx, %rax
	movq	%rax, 8(%rcx)
	movq	-48(%rbp), %rax
	movq	-40(%rbp), %rdx
	shlq	$2, %rdx
	addq	%rdx, %rax
	movq	%rax, 16(%rcx)
.LBB56_14:
	jmp	.LBB56_15
.LBB56_15:
	addq	$176, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.LBB56_16:
	.cfi_def_cfa %rbp, 16
	movq	-56(%rbp), %rdi
	callq	_Unwind_Resume
.LBB56_17:
.Ltmp38:
                                        # kill: def $edx killed $edx killed $rdx
	movq	%rax, %rdi
	movl	%edx, -172(%rbp)        # 4-byte Spill
	callq	__clang_call_terminate
.LBB56_18:
.Lfunc_end56:
	.size	_ZNSt6vectorIiSaIiEE17_M_default_appendEm, .Lfunc_end56-_ZNSt6vectorIiSaIiEE17_M_default_appendEm
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table56:
.Lexception5:
	.byte	255                     # @LPStart Encoding = omit
	.byte	3                       # @TType Encoding = udata4
	.uleb128 .Lttbase3-.Lttbaseref3
.Lttbaseref3:
	.byte	1                       # Call site Encoding = uleb128
	.uleb128 .Lcst_end5-.Lcst_begin5
.Lcst_begin5:
	.uleb128 .Lfunc_begin5-.Lfunc_begin5 # >> Call Site 1 <<
	.uleb128 .Ltmp28-.Lfunc_begin5  #   Call between .Lfunc_begin5 and .Ltmp28
	.byte	0                       #     has no landing pad
	.byte	0                       #   On action: cleanup
	.uleb128 .Ltmp28-.Lfunc_begin5  # >> Call Site 2 <<
	.uleb128 .Ltmp29-.Ltmp28        #   Call between .Ltmp28 and .Ltmp29
	.uleb128 .Ltmp30-.Lfunc_begin5  #     jumps to .Ltmp30
	.byte	1                       #   On action: 1
	.uleb128 .Ltmp29-.Lfunc_begin5  # >> Call Site 3 <<
	.uleb128 .Ltmp31-.Ltmp29        #   Call between .Ltmp29 and .Ltmp31
	.byte	0                       #     has no landing pad
	.byte	0                       #   On action: cleanup
	.uleb128 .Ltmp31-.Lfunc_begin5  # >> Call Site 4 <<
	.uleb128 .Ltmp34-.Ltmp31        #   Call between .Ltmp31 and .Ltmp34
	.uleb128 .Ltmp35-.Lfunc_begin5  #     jumps to .Ltmp35
	.byte	0                       #   On action: cleanup
	.uleb128 .Ltmp36-.Lfunc_begin5  # >> Call Site 5 <<
	.uleb128 .Ltmp37-.Ltmp36        #   Call between .Ltmp36 and .Ltmp37
	.uleb128 .Ltmp38-.Lfunc_begin5  #     jumps to .Ltmp38
	.byte	1                       #   On action: 1
	.uleb128 .Ltmp37-.Lfunc_begin5  # >> Call Site 6 <<
	.uleb128 .Lfunc_end56-.Ltmp37   #   Call between .Ltmp37 and .Lfunc_end56
	.byte	0                       #     has no landing pad
	.byte	0                       #   On action: cleanup
.Lcst_end5:
	.byte	1                       # >> Action Record 1 <<
                                        #   Catch TypeInfo 1
	.byte	0                       #   No further actions
	.p2align	2
                                        # >> Catch TypeInfos <<
	.long	0                       # TypeInfo 1
.Lttbase3:
	.p2align	2
                                        # -- End function
	.section	.text._ZNSt6vectorIiSaIiEE15_M_erase_at_endEPi,"axG",@progbits,_ZNSt6vectorIiSaIiEE15_M_erase_at_endEPi,comdat
	.weak	_ZNSt6vectorIiSaIiEE15_M_erase_at_endEPi # -- Begin function _ZNSt6vectorIiSaIiEE15_M_erase_at_endEPi
	.p2align	4, 0x90
	.type	_ZNSt6vectorIiSaIiEE15_M_erase_at_endEPi,@function
_ZNSt6vectorIiSaIiEE15_M_erase_at_endEPi: # @_ZNSt6vectorIiSaIiEE15_M_erase_at_endEPi
.Lfunc_begin6:
	.cfi_startproc
	.cfi_personality 3, __gxx_personality_v0
	.cfi_lsda 3, .Lexception6
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$64, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rax
	movq	8(%rax), %rcx
	movq	-16(%rbp), %rdx
	subq	%rdx, %rcx
	sarq	$2, %rcx
	movq	%rcx, -24(%rbp)
	cmpq	$0, -24(%rbp)
	movq	%rax, -32(%rbp)         # 8-byte Spill
	je	.LBB57_3
# %bb.1:
	movq	-16(%rbp), %rdi
	movq	-32(%rbp), %rax         # 8-byte Reload
	movq	8(%rax), %rsi
	movq	%rdi, -40(%rbp)         # 8-byte Spill
	movq	%rax, %rdi
	movq	%rsi, -48(%rbp)         # 8-byte Spill
	callq	_ZNSt12_Vector_baseIiSaIiEE19_M_get_Tp_allocatorEv
.Ltmp39:
	movq	-40(%rbp), %rdi         # 8-byte Reload
	movq	-48(%rbp), %rsi         # 8-byte Reload
	movq	%rax, %rdx
	callq	_ZSt8_DestroyIPiiEvT_S1_RSaIT0_E
.Ltmp40:
	jmp	.LBB57_2
.LBB57_2:
	movq	-16(%rbp), %rax
	movq	-32(%rbp), %rcx         # 8-byte Reload
	movq	%rax, 8(%rcx)
.LBB57_3:
	addq	$64, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.LBB57_4:
	.cfi_def_cfa %rbp, 16
.Ltmp41:
                                        # kill: def $edx killed $edx killed $rdx
	movq	%rax, %rdi
	movl	%edx, -52(%rbp)         # 4-byte Spill
	callq	__clang_call_terminate
.Lfunc_end57:
	.size	_ZNSt6vectorIiSaIiEE15_M_erase_at_endEPi, .Lfunc_end57-_ZNSt6vectorIiSaIiEE15_M_erase_at_endEPi
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table57:
.Lexception6:
	.byte	255                     # @LPStart Encoding = omit
	.byte	3                       # @TType Encoding = udata4
	.uleb128 .Lttbase4-.Lttbaseref4
.Lttbaseref4:
	.byte	1                       # Call site Encoding = uleb128
	.uleb128 .Lcst_end6-.Lcst_begin6
.Lcst_begin6:
	.uleb128 .Ltmp39-.Lfunc_begin6  # >> Call Site 1 <<
	.uleb128 .Ltmp40-.Ltmp39        #   Call between .Ltmp39 and .Ltmp40
	.uleb128 .Ltmp41-.Lfunc_begin6  #     jumps to .Ltmp41
	.byte	1                       #   On action: 1
.Lcst_end6:
	.byte	1                       # >> Action Record 1 <<
                                        #   Catch TypeInfo 1
	.byte	0                       #   No further actions
	.p2align	2
                                        # >> Catch TypeInfos <<
	.long	0                       # TypeInfo 1
.Lttbase4:
	.p2align	2
                                        # -- End function
	.section	.text._ZNKSt6vectorIiSaIiEE8max_sizeEv,"axG",@progbits,_ZNKSt6vectorIiSaIiEE8max_sizeEv,comdat
	.weak	_ZNKSt6vectorIiSaIiEE8max_sizeEv # -- Begin function _ZNKSt6vectorIiSaIiEE8max_sizeEv
	.p2align	4, 0x90
	.type	_ZNKSt6vectorIiSaIiEE8max_sizeEv,@function
_ZNKSt6vectorIiSaIiEE8max_sizeEv:       # @_ZNKSt6vectorIiSaIiEE8max_sizeEv
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	callq	_ZNKSt12_Vector_baseIiSaIiEE19_M_get_Tp_allocatorEv
	movq	%rax, %rdi
	callq	_ZNSt6vectorIiSaIiEE11_S_max_sizeERKS0_
	addq	$16, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end58:
	.size	_ZNKSt6vectorIiSaIiEE8max_sizeEv, .Lfunc_end58-_ZNKSt6vectorIiSaIiEE8max_sizeEv
	.cfi_endproc
                                        # -- End function
	.section	.text._ZSt27__uninitialized_default_n_aIPimiET_S1_T0_RSaIT1_E,"axG",@progbits,_ZSt27__uninitialized_default_n_aIPimiET_S1_T0_RSaIT1_E,comdat
	.weak	_ZSt27__uninitialized_default_n_aIPimiET_S1_T0_RSaIT1_E # -- Begin function _ZSt27__uninitialized_default_n_aIPimiET_S1_T0_RSaIT1_E
	.p2align	4, 0x90
	.type	_ZSt27__uninitialized_default_n_aIPimiET_S1_T0_RSaIT1_E,@function
_ZSt27__uninitialized_default_n_aIPimiET_S1_T0_RSaIT1_E: # @_ZSt27__uninitialized_default_n_aIPimiET_S1_T0_RSaIT1_E
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	-8(%rbp), %rdi
	movq	-16(%rbp), %rsi
	callq	_ZSt25__uninitialized_default_nIPimET_S1_T0_
	addq	$32, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end59:
	.size	_ZSt27__uninitialized_default_n_aIPimiET_S1_T0_RSaIT1_E, .Lfunc_end59-_ZSt27__uninitialized_default_n_aIPimiET_S1_T0_RSaIT1_E
	.cfi_endproc
                                        # -- End function
	.section	.text._ZNKSt6vectorIiSaIiEE12_M_check_lenEmPKc,"axG",@progbits,_ZNKSt6vectorIiSaIiEE12_M_check_lenEmPKc,comdat
	.weak	_ZNKSt6vectorIiSaIiEE12_M_check_lenEmPKc # -- Begin function _ZNKSt6vectorIiSaIiEE12_M_check_lenEmPKc
	.p2align	4, 0x90
	.type	_ZNKSt6vectorIiSaIiEE12_M_check_lenEmPKc,@function
_ZNKSt6vectorIiSaIiEE12_M_check_lenEmPKc: # @_ZNKSt6vectorIiSaIiEE12_M_check_lenEmPKc
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$96, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	movq	%rax, -48(%rbp)         # 8-byte Spill
	callq	_ZNKSt6vectorIiSaIiEE8max_sizeEv
	movq	-48(%rbp), %rdi         # 8-byte Reload
	movq	%rax, -56(%rbp)         # 8-byte Spill
	callq	_ZNKSt6vectorIiSaIiEE4sizeEv
	movq	-56(%rbp), %rcx         # 8-byte Reload
	subq	%rax, %rcx
	cmpq	-16(%rbp), %rcx
	jae	.LBB60_2
# %bb.1:
	movq	-24(%rbp), %rdi
	callq	_ZSt20__throw_length_errorPKc
.LBB60_2:
	movq	-48(%rbp), %rdi         # 8-byte Reload
	callq	_ZNKSt6vectorIiSaIiEE4sizeEv
	movq	-48(%rbp), %rdi         # 8-byte Reload
	movq	%rax, -64(%rbp)         # 8-byte Spill
	callq	_ZNKSt6vectorIiSaIiEE4sizeEv
	movq	%rax, -40(%rbp)
	leaq	-40(%rbp), %rdi
	leaq	-16(%rbp), %rsi
	callq	_ZSt3maxImERKT_S2_S2_
	movq	-64(%rbp), %rcx         # 8-byte Reload
	addq	(%rax), %rcx
	movq	%rcx, -32(%rbp)
	movq	-32(%rbp), %rax
	movq	-48(%rbp), %rdi         # 8-byte Reload
	movq	%rax, -72(%rbp)         # 8-byte Spill
	callq	_ZNKSt6vectorIiSaIiEE4sizeEv
	movq	-72(%rbp), %rcx         # 8-byte Reload
	cmpq	%rax, %rcx
	jb	.LBB60_4
# %bb.3:
	movq	-32(%rbp), %rax
	movq	-48(%rbp), %rdi         # 8-byte Reload
	movq	%rax, -80(%rbp)         # 8-byte Spill
	callq	_ZNKSt6vectorIiSaIiEE8max_sizeEv
	movq	-80(%rbp), %rcx         # 8-byte Reload
	cmpq	%rax, %rcx
	jbe	.LBB60_5
.LBB60_4:
	movq	-48(%rbp), %rdi         # 8-byte Reload
	callq	_ZNKSt6vectorIiSaIiEE8max_sizeEv
	movq	%rax, -88(%rbp)         # 8-byte Spill
	jmp	.LBB60_6
.LBB60_5:
	movq	-32(%rbp), %rax
	movq	%rax, -88(%rbp)         # 8-byte Spill
.LBB60_6:
	movq	-88(%rbp), %rax         # 8-byte Reload
	addq	$96, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end60:
	.size	_ZNKSt6vectorIiSaIiEE12_M_check_lenEmPKc, .Lfunc_end60-_ZNKSt6vectorIiSaIiEE12_M_check_lenEmPKc
	.cfi_endproc
                                        # -- End function
	.section	.text._ZNSt6vectorIiSaIiEE11_S_relocateEPiS2_S2_RS0_,"axG",@progbits,_ZNSt6vectorIiSaIiEE11_S_relocateEPiS2_S2_RS0_,comdat
	.weak	_ZNSt6vectorIiSaIiEE11_S_relocateEPiS2_S2_RS0_ # -- Begin function _ZNSt6vectorIiSaIiEE11_S_relocateEPiS2_S2_RS0_
	.p2align	4, 0x90
	.type	_ZNSt6vectorIiSaIiEE11_S_relocateEPiS2_S2_RS0_,@function
_ZNSt6vectorIiSaIiEE11_S_relocateEPiS2_S2_RS0_: # @_ZNSt6vectorIiSaIiEE11_S_relocateEPiS2_S2_RS0_
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$48, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	%rcx, -32(%rbp)
	movq	-8(%rbp), %rdi
	movq	-16(%rbp), %rsi
	movq	-24(%rbp), %rdx
	movq	-32(%rbp), %rcx
	callq	_ZNSt6vectorIiSaIiEE14_S_do_relocateEPiS2_S2_RS0_St17integral_constantIbLb1EE
	addq	$48, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end61:
	.size	_ZNSt6vectorIiSaIiEE11_S_relocateEPiS2_S2_RS0_, .Lfunc_end61-_ZNSt6vectorIiSaIiEE11_S_relocateEPiS2_S2_RS0_
	.cfi_endproc
                                        # -- End function
	.section	.text._ZNKSt12_Vector_baseIiSaIiEE19_M_get_Tp_allocatorEv,"axG",@progbits,_ZNKSt12_Vector_baseIiSaIiEE19_M_get_Tp_allocatorEv,comdat
	.weak	_ZNKSt12_Vector_baseIiSaIiEE19_M_get_Tp_allocatorEv # -- Begin function _ZNKSt12_Vector_baseIiSaIiEE19_M_get_Tp_allocatorEv
	.p2align	4, 0x90
	.type	_ZNKSt12_Vector_baseIiSaIiEE19_M_get_Tp_allocatorEv,@function
_ZNKSt12_Vector_baseIiSaIiEE19_M_get_Tp_allocatorEv: # @_ZNKSt12_Vector_baseIiSaIiEE19_M_get_Tp_allocatorEv
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end62:
	.size	_ZNKSt12_Vector_baseIiSaIiEE19_M_get_Tp_allocatorEv, .Lfunc_end62-_ZNKSt12_Vector_baseIiSaIiEE19_M_get_Tp_allocatorEv
	.cfi_endproc
                                        # -- End function
	.section	.text._ZSt25__uninitialized_default_nIPimET_S1_T0_,"axG",@progbits,_ZSt25__uninitialized_default_nIPimET_S1_T0_,comdat
	.weak	_ZSt25__uninitialized_default_nIPimET_S1_T0_ # -- Begin function _ZSt25__uninitialized_default_nIPimET_S1_T0_
	.p2align	4, 0x90
	.type	_ZSt25__uninitialized_default_nIPimET_S1_T0_,@function
_ZSt25__uninitialized_default_nIPimET_S1_T0_: # @_ZSt25__uninitialized_default_nIPimET_S1_T0_
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movb	$1, -17(%rbp)
	movq	-8(%rbp), %rdi
	movq	-16(%rbp), %rsi
	callq	_ZNSt27__uninitialized_default_n_1ILb1EE18__uninit_default_nIPimEET_S3_T0_
	addq	$32, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end63:
	.size	_ZSt25__uninitialized_default_nIPimET_S1_T0_, .Lfunc_end63-_ZSt25__uninitialized_default_nIPimET_S1_T0_
	.cfi_endproc
                                        # -- End function
	.section	.text._ZNSt27__uninitialized_default_n_1ILb1EE18__uninit_default_nIPimEET_S3_T0_,"axG",@progbits,_ZNSt27__uninitialized_default_n_1ILb1EE18__uninit_default_nIPimEET_S3_T0_,comdat
	.weak	_ZNSt27__uninitialized_default_n_1ILb1EE18__uninit_default_nIPimEET_S3_T0_ # -- Begin function _ZNSt27__uninitialized_default_n_1ILb1EE18__uninit_default_nIPimEET_S3_T0_
	.p2align	4, 0x90
	.type	_ZNSt27__uninitialized_default_n_1ILb1EE18__uninit_default_nIPimEET_S3_T0_,@function
_ZNSt27__uninitialized_default_n_1ILb1EE18__uninit_default_nIPimEET_S3_T0_: # @_ZNSt27__uninitialized_default_n_1ILb1EE18__uninit_default_nIPimEET_S3_T0_
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rdi
	movq	-16(%rbp), %rsi
	movl	$0, -20(%rbp)
	leaq	-20(%rbp), %rdx
	callq	_ZSt6fill_nIPimiET_S1_T0_RKT1_
	addq	$32, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end64:
	.size	_ZNSt27__uninitialized_default_n_1ILb1EE18__uninit_default_nIPimEET_S3_T0_, .Lfunc_end64-_ZNSt27__uninitialized_default_n_1ILb1EE18__uninit_default_nIPimEET_S3_T0_
	.cfi_endproc
                                        # -- End function
	.section	.text._ZSt6fill_nIPimiET_S1_T0_RKT1_,"axG",@progbits,_ZSt6fill_nIPimiET_S1_T0_RKT1_,comdat
	.weak	_ZSt6fill_nIPimiET_S1_T0_RKT1_ # -- Begin function _ZSt6fill_nIPimiET_S1_T0_RKT1_
	.p2align	4, 0x90
	.type	_ZSt6fill_nIPimiET_S1_T0_RKT1_,@function
_ZSt6fill_nIPimiET_S1_T0_RKT1_:         # @_ZSt6fill_nIPimiET_S1_T0_RKT1_
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	-8(%rbp), %rdi
	callq	_ZSt12__niter_baseIPiET_S1_
	movq	-16(%rbp), %rsi
	movq	-24(%rbp), %rdx
	movq	%rax, %rdi
	callq	_ZSt10__fill_n_aIPimiEN9__gnu_cxx11__enable_ifIXsr11__is_scalarIT1_EE7__valueET_E6__typeES4_T0_RKS3_
	leaq	-8(%rbp), %rdi
	movq	%rax, %rsi
	callq	_ZSt12__niter_wrapIPiET_RKS1_S1_
	addq	$32, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end65:
	.size	_ZSt6fill_nIPimiET_S1_T0_RKT1_, .Lfunc_end65-_ZSt6fill_nIPimiET_S1_T0_RKT1_
	.cfi_endproc
                                        # -- End function
	.section	.text._ZSt10__fill_n_aIPimiEN9__gnu_cxx11__enable_ifIXsr11__is_scalarIT1_EE7__valueET_E6__typeES4_T0_RKS3_,"axG",@progbits,_ZSt10__fill_n_aIPimiEN9__gnu_cxx11__enable_ifIXsr11__is_scalarIT1_EE7__valueET_E6__typeES4_T0_RKS3_,comdat
	.weak	_ZSt10__fill_n_aIPimiEN9__gnu_cxx11__enable_ifIXsr11__is_scalarIT1_EE7__valueET_E6__typeES4_T0_RKS3_ # -- Begin function _ZSt10__fill_n_aIPimiEN9__gnu_cxx11__enable_ifIXsr11__is_scalarIT1_EE7__valueET_E6__typeES4_T0_RKS3_
	.p2align	4, 0x90
	.type	_ZSt10__fill_n_aIPimiEN9__gnu_cxx11__enable_ifIXsr11__is_scalarIT1_EE7__valueET_E6__typeES4_T0_RKS3_,@function
_ZSt10__fill_n_aIPimiEN9__gnu_cxx11__enable_ifIXsr11__is_scalarIT1_EE7__valueET_E6__typeES4_T0_RKS3_: # @_ZSt10__fill_n_aIPimiEN9__gnu_cxx11__enable_ifIXsr11__is_scalarIT1_EE7__valueET_E6__typeES4_T0_RKS3_
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	-24(%rbp), %rax
	movl	(%rax), %ecx
	movl	%ecx, -28(%rbp)
	movq	-16(%rbp), %rax
	movq	%rax, -40(%rbp)
.LBB66_1:                               # =>This Inner Loop Header: Depth=1
	cmpq	$0, -40(%rbp)
	jbe	.LBB66_4
# %bb.2:                                #   in Loop: Header=BB66_1 Depth=1
	movl	-28(%rbp), %eax
	movq	-8(%rbp), %rcx
	movl	%eax, (%rcx)
# %bb.3:                                #   in Loop: Header=BB66_1 Depth=1
	movq	-40(%rbp), %rax
	addq	$-1, %rax
	movq	%rax, -40(%rbp)
	movq	-8(%rbp), %rax
	addq	$4, %rax
	movq	%rax, -8(%rbp)
	jmp	.LBB66_1
.LBB66_4:
	movq	-8(%rbp), %rax
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end66:
	.size	_ZSt10__fill_n_aIPimiEN9__gnu_cxx11__enable_ifIXsr11__is_scalarIT1_EE7__valueET_E6__typeES4_T0_RKS3_, .Lfunc_end66-_ZSt10__fill_n_aIPimiEN9__gnu_cxx11__enable_ifIXsr11__is_scalarIT1_EE7__valueET_E6__typeES4_T0_RKS3_
	.cfi_endproc
                                        # -- End function
	.section	.text._ZSt3maxImERKT_S2_S2_,"axG",@progbits,_ZSt3maxImERKT_S2_S2_,comdat
	.weak	_ZSt3maxImERKT_S2_S2_   # -- Begin function _ZSt3maxImERKT_S2_S2_
	.p2align	4, 0x90
	.type	_ZSt3maxImERKT_S2_S2_,@function
_ZSt3maxImERKT_S2_S2_:                  # @_ZSt3maxImERKT_S2_S2_
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -16(%rbp)
	movq	%rsi, -24(%rbp)
	movq	-16(%rbp), %rax
	movq	(%rax), %rax
	movq	-24(%rbp), %rcx
	cmpq	(%rcx), %rax
	jae	.LBB67_2
# %bb.1:
	movq	-24(%rbp), %rax
	movq	%rax, -8(%rbp)
	jmp	.LBB67_3
.LBB67_2:
	movq	-16(%rbp), %rax
	movq	%rax, -8(%rbp)
.LBB67_3:
	movq	-8(%rbp), %rax
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end67:
	.size	_ZSt3maxImERKT_S2_S2_, .Lfunc_end67-_ZSt3maxImERKT_S2_S2_
	.cfi_endproc
                                        # -- End function
	.section	.text._ZNSt6vectorIiSaIiEE14_S_do_relocateEPiS2_S2_RS0_St17integral_constantIbLb1EE,"axG",@progbits,_ZNSt6vectorIiSaIiEE14_S_do_relocateEPiS2_S2_RS0_St17integral_constantIbLb1EE,comdat
	.weak	_ZNSt6vectorIiSaIiEE14_S_do_relocateEPiS2_S2_RS0_St17integral_constantIbLb1EE # -- Begin function _ZNSt6vectorIiSaIiEE14_S_do_relocateEPiS2_S2_RS0_St17integral_constantIbLb1EE
	.p2align	4, 0x90
	.type	_ZNSt6vectorIiSaIiEE14_S_do_relocateEPiS2_S2_RS0_St17integral_constantIbLb1EE,@function
_ZNSt6vectorIiSaIiEE14_S_do_relocateEPiS2_S2_RS0_St17integral_constantIbLb1EE: # @_ZNSt6vectorIiSaIiEE14_S_do_relocateEPiS2_S2_RS0_St17integral_constantIbLb1EE
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$48, %rsp
	movq	%rdi, -16(%rbp)
	movq	%rsi, -24(%rbp)
	movq	%rdx, -32(%rbp)
	movq	%rcx, -40(%rbp)
	movq	-16(%rbp), %rdi
	movq	-24(%rbp), %rsi
	movq	-32(%rbp), %rdx
	movq	-40(%rbp), %rcx
	callq	_ZSt12__relocate_aIPiS0_SaIiEET0_T_S3_S2_RT1_
	addq	$48, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end68:
	.size	_ZNSt6vectorIiSaIiEE14_S_do_relocateEPiS2_S2_RS0_St17integral_constantIbLb1EE, .Lfunc_end68-_ZNSt6vectorIiSaIiEE14_S_do_relocateEPiS2_S2_RS0_St17integral_constantIbLb1EE
	.cfi_endproc
                                        # -- End function
	.section	.text._ZSt12__relocate_aIPiS0_SaIiEET0_T_S3_S2_RT1_,"axG",@progbits,_ZSt12__relocate_aIPiS0_SaIiEET0_T_S3_S2_RT1_,comdat
	.weak	_ZSt12__relocate_aIPiS0_SaIiEET0_T_S3_S2_RT1_ # -- Begin function _ZSt12__relocate_aIPiS0_SaIiEET0_T_S3_S2_RT1_
	.p2align	4, 0x90
	.type	_ZSt12__relocate_aIPiS0_SaIiEET0_T_S3_S2_RT1_,@function
_ZSt12__relocate_aIPiS0_SaIiEET0_T_S3_S2_RT1_: # @_ZSt12__relocate_aIPiS0_SaIiEET0_T_S3_S2_RT1_
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$48, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	%rcx, -32(%rbp)
	movq	-8(%rbp), %rdi
	callq	_ZSt12__niter_baseIPiET_S1_
	movq	-16(%rbp), %rdi
	movq	%rax, -40(%rbp)         # 8-byte Spill
	callq	_ZSt12__niter_baseIPiET_S1_
	movq	-24(%rbp), %rdi
	movq	%rax, -48(%rbp)         # 8-byte Spill
	callq	_ZSt12__niter_baseIPiET_S1_
	movq	-32(%rbp), %rcx
	movq	-40(%rbp), %rdi         # 8-byte Reload
	movq	-48(%rbp), %rsi         # 8-byte Reload
	movq	%rax, %rdx
	callq	_ZSt14__relocate_a_1IiiENSt9enable_ifIXsr3std24__is_bitwise_relocatableIT_EE5valueEPS1_E4typeES2_S2_S2_RSaIT0_E
	addq	$48, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end69:
	.size	_ZSt12__relocate_aIPiS0_SaIiEET0_T_S3_S2_RT1_, .Lfunc_end69-_ZSt12__relocate_aIPiS0_SaIiEET0_T_S3_S2_RT1_
	.cfi_endproc
                                        # -- End function
	.section	.text._ZSt14__relocate_a_1IiiENSt9enable_ifIXsr3std24__is_bitwise_relocatableIT_EE5valueEPS1_E4typeES2_S2_S2_RSaIT0_E,"axG",@progbits,_ZSt14__relocate_a_1IiiENSt9enable_ifIXsr3std24__is_bitwise_relocatableIT_EE5valueEPS1_E4typeES2_S2_S2_RSaIT0_E,comdat
	.weak	_ZSt14__relocate_a_1IiiENSt9enable_ifIXsr3std24__is_bitwise_relocatableIT_EE5valueEPS1_E4typeES2_S2_S2_RSaIT0_E # -- Begin function _ZSt14__relocate_a_1IiiENSt9enable_ifIXsr3std24__is_bitwise_relocatableIT_EE5valueEPS1_E4typeES2_S2_S2_RSaIT0_E
	.p2align	4, 0x90
	.type	_ZSt14__relocate_a_1IiiENSt9enable_ifIXsr3std24__is_bitwise_relocatableIT_EE5valueEPS1_E4typeES2_S2_S2_RSaIT0_E,@function
_ZSt14__relocate_a_1IiiENSt9enable_ifIXsr3std24__is_bitwise_relocatableIT_EE5valueEPS1_E4typeES2_S2_S2_RSaIT0_E: # @_ZSt14__relocate_a_1IiiENSt9enable_ifIXsr3std24__is_bitwise_relocatableIT_EE5valueEPS1_E4typeES2_S2_S2_RSaIT0_E
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$48, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	%rcx, -32(%rbp)
	movq	-16(%rbp), %rax
	movq	-8(%rbp), %rcx
	subq	%rcx, %rax
	sarq	$2, %rax
	movq	%rax, -40(%rbp)
	cmpq	$0, -40(%rbp)
	jle	.LBB70_2
# %bb.1:
	movq	-24(%rbp), %rax
	movq	-8(%rbp), %rcx
	movq	-40(%rbp), %rdx
	shlq	$2, %rdx
	movq	%rax, %rdi
	movq	%rcx, %rsi
	callq	memmove
.LBB70_2:
	movq	-24(%rbp), %rax
	movq	-40(%rbp), %rcx
	shlq	$2, %rcx
	addq	%rcx, %rax
	addq	$48, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end70:
	.size	_ZSt14__relocate_a_1IiiENSt9enable_ifIXsr3std24__is_bitwise_relocatableIT_EE5valueEPS1_E4typeES2_S2_S2_RSaIT0_E, .Lfunc_end70-_ZSt14__relocate_a_1IiiENSt9enable_ifIXsr3std24__is_bitwise_relocatableIT_EE5valueEPS1_E4typeES2_S2_S2_RSaIT0_E
	.cfi_endproc
                                        # -- End function
	.section	.text._ZN9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEEC2ERKS1_,"axG",@progbits,_ZN9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEEC2ERKS1_,comdat
	.weak	_ZN9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEEC2ERKS1_ # -- Begin function _ZN9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEEC2ERKS1_
	.p2align	4, 0x90
	.type	_ZN9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEEC2ERKS1_,@function
_ZN9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEEC2ERKS1_: # @_ZN9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEEC2ERKS1_
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rax
	movq	-16(%rbp), %rcx
	movq	(%rcx), %rcx
	movq	%rcx, (%rax)
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end71:
	.size	_ZN9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEEC2ERKS1_, .Lfunc_end71-_ZN9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEEC2ERKS1_
	.cfi_endproc
                                        # -- End function
	.section	.text._ZNK9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEE4baseEv,"axG",@progbits,_ZNK9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEE4baseEv,comdat
	.weak	_ZNK9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEE4baseEv # -- Begin function _ZNK9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEE4baseEv
	.p2align	4, 0x90
	.type	_ZNK9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEE4baseEv,@function
_ZNK9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEE4baseEv: # @_ZNK9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEE4baseEv
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end72:
	.size	_ZNK9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEE4baseEv, .Lfunc_end72-_ZNK9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEE4baseEv
	.cfi_endproc
                                        # -- End function
	.section	.text.startup,"ax",@progbits
	.p2align	4, 0x90         # -- Begin function _GLOBAL__sub_I_vector_t.cpp
	.type	_GLOBAL__sub_I_vector_t.cpp,@function
_GLOBAL__sub_I_vector_t.cpp:            # @_GLOBAL__sub_I_vector_t.cpp
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	callq	__cxx_global_var_init
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end73:
	.size	_GLOBAL__sub_I_vector_t.cpp, .Lfunc_end73-_GLOBAL__sub_I_vector_t.cpp
	.cfi_endproc
                                        # -- End function
	.type	_ZStL8__ioinit,@object  # @_ZStL8__ioinit
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
	.hidden	__dso_handle
	.type	.Lconstinit,@object     # @constinit
	.section	.rodata,"a",@progbits
	.p2align	2
.Lconstinit:
	.long	1                       # 0x1
	.long	2                       # 0x2
	.long	3                       # 0x3
	.long	4                       # 0x4
	.long	5                       # 0x5
	.size	.Lconstinit, 20

	.type	.L.str,@object          # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	", "
	.size	.L.str, 3

	.type	.L.str.1,@object        # @.str.1
.L.str.1:
	.asciz	"cannot create std::vector larger than max_size()"
	.size	.L.str.1, 49

	.type	.L.str.2,@object        # @.str.2
.L.str.2:
	.asciz	"vector::_M_default_append"
	.size	.L.str.2, 26

	.section	.init_array,"aw",@init_array
	.p2align	3
	.quad	_GLOBAL__sub_I_vector_t.cpp

	.ident	"clang version 9.0.0-2 (tags/RELEASE_900/final)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym __cxx_global_var_init
	.addrsig_sym __cxa_atexit
	.addrsig_sym __gxx_personality_v0
	.addrsig_sym _ZNSolsEm
	.addrsig_sym _ZNKSt6vectorIiSaIiEE4sizeEv
	.addrsig_sym _ZNSolsEPFRSoS_E
	.addrsig_sym _ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_
	.addrsig_sym _ZNSt6vectorIiSaIiEE6resizeEm
	.addrsig_sym _ZNSt6vectorIiSaIiEE5beginEv
	.addrsig_sym _ZNSt6vectorIiSaIiEE3endEv
	.addrsig_sym _ZN9__gnu_cxxneIPiSt6vectorIiSaIiEEEEbRKNS_17__normal_iteratorIT_T0_EESA_
	.addrsig_sym _ZNK9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEEdeEv
	.addrsig_sym _ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	.addrsig_sym _ZNSolsEi
	.addrsig_sym _ZN9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEEppEv
	.addrsig_sym _ZNSt6vectorIiSaIiEE19_M_range_initializeIPKiEEvT_S5_St20forward_iterator_tag
	.addrsig_sym _ZNKSt16initializer_listIiE5beginEv
	.addrsig_sym _ZNKSt16initializer_listIiE3endEv
	.addrsig_sym _ZSt8distanceIPKiENSt15iterator_traitsIT_E15difference_typeES3_S3_
	.addrsig_sym _ZNSt12_Vector_baseIiSaIiEE11_M_allocateEm
	.addrsig_sym _ZNSt6vectorIiSaIiEE17_S_check_init_lenEmRKS0_
	.addrsig_sym _ZNSt12_Vector_baseIiSaIiEE19_M_get_Tp_allocatorEv
	.addrsig_sym _ZSt22__uninitialized_copy_aIPKiPiiET0_T_S4_S3_RSaIT1_E
	.addrsig_sym _ZSt10__distanceIPKiENSt15iterator_traitsIT_E15difference_typeES3_S3_St26random_access_iterator_tag
	.addrsig_sym _ZSt19__iterator_categoryIPKiENSt15iterator_traitsIT_E17iterator_categoryERKS3_
	.addrsig_sym _ZNSt16allocator_traitsISaIiEE8allocateERS0_m
	.addrsig_sym _ZN9__gnu_cxx13new_allocatorIiE8allocateEmPKv
	.addrsig_sym _ZNK9__gnu_cxx13new_allocatorIiE8max_sizeEv
	.addrsig_sym _ZSt17__throw_bad_allocv
	.addrsig_sym _Znwm
	.addrsig_sym _ZNSt6vectorIiSaIiEE11_S_max_sizeERKS0_
	.addrsig_sym _ZSt20__throw_length_errorPKc
	.addrsig_sym _ZNSt16allocator_traitsISaIiEE8max_sizeERKS0_
	.addrsig_sym _ZSt3minImERKT_S2_S2_
	.addrsig_sym __clang_call_terminate
	.addrsig_sym __cxa_begin_catch
	.addrsig_sym _ZSt9terminatev
	.addrsig_sym _ZSt18uninitialized_copyIPKiPiET0_T_S4_S3_
	.addrsig_sym _ZNSt20__uninitialized_copyILb1EE13__uninit_copyIPKiPiEET0_T_S6_S5_
	.addrsig_sym _ZSt4copyIPKiPiET0_T_S4_S3_
	.addrsig_sym _ZSt14__copy_move_a2ILb0EPKiPiET1_T0_S4_S3_
	.addrsig_sym _ZSt12__miter_baseIPKiET_S2_
	.addrsig_sym _ZSt12__niter_wrapIPiET_RKS1_S1_
	.addrsig_sym _ZSt13__copy_move_aILb0EPKiPiET1_T0_S4_S3_
	.addrsig_sym _ZSt12__niter_baseIPKiET_S2_
	.addrsig_sym _ZSt12__niter_baseIPiET_S1_
	.addrsig_sym _ZNSt11__copy_moveILb0ELb1ESt26random_access_iterator_tagE8__copy_mIiEEPT_PKS3_S6_S4_
	.addrsig_sym _ZNKSt16initializer_listIiE4sizeEv
	.addrsig_sym _ZNSt12_Vector_baseIiSaIiEE13_M_deallocateEPim
	.addrsig_sym _ZNSt16allocator_traitsISaIiEE10deallocateERS0_Pim
	.addrsig_sym _ZN9__gnu_cxx13new_allocatorIiE10deallocateEPim
	.addrsig_sym _ZdlPv
	.addrsig_sym _ZSt8_DestroyIPiiEvT_S1_RSaIT0_E
	.addrsig_sym _ZSt8_DestroyIPiEvT_S1_
	.addrsig_sym _ZNSt12_Destroy_auxILb1EE9__destroyIPiEEvT_S3_
	.addrsig_sym _ZNSt6vectorIiSaIiEE17_M_default_appendEm
	.addrsig_sym _ZNSt6vectorIiSaIiEE15_M_erase_at_endEPi
	.addrsig_sym _ZNKSt6vectorIiSaIiEE8max_sizeEv
	.addrsig_sym _ZSt27__uninitialized_default_n_aIPimiET_S1_T0_RSaIT1_E
	.addrsig_sym _ZNKSt6vectorIiSaIiEE12_M_check_lenEmPKc
	.addrsig_sym __cxa_rethrow
	.addrsig_sym __cxa_end_catch
	.addrsig_sym _ZNSt6vectorIiSaIiEE11_S_relocateEPiS2_S2_RS0_
	.addrsig_sym _ZNKSt12_Vector_baseIiSaIiEE19_M_get_Tp_allocatorEv
	.addrsig_sym _ZSt25__uninitialized_default_nIPimET_S1_T0_
	.addrsig_sym _ZNSt27__uninitialized_default_n_1ILb1EE18__uninit_default_nIPimEET_S3_T0_
	.addrsig_sym _ZSt6fill_nIPimiET_S1_T0_RKT1_
	.addrsig_sym _ZSt10__fill_n_aIPimiEN9__gnu_cxx11__enable_ifIXsr11__is_scalarIT1_EE7__valueET_E6__typeES4_T0_RKS3_
	.addrsig_sym _ZSt3maxImERKT_S2_S2_
	.addrsig_sym _ZNSt6vectorIiSaIiEE14_S_do_relocateEPiS2_S2_RS0_St17integral_constantIbLb1EE
	.addrsig_sym _ZSt12__relocate_aIPiS0_SaIiEET0_T_S3_S2_RT1_
	.addrsig_sym _ZSt14__relocate_a_1IiiENSt9enable_ifIXsr3std24__is_bitwise_relocatableIT_EE5valueEPS1_E4typeES2_S2_S2_RSaIT0_E
	.addrsig_sym _ZNK9__gnu_cxx17__normal_iteratorIPiSt6vectorIiSaIiEEE4baseEv
	.addrsig_sym _GLOBAL__sub_I_vector_t.cpp
	.addrsig_sym _Unwind_Resume
	.addrsig_sym _ZStL8__ioinit
	.addrsig_sym __dso_handle
	.addrsig_sym .Lconstinit
	.addrsig_sym _ZSt4cout
