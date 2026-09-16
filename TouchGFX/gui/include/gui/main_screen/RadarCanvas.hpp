#ifndef RADAR_CANVAS_HPP
#define RADAR_CANVAS_HPP

#include <touchgfx/widgets/Widget.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/Color.hpp>

struct ObstaclePoint {
    int x;
    int y;
    bool active;
};

class RadarCanvas : public touchgfx::Widget
{
public:
    RadarCanvas()
    {
        // Cho phép Widget nhận diện vùng vẽ
        setWidth(320);
        setHeight(240);
        
        for (int i = 0; i <= 180; i++) {
            points[i].active = false;
        }
    }

    void setPoints(const ObstaclePoint* map)
    {
        for (int i = 0; i <= 180; i++) {
            points[i] = map[i];
        }
    }

    // Hàm vẽ trực tiếp lên LCD Framebuffer
    virtual void draw(const touchgfx::Rect& invalidatedArea) const override
    {
        // Lấy màu đỏ (RGB565)
        touchgfx::colortype redColor = touchgfx::Color::getColorFromRGB(255, 0, 0);

        for (int i = 0; i <= 180; i++)
        {
            if (points[i].active)
            {
                // Vẽ chấm vuông 3x3 pixel tại tọa độ (x, y)
                touchgfx::Rect dotRect(points[i].x - 1, points[i].y - 1, 3, 3);
                touchgfx::Rect intersect = invalidatedArea & dotRect;

                if (!intersect.isEmpty())
                {
                    HAL::lcd().fillRect(intersect, redColor, 255);
                }
            }
        }
    }

    virtual touchgfx::Rect getSolidRect() const override
    {
        return touchgfx::Rect(0, 0, 0, 0); // Vùng trong suốt
    }

private:
    ObstaclePoint points[181];
};

#endif // RADAR_CANVAS_HPP