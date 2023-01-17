<i>note that some comments from the source code files are in Romanian language. Moreover, "v0" is the first version and it might not work/</i>
<br>
-besides the structure given in the request, I created two others,
namely one to store a pixel from the image, then one with the actual tree,
because I didn't know exactly how to go through it without seeing it like a list
1. read:
<br>-this function reads a ppm file. after declaring the image as a matrix of pixels,<br>
-followed by a series of buffers in which I will read the metadata.
<br>- open the input file; if he could not open it, close the program
<br>- read the header next
<br>- check the correctness of the data
<br>- after reading the header, I read the empty spaces and then move on to reading the binary image,
allocating memory along the way for its storage
2.compress:
<br>- here is the solution to the first requirement with image compression
-I make three sums, which, at the beginning, are all 0, one for each color,
and I divide these three sums by the size (squared) of the image, to calculate
the average color per color component, and that's because a pixel is a square,
and the square has an area; area is the product of two equal sides;
-then I calculate the average/score using the given formula
<br>- the middle colors are placed in the tree and the formula is checked with the similarity score
<br>- if the score is higher than the threshold, it is further divided
<br>- if it can no longer be divided, then it is a leaf, therefore all links are null
3. for:
<br>- I make a vector of connections so that I know exactly where to put the pixels
-how much time is left to travel in the compression shaft to make room for a new connection,
-put what's left of the tree there, say the next node is and move on to the next node
<br>- go through as required, (clockwise) recursively, until the leaves
4.createvector:
<br>- the compression shaft is traversed
-the i-th node in the tree receives the respective data
<br>- I put the colors to form what is compressed in the size of the block
<br>- if there is no leaf -> it is taken on a case by case basis for all 4 sections,
I put an order number on it, otherwise I mark it as a leaf
5.createtree:
<br>- allocate the compression tree and take the right color from the vector to put it in the tree,
including the size it is
<br>- if there is no leaf, better said as long as there is no leaf, I move on,
recursively, not necessarily in order
<br>- if the leaf has been reached, i.e. where there is -1 in the area/link,
then the links in the respective node in the tree are null, having nowhere else to go
6. decompress:
<br>- as long as it is not a leaf (it is marked with -1), reformat the image in binary,
in the matrix that holds the image
the forces move as much as the size of the image, (the area in the tree structure),
especially since usually x and y (from where it starts) are taken as zero, when calling (left, top)
<br>- it is taken by cases, by colors;
<br>- the compression vector is traversed recursively, recreating the image progressively, especially as
likewise, it is taken in quarters, in the order specified in the statement
-always halve and add, as the image is divided
in four, so the size of a line, as it were, would be half
6. write:
<br>- this function writes the new image in the out file
-write the elements of the image in order: the format (I already know it's P6); dimensions
(in general, I saw that it's a square image), so it doesn't matter which one
length or width take; the number of rgb pixels, usually 255
-I write the binary image line by line, and I don't forget to close the file at the end
7. vertically:
if it is not a leaf, or rather as long as it is not a leaf, because this
is the stopping condition for recursion
<br>- it is completed up to the leaves, in the order listed (like the hands of a clock),
a helper node is used which will be used to exchange between the nodes of the tree,
more precisely at the interchange of the upper left node with the lower left one and the lower right node with the one
right-up because left-right change is made,
being a vertical twist of the image, then the change is made, in turn, but se
recursively returns from each block.
8.horizontally:
<br>- the procedure is similar to horizontally, except that the nodes from
the upper-left blocks with the upper-right one and the lower-left blocks with the lower-right one
9.deallocate:
-free recursively to the blocks, starting from the bottom-left and going in reverse until everything is freed
10.main:
-if compression is desired: after I declare almost everything I need, I initialize the compression tree.
after reading the image and the compression factor, given as arguments, I use the compress function,
then I allocate the compression vector and put the data in it. then I release what I have allocated so far.
<br>- if decompression is desired:
I read the number of colors, the number of nodes, the color vector, initialize the tree and put the data in it.
I do all this with the createtree and decompress functions; release the vector;
I take the size of the image from the tree, allocate memory for the image and only now write the image.
<br>- if you want to mirror the image: after compressing the image, check what kind of
twist is desired and, depending on this, the horizontally and vertically functions are used.
I decompress and write the new image, and finally free what I have allocated. 
