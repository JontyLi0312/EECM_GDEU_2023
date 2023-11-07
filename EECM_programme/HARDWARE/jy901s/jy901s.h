#ifndef __JY901S_H
#define __JY901S_H

typedef struct
{
    float roll;  // 横滚角
    float pitch; // 俯仰角
    float yaw;   // 航向角
} jy901s_angleData;

void jy901s_init(void);
void jy901s_getData(jy901s_angleData *jy901s_angleData);
void jy901s_yawSet0(void);

#endif
