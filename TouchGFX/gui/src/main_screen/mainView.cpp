#include <gui/main_screen/mainView.hpp>
#include <cmath>
#include <touchgfx/Utils.hpp>

#define PI 3.1415926535f
#define CENTER_X 160
#define CENTER_Y 174
#define MAX_RADIUS 130    // Ban kinh toi da tren LCD
#define MAX_DISTANCE 100.0f // Khoang cach toi da cam bien do


mainView::mainView() : currentAngle(0), step(1)
{
    clearAllPoints();
}

void mainView::setupScreen()
{
    mainViewBase::setupScreen();

    // Thêm Canvas vẽ mảng điểm vào màn hình
    radarCanvas.setPosition(0, 0, 320, 240);
    add(radarCanvas);

    // Cap vung chua phu toan man hinh cho line servoAngle
    servoAngle.setPosition(0, 0, 320, 240);
    // Dat diem goc co dinh cho Line tai tam Radar
    servoAngle.setStart(CENTER_X, CENTER_Y);
    updateLinePosition(currentAngle);
}

void mainView::tearDownScreen()
{
    mainViewBase::tearDownScreen();
}

void mainView::handleTickEvent()
{
    // currentAngle += step;

    // if (currentAngle >=180)
    // {
    //     currentAngle = 180;
    //     step = -1;
    // }
    // else if (currentAngle <= 0)
    // {
    //     currentAngle = 0;
    //     step = 1;
    // }
    // updateLinePosition(currentAngle);
}

void mainView::updateLinePosition(int angle)
{
    float rad = angle * PI / 180.0f;

    int endX = CENTER_X - static_cast<int>(MAX_RADIUS * cosf(rad));
    int endY = CENTER_Y - static_cast<int>(MAX_RADIUS * sinf(rad));

    // touchgfx_printf("Angle = %d, X = %d, Y = %d\n", angle, endX, endY);

    servoAngle.invalidate();
    servoAngle.setEnd(endX, endY);
    servoAngle.invalidate();
}

// Hàm xoá sạch dữ liệu mảng khi hết 1 lượt quét 180 độ
void mainView::clearAllPoints()
{
    for (int i = 0; i <= 180; i++)
    {
        radarMap[i].active = false;
        radarMap[i].x = 0;
        radarMap[i].y = 0;
    }

    // Cập nhật lại canvas trống[cite: 1]
    radarCanvas.setPoints(radarMap);
    radarCanvas.invalidate();
}

void mainView::updateRadar(int angle, float distance)
{
    // Giới hạn góc nằm trong khoảng 0 - 180 độ
    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;

    // 1. KIỂM TRA ĐỔI CHIỀU QUÉT / ĐỦ 180 ĐỘ ĐỂ RESET BẢN ĐỒ
    // Nếu phát hiện vừa quét xong từ 180 vọt về 179 (quay lại) hoặc từ 0 vọt lên 1
    if ((lastAngle == 180 && angle < 180) || (lastAngle == 0 && angle > 0))
    {
        clearAllPoints(); // Xóa mảng dữ liệu điểm
        kalmanFilter.reset(distance);
        // redDot.invalidate();
        // redDot.setVisible(false); // Ẩn điểm hiện tại
    }
    lastAngle = angle; // Cập nhật góc vừa quét

    // 2. CẬP NHẬT KIM QUÉT SERVO
    updateLinePosition(angle);

    // 2. In con số khoảng cách lên Text Area
    // 3. TÍNH TOÁN VÀ LƯU TỌA ĐỘ VÀO MẢNG `radarMap`
    float filteredDistance = distance;
    if (distance > 0 && distance <= MAX_DISTANCE)
    {
        filteredDistance = kalmanFilter.update(distance);

        // Hiển thị khoảng cách lên Text Area
        int mainPart = static_cast<int>(distance);
        int decPart = static_cast<int>((distance - mainPart) * 10);
        
        textAreaDistance.invalidate();
        Unicode::snprintf(textAreaDistanceBuffer, TEXTAREADISTANCE_SIZE, "%d.%dcm", mainPart, decPart);
        textAreaDistance.invalidate();

        // Quy đổi ra tọa độ X, Y trên màn hình LCD
        float rad = angle * PI / 180.0f;
        float r = (filteredDistance / MAX_DISTANCE) * MAX_RADIUS;

        int x = CENTER_X - static_cast<int>(r * cosf(rad));
        int y = CENTER_Y - static_cast<int>(r * sinf(rad));

        // Ghi nhận điểm mới vào mảng dữ liệu tại vị trí index = angle
        radarMap[angle].x = x;
        radarMap[angle].y = y;
        radarMap[angle].active = true;

        // Cập nhật vị trí hiển thị cho widget `redDot` chính (vật cản mới nhất)
        redDot.invalidate();
        redDot.moveTo(x - (redDot.getWidth() / 2), y - (redDot.getHeight() / 2));
        redDot.setVisible(true);
        redDot.invalidate();
    }
    else
    {
        // Góc này không có vật cản -> Đánh dấu ngắt active trong mảng
        radarMap[angle].active = false;

        textAreaDistance.invalidate();
        Unicode::snprintf(textAreaDistanceBuffer, TEXTAREADISTANCE_SIZE, "---cm");
        textAreaDistance.invalidate();

        redDot.invalidate();
        redDot.setVisible(false);
    }

    // 4. VẼ LẠI TOÀN BỘ CÁC ĐIỂM TRONG MẢNG LÊN MÀN HÌNH
    radarCanvas.setPoints(radarMap);
    radarCanvas.invalidate(); // Báo TouchGFX vẽ lại canvas mảng điểm
}
