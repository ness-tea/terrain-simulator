#include <stdio.h>
#include <stdlib.h>
#include <math.h>                  // for mathematical operations in circle algorithm
#include <time.h>                  // for srand()
#include <vector>

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif


// G L O B A L   C O N S T A N T S

// keeps track of terrain states
float colour[] = {0.5, 0.5, 0.5};
GLenum renderMode = GL_QUAD_STRIP;

int lightTracker = 0;
bool turnOnLight = false;
bool light2 = false;

bool shade_FLAT = true;
bool shade_GOURAUD = false;

int renderCount = 0;
bool solid = true;
bool wire = false;
bool solid_wire = false;

bool reset = false;

int rotateX = 0;

// User interactivity with terrain
bool bump = false;
bool dent = false;
int userX;
int userY;

// Used by the Circle algorithm          
float pd;                         
float rd;                      

// Used by drawTerrain functions
int xtemp0, ztemp0;               // for storing random coordinates in Circle algorithm
int count0 = 30;                  // iteration count for Circle algorithm   
int circSize0 = 15;                // circle size for Circle algorithm 
float disp0 = 3;                  // maximum height variation for Circle algorithm   
int c0 = 0;

int xtemp1, ztemp1;
int count1 = 80;
int circSize1 = 25;
float disp1 = 5;     
int c1 = 0;

int xtemp2, ztemp2;
int count2 = 95;
int circSize2 = 35;
float disp2 = 6;  
int c2 = 0;

int xtemp3, ztemp3;
int count3 = 97;
int circSize3 = 45;
float disp3 = 8; 
int c3 = 0;

int xtemp4, ztemp4;
int count4 = 105;
int circSize4 = 55;
float disp4 = 9; 
int c4 = 0;

int xtemp5, ztemp5;
int count5 = 115;
int circSize5 = 60;
float disp5 = 12; 
int c5 = 0;

// keeps track of which terrain size is drawn
bool size50 = true; 
bool size100 = false; 
bool size150 = false; 
bool size200 = false; 
bool size250 = false; 
bool size300 = false;     

// no. of vertices in each sized-terrain /w arrays for setting aside memory
const int numOfVerts0 = 50;
const int numOfVerts1 = 100;
const int numOfVerts2 = 150;
const int numOfVerts3 = 200;
const int numOfVerts4 = 250;
const int numOfVerts5 = 300;

float** verts0 = new float*[numOfVerts0];
float** verts1 = new float*[numOfVerts1];
float** verts2 = new float*[numOfVerts2];
float** verts3 = new float*[numOfVerts3];
float** verts4 = new float*[numOfVerts4];
float** verts5 = new float*[numOfVerts5];

// for creating custom terrain
float** create0 = new float*[numOfVerts0];
float** create1 = new float*[numOfVerts1];
float** create2 = new float*[numOfVerts2];
float** create3 = new float*[numOfVerts3];
float** create4 = new float*[numOfVerts4];
float** create5 = new float*[numOfVerts5];

// stores surface normals for QUAD and TRI terrains
float vec3[] ={0,0,0};

// Stores info for LIGHTING
float pos0[4] = {-150, 150, 150, 1};
float amb0[4] = {colour[0]*0.2f, colour[1]*0.2f, colour[2]*0.2f, 1};
float dif0[4] = {colour[0]*0.8f, colour[1]*0.8f, colour[2]*0.8f, 1};
float spc0[4] = {colour[0]*0.2f, colour[1]*0.2f, colour[2]*0.2f, 1};

float pos1[4] = {150, 150, -150, 1};
float amb1[4] = {colour[0]*0.3f, colour[1]*0.3f, colour[2]*0.3f, 1};
float dif1[4] = {colour[0]*0.7f, colour[1]*0.7f, colour[2]*0.7f, 1};
float spc1[4] = {colour[0]*0.5f, colour[1]*0.5f, colour[2]*0.5f, 1};

// camera position and camera target
// For 50 X 50 Terrain
float camPos0[] = {75, 60, 75};
float camTarget0[] = {25, 0, 25};
float angle0 = 0.0;
float hyp0 = 70.71;

// For 100 X 100 Terrain
float camPos1[] = {150, 100, 150};
float camTarget1[] = {50, 0, 50};
float angle1 = 0.0;
float hyp1 = 141.4;

// For 150 X 150 Terrain
float camPos2[] = {220, 150, 220};
float camTarget2[] = {75, 0, 75};
float angle2 = 0.0;
float hyp2 = 205.1;

// For 200 X 200 Terrain
float camPos3[] = {300, 200, 300};
float camTarget3[] = {100, 0, 100}; 
float angle3 = 0.0;
float hyp3 = 282.8;

// For 250 X 250 Terrain
float camPos4[] = {380, 280, 380};
float camTarget4[] = {125, 0, 125};
float angle4 = 0.0;
float hyp4 = 360.6;

// For 300 X 300 Terrain
float camPos5[] = {450, 360, 450};
float camTarget5[] = {150, 0, 150};
float angle5 = 0.0;
float hyp5 = 424.3;

// For the 'Create a Terrain' option
bool terrain50 = false;
bool terrain100 = false;
bool terrain150 = false;
bool terrain200 = false;
bool terrain250 = false;
bool terrain300 = false;

// C I R C L E   A L G O R I T H M 

// function for circle algorithm, used for heightmap calculation
void circAlg(int nv, float** v, int circSize, int disp, int xC, int zC) {

// --------- ITERATE CIRC ALG IN DRAW TERRAIN INSTEAD ---------------
    xC = rand() % (nv - 1) + 1;
    zC = rand() % (nv - 1) + 1;
    for (int x = 0; x < nv; ++x) {
        for (int z = 0; z < nv; ++z) {
            rd = sqrt(pow(xC - x, 2) + pow(zC - z, 2));
            pd = rd*2.0/circSize;
            if (fabs(pd) <= 1.0) {
                v[x][z] += disp/2.0 + cos(3.14*pd)*disp/2.0;
            }
        }
    }
}

// USER-DEFINED version 1 - bump up according to user mouse click
void bumpUp(int nv, float** v, int circSize, int disp, int xC, int zC) {
    for (int x = 0; x < nv; ++x) {
        for (int z = 0; z < nv; ++z) {
            rd = sqrt(pow(xC - x, 2) + pow(zC - z, 2));
            pd = rd*2.0/circSize;
            if (fabs(pd) <= 1.0) {
                v[x][z] += disp/2.0 + cos(3.14*pd)*disp/2.0;
            }
        }
    }
}

// USER-DEFINED version 2 - dent down according to user mouse click
void dentDown(int nv, float** v, int circSize, int disp, int xC, int zC) {
    for (int x = 0; x < nv; ++x) {
        for (int z = 0; z < nv; ++z) {
            rd = sqrt(pow(xC - x, 2) + pow(zC - z, 2));
            pd = rd*2.0/circSize;
            if (fabs(pd) <= 1.0) {
                v[x][z] -= disp/2.0 + cos(3.14*pd)*disp/2.0;
            }
        }
    }
}


// K E Y B O A R D   F U N C T I O N S
void keyboard(unsigned char key, int xIn, int yIn) {
    
    switch (key) {
        // closes the program window
        case 'q':
        case 27:
            printf("\n\n\nThank you for using the I N T E R A C T I V E   T E R R A I N  2 0 0 0 !\n\n\n");
            exit(0);
            break;

        // toggle between wireframe modes
        case 'w':
            renderCount += 1;
            if (renderCount == 1) {
                solid = false;
                // solid_wire = false;
                wire = true;
                printf("WIREFRAME mode on\n");
            }
            if (renderCount == 2) {
                solid = true;
                wire = true;
                // solid_wire = true;
                printf("SOLID & WIREFRAME mode on\n");
            }
            if (renderCount == 3) {
                // solid_wire = ;
                wire = false;
                solid = true;
                renderCount = 0;
                printf("SOLID mode on (Default)\n");
            }
            break;

        case 't':
            renderMode = GL_TRIANGLE_STRIP;
            printf("TRIANGLE STRIP mode on\n");
            break;
        case 'y':
            renderMode = GL_QUAD_STRIP;
            printf("QUAD STRIP mode on\n");
            break;

        // resets the terrain
        case 'r':
            reset = true;
            printf("RESET Terrain\n");
            break;
        
        // turns on lighting
        case 'l':
            lightTracker += 1;
            if (lightTracker == 1) {
                turnOnLight = true;
                printf("Lighting ENABLED\n");
            }
            if (lightTracker == 2) {
                light2 = true;
                printf("Light DISABLED\n");
            }
            if (lightTracker == 3) {
                turnOnLight = false;
                light2 = false;
                lightTracker = 0;
            }
            break;
        
        // moving Light 1 along - x-axis
        case 'o':
            if (turnOnLight == true) {
                pos0[0] -= 5;
            }
            break;

        // moving Light 1 along + x-axis
        case 'p':
            if (turnOnLight == true) {
                pos0[0] += 5;
            }
            break;
        
        // moving Light 2 along - x axis
        case 'u':
            if (light2 == true) {
                pos1[0] -= 5;
            }
            break;

        // moving Light 2 along + x-axis
        case 'i':
            if (light2 == true) {
                pos1[0] += 5;
            }
            break;
    }
}
    
void special(int key, int xIn, int yIn) {
    
    switch (key) {

        // for rotating the terrain
        case GLUT_KEY_UP:
            if (rotateX < 100) {
                if (size50 == true || terrain50 == true) {
                    camPos0[1] += 1;
                    camPos0[2] -= 1;
                }
                if (size100 == true || terrain100 == true) {
                    camPos1[1] += 1;
                    camPos1[2] -= 1;
                }
                if (size150 == true || terrain150 == true) {
                    camPos2[1] += 1;
                    camPos2[2] -= 1;
                }
                if (size200 == true || terrain200 == true) {
                    camPos3[1] += 1;
                    camPos3[2] -= 1;
                }
                if (size250 == true || terrain250 == true) {
                    camPos4[1] += 1;
                    camPos4[2] -= 1;
                }
                if (size300 == true || terrain300 == true) {
                    camPos5[1] += 1;
                    camPos5[2] -= 1;
                }
            }
            rotateX += 1;
            break;
        case GLUT_KEY_DOWN:
            if (rotateX > -50) {
                if (size50 == true || terrain50 == true) {
                    camPos0[1] -= 1;
                    camPos0[2] += 1;
                }
                if (size100 == true || terrain100 == true) {
                    camPos1[1] -= 1;
                    camPos1[2] += 1;
                }
                if (size150 == true || terrain150 == true) {
                    camPos2[1] -= 1;
                    camPos2[2] += 1;
                }
                if (size200 == true || terrain200 == true) {
                    camPos3[1] -= 1;
                    camPos3[2] += 1;
                }
                if (size250 == true || terrain250 == true) {
                    camPos4[1] -= 1;
                    camPos4[2] += 1;
                }
                if (size300 == true || terrain300 == true) {
                    camPos5[1] -= 1;
                    camPos5[2] += 1;
                }
            }
            rotateX -= 1;
            break;
        case GLUT_KEY_LEFT:
            if (size50 == true || terrain50 == true) {
                angle0 += 0.02f;
                camPos0[0] = hyp0*cos(angle0);
                camPos0[2] = hyp0*sin(angle0);
            }
            if (size100 == true || terrain100 == true) {
                angle1 += 0.02f;
                camPos1[0] = hyp1*cos(angle1);
                camPos1[2] = hyp1*sin(angle1);
            }
            if (size150 == true || terrain150 == true) {
                angle2 += 0.02f;
                camPos2[0] = hyp2*cos(angle2);
                camPos2[2] = hyp2*sin(angle2);
            }
            if (size200 == true || terrain200 == true) {
                angle3 += 0.02f;
                camPos3[0] = hyp3*cos(angle3);
                camPos3[2] = hyp3*sin(angle3);
            }
            if (size250 == true || terrain250 == true) {
                angle4 += 0.02f;
                camPos4[0] = hyp4*cos(angle4);
                camPos4[2] = hyp4*sin(angle4);
            }
            if (size300 == true || terrain300 == true) {
                angle5 += 0.02f;
                camPos5[0] = hyp5*cos(angle5);
                camPos5[2] = hyp5*sin(angle5);
            }
            break;
        case GLUT_KEY_RIGHT:
            if (size50 == true || terrain50 == true) {
                angle0 -= 0.02f;
                camPos0[0] = hyp0*cos(angle0);
                camPos0[2] = hyp0*sin(angle0);
            }
            if (size100 == true || terrain100 == true) {
                angle1 -= 0.02f;
                camPos1[0] = hyp1*cos(angle1);
                camPos1[2] = hyp1*sin(angle1);
            }
            if (size150 == true || terrain150 == true) {
                angle2 -= 0.02f;
                camPos2[0] = hyp2*cos(angle2);
                camPos2[2] = hyp2*sin(angle2);
            }
            if (size200 == true || terrain200 == true) {
                angle3 -= 0.02f;
                camPos3[0] = hyp3*cos(angle3);
                camPos3[2] = hyp3*sin(angle3);
            }
            if (size250 == true || terrain250 == true) {
                angle4 -= 0.02f;
                camPos4[0] = hyp4*cos(angle4);
                camPos4[2] = hyp4*sin(angle4);
            }
            if (size300 == true || terrain300 == true) {
                angle5 -= 0.02f;
                camPos5[0] = hyp5*cos(angle5);
                camPos5[2] = hyp5*sin(angle5);
            }
            break;
    }
}


// M O U S E   F U N C T I O N S - for user interactivity with terrain!
void mouse(int btn,  int state, int x, int y) {
    if (btn == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            userX = x;
            userY = y;
            bump = true;
        }
        if (state == GLUT_UP) {
            bump = false;
        }
    }
    if (btn == GLUT_RIGHT_BUTTON) {
        if (state == GLUT_DOWN) {
            userX = x;
            userY = y;
            dent = true;
        }
        if (state == GLUT_UP) {
            dent = false;
        }
    }
}


// I N I T I A L I Z A T I O N
void init(void) {
    
    glClearColor(0, 0, 0, 0.5);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1, 1, 2000);

    // setting aside memory and initializing arrays for vertices
    // (1) 50 X 50 TERRAIN
    for (int i = 0; i < numOfVerts0; ++i){
        verts0[i] = new float[numOfVerts0];
    }
    for (int x = 0; x < numOfVerts0; ++x){
        for (int z = 0; z < numOfVerts0; ++z){
            verts0[x][z] = 0;
        }
    }

    // (2) 100 X 100 TERRAIN
    for (int i = 0; i < numOfVerts1; ++i){
        verts1[i] = new float[numOfVerts1];
    }
    for (int x = 0; x < numOfVerts1; ++x){
        for (int z = 0; z < numOfVerts1; ++z){
            verts1[x][z] = 0;
        }
    }
    
    // (3) 150 X 150 TERRAIN
    for (int i = 0; i < numOfVerts2; ++i){
        verts2[i] = new float[numOfVerts2];
    }
    for (int x = 0; x < numOfVerts2; ++x){
        for (int z = 0; z < numOfVerts2; ++z){
            verts2[x][z] = 0;
        }
    }

    // (4) 200 X 200 TERRAIN
    for (int i = 0; i < numOfVerts3; ++i){
        verts3[i] = new float[numOfVerts3];
    }
    for (int x = 0; x < numOfVerts3; ++x){
        for (int z = 0; z < numOfVerts3; ++z){
            verts3[x][z] = 0;
        }
    }

    // (5) 250 X 250 TERRAIN
    for (int i = 0; i < numOfVerts4; ++i){
        verts4[i] = new float[numOfVerts4];
    }
    for (int x = 0; x < numOfVerts4; ++x){
        for (int z = 0; z < numOfVerts4; ++z){
            verts4[x][z] = 0;
        }
    }

    // (6) 300 X 300 TERRAIN
    for (int i = 0; i < numOfVerts5; ++i){
        verts5[i] = new float[numOfVerts5];
    }
    for (int x = 0; x < numOfVerts5; ++x){
        for (int z = 0; z < numOfVerts5; ++z){
            verts5[x][z] = 0;
        }
    }

    // initializing the terrains to be created
    // (1) 50 X 50 TERRAIN
    for (int i = 0; i < numOfVerts0; ++i){
        create0[i] = new float[numOfVerts0];
    }
    for (int x = 0; x < numOfVerts0; ++x){
        for (int z = 0; z < numOfVerts0; ++z){
            create0[x][z] = 0;
        }
    }
    // (2) 100 X 100 TERRAIN
    for (int i = 0; i < numOfVerts1; ++i){
        create1[i] = new float[numOfVerts1];
    }
    for (int x = 0; x < numOfVerts1; ++x){
        for (int z = 0; z < numOfVerts1; ++z){
            create1[x][z] = 0;
        }
    }
    // (3) 150 X 150 TERRAIN
    for (int i = 0; i < numOfVerts2; ++i){
        create2[i] = new float[numOfVerts2];
    }
    for (int x = 0; x < numOfVerts2; ++x){
        for (int z = 0; z < numOfVerts2; ++z){
            create2[x][z] = 0;
        }
    }
    // (4) 200 X 200 TERRAIN
    for (int i = 0; i < numOfVerts3; ++i){
        create3[i] = new float[numOfVerts3];
    }
    for (int x = 0; x < numOfVerts3; ++x){
        for (int z = 0; z < numOfVerts3; ++z){
            create3[x][z] = 0;
        }
    }
    // (5) 250 X 250 TERRAIN
    for (int i = 0; i < numOfVerts4; ++i){
        create4[i] = new float[numOfVerts4];
    }
    for (int x = 0; x < numOfVerts4; ++x){
        for (int z = 0; z < numOfVerts4; ++z){
            create4[x][z] = 0;
        }
    }
    // (1) 300 X 300 TERRAIN
    for (int i = 0; i < numOfVerts5; ++i){
        create5[i] = new float[numOfVerts5];
    }
    for (int x = 0; x < numOfVerts5; ++x){
        for (int z = 0; z < numOfVerts5; ++z){
            create5[x][z] = 0;
        }
    }
}


// N O R M A L I Z I N G - normalizes surface normals for correct lighting
void normalize(int x, int y, int z) {
    float length = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));

    vec3[0] /= length;
    vec3[1] /= length;
    vec3[2] /= length;

}


// S U R F A C E   N O R M A L S - function for calculating surface normals
void calculateSurfaceNormal(int x1, int y1, int z1, int x2, int y2, int z2, int x3, int y3, int z3) {
    int vec1[] = {0,0,0};
    int vec2[] = {0,0,0};

    vec1[0] = x2 - x1;
    vec1[1] = y2 - y1;
    vec1[2] = z2 - z1;
    vec2[0] = x3 - x1;
    vec2[1] = y3 - y1;
    vec2[2] = z3 - z1;

    vec3[0] = vec1[1]*vec2[2]-vec1[2]*vec2[1]; 
    vec3[1] = vec1[2]*vec2[0]-vec1[0]*vec2[2]; 
    vec3[2] = vec1[0]*vec2[1]-vec1[1]*vec2[0];

    normalize(vec3[0], vec3[1], vec3[2]);
    glNormal3fv(vec3);
}


// T E R R A I N   R E N D E R I N G - functions that draw the terrains
void drawTerrain(int nv, float** v, int disp) {

    for (int x = 0; x < (nv - 1); ++x) {
        for (int z = (nv - 1); z > 0; --z) {
            
            // renderMode is either GL_QUAD_STRIP or GL_TRIANGLE_STRIP, dependent on user's toggle
            glBegin(renderMode);             
                glColor3f(colour[0] + 0.1*v[x+1][z-1]/disp, colour[1] + 0.1*v[x+1][z-1]/disp, colour[2] + 0.1*v[x+1][z-1]/disp);
                glVertex3f(x+1, v[x+1][z-1], z-1);        
                
                glColor3f(colour[0] + 0.1*v[x][z-1]/disp, colour[1] + 0.1*v[x][z-1]/disp, colour[2] + 0.1*v[x][z-1]/disp);
                glVertex3f(x, v[x][z-1], z-1);

                calculateSurfaceNormal(x+1, v[x+1][z], z, x+1, v[x+1][z-1], z-1, x, v[x][z-1], z-1);   

                glColor3f(colour[0] + 0.1*v[x+1][z]/disp, colour[1] + 0.1*v[x+1][z]/disp, colour[2] + 0.1*v[x+1][z]/disp);
                glVertex3f(x+1, v[x+1][z], z);

                calculateSurfaceNormal(x, v[x][z], z, x+1, v[x+1][z], z, x, v[x][z-1], z-1);

                glColor3f(colour[0] + 0.1*v[x][z]/disp, colour[1] + 0.1*v[x][z]/disp, colour[2] + 0.1*v[x][z]/disp);
                glVertex3f(x, v[x][z], z);

            glEnd();
        }
    }
}


// R E S E T   F U N C T I ON - resets heightmap values of terrain
void resetTerrain(int nv, float** v) {
    for (int x = 0; x < nv; ++x){
        for (int z = 0; z < nv; ++z){
            v[x][z] = 0;
        }
    }
}


// D I S P L A Y
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    //--------------------- SHADING MODELS -----------------------
    if (shade_FLAT == true) {
        glShadeModel(GL_FLAT);
    }
    if (shade_GOURAUD == true) {
        glShadeModel(GL_SMOOTH);
    }

    //-------------- WIREFRAME/SOLID REPRESENTATIONS --------------------
    if (solid = true) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    if (wire == true) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if (solid_wire == true) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glEnable(GL_POLYGON_OFFSET_LINE);
        glPolygonOffset(1.0,1.0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        drawTerrain(numOfVerts0, verts0, disp0);
    }

    //---------------- LIGHTING FUNCTIONS ---------------------------
    // For enabling polygon culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_NORMALIZE);
  
    if (turnOnLight == true) {

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        
        glEnable(GL_LIGHT0);        // First light
        if (light2 == true) {
            glEnable(GL_LIGHT1);        // Second light
        }

        glLightfv(GL_LIGHT0, GL_POSITION, pos0);
        glLightfv(GL_LIGHT0, GL_AMBIENT, amb0);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, dif0);
        glLightfv(GL_LIGHT0, GL_SPECULAR, spc0);
        
        glLightfv(GL_LIGHT1, GL_POSITION, pos1);
        glLightfv(GL_LIGHT1, GL_AMBIENT, amb1);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, dif1);
        glLightfv(GL_LIGHT1, GL_SPECULAR, spc1);
    }

    if (turnOnLight == false) {
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHT1);
    }

    //---------------- DRAWING THE TERRAIN - Different sizes ----------------
    // (1) 50 X 50
    if (size50 == true) {

        // resets matrix and sets the camera position depending on terrain size
        glLoadIdentity();
        gluLookAt(camPos0[0], camPos0[1], camPos0[2], camTarget0[0], camTarget0[1], camTarget0[2], 0, 1, 0);
        
        // Iterates the Circle algorithm
        while (c0 < count0) {
            circAlg(numOfVerts0, verts0, circSize0, disp0, xtemp0, ztemp0);
            c0 += 1;
        }

        // Left-click on 2D overhead and create bump upwards
        if (bump == true) {
            bumpUp(numOfVerts0, verts0, 10, 1, userX, userY);
        }
        // Right-click on 2D overhead and create dent downwards
        if (dent == true) {
            dentDown(numOfVerts0, verts0, 10, 1, userX, userY);
        }

        // draws the terrain
        drawTerrain(numOfVerts0, verts0, disp0); 
    }

    // (2) 100 X 100
    if (size100 == true) {
        glLoadIdentity();
        gluLookAt(camPos1[0], camPos1[1], camPos1[2], camTarget1[0], camTarget1[1], camTarget1[2], 0, 1, 0);
        while (c1 < count1) {
            circAlg(numOfVerts1, verts1, circSize1, disp1, xtemp1, ztemp1);
            c1 += 1;
        }

        if (bump == true) {
            bumpUp(numOfVerts1, verts1, 20, 1, userX, userY);
        }
        if (dent == true) {
            dentDown(numOfVerts1, verts1, 20, 1, userX, userY);
        }

        drawTerrain(numOfVerts1, verts1, disp1);
    }

    // (3) 150 X 150 
    if (size150 == true) {
        glLoadIdentity();
        gluLookAt(camPos2[0], camPos2[1], camPos2[2], camTarget2[0], camTarget2[1], camTarget2[2], 0, 1, 0);
        while (c2 < count2) {
            circAlg(numOfVerts2, verts2, circSize2, disp2, xtemp2, ztemp2);
            c2 += 1;
        }

        if (bump == true) {
            bumpUp(numOfVerts2, verts2, 30, 2, userX, userY);
        }
        if (dent == true) {
            dentDown(numOfVerts2, verts2, 30, 1, userX, userY);
        }

        drawTerrain(numOfVerts2, verts2, disp2);
    }
 
    // (4) 200 X 200
    if (size200 == true) {
        glLoadIdentity();
        gluLookAt(camPos3[0], camPos3[1], camPos3[2], camTarget3[0], camTarget3[1], camTarget3[2], 0, 1, 0);
        while (c3 < count3) {
            circAlg(numOfVerts3, verts3, circSize3, disp3, xtemp3, ztemp3);
            c3 += 1;
        }

        if (bump == true) {
            bumpUp(numOfVerts3, verts3, 50, 5, userX, userY);
        }
        if (dent == true) {
            dentDown(numOfVerts3, verts3, 50, 5, userX, userY);
        }

        drawTerrain(numOfVerts3, verts3, disp3);
    }
    
    // (5) 250 X 250
    if (size250 == true) {
        glLoadIdentity();
        gluLookAt(camPos4[0], camPos4[1], camPos4[2], camTarget4[0], camTarget4[1], camTarget4[2], 0, 1, 0);
        while (c4 < count4) {
            circAlg(numOfVerts4, verts4, circSize4, disp4, xtemp4, ztemp4);
            c4 += 1;
        }

        if (bump == true) {
            bumpUp(numOfVerts4, verts4, 65, 7, userX, userY);
        }
        if (dent == true) {
            dentDown(numOfVerts4, verts4, 65, 7, userX, userY);
        }

        drawTerrain(numOfVerts4, verts4, disp4);
    }

    // (6) 300 X 300
    if (size300 == true) {
        glLoadIdentity();
        gluLookAt(camPos5[0], camPos5[1], camPos5[2], camTarget5[0], camTarget5[1], camTarget5[2], 0, 1, 0);
        while (c5 < count5) {
            circAlg(numOfVerts5, verts5, circSize5, disp5, xtemp5, ztemp5);
            c5 += 1;
        }

        if (bump == true) {
            bumpUp(numOfVerts5, verts5, 80, 9, userX, userY);
        }
        if (dent == true) {
            dentDown(numOfVerts5, verts5, 80, 9, userX, userY);
        }

        drawTerrain(numOfVerts5, verts5, disp5);
    }

    //------------- CREATE A TERRAIN FUNCTION ---------------------------
    if (terrain50 == true) {
        // resets matrix and sets the camera position depending on terrain size
        glLoadIdentity();
        gluLookAt(camPos0[0], camPos0[1], camPos0[2], camTarget0[0], camTarget0[1], camTarget0[2], 0, 1, 0);

        // Left-click on 2D overhead and create bump upwards
        if (bump == true) {
            bumpUp(numOfVerts0, create0, 10, 1, userX, userY);
        }
        // Right-click on 2D overhead and create dent downwards
        if (dent == true) {
            dentDown(numOfVerts0, create0, 10, 1, userX, userY);
        }

        // draws the terrain
        drawTerrain(numOfVerts0, create0, disp0); 
    }

    if (terrain100 == true) {
        // resets matrix and sets the camera position depending on terrain size
        glLoadIdentity();
        gluLookAt(camPos1[0], camPos1[1], camPos1[2], camTarget1[0], camTarget1[1], camTarget1[2], 0, 1, 0);

        // Left-click on 2D overhead and create bump upwards
        if (bump == true) {
            bumpUp(numOfVerts1, create1, 20, 2, userX, userY);
        }
        // Right-click on 2D overhead and create dent downwards
        if (dent == true) {
            dentDown(numOfVerts1, create1, 20, 2, userX, userY);
        }

        // draws the terrain
        drawTerrain(numOfVerts1, create1, disp1); 
    }

    if (terrain150 == true) {
        glLoadIdentity();
        gluLookAt(camPos2[0], camPos2[1], camPos2[2], camTarget2[0], camTarget2[1], camTarget2[2], 0, 1, 0);

        if (bump == true) {
            bumpUp(numOfVerts2, create2, 30, 2, userX, userY);
        }
        if (dent == true) {
            dentDown(numOfVerts2, create2, 30, 1, userX, userY);
        }

        drawTerrain(numOfVerts2, create2, disp2);
    }
 
    // (4) 200 X 200
    if (terrain200 == true) {
        glLoadIdentity();
        gluLookAt(camPos3[0], camPos3[1], camPos3[2], camTarget3[0], camTarget3[1], camTarget3[2], 0, 1, 0);

        if (bump == true) {
            bumpUp(numOfVerts3, create3, 50, 5, userX, userY);
        }
        if (dent == true) {
            dentDown(numOfVerts3, create3, 50, 5, userX, userY);
        }

        drawTerrain(numOfVerts3, create3, disp3);
    }
    
    // (5) 250 X 250
    if (terrain250 == true) {
        glLoadIdentity();
        gluLookAt(camPos4[0], camPos4[1], camPos4[2], camTarget4[0], camTarget4[1], camTarget4[2], 0, 1, 0);

        if (bump == true) {
            bumpUp(numOfVerts4, create4, 65, 7, userX, userY);
        }
        if (dent == true) {
            dentDown(numOfVerts4, create4, 65, 7, userX, userY);
        }

        drawTerrain(numOfVerts4, create4, disp4);
    }

    // (6) 300 X 300
    if (terrain300 == true) {
        glLoadIdentity();
        gluLookAt(camPos5[0], camPos5[1], camPos5[2], camTarget5[0], camTarget5[1], camTarget5[2], 0, 1, 0);

        if (bump == true) {
            bumpUp(numOfVerts5, create5, 80, 9, userX, userY);
        }
        if (dent == true) {
            dentDown(numOfVerts5, create5, 80, 9, userX, userY);
        }

        drawTerrain(numOfVerts5, create5, disp5);
    }

    
    //--------------- RESET FUNCTION -------------------------------------
    if (reset == true) {
        // glClear(GL_COLOR_BUFFER_BIT);
        // glLoadIdentity();

        // // (1) 50 X 50
        // if (size50 == true) {
        //     gluLookAt(camPos0[0], camPos0[1], camPos0[2], camTarget0[0], camTarget0[1], camTarget0[2], 0, 1, 0);
        //     resetTerrain(numOfVerts0, verts0);
            
        //     // Iterates the Circle algorithm
        //     srand(time(NULL));
        //     while (c0 < count0) {
        //         circAlg(numOfVerts0, verts0, circSize0, disp0, xtemp0, ztemp0);
        //         c0 += 1;
        //     }
        //     drawTerrain(numOfVerts0, verts0, disp0); 
        // }

        // // (2) 100 X 100
        // if (size100 == true) {
        //     glLoadIdentity();
        //     gluLookAt(camPos1[0], camPos1[1], camPos1[2], camTarget1[0], camTarget1[1], camTarget1[2], 0, 1, 0);
        //     srand(time(NULL));
        //     while (c1 < count1) {
        //         circAlg(numOfVerts1, verts1, circSize1, disp1, xtemp1, ztemp1);
        //         c1 += 1;
        //     }
        //     drawTerrain(numOfVerts1, verts1, disp1);
        // }

        // // (3) 150 X 150 
        // if (size150 == true) {
        //     glLoadIdentity();
        //     gluLookAt(camPos2[0], camPos2[1], camPos2[2], camTarget2[0], camTarget2[1], camTarget2[2], 0, 1, 0);
        //     srand(time(NULL));
        //     while (c2 < count2) {
        //         circAlg(numOfVerts2, verts2, circSize2, disp2, xtemp2, ztemp2);
        //         c2 += 1;
        //     }
        //     drawTerrain(numOfVerts2, verts2, disp2);
        // }
    
        // // (4) 200 X 200
        // if (size200 == true) {
        //     glLoadIdentity();
        //     gluLookAt(camPos3[0], camPos3[1], camPos3[2], camTarget3[0], camTarget3[1], camTarget3[2], 0, 1, 0);
        //     srand(time(NULL));
        //     while (c3 < count3) {
        //         circAlg(numOfVerts3, verts3, circSize3, disp3, xtemp3, ztemp3);
        //         c3 += 1;
        //     }
        //     drawTerrain(numOfVerts3, verts3, disp3);
        // }
        
        // // (5) 250 X 250
        // if (size250 == true) {
        //     glLoadIdentity();
        //     gluLookAt(camPos4[0], camPos4[1], camPos4[2], camTarget4[0], camTarget4[1], camTarget4[2], 0, 1, 0);
        //     srand(time(NULL));
        //     while (c4 < count4) {
        //         circAlg(numOfVerts4, verts4, circSize4, disp4, xtemp4, ztemp4);
        //         c4 += 1;
        //     }
        //     drawTerrain(numOfVerts4, verts4, disp4);
        // }

        // // (6) 300 X 300
        // if (size300 == true) {
        //     glLoadIdentity();
        //     gluLookAt(camPos5[0], camPos5[1], camPos5[2], camTarget5[0], camTarget5[1], camTarget5[2], 0, 1, 0);
        //     srand(time(NULL));
        //     while (c5 < count5) {
        //         circAlg(numOfVerts5, verts5, circSize5, disp5, xtemp5, ztemp5);
        //         c5 += 1;
        //     }
        //     drawTerrain(numOfVerts5, verts5, disp5);
        // }
        reset = false;
    }

    glutSwapBuffers();
    glutPostRedisplay();
}


// D R A W   2 D  O V E R V I E W - for second window displaying Terrain overhead
void draw2D(int nv, float** v, int disp) {
    for (int x = 0; x < nv; ++x) {
        for (int z = 0; z < nv; ++z) {
            glBegin(GL_POINTS);             
                glColor3f(colour[0]+0.1*v[x][z]/disp, colour[1]+0.1*v[x][z]/disp, colour[2]+0.1*v[x][z]/disp);
                glVertex2f(x, z);       
            glEnd();
        }
    }
}


// D I S P L A Y   2 D - draws the 2D overview of terrain to window
void display2(void) {
    glClearColor(0, 0, 0, 0.5);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 300, 300, 0);

    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Drawing overhead for generated terrains
    if (size50 == true) {
        draw2D(numOfVerts0, verts0, disp0);
    }    
    if (size100 == true) {
        draw2D(numOfVerts1, verts1, disp1);
    }
    if (size150 == true) {
        draw2D(numOfVerts2, verts2, disp2);
    }
    if (size200 == true) {
        draw2D(numOfVerts3, verts3, disp3);
    }
    if (size250 == true) {
        draw2D(numOfVerts4, verts4, disp4);
    }
    if (size300 == true) {
        draw2D(numOfVerts5, verts5, disp5);
    }
    
    // overhead for BONSU (creating terrains)
    if (terrain50 == true) {
        draw2D(numOfVerts0, create0, disp0);
    }
    if (terrain100 == true) {
        draw2D(numOfVerts1, create1, disp1);
    }
    if (terrain150 == true) {
        draw2D(numOfVerts2, create2, disp2);
    }
    if (terrain200 == true) {
        draw2D(numOfVerts3, create3, disp3);
    }
    if (terrain250 == true) {
        draw2D(numOfVerts4, create4, disp4);
    }
    if (terrain300 == true) {
        draw2D(numOfVerts5, create5, disp5);
    }

    glutSwapBuffers();
    glutPostRedisplay();
}


// M E N U   P R O C E S S O R S

// menuGridSize - For the Grid size
void menuGridSize(int value){
    if (value == 1) {
        size50 = true; 
        size100 = false; 
        size150 = false; 
        size200 = false; 
        size250 = false; 
        size300 = false;  

        terrain50 = false;
        terrain100 = false;
        terrain150 = false;
        terrain200 = false;
        terrain250 = false;
        terrain300 = false;
        printf("Terrain Size 50 x 50\n");
    }
    if (value == 2) {
        size50 = false; 
        size100 = true; 
        size150 = false; 
        size200 = false; 
        size250 = false; 
        size300 = false; 

        terrain50 = false;
        terrain100 = false;
        terrain150 = false;
        terrain200 = false;
        terrain250 = false;
        terrain300 = false;
        printf("Terrain Size 100 x 100\n");
    }
    if (value == 3) {
        size50 = false; 
        size100 = false; 
        size150 = true; 
        size200 = false; 
        size250 = false; 
        size300 = false; 

        terrain50 = false;
        terrain100 = false;
        terrain150 = false;
        terrain200 = false;
        terrain250 = false;
        terrain300 = false;
        printf("Terrain Size 150 x 150\n");
    }
    if (value == 4) {
        size50 = false; 
        size100 = false; 
        size150 = false; 
        size200 = true; 
        size250 = false; 
        size300 = false;    
        
        terrain50 = false;
        terrain100 = false;
        terrain150 = false;
        terrain200 = false;
        terrain250 = false;
        terrain300 = false;
        printf("Terrain Size 200 x 200\n");
    }
    if (value == 5) {
        size50 = false; 
        size100 = false; 
        size150 = false; 
        size200 = false; 
        size250 = true; 
        size300 = false; 

        terrain50 = false;
        terrain100 = false;
        terrain150 = false;
        terrain200 = false;
        terrain250 = false;
        terrain300 = false;
        printf("Terrain Size 250 x 250\n");
    }
    if (value == 6) {
        size50 = false; 
        size100 = false; 
        size150 = false; 
        size200 = false; 
        size250 = false; 
        size300 = true; 

        terrain50 = false;
        terrain100 = false;
        terrain150 = false;
        terrain200 = false;
        terrain250 = false;
        terrain300 = false;
        printf("Terrain Size 300 x 300\n");
    }
}

// menuShading - For Shading mode
void menuShading(int value){
    if (value == 1) {
        shade_GOURAUD = false;
        shade_FLAT = true;
        printf("Shading Mode FLAT\n");
    }
    if (value == 2) {
        shade_FLAT = false;
        shade_GOURAUD = true;
        printf("Shading Mode GOURAUD\n");
    }
}

// menuColour - Colour options
void menuColour(int value){
    if (value == 1) {
        colour[0] = 0.5;
        colour[1] = 0;
        colour[2] = 0;
        printf("Terrain Colour RED\n");
    }
    if (value == 2) {
        colour[0] = 0.5;
        colour[1] = 0.25;
        colour[2] = 0;
        printf("Terrain Colour ORANGE\n");
    }
    if (value == 3) {
        colour[0] = 0.5;
        colour[1] = 0.5;
        colour[2] = 0;
        printf("Terrain Colour YELLOW\n");
    }
    if (value == 4) {
        colour[0] = 0;
        colour[1] = 0.5;
        colour[2] = 0;
        printf("Terrain Colour GREEN\n");
    }
    if (value == 5) {
        colour[0] = 0;
        colour[1] = 0;
        colour[2] = 0.5;
        printf("Terrain Colour BLUE\n");
    }
    if (value == 6) {
        colour[0] = 0.25;
        colour[1] = 0;
        colour[2] = 0.25;
        printf("Terrain Colour PURPLE\n");
    }
    if (value == 7) {
        colour[0] = 0.5;
        colour[1] = 0.5;
        colour[2] = 0.5;
        printf("Terrain Colour GREY\n");
    }
}

// BONUS FEATURE - Create a terrain menu proc
void menuCreate(int value) {
    if (value == 1) {
        terrain50 = true;
        terrain100 = false;
        terrain150 = false;
        terrain200 = false;
        terrain250 = false;
        terrain300 = false;

        size50 = false;
        size100 = false; 
        size150 = false; 
        size200 = false; 
        size250 = false; 
        size300 = false; 
        resetTerrain(numOfVerts0, create0);
        printf("Creating a 50 x 50 terrain\n");
    }
    if (value == 2) {
        terrain50 = false;
        terrain100 = true;
        terrain150 = false;
        terrain200 = false;
        terrain250 = false;
        terrain300 = false;
 
        size50 = false;
        size100 = false; 
        size150 = false; 
        size200 = false; 
        size250 = false; 
        size300 = false; 
        resetTerrain(numOfVerts1, create1);
        printf("Creating a 100 x 100 terrain\n");
    }
    if (value == 3) {
        terrain50 = false;
        terrain100 = false;
        terrain150 = true;
        terrain200 = false;
        terrain250 = false;
        terrain300 = false;

        size50 = false;
        size100 = false; 
        size150 = false; 
        size200 = false; 
        size250 = false; 
        size300 = false; 
        resetTerrain(numOfVerts2, create2);
        printf("Creating a 150 x 150 terrain\n");
    }
    if (value == 4) {
        terrain50 = false;
        terrain100 = false;
        terrain150 = false;
        terrain200 = true;
        terrain250 = false;
        terrain300 = false;

        size50 = false;
        size100 = false; 
        size150 = false; 
        size200 = false; 
        size250 = false; 
        size300 = false; 
        resetTerrain(numOfVerts3, create3);
        printf("Creating a 200 x 200 terrain\n");
    }
    if (value == 5) {
        terrain50 = false;
        terrain100 = false;
        terrain150 = false;
        terrain200 = false;
        terrain250 = true;
        terrain300 = false;

        size50 = false;
        size100 = false; 
        size150 = false; 
        size200 = false; 
        size250 = false; 
        size300 = false; 
        resetTerrain(numOfVerts4, create4);
        printf("Creating a 250 x 250 terrain\n");
    }
    if (value == 6) {
        terrain50 = false;
        terrain100 = false;
        terrain150 = false;
        terrain200 = false;
        terrain250 = false;
        terrain300 = true;

        size50 = false;
        size100 = false; 
        size150 = false; 
        size200 = false; 
        size250 = false; 
        size300 = false; 
        resetTerrain(numOfVerts5, create5);
        printf("Creating a 300 x 300 terrain\n");
    }
}

// menuProc
void menuProc(int value){
    if (value == 1) {
        printf("\n\n\nThank you for using the I N T E R A C T I V E   T E R R A I N  2 0 0 0 !\n\n\n");
        exit(0);
    }
}

// creating the actual menu
void createOurMenu(){
    int subMenu_gridSize = glutCreateMenu(menuGridSize);
    glutAddMenuEntry("50 x 50", 1);
    glutAddMenuEntry("100 x 100", 2);
    glutAddMenuEntry("150 x 150", 3);
    glutAddMenuEntry("200 x 200", 4);
    glutAddMenuEntry("250 x 250", 5);
    glutAddMenuEntry("300 x 300", 6);

    int subMenu_shadingMode = glutCreateMenu(menuShading);
    glutAddMenuEntry("Flat", 1);
    glutAddMenuEntry("Gouraud", 2);

    int subMenu_colour = glutCreateMenu(menuColour);
    glutAddMenuEntry("Red", 1);
    glutAddMenuEntry("Orange", 2);
    glutAddMenuEntry("Yellow", 3);
    glutAddMenuEntry("Green", 4);
    glutAddMenuEntry("Blue", 5);
    glutAddMenuEntry("Purple", 6);
    glutAddMenuEntry("Grey", 7);

    int subMenu_createTerrain = glutCreateMenu(menuCreate);
    glutAddMenuEntry("50 x 50", 1);
    glutAddMenuEntry("100 x 100", 2);
    glutAddMenuEntry("150 x 150", 3);
    glutAddMenuEntry("200 x 200", 4);
    glutAddMenuEntry("250 x 250", 5);
    glutAddMenuEntry("300 x 300", 6);

    int main_id = glutCreateMenu(menuProc);
    glutAddSubMenu("Grid Size", subMenu_gridSize);
    glutAddSubMenu("Shading Mode", subMenu_shadingMode);
    glutAddSubMenu("Colour", subMenu_colour);
    glutAddSubMenu("Create a Terrain", subMenu_createTerrain);
    glutAddMenuEntry("Quit", 1);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}


// M A N U AL - instructions that will show up in command prompt
int manual()
{
    printf("\n\n\n --- Welcome to the I N T E R A C T I V E  T E R R A I N  2 0 0 0 --- \n");
    printf("\n How to navigate the program:\n\n");
    printf("    *** 'ARROW KEYS' - Orbit the terrain\n");
    printf("    *** 'MOUSE BUTTONS' - Click on 2D Terrain\n");
    printf("         overview for user interactivity\n");
    printf("      ~  'LEFT' - Add bump to terrain\n");
    printf("      ~  'RIGHT' - Add dent to terrain\n");
    printf("    *** 'T' - Triangle-strip terrain\n");
    printf("    *** 'Y' - Quad-strip terrain\n");
    printf("    *** 'L' - Enable Lighting\n");
    printf("      ~  Click once to enable Light 1 \n");
    printf("      ~  Click twice to enable Light 2 \n");
    printf("      ~  Click thrice to disable Lighting \n");
    printf("      ~ 'O', 'P' - Move light 1 along x-axis (front side)\n");
    printf("      ~ 'U', 'I' - Move light 2 along x-axis (back side)\n");
    printf("    *** 'W' - Viewing mode\n");
    printf("      ~  Click once for WIREFRAME mode\n");
    printf("      ~  Click twice for SOLID & WIREFRAME mode\n");
    printf("      ~  Click thrice to go back to default (SOLID) mode\n");
    printf("    ***  Right-Click on the mouse for Menu options:\n");
    printf("      ~  Grid Size\n");
    printf("      ~  Shading Mode (Flat or Gouraud)\n");
    printf("      ~  Colour\n");
    printf("      ~  Create a Terrain\n");
    printf("    *** 'R' - Reset terrain\n");
    printf("    *** 'Q', ESC - Quit the program\n");
    printf("\n\n Truong Productions\n\n\n");
    printf("\n ------- * ACTION LOG * -------\n\n");

    return 0;
}


// M A I N   F U N C T I O N - callback functions
int main(int argc, char** argv) {
    manual();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutInitWindowSize(300, 300);
    glutInitWindowPosition(450, 55);

    glutCreateWindow("2D Terrain Overview");
    glutDisplayFunc(display2);

    glutMouseFunc(mouse);

    glutInitWindowSize(700, 700);
    glutInitWindowPosition(750, 55);

    glutCreateWindow("Truong - Interactive Terrain 2000");

    glutDisplayFunc(display);

    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    
    init();

    createOurMenu();

    glutMainLoop();
    return(0);

}