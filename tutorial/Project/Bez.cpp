#include "Bez.h"

Bez::Bez() {
    float x = 0.5528;
	bez_points[0] = Eigen::Vector4f(-4, 0, 0, 0);
	bez_points[1] = Eigen::Vector4f(-1, x, 0, 0);
	bez_points[2] = Eigen::Vector4f(-x, 1, 0, 0);
	bez_points[3] = Eigen::Vector4f(0, 4, 0, 0);
	bez_points[4] = Eigen::Vector4f(x, 1, 0, 0);
	bez_points[5] = Eigen::Vector4f(1, -x, 0, 0);
	bez_points[6] = Eigen::Vector4f(-4, 0, 0, 0);
}

Eigen::Vector4f* Bez::get_points(int section) {
    return bez_points + section*3;
}

float Bez::bezier(float t, int section) {
    Eigen::Vector4f points[4] = get_points(section);
    float t_1 = t, t_2 = t*t, t_3 = t_1 * t_2;
    float tc_1 = (1 -t), tc_2 = (1 -t)*(1 - t), tc_3 = tc_1 * tc_2;

    return tc_3 * points[0] + 3 *tc_2 * t_1 * points[1] + 3* tc_1 * t_2 * points[2] + t_3 * points[3];
}