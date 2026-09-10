#ifndef KEYPOINT_H
#define KEYPOINT_H

namespace sift {
    struct Point2f {
        float x = 0.0f;
        float y = 0.0f;
    };

    struct KeyPoint {
        float size = 0.0f;
        float angle = -1;
        float response = 0.0f;
        int octave = 0;
        float level = 0; // Scale
        int class_id = -1;
        Point2f pt;

        KeyPoint();
        KeyPoint(Point2f pt, float size, float angle,
            float response, int octave, int class_id);
        KeyPoint(const KeyPoint& other) = default;
        KeyPoint& operator=(KeyPoint& other) {
            KeyPoint kp;
            kp.pt = other.pt;
            kp.size = other.size;
            kp.angle = other.angle;
            kp.response = other.response;
            kp.octave = other.octave;
            kp.level = other.level;
            kp.class_id = other.class_id;

            return kp;
        }
    };

}

inline sift::KeyPoint::KeyPoint()
{
}

inline sift::KeyPoint::KeyPoint(Point2f pt, float size, float angle, 
    float response, int octave, int class_id)
{
}

#endif