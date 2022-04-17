File: readme.txt
Author: YOUR NAME HERE
----------------------

1.
I design the header and payload system by the following in explict. I first set a size_t pointer points to the header, because I know for sure that the header size is a multiple of 8 so that I could dereference directly without cast to get the value of header each time. Then I use a struct which contains two size_t pointer, namely prev and next to point to the prev and next free block if current block is a free blok. Because in this way, I could access the prev and next free block clearly and easily. The way I search for free blocks is basicly in function find_free_blocks. I use a size_t pointer ffh to denote the address of first free block, then I iterate through all the free blocks using the double linked list. In this way(using the ffh), I could always has the access to the first free_block.
For implicit, my alloctor would have better performance if the requests format is like str1 = mymalloc(100), myrealloc(str1, 200), str2 = mymalloc(300), myrealloc(str2, 400)... Keep doing mymalloc and myrealloc and keep the size in ascending order. Because in realloc in implicit, I always choose to free the str and malloc it to a new place and never do in place realloc. So in this setting, the freed blocks can never be used again. But if the request format is like str1 = mymalloc(800), myrealloc(str1, 700), str2 = mymalloc(600), myrealloc(str2, 500)... Keep doing mymalloc and myrealloc and keep the size in ascending order. Then my implicit could behave better because the latter malloc could use the freed blocks before.
Originally, I write one line of code in implicit.c as "if ((needed == first_61_bits - HEADER_SIZE) || needed == first_61_bits) {" and the instruction count for this line under samples/pattern-repeat.script is 3500. Then I change it to "if (needed >= first_61_bits - HEADER_SIZE) {" The instruction count decrease to 2000.

2. Assume client would not free an freed pointer twice or malloc the same pointer that is not freed twice. If the client double free one pointer in using implicit allocator, and the two free are contiuous, the program will not crash. However, if the free are separate, it depends and there is posibility that it would free other used blocks later and cause the program to crash. If the client double malloc it would create another block, the pointer would points to that newly created one, and we would lose track of the previous address.

Tell us about your quarter in CS107!
-----------------------------------
This is indeed an awesome quarter! Love it.


