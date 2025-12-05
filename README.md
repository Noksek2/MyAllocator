# MyAllocator
simple arena allocator 

# MPL 2.0
I dont know license but i think it is cool


just kidding


## Implementation explain
- make arena memory
- if memory fulled, add next arena(if len < MAX_ARENA), or add in front of current arena. 
- manage arena list (head, current=tail)
- use arena_check(point) and rewind, if you want to back

bad english yeah
