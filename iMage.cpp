//Author: Srinivas Nayak
//Email: sinu.nayak2001@gmail.com

//Copyright (c) 2022 Srinivas Nayak - All Rights Reserved.

//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
//WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
//AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
//BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
//ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
//OR OTHER DEALINGS IN THE SOFTWARE.

//To compile and link FLTK programs
//get fltk-1.3.5-source.tar.gz
//extract and do
//# cd spot/fltk-1.3.5
//# make
//fltk got compiled.
//now do
//# cd spot/fltk-1.3.5
//# ./fltk-config --cxxflags
//and
//# ./fltk-config --ldflags
//these two gives needed compiler and linker flags for our program.
//first one gives
//-I. -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/freetype2 -I/usr/include/libpng16 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_THREAD_SAFE -D_REENTRANT
//second one gives
//-L./lib -lfltk -lXrender -lXcursor -lXfixes -lXext -lXft -lfontconfig -lXinerama -lpthread -ldl -lm -lX11
//now do
//# cd ..
//now add the above outputs to your compile command, to get
//g++ iMage.cpp -Wall -g
//-I./fltk-1.3.5 -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/freetype2 -I/usr/include/libpng16 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_THREAD_SAFE -D_REENTRANT
//-L./fltk-1.3.5/lib -lfltk -lXrender -lXcursor -lXfixes -lXext -lXft -lfontconfig -lXinerama -lpthread -ldl -lm -lX11
//now this command will compile your code.
//in a single line, your command will be...
//g++ iMage.cpp -Wall -g -I./fltk-1.3.5 -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/freetype2 -I/usr/include/libpng16 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_THREAD_SAFE -D_REENTRANT -L./fltk-1.3.5/lib -lfltk -lXrender -lXcursor -lXfixes -lXext -lXft -lfontconfig -lXinerama -lpthread -ldl -lm -lX11

//in Windows, for fltk 1.3.7, to get a statically linked exe, command is
//g++ iMage.cpp -Wall -g -o iMage.exe -I./fltk-1.3.7 -mwindows -DWIN32 -DUSE_OPENGL32 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -mwindows ./fltk-1.3.7/lib/libfltk.a -lole32 -luuid -lcomctl32 -static-libgcc -static-libstdc++ -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive

//note that: for static linking we ran --ldstaticflags option, not --ldflags
//after adding all command fragments, we even added -static-libgcc -static-libstdc++ -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive
//this is to get away with the libstdc++-6.dll, libgcc_s_seh-1.dll, libwinpthread-1.dll and link them statically.
//this will give us statically linked a.exe

//however, in windows, the created iMage.exe has some properties,
//like File description, Product name, Copyright etc.
//which is shown to us while hovering mouse on the exe file.
//these properties are added to a program via a resource script file (*.rc)
//first *.rc file is compiled by "windres" tool to get *.o file
//and then the *.o file is linked to our program.
//here, while statically linking to libwinpthread, we get its default resource file.
//so when we mouse hover on iMage.exe, we see File description as "POSIX WinThreads for Windows".
//to remove it, we have to copy C:\msys64\mingw64\x86_64-w64-mingw32\lib\libwinpthread.a
//to our iMage.cpp file directory. now we have to remove resource file object from it.
//using command, ar -d libwinpthread.a version.o
//finally we have to use this libwinpthread.a in our command like this
//g++ iMage.cpp -Wall -g -o iMage.exe -I./fltk-1.3.7 -mwindows -DWIN32 -DUSE_OPENGL32 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -mwindows ./fltk-1.3.7/lib/libfltk.a -lole32 -luuid -lcomctl32 -static-libgcc -static-libstdc++ -Wl,-Bstatic,--whole-archive ./libwinpthread.a -Wl,--no-whole-archive

//******************

//Data Structures:

//1. element list has element nodes
//2. each element node has color and a list of point nodes
//3. each point node has a pixel position

//1. text list has text nodes
//2. each text node has one text and associated text box widget

//******************

//General Algorithm:

//on click on iMage app, open clean slate
//on click on iMage*.1mg file, open iMage*.1mg
//on close, save iMage_closed_timesecs.1mg

//for save mode
//on save, save iMage_saved_timesecs.1mg

//for move delete mode
//on left push bring to top
//on left drag reposition and redraw
//on right push delete it

//for other modes
//push drag is one drawing task
//on left push create top element
//on left drag append top one and redraw

//for all modes, use release for cleanup

//******************

//Flickering avoidance usually involves one of these methods:
//- Overlay plane drawing
//- XOR drawing
//- redrawing entire image with graphic overlays
//- drawing into a separate widget

//******************

//Flickering avoidance Algorithm:

//mov mode:
//push:
//find element
//remove element
//add element to top
//erase top from (buffer)
//remove element
//draw rest all to (buffer)
//add element to top
//drag:
//erase top from (screen)
//modify top element
//draw top on (screen)

//del mode:
//push:
//(same as move mode push)
//find element
//remove element
//add element to top
//erase top from (buffer)
//remove element
//draw rest all to (buffer)
//add element to top
//drag:
//erase top from (screen)

//pen mode:
//push:
//add element to top
//draw top on (screen)
//drag:
//modify top element
//draw top on (screen)

//rect, line, ellip, arrow mode:
//push:
//add element to top
//remove element
//draw rest all to (buffer)
//add element to top
//draw top on (screen)
//drag:
//(same as mov mode drag)
//erase top from (screen)
//modify top element
//draw top on (screen)

//******************

//History:
//- initial implementation

#include<stdio.h> //printf, sprintf
#include<stdlib.h> //malloc, free
#include<string.h> //memset, strcpy
#include<limits.h> //INT_MAX
#include<math.h> //pow
#include<time.h> //time
//#include<sys/time.h> //gettimeofday

#include <FL/Fl.H> //must for all fltk programs
#include <FL/Fl_Window.H> //every widget class needs its own header file
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Input.H>
#include <FL/fl_ask.H>
#include <FL/fl_utf8.h>

#include<float.h> //FLT_MIN

//******************
class TxtBox: public Fl_Box
{
public:
    TxtBox(int x, int y, int w, int h);
protected:
    //must be defined for this widget's draw to happen
    void draw();
    int handle(int e);
private:
};

class ImgBox: public Fl_Box
{
public:
    ImgBox(int x, int y, int w, int h);
protected:
    //must be defined for this widget's draw to happen
    void draw();
    int handle(int e);
private:
};

//******************

#define MAX_X 1339
#define MAX_Y 649
#define MAX_TXT_LEN 127

typedef struct point point;

struct point
{
    int x;
    int y;
};

typedef struct node node;
typedef struct list list;

struct node
{
    point pixel;

    Fl_Color color;
    list* point_list;

    char* txt;
    TxtBox* tbox;

    node* next;
    node* prev;
};

struct list
{
    node* head;
    node* tail;
};

//******************

node* nalloc();
void ninitpix(node* n, int x, int y);
void ninitplist(node* n, list* pointlist, Fl_Color color);
void ninittbox(node* n, TxtBox* tbox);
void nfree(node* n);

list* lalloc();
void lfree(list* l);
int lempty(list* l);
node* linsert(list* l, node* n);
node* lremfifo(list* l);
node* lremlifo(list* l);
node* lremmid(list* l, node* n);

//******************

node* nalloc()
{
    node* n = (node*)malloc(sizeof(node));
    if( ! n)
        return 0;

    n->pixel.x = -1;
    n->pixel.y = -1;
    n->color = FL_BLACK;
    n->point_list = 0;
    n->txt = 0;
    n->tbox = 0;

    n->next = 0;
    n->prev = 0;
    return n;
}

void ninitpix(node* n, int x, int y)
{
    if(x<0||x>MAX_X||y<0||y>MAX_Y)
        printf("bad pixel node: %d, %d\n", x, y);

    n->pixel.x = x;
    n->pixel.y = y;
}

void ninitplist(node* n, list* pointlist, Fl_Color color)
{
    n->color = color;
    n->point_list = pointlist;
}

void ninittbox(node* n, TxtBox* tbox)
{
    n->txt = 0;
    n->tbox = tbox;
}

void nfree(node* n)
{
    if(n)
        free(n);
}

list* lalloc()
{
    list* l = (list *)malloc(sizeof(list));
    if( ! l)
        return 0;

    node* h = nalloc();
    if( ! h)
        {free(l); return 0;}

    node* t = nalloc();
    if( ! t)
        {free(l); nfree(h); return 0;}

    h->prev = 0;
    h->next = t;
    t->prev = h;
    t->next = 0;

    l->head = h;
    l->tail = t;

    return l;
}

void lfree(list* l)
{
    if(l)
    {
        if(l->head)
            nfree(l->head);
        if(l->tail)
            nfree(l->tail);
        free(l);
    }
}

int lempty(list* l)
{
    node* h = l->head;
    node* t = l->tail;

    if(h->next == t)
        return 1;
    else
        return 0;
}

node* linsert(list* l, node* n)
{
    node* h = l->head;
    node* o = h->next;

    h->next = n;
    n->prev = h;
    n->next = o;
    o->prev = n;

    return n;
}

node* lremfifo(list* l)
{
    if(lempty(l))
        return 0;
    node* t = l->tail;
    node* n = t->prev;
    node* o = n->prev;
    o->next = t;
    t->prev = o;

    n->next = 0;
    n->prev = 0;
    return n;
}

node* lremlifo(list* l)
{
    if(lempty(l))
        return 0;
    node* h = l->head;
    node* n = h->next;
    node* o = n->next;
    o->prev = h;
    h->next = o;

    n->next = 0;
    n->prev = 0;
    return n;
}

node* lremmid(list* l, node* n)
{
    if(lempty(l))
        return 0;
    n->prev->next = n->next;
    n->next->prev = n->prev;

    n->next = 0;
    n->prev = 0;
    return n;
}

//******************

typedef struct
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} pix;

pix red = {255, 0, 0, 0};
pix green = {0, 255, 0, 0};
pix blue = {0, 0, 255, 0};
pix white = {255, 255, 255, 0};

pix slate[MAX_Y+1][MAX_X+1] = {0};

typedef struct
{
    Fl_Window *window;
        Fl_Box* obox;
        Fl_Group *b1g;
            Fl_Button* mov_delb;
            Fl_Button* penb;
            Fl_Button* rectb;
            Fl_Button* lineb;
            Fl_Button* ellipb;
            Fl_Button* arrowb;
            Fl_Button* saveb;
            Fl_Button* helpb;
            Fl_Button* aboutb;
        ImgBox* ibox;
        //Fl_Group *b2g;
} widgets;
widgets w;

typedef enum
{
    MOV_DEL_MODE,
    PEN_MODE,
    RECT_MODE,
    LINE_MODE,
    ELLIP_MODE,
    ARROW_MODE
} mode;

mode curr_mode = MOV_DEL_MODE;
mode prev_mode = PEN_MODE;
Fl_Color curr_color = FL_BLUE;
list* elem_l = NULL;
list* txt_l = NULL;
int prog_start = 1;
Fl_Input* intxt = 0;

int on_left_drag = 0;

int x_grabbed_to_move = -1;
int y_grabbed_to_move = -1;
int x_of_ellipse = -1;
int y_of_ellipse = -1;
int x_grabbed_to_del = -1;
int y_grabbed_to_del = -1;
int show_event = 0; //for case of window minimize/restore
int txt_box_resized = 0;

node* txt_grabbed_to_move = 0;
int txt_x_grabbed_to_move = -1;
int txt_y_grabbed_to_move = -1;
int prep_buf_for_txt_drag = 0;

node* txt_grabbed_to_del = 0;

FILE* infp = NULL;
FILE* outfp = NULL;

char save_path[1000+1]={0};

//******************

void globalize(int o_x, int o_y, double o_t, int p_x, int p_y, int* res_x, int* res_y);
double angle_of_rotation(int x1, int y1, int x2, int y2);

void mov_delb_callback(Fl_Widget* wd, void* data);
void penb_callback(Fl_Widget* wd, void* data);
void rectb_callback(Fl_Widget* wd, void* data);
void lineb_callback(Fl_Widget* wd, void* data);
void ellipb_callback(Fl_Widget* wd, void* data);
void arrowb_callback(Fl_Widget* wd, void* data);
void saveb_callback(Fl_Widget* wd, void* data);
void helpb_callback(Fl_Widget* wd, void* data);
void aboutb_callback(Fl_Widget* wd, void* data);

void mainwin_callback(Fl_Widget* wd, void* data);

void set_next_color(Fl_Color curr_color);
void reset_all_button_color();
pix* get_pix_rgb_from_col_name(Fl_Color col);
Fl_Color get_col_name_from_pix_rgb(pix* pix_col);

void linemostlyhoriz(list* pt_l, int x1, int y1, int x2, int y2);
void linemostlyvert(list* pt_l, int x1, int y1, int x2, int y2);
void linepoints(list* pt_l, int x1, int y1, int x2, int y2);

void putpix(list* pt_l, int x, int y);
void check_n_put(list* pt_l, int x, int y);

void new_ellipsepoints(list* pt_l, int x1, int y1, int x2, int y2);

void ellipsepoints(list* pt_l, int x1, int y1, int x2, int y2);
void ellipsemostlyhoriz(list* pt_l, int x1, int y1, int x2, int y2);
void ellipsemostlyvert(list* pt_l, int x1, int y1, int x2, int y2);
void ellipsesympoints(list* pt_l, int midx, int midy, int x, int y);

void swapuc(unsigned char* a, unsigned char* b);
void swapi(int* a, int p, int q);
void inssort(int* a, int l);
int cmp(const void* pa, const void* pb);

//void copy_slate_to_snap();
//void copy_snap_to_slate();

void draw_top_element_on_screen(ImgBox* imgbox);
void draw_all_elements_on_buffer();
void erase_top_element_on_screen(ImgBox* imgbox);
void erase_top_element_on_buffer();

void copy_area_from_buf_to_screen(int x1, int y1, int x2, int y2);

void convert_input_to_box();
void create_txt_node_with_first_point(int mouse_x, int mouse_y);

void create_element_node_with_first_point(int mouse_x, int mouse_y);
void destroy_element_node(node* elem_n);
node* find_element_node(int mouse_x, int mouse_y);

node* find_txt_node(int pos_x, int pos_y);

void save_to_file(int app_closed);
void save_elem_list();
void save_tbox_list();
void read_from_file();

void reposition_box(Fl_Widget* box, int new_x, int new_y); //new_x is wrt app window

int main(int argc, char **argv);

//******************

void globalize(int o_x, int o_y, double o_t, int p_x, int p_y, int* res_x, int* res_y)
{
    if(p_x==0)
        p_x=1;
    double tanb = p_y*1.0/p_x; //*1.0 for making division floating
    double b = atan(tanb); //rad
    double r = sqrt(p_x*p_x + p_y*p_y);

    double p_global_x=0;
    double p_global_y=0;
    double t;
    double g;

    if(0<=o_t && o_t<90)
    {
        t = o_t-0;
        t = t*3.142/180; //rad
        g = t-b;

        if(g>=0)
        {
            p_global_x = o_x + r*cos(g);
            p_global_y = o_y - r*sin(g);
        }
        else //g<0
        {
            g = 0-g;
            p_global_x = o_x + r*cos(g);
            p_global_y = o_y + r*sin(g);
        }
    }
    if(90<=o_t && o_t<180)
    {
        t = o_t-90;
        t = t*3.142/180; //rad
        g = t-b;

        if(g>=0)
        {
            p_global_x = o_x - r*sin(g);
            p_global_y = o_y - r*cos(g);
        }
        else //g<0
        {
            g = 0-g;
            p_global_x = o_x + r*sin(g);
            p_global_y = o_y - r*cos(g);
        }
    }
    if(180<=o_t && o_t<270)
    {
        t = o_t-180;
        t = t*3.142/180; //rad
        g = t-b;

        if(g>=0)
        {
            p_global_x = o_x - r*cos(g);
            p_global_y = o_y + r*sin(g);
        }
        else //g<0
        {
            g = 0-g;
            p_global_x = o_x - r*cos(g);
            p_global_y = o_y - r*sin(g);
        }
    }
    if(270<=o_t && o_t<360)
    {
        t = o_t-270;
        t = t*3.142/180; //rad
        g = t-b;

        if(g>=0)
        {
            p_global_x = o_x + r*sin(g);
            p_global_y = o_y + r*cos(g);
        }
        else //g<0
        {
            g = 0-g;
            p_global_x = o_x - r*sin(g);
            p_global_y = o_y + r*cos(g);
        }
    }
    *res_x = p_global_x;
    *res_y = p_global_y;
}

double angle_of_rotation(int x1, int y1, int x2, int y2)
{
    double t=0;
    double rad;

    if(x2>x1 && y2<y1)
    {
        //1st q
        rad = atan(abs(y2-y1)*1.0/abs(x2-x1)); //*1.0 for making division floating
        t = 0 + rad*180/3.142;
    }
    if(x2<x1 && y2<y1)
    {
        //2nd q
        rad = atan(abs(x2-x1)*1.0/abs(y2-y1));
        t = 90 + rad*180/3.142;
    }
    if(x2<x1 && y2>y1)
    {
        //3rd q
        rad = atan(abs(y2-y1)*1.0/abs(x2-x1));
        t = 180 + rad*180/3.142;
    }
    if(x2>x1 && y2>y1)
    {
        //4th q
        rad = atan(abs(x2-x1)*1.0/abs(y2-y1));
        t = 270 + rad*180/3.142;
    }

    if(x2>x1 && y2==y1)
    {
        //x axis
        t = 0;
    }
    if(x2<x1 && y2==y1)
    {
        //-x axis
        t = 180;
    }
    if(x2==x1 && y2<y1)
    {
        //y axis
        t = 90;
    }
    if(x2==x1 && y2>y1)
    {
        //-y axis
        t = 270;
    }

    return t;
}

//******************

//find_element_node
node* find_element_node(int mouse_x, int mouse_y)
{
    int found = 0;

    //loop all element node
    node* elem_n = elem_l->head->next;
    while (elem_n->next != 0) //elem_n->next == 0 for dummy tail node
    {
        //get point list
        list* pt_l = elem_n->point_list;
        //loop all point node
        node* pt_n = pt_l->head->next;
        while (pt_n->next != 0) //pt_n->next == 0 for dummy tail node
        {
            if(pt_n->pixel.x == mouse_x &&
                pt_n->pixel.y == mouse_y)
            {
                found = 1; break;
            }
            pt_n = pt_n->next;
        }
        if(found == 1)
            break;
        elem_n = elem_n->next;
    }

    if(found == 1)
        return elem_n;
    else
        return NULL;
}

//create_element_node
void create_element_node_with_first_point(int mouse_x, int mouse_y)
{
    node* elem_n = nalloc();
    linsert(elem_l, elem_n);
    //create point list
    list* pt_l = lalloc();
    ninitplist(elem_n, pt_l, curr_color);
    //create first point node
    node* pt_n = nalloc();
    ninitpix(pt_n, mouse_x, mouse_y);
    linsert(pt_l, pt_n);
}

void create_txt_node_with_first_point(int mouse_x, int mouse_y)
{
    TxtBox* txtb = new TxtBox(w.ibox->x()+mouse_x, w.ibox->y()+mouse_y, 100, 24); //default font size is 14 pixel, we add 5 pixel margin
    txtb->box(FL_ROUNDED_BOX); //FL_NO_BOX/FL_PLASTIC_UP_BOX/FL_ROUNDED_BOX better
    //txtb->labelfont(FL_SCREEN); //monospaced font type
    //txtb->labelsize(14); //font size in pixel
    reposition_box(txtb, w.ibox->x()+mouse_x, w.ibox->y()+mouse_y);

    //txtb->labelcolor(FL_BLACK);
    //txtb->labelfont(FL_SCREEN); //font type
    //txtb->labelsize(14); //font size in pixel
    //txtb->label("click!");

    node* txt_n = nalloc();
    ninittbox(txt_n, txtb);
    linsert(txt_l, txt_n);

    w.window->add(*txtb); //append this widget at end of main window's child list
    //txtb->hide(); //causes something weird, ibox gets redrawn :-(

    intxt = new Fl_Input(w.ibox->x()+mouse_x, w.ibox->y()+mouse_y, 100, 24);
    Fl::focus(intxt);
    w.window->add(*intxt); //append this widget at end of main window's child list
    reposition_box(intxt, w.ibox->x()+mouse_x, w.ibox->y()+mouse_y);
    intxt->redraw();
}

void convert_input_to_box()
{
    int ww; int hh;

    //close top text input box
    if(intxt != NULL)
    {
        node* txt_n = txt_l->head->next;
        int in_len = strlen(intxt->value());
        if(in_len==0)
        {
            lremmid(txt_l, txt_n);
            w.window->remove(txt_n->tbox);
            Fl::delete_widget(txt_n->tbox); //safe widget delete
            nfree(txt_n);
        }
        else
        {
            ww=0; hh=0; //needed for fl_measure or measure_label
            //needed for fl_measure or measure_label etc
            //so better set this in the beginning
            fl_font(FL_HELVETICA, 14);

            txt_n->txt = (char*)malloc(MAX_TXT_LEN+1);
            strncpy(txt_n->txt, intxt->value(), MAX_TXT_LEN);
            txt_n->txt[MAX_TXT_LEN] = '\0';
            txt_n->tbox->label(txt_n->txt);
            //txt_n->tbox->labelfont(FL_SCREEN); //monospaced font type
            //txt_n->tbox->labelsize(14); //font size in pixel
            txt_n->tbox->measure_label(ww, hh);
            ww = ww + 10; //10 for left right margin
            if(ww < 24) ww = 24; //min width 24
            txt_n->tbox->size(ww, 24); //height always 24
            //txt_n->tbox->show(); //no hide no show
            reposition_box(txt_n->tbox, txt_n->tbox->x(), txt_n->tbox->y());
        }
        ::w.window->remove(*intxt);
        //when we type into input text box, mouse cursor disappears
        //if while typing in input text box, we press escape button,
        //mouse cursor will anyway disappear for typing,
        //and input text box will disappear for escape,
        //then we have no cursor at all in the whole window.
        //so below code will help in restoring cursor for the window
        ::w.window->cursor(FL_CURSOR_DEFAULT); //for cursor disappearing issue
        Fl::delete_widget(intxt); //safe widget delete
        intxt = 0;
        txt_box_resized = 1;
    }
}
void destroy_element_node(node* elem_n)
{
    //delete all points
    list* pt_l = elem_n->point_list;
    while (pt_l->head->next != pt_l->tail) //pt_n->next == 0 for dummy tail node
    {
        node *n = lremlifo(pt_l);
        nfree(n);
    }
    //delete point list
    lfree(pt_l);
    //delete element node
    nfree(elem_n);
}

//******************

ImgBox::ImgBox(int x, int y, int w, int h):Fl_Box(x, y, w, h)
{
}

int ImgBox::handle(int e)
{
    if(e == FL_SHOW)
    {
        //printf("show event\n");
        show_event=1; //we draw full after window restore

        return 1;
    }

    if(curr_mode == MOV_DEL_MODE)
    {
        if(e == FL_PUSH && Fl::event_button() == FL_LEFT_MOUSE)
        {
            //fprintf(stderr, "left push event, x,y: %d,%d\n", Fl::event_x(), Fl::event_y());
            int mouse_x = Fl::event_x() - Fl_Box::x();
            int mouse_y = Fl::event_y() - Fl_Box::y();
            //fprintf(stderr, "mouse, x,y: %d,%d\n", mouse_x, mouse_y);
            if(mouse_x<0)
                mouse_x = 0;
            if(mouse_y<0)
                mouse_y = 0;
            if(mouse_x>MAX_X)
                mouse_x = MAX_X;
            if(mouse_y>MAX_Y)
                mouse_y = MAX_Y;

            node* found_element = find_element_node(mouse_x, mouse_y);
            if(found_element != NULL)
            {
                //remove the elem_n
                lremmid(elem_l, found_element);

                //now add it to top
                linsert(elem_l, found_element);

                //erase top from buffer
                erase_top_element_on_buffer();

                //remove the elem_n
                lremmid(elem_l, found_element);

                //draw rest all elem_n on slate
                draw_all_elements_on_buffer();

                //now add it to top
                linsert(elem_l, found_element);

                x_grabbed_to_move = mouse_x;
                y_grabbed_to_move = mouse_y;
            }
            else
            {
                x_grabbed_to_move = -1;
                y_grabbed_to_move = -1;
            }

            //fl_cursor(FL_CURSOR_MOVE);
            redraw();
            return 1;
        }
        if(e == FL_DRAG && Fl::event_button() == FL_LEFT_MOUSE)
        {
            if (x_grabbed_to_move != -1
            && y_grabbed_to_move != -1)
            {
                //fprintf(stderr, "left drag event, x,y: %d,%d\n", Fl::event_x(), Fl::event_y());
                int mouse_x = Fl::event_x() - Fl_Box::x();
                int mouse_y = Fl::event_y() - Fl_Box::y();
                //fprintf(stderr, "mouse, x,y: %d,%d\n", mouse_x, mouse_y);
                if(mouse_x<0)
                    mouse_x = 0;
                if(mouse_y<0)
                    mouse_y = 0;
                if(mouse_x>MAX_X)
                    mouse_x = MAX_X;
                if(mouse_y>MAX_Y)
                    mouse_y = MAX_Y;

                int dx = mouse_x - x_grabbed_to_move;
                int dy = mouse_y - y_grabbed_to_move;

                //for next drag event
                x_grabbed_to_move = mouse_x;
                y_grabbed_to_move = mouse_y;

                //copy top element pixels from slate to screen
                erase_top_element_on_screen(this);

                //change top element point
                //values because of drag
                //get top element
                node* elem_n = elem_l->head->next;
                if (elem_n->next != 0) //elem_n->next == 0 for dummy tail node
                {
                    list* pt_l = elem_n->point_list;
                    //loop all pt_n
                    node* pt_n = pt_l->head->next;
                    while (pt_n->next != 0) //pt_n->next == 0 for dummy tail node
                    {
                        pt_n->pixel.x += dx; //TODO
                        pt_n->pixel.y += dy;

                        pt_n = pt_n->next;
                    }
                }

                //draw top element on screen
                draw_top_element_on_screen(this);
            }

            redraw();
            return 1;
        }
        if(e == FL_RELEASE && Fl::event_button() == FL_LEFT_MOUSE)
        {
            if (x_grabbed_to_move != -1
            && y_grabbed_to_move != -1)
            {
                //little clean up
                x_grabbed_to_move = -1;
                y_grabbed_to_move = -1;
            }

            return 1;
        }

        //*****************

        if(e == FL_PUSH && Fl::event_button() == FL_RIGHT_MOUSE)
        {
            //fprintf(stderr, "right push event, x,y: %d,%d\n", Fl::event_x(), Fl::event_y());
            int mouse_x = Fl::event_x() - Fl_Box::x();
            int mouse_y = Fl::event_y() - Fl_Box::y();
            if(mouse_x<0)
                mouse_x = 0;
            if(mouse_y<0)
                mouse_y = 0;
            if(mouse_x>MAX_X)
                mouse_x = MAX_X;
            if(mouse_y>MAX_Y)
                mouse_y = MAX_Y;

            node* found_element = find_element_node(mouse_x, mouse_y);
            if(found_element != NULL)
            {
                //remove element node
                lremmid(elem_l, found_element);

                //now add it to top
                linsert(elem_l, found_element);

                //erase top from buffer
                erase_top_element_on_buffer();

                //remove the elem_n
                lremmid(elem_l, found_element);

                //draw rest all elem_n on slate
                draw_all_elements_on_buffer();

                //now add it to top
                linsert(elem_l, found_element);

                x_grabbed_to_del = mouse_x;
                y_grabbed_to_del = mouse_y;
            }
            else
            {
                x_grabbed_to_del = -1;
                y_grabbed_to_del = -1;
            }

            redraw();
            return 1;
        }
        if(e == FL_DRAG && Fl::event_button() == FL_RIGHT_MOUSE)
        {
            if (x_grabbed_to_del != -1
            && y_grabbed_to_del != -1)
            {
                //nothing to do here
            }

            return 1;
        }
        if(e == FL_RELEASE && Fl::event_button() == FL_RIGHT_MOUSE)
        {
            if (x_grabbed_to_del != -1
            && y_grabbed_to_del != -1)
            {
                //erase to elem
                erase_top_element_on_screen(this);

                //get top element
                node* elem_n = elem_l->head->next;

                //remove element node
                lremmid(elem_l, elem_n);

                //delete all points
                destroy_element_node(elem_n);

                //little clean up
                x_grabbed_to_del = -1;
                y_grabbed_to_del = -1;
            }

            redraw();
            return 1;
        }
    }

    else if (curr_mode == PEN_MODE)
    {
        if(e == FL_PUSH && Fl::event_button() == FL_LEFT_MOUSE)
        {
            //fprintf(stderr, "left push event, x,y: %d,%d\n", Fl::event_x(), Fl::event_y());
            int mouse_x = Fl::event_x() - Fl_Box::x();
            int mouse_y = Fl::event_y() - Fl_Box::y();
            if(mouse_x<0)
                mouse_x = 0;
            if(mouse_y<0)
                mouse_y = 0;
            if(mouse_x>MAX_X)
                mouse_x = MAX_X;
            if(mouse_y>MAX_Y)
                mouse_y = MAX_Y;

            //create element node
            create_element_node_with_first_point(mouse_x, mouse_y);

            //draw top element on screen
            draw_top_element_on_screen(this);

            //***********
            convert_input_to_box();

            //create element node
            create_txt_node_with_first_point(mouse_x, mouse_y);
            //***********

            redraw();
            return 1;
        }
        if(e == FL_DRAG && Fl::event_button() == FL_LEFT_MOUSE)
        {
            //fprintf(stderr, "left drag event, x,y: %d,%d\n", Fl::event_x(), Fl::event_y());
            int mouse_x = Fl::event_x() - Fl_Box::x();
            int mouse_y = Fl::event_y() - Fl_Box::y();
            if(mouse_x<0)
                mouse_x = 0;
            if(mouse_y<0)
                mouse_y = 0;
            if(mouse_x>MAX_X)
                mouse_x = MAX_X;
            if(mouse_y>MAX_Y)
                mouse_y = MAX_Y;

            on_left_drag = 1;
            convert_input_to_box();

            //get top element node
            node* elem_n = elem_l->head->next;
            //get point list
            list* pt_l = elem_n->point_list;

            //add all middle point nodes
            //between last drag point and this drag point
            int x1 = pt_l->head->next->pixel.x;
            int y1 = pt_l->head->next->pixel.y;
            int x2 = mouse_x;
            int y2 = mouse_y;
            linepoints(pt_l, x1, y1, x2, y2);

            //finally add last point node, this drag point
            node* pt_n = nalloc();
            ninitpix(pt_n, mouse_x, mouse_y);
            linsert(pt_l, pt_n);

            //draw top element on screen
            draw_top_element_on_screen(this);

            redraw();
            return 1;
        }
        if(e == FL_RELEASE && Fl::event_button() == FL_LEFT_MOUSE)
        {
            on_left_drag = 0;

            return 1;
        }

        //**********
        if(e == FL_PUSH && Fl::event_button() == FL_RIGHT_MOUSE)
        {
            convert_input_to_box();

            redraw();
            return 1;
        }
        if(e == FL_RELEASE && Fl::event_button() == FL_RIGHT_MOUSE)
        {
            //nothing to clean
            return 1;
        }
        //**********
    }

    else if (curr_mode == RECT_MODE)
    {
        if(e == FL_PUSH && Fl::event_button() == FL_LEFT_MOUSE)
        {
            //fprintf(stderr, "left push event, x,y: %d,%d\n", Fl::event_x(), Fl::event_y());
            int mouse_x = Fl::event_x() - Fl_Box::x();
            int mouse_y = Fl::event_y() - Fl_Box::y();
            if(mouse_x<0)
                mouse_x = 0;
            if(mouse_y<0)
                mouse_y = 0;
            if(mouse_x>MAX_X)
                mouse_x = MAX_X;
            if(mouse_y>MAX_Y)
                mouse_y = MAX_Y;

            //create element node
            create_element_node_with_first_point(mouse_x, mouse_y);

            //get top element
            node* elem_n = elem_l->head->next;

            //remove the elem_n
            lremmid(elem_l, elem_n);

            //draw rest all elem_n on slate
            draw_all_elements_on_buffer();

            //now add it to top
            linsert(elem_l, elem_n);

            //draw top element on screen
            draw_top_element_on_screen(this);

            //***********
            convert_input_to_box();

            //create element node
            create_txt_node_with_first_point(mouse_x, mouse_y);
            //***********

            redraw();
            return 1;
        }
        if(e == FL_DRAG && Fl::event_button() == FL_LEFT_MOUSE)
        {
            //fprintf(stderr, "left drag event, x,y: %d,%d\n", Fl::event_x(), Fl::event_y());
            int mouse_x = Fl::event_x() - Fl_Box::x();
            int mouse_y = Fl::event_y() - Fl_Box::y();
            if(mouse_x<0)
                mouse_x = 0;
            if(mouse_y<0)
                mouse_y = 0;
            if(mouse_x>MAX_X)
                mouse_x = MAX_X;
            if(mouse_y>MAX_Y)
                mouse_y = MAX_Y;

            on_left_drag = 1;
            convert_input_to_box();

            //copy top element pixels from slate to screen
            erase_top_element_on_screen(this);

            //get top element node
            node* elem_n = elem_l->head->next;
            //get point list
            list* pt_l = elem_n->point_list;

            //delete all but first point node
            node* first_n = lremfifo(pt_l); //keep first node to insert later
            while (pt_l->head->next != pt_l->tail) //pt_n->next == 0 for dummy tail node
            {
                node *n = lremlifo(pt_l);
                nfree(n);
            }
            linsert(pt_l, first_n);

            //add all rectangle point nodes
            int x1 = pt_l->head->next->pixel.x;
            int y1 = pt_l->head->next->pixel.y;
            int x2 = mouse_x;
            int y2 = mouse_y;
            linepoints(pt_l, x1, y1, x2, y1);
            linepoints(pt_l, x1, y1, x1, y2);
            linepoints(pt_l, x2, y2, x2, y1);
            linepoints(pt_l, x2, y2, x1, y2);

            //draw top element on screen
            draw_top_element_on_screen(this);

            redraw();
            return 1;
        }
        if(e == FL_RELEASE && Fl::event_button() == FL_LEFT_MOUSE)
        {
            on_left_drag = 0;

            return 1;
        }

        //**********
        if(e == FL_PUSH && Fl::event_button() == FL_RIGHT_MOUSE)
        {
            convert_input_to_box();

            redraw();
            return 1;
        }
        if(e == FL_RELEASE && Fl::event_button() == FL_RIGHT_MOUSE)
        {
            //nothing to clean
            return 1;
        }
        //**********
    }

    else if (curr_mode == LINE_MODE)
    {
        if(e == FL_PUSH && Fl::event_button() == FL_LEFT_MOUSE)
        {
            //fprintf(stderr, "left push event, x,y: %d,%d\n", Fl::event_x(), Fl::event_y());
            int mouse_x = Fl::event_x() - Fl_Box::x();
            int mouse_y = Fl::event_y() - Fl_Box::y();
            if(mouse_x<0)
                mouse_x = 0;
            if(mouse_y<0)
                mouse_y = 0;
            if(mouse_x>MAX_X)
                mouse_x = MAX_X;
            if(mouse_y>MAX_Y)
                mouse_y = MAX_Y;

            //create element node
            create_element_node_with_first_point(mouse_x, mouse_y);

            //get top element
            node* elem_n = elem_l->head->next;

            //remove the elem_n
            lremmid(elem_l, elem_n);

            //draw rest all elem_n on slate
            draw_all_elements_on_buffer();

            //now add it to top
            linsert(elem_l, elem_n);

            //draw top element on screen
            draw_top_element_on_screen(this);

            //***********
            convert_input_to_box();

            //create element node
            create_txt_node_with_first_point(mouse_x, mouse_y);
            //***********

            redraw();
            return 1;
        }
        if(e == FL_DRAG && Fl::event_button() == FL_LEFT_MOUSE)
        {
            //fprintf(stderr, "left drag event, x,y: %d,%d\n", Fl::event_x(), Fl::event_y());
            int mouse_x = Fl::event_x() - Fl_Box::x();
            int mouse_y = Fl::event_y() - Fl_Box::y();
            if(mouse_x<0)
                mouse_x = 0;
            if(mouse_y<0)
                mouse_y = 0;
            if(mouse_x>MAX_X)
                mouse_x = MAX_X;
            if(mouse_y>MAX_Y)
                mouse_y = MAX_Y;

            on_left_drag = 1;
            convert_input_to_box();

            //copy top element pixels from slate to screen
            erase_top_element_on_screen(this);

            //get top element node
            node* elem_n = elem_l->head->next;
            //get point list
            list* pt_l = elem_n->point_list;

            //delete all but first point node
            node* first_n = lremfifo(pt_l); //keep first node to insert later
            while (pt_l->head->next != pt_l->tail) //pt_n->next == 0 for dummy tail node
            {
                node *n = lremlifo(pt_l);
                nfree(n);
            }
            linsert(pt_l, first_n);

            //add all middle point nodes
            int x1 = pt_l->head->next->pixel.x;
            int y1 = pt_l->head->next->pixel.y;
            int x2 = mouse_x;
            int y2 = mouse_y;
            linepoints(pt_l, x1, y1, x2, y2);

            //draw top element on screen
            draw_top_element_on_screen(this);

            redraw();
            return 1;
        }
        if(e == FL_RELEASE && Fl::event_button() == FL_LEFT_MOUSE)
        {
            on_left_drag = 0;

            return 1;
        }

        //**********
        if(e == FL_PUSH && Fl::event_button() == FL_RIGHT_MOUSE)
        {
            convert_input_to_box();

            redraw();
            return 1;
        }
        if(e == FL_RELEASE && Fl::event_button() == FL_RIGHT_MOUSE)
        {
            //nothing to clean
            return 1;
        }
        //**********
    }

    else if (curr_mode == ELLIP_MODE)
    {
        if(e == FL_PUSH && Fl::event_button() == FL_LEFT_MOUSE)
        {
            //fprintf(stderr, "left push event, x,y: %d,%d\n", Fl::event_x(), Fl::event_y());
            int mouse_x = Fl::event_x() - Fl_Box::x();
            int mouse_y = Fl::event_y() - Fl_Box::y();
            if(mouse_x<0)
                mouse_x = 0;
            if(mouse_y<0)
                mouse_y = 0;
            if(mouse_x>MAX_X)
                mouse_x = MAX_X;
            if(mouse_y>MAX_Y)
                mouse_y = MAX_Y;

            //create element node
            create_element_node_with_first_point(mouse_x, mouse_y);

            //get top element
            node* elem_n = elem_l->head->next;

            //remove the elem_n
            lremmid(elem_l, elem_n);

            //draw rest all elem_n on slate
            draw_all_elements_on_buffer();

            //now add it to top
            linsert(elem_l, elem_n);

            //draw top element on screen
            draw_top_element_on_screen(this);

            //save starting mouse point
            x_of_ellipse = mouse_x;
            y_of_ellipse = mouse_y;

            //***********
            convert_input_to_box();

            //create element node
            create_txt_node_with_first_point(mouse_x, mouse_y);
            //***********

            redraw();
            return 1;
        }
        if(e == FL_DRAG && Fl::event_button() == FL_LEFT_MOUSE)
        {
            //fprintf(stderr, "left drag event, x,y: %d,%d\n", Fl::event_x(), Fl::event_y());
            int mouse_x = Fl::event_x() - Fl_Box::x();
            int mouse_y = Fl::event_y() - Fl_Box::y();
            if(mouse_x<0)
                mouse_x = 0;
            if(mouse_y<0)
                mouse_y = 0;
            if(mouse_x>MAX_X)
                mouse_x = MAX_X;
            if(mouse_y>MAX_Y)
                mouse_y = MAX_Y;

            on_left_drag = 1;
            convert_input_to_box();

            //copy top element pixels from slate to screen
            erase_top_element_on_screen(this);

            //get top element node
            node* elem_n = elem_l->head->next;
            //get point list
            list* pt_l = elem_n->point_list;

            //delete all point node
            while (pt_l->head->next != pt_l->tail) //pt_n->next == 0 for dummy tail node
            {
                node *n = lremlifo(pt_l);
                nfree(n);
            }

            //add all ellipse point nodes
            int x1 = x_of_ellipse;
            int y1 = y_of_ellipse;
            int x2 = mouse_x;
            int y2 = mouse_y;
            ellipsepoints(pt_l, x1, y1, x2, y2);
            //new_ellipsepoints(pt_l, x1, y1, x2, y2);

            //draw top element on screen
            draw_top_element_on_screen(this);

            redraw();
            return 1;
        }
        if(e == FL_RELEASE && Fl::event_button() == FL_LEFT_MOUSE)
        {
            on_left_drag = 0;

            x_of_ellipse = -1;
            y_of_ellipse = -1;

            return 1;
        }

        //**********
        if(e == FL_PUSH && Fl::event_button() == FL_RIGHT_MOUSE)
        {
            convert_input_to_box();

            redraw();
            return 1;
        }
        if(e == FL_RELEASE && Fl::event_button() == FL_RIGHT_MOUSE)
        {
            //nothing to clean
            return 1;
        }
        //**********
    }

    else if (curr_mode == ARROW_MODE)
    {
        if(e == FL_PUSH && Fl::event_button() == FL_LEFT_MOUSE)
        {
            //fprintf(stderr, "left push event, x,y: %d,%d\n", Fl::event_x(), Fl::event_y());
            int mouse_x = Fl::event_x() - Fl_Box::x();
            int mouse_y = Fl::event_y() - Fl_Box::y();
            if(mouse_x<0)
                mouse_x = 0;
            if(mouse_y<0)
                mouse_y = 0;
            if(mouse_x>MAX_X)
                mouse_x = MAX_X;
            if(mouse_y>MAX_Y)
                mouse_y = MAX_Y;

            //create element node
            create_element_node_with_first_point(mouse_x, mouse_y);

            //get top element
            node* elem_n = elem_l->head->next;

            //remove the elem_n
            lremmid(elem_l, elem_n);

            //draw rest all elem_n on slate
            draw_all_elements_on_buffer();

            //now add it to top
            linsert(elem_l, elem_n);

            //draw top element on screen
            draw_top_element_on_screen(this);

            //***********
            convert_input_to_box();

            //create element node
            create_txt_node_with_first_point(mouse_x, mouse_y);
            //***********

            redraw();
            return 1;
        }
        if(e == FL_DRAG && Fl::event_button() == FL_LEFT_MOUSE)
        {
            //fprintf(stderr, "left drag event, x,y: %d,%d\n", Fl::event_x(), Fl::event_y());
            int mouse_x = Fl::event_x() - Fl_Box::x();
            int mouse_y = Fl::event_y() - Fl_Box::y();
            if(mouse_x<0)
                mouse_x = 0;
            if(mouse_y<0)
                mouse_y = 0;
            if(mouse_x>MAX_X)
                mouse_x = MAX_X;
            if(mouse_y>MAX_Y)
                mouse_y = MAX_Y;

            on_left_drag = 1;
            convert_input_to_box();

            //copy top element pixels from slate to screen
            erase_top_element_on_screen(this);

            //get top element node
            node* elem_n = elem_l->head->next;
            //get point list
            list* pt_l = elem_n->point_list;

            //delete all but first point node
            node* first_n = lremfifo(pt_l); //keep first node to insert later
            while (pt_l->head->next != pt_l->tail) //pt_n->next == 0 for dummy tail node
            {
                node *n = lremlifo(pt_l);
                nfree(n);
            }
            linsert(pt_l, first_n);

            //add all middle point nodes
            int x1 = pt_l->head->next->pixel.x;
            int y1 = pt_l->head->next->pixel.y;
            int x2 = mouse_x;
            int y2 = mouse_y;
            linepoints(pt_l, x1, y1, x2, y2);

            //add arrow head line points
            double t = angle_of_rotation(x1, y1, x2, y2);
            //printf("angle_of_rotation = %f\n", t);
            double r = sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
            //printf("x1, y1 = %d, %d\n", x1, y1);
            //printf("x2, y2 = %d, %d\n", x2, y2);
            //printf("r = %f\n", r);

            //int p1x, p1y, rotated_p1x, rotated_p1y;
            //p1x = r; p1y = 0;
            //globalize(x1, y1, t, p1x, p1y, &rotated_p1x, &rotated_p1y);
            //printf("p1x, p1y = %d, %d\n", p1x, p1y);
            //printf("rotated p1x, p1y = %d, %d\n", rotated_p1x, rotated_p1y);

            int p1x, p1y, rotated_p1x, rotated_p1y;
            p1x = r-15; p1y = 0-6;
            globalize(x1, y1, t, p1x, p1y, &rotated_p1x, &rotated_p1y);
            //printf("p1x, p1y = %d, %d\n", p1x, p1y);
            //printf("rotated p1x, p1y = %d, %d\n", rotated_p1x, rotated_p1y);

            int p2x, p2y, rotated_p2x, rotated_p2y;
            p2x = r-15; p2y = 0+6;
            globalize(x1, y1, t, p2x, p2y, &rotated_p2x, &rotated_p2y);
            //printf("p2x, p2y = %d, %d\n", p2x, p2y);
            //printf("rotated p2x, p2y = %d, %d\n", rotated_p2x, rotated_p2y);

            linepoints(pt_l, rotated_p1x, rotated_p1y, x2, y2);
            linepoints(pt_l, rotated_p2x, rotated_p2y, x2, y2); //TODO

            //draw top element on screen
            draw_top_element_on_screen(this);

            redraw();
            return 1;
        }
        if(e == FL_RELEASE && Fl::event_button() == FL_LEFT_MOUSE)
        {
            //fprintf(stderr, "left release event, x,y: %d,%d\n", Fl::event_x(), Fl::event_y());

            on_left_drag = 0;

            return 1;
        }

        //**********
        if(e == FL_PUSH && Fl::event_button() == FL_RIGHT_MOUSE)
        {
            convert_input_to_box();

            redraw();
            return 1;
        }
        if(e == FL_RELEASE && Fl::event_button() == FL_RIGHT_MOUSE)
        {
            //nothing to clean
            return 1;
        }
        //**********
    }

    return(Fl_Box::handle(e));
}

void ImgBox::draw()
{
    //printf("draw!\n");

    //draw parent widget
    Fl_Box::draw();

    //int r = 255;
    //int g=0;
    //int b=0;
    //fl_color(fl_rgb_color(r, g, b));

    //int x1 = Fl_Box::x()+1; //this is our 0 for drawing
    //int y1 = Fl_Box::y()+1;
    //int x2 = Fl_Box::x()+Fl_Box::w()-1 -1; //this is our 599 for drawing
    //int y2 = Fl_Box::y()+Fl_Box::h()-1 -1;
    //fl_line(x1, y1, x2, y1);
    //fl_line(x1, y1, x1, y2);
    //fl_line(x2, y2, x2, y1);
    //fl_line(x2, y2, x1, y2);

    //fl_color(FL_BLACK);
    //ellipsepoints(NULL, 10, 10, 50, 10);
    //fl_color(FL_RED);
    //ellipsepoints(NULL, 10, 10, 50, 20);
    //fl_color(FL_GREEN);
    //ellipsepoints(NULL, 10, 10, 50, 30);
    //fl_color(FL_BLUE);
    //ellipsepoints(NULL, 10, 10, 50, 40);
    //fl_color(FL_RED);
    //ellipsepoints(NULL, 10, 10, 50, 50);
    //fl_color(FL_BLACK);
    //ellipsepoints(NULL, 10, 10, 40, 50);
    //fl_color(FL_RED);
    //ellipsepoints(NULL, 10, 10, 30, 50);
    //fl_color(FL_GREEN);
    //ellipsepoints(NULL, 10, 10, 20, 50);
    //fl_color(FL_BLUE);
    //ellipsepoints(NULL, 10, 10, 10, 50);

    //slate[5][5] = red;
    //slate[5][6] = red;
    //slate[6][5] = red;
    //slate[6][6] = red;
    //fl_draw_image((const uchar*)&slate, x()+1, y()+1, MAX_X, MAX_Y, 4, MAX_X*4);

/*
    //loop all elem_n
    node* elem_n = elem_l->head->next;
    while (elem_n->next != 0) //elem_n->next == 0 for dummy tail node
    {
        Fl_Color col = elem_n->color;
        list* pt_l = elem_n->point_list;
        //loop all pt_n
        node* pt_n = pt_l->head->next;
        while (pt_n->next != 0) //pt_n->next == 0 for dummy tail node
        {
            fl_color(col);
            fl_point(x()+pt_n->pixel.x, y()+pt_n->pixel.y);

            pt_n = pt_n->next;
        }

        elem_n = elem_n->next;
    }
*/

    if(prog_start == 1)
    {
        memset((unsigned char*)&slate, 255, (MAX_Y+1)*(MAX_X+1)*4);
        for(int i=3; i<(MAX_Y+1)*(MAX_X+1)*4; i+=4)
        {
            *((unsigned char*)&slate + i) = 0;
        }
        fl_draw_image((const uchar*)&slate, x(), y(), (MAX_X+1), (MAX_Y+1), 4, (MAX_X+1)*4);
        prog_start = 0;
    }

    if(show_event==1
    || txt_box_resized == 1)
    {
        if(on_left_drag == 1)
        {
            //don't display top element
            //because it is half constructed

            //get top element
            node* elem_n = elem_l->head->next;

            //remove the elem_n
            lremmid(elem_l, elem_n);

            //draw rest all elem_n on slate
            draw_all_elements_on_buffer();

            //now add it to top
            linsert(elem_l, elem_n);
        }
        else
        {
            //draw all (truly all) elements on slate
            draw_all_elements_on_buffer();
        }

        //copy slate to screen
        fl_draw_image((const uchar*)&slate, x(), y(), (MAX_X+1), (MAX_Y+1), 4, (MAX_X+1)*4);

        if (show_event == 1)
            show_event=0;
        if(txt_box_resized == 1)
            txt_box_resized = 0;
    }

    //in all other cases,
    //in addition to the above
    //draw these

    //loop all txt_n
    node* txt_n = txt_l->head->next;
    while (txt_n->next != 0) //txt_n->next == 0 for dummy tail node
    {
        txt_n->tbox->redraw();
        txt_n = txt_n->next;
    }

    if(intxt != NULL)
        intxt->redraw();
}

TxtBox::TxtBox(int x, int y, int w, int h):Fl_Box(x, y, w, h)
{
}

//find_txt_node
node* find_txt_node(int pos_x, int pos_y)
{
    int found = 0;

    //loop all txt node
    node* txt_n = txt_l->head->next;
    while (txt_n->next != 0) //txt_n->next == 0 for dummy tail node
    {
        if(txt_n->tbox->x() == pos_x &&
           txt_n->tbox->y() == pos_y)
        {
            found = 1; break;
        }
        txt_n = txt_n->next;
    }

    if(found == 1)
        return txt_n;
    else
        return NULL;
}

int TxtBox::handle(int e)
{
    if(curr_mode == MOV_DEL_MODE)
    {
        if(e == FL_PUSH && Fl::event_button() == FL_LEFT_MOUSE)
        {
            //fprintf(stderr, "text box left push event, x,y: %d,%d\n", Fl::event_x(), Fl::event_y());
            //fprintf(stderr, "text box, x,y: %d,%d\n", Fl_Box::x(), Fl_Box::y());

            int mouse_x = Fl::event_x() - ::w.ibox->x();
            int mouse_y = Fl::event_y() - ::w.ibox->y();
            //printf("push mouse x, y = %d, %d\n", mouse_x, mouse_y);

            if(mouse_x<0)
                mouse_x = 0;
            if(mouse_y<0)
                mouse_y = 0;
            if(mouse_x>MAX_X)
                mouse_x = MAX_X;
            if(mouse_y>MAX_Y)
                mouse_y = MAX_Y;

            node* found_txt = find_txt_node(Fl_Box::x(), Fl_Box::y());
            if(found_txt != NULL)
            {
                //printf("found text box x, y = %d, %d\n", found_txt->tbox->x(), found_txt->tbox->y());
                //remove the txt_n
                lremmid(txt_l, found_txt);
                ::w.window->remove(found_txt->tbox);

                //now add it to top
                linsert(txt_l, found_txt);
                ::w.window->add(found_txt->tbox);

                txt_grabbed_to_move = found_txt;
                txt_x_grabbed_to_move = mouse_x;
                txt_y_grabbed_to_move = mouse_y;
                prep_buf_for_txt_drag = 1;
            }
            else
            {
                txt_grabbed_to_move = 0;
                txt_x_grabbed_to_move = -1;
                txt_y_grabbed_to_move = -1;
                prep_buf_for_txt_drag = 0;
            }

            redraw();
            return 1;
        }
        if(e == FL_DRAG && Fl::event_button() == FL_LEFT_MOUSE)
        {
            //fprintf(stderr, "text box left drag event, x,y: %d,%d\n", Fl::event_x(), Fl::event_y());
            //fprintf(stderr, "text box, x,y: %d,%d\n", Fl_Box::x(), Fl_Box::y());

            if (txt_grabbed_to_move != 0)
            {
                int mouse_x = Fl::event_x() - ::w.ibox->x();
                int mouse_y = Fl::event_y() - ::w.ibox->y();
                //printf("new mouse x, y = %d, %d\n", mouse_x, mouse_y);

                if(mouse_x<0)
                    mouse_x = 0;
                if(mouse_y<0)
                    mouse_y = 0;
                if(mouse_x>MAX_X)
                    mouse_x = MAX_X;
                if(mouse_y>MAX_Y)
                    mouse_y = MAX_Y;

                int dx = mouse_x - txt_x_grabbed_to_move;
                int dy = mouse_y - txt_y_grabbed_to_move;

                //for next drag event
                //txt_grabbed_to_move remains same
                txt_x_grabbed_to_move = mouse_x;
                txt_y_grabbed_to_move = mouse_y;

                int box_x = x() + dx; //new position of text box
                int box_y = y() + dy;

                if(prep_buf_for_txt_drag == 1)
                {
                    draw_all_elements_on_buffer();
                    prep_buf_for_txt_drag = 0;
                }

                copy_area_from_buf_to_screen(     x()    - ::w.ibox->x(),      y()    - ::w.ibox->y(),
                                             (x()+w()-1) - ::w.ibox->x(), (y()+h()-1) - ::w.ibox->y()     );

                //txt_grabbed_to_move->tbox->position(box_x, box_y);
                reposition_box(txt_grabbed_to_move->tbox, box_x, box_y);
            }

            redraw();
            return 1;
        }
        if(e == FL_RELEASE && Fl::event_button() == FL_LEFT_MOUSE)
        {
            if(txt_grabbed_to_move != 0)
            {
                txt_grabbed_to_move = 0;
                txt_x_grabbed_to_move = -1;
                txt_y_grabbed_to_move = -1;
                prep_buf_for_txt_drag = 0;
            }
            ::w.ibox->redraw();
            return 1;
        }
        //*************
        if(e == FL_PUSH && Fl::event_button() == FL_RIGHT_MOUSE)
        {
            //fprintf(stderr, "text box right push event, x,y: %d,%d\n", Fl::event_x(), Fl::event_y());
            //fprintf(stderr, "text box, x,y: %d,%d\n", Fl_Box::x(), Fl_Box::y());

            node* found_txt = find_txt_node(Fl_Box::x(), Fl_Box::y());
            if(found_txt != NULL)
            {
                //printf("found text box x, y = %d, %d\n", found_txt->tbox->x(), found_txt->tbox->y());

                draw_all_elements_on_buffer();
                copy_area_from_buf_to_screen(     x()    - ::w.ibox->x(),      y()    - ::w.ibox->y(),
                                             (x()+w()-1) - ::w.ibox->x(), (y()+h()-1) - ::w.ibox->y()     );

                txt_grabbed_to_del = found_txt;
            }
            else
            {
                //nothing to do
                txt_grabbed_to_del = NULL;
            }

            //redraw();
            return 1;
        }
        if(e == FL_RELEASE && Fl::event_button() == FL_RIGHT_MOUSE)
        {
            if(txt_grabbed_to_del != NULL)
            {
                //remove the txt_n
                lremmid(txt_l, txt_grabbed_to_del);
                ::w.window->remove(txt_grabbed_to_del->tbox);
                free(txt_grabbed_to_del->txt);
                Fl::delete_widget(txt_grabbed_to_del->tbox); //safe widget delete
                nfree(txt_grabbed_to_del);
            }

            ::w.ibox->redraw();
            return 1;
        }
    }

    return(Fl_Box::handle(e));
}

void TxtBox::draw()
{
    //draw parent widget
    Fl_Box::draw();
}

//******************

/*
void copy_slate_to_snap()
{
    memcpy((unsigned char*)&snap, (unsigned char*)&slate, MAX_Y*MAX_X*4); //dst, src, n
}

void copy_snap_to_slate()
{
    memcpy((unsigned char*)&slate, (unsigned char*)&snap, MAX_Y*MAX_X*4); //dst, src, n
}
*/

void reposition_box(Fl_Widget* box, int new_x, int new_y) //new_x is wrt app window
{
    int final_x = new_x;
    int final_y = new_y;

    int max_x = ::w.ibox->x()+MAX_X-(box->w()-1); //maximum limit
    int max_y = ::w.ibox->y()+MAX_Y-(box->h()-1);

    if(final_x < ::w.ibox->x())
        final_x = ::w.ibox->x(); //minimum limit
    if(final_y < ::w.ibox->y())
        final_y = ::w.ibox->y();
    if(final_x > max_x)
        final_x = max_x;
    if(final_y > max_y)
        final_y = max_y;

    box->position(final_x, final_y);
}

//draw all elements on slate and image box if not null
void draw_all_elements_on_buffer()
{
    //loop all elem_n
    node* elem_n = elem_l->head->next; //leave the top element
    while (elem_n->next != 0) //elem_n->next == 0 for dummy tail node
    {
        Fl_Color col = elem_n->color;
        //get point list
        list* pt_l = elem_n->point_list;
        //loop all pt_n
        node* pt_n = pt_l->head->next;
        while (pt_n->next != 0) //pt_n->next == 0 for dummy tail node
        {
            if(pt_n->pixel.x >=0 && pt_n->pixel.x <= MAX_X
            && pt_n->pixel.y >=0 && pt_n->pixel.y <= MAX_Y)
            {
                pix* pixcol = get_pix_rgb_from_col_name(col);
                *( (pix*)&slate + (pt_n->pixel.y * (MAX_X+1)) + (pt_n->pixel.x) ) = *pixcol;
            }
            pt_n = pt_n->next;
        }

        elem_n = elem_n->next;
    }
}

//draw top element on slate or image box
void draw_top_element_on_screen(ImgBox* imgbox)
{
    //get top element
    node* elem_n = elem_l->head->next;
    Fl_Color col = elem_n->color;
    //get point list
    list* pt_l = elem_n->point_list;
    //loop all pt_n
    node* pt_n = pt_l->head->next;
    while (pt_n->next != 0) //pt_n->next == 0 for dummy tail node
    {
        if(imgbox != NULL)
        {
            if(pt_n->pixel.x >=0 && pt_n->pixel.x <= MAX_X
            && pt_n->pixel.y >=0 && pt_n->pixel.y <= MAX_Y)
            {
                fl_color(col);
                fl_point(imgbox->x()+pt_n->pixel.x, imgbox->y()+pt_n->pixel.y);
            }
        }

        pt_n = pt_n->next;
    }
}

//undraw top element on image box
void erase_top_element_on_screen(ImgBox* imgbox)
{
    //get top element
    node* elem_n = elem_l->head->next;
    //get point list
    list* pt_l = elem_n->point_list;
    //loop all pt_n
    node* pt_n = pt_l->head->next;
    while (pt_n->next != 0) //pt_n->next == 0 for dummy tail node
    {
        if(imgbox != NULL)
        {
            if(pt_n->pixel.x >=0 && pt_n->pixel.x <= MAX_X
            && pt_n->pixel.y >=0 && pt_n->pixel.y <= MAX_Y)
            {
                pix* valp = (pix*)&slate + (pt_n->pixel.y * (MAX_X+1)) + (pt_n->pixel.x);
                //printf("valp = %d, %d, %d, %d\n", valp->r, valp->g, valp->b, valp->a);
                Fl_Color col = get_col_name_from_pix_rgb(valp);
                fl_color(col);
                fl_point(imgbox->x()+pt_n->pixel.x, imgbox->y()+pt_n->pixel.y);
            }
        }

        pt_n = pt_n->next;
    }
}

//copy area from buf to screen
void copy_area_from_buf_to_screen(int x1, int y1, int x2, int y2)
{
    int x;
    int y;
    for(x=x1; x<=x2; x++)
    {
        for(y=y1; y<=y2; y++)
        {
            if(x >= 0 && x <= MAX_X
            && y >= 0 && y <= MAX_Y)
            {
                pix* valp = (pix*)&slate + (y*(MAX_X+1)) + (x);
                //printf("valp = %d, %d, %d, %d\n", valp->r, valp->g, valp->b, valp->a);
                Fl_Color col = get_col_name_from_pix_rgb(valp);
                fl_color(col);
                fl_point(::w.ibox->x()+x, ::w.ibox->y()+y);
            }
        }
    }
}

//draw top element pixels in white on slate
void erase_top_element_on_buffer()
{
    Fl_Color col = FL_WHITE;

    //get top element
    node* elem_n = elem_l->head->next;
    //get point list
    list* pt_l = elem_n->point_list;
    //loop all pt_n
    node* pt_n = pt_l->head->next;
    while (pt_n->next != 0) //pt_n->next == 0 for dummy tail node
    {
        if(pt_n->pixel.x >=0 && pt_n->pixel.x <= MAX_X
        && pt_n->pixel.y >=0 && pt_n->pixel.y <= MAX_Y)
        {
            pix* pixcol = get_pix_rgb_from_col_name(col);
            //printf("on buf %d %d %d %d\n", pixcol->r, pixcol->g, pixcol->b, pixcol->a);
            *( (pix*)&slate + (pt_n->pixel.y * (MAX_X+1)) + (pt_n->pixel.x) ) = *pixcol;
        }

        pt_n = pt_n->next;
    }
}

//******************

void mov_delb_callback(Fl_Widget* wd, void* data)
{
    if(intxt != NULL)
    {
        convert_input_to_box();
        ::w.ibox->redraw();
    }

    if(curr_mode != MOV_DEL_MODE)
        prev_mode = curr_mode;

    curr_mode = MOV_DEL_MODE;
    reset_all_button_color();
    w.mov_delb->color(curr_color);
    Fl::focus(w.mov_delb);
    w.mov_delb->redraw();
}

void penb_callback(Fl_Widget* wd, void* data)
{
    if(intxt != NULL)
    {
        convert_input_to_box();
        ::w.ibox->redraw();
    }

    prev_mode = MOV_DEL_MODE;

    if (curr_mode == PEN_MODE)
    {
        set_next_color(curr_color); //chose not current color
        w.penb->color(curr_color);
        w.penb->redraw();
    }

    curr_mode = PEN_MODE;
    reset_all_button_color();
    w.penb->color(curr_color);
    Fl::focus(w.penb);
    w.penb->redraw();
}

void rectb_callback(Fl_Widget* wd, void* data)
{
    if(intxt != NULL)
    {
        convert_input_to_box();
        ::w.ibox->redraw();
    }

    prev_mode = MOV_DEL_MODE;

    if (curr_mode == RECT_MODE)
    {
        set_next_color(curr_color);
        w.rectb->color(curr_color);
        w.rectb->redraw();
    }

    curr_mode = RECT_MODE;
    reset_all_button_color();
    w.rectb->color(curr_color);
    Fl::focus(w.rectb);
    w.rectb->redraw();
}

void lineb_callback(Fl_Widget* wd, void* data)
{
    if(intxt != NULL)
    {
        convert_input_to_box();
        ::w.ibox->redraw();
    }

    prev_mode = MOV_DEL_MODE;

    if (curr_mode == LINE_MODE)
    {
        set_next_color(curr_color);
        w.lineb->color(curr_color);
        w.lineb->redraw();
    }

    curr_mode = LINE_MODE;
    reset_all_button_color();
    w.lineb->color(curr_color);
    Fl::focus(w.lineb);
    w.lineb->redraw();
}

void ellipb_callback(Fl_Widget* wd, void* data)
{
    if(intxt != NULL)
    {
        convert_input_to_box();
        ::w.ibox->redraw();
    }

    prev_mode = MOV_DEL_MODE;

    if (curr_mode == ELLIP_MODE)
    {
        set_next_color(curr_color);
        w.ellipb->color(curr_color);
        w.ellipb->redraw();
    }

    curr_mode = ELLIP_MODE;
    reset_all_button_color();
    w.ellipb->color(curr_color);
    Fl::focus(w.ellipb);
    w.ellipb->redraw();
}

void arrowb_callback(Fl_Widget* wd, void* data)
{
    if(intxt != NULL)
    {
        convert_input_to_box();
        ::w.ibox->redraw();
    }

    prev_mode = MOV_DEL_MODE;

    if (curr_mode == ARROW_MODE)
    {
        set_next_color(curr_color);
        w.arrowb->color(curr_color);
        w.arrowb->redraw();
    }

    curr_mode = ARROW_MODE;
    reset_all_button_color();
    w.arrowb->color(curr_color);
    Fl::focus(w.arrowb);
    w.arrowb->redraw();
}

void saveb_callback(Fl_Widget* wd, void* data)
{
    if(intxt != NULL)
    {
        convert_input_to_box();
        ::w.ibox->redraw();
    }

    save_to_file(0); //app not closed
    fl_alert("iMage file saved!");
}

void mainwin_callback(Fl_Widget* wd, void* data)
{
    if(intxt != NULL)
    {
        convert_input_to_box();
        ::w.ibox->redraw();
    }

    if (Fl::event()==FL_SHORTCUT && Fl::event_key()==FL_Escape)
    {
        if(curr_mode != MOV_DEL_MODE)
        {
            mov_delb_callback(0, 0);
        }
        else //curr_mode == MOV_DEL_MODE
        {
            if(prev_mode == PEN_MODE)
            {
                penb_callback(0, 0);
            }
            if(prev_mode == RECT_MODE)
            {
                rectb_callback(0, 0);
            }
            if(prev_mode == LINE_MODE)
            {
                lineb_callback(0, 0);
            }
            if(prev_mode == ELLIP_MODE)
            {
                ellipb_callback(0, 0);
            }
            if(prev_mode == ARROW_MODE)
            {
                arrowb_callback(0, 0);
            }
        }

        //force refresh
        show_event = 1;
        w.ibox->redraw();

        return; // ignore Escape
    }
    else
    {
        switch( fl_choice("Save work now?", "Yes", "No", 0) )
        {
            case 0: // Yes
                save_to_file(1); //app closed
                exit(0);
            case 1: // No (default)
                exit(0); //to exit
                //break; //to continue the app
        }
    }
}

void helpb_callback(Fl_Widget* wd, void* data)
{
    fl_alert("Help!\n"
    "\n"
    "In Pen, Line, Arrow, Rectangle and Ellipse mode,\n"
    "Left Click and Drag: Creates the iMage object.\n"
    "Left Click: Shows single line text editor.\n"
    "Right Click: Converts text editor to text box.\n"
    "More Clicks on mode buttons: Gives colors.\n"
    "\n"
    "In Move Delete Mode,\n"
    "Left Click and Drag: Moves the iMage object.\n"
    "Right Click: Deletes the iMage object.\n"
    "\n"
    "Escape Key: Toggles between Mov,Del mode and others;\n"
    "and Refreshes the screen when needed.\n"
    "\n"
    "Save Button: Saves iMage files to Windows Desktop,\n"
    "Or Linux Working Directory.\n"
    "\n"
    "About Button: Shows iMage version, Author email,\n"
    "and Copyright information.\n"
    );
}

void aboutb_callback(Fl_Widget* wd, void* data)
{
    fl_alert("About iMage!\n"
    "\n"
    "iMage\n"
    "version: 1.0\n"
    "\n"
    "Author: Srinivas Nayak\n"
    "Email: sinu.nayak2001 at gmail.com\n"
    "\n"
    "Copyright (c) Srinivas Nayak - All rights reserved.\n"
    "\n"
    "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND,\n"
    "EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE\n"
    "WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE\n"
    "AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS\n"
    "BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,\n"
    "WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,\n"
    "ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE\n"
    "OR OTHER DEALINGS IN THE SOFTWARE.\n"
    );

}

//******************

void set_next_color(Fl_Color color)
{
    if(color == FL_RED)
        curr_color = FL_GREEN;
    if(color == FL_GREEN)
        curr_color = FL_BLUE;
    if(color == FL_BLUE)
        curr_color = FL_RED;
}

void reset_all_button_color()
{
    //FL_BACKGROUND_COLOR
    w.mov_delb->color(FL_BACKGROUND_COLOR);
    w.mov_delb->redraw();
    w.penb->color(FL_BACKGROUND_COLOR);
    w.penb->redraw();
    w.rectb->color(FL_BACKGROUND_COLOR);
    w.rectb->redraw();
    w.lineb->color(FL_BACKGROUND_COLOR);
    w.lineb->redraw();
    w.ellipb->color(FL_BACKGROUND_COLOR);
    w.ellipb->redraw();
    w.arrowb->color(FL_BACKGROUND_COLOR);
    w.arrowb->redraw();
}

pix* get_pix_rgb_from_col_name(Fl_Color col)
{
    pix* pptr = &white;;
    if(col == FL_RED)
        pptr = &red;
    if(col == FL_GREEN)
        pptr = &green;
    if(col == FL_BLUE)
        pptr = &blue;
    return pptr;
}

Fl_Color get_col_name_from_pix_rgb(pix* pix_col)
{
    Fl_Color col = FL_WHITE;
    if(pix_col->r == red.r && pix_col->g == red.g && pix_col->b == red.b)
        col = FL_RED;
    if(pix_col->r == green.r && pix_col->g == green.g && pix_col->b == green.b)
        col = FL_GREEN;
    if(pix_col->r == blue.r && pix_col->g == blue.g && pix_col->b == blue.b)
        col = FL_BLUE;
    return col;
}

//bresenham line for octant 1, 4, 5, 8
void linemostlyhoriz(list* pt_l, int x1, int y1, int x2, int y2)
{
    int dx=(x2-x1);
    int dy=(y2-y1);

    int adx=abs(x2-x1);
    int ady=abs(y2-y1);

    int p = 2*ady - adx;
    int twoady = 2*ady;
    int twoadyadx = 2*(ady - adx);

    int sx, sy, ex, ey; //start/end x and y

    if(dx>0)
        {sx=x1; sy=y1; ex=x2; ey=y2;}
    else
        {sx=x2; sy=y2; ex=x1; ey=y1;} //exchange

    dx=(ex-sx);
    dy=(ey-sy);

    putpix(pt_l, sx, sy);
    while(sx<ex)
    {
        if(p<0)
        {
            sx++;
            p += twoady;
        }
        else
        {
            sx++;
            if(dy>0)
                sy++;
            else
                sy--;
            p += twoadyadx;
        }
        putpix(pt_l, sx, sy);
    }
}

//bresenham line for octant 2, 3, 6, 7
void linemostlyvert(list* pt_l, int x1, int y1, int x2, int y2)
{
    int dx=(x2-x1);
    int dy=(y2-y1);

    int adx=abs(x2-x1);
    int ady=abs(y2-y1);

    int p = 2*adx - ady;
    int twoadx = 2*adx;
    int twoadxady = 2*(adx - ady);

    int sx, sy, ex, ey; //start/end x and y

    if(dy>0)
        {sx=x1; sy=y1; ex=x2; ey=y2;}
    else
        {sx=x2; sy=y2; ex=x1; ey=y1;} //exchange

    dx=(ex-sx);
    dy=(ey-sy);

    putpix(pt_l, sx, sy);
    while(sy<ey)
    {
        if(p<0)
        {
            sy++;
            p += twoadx;
        }
        else
        {
            sy++;
            if(dx>0)
                sx++;
            else
                sx--;
            p += twoadxady;
        }
        putpix(pt_l, sx, sy);
    }
}

void linepoints(list* pt_l, int x1, int y1, int x2, int y2)
{
    int dx=(x2-x1);
    int dy=(y2-y1);

    int adx=abs(x2-x1);
    int ady=abs(y2-y1);

    int sx, sy, ex, ey; //start/end x and y

    if(dy==0)
    {
        if(dx>0)
            {sx=x1; sy=y1; ex=x2; ey=y2;}
        else
            {sx=x2; sy=y2; ex=x1; ey=y1;} //exchange

        int x;
        for(x=sx; x<=ex; x++)
            {putpix(pt_l, x, sy);}
    }
    else if(dx==0)
    {
        if(dy>0)
            {sx=x1; sy=y1; ex=x2; ey=y2;}
        else
            {sx=x2; sy=y2; ex=x1; ey=y1;} //exchange

        int y;
        for(y=sy; y<=ey; y++)
            {putpix(pt_l, sx, y);}
    }
    else if(adx>=ady)
    {
        linemostlyhoriz(pt_l, x1, y1, x2, y2);
    }
    else //adx<ady
    {
        linemostlyvert(pt_l, x1, y1, x2, y2);
    }
}

void check_n_put(list* pt_l, int x, int y)
{
    //if(x<0||x>MAX_X||y<0||y>MAX_Y)
        //printf("check_n_put bad node: %d, %d\n", x, y);

    if(x<0)
        x = 0;
    if(y<0)
        y = 0;
    if(x>MAX_X)
        x = MAX_X;
    if(y>MAX_Y)
        y = MAX_Y;

    node* pt_n;
    pt_n = nalloc(); ninitpix(pt_n, x, y); linsert(pt_l, pt_n);
}

void putpix(list* pt_l, int x, int y)
{
    //Fl_Box* box = w.ibox;
    //fl_point(box->x()+x, box->y()+y);
    //printf("draw %d, %d at %d, %d\n", x, y, box->x()+x, box->y()+y);

    //fl_color(col);
    //fl_point(box->x()+x-1, box->y()+y-1);
    //fl_point(box->x()+x-1, box->y()+y);
    //fl_point(box->x()+x-1, box->y()+y+1);
    //fl_point(box->x()+x, box->y()+y-1);
    //fl_point(box->x()+x, box->y()+y);
    //fl_point(box->x()+x, box->y()+y+1);
    //fl_point(box->x()+x+1, box->y()+y-1);
    //fl_point(box->x()+x+1, box->y()+y);
    //fl_point(box->x()+x+1, box->y()+y+1);
    //printf("draw %d, %d\n", x, y);

    //below lines commented out just for optimization
    check_n_put(pt_l, x-1, y-1);
    check_n_put(pt_l, x-1, y);
    check_n_put(pt_l, x-1, y+1);
    check_n_put(pt_l, x, y-1);
    check_n_put(pt_l, x, y);
    check_n_put(pt_l, x, y+1);
    check_n_put(pt_l, x+1, y-1);
    check_n_put(pt_l, x+1, y);
    check_n_put(pt_l, x+1, y+1);
}

//draw 4 symmetrical ellipse points
void ellipsesympoints(list* pt_l, int midx, int midy, int x, int y)
{
    putpix(pt_l, midx+x, midy+y);
    putpix(pt_l, midx+x, midy-y);
    putpix(pt_l, midx-x, midy+y);
    putpix(pt_l, midx-x, midy-y);
}

//bresenham ellipse
void ellipsemostlyhoriz(list* pt_l, int x1, int y1, int x2, int y2)
{
    int dx=(x2-x1);
    int dy=(y2-y1);

    int sx, sy, ex, ey; //start/end x and y

    if(dx>0)
        {sx=x1; sy=y1; ex=x2; ey=y2;}
    else
        {sx=x2; sy=y2; ex=x1; ey=y1;} //exchange

    dx=(ex-sx);
    dy=(ey-sy);

    if(dy>0)
        {} //no exchange
    else
        {int t; t=sy; sy=ey; ey=t;} //exchange

    dx=(ex-sx);
    dy=(ey-sy);

    double a=dx/2; //major radius
    double b=dy/2; //minor radius

    int midx=(sx+ex)/2;
    int midy=(sy+ey)/2;

    double x=0;
    double y=b;

    ellipsesympoints(pt_l, midx, midy, x, y);
    double d1=b*b-(a*a*b)+(0.25*a*a);
    while(a*a*(y-0.5) > b*b*(x+1)) //2nd octant
    {
        if(d1<0)
        {
            d1+=b*b*(2*x+3);
        }
        else
        {
            d1+=b*b*(2*x+3)+a*a*(-2*y+2);
            y--;
        }
        x++;
        ellipsesympoints(pt_l, midx, midy, x, y);
    }

    double d2=b*b*(x+0.5)*(x+0.5)+a*a*(y-1)*(y-1)-a*a*b*b;
    while(y>0) //1st octant
    {
        if(d2<0)
        {
            d2+=b*b*(2*x+2)+a*a*(-2*y+3);
            x++;
        }
        else
        {
            d2+=a*a*(-2*y+3);
        }
        y--;
        ellipsesympoints(pt_l, midx, midy, x, y);
    }
}

//bresenham ellipse
void ellipsemostlyvert(list* pt_l, int x1, int y1, int x2, int y2)
{
    int dx=(x2-x1);
    int dy=(y2-y1);

    int sx, sy, ex, ey; //start/end x and y

    if(dx>0)
        {sx=x1; sy=y1; ex=x2; ey=y2;}
    else
        {sx=x2; sy=y2; ex=x1; ey=y1;} //exchange

    dx=(ex-sx);
    dy=(ey-sy);

    if(dy>0)
        {} //no exchange
    else
        {int t; t=sy; sy=ey; ey=t;} //exchange

    dx=(ex-sx);
    dy=(ey-sy);

    double a=dy/2; //major radius
    double b=dx/2; //minor radius

    int midx=(sx+ex)/2;
    int midy=(sy+ey)/2;

    double y=0;
    double x=b;

    ellipsesympoints(pt_l, midx, midy, x, y);
    double d1=b*b-(a*a*b)+(0.25*a*a);
    while(a*a*(x-0.5) > b*b*(y+1)) //2nd octant
    {
        if(d1<0)
        {
            d1+=b*b*(2*y+3);
        }
        else
        {
            d1+=b*b*(2*y+3)+a*a*(-2*x+2);
            x--;
        }
        y++;
        ellipsesympoints(pt_l, midx, midy, x, y);
    }

    double d2=b*b*(y+0.5)*(y+0.5)+a*a*(x-1)*(x-1)-a*a*b*b;
    while(x>0) //1st octant
    {
        if(d2<0)
        {
            d2+=b*b*(2*y+2)+a*a*(-2*x+3);
            y++;
        }
        else
        {
            d2+=a*a*(-2*x+3);
        }
        x--;
        ellipsesympoints(pt_l, midx, midy, x, y);
    }
}

void ellipsepoints(list* pt_l, int x1, int y1, int x2, int y2)
{
    int dx=(x2-x1);
    int dy=(y2-y1);

    int adx=abs(x2-x1);
    int ady=abs(y2-y1);

    int sx, sy, ex, ey; //start/end x and y

    if(dy==0)
    {
        if(dx>0)
            {sx=x1; sy=y1; ex=x2; ey=y2;}
        else
            {sx=x2; sy=y2; ex=x1; ey=y1;} //exchange

        int x;
        for(x=sx; x<=ex; x++)
            {putpix(pt_l, x, sy);}
    }
    else if(dx==0)
    {
        if(dy>0)
            {sx=x1; sy=y1; ex=x2; ey=y2;}
        else
            {sx=x2; sy=y2; ex=x1; ey=y1;} //exchange

        int y;
        for(y=sy; y<=ey; y++)
            {putpix(pt_l, sx, y);}
    }
    else if(adx>=ady)
    {
        ellipsemostlyhoriz(pt_l, x1, y1, x2, y2);
    }
    else //adx<ady
    {
        ellipsemostlyvert(pt_l, x1, y1, x2, y2);
    }
}

void new_ellipsepoints(list* pt_l, int x1, int y1, int x2, int y2)
{
    int sx, sy, ex, ey; //start/end x and y

    int dx=(x2-x1);
    int dy=(y2-y1);

    if(dx>0)
        {sx=x1; sy=y1; ex=x2; ey=y2;}
    else
        {sx=x2; sy=y2; ex=x1; ey=y1;} //exchange

    dx=(ex-sx);
    dy=(ey-sy);

    if(dy>0)
        {} //no exchange
    else
        {int t; t=sy; sy=ey; ey=t;} //exchange

    dx=(ex-sx);
    dy=(ey-sy);

    if(dy==0)
    {
        if(dx>0)
            {sx=x1; sy=y1; ex=x2; ey=y2;}
        else
            {sx=x2; sy=y2; ex=x1; ey=y1;} //exchange

        int x;
        for(x=sx; x<=ex; x++)
            {putpix(pt_l, x, sy);}
    }
    else if(dx==0)
    {
        if(dy>0)
            {sx=x1; sy=y1; ex=x2; ey=y2;}
        else
            {sx=x2; sy=y2; ex=x1; ey=y1;} //exchange

        int y;
        for(y=sy; y<=ey; y++)
            {putpix(pt_l, sx, y);}
    }

    double a=dx/2; //major radius
    double b=dy/2; //minor radius

    int midx=(sx+ex)/2;
    int midy=(sy+ey)/2;

    double y=0;
    double x=0;

    for(x=0; x<=a; x++)
    {
        y = sqrt( ((a*a*b*b - b*b*x*x)*1.0) / (a*a) );
        ellipsesympoints(pt_l, midx, midy, x, y);
    }
}

void swapuc(unsigned char* a, unsigned char* b)
{
    unsigned char t;
    t = *a;
    *a = *b;
    *b = t;
}

void swapi(int* a, int p, int q)
{
    int k;
    k = a[p];
    a[p] = a[q];
    a[q] = k;
}

//sort in ascending order
void inssort(int*a, int l)
{
    int i;
    for(i=1; i<l; i++)
    {
        int j = i; //take ith element,
        //insert it between 0th and (i-1)th element
        //where it should actually stay.
        //for this, compare ith to (i-1)th element,
        //if lesser, swap and continue the same
        //untill it is no more lesser.
        //once it is not lesser, stop.
        //because, after iteration i=1,
        //0th to 1st elements are sorted.
        //similarly, after iteration i=5,
        //0th to 5th elements are sorted, and so on.
        while(j>0 && a[j]<a[j-1])
        {
            swapi(a, j, j-1);
            j--;
        }
    }
}

int cmp(const void* pa, const void* pb)
{
    int a = *(int*)pa;
    int b = *(int*)pb;

    if(a > b) return 1;
    else if(a < b) return -1;
    else return 0;
}

void save_to_file(int app_closed)
{
    char fname[1000+1]={'\0'};
    int fnum = time(0);

    //#if defined(_WIN32)
    //#elif defined(__APPLE__)
    //#else
    //Linux specific code ...
    //#endif

#if defined(_WIN32)
    if(app_closed == 1)
        sprintf(fname, "%s\\iMage_closed_%d.1mg", save_path, fnum);
    else
        sprintf(fname, "%s\\iMage_saved_%d.1mg", save_path, fnum);
#else
    if(app_closed == 1)
        sprintf(fname, "iMage_closed_%d.1mg", fnum);
    else
        sprintf(fname, "iMage_saved_%d.1mg", fnum);
#endif

    outfp = fopen(fname, "wb");
    if(!outfp)
    {
        printf("outfp file create error\n");
        return;
    }

    //save element list
    save_elem_list();
    //save text box list
    save_tbox_list();

    fclose(outfp);
}

void save_elem_list()
{
    //loop all element node
    node* elem_n = elem_l->head->next;
    while (elem_n->next != 0) //elem_n->next == 0 for dummy tail node
    {
        if(elem_n->color == FL_RED)
            fprintf(outfp, "r\n");
        if(elem_n->color == FL_GREEN)
            fprintf(outfp, "g\n");
        if(elem_n->color == FL_BLUE)
            fprintf(outfp, "b\n");

        //get point list
        list* pt_l = elem_n->point_list;
        //loop all point node
        node* pt_n = pt_l->head->next;
        while (pt_n->next != 0) //pt_n->next == 0 for dummy tail node
        {
            fprintf(outfp, "%d,%d\n", pt_n->pixel.x, pt_n->pixel.y);

            pt_n = pt_n->next;
        }
        elem_n = elem_n->next;
    }
}

void save_tbox_list()
{
    fprintf(outfp, "t\n");
    //loop all txt_n
    node* txt_n = txt_l->head->next;
    while (txt_n->next != 0) //txt_n->next == 0 for dummy tail node
    {
        fprintf(outfp, "%d,%d,%s\n", txt_n->tbox->x(), txt_n->tbox->y(), txt_n->txt);
        txt_n = txt_n->next;
    }
}

void read_from_file()
{
    fpos_t pos;
    int c, n; //char
    int r;
    int x, y;
    char t[MAX_TXT_LEN+1];

    node* elem_n;
    list* pt_l;
    node* pt_n;

    node* txt_n;
    TxtBox* txtb;
    int ww; int hh;

    //hope a "r" or "g" or "b"
    fgetpos(infp, &pos);
    c = fgetc(infp); n = fgetc(infp);
    while( (c == 'r'||c == 'g'||c == 'b') && n == '\n')
    {
        //printf("%c\n", c);

        //create element node
        elem_n = nalloc();
        linsert(elem_l, elem_n);
        //create point list
        pt_l = lalloc();
        if(c == 'r')
            ninitplist(elem_n, pt_l, FL_RED);
        if(c == 'g')
            ninitplist(elem_n, pt_l, FL_GREEN);
        if(c == 'b')
            ninitplist(elem_n, pt_l, FL_BLUE);

        //hope a number
        fgetpos(infp, &pos);
        r = fscanf(infp, "%d,%d", &x, &y); n = fgetc(infp);
        while(r == 2 && n == '\n')
        {
            //hurray! print number
            //printf("%d,%d\n", x, y);

            //create point node
            pt_n = nalloc();
            ninitpix(pt_n, x, y);
            linsert(pt_l, pt_n);

            //hope another number
            fgetpos(infp, &pos);
            r = fscanf(infp, "%d,%d", &x, &y); n = fgetc(infp);
        }
        //not another number?
        fsetpos(infp, &pos);

        //hope another "r" or "g" or "b"
        fgetpos(infp, &pos);
        c = fgetc(infp); n = fgetc(infp);
    }
    //not another "r" or "g" or "b"?
    fsetpos(infp, &pos);

    //hope a "t"
    fgetpos(infp, &pos);
    c = fgetc(infp); n = fgetc(infp);
    while(c == 't' && n == '\n')
    {
        //printf("t\n");

        //hope a number
        fgetpos(infp, &pos);
        r = fscanf(infp, "%d,%d,%[^\n]", &x, &y, t); n = fgetc(infp);
        while(r == 3 && n == '\n')
        {
            //hurray! print number
            //printf("%d,%d,%s\n", x, y, t);

            //create text node
            txtb = new TxtBox(x, y, 100, 24); //default font size is 14 pixel, we add 5 pixel margin
            txtb->box(FL_ROUNDED_BOX); //FL_NO_BOX/FL_PLASTIC_UP_BOX/FL_ROUNDED_BOX better
            txt_n = nalloc();
            ninittbox(txt_n, txtb);
            linsert(txt_l, txt_n);

            //add text widget to window
            w.window->add(*txtb);

            //assign its label text
            txt_n->txt = (char*)malloc(MAX_TXT_LEN+1);
            strncpy(txt_n->txt, t, MAX_TXT_LEN); //strncpy copies last null char if nullcomes within n (n included)
            txt_n->txt[MAX_TXT_LEN] = '\0'; //safety
            txt_n->tbox->label(txt_n->txt);

            //char temp[MAX_TXT_LEN+1];
            //strcpy(temp, txt_n->txt); //strcpy copies last null char
            //temp[MAX_TXT_LEN] = '\0'; //safety
            //int len = strlen(temp);
            //for(int i=0; i<len; i++)
            //{
                //if(temp[i] == ' ')
                //{
                    //temp[i] = 'f'; //approximating font width of space char
                //}
            //}
            //txt_n->tbox->label(temp);

            //txt_n->tbox->labelfont(FL_HELVETICA); //default font type, ==Arial in windows
            //txt_n->tbox->labelsize(14); //font size in pixel
            ww=0; hh=0; //needed for fl_measure or measure_label
            fl_font(FL_HELVETICA, 14); //needed for fl_measure or measure_label
            //fl_measure(txt_n->txt, ww, hh);
            txt_n->tbox->measure_label(ww, hh);
            ww = ww + 10; //10 for left right margin
            if(ww < 24) ww = 24; //min width 24
            txt_n->tbox->size(ww, 24); //height always 24

            //hope another number
            fgetpos(infp, &pos);
            r = fscanf(infp, "%d,%d,%[^\n]", &x, &y, t); n = fgetc(infp);
        }
        //not another number?
        fsetpos(infp, &pos);

        //hope another "t"
        fgetpos(infp, &pos);
        c = fgetc(infp); n = fgetc(infp);
    }
    //not another "t"?
    fsetpos(infp, &pos);
    fclose(infp);
    infp = NULL;
}

//******************

int main(int argc, char **argv)
{
    int m=10; //margin

    int b1gw=MAX_X+1, b1gh=25;
    int b1bw=75, b1bh=25;

    int ibw=MAX_X+1, ibh=MAX_Y+1;

    //int b2gw=75, b2gh=MAX_Y+1;
    //int b2bw=75, b2bh=25;

    int ww=0;
    int wh=0;

    elem_l = lalloc();
    txt_l = lalloc();

    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
    Fl::visual(FL_RGB);

    char mainlabel[1000+1]={0};

#if defined(_WIN32)
    snprintf(mainlabel, 1000, "%s", "iMage");
#else
    char cwd[1000+1]={0};
    fl_getcwd(cwd, 1000);
    snprintf(mainlabel, 1000, "%s (Working Dir: %s)", "iMage", cwd);
#endif

#if defined(_WIN32)
    char* uname;
    uname = fl_getenv("username");
    printf("uname = %s\n", uname);
    snprintf(save_path, 1000, "C:\\Users\\%s\\Desktop", uname);
    printf("save_path = %s\n", save_path);
#else
    //save in current working directory
#endif

    //create window
    w.window = new Fl_Window(5, 5, ww, wh, mainlabel);
    {
        w.window->begin(); //sart adding below widgets to it

        //w.obox = new Fl_Box(5, 5, 10, 10, "O");
        //w.obox->box(FL_NO_BOX);

        //below box(or panel) automatically gets added into
        //above window, even  if window->begin() is not called
        w.b1g = new Fl_Group(m, m, b1gw, b1gh);
        {
            w.saveb = new Fl_Button(w.b1g->x() +(b1bw+m)*0, w.b1g->y(), b1bw, b1bh, "Save");
            w.saveb->callback(saveb_callback);

            w.penb = new Fl_Button(w.b1g->x() +(b1bw+m)*1, w.b1g->y(), b1bw, b1bh, "Pencil");
            w.penb->callback(penb_callback);

            w.lineb = new Fl_Button(w.b1g->x() +(b1bw+m)*2, w.b1g->y(), b1bw, b1bh, "Line");
            w.lineb->callback(lineb_callback);

            w.arrowb = new Fl_Button(w.b1g->x() +(b1bw+m)*3, w.b1g->y(), b1bw, b1bh, "Arrow");
            w.arrowb->callback(arrowb_callback);

            w.rectb = new Fl_Button(w.b1g->x() +(b1bw+m)*4, w.b1g->y(), b1bw, b1bh, "Rectangle");
            w.rectb->callback(rectb_callback);

            w.ellipb = new Fl_Button(w.b1g->x() +(b1bw+m)*5, w.b1g->y(), b1bw, b1bh, "Ellipse");
            w.ellipb->callback(ellipb_callback);

            w.mov_delb = new Fl_Button(w.b1g->x() +(b1bw+m)*6, w.b1g->y(), b1bw, b1bh, "Mov,Del");
            w.mov_delb->callback(mov_delb_callback);
            w.mov_delb->color(curr_color);
            Fl::focus(w.mov_delb);

            w.helpb = new Fl_Button(w.b1g->x() +(b1bw+m)*7, w.b1g->y(), b1bw, b1bh, "Help");
            w.helpb->callback(helpb_callback);

            //*********

            w.aboutb = new Fl_Button(w.b1g->x() +b1gw-b1bw, w.b1g->y(), b1bw, b1bh, "About");
            w.aboutb->callback(aboutb_callback);

        } w.b1g->end();

        w.ibox = new ImgBox(m, m+b1gh+m, ibw, ibh);
        //printf("image box ordered x, y, w, h = %d, %d, %d, %d\n", m+b1gw+m, m, ibw, ibh);
        //printf("image box created x, y, w, h = %d, %d, %d, %d\n", w.ibox->x(), w.ibox->y(), w.ibox->w(), w.ibox->h());
        w.ibox->box(FL_NO_BOX); //FL_BORDER_FRAME //frame only draws border, box draws inside
        //w.ibox->color(FL_WHITE); //frame with white border

        ww = m+b1gw+m;
        wh = m+b1gh+m+ibh+m; //recalculate
        w.window->size(ww, wh); //means resize
    } w.window->end(); //it says, no more widgets will be added to window

    Fl::get_system_colors();
    w.window->color(8);

    w.window->callback(mainwin_callback);

    if (argc > 2 )
    {
        printf("usage: %s file.1mg\n", argv[0]);
        return -1;
    }
    else if(argc == 2)
    {
        char *filename = argv[1];
        infp = fopen(filename, "rb");

        if(infp == NULL)
        {
            printf("file not opening!\n");
            return -2;
        }
        else
        {
            //good, show file data on draw pad
            read_from_file();
        }
    }
    else if(argc == 1)
    {
        //good, show blank draw pad
    }

    //draw window on screen
    //w.window->show(argc, argv);
    w.window->show();

    //event loop; close the window with ESC
    return Fl::run();
}

