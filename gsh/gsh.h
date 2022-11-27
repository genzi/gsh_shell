#ifndef __GSH_H__
#define __GSH_H__


typedef enum {
    GshStatus_Error = -1,
    GshStatus_OK    = 0
} GshStatus_t;


GshStatus_t GshRun(void) ;

#endif  // end __GSH_H__
