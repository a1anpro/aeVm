#include "aeString.h"
#include "aeList.h"
#include "aeUtils.h"
#include "asmTest.h"
#include "vmTest.h"
#include "aeMap.h"
#include "asm.h"

#include "raylib.h"


 void
 clearScreen() {
     ClearBackground(RAYWHITE);
 }

 void
 drawPoint(int x, int y) {
     const int pixel = 10;
     DrawRectangle(x * pixel, y * pixel, pixel, pixel, RED);
 }

 void
 render(void) {
     clearScreen();
 //     DrawText("hello", 50, 10, 20, BLACK);

     //
     aeList output = vmTestDraw();
    
     // 显存
     aeList screen = aeListCut(output, 156, 256);
     size_t i;
     for (i = 0; i < 100; ++i) {
         size_t x = i % 10;
         size_t y = i / 10;
        
         int data = aeListGetItem(screen, i);
         if (data != 0) {
             drawPoint(x, y);
         }
     }
 }


 int
 main(void) {
     const int width = 100;
     const int height = 100;
     InitWindow(width, height, "axe3");
     SetTargetFPS(60);
    
     while (!WindowShouldClose()) {
         BeginDrawing();
             render();
         EndDrawing();
     }
     CloseWindow();
     return 0;
 }


//int main() {
//    // asmTest();
//    vmTest();
//
//    return 0;
//}
