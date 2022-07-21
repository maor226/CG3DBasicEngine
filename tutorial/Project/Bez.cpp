#include "Bez.h"
Bez::Bez() {
    float x = 0.5528;
	bez_points[0] = Eigen::Vector2f(-4, 0);
	bez_points[1] = Eigen::Vector2f(-1, x);
	bez_points[2] = Eigen::Vector2f(-x, 1);
	bez_points[3] = Eigen::Vector2f(0, 4);
	bez_points[4] = Eigen::Vector2f(x, 1);
	bez_points[5] = Eigen::Vector2f(1, -x);
	bez_points[6] = Eigen::Vector2f(-4, 0);
}

Eigen::Vector2f* Bez::get_points(int section) {
    return bez_points + section*3;
}

Eigen::Vector3d Bez::bezier(float t, int section) {
    Eigen::Vector2f points[4] = get_points(section);
    float t_1 = t, t_2 = t*t, t_3 = t_1 * t_2;
    float tc_1 = (1 -t), tc_2 = (1 -t)*(1 - t), tc_3 = tc_1 * tc_2;

    Eigen::Vector2f res = tc_3 * points[0] + 3 *tc_2 * t_1 * points[1] + 3* tc_1 * t_2 * points[2] + t_3 * points[3];
    return Eigen::Vector3d(res[0], res[1], 0);
}
