/*
 * edimg : a simple image creater
 * - a tool write some blocks into one file
 */

#include	<unistd.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<fcntl.h>
#include	<stdio.h>
#include	<memory.h>
#include	<stdlib.h>
#include	<assert.h>
#include	<errno.h>
#include	<string.h>


#define	MAX_PATH	260
#define	str2l(str)	strtol(str, NULL, 0)


void	do_edimg(
		int	argc,
		char	*argv[]);


char	*img_buf;
int	img_fd;
char	img_name[MAX_PATH] = "test.img";
int	img_size;


int	main(
		int	argc,
		char	*argv[])
{
	do_edimg(argc, argv);
	return	0;
}


void	do_edimg(
		int	argc,
		char	*argv[])
{
	/* OUTPUT	OUTSIZE	BLOCK		SIZE	OFFSET	*/
	/* out.img	1440M	boot.bin	512	0	*/
	char	*blk_buf;
	int	blk_fd;
	int	blk_size;
	int	blk_i;
	int	blk_argc = 3;
	int	offset;
	if (argc < 3)
		goto	show_usage;
	img_size = str2l(argv[2]);
	img_buf = malloc(img_size);
	printf("creating image '%s'\n", strcpy(img_name, argv[1]));	/* may overflow here */
	if ((argc - 3) % 3)
		goto	show_usage;
	while (blk_argc < argc) {
		if (!(blk_fd = open(argv[blk_argc], O_RDONLY))) {
			perror("open");
			goto	error;
		}
		blk_buf = malloc(
				blk_size = str2l(argv[blk_argc + 1]));
		offset = strtol(argv[blk_argc + 2], NULL, 0);
		assert(offset >= 0);
		assert(offset + blk_size >= 0);
		if (0 > read(blk_fd, blk_buf, blk_size)) {
			perror("read");
			goto	error;
		}
		memcpy(img_buf + offset, blk_buf, blk_size);
		close(blk_fd);
		++blk_i;
		blk_argc += 3;
		free(blk_buf);
	}
	if(!(img_fd = open(img_name, O_WRONLY | O_CREAT,
					S_IREAD | S_IWRITE | S_IRGRP | S_IWGRP | S_IROTH))) {
		perror("open");
		goto	error;
	}
	if (0 > write(img_fd, img_buf, img_size)) {
		perror("write");
		goto	error;
	}
done:
	exit(0);
show_usage:
	printf("Usage : edimg <OUTPUT> <OUTSIZE> [<BLOCK> <SIZE> <OFFSET> [...]]\n");
	errno = -1;
error:
	close(img_fd);
	free(img_buf);
	exit(errno);
}


