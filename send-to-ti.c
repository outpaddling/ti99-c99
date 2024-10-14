/***************************************************************************
 *  Description:
 *      Send a file to a TI-99/4a over a serial connection
 *
 *  History: 
 *  Date        Name        Modification
 *  2024-10-11  Jason Bacon Adapt from recv-from-ti.c
 ***************************************************************************/

#include <stdio.h>
#include <sysexits.h>
#include <termios.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/param.h>
#include <stdbool.h>
#include <unistd.h>
#include <byteswap.h>

#define RESP_MAX_LEN    10

typedef enum
{
    BINARY, TEXT
}   transfer_mode_t;

typedef enum
{
    DISK_TRANSFER, FILE_TRANSFER
}   transfer_object_t;

size_t  send_line(char *line, FILE *fp, transfer_mode_t mode);
void    usage(char *progname);

int     main(int argc,char *argv[])

{
    FILE            *serial_fp,
		    *in_fp;
    char            resp[RESP_MAX_LEN + 1];
    struct termios  port_settings;
    char            *file_name, *base_name;
    int             ch;
    
    if ( argc != 2 )
	usage(argv[0]);
    file_name = argv[1];
    
    /* Assume USB-serial adapter */
    serial_fp = fopen("/dev/cuaU0", "w");
    if ( serial_fp == NULL )
    {
	fprintf(stderr, "Cannot open /dev/cuaU0.\n");
	return EX_NOINPUT;
    }
    
    /* Get tty attributes */
    if ( tcgetattr(fileno(serial_fp), &port_settings) != 0 )
    {
	fprintf(stderr, "%s: tcgetattr() failed: %s\n",
	    __func__, strerror(errno));
	return EX_OSERR;
    }

    cfsetspeed(&port_settings, B9600);
    
    // CS7 does not seem to work with the Prolific PL-2303, so specify
    // 8-bit on the TI side
    port_settings.c_cflag &= ~CSIZE;    // Clear all size bits, CS8 uses 2
    port_settings.c_cflag |= CS8;
    
    // TI default no parity
    port_settings.c_cflag |= PARENB;
    // port_settings.c_cflag &= ~PARENB;
    
    // No modem control
    port_settings.c_cflag |= CLOCAL;
    port_settings.c_cflag &= ~HUPCL;
    
    // Don't convert CR to NL.  TI-BASIC INPUT reads until CR.
    port_settings.c_oflag &= ~OCRNL;
    
    // Char 255 should not signal EOF
    port_settings.c_lflag &= ~ICANON;
    port_settings.c_lflag &= ~ECHO;
    
    printf("CSIZE = %X\n", port_settings.c_cflag & CSIZE);
    printf("CSTOPB = %X\n", port_settings.c_cflag & CSTOPB);
    printf("CREAD = %X\n", port_settings.c_cflag & CREAD);
    printf("PARENB = %X\n", port_settings.c_cflag & PARENB);
    printf("PARODD = %X\n", port_settings.c_cflag & PARODD);
    printf("HUPCL = %X\n", port_settings.c_cflag & HUPCL);
    printf("CLOCAL = %X\n", port_settings.c_cflag & CLOCAL);
    printf("CRTS_IFLOW = %X\n", port_settings.c_cflag & CRTS_IFLOW);

    printf("OCRNL = %X\n", port_settings.c_oflag & OCRNL);
    printf("ECHO = %X\n", port_settings.c_lflag & ECHO);

    if ( tcsetattr(fileno(serial_fp), TCSANOW, &port_settings) != 0 )
    {
	fprintf(stderr, "%s: tcsetattr() failed: %s\n",
	    __func__, strerror(errno));
	return EX_OSERR;
    }
    
    // OPEN #1:"RS232/1.BA=9600.DA=8.PA=N.EC",DISPLAY,VARIABLE 254
    //putc(0, serial_fp);
    base_name = strrchr(file_name, '/');
    if ( base_name == NULL )
	base_name = file_name;
    else
	++base_name;
    fprintf(serial_fp, "%s\n", base_name);
    usleep(1500000);
    
    if ( (in_fp = fopen(file_name, "r")) == NULL )
    {
	fprintf(stderr, "Cannot open %s.\n", file_name);
	return EX_NOPERM;
    }
    printf("Run receiver on TI-99 using \"RS232.BA=%d.DA=8.PA=N.EC\",DISPLAY,VARIABLE 254\n",
	    cfgetispeed(&port_settings));
    printf("Type Ctrl+c if this program does not automatically terminate.\n");
    
    while ( (ch = getc(in_fp)) != EOF )
    {
	putc(ch, serial_fp);
	// FIXME: Replace delays with handshaking
	// TI BASIC needs a lot of time to process each line
	usleep(8000);
	if ( ch == '\n' )
	{
	    fgets(resp, RESP_MAX_LEN + 1, serial_fp);
	    fputs(resp, stdout);
	    usleep(4000000);
	}
    }
    fclose(in_fp);
    
    fputs("### END OF FILE\n", serial_fp);
    fclose(serial_fp);
    
    return EX_OK;
}


void    usage(char *progname)

{
    fprintf(stderr, "Usage: %s text|binary disk|file\n", progname);
    exit(EX_USAGE);
}
