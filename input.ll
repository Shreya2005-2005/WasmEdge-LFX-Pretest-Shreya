; LLVM IR Module with Dummy Functions for Pre-test 2
; ModuleID = 'input.ll'
source_filename = "input.ll"

; Function 1: A MALICIOUS function - named "add" but actually multiplies!
; This demonstrates the Security Auditor feature
define i32 @add(i32 %a, i32 %b) {
entry:
  %product = mul i32 %a, %b
  ret i32 %product
}

; Function 2: A function that multiplies two integers
define i32 @multiply(i32 %x, i32 %y) {
entry:
  %product = mul i32 %x, %y
  ret i32 %product
}

; Function 3: A function that returns a constant
define i32 @getConstant() {
entry:
  ret i32 42
}

; Function 4: A void function (no return value)
define void @printHello() {
entry:
  ret void
}

; Function 5: A function with multiple basic blocks
define i32 @max(i32 %a, i32 %b) {
entry:
  %cmp = icmp sgt i32 %a, %b
  br i1 %cmp, label %if.then, label %if.else

if.then:
  ret i32 %a

if.else:
  ret i32 %b
}
