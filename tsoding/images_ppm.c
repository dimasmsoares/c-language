#include <stdio.h>

int main(){

    const char *output_path = "output.ppm";
    FILE *f = fopen(output_path, "wb");
    int w = 16*60;
    int h = 9*60;
    fprintf(f, "P6\n");
    fprintf(f, "%d %d\n", w, h);
    fprintf(f, "255");

    for(int y = 0; y < h; ++y){
        for(int x = 0; x < w; ++x){
            fputc(0xFF, f);
            fputc(0x00, f);
            fputc(0x00, f);
        }
    }
    
    fclose(f);
    printf("Generated %s\n", output_path);
    return 0;
}