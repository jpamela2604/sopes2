#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <limits.h>
 
#define MAX_EVENTS 1024 /*Max. number of events to process at one go*/
#define LEN_NAME 16 /*Assuming that the length of the filename won't exceed 16 bytes*/
#define EVENT_SIZE  ( sizeof (struct inotify_event) ) /*size of one event*/
#define BUF_LEN     ( MAX_EVENTS * ( EVENT_SIZE + LEN_NAME )) /*buffer to store the data of events*/
 
int main( int argc, char **argv ) 
{
  int length, i = 0, wd;
  int fd;
  char buffer[BUF_LEN];
 
  /* Initialize Inotify*/
  fd = inotify_init();
  if ( fd < 0 ) {
    perror( "No se pudo iniciar inotify");
  }
 
  /* add watch to starting directory */
  wd = inotify_add_watch(fd, "/", IN_CREATE | IN_MODIFY | IN_DELETE); 
 
  if (wd == -1)
    {
      printf("NO se pudo agregar reloj %s\n","/");
    }
  else
    {
      printf("Watching:: %s\n","/");
    }
 
         FILE *fptr;

  /* do it forever*/
  while(1)
    {
      i = 0;
      length = read(fd, buffer, BUF_LEN );  
 
      if ( length < 0 ) {
        perror( "read" );
      }  
 
      while ( i < length ) {


                fptr = fopen("/var/proyecto/201222673_files.log","a");

        

                          if(fptr == NULL)
                {
                    printf("Error!");   
                    return -1;           
                }

              //int num = 300;
                  
                   
        struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];
        if ( event->len ) {
          if ( event->mask & IN_CREATE) {
            if (event->mask & IN_ISDIR)
              {
                printf( "El directorio %s fue creado.\n", event->name );     
               fprintf(fptr,"El directorio %s fue creado.\n", event->name);  
              }
            else
              {
                printf( "El archivo %s fue creado con WD %d\n", event->name, event->wd );  
              fprintf(fptr,"El archivo %s fue creado con WD %d\n", event->name, event->wd);   
              }   
          }
           
          if ( event->mask & IN_MODIFY) {
            if (event->mask & IN_ISDIR){
              printf( "El directorio %s fue modificado.\n", event->name );    
              fprintf(fptr,"El directorio %s fue modificado.\n", event->name );     
            }
            else{
              printf( "El archivo %s fue modificado. %d\n", event->name, event->wd );   
              fprintf(fptr,"El archivo %s fue modificado.%d\n", event->name, event->wd );  
            }   
          }
           
          if ( event->mask & IN_DELETE) {
            if (event->mask & IN_ISDIR){
              printf( "El directorio %s fue eliminado.\n", event->name );   
              fprintf(fptr,"El directorio %s fue eliminado.\n", event->name  ); 
            }    
            else{
              printf( "El archivo %s fue eliminado.  %d\n", event->name, event->wd );     
              fprintf(fptr,"El archivo %s fue eliminado.  %d\n", event->name, event->wd  );  
            }
          }  
 
 
          i += EVENT_SIZE + event->len;
          
        }
        fclose(fptr);
      }
    }
 
  /* Clean up*/
  inotify_rm_watch( fd, wd );
  close( fd );
   
  return 0;
}
