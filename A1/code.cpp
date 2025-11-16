#include <iostream>
#include <cmath>
#include <random>
#include <iomanip>
#include <vector>
#include <numbers>
#include <matplot/matplot.h>

class Point {
private:
    double _x;
    double _y;
public:
    Point() = default;
    Point(double x, double y) : _x(x), _y(y) {}
    Point(const Point& other) = default;
    Point& operator= (const Point& other) = default;
    double get_x() const { return _x; }
    double get_y() const { return _y; }
};
class Circle {
private:
    Point _center;
    double _r;
public:
    Circle() = default;
    Circle(double x, double y, double r) : _r(std::abs(r)) {
        Point point{ x, y };
        _center = point;
    }
    Circle(Point center, double r) : _center(center), _r(std::abs(r)) {};
    double get_x() const { return _center.get_x(); }
    double get_y() const { return _center.get_y(); }
    double get_r() const { return _r; }
    bool is_in_circle(const Point& point) const {
        return (pow(point.get_x() - _center.get_x(), 2) + pow(point.get_y() - _center.get_y(), 2) <= pow(_r, 2));
    }
};
bool is_in_three_circles(const Point& point, const Circle& circle_1, const Circle& circle_2, const Circle& circle_3) {
    return circle_1.is_in_circle(point) && circle_2.is_in_circle(point) && circle_3.is_in_circle(point);
}
enum class RectangleType { Wide, Narrow };
double calculate_area(const Circle& circle_1, const Circle& circle_2, const Circle& circle_3, const RectangleType& type, size_t n) {
    size_t count = 0;
    double start_x = type == RectangleType::Wide ? std::fmin(std::fmin(circle_1.get_x() - circle_1.get_r(), circle_2.get_x() - circle_2.get_r()), circle_3.get_x() - circle_3.get_r()) :
        std::fmax(std::fmax(circle_1.get_x() - circle_1.get_r(), circle_2.get_x() - circle_2.get_r()), circle_3.get_x() - circle_3.get_r());
    double end_x = type == RectangleType::Wide ? std::fmax(std::fmax(circle_1.get_x() + circle_1.get_r(), circle_2.get_x() + circle_2.get_r()), circle_3.get_x() + circle_3.get_r()) :
        std::fmin(std::fmin(circle_1.get_x() + circle_1.get_r(), circle_2.get_x() + circle_2.get_r()), circle_3.get_x() + circle_3.get_r());
    double start_y = type == RectangleType::Wide ? std::fmin(std::fmin(circle_1.get_y() - circle_1.get_r(), circle_2.get_y() - circle_2.get_r()), circle_3.get_y() - circle_3.get_r()) :
        std::fmax(std::fmax(circle_1.get_y() - circle_1.get_r(), circle_2.get_y() - circle_2.get_r()), circle_3.get_y() - circle_3.get_r());
    double end_y = type == RectangleType::Wide ? std::fmax(std::fmax(circle_1.get_y() + circle_1.get_r(), circle_2.get_y() + circle_2.get_r()), circle_3.get_y() + circle_3.get_r()) :
        std::fmin(std::fmin(circle_1.get_y() + circle_1.get_r(), circle_2.get_y() + circle_2.get_r()), circle_3.get_y() + circle_3.get_r());;
    std::random_device rand;
    std::mt19937 eng(rand());
    std::uniform_real_distribution<> rand_num_x(start_x, end_x);
    std::uniform_real_distribution<> rand_num_y(start_y, end_y);
    for (size_t i = 0; i < n; ++i) {
        Point point{ rand_num_x(eng), rand_num_y(eng) };
        if (is_in_three_circles(point, circle_1, circle_2, circle_3)) ++count;
    }
    return static_cast<double>(count) / static_cast<double>(n) * ((std::abs(end_x - start_x)) * (std::abs(end_y - start_y)));
}
int main() {
    Circle circle_1{ 1, 1, 1 };
    Circle circle_2{ 1.5, 2, sqrt(5) / 2 };
    Circle circle_3{ 2, 1.5, sqrt(5) / 2 };
    std::vector<double> narrow_results{};
    std::vector<double> wide_results{};
    std::vector<double> narrow_diff{};
    std::vector<double> wide_diff{};
    std::vector<size_t> nums{};
    double precise_area = 0.25 * std::numbers::pi + 1.25 * asin(0.8) - 1;
    std::ofstream file_wide("wide_data.txt");
    std::ofstream file_narrow("narrow_data.txt");
    file_wide << "step area\n";
    file_narrow << "step area\n";
    for (size_t i = 100; i < 100000; i += 500) {
        nums.push_back(i);
        double narrow = calculate_area(circle_1, circle_2, circle_3, RectangleType::Narrow, i);
        double wide = calculate_area(circle_1, circle_2, circle_3, RectangleType::Wide, i);
        narrow_results.push_back(narrow);
        wide_results.push_back(wide);
        narrow_diff.push_back(precise_area - narrow);
        wide_diff.push_back(precise_area - wide);
        file_wide << i << ' ' << wide << '\n';
        file_narrow << i << ' ' << narrow << '\n';
    }

    matplot::plot(nums, wide_results);
    matplot::line(100, precise_area, 99600, precise_area)->color("red");
    matplot::title("generated points (wide rectangular area)");
    matplot::sgtitle("Area of the figure depending on the number of");
    matplot::legend({ "Deviation", "Precise" });
    matplot::xlabel("Number of generated points");
    matplot::ylabel("Area of the figure");
    matplot::grid("on");
    matplot::save("graphic_wide_1.png");

    matplot::plot(nums, narrow_results);
    matplot::line(100, precise_area, 99600, precise_area)->color("red");
    matplot::title("generated points (narrow rectangular area)");
    matplot::sgtitle("Area of the figure depending on the number of");
    matplot::legend({ "Deviation", "Precise" });
    matplot::xlabel("Number of generated points");
    matplot::ylabel("Area of the figure");
    matplot::grid("on");
    matplot::save("graphic_narrow_1.png");

    matplot::plot(nums, wide_diff);
    matplot::line(100, 0, 99600, 0)->color("red");
    matplot::title("approximate value from an exact estimate (wide rectangular area)");
    matplot::sgtitle("Magnitude of the relative deviation of an");
    matplot::legend({ "Deviation", "Precise" });
    matplot::xlabel("Number of generated points");
    matplot::ylabel("Magnitude of the relative deviation");
    matplot::grid("on");
    matplot::save("graphic_wide_2.png");

    matplot::plot(nums, narrow_diff);
    matplot::line(100, 0, 99600, 0)->color("red");
    matplot::title("approximate value from an exact estimate (narrow rectangular area)");
    matplot::sgtitle("Magnitude of the relative deviation of an");
    matplot::legend({"Deviation", "Precise"});
    matplot::xlabel("Number of generated points");
    matplot::ylabel("Magnitude of the relative deviation");
    matplot::grid("on");
    matplot::save("graphic_narrow_2.png");

    return 0;
}
