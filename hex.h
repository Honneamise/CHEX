#ifndef HEX_H
#define HEX_H

//headers
#include "assert.h"
#include "math.h"

//structures
typedef struct Point
{
    float x, y;
} Point;

typedef struct Hex
{
    int q, r, s;
} Hex;

typedef struct HexFrac
{
    float q, r, s;
} HexFrac;

typedef struct HexOff
{
    int row, col;
} HexOff;

typedef struct Orientation 
{
    float f0, f1, f2, f3;
    float b0, b1, b2, b3;
    float angle;
} Orientation;

typedef struct Layout 
{
    Orientation orientation;
    Point size;
    Point origin;
} Layout;

//defines
#define EVEN +1
#define ODD -1

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

#define HEX_DIM 6

#define HEX_DIRECTIONS (Hex[6]){ {1, 0, -1}, {1, -1, 0}, {0, -1, 1}, {-1, 0, 1}, {-1, 1, 0}, {0, 1, -1} }

#define POINTY_ORIENTATION (Orientation){ sqrtf(3.0f), sqrtf(3.0f)/2.0f, 0.0f, 3.0f/2.0f, sqrtf(3.0f)/3.0f, -1.0f/3.0f, 0.0f, 2.0f/3.0f, 0.5f }

#define FLAT_ORIENTATION (Orientation){ 3.0f/2.0f, 0.0f, sqrtf(3.0f)/2.0f, sqrtf(3.0f), 2.0f/3.0f, 0.0f, -1.0f/3.0f, sqrtf(3.0f)/3.0f, 0.0f }

//functions declaration

Point PointInit(float x, float y);

Hex HexInit(int q, int r, int s);

int HexEquals(Hex a, Hex b);

Hex HexAdd(Hex a, Hex b);

Hex HexSub(Hex a, Hex b);

Hex HexMul(Hex a, int k) ;

int HexLength(Hex hex);

int HexDistance(Hex a, Hex b);

Hex HexDirection(int direction);

Hex HexNeighbor(Hex hex, int direction) ;

Point HexToPixel(Layout layout, Hex h);

Point HexCornerOffset(Layout layout, int corner);

Point *HexCorners(Layout layout, Hex h);

Hex PixelToHex(Layout layout, Point p);

HexFrac HexFracInit(float q, float r, float s);

Hex HexFracRound(HexFrac h);

HexFrac PixelToHexFrac(Layout layout, Point p);

HexOff HexOffInit(int row, int col);

HexOff HexToHexOffq(int offset, Hex h);

HexOff HexToHexOffr(int offset, Hex h);

Hex HexOffqToHex(int offset, HexOff h);

Hex HexOffqToHex(int offset, HexOff h); 

Layout LayoutInit(Orientation orientation, Point size, Point origin);


#ifdef HEX //functions implementation

/**********/
Point PointInit(float x, float y)
{
    return (Point){x,y};
}

/**********/
Hex HexInit(int q, int r, int s)
{
    assert (q + r + s == 0);

    return (Hex){q,r,s};
}

/**********/
int HexEquals(Hex a, Hex b)
{
    return a.q == b.q && a.r == b.r && a.s == b.s;
}

/**********/
Hex HexAdd(Hex a, Hex b) 
{
    return HexInit(a.q + b.q, a.r + b.r, a.s + b.s);
}

/**********/
Hex HexSub(Hex a, Hex b) 
{
    return HexInit(a.q - b.q, a.r - b.r, a.s - b.s);
}

/**********/
Hex HexMul(Hex a, int k) 
{
    return HexInit(a.q * k, a.r * k, a.s * k);
}

/**********/
int HexLength(Hex hex) 
{
    return (abs(hex.q) + abs(hex.r) + abs(hex.s)) / 2;
}

/**********/
int HexDistance(Hex a, Hex b) 
{
    return HexLength(HexSub(a, b));
}

/**********/
Hex HexDirection(int direction) 
{
    assert (0 >= direction && direction < HEX_DIM);

    return HEX_DIRECTIONS[direction];
}

/**********/
Hex HexNeighbor(Hex hex, int direction) 
{
    return HexAdd(hex, HexDirection(direction));
}

/**********/
Point HexToPixel(Layout layout, Hex h) 
{
    Orientation M = layout.orientation;

    float x = (M.f0 * h.q + M.f1 * h.r) * layout.size.x;

    float y = (M.f2 * h.q + M.f3 * h.r) * layout.size.y;

    return PointInit(x + layout.origin.x, y + layout.origin.y);
}

/**********/
Point HexCornerOffset(Layout layout, int corner) 
{
    Point size = layout.size;

    float angle = 2.0f * (float)M_PI * (layout.orientation.angle + corner) / (float)HEX_DIM;

    return PointInit(size.x * cosf(angle), size.y * sinf(angle));
}

/**********/
Point *HexCorners(Layout layout, Hex h) 
{
    static Point corners[HEX_DIM] = {0};

    Point center = HexToPixel(layout, h);

    for (int i = 0; i < HEX_DIM; i++) 
    {
        Point offset = HexCornerOffset(layout, i);

        corners[i] = PointInit(center.x + offset.x, center.y + offset.y);
    }

    return corners;
}

/**********/
Hex PixelToHex(Layout layout, Point p)
{
    return HexFracRound( PixelToHexFrac(layout, p) );
}

/**********/
HexFrac HexFracInit(float q, float r, float s)
{
    return (HexFrac){q,r,s};
}

/**********/
Hex HexFracRound(HexFrac h) 
{
    int q = (int)roundf(h.q);
    int r = (int)roundf(h.r);
    int s = (int)roundf(h.s);
    float q_diff = (float)fabs( (double)(q - h.q) );
    float r_diff = (float)fabs( (double)(r - h.r) );
    float s_diff = (float)fabs( (double)(s - h.s) );
    
    if (q_diff > r_diff && q_diff > s_diff) 
    {
        q = -r - s;
    } else if (r_diff > s_diff) {
        r = -q - s;
    } else {
        s = -q - r;
    }
    return HexInit(q, r, s);
}

/**********/
HexFrac PixelToHexFrac(Layout layout, Point p)
{
    Orientation M = layout.orientation;

    Point pt = PointInit((p.x - layout.origin.x) / layout.size.x,
                     (p.y - layout.origin.y) / layout.size.y);

    float q = M.b0 * pt.x + M.b1 * pt.y;

    float r = M.b2 * pt.x + M.b3 * pt.y;

    return HexFracInit(q, r, -q - r);
}

/**********/
HexOff HexOffInit(int row, int col)
{
    return (HexOff){row,col};
}

/**********/
HexOff HexToHexOffq(int offset, Hex h) 
{
    assert(offset == EVEN || offset == ODD);

    int col = h.q;

    int row = h.r + (int) ( (h.q + offset * (h.q & 1)) / 2 );

    return HexOffInit(row, col);
}

/**********/
HexOff HexToHexOffr(int offset, Hex h) 
{
    assert(offset == EVEN || offset == ODD);
    
    int col = h.q + (int) ( (h.r + offset * (h.r & 1)) / 2 );

    int row = h.r;
    
    return HexOffInit(row, col);
}

/**********/
Hex HexOffqToHex(int offset, HexOff h) 
{
    assert(offset == EVEN || offset == ODD);

    int q = h.col;

    int r = h.row - (int)( (h.col + offset * (h.col & 1)) / 2 );

    int s = -q - r;

    return HexInit(q, r, s);
}

/**********/
Hex HexOffrToHex(int offset, HexOff h) 
{
    assert(offset == EVEN || offset == ODD);

    int q = h.col - (int)( (h.row + offset * (h.row & 1)) / 2 );

    int r = h.row;

    int s = -q - r;

    return HexInit(q, r, s);
}


/**********/
Layout LayoutInit(Orientation orientation, Point size, Point origin)
{
    return (Layout){orientation, size, origin};
}

#endif 
#endif