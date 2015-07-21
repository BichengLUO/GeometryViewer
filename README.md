Simple Polygon & Mesh Viewer
===============================

This is an MFC program for viewing simple polygon and its mesh. It's actually used for Computational Geometry class, but it is better if you use it as a handy tool for research or project.

Format Requirement
------------------
A simple polygon should be described as a set of points. Each point consists of two coordinates, x and y and every point occupies one line. The polygon file format should be like this:
> 116.118 99.6337

> 112.181 105.222

> 109.212 105.154

> 107.377 106.332

> 105.472 107.083

> 110.883 107.467

> 112.639 112.464

> 110.426 109.064

> ......

A mesh should be described as a set of triangles. Regardless of steiner points, every point in triangles can be named according to its index. Thus, the mesh file format should be like this:
> 19 22 18

> 20 21 22

> 23 18 22

> 23 17 18

> 25 17 23

> ......

> **Note:** The index of the point starts from 0 instead of 1
