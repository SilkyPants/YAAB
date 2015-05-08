
#include "common.h"

#include "marker.h"

int main(void) {
    
    g_Marker.Init();
    
    while (1)
    {
        g_Marker.IdleLoop();
    }
    
	return 0; // never reached
}