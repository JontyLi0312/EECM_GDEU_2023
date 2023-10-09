#ifndef __JY901S_H
#define __JY901S_H

typedef struct{
    float roll;
    float pitch;
    float yaw;
}jy901s_angleData;

void jy901s_init(void);
void jy901s_getData(jy901s_angleData *jy901s_angleData);
void jy901s_yawSet0(void);

#endif
