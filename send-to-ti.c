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

#define MAX_LINE_LEN    1024
#define MAX_RESP_LEN    10

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
    struct termios  port_settings;
    struct stat     st;
    char            filename[PATH_MAX+1],
		    *p,
		    line[MAX_LINE_LEN+1],
		    resp[MAX_RESP_LEN+1];
    transfer_mode_t transfer_mode;
    transfer_object_t object_type;
    
    if ( argc != 3 )
	usage(argv[0]);

    if ( strcmp(argv[1], "text") == 0 )
	transfer_mode = TEXT;
    else if ( strcmp(argv[1], "binary") == 0 )
	transfer_mode = BINARY;
    else
	usage(argv[0]);
    
    if ( strcmp(argv[2], "disk") == 0 )
	object_type = DISK_TRANSFER;
    else if ( strcmp(argv[2], "file") == 0 )
	object_type = FILE_TRANSFER;
    else
	usage(argv[0]);
    
    /* Assume USB-serial adapter */
    serial_fp = fopen("/dev/cuaU0", "r");
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
    // port_settings.c_cflag |= CS7;
    port_settings.c_cflag |= CLOCAL|CREAD;
    // TI default no parity
    port_settings.c_cflag &= ~PARENB;
    // Don't convert CR to NL, strip CRs in send_line() if text file
    port_settings.c_iflag &= ~ICRNL;
    // Char 255 should not signal EOF
    port_settings.c_lflag &= ~ICANON;

    if ( tcsetattr(fileno(serial_fp), TCSANOW, &port_settings) != 0 )
    {
	fprintf(stderr, "%s: tcsetattr() failed: %s\n",
	    __func__, strerror(errno));
	return EX_OSERR;
    }
    
    do
    {
	do
	{
	    printf("Filename for next disk or file? (Press return if no more disks)\n");
	    fgets(filename, PATH_MAX + 1, stdin);
	    if ( *filename == '\0' )
		break;
	    filename[strlen(filename)] = '\0';  // Trim \n
	    
	    if ( stat(filename, &st) == 0 )
	    {
		printf("%s exists.  Overwrite? (yes/no) ",filename);
		send_line(resp, stdin, TEXT);
		if ( strcmp(resp, "yes") != 0 )
		    return 0;
	    }
	    else
		strlcpy(resp, "yes", 4);
	}   while ( strcmp(resp, "yes") != 0 );
	
	if ( *filename == '\0' )
	    break;
	
	if ( (in_fp = fopen(filename, "r")) == NULL )
	{
	    fprintf(stderr, "Cannot open %s.\n", filename);
	    return EX_NOPERM;
	}
	printf("Run receiver on TI-99 using RS232.BA=%d.DA=8\n",
		cfgetispeed(&port_settings));
	printf("Type Ctrl+c if this program does not automatically terminate.\n");
	
	if ( send_line(line, serial_fp, transfer_mode) == EOF )
	{
	    fprintf(stderr, "EOF sending first line.\n");
	    return EX_DATAERR;
	}
	
	if ( object_type == DISK_TRANSFER )
	{
	    if ( memcmp(line, "DISK NAME", 9) != 0 )
	    {
		fprintf(stderr, "Expected 'DISK NAME' on first line.\n");
		return EX_DATAERR;
	    }
	    fputs(line, in_fp);
	}
	
	while ( fgets(line, MAX_LINE_LEN + 1, in_fp) != NULL )
	{
	    send_line(line, serial_fp, transfer_mode);
	    // Not assuming text file
	    for (p = line; *p != '\0'; ++p)
		if ( isprint(*p) || isspace(*p) )
		    putchar(*p);
		else
		    printf(" \\%03o ", (unsigned char)*p);
	    putchar('\n');
	    fflush(stdout);
	}
    }   while ( *filename != '\0' );
    fclose(serial_fp);
    return EX_OK;
}


size_t  send_line(char *line, FILE *fp, transfer_mode_t mode)

{
    char    *p = line;

    // TI "LIST" command inserts null characters in the line numbers
    // Also filter out CRs here, TI sends CR+LF unless using RS232.LF
    while ( *p != '\0' )
    {
	putc(*p++, fp);
    }
    return p - line;
}


void    usage(char *progname)

{
    fprintf(stderr, "Usage: %s text|binary disk|file\n", progname);
    exit(EX_USAGE);
}
