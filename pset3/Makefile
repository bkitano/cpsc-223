

Plist_Unit: plist_unit.c point.c plist.c
	c99 -Wall -g3 -pedantic -o $@ $^ -lm
	
Closest_Pair: closest_pair.c point.c plist.c
	c99 -Wall -g3 -pedantic -o $@ $^ -lm
	
Test: test.c point.c plist.c closest_pair.c
	c99 -Wall -g3 -pedantic -o $@ $^ -lm