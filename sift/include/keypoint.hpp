#ifndef KEYPOINT_H
#define KEYPOINT_H

struct Point2f{
    float x = 0.0f;
    float y = 0.0f;
};

struct KeyPoint{
    float size = 0.0f;
    float angle = -1;
    float response = 0.0f;
    int octave = 0;
    int class_id = -1;

    KeyPoint(Point2f pt, float size, float angle, 
        float response, int octave, int class_id);
};

#endif