#ifndef KEYPOINT_H
#define KEYPOINT_H

namespace sift {
    struct Point2f {
        float x = 0.0f;
        float y = 0.0f;
    };

    struct KeyPoint {
        /** 
        * @brief Represents a feature keypoint detected by the SIFT algorithm. 
        * 
        * Stores the spatial location, characteristic scale, orientation, 
        * detection response (pixel value), and position of the keypoint within the SIFT 
        * scale-space pyramid. 
        * 
        * @var pt 
        * Keypoint location in image coordinates. 
        * 
        * @var size 
        * Characteristic scale or diameter of the keypoint. 
        * 
        * @var angle 
        * Dominant orientation of the keypoint in degrees. 
        * A value of -1 indicates that no orientation has been assigned. 
        * 
        * @var response 
        * Strength or significance of the detected keypoint. 
        * 
        * @var octave 
        * Octave of the SIFT pyramid in which the keypoint was detected. 
        * 
        * @var level 
        * Scale-space level within the octave at which the keypoint was 
        * detected. May contain a fractional value after keypoint 
        * localization and interpolation. 
        * 
        * @var class_id 
        * Optional identifier used to associate the keypoint with a particular class or object.
        */

        Point2f pt;
        float size = 0.0f;
        float angle = -1;
        float response = 0.0f;
        int octave = 0;
        float level = 0;
        int class_id = -1;
        
        KeyPoint() = default;
        KeyPoint::KeyPoint(Point2f pt, float size, float angle,
            float response, int octave, int class_id)
            : pt(pt),
            size(size),
            angle(angle),
            response(response),
            octave(octave),
            class_id(class_id)
        {
        }
        KeyPoint(const KeyPoint& other) = default;
        KeyPoint& operator=(const KeyPoint& other) = default;

    };

}


#endif