#include "stb_image_write.h"

#include "Camera.h"
#include "Material.h"

int main()
{
    auto materialGround = std::make_shared<Lambert>(Color(0.8, 0.8, 0.0));
    auto materialCenter = std::make_shared<Lambert>(Color(0.1, 0.2, 0.5));
    auto materialLeft = std::make_shared<Dielectric>(1.5);
    auto materialRight = std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.0);
    Scene scene;
    scene.Add(std::make_shared<Sphere>(Point3(0, -100.5, -1), 100, materialGround));
    scene.Add(std::make_shared<Sphere>(Point3(0, 0, -1), 0.5, materialCenter));
    scene.Add(std::make_shared<Sphere>(Point3(-1.0, 0, -1.0), 0.5, materialLeft));
    scene.Add(std::make_shared<Sphere>(Point3(-1.0, 0, -1.0), -0.4, materialLeft));
    scene.Add(std::make_shared<Sphere>(Point3(1.0, 0, -1.0), 0.5, materialRight));

    Camera camera;
    camera.Render("image.jpg", scene);

    return 0;
}
