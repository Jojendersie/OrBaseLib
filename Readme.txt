Description file of OrBaseLib Project

Author: Johannes Jendersie

Content:
1. Lizense
2. Project intention
3. List of functionalities

********************************************************************************
1. Lizense
Here is a quiete easy licensing as open source:
http://creativecommons.org/licenses/by/3.0/
If you use parts of this project, please let me know what the purpose of your
project. You can do this by a comment at https://github.com/Jojendersie/.
Futhermore you have to state this project as a source of your project.

********************************************************************************
2. Project intention
This is a mishmash of differnt (more or less) standard functionalities.
I do not like the muddle of includes I need for several other projects. So this
libary sum up all things I am using as native code. Mainly there are mathematics
and abstract data types like trees and lists.

********************************************************************************
3. List of functionalities
Everything with a * could be bugy. They were imported from an former project of
me and not tested with the new specifications.
Math:
	Cos, Sin, ... - some crazy algrorithms which seems to be faster than there std counter part
	Vector2, Vector3, Vector4 - 2D, 3D and 4D vector math - all you need for 3D game development
	Matrix - 4x4 matrix transformation matrizes for the 3D space (left handed like OpenGL)*
	Matrix2x3 - a small matrix simulating a 3x3 matrix where the third row is always (0,0,1); this can be used for 2D transformations (textures, 2D games)
ADT:
	AVLTree - balanced binary search tree (autobalancing)
	SplayTree - self organisationg search tree (statisticaly optimal)
	Trie - string matching and searching structur*
	Buffer - stack and/or queue in one class (multithreadsafe*)
	Heap - implementation of the fibonacci heap (fast priority search structur)
	Hash - A hashmap with different behaviors using strings and/or other arbitary keys mixed
	List - A very simple double linked list
Algorithm:
	Mersenne Twister - a very good pseudo random number generator
	Hash - CRC and other likely hash functions
	Adaptive huffman - lossless compression codec
	AVCoder - something like an LZ77 coder with adaptive huffman - not that fast in compression, but good rate and uncoding speed