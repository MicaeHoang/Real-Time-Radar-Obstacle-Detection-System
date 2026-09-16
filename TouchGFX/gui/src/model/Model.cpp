#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

extern "C" {
    extern volatile int g_servoAngle;
    extern volatile float g_sensorDistance;
}

Model::Model() : modelListener(0)
{

}

void Model::tick()
{
    // Đẩy thông số góc quay và khoảng cách sang Presenter đang kích hoạt
    if (modelListener != 0)
    {
        modelListener->updateRadarData(g_servoAngle, g_sensorDistance);
    }
}
