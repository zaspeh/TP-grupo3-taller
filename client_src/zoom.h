#ifndef ZOOM_H_
#define ZOOM_H_

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 720;

const int WORLD_SCALE = 4;
const int WORLD_WIDTH = SCREEN_WIDTH * WORLD_SCALE;
const int WORLD_HEIGHT = SCREEN_HEIGHT * WORLD_SCALE;

class Zoom {
private:
    float currentZoom;
    float targetZoom;
    const float MAX_ZOOM_IN;
    const float MIN_ZOOM_OUT;
    const float ZOOM_SMOOTHNESS;
    const float MIN_DISTANCE;
    const float MAX_DISTANCE;

public:
    Zoom(float maxZoomIn = 2.0f, float minZoomOut = 0.5f, 
         float smoothness = 0.1f, float minDist = 150.0f, float maxDist = 800.0f)
        : currentZoom(1.0f)
        , targetZoom(1.0f)
        , MAX_ZOOM_IN(maxZoomIn)
        , MIN_ZOOM_OUT(minZoomOut)
        , ZOOM_SMOOTHNESS(smoothness)
        , MIN_DISTANCE(minDist)
        , MAX_DISTANCE(maxDist) {}

    void update(float maxDistance) {
        if (maxDistance < MIN_DISTANCE) {
            targetZoom = MAX_ZOOM_IN;
        } else if (maxDistance > MAX_DISTANCE) {
            targetZoom = MIN_ZOOM_OUT;
        } else {
            float t = (maxDistance - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE);
            targetZoom = MAX_ZOOM_IN + t * (MIN_ZOOM_OUT - MAX_ZOOM_IN);
        }

        currentZoom += (targetZoom - currentZoom) * ZOOM_SMOOTHNESS;
    }

    float getCurrentZoom() const { return currentZoom; }
    float getVisibleWidth() const { return SCREEN_WIDTH / currentZoom; }
    float getVisibleHeight() const { return SCREEN_HEIGHT / currentZoom; }
};

#endif
