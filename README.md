# MyAllocator
simple arena allocator in C

## UNSTABLE (v0.1.0)

## License 
- **apache license 2.0**

## Implementation explain
- make arena memory
- if memory fulled, add next arena(if len < MAX_ARENA), or add in front of current arena. 
- manage arena list (head, current=tail)
- use arena_check(point) and rewind, if you want to back

## Functions
- allocator_new
- allocator_new_size
- arena_alloc
- arena_free
- arena_reset
- arena_rewind
- arena_check_new
