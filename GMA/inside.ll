; ModuleID = 'inside.c'
source_filename = "inside.c"
target datalayout = "e-m:e-p:32:32-i1:8:32-i8:8:32-i16:16:32-i32:32:32-f32:32:32-i64:32-f64:32-a:0:32-n32"
target triple = "mag"

; Function Attrs: noinline nounwind optnone
define dso_local i32 @checkPixel(i32 noundef %ij, i32 noundef %i_1j, i32 noundef %ij_1, i32 noundef %i1j, i32 noundef %ij1, i32 noundef %speed) #0 {
entry:
  %ij.addr = alloca i32, align 4
  %i_1j.addr = alloca i32, align 4
  %ij_1.addr = alloca i32, align 4
  %i1j.addr = alloca i32, align 4
  %ij1.addr = alloca i32, align 4
  %speed.addr = alloca i32, align 4
  store i32 %ij, i32* %ij.addr, align 4
  store i32 %i_1j, i32* %i_1j.addr, align 4
  store i32 %ij_1, i32* %ij_1.addr, align 4
  store i32 %i1j, i32* %i1j.addr, align 4
  store i32 %ij1, i32* %ij1.addr, align 4
  store i32 %speed, i32* %speed.addr, align 4
  %0 = load i32, i32* %ij.addr, align 4
  %1 = load i32, i32* %i_1j.addr, align 4
  %2 = load i32, i32* %ij_1.addr, align 4
  %add = add nsw i32 %1, %2
  %3 = load i32, i32* %i1j.addr, align 4
  %add1 = add nsw i32 %add, %3
  %4 = load i32, i32* %ij1.addr, align 4
  %add2 = add nsw i32 %add1, %4
  %5 = load i32, i32* %ij.addr, align 4
  %mul = mul nsw i32 4, %5
  %sub = sub nsw i32 %add2, %mul
  %6 = load i32, i32* %speed.addr, align 4
  %div = sdiv i32 %sub, %6
  %add3 = add nsw i32 %0, %div
  ret i32 %add3
}

; Function Attrs: noinline nounwind optnone
define dso_local i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  %call = call i32 @checkPixel(i32 noundef 0, i32 noundef 1, i32 noundef 4, i32 noundef 3, i32 noundef 2, i32 noundef 6)
  ret i32 0
}

attributes #0 = { noinline nounwind optnone "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"frame-pointer", i32 2}
!2 = !{!"clang version 15.0.0 (https://github.com/MarkGoncharovAl/llvm-project d481c6a639cc05184087c6d370da0d4687839cd8)"}
