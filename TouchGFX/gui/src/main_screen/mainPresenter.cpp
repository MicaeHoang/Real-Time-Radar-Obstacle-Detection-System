#include <gui/main_screen/mainView.hpp>
#include <gui/main_screen/mainPresenter.hpp>

mainPresenter::mainPresenter(mainView& v)
    : view(v)
{

}

void mainPresenter::activate()
{

}

void mainPresenter::deactivate()
{

}

void mainPresenter::updateRadarData(int angle, float distance)
{
    // Truyền dữ liệu trực tiếp sang cho View xử lý đồ họa
    view.updateRadar(angle, distance);
}

void mainPresenter::setServoAngle(int angle)
{
    view.updateLinePosition(angle);
}
