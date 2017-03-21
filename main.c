//
//  main.c
//  Nv20To422P10
//
//  Created by Hank Lee on 20/03/17.
//  Copyright (c) 2017 Hank Lee. All rights reserved.
//

#include <stdint.h>
#include <stdio.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#include "unpack.h"


#define MAX_WIDTH   7680
#define MAX_HEIGHT  4320


static uint8_t img[MAX_WIDTH * MAX_HEIGHT * 4];
static uint8_t y_dst[MAX_WIDTH * MAX_HEIGHT * 2];
static uint8_t u_dst[MAX_WIDTH * MAX_HEIGHT];
static uint8_t v_dst[MAX_WIDTH * MAX_HEIGHT];


int main(int argc, char *argv[])
{
    //quatre_pixel q_pix;
    uint64_t *y;
    uint32_t *u;
    uint32_t *v;
    int ifd, ofd;

    int width;
    int height;
    int wxh;
    
    int i, j;
    int count = 0;
    ssize_t rd_sz;
    
    char *cp;
    char output[256] = { 0 };
    
    if (argc < 4)
    {
        fprintf(stderr, "usage: %s [input_file] [width] [height]\n", argv[0]);
        
        return -1;
    }


    cp = NULL;


    // get input file name from comand line
    ifd = open(argv[1], O_RDONLY);
    if (ifd < 0)
    {
        perror(argv[1]);
        exit(EXIT_FAILURE);
    }


    // specify output file name
    cp = strrchr(argv[1], '_');
    strncpy(output, argv[1], cp - argv[1]);
    strcat(output, ".yuv");
    
    ofd = open(output, O_RDWR | O_CREAT, S_IRUSR);

    width   = atoi(argv[2]);
    height  = atoi(argv[3]);
    wxh     = width * height;


    while (1)
    {
        rd_sz = read(ifd, img, wxh * 2 * 10 / 8);
        
        if (rd_sz == wxh * 2 * 10 / 8)
        {
            y = (uint64_t *) y_dst;
            u = (uint32_t *) u_dst;
            v = (uint32_t *) v_dst;
    
            // Y
            quatre_pixel *p = (quatre_pixel *) img;
            for (i = 0; i < height; i++)
            {
                for (j = 0; j < width / 4; j++) // 4 horizantal pixel at a time
                {
                    unpack_y((uint8_t *) y, p);

                    y++;
                    p++;
                }
            }
            write(ofd, y_dst, width * height * 2);
          
            // U, V
            quatre_pixel *q = p;
            for (i = 0; i < height; i++)
            {
                for (j = 0; j < width / 4; j++) // 2 horizontal pixel at a time
                {                    
                    unpack_uv
                    (
                        (uint8_t *) u,
                        (uint8_t *) v,
                        q
                    );

                    u++;
                    v++;
                    q++;
                }
            }
            write(ofd, u_dst, width * height);
            write(ofd, v_dst, width * height);
          
            fprintf(stderr, "Frame %d completed.\n", count);
            count++;
        }
        else
        {
            break;
        }        
    }
    
    close(ifd);
    close(ofd);
    
    fprintf(stderr, "Done\n");
    fprintf(stderr, "Output file: %s\n", output);
    
    return 0;
}
