

# obviously things go ahead of this
Plist_Unit: plist_unit.c point.c plist.c
	c99 -Wall -pedantic -o Plist_Unit plist_unit.c point.c plist.c -lm
	
Closest_Pair: closest_pair.c point.c plist.c
	c99 -Wall -pedantic -o $@ $^ -lm
	
Test: test.c point.c plist.c
	c99 -Wall -pedantic -o $@ $^ -lm