
#include "common.h"
#include "marker.h"

int main(void) {
    
    initMarker();

	while (1) {
        loopMarker();
	}
    
	return 0; // never reached
}
