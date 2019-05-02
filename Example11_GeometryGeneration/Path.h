#pragma once
#include "Cave.h"
struct cells;
class Path
{
public:
	Path();
	~Path();
	void createPath(Cave* one, Cave* two);
	void createXpath(Cave* one, Cave* two);
	bool overlap(int start1, int end1, int start2, int end2);

	cells* getPath() { return path; }
	int getSize() { return size; }

private:
	int random(int low, int high);
	cells* path;
	int size;
};

