Convex Hull Viewer
===============================
This is an MFC program for viewing points and their convex hull. It's actually used for Computational Geometry class, but it is better if you use it as a handy tool for research or project.

![ConvexHullViewer](/images/convex_hull_viewer.PNG)

Points Format Requirement
------------------
The format of point is quite simple. Each point occupied one line, formatting as `[p.x] [p.y]` like this:
```
116.118 99.6337
112.181 105.222
109.212 105.154
107.377 106.332
......
```
Convex Hull Format Requirement
------------------
The format of convex hull is a list of point index which occupied one line. The example will be like:

```
39
4
5
6
15
......
```

Intersection Viewer
===============================

This is an MFC program for viewing complex scenes filled with lines, rays, segments and circles as well as their intersection points. It's actually used for Computational Geometry class, but it is better if you use it as a handy tool for research or project.

![IntersectionViewer](/images/intersection_viewer.PNG)

Scene Format Requirement
------------------
A scene contains various kinds of geometric objects such as lines, rays and segments. There are different type format for each kind of elements. If the element is line, it's represented as: `L [p.x] [p.y] [d.x] [d.y]`.  `p.x` and `p.y` means the coordinates of the point on the line, and `d.x` and `d.y` indicates the direction vector for the line. If the element is ray, it's formatted as `R [s.x] [s.y] [d.x] [d.y]`. `s.x` and `s.y` refers to the start point of the ray and `d.x`/`d.y` remains the meaning like the formatting of a line. If the element is segment, the format is `S [p1.x] [p1.y] [p2.x] [p2.y]`. `p1` and `p2` means the two end points for the segment. If the type is circle, then `C [c.x] [c.y] [r]`. `c.x` and `c.y` means the center of the circle and `r` means the radius. The detailed example will be like this:

```
S 87.3012 14.7008 71.514 89.3826
S 91.8729 65.9993 4.57472 40.3241
R 76.5831 79.7815 -73.4855 98.8952
R 15.4241 7.95312 64.9403 36.9671
L 83.5505 52.2507 -42.5336 -70.0735
L 96.0936 61.5619 57.0421 54.4176
C 27.1584 7.06503 6.98278
C 67.2842 26.9326 21.5706
......
```

Intersection Format Requirement
------------------
The format of intersections is quite simple, which is just a list of intersection points. Each intersection point occupied one line, formatting as `[p.x] [p.y]` like this:

```
79.7224 45.9439
263.097 220.882
69.5675 89.2229
-130.996 -47.2213
51.2919 38.1724
25.2063 59.0439
......
```

Simple Polygon & Mesh Viewer
===============================

This is an MFC program for viewing simple polygon and its mesh. It's actually used for Computational Geometry class, but it is better if you use it as a handy tool for research or project.

![SimplePolygonMeshViewer](/images/simple_polygon_mesh_viewer.PNG)

Simple Polygon Format Requirement
------------------
A simple polygon should be described as a set of points. Each point consists of two coordinates, x and y and every point occupies one line. The polygon file format should be like this:
```
116.118 99.6337
112.181 105.222
109.212 105.154
107.377 106.332
105.472 107.083
110.883 107.467
112.639 112.464
110.426 109.064
......
```

Mesh Format Requirement
------------------
A mesh should be described as a set of triangles. Regardless of steiner points, every point in triangles can be named according to its index. Thus, the mesh file format should be like this:
```
19 22 18
20 21 22
23 18 22
23 17 18
25 17 23
......
```

> **Note:** The index of the point starts from 0 instead of 1

Windowing Query Viewer
===============================
Windowing query viewer can help us view the axis-parallel segments and some windowing queries. It's actually used for Computational Geometry class, but it is better if you use it as a handy tool for research or project.

![WindowingQueryViewer](/images/windowing_query_viewer.PNG)

Axis-parallel Segments and Windowing Queries Format Requirement
--------------------------
The input file starts with the number of axis-parallel segments and the number of windowing queries in the first line, like `m n`. Then, `m` lines stand for axis-parallel segments, which have the format like `[p.x] [p.y] [len] [ori]`. `[p.x] [p.y]` represent the start point of the segment. `[len]` refers to the length of the segment and `[ori]` indicates the orientation of the segment.

`[ori]` | meaning
--------|---------
0| Up
1| Down
2| Left
3| Right

For windowing queries, The following `n` lines will have the format like `[left] [top] [width] [height]`. This format means a rectangle with the left-upper point whose coordinate is `[left] [top]`, and the size of the rectangle is defined by `[width] [height]`. The whole input file will be like:

```
50 200
20.2643 44.3342 31.2076 1
89.114 24.9611 3.20695 3
79.2505 59.7186 2.24232 3
69.0023 15.8147 65.2666 2
65.6209 49.4888 13.9376 3
34.431 81.0175 59.8032 0
11.0172 20.6946 4.82756 2
......
4.58419 73.7123 21.6079 19.5633
65.9172 16.7354 22.0043 11.2834
14.6178 81.5513 69.2462 13.1753
65.8788 30.9769 29.0897 17.1584
4.33699 20.1715 27.7067 16.0607
34.7618 80.8893 61.8565 11.4205
70.5206 62.2587 14.0615 26.3661
16.4827 8.3636 16.603 13.0221
......
```
