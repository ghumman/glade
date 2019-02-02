
#include "highgui.h"
#include <gtk/gtk.h>
#include <opencv/cv.h>
#include <opencv/cxcore.h>

GdkPixbuf* pix;
IplImage* frame;
CvCapture* capture;

gboolean
expose_event_callback(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
  while(1) {
         frame = cvQueryFrame( capture );
          if(!frame) break;  

         pix = gdk_pixbuf_new_from_data((guchar*) frame->imageData,
         GDK_COLORSPACE_RGB, FALSE, frame->depth, frame->width,
         frame->height, (frame->widthStep), NULL, NULL);


         gdk_draw_pixbuf(widget->window,
   widget->style->fg_gc[GTK_WIDGET_STATE (widget)], pix, 0, 0, 0, 0,
   -1, -1, GDK_RGB_DITHER_NONE, 0, 0); /* Other possible values are  GDK_RGB_DITHER_MAX,  GDK_RGB_DITHER_NORMAL */

         char c = cvWaitKey(33);
         if( c == 27 ) break;
   }

   cvReleaseCapture( &capture );   
   return TRUE;
}

int main( int argc, char** argv ) {

   /* GtkWidget is the storage type for widgets */
   GtkWidget *window;
   GtkWidget *drawing_area;

   gtk_init (&argc, &argv);

   CvCapture* capture = cvCreateCameraCapture(0); 
   /* create a new window */
   window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title (GTK_WINDOW (window), "Hello WebCam and OpenCV!");
   g_signal_connect (G_OBJECT (window), "destroy",
    G_CALLBACK (gtk_main_quit), NULL);

   /* Sets the border width of the window. */
   gtk_container_set_border_width (GTK_CONTAINER (window), 10);

   /* Now add a child widget to the aspect frame */
   drawing_area = gtk_drawing_area_new();


   /* window since we are forcing a aspect ratio */
   gtk_widget_set_size_request(drawing_area, 600, 400);
   gtk_container_add(GTK_CONTAINER (window), drawing_area);
   gtk_widget_show(drawing_area);

   g_signal_connect (G_OBJECT (drawing_area), "expose_event",
      G_CALLBACK (expose_event_callback), NULL);

   /* and the window */
   gtk_widget_show (window);

   /* All GTK applications must have a gtk_main(). Control ends here
    * and waits for an event to occur (like a key press or
    * mouse event). */
   gtk_main ();

   return 0;
}


