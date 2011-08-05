/*
	writeall and readall
	AUP2, Sec. 2.09, 2.10

	Copyright 2003 by Marc J. Rochkind. All rights reserved.
	May be copied only for purposes and under conditions described
	on the Web page www.basepath.com/aup/copyright.htm.

	The Example Files are provided "as is," without any warranty;
	without even the implied warranty of merchantability or fitness
	for a particular purpose. The author and his publisher are not
	responsible for any damages, direct or incidental, resulting
	from the use or non-use of these Example Files.

	The Example Files may contain defects, and some contain deliberate
	coding mistakes that were included for educational reasons.
	You are responsible for determining if and how the Example Files
	are to be used.

*/
#include "defs.h"

/*
	Supposed to be a substitute for write(), so doesn't use ec.
	Unlike write(), can return -1 and also complete one or more
	partial writes.
*/
/*[writeall]*/
ssize_t writeall(int fd, const void *buf, size_t nbyte)
{
	ssize_t nwritten = 0, n;

	do {
		if ((n = write(fd, &((const char *)buf)[nwritten],
		  nbyte - nwritten)) == -1) {
			if (errno == EINTR)
				continue;
			else
				return -1;
		}
		nwritten += n;
	} while (nwritten < nbyte);
	return nwritten;
}
/*[readall]*/
ssize_t readall(int fd, void *buf, size_t nbyte)
{
	ssize_t nread = 0, n;

	do {
		if ((n = read(fd, &((char *)buf)[nread], nbyte - nread)) == -1) {
			if (errno == EINTR)
				continue;
			else
				return -1;
		}
		if (n == 0)
			return nread;
		nread += n;
	} while (nread < nbyte);
	return nread;
}
/*[]*/
