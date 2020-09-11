#include "aeString.h"
#include "aeList.h"
#include "aeUtils.h"
#include "asmTest.h"
#include "vmTest.h"
#include "aeMap.h"
#include "asm.h"
// #include "raylib.h"


int main() {
   // asmTest();
   vmTest();
   
   return 0;
}


// void
// clearScreen() {
//     ClearBackground(RAYWHITE);
// }

// void
// drawPoint(int x, int y) {
//     const int pixel = 2;
//     DrawRectangle(x * pixel, y * pixel, pixel, pixel, RED);
// }

// void
// render(void) {
//     clearScreen();
//     // 屏幕有200/5=40行 40列
// //    drawPoint(0, 0);
// //     DrawText("hello", 50, 10, 20, BLACK);
    
//     // 画屏幕，数组有100个点
// //    aeList screen = utilGetListByLen(100);
// //    aeListSetItem(screen, 9, 100);
//     aeList output = vmTestDraw();
    
//     // 显存
//     aeList screen = aeListCut(output, 156, 256);
//     size_t i;
//     for (i = 0; i < 100; ++i) {
//         size_t x = i % 10;
//         size_t y = i / 10;
        
//         int data = aeListGetItem(screen, i);
//         if (data != 0) {
//             drawPoint(x, y);
//         }
//     }
// }


// int
// main(void) {
//     const int width = 100;
//     const int height = 100;
//     InitWindow(width, height, "hello");
//     SetTargetFPS(60);
    
//     while (!WindowShouldClose()) {
//         BeginDrawing();
//             render();
//         EndDrawing();
//     }
//     CloseWindow();
//     return 0;
// }
