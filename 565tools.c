/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
	
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define to565_ariesve(r,g,b)                                            \
    ( ((b) >> 3) | (((r) >> 2) << 5) | (((g) >> 3) << 11))

#define from565_b_ariesve(x) ((((x) >> 11) & 0x1f) * 255 / 31)
#define from565_r_ariesve(x) ((((x) >> 5) & 0x3f) * 255 / 63)
#define from565_g_ariesve(x) (((x) & 0x1f) * 255 / 31)

#define to565(r,g,b)                                            \
    ((((r) >> 3) << 11) | (((g) >> 2) << 5) | ((b) >> 3))

#define from565_r(x) ((((x) >> 11) & 0x1f) * 255 / 31)
#define from565_g(x) ((((x) >> 5) & 0x3f) * 255 / 63)
#define from565_b(x) (((x) & 0x1f) * 255 / 31)

void from_565_rle(int ariesve)
{
    unsigned short times, count;
    unsigned short in [2];
    unsigned short color, outR, outG, outB;
    unsigned total = 0;

    while(read(0, in, 4)) {
        count = in[0];
	color = in[1];
	total += count;

	if(ariesve) {
	outR = from565_r_ariesve(color);
	outG = from565_g_ariesve(color);
	outB = from565_b_ariesve(color);
	} else {
	outR = from565_r(color);
	outG = from565_g(color);
	outB = from565_b(color);
	}

	for (times=0;times<count;times++) {
	    write(1, &outR, 1);
	    write(1, &outG, 1);
	    write(1, &outB, 1);
	}
    }

    fprintf(stderr,"%d pixels\n",total);
}

void to_565_rle(int ariesve)
{
    unsigned char in[3];
    unsigned short last, color, count;
    unsigned total = 0;
    count = 0;

    while(read(0, in, 3) == 3) {
		if(ariesve)
	        color = to565_ariesve(in[0],in[1],in[2]);
		else
			color = to565(in[0],in[1],in[2]);
        if (count) {
            if ((color == last) && (count != 65535)) {
                count++;
                continue;
            } else {
                write(1, &count, 2);
                write(1, &last, 2);
                total += count;
            }
        }
        last = color;
        count = 1;
    }
    if (count) {
        write(1, &count, 2);
        write(1, &last, 2);
        total += count;
    }
    fprintf(stderr,"%d pixels\n",total);
}

int main(int argc, char **argv)
{
	if (argc == 2) {
		if (!strcmp(argv[1],"-rle2raw"))
		{
			from_565_rle(0);
			return 0;
		}
		else if (!strcmp(argv[1],"-raw2rle")) 
		{
			to_565_rle(0);
			return 0;
		}
    } else if (argc == 3) {
		if (!strcmp(argv[1],"-ariesve"))
		{
			if (!strcmp(argv[2],"-rle2raw"))
			{
				from_565_rle(1);
				return 0;
			}
			else if (!strcmp(argv[2],"-raw2rle"))
			{
				to_565_rle(1);
				return 0;
			}		
		}
		else if (!strcmp(argv[2],"-ariesve"))
		{
			if (!strcmp(argv[1],"-rle2raw"))
			{
				from_565_rle(1);
				return 0;
			}
			else if (!strcmp(argv[1],"-raw2rle"))
			{
				to_565_rle(1);
				return 0;
			}		
		}
    }
    fprintf(stderr,"Usage : %s [-rle2raw |-raw2rle] [-ariesve] < input_file > output_file\n   -rle2raw : convert RLE to raw RGB\n   -rle2raw : convert raw RGB to rle\n   -ariesve : use ariesve format (brg instead of rgb)\n",argv[0]);
    return 1;
}

