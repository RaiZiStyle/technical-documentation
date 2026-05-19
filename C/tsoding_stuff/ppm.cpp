#include <stdio.h>
#include <math.h>

#define FACTOR 60
#define OUTPUT_NAMEFILE "ppm_output/output-%02d.ppm"

#define DEFAULT_PPM 1
// #define SHADER_PPM 1

// How to feed the output to ffmpeg : ffmpeg -i ppm_output/output-%02d.ppm -r 60 output.mp4

#ifdef SHADER_PPM
struct vec4 {
    float x, y, z, w;
    vec4(float x = 0, float y = 0, float z = 0, float w = 0):
        x(x), y(y), z(z), w(w)
    {}
};

struct vec2 {
    float x, y;
    vec2(float x = 0, float y = 0):
        x(x), y(y)
    {}
    vec2 yx() const { return vec2(y, x); }
    vec4 xyyx() const { return vec4(x, y, y, x); }
};

vec2 operator *(const vec2 &a, float s) { return vec2(a.x*s, a.y*s); }
vec2 operator +(const vec2 &a, float s) { return vec2(a.x+s, a.y+s); }
vec2 operator *(float s, const vec2 &a) { return a*s; }
vec2 operator -(const vec2 &a, const vec2 &b) { return vec2(a.x-b.x, a.y-b.y); }
vec2 operator +(const vec2 &a, const vec2 &b) { return vec2(a.x+b.x, a.y+b.y); }
vec2 operator *(const vec2 &a, const vec2 &b) { return vec2(a.x*b.x, a.y*b.y); }
vec2 operator /(const vec2 &a, float s) { return vec2(a.x/s, a.y/s); }
float dot(const vec2 &a, const vec2 &b) { return a.x*b.x + a.y*b.y; }
vec2 abs(const vec2 &a) { return vec2(fabsf(a.x), fabsf(a.y)); } 
vec2 &operator +=(vec2 &a, const vec2 &b) { a = a + b; return a; }
vec2 &operator +=(vec2 &a, float s) { a = a + s; return a; }
vec2 cos(const vec2 &a) { return vec2(cosf(a.x), cosf(a.y)); } 
vec4 sin(const vec4 &a) { return vec4(sinf(a.x), sinf(a.y), sinf(a.z), sinf(a.w)); } 
vec4 exp(const vec4 &a) { return vec4(expf(a.x), expf(a.y), expf(a.z), expf(a.w)); } 
vec4 tanh(const vec4 &a) { return vec4(tanhf(a.x), tanhf(a.y), tanhf(a.z), tanhf(a.w)); } 
vec4 operator +(const vec4 &a, float s) { return vec4(a.x+s, a.y+s, a.z+s, a.w+s); }
vec4 operator *(const vec4 &a, float s) { return vec4(a.x*s, a.y*s, a.z*s, a.w*s); }
vec4 operator *(float s, const vec4 &a) { return a*s; }
vec4 operator +(const vec4 &a, const vec4 &b) { return vec4(a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w); }
vec4 &operator +=(vec4 &a, const vec4 &b) { a = a + b; return a; }
vec4 operator -(float s, const vec4 &a) { return vec4(s-a.x, s-a.y, s-a.z, s-a.w); }
vec4 operator /(const vec4 &a, const vec4 &b) { return vec4(a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w); }
#endif


int main(int argc, char const* argv[]) {
    char buff[256] = {0};
    for (int i = 0; i < 60; i++) {
        snprintf(buff, sizeof(buff), OUTPUT_NAMEFILE, i);
        const char* output_path = buff;
        FILE* f = fopen(output_path, "wb");
        int w = 16 * FACTOR;
        int h = 9 * FACTOR;
#ifdef SHADER_PPM
        vec2 r = {(float)w, (float)h};
        float t = ((float)i/240)*2*M_PI;
#endif        
        fprintf(f, "P6\n");
        fprintf(f, "%d %d\n", w, h);
        fprintf(f, "255\n");
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
#ifdef DEFAULT_PPM                
                // x/FACTOR is the collum of the cell, y/FACTOR is the row of the cell,sum it you have an index
                if (((x+i) / 60 + (y+i) / 60) % 2) {
                    fputc(0xFF, f);
                    fputc(0xFF, f);
                    fputc(0xFF, f);
                } else {
                    fputc(0x00, f);
                    fputc(0x00, f);
                    fputc(0x00, f);
                }
#elif SHADER_PPM
                vec4 o;
                vec2 FC = {(float)x, (float)y};
                //////////////////////////////
                // https://x.com/XorDev/status/1894123951401378051
                // vec2 p=(FC*2.-r)/r.y,l,i,v=p*(l+=4.-4.*abs(.7-dot(p,p)));

                // vec2 FC = {(float)x,(float)y};
                vec2 p=(FC*2.-r)/r.y,l,i,v=p*(l+=4.-4.*abs(.7-dot(p,p)));
                for(;i.y++<8.;o+=(sin(v.xyyx())+1.)*abs(v.x-v.y))v+=cos(v.yx()*i.y+i+t)/i.y+.7;
                o=tanh(5.*exp(l.x-4.-p.y*vec4(-1,1,2,0))/o);
                //////////////////////////////
                fputc(o.x*255, f);
                fputc(o.y*255, f);
                fputc(o.z*255, f);
#endif
            }
        }
        fclose(f);
        printf("Generated %s\n", output_path);
    }
    return 0;
}
