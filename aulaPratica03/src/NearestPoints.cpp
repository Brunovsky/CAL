/*
 * NearestPoints.cpp
 */

#include <limits>
#include <thread>
//#include <boost/thread/thread.hpp>
#include <algorithm>
#include <cmath>
#include "NearestPoints.h"
#include "Point.h"

const double MAX_DOUBLE = std::numeric_limits<double>::max();

Result::Result(double dmin, Point p1, Point p2) {
	this->dmin = dmin;
	this->p1 = p1;
	this->p2 = p2;
}

Result::Result() {
	this->dmin = MAX_DOUBLE;
	this->p1 = Point(0,0);
	this->p2 = Point(0,0);
}

/**
 * Auxiliary functions to sort vector of points by X or Y axis.
 */
static void sortByX(vector<Point> &v, int left, int right)
{
	std::sort(v.begin( ) + left, v.begin() + right + 1,
		[](Point p, Point q){ return p.x < q.x || (p.x == q.x && p.y < q.y); });
}

static void sortByY(vector<Point> &v, int left, int right) // @suppress("Unused static function")
{
	std::sort(v.begin( ) + left, v.begin() + right + 1,
		[](Point p, Point q){ return p.y < q.y || (p.y == q.y && p.x < q.x); });
}

/**
 * Brute force algorithm O(N^2).
 */
Result nearestPoints_BF(vector<Point> &vp) {
	Result res;

	for (size_t i = 0; i < vp.size(); ++i) {
		for (size_t j = i + 1; j < vp.size(); ++j) {
			double d = vp[i].distSquare(vp[j]);
			if (d < res.dmin) {
				res.dmin = d;
				res.p1 = vp[i];
				res.p2 = vp[j];
				if (d == 0) {
					res.dmin = sqrt(res.dmin);
					return res;
				}
			}
		}
	}

	res.dmin = sqrt(res.dmin);
	return res;
}

/**
 * Improved brute force algorithm, that first sorts points by X axis.
 */
Result nearestPoints_BF_SortByX(vector<Point> &vp) {
	Result res;
	sortByX(vp, 0, vp.size()-1);

	for (size_t i = 0; i < vp.size(); ++i) {
		for (size_t j = i + 1; j < vp.size(); ++j) {
			double x = vp[j].x - vp[i].x;
			if (x >= res.dmin) break;

			double d = vp[i].distance(vp[j]);
			if (d < res.dmin) {
				res.dmin = d;
				res.p1 = vp[i];
				res.p2 = vp[j];
				if (d == 0) return res;
			}
		}
	}

	return res;
}


/**
 * Auxiliary function to find nearest points in strip, as indicated
 * in the assignment, with points sorted by Y coordinate.
 * The strip is the part of vp between indices left and right (inclusive).
 * "res" contains initially the best solution found so far.
 */
static void npByY(vector<Point> &vp, int left, int right, Result &res)
{
	// TODO.

	for (int i = left; i <= right; ++i) {
		for (int j = i + 1; j <= right; ++j) {
			double y = vp[j].y - vp[i].y;
			if (y >= res.dmin) break;

			double d = vp[i].distance(vp[j]);
			if (d < res.dmin) {
				res.dmin = d;
				res.p1 = vp[i];
				res.p2 = vp[j];
				if (d == 0) return;
			}
		}
	}
}


/**
 * Recursive divide and conquer algorithm.
 * Finds the nearest points in "vp" between indices left and right (inclusive),
 * using at most numThreads.
 */
static Result np_DC(vector<Point> &vp, int left, int right, int numThreads) {
	// Base case of two points
	// TODO.
	if (left == right - 1) {
		return Result(vp[left].distance(vp[right]), vp[left], vp[right]);
	}

	// Base case of a single point: no solution, so distance is MAX_DOUBLE
	// TODO.
	if (left == right) {
		return Result();
	}

	// Divide in halves (left and right) and solve them recursively,
	// possibly in parallel (in case numThreads > 1)
	// TODO.
	int m = (left + right) / 2;
	//std::thread t;
	Result resLeft = np_DC(vp, left, m, numThreads > 1 ? numThreads / 2 : 1);
	Result resRight = np_DC(vp, m + 1, right, numThreads > 1 ? numThreads / 2 : 1);

	// Select the best solution from left and right
	Result res = resLeft.dmin < resRight.dmin ? resLeft : resRight;

	// Determine the strip area around middle point
	// TODO.
	double delta = resLeft.dmin;
	int stripLeft = left;
	int stripRight = right;
	double stripX = (vp[m].x + vp[m + 1].x) / 2;

	for (int i = m; i >= left; --i) {
		if (stripX - vp[i].x >= delta) {
			stripLeft = i + 1;
			break;
		}
	}
	for (int j = m + 1; j <= right; ++j) {
		if (vp[j].x - stripX >= delta) {
			stripRight = j - 1;
			break;
		}
	}

	// Order points in strip area by Y coordinate
	// TODO.
	sortByY(vp, stripLeft, stripRight);

	// Calculate nearest points in strip area (using npByY function)
	// TODO.
	npByY(vp, stripLeft, stripRight, res);

	// Reorder points in strip area back by X coordinate
	// TODO.
	sortByX(vp, stripLeft, stripRight);

	return res;
}


/**
 * Defines the number of threads to be used.
 */
static int numThreads = 1;
void setNumThreads(int num)
{
	numThreads = num;
}

/*
 * Divide and conquer approach, single-threaded version.
 */
Result nearestPoints_DC(vector<Point> &vp) {
	sortByX(vp, 0, vp.size() - 1);
	return np_DC(vp, 0, vp.size() - 1, 1);
}


/*
 * Multi-threaded version, using the number of threads specified
 * by setNumThreads().
 */
Result nearestPoints_DC_MT(vector<Point> &vp) {
	sortByX(vp, 0, vp.size() - 1);
	return np_DC(vp, 0, vp.size() - 1, numThreads);
}
