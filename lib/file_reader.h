

#ifndef _AOC_FILE_READER_H
#define _AOC_FILE_READER_H

/*
    File reader takes `filename` just like `fopen`
    and returns a `malloc`'d string of the contents of the entire file.
 */
char *file_reader(const char *filename);

#endif // _AOC_FILE_READER_H