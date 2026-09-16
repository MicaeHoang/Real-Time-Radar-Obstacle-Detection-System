#ifndef MAINVIEW_HPP
#define MAINVIEW_HPP

#include <gui_generated/main_screen/mainViewBase.hpp>
#include <gui/main_screen/mainPresenter.hpp>
#include <gui/main_screen/RadarCanvas.hpp>

// Lớp Lọc Kalman 1D
class SimpleKalmanFilter {
private:
    float q = 0.2f; // Process noise
    float r = 3.00f; // Measurement noise
    float x = 0.00f; // Estimated value
    float p = 1.00f; // Estimation error
    float k = 0.00f; // Kalman gain

public:
    void reset(float initValue) {
        x = initValue;
        p = 1.00f;
    }

    float update(float measurement) {
        p = p + q;
        k = p / (p + r);
        x = x + k * (measurement - x);
        p = (1.0f - k) * p;
        return x;
    }
};

class mainView : public mainViewBase
{
public:
    mainView();
    virtual ~mainView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    // Ham tu dong goi sau moi Frame man hinh
    virtual void handleTickEvent();

    // Hàm cập nhật màn hình
    void updateRadar(int angle, float distance);

    void updateLinePosition(int angle);
    void clearAllPoints();
protected:
    int currentAngle; // Goc quet hien tai (0 - 180)
    int step;         // Huong quet: +1 (tang) hoac -1 (giam)
    int lastAngle = -1;
    ObstaclePoint radarMap[181];
    RadarCanvas radarCanvas;
    SimpleKalmanFilter kalmanFilter;
};

#endif // MAINVIEW_HPP
